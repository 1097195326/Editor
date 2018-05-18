// Fill out your copyright notice in the Description page of Project Settings.

#include "GHttpRequest.h"
#include "MsgHttp.h"
#include "BaseMsgConst.h"
#include "MsgLocal.h"
#include "GTimerMgr.h"
#include "Serialization/JsonSerializer.h"



UGHttpRequest::UGHttpRequest(FString & _url, FString & _source, FString & _shopCode,
	FString & _os, FString & _version, FString & _model, FString & _deviceCode,
	FString & _appVersion, FString & _apiVersion, FString & _appType, FString & _channel)
{
	m_url = _url;

	m_source = _source;

	m_shopCode = _shopCode;

	m_os = _os;

	m_version = _version;

	m_model = _model;

	m_deviceCode = _deviceCode;
	 
	m_appVersion = _appVersion;

	m_apiVersion = _apiVersion;

	m_appType = _appType;

	m_channel = _channel;

	m_isRequesting = false;

	m_isStopApp = false;

	m_threadRun = true;

	// 打开发送的线程.
	//m_sThread = new std::thread(std::bind(&UGHttpRequest::SendMsgThread, this));
    timer_task_ptr task(new TimerTask(this, &UGHttpRequest::OnThreadUpdate, 0.0f, true));
    
    m_timerId = GTimerMgr::GetInstance()->RegisterTimer(task);
}

UGHttpRequest::~UGHttpRequest()
{

}

// 子线程调用此函数.
bool UGHttpRequest::OnThreadUpdate(float _t)
{
	if (!m_msgQueue.empty() && m_msgQueue.size() > 0 && !m_isRequesting)
	{
		// 取容器里的第一个值发送.
		m_msg_send_mutex.lock();

		auto iter = m_msgQueue.begin();

		if (iter != m_msgQueue.end())
		{
			msg_ptr  msg = *iter;

			SendMessage(msg);

			m_msgQueue.erase(iter);
		}
		m_msg_send_mutex.unlock();
	}
    return false;
}

void UGHttpRequest::PushMsg(msg_ptr _msg)
{
	m_msg_send_mutex.lock();
	m_msgQueue.push_back(_msg);
	m_msg_send_mutex.unlock();
}

void UGHttpRequest::SendMsgThread()
{
	while (true)
	{
		if (m_threadRun)
		{
			OnThreadUpdate(0.0f);

			// 60帧一次发送或者接收消息.
			std::this_thread::sleep_for(std::chrono::milliseconds(16));
		}
		else
		{
			if (IsHttpRequesting())
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(16));
			}
			else
			{
				break;
			}
		}
	}
}

bool UGHttpRequest::IsHttpRequesting()
{
	return m_isRequesting;
}

void UGHttpRequest::SaveCookie(FHttpResponsePtr& inResp) 
{
	FString cookie = inResp->GetHeader(TEXT("Set-Cookie"));

	m_cookieCache = cookie;
}

void UGHttpRequest::ClearCookie() 
{
	m_cookieCache = "";
}

void UGHttpRequest::SendMessage(msg_ptr _msg)
{
	m_isRequesting = true;

	m_msg = _msg;

	MsgHttp *mHttpMsg = _msg->ConvertMsg<MsgHttp>();

	FHttpModule::Get().SetHttpTimeout(60);

	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();

	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));

	FString & cookieticket = mHttpMsg->m_token;

	HttpRequest->SetHeader(TEXT("token"), cookieticket);

	HttpRequest->SetHeader(TEXT("source"), m_source);

	HttpRequest->SetHeader(TEXT("operator"), mHttpMsg->m_operator);

	HttpRequest->SetHeader(TEXT("shopCode"), m_shopCode);

	HttpRequest->SetHeader(TEXT("os"), m_os);

	HttpRequest->SetHeader(TEXT("version"),m_version);

	HttpRequest->SetHeader(TEXT("model"), m_model);

	HttpRequest->SetHeader(TEXT("deviceCode"), m_deviceCode);

	HttpRequest->SetHeader(TEXT("appVersion"), m_appVersion);

	HttpRequest->SetHeader(TEXT("apiVersion"), m_apiVersion);

	HttpRequest->SetHeader(TEXT("appType"), m_appType);

	HttpRequest->SetHeader(TEXT("channel"), m_channel);

	HttpRequest->SetHeader(TEXT("Cookie"), m_cookieCache);

	FString mHttpUrl;
    if(!mHttpMsg->m_httpUrl.IsEmpty() && !mHttpMsg->m_httpUrl.Equals(""))
    {
        mHttpUrl =mHttpMsg->m_httpUrl;
    }
	else if (!mHttpMsg->m_urlAppend.IsEmpty() && !mHttpMsg->m_urlAppend.Equals(""))
	{
		mHttpUrl = FString::Printf(TEXT("%s%s"), *m_url, *mHttpMsg->m_urlAppend);
	}
	else
	{
		mHttpUrl = FString::Printf(TEXT("%s%s"), *m_url, *mHttpMsg->m_uri);
	}

	HttpRequest->SetURL(mHttpUrl);

	HttpRequest->SetVerb(mHttpMsg->m_inIsReqest ? TEXT("GET") : TEXT("POST"));

	HttpRequest->SetContentAsString(mHttpMsg->m_httpContent);
	
	HttpRequest->OnProcessRequestComplete() = FHttpRequestCompleteDelegate::CreateRaw(this, &UGHttpRequest::OnMsgResponse);
	
	UE_LOG(GHttpRequest, Log, TEXT("url(%s),parameter:%s"), *mHttpUrl, *mHttpMsg->m_httpContent);

	HttpRequest->ProcessRequest();
    
    m_HttpRequest.Add(HttpRequest);
}

void UGHttpRequest::SetStopAppState()
{
	m_isStopApp = true;

	m_isRequesting = false;
}

void UGHttpRequest::GRelease()
{
	SetStopAppState();

    for(int32 i=0;i<m_HttpRequest.Num();i++)
    {
        TSharedRef<IHttpRequest> HttpRequest =m_HttpRequest[i];
        
        HttpRequest->CancelRequest();
        
        m_HttpRequest.RemoveAt(i);
    }
    
	m_threadRun = false;

	//m_sThread->join();

	auto iter = m_msgQueue.begin();

	while (iter != m_msgQueue.end())
	{
		iter = m_msgQueue.erase(iter);
	}

	m_msgQueue.clear();

	m_msg = nullptr;

	m_isRequesting = false;

	delete this;
}

// 消息异常处理.
void UGHttpRequest::OnMsgError(EnumMsgResponseErrorCode _errorCode,int32 _serverCode,const FString & _errorInfo)
{
	// 错误信息单独在广播一个消息.
	msg_ptr receiveErrorMsg(new MsgLocal(MessageTypeLocal, BaseMsgConst::SYSTEM_HTTP_RESPONSE_ERROR, _errorCode, _serverCode, _errorInfo,m_msg));

	// 放在队列里.
	MsgCenter::Instance()->AddReceiveMsg(receiveErrorMsg);
}

void UGHttpRequest::OnMsgResponse(FHttpRequestPtr inReq, FHttpResponsePtr inResp, bool inSuccess)
{
	if (m_isStopApp)
	{
		return;
	}
    
    for(int32 i=0;i<m_HttpRequest.Num();i++)
    {
        m_HttpRequest.RemoveAt(i);
    }

	if (!inSuccess && m_allRepeatCount > m_currentRepeatCount)
	{
		// 最多请求3次。3次还失败就发送通知.
        m_currentRepeatCount = m_currentRepeatCount+1;
        
        UE_LOG(GHttpRequest, Error, TEXT("http repeat count %d"), m_currentRepeatCount);
        
        SendMessage(m_msg);
	}
    else
    {
        m_currentRepeatCount = 0;
        
        if (inResp.IsValid())
        {
            SaveCookie(inResp);
        }
        
        msg_ptr mHttpmsg(new MsgHttp(MessageTypeHttpResponse,m_msg->MsgID()));
        
        mHttpmsg->ConvertMsg<MsgHttp>()->Decode(inReq, inResp, inSuccess);
        
        if (mHttpmsg->GetMsgErrorCode() != EnumMsgResponseErrorCode::MessageTrue)
        {
            OnMsgError(mHttpmsg->GetMsgErrorCode(), mHttpmsg->m_serverCode, mHttpmsg->GetMsgErrorInfo());
            
            UE_LOG(GHttpRequest, Error, TEXT("http error%s"), *mHttpmsg->GetMsgErrorInfo());
        }
        
        // 放在队列里.
        MsgCenter::Instance()->AddReceiveMsg(mHttpmsg);
        
        m_msg = nullptr;
        
        m_isRequesting = false;
    }
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "GHttpDownload.h"
#include "BaseMsgConst.h"
GHttpDownload::GHttpDownload()
{
	m_threadRun = true;
	// 打开发送的线程.
	m_sThread = new std::thread(std::bind(&GHttpDownload::SendMsgThread, this));
}

GHttpDownload::~GHttpDownload()
{
}

bool GHttpDownload::IsHttpRequesting()
{
	
	return m_isRequesting;
}

void GHttpDownload::GRelease()
{
	SetStopAppState();

	m_threadRun = false;

	m_sThread->join();

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

void GHttpDownload::SendMessage(msg_ptr _msg)
{
	m_isRequesting = true;

	m_msg = _msg;

	MsgHttp *mHttpMsg = _msg->ConvertMsg<MsgHttp>();

	FHttpModule::Get().SetHttpTimeout(60);

	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();

	//HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));

	FString mHttpUrl = mHttpMsg->m_urlAppend;
	
	HttpRequest->SetURL(mHttpUrl);

	HttpRequest->SetVerb(mHttpMsg->m_inIsReqest ? TEXT("GET") : TEXT("POST"));

	//HttpRequest->SetContentAsString(mHttpMsg->m_httpContent);

	HttpRequest->OnProcessRequestComplete() = FHttpRequestCompleteDelegate::CreateRaw(this, &GHttpDownload::OnMsgResponse);

	HttpRequest->ProcessRequest();
}

void GHttpDownload::SetStopAppState()
{
	m_isStopApp = true;

	m_isRequesting = false;
}

// 消息异常处理.
void GHttpDownload::OnMsgError(EnumMsgResponseErrorCode _errorCode,int32 _serverCode, const FString & _errorInfo)
{
	// 错误信息单独在广播一个消息.
	msg_ptr receiveErrorMsg(new MsgLocal(MessageTypeLocal, BaseMsgConst::SYSTEM_HTTP_RESPONSE_ERROR, _errorCode, _serverCode, _errorInfo, m_msg));

	// 放在队列里.
	MsgCenter::Instance()->AddReceiveMsg(receiveErrorMsg);
}

void GHttpDownload::SendMsgThread()
{
	while (true)
	{
		if (m_threadRun)
		{
			OnThreadUpdate();

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

void GHttpDownload::OnThreadUpdate()
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
}

void GHttpDownload::PushMsg(msg_ptr _msg)
{
	m_msg_send_mutex.lock();
	m_msgQueue.push_back(_msg);
	m_msg_send_mutex.unlock();
}

void GHttpDownload::OnMsgResponse(FHttpRequestPtr inReq, FHttpResponsePtr inResp, bool inSuccess)
{
	if (m_isStopApp)
	{
		return;
	}

	if (inSuccess)
	{
		if (inResp.IsValid())
		{
			/*const TArray<uint8> & mContent = inResp->GetContent();

			int32 mContentLenght = inResp->GetContentLength();

			Byte * mByte = new Byte(mContentLenght);

			memcpy(mByte, mContent.GetData(), mContentLenght);*/

			FString mContent = inResp->GetContentAsString();

			msg_ptr mHttpmsg(new MsgHttp(MessageTypeHttpResponse, m_msg->MsgID(), mContent, m_msg->ConvertMsg<MsgHttp>()->m_httpPenetrate));

			// 放在队列里.
			MsgCenter::Instance()->AddReceiveMsg(mHttpmsg);
		}
		else
		{
			OnMsgError(EnumMsgResponseErrorCode::MessageHttpResponseError,0, TEXT("网络连接异常"));
		}
	}
	else
	{
		OnMsgError(EnumMsgResponseErrorCode::MessageHttpResponseError,0, TEXT("网络连接异常"));
	}

	m_msg = nullptr;

	m_isRequesting = false;
}

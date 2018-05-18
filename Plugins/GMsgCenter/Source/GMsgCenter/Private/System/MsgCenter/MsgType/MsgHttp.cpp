// Fill out your copyright notice in the Description page of Project Settings.
#include "GMsgCenter.h"
#include "MsgHttp.h"
#include "Serialization/JsonSerializer.h"

MsgHttp::MsgHttp()
{
}


MsgHttp::MsgHttp(int32 _unChannelID,int32 _msgId)
{
	m_unChannelID = _unChannelID;

	m_unMsgID = _msgId;
}

MsgHttp::MsgHttp(int32 _unChannelID, int32 _unMsgID, const FString & _token, const FString & _operator, bool _inIsReqest)
{
	m_unChannelID = _unChannelID;

	m_unMsgID = _unMsgID;

	m_token = _token;

	m_operator = _operator;

	m_inIsReqest = _inIsReqest;
}

MsgHttp::MsgHttp(int32 _unChannelID, int32 _unMsgID, const FString & _urlAppend, void * _httpPenetrate, bool _inIsReqest)
{
	m_unChannelID = _unChannelID;

	m_unMsgID = _unMsgID;

	m_urlAppend = _urlAppend;

	m_httpPenetrate = _httpPenetrate;

	m_inIsReqest = _inIsReqest;
}

MsgHttp::MsgHttp(int32 _unChannelID, int32 _unMsgID, Byte* _content, int32 _size,void * _httpPenetrate)
{
	m_unChannelID = _unChannelID;

	m_unMsgID = _unMsgID;

	m_bytes = _content;

	m_httpPenetrate = _httpPenetrate;

	m_byteSize = _size;
}

MsgHttp::MsgHttp(int32 _unChannelID, int32 _unMsgID, FString & _content, void * _httpPenetrate)
{
	m_unChannelID = _unChannelID;

	m_unMsgID = _unMsgID;

	m_httpContent = _content;

	m_httpPenetrate = _httpPenetrate;
    
    m_inIsReqest = false;
}

MsgHttp::~MsgHttp()
{
	m_httpJsonObj = nullptr;
	UE_LOG(GMsgCenterLog, Error, TEXT("MsgHttp released~~~~~~~~~~~~~~~~~\n"));

	if (m_bytes != nullptr && m_byteSize > 0)
		delete m_bytes;
}

MsgHttp::MsgHttp(int32 _unChannelID, int32 _unMsgID,const FString & _token,const FString & _operator, bool _inIsReqest,TSharedPtr<FJsonObject> & _content, const FString & _urlAppend)
{
	m_unChannelID = _unChannelID;

	m_unMsgID = _unMsgID;

	m_token = _token;

	m_operator = _operator;

	m_urlAppend = _urlAppend;

	m_inIsReqest = _inIsReqest;

	TSharedRef<TJsonWriter<TCHAR>> t_writer = TJsonWriterFactory<>::Create(&m_httpContent);
	    
	FJsonSerializer::Serialize(_content.ToSharedRef(), t_writer);
	//Encode(_content, _pageIndex, _pageSize); 
}

MsgHttp::MsgHttp(int32 _unChannelID, int32 _unMsgID,const FString & _content,const FString & _httpUrl)
{
    m_unChannelID = _unChannelID;
    
    m_unMsgID = _unMsgID;
    
    m_httpUrl = _httpUrl;
    
    m_inIsReqest = false;
    m_httpContent = _content;
}


int32 MsgHttp::GetByteLenght()
{
	return m_byteSize;
}

void MsgHttp::Encode(TSharedPtr<FJsonObject> & _content, int32 _pageIndex, int32 _pageSize)
{
	//TSharedPtr<FJsonObject> t_dataObject = MakeShareable(new FJsonObject);

	//t_dataObject->SetNumberField("pageIndex", _pageIndex);

	//t_dataObject->SetNumberField("pageSize", _pageSize);

	//t_dataObject->SetObjectField("content", _content);

	//// json 转换成FString.
	//TSharedPtr<FJsonObject> t_jsonObject = MakeShareable(new FJsonObject);

	//t_jsonObject->SetNumberField("msgId", m_unMsgID);
	//  
	//t_jsonObject->SetObjectField("data", t_dataObject);

	//// json转成FString.
	//if (t_jsonObject.IsValid() && t_jsonObject->Values.Num() > 0)
	//{
	//	TSharedRef<TJsonWriter<TCHAR>> t_writer = TJsonWriterFactory<>::Create(&m_httpContent);
	//	    
	//	FJsonSerializer::Serialize(t_jsonObject.ToSharedRef(), t_writer);
	//}
}

void MsgHttp::Decode(FHttpRequestPtr & inReq, FHttpResponsePtr & inResp, bool inSuccess)
{
	if (inSuccess)
	{
		if (inResp.IsValid())
		{
			// 解析json获取msgid，并放入消息队列里.
			FString mResponseMsg = inResp->GetContentAsString(); 

			UE_LOG(GMsgCenterLog, Error, TEXT("~~~~~~~~~~~~~~~~~~%s"), *mResponseMsg);

			if (!mResponseMsg.IsEmpty())
			{
				TSharedPtr<FJsonObject> mJsonObject;

				TSharedRef<TJsonReader<>> t_reader = TJsonReaderFactory<>::Create(mResponseMsg);

				if (FJsonSerializer::Deserialize(t_reader, mJsonObject))
				{
					int32 code = mJsonObject->GetIntegerField(TEXT("code"));

					FString sMsg = mJsonObject->GetStringField(TEXT("msg"));

					//TSharedPtr<FJsonObject> mContent = mJsonObject->GetObjectField("data");

					m_httpJsonObj = mJsonObject;

					m_serverCode = code;

					m_serverMsg = sMsg;

					int32 mCode = m_serverCode;
					
					if (1000000 == mCode)
					{
						m_msgResponseEror = EnumMsgResponseErrorCode::MessageTrue;

						m_msgErrorInfo = sMsg;
					}
					else
					{
						m_msgResponseEror = EnumMsgResponseErrorCode::MessageHttpResponseContentError;

						m_msgErrorInfo = sMsg;
					}
				}
				else
				{
					m_msgErrorInfo = TEXT("json 反序列化失败.");

					m_msgResponseEror = EnumMsgResponseErrorCode::MessageHttpResponseContentError;
				}
			}
			else
			{
				m_msgErrorInfo = TEXT("http response 内容为空.");

				m_msgResponseEror = EnumMsgResponseErrorCode::MessageHttpResponseContentError;
			}
		}
		else
		{
			m_msgErrorInfo = TEXT("http response inResp内容不存在.");

			m_msgResponseEror = EnumMsgResponseErrorCode::MessageHttpResponseContentError;

		}
	}
	else
	{
		m_msgErrorInfo = TEXT("http 调用失败，time out 或者网络连接异常.");

		m_msgResponseEror = EnumMsgResponseErrorCode::MessageHttpResponseError;
	}
}

TSharedPtr<FJsonObject> MsgHttp::GetHttpContent()
{
	return m_httpJsonObj;
}


Byte* MsgHttp::GetByteContent()
{
	return m_bytes;
}

int32 MsgHttp::GetServerCode()
{
  	return m_serverCode;
}


FString MsgHttp::GetServerMsg()
{
	return m_serverMsg;
}

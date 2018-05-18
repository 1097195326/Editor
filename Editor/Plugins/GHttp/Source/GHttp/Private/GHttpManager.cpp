// Fill out your copyright notice in the Description page of Project Settings.

#include "GHttpManager.h"
#include "HaoshaHttpChannel.h"
#include "GIHttpRequest.h"
#include "GHttpDownload.h"
#include "PrototypeManager.h"
#include "PrototypeConst.h"


GHttpManager * GHttpManager::m_pInstance;



GHttpManager::GHttpManager()
{
}

GHttpManager::~GHttpManager()
{
}

GHttpManager * GHttpManager::GetInstance()
{
	if (m_pInstance == nullptr)  //判断是否第一次调用.
		m_pInstance = new GHttpManager();

	return m_pInstance;
}

void GHttpManager::CreateHaoshaHttp(FString & _url, FString & _source, FString & _shopCode,
	FString & _os, FString & _version, FString & _model, FString & _deviceCode,
	FString & _appVersion, FString & _apiVersion, FString & _appType, FString & _channel)
{
	// 先注册消息频道.
	MsgChannel * channel = new HaoshaHttpChannel((uint32)MessageTypeHttpRequest);
	MsgCenter::Instance()->AddChannel((uint32)MessageTypeHttpRequest, channel);


	channel = new LocalChannel((uint32)MessageTypeHttpResponse);
	MsgCenter::Instance()->AddChannel((uint32)MessageTypeHttpResponse, channel);

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

	m_mappingPrototype = PrototypeManager::GetInstance()->GetPrototypeByPID<HttpMappingPrototype>(PrototypeConst::P_HTTP_MAPPING_ID);


	GIHttpRequest * mHttpRequest = new UGHttpRequest(_url, _source, _shopCode, _os, _version, _model, _deviceCode, _appVersion, _apiVersion, _appType, _channel);

	FString mChannelName = TEXT("hosaHttp");

	m_mapHttpRequest.Add(mChannelName, mHttpRequest);
}


void GHttpManager::CreateDownloadHttp()
{
	GHttpDownload * mHttp = new GHttpDownload();

	FString mChannelName = TEXT("DownloadHttp");

	m_mapHttpRequest.Add(mChannelName, mHttp);
}

void GHttpManager::GRelease()
{
	for (TMap<FString, GIHttpRequest*>::TConstIterator iter = m_mapHttpRequest.CreateConstIterator(); iter; ++iter)
	{
		GIHttpRequest * mRequest = iter->Value;

		mRequest->GRelease();
	}

	m_mapHttpRequest.Empty();

	m_pInstance = nullptr;

	delete this;
}

// 根据channel来调用此函数.
void GHttpManager::PushHaoshMessage(msg_ptr _msg)
{ 
	MsgHttp * mHttpMsg = _msg->ConvertMsg<MsgHttp>();

	int32 mMsgId = _msg->MsgID();

	HttpMapping * mMapping = m_mappingPrototype->GetMappingByMsgID(mMsgId);

	if (nullptr == mMapping)
		return;

	mHttpMsg->m_uri = mMapping->m_httpUri;
	
	if (!m_mapHttpRequest.Contains(mMapping->m_channelName))
	{
		return;   
	}
	   
	GIHttpRequest * mRequest = m_mapHttpRequest[mMapping->m_channelName];
	  
	mRequest->PushMsg(_msg);
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GHttpRequest.h"
#include "GIHttpRequest.h"
#include "HttpMappingPrototype.h"

/**
 * 
 */


class GHTTP_API GHttpManager
{
private:

	// 单利.
	static GHttpManager * m_pInstance;

	HttpMappingPrototype *          m_mappingPrototype = nullptr;

	FString							m_cookieCache;

	FString							m_source;
	FString							m_shopCode;
	FString							m_os;
	FString							m_version;
	FString							m_model;
	FString							m_deviceCode;
	FString							m_appVersion;
	FString							m_apiVersion;
	FString							m_appType;
	FString							m_channel;

	// http对象管理.
	TMap<FString, GIHttpRequest*>   m_mapHttpRequest;



public:
	GHttpManager();
	~GHttpManager();

	// 单利.
	static GHttpManager * GetInstance();

	void CreateHaoshaHttp(FString & _url, FString & _source, FString & _shopCode,
		FString & _os, FString & _version, FString & _model, FString & _deviceCode,
		FString & _appVersion, FString & _apiVersion, FString & _appType, FString & _channel);

	void CreateDownloadHttp();

	void GRelease();

public:
	/**
	*   // 根据channel来调用此函数..
	*/
	void PushHaoshMessage(msg_ptr _msg);

};

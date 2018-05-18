// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpModule.h"
#include <thread>
#include <mutex>
#include <iostream>
#include <stdint.h>
#include <string>
#include "castle_std.h"
/**
 * 
 */
class GCORE_API GHttpRequestTool
{
//private:
//	
//	TSharedRef<IHttpRequest> m_HttpRequest;
//
//	bool  m_isCancel = false;
//
//	bool  m_isFinish = false;
//
//public:
//	
//	GHttpRequestTool();
//
//	~GHttpRequestTool();
//
//	std::function<void(FHttpRequestPtr inReq, FHttpResponsePtr inResp, bool inSuccess)> m_callbackFunc;
//
//	FString m_url;
//
//public:
//
//	template<typename T>
//	static GHttpRequestTool* DownloadImage(FString & _url, T* obj, void (T::*_func)(FHttpRequestPtr inReq, FHttpResponsePtr inResp, bool inSuccess))
//	{
//		GHttpRequestTool * mTool = new GHttpRequestTool();
//
//		mTool->m_callbackFunc = std::function<void(FHttpRequestPtr inReq, FHttpResponsePtr inResp, bool inSuccess)>(std::bind(_func, obj, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
//
//		mTool->m_url = _url;
//
//		mTool->Request();
//
//		return mTool;
//	}
//
//public:
//
//	void OnMsgResponse(FHttpRequestPtr inReq, FHttpResponsePtr inResp, bool inSuccess);
//
//	void Request();
//
//	void Cancel();
};

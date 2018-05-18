// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GIHttpRequest.h"


using namespace std;

/**
 * 
 */
class GHTTP_API UGHttpRequest:public GIHttpRequest
{
private:
	
	FString                         m_url;

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

    int32                           m_timerId=0;

	msg_ptr							m_msg;

	// 判断是否关闭app.
	bool							m_isStopApp = false;

	// 标记当前是否正在调用http.
	bool                            m_isRequesting = false;

	std::thread *	m_sThread = nullptr;

	bool            m_threadRun = false;

	// 发送msg队列.
	std::vector<msg_ptr>            m_msgQueue;

	// 发送msg消息包队列用的互斥锁.
	mutex                           m_msg_send_mutex;
    
    TArray<TSharedRef<IHttpRequest>> m_HttpRequest;
    
    // 自动重连次数.
    int32                           m_allRepeatCount=3;
    
    // 当前自动重连次数.
    int32                           m_currentRepeatCount=0;

public:
	
	UGHttpRequest(FString & _url, FString & _source, FString & _shopCode,
		FString & _os, FString & _version, FString & _model, FString & _deviceCode,
		FString & _appVersion, FString & _apiVersion, FString & _appType, FString & _channel);
	
	~UGHttpRequest();

public:

	// 获取当前状态.
	virtual bool IsHttpRequesting() override;
	 
	// 收到消息回调.
	void OnMsgResponse(FHttpRequestPtr inReq, FHttpResponsePtr inResp, bool inSuccess);

	void SaveCookie(FHttpResponsePtr& inResp);

	void ClearCookie();

	virtual void GRelease() override;

	// 消息异常处理.
	void OnMsgError(EnumMsgResponseErrorCode _errorCode, int32 _serverCode,const FString & _errorInfo);

	// 发送消息.
	virtual void SendMessage(msg_ptr _msg) override;
	    
	// 设置了关闭app的状态.
	virtual void SetStopAppState() override;

	// 线程里负责发送数据.
	void SendMsgThread();

	bool OnThreadUpdate(float _t);

	// 把消息放入消息队列里.
	virtual void PushMsg(msg_ptr _msg) override;
};

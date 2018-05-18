// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MsgCenter.h"
#include "GIHttpRequest.h"


using namespace std;
/**
 * http下载字符串用.
 */
class GHTTP_API GHttpDownload :public GIHttpRequest
{
private:
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

public:

	GHttpDownload();

	virtual ~GHttpDownload();

public:

	// 获取当前状态.
	virtual bool IsHttpRequesting() override;

	virtual void GRelease() override;

	// 发送消息.
	virtual void SendMessage(msg_ptr _msg) override;

	// 设置了关闭app的状态.
	virtual void SetStopAppState() override;

	// 线程里负责发送数据.
	void SendMsgThread();

	void OnThreadUpdate();

	// 把消息放入消息队列里.
	virtual void PushMsg(msg_ptr _msg) override;

	// 收到消息回调.
	void OnMsgResponse(FHttpRequestPtr inReq, FHttpResponsePtr inResp, bool inSuccess);

	void OnMsgError(EnumMsgResponseErrorCode _errorCode, int32 _serverCode, const FString & _errorInfo);
};

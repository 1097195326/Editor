// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MsgBase.h"
/**
 * 
 */
class GMSGCENTER_API MsgLocal:public MsgBase
{

private:
	// 消息体，上层程序员需要自行释放内存.
	content_ptr m_content = nullptr;

	FString  m_strContent;

public:

	msg_ptr m_presentMsg;

public:
	MsgLocal();
	virtual ~MsgLocal();

	// 普通构造函数.
	MsgLocal(int32 _unChannelID, int32 _unMsgID,content_ptr _content);

	// 消息异常时候发送的消息.
	MsgLocal(int32 _unChannelID, int32 _unMsgID, EnumMsgResponseErrorCode _errorCode,const FString & _strContent);

	// http消息异常判断是否重发时候用.
	MsgLocal(int32 _unChannelID, int32 _unMsgID, EnumMsgResponseErrorCode _errorCode,int32 _servercode, const FString & _strContent,msg_ptr _presentMsg);
public:
	//  获取内容. 
	content_ptr Content();
};

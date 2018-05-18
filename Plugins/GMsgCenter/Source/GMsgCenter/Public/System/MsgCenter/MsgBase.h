// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include <iostream>
#include <sstream>
#include "castle_std.h"
#include "Serialization/JsonSerializer.h"




/**
* 消息系统的消息体.
*
*  content自行管理内存.
*
*  bytes 必须是new出来的对象。并且由msgbase来释放.
*/

// 消息错误信息号.
enum EnumMsgResponseErrorCode :int32
{
	MessageTrue = 0,                // 消息正常.
	MessageFalse,                   // 消息异常.
	MessageHttpResponseTimeOut,     // http 延迟.
	MessageHttpResponseError,       // http 异常.   
	MessageHttpResponseContentError,// http 内容异常,反序列化失败等.
};

typedef void * content_ptr;

class GMSGCENTER_API MsgBase
{
protected:
    
    //  消息频道号.
    int32 m_unChannelID;

	// 用户ID，唯一标示.
	int32 m_playerID;
    
    // 消息号.
    int32 m_unMsgID;
    
	// 消息异常code.
	EnumMsgResponseErrorCode m_msgResponseEror = EnumMsgResponseErrorCode::MessageTrue;

	// 消息异常信息.
	FString m_msgErrorInfo;

public:
	// 浩沙http返回的code跟msg.
	int32 m_serverCode = 0;

public:

	MsgBase();

	virtual ~MsgBase();

	// 获取消息是否异常.
	EnumMsgResponseErrorCode GetMsgErrorCode();
    
    //  获取频道号.
    int32 ChannelID();
    
    //  获取消息号.
    int32 MsgID();
    
	FString GetMsgErrorInfo();

public:

	// byte转string.
	FString StringFromBinaryArray(Byte* _content, int32 _size);

	// 强转类型.
	template<typename T>
	//_1:msg体.
	T* ConvertMsg()
	{
		return (T*)this;
	}
    
};

typedef std::shared_ptr<MsgBase>		            msg_ptr;
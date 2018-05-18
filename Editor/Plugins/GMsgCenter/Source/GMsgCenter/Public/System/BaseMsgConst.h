// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 此类负责定义消息号区段的作用，真实的消息号别写在这个类里.
 */
class GMSGCENTER_API BaseMsgConst
{
public:
	// HTTP连接异常消息号.
	static const int32 SYSTEM_HTTP_RESPONSE_ERROR       = 0;

	// 1~99是固定系统消息.100以后是自定义消息.
	static const int32 LOCAL_MSG_BEGIN					= 100;

	static const int32 LOCAL_MSG_END					= 999;

	// HTTP服务器消息是1000 ~ 9999.
	static const int32 S_HTTP_MSG_BEGIN					= 1000;
	static const int32 S_HTTP_MSG_END					= 9999;

	// socket 消息暂时不用.
};

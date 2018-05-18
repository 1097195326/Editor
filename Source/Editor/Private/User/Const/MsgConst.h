// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseMsgConst.h"

/**
*  此类生命消息号.
 */
class MsgConst:public BaseMsgConst
{
public:

	
    // ======================== 服务器通讯的消息id 区. ======================== .
    // 登录网关.
    static const int32 S_LOGIN                              = S_HTTP_MSG_BEGIN+1;
	static const int32 S_FAST_LOGIN							= S_HTTP_MSG_BEGIN + 2;//快速登录.
	static const int32 S_CHECK_NUMBER                        = S_HTTP_MSG_BEGIN + 4;  //验证码.
	static const int32 S_PASSWORD_CHECK_NUMBER						= S_HTTP_MSG_BEGIN + 5;  //重置密码验证码.
	static const int32 S_FORCED_UPDATE						= 3001;  //强制更新.
    //========================       内部消息.       ========================.

	// 1~99是固定系统消息.100以后是自定义消息.
	// 刷新当前UI.
	static const int32 LC_REF_UI							= LOCAL_MSG_BEGIN +1;

};
















































































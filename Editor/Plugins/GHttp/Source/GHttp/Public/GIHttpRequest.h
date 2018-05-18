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
#include "MsgCenter.h"


DECLARE_LOG_CATEGORY_EXTERN(GHttpRequest, Log, All);
/**
 * 
 */
class GHTTP_API GIHttpRequest
{
public:
	GIHttpRequest();
	virtual ~GIHttpRequest();

public:

	// 获取当前状态.
	virtual bool IsHttpRequesting() = 0;
	
	virtual void GRelease() = 0;
	
	// 发送消息.
	virtual void SendMessage(msg_ptr _msg) = 0;

	// 设置了关闭app的状态.
	virtual void SetStopAppState() = 0;
	
	// 把消息放入消息队列里.
	virtual void PushMsg(msg_ptr _msg) = 0;
};

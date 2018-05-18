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

	// ��ȡ��ǰ״̬.
	virtual bool IsHttpRequesting() = 0;
	
	virtual void GRelease() = 0;
	
	// ������Ϣ.
	virtual void SendMessage(msg_ptr _msg) = 0;

	// �����˹ر�app��״̬.
	virtual void SetStopAppState() = 0;
	
	// ����Ϣ������Ϣ������.
	virtual void PushMsg(msg_ptr _msg) = 0;
};

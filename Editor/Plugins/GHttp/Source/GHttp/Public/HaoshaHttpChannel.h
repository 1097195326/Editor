// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MsgChannel.h"

/**
 * 
 */
class GHTTP_API HaoshaHttpChannel : public MsgChannel
{
public:
	HaoshaHttpChannel(uint32 _channelID);
	virtual ~HaoshaHttpChannel();

public:
	virtual void SendMsg(msg_ptr _msg) override;

};

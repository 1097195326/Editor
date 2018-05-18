// Fill out your copyright notice in the Description page of Project Settings.

#include "HaoshaHttpChannel.h"
#include "GHttpManager.h"

HaoshaHttpChannel::HaoshaHttpChannel(uint32 _channelID) : MsgChannel(_channelID)
{
}

HaoshaHttpChannel::~HaoshaHttpChannel()
{
}


void HaoshaHttpChannel::SendMsg(msg_ptr _msg)
{
	//  直接发送到socket类.
	GHttpManager::GetInstance()->PushHaoshMessage(_msg);
}

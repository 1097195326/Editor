                                                                                                                                                                                                                                                                                                                                                   // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MsgChannel.h"

/**
 * 
 */
class GMSGCENTER_API LocalChannel: public MsgChannel
{
public:
	LocalChannel(uint32 _channelID);
	~LocalChannel();
    
    virtual void SendMsg(msg_ptr msg) override;
};

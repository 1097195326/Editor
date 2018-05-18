// Fill out your copyright notice in the Description page of Project Settings.

#include "GMsgCenter.h"
#include "LocalChannel.h"

LocalChannel::LocalChannel(uint32 _channelID): MsgChannel(_channelID)
{
}

LocalChannel::~LocalChannel()
{
}


void LocalChannel::SendMsg(msg_ptr msg)
{
    uint32 mMsgID = msg->MsgID();
    
    XY_IF_FOUND_IN_MAP(mMsgID,it,map_handlers,MsgFuncMap)
    {
        MsgFuncVector* mVector = it->second;
        
		for (int32 i= mVector->size()-1;i>=0;i--)
		{
			msg_function mHandler = (*mVector)[i];

			mHandler(msg);
		}

   //     // 遍历vector 发送 . 
   //     auto iter = mVector->begin();
   //         
   //     while (iter != mVector->end())
   //     {
   //         msg_function mHandler = *iter;
   //         
			//++iter;

   //         mHandler(msg);
   //     }
    }
}

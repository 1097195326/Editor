// Fill out your copyright notice in the Description page of Project Settings.

#include "GMsgCenter.h"
#include "MsgBase.h"


MsgBase::MsgBase()
{

}

// byte转string.
FString MsgBase::StringFromBinaryArray(Byte* _content,int32 _size)
{
	const std::string cstr(reinterpret_cast<const char*>(_content), _size);

	FString str(UTF8_TO_TCHAR(cstr.c_str()));

	return str;
}



MsgBase::~MsgBase()
{
    
    
	//UE_LOG(GMsgCenterLog, Error, TEXT("MsgBase released~~~~~~~~~~~~~~~~~\n"));
}


EnumMsgResponseErrorCode MsgBase::GetMsgErrorCode()
{
	return m_msgResponseEror;
}

//  获取频道号.
int32 MsgBase::ChannelID()
{
    return m_unChannelID;
}

//  获取消息号 .
int32 MsgBase::MsgID()
{
    return m_unMsgID;
}


FString MsgBase::GetMsgErrorInfo()
{
	return m_msgErrorInfo;
}

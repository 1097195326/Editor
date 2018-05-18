// Fill out your copyright notice in the Description page of Project Settings.
#include "GMsgCenter.h"
#include "MsgLocal.h"

MsgLocal::MsgLocal()
{
}


MsgLocal::MsgLocal(int32 _unChannelID, int32 _unMsgID, content_ptr _content)
{
	// 构造函数.
	m_unChannelID = _unChannelID;

	m_unMsgID = _unMsgID;

	m_content = _content;

}

MsgLocal::MsgLocal(int32 _unChannelID, int32 _unMsgID, EnumMsgResponseErrorCode _errorCode, const FString & _strContent)
{
	// 构造函数.
	m_unChannelID = _unChannelID;

	m_unMsgID = _unMsgID;

	m_strContent = _strContent;

	m_msgResponseEror = _errorCode;
}

MsgLocal::MsgLocal(int32 _unChannelID, int32 _unMsgID, EnumMsgResponseErrorCode _errorCode, int32 _servercode, const FString & _strContent, msg_ptr _presentMsg)
{
	// 构造函数.
	m_unChannelID = _unChannelID;

	m_serverCode = _servercode;

	m_unMsgID = _unMsgID;

	m_strContent = _strContent;

	m_msgResponseEror = _errorCode;

	m_presentMsg = _presentMsg;
}

MsgLocal::~MsgLocal()
{
	//UE_LOG(GMsgCenterLog, Error, TEXT("MsgLocal released~~~~~~~~~~~~~~~~~\n"));
}

//  获取内容. 
content_ptr MsgLocal::Content()
{
	return m_content;
}
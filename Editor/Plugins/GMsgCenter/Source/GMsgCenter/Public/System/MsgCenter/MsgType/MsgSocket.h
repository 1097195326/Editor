// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MsgBase.h"

/**
 * 
 */
class GMSGCENTER_API MsgSocket:public MsgBase
{

private:
	// socket 消息内容FString格式的.
	FString m_fromServerContent;

	// 发送网络消息时候堆内存，接受消息时候是栈内存,上层程序员不需要关心这个变量内存释放的问题.
	Byte * m_bytes = nullptr;

	int32 m_byteLength = 0;

public:
	Byte * GetBytes();

	const int32 GetByteLength();

	const int32 GetMessageLength();

	// 序列化.
	void Encode(Byte * _buffse, int32 & _size);

	// 反序列化.
	void Decode(Byte * _bytes, int32 _byteLength);

public:
	MsgSocket();
	virtual ~MsgSocket();

	// 接受网络消息构造函数.
	MsgSocket(int32 _unChannelID, Byte * _bytes, int32 _byteLength);

	// 发送网络消息构造函数.
	MsgSocket(int32 _unChannelID, int32 _unMsgID, Byte * _bytes, int32 _byteLength);

	// 发送网络消息构造函数.
	MsgSocket(int32 _unChannelID, int32 _unMsgID, FString * _str);
};

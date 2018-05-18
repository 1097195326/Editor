// Fill out your copyright notice in the Description page of Project Settings.
#include "GMsgCenter.h"
#include "MsgSocket.h"

MsgSocket::MsgSocket()
{
}

MsgSocket::~MsgSocket()
{
	if (m_bytes != nullptr && m_byteLength > 0)
		delete m_bytes;
}

// 发送网络消息构造函数.
MsgSocket::MsgSocket(int32 _unChannelID, int32 _unMsgID, Byte * _bytes, int32 _byteLength)
{
	// 构造函数.
	m_unChannelID = _unChannelID;

	m_unMsgID = _unMsgID;

	m_bytes = _bytes;

	m_byteLength = _byteLength;
}

Byte * MsgSocket::GetBytes()
{
	return m_bytes;
}



// 接受网络消息构造函数相当于Decode.
MsgSocket::MsgSocket(int32 _unChannelID, Byte * _bytes, int32 _byteLength)
{
	m_unChannelID = _unChannelID;

	Decode(_bytes, _byteLength);
}

// 发送网络消息构造函数.
MsgSocket::MsgSocket(int32 _unChannelID, int32 _unMsgID, FString * _str)
{
	// 构造函数.
	m_unChannelID = _unChannelID;

	m_unMsgID = _unMsgID;

	TCHAR *seriallizedChar = _str->GetCharArray().GetData();

	Byte* mContentByte = (Byte*)TCHAR_TO_UTF8(seriallizedChar);

	int32 size = FCString::Strlen(seriallizedChar) + 1;

	m_byteLength = size;

	m_bytes = new Byte[size];

	memcpy(m_bytes, mContentByte, size);
}

/// <gx>.
/// 对消息进行网络发送前的编码.
/// 消息头共12字节.
/// 4 字节总长度 | 4 字节codeID | 4 字节playerid .
/// </gx>.
void MsgSocket::Encode(Byte * _buffse, int32 & _size)
{
	Byte * body = (Byte*)m_bytes;

	int32 bodySize = m_byteLength;

	int32 packageSize = 4 + bodySize;

	memcpy(&_buffse[_size], &bodySize, sizeof(int));

	memcpy(&_buffse[_size + 4], &m_unMsgID, sizeof(int));

	if (m_byteLength > 0)
		memcpy(&_buffse[_size + 8], body, m_byteLength);

	_size = _size + GetMessageLength();
}

const int32 MsgSocket::GetMessageLength()
{
	return m_byteLength + 8;
}

const int32 MsgSocket::GetByteLength()
{
	return m_byteLength;
}

void MsgSocket::Decode(Byte * _bytes, int32 _byteLength)
{
	// 偏移.
	int32 _offset = 0;

	m_unMsgID = (int)desToByte(_bytes, _offset);

	m_byteLength = _byteLength - 4 < 0 ? 0 : _byteLength - 4;

	if (m_byteLength > 0)
	{
		m_bytes = new Byte[m_byteLength];

		memcpy(m_bytes, &_bytes[4], m_byteLength);

		m_fromServerContent = StringFromBinaryArray(m_bytes, m_byteLength);
	}
}
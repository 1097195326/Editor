// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "MsgBase.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpModule.h"

/**
 * 浩沙HTTP 消息处理.
 */
class GMSGCENTER_API MsgHttp:public MsgBase
{

public:

	FString m_uri;
    
    FString m_httpUrl = TEXT("");

	FString m_httpContent;

	FString m_operator;

	FString m_token;

	FString m_urlAppend = TEXT("");

	bool	m_inIsReqest;

	FString m_serverMsg;

	// 消息体，http内容jsonObject格式.
	TSharedPtr<FJsonObject> m_httpJsonObj;

	Byte * m_bytes = nullptr;

	int32 m_byteSize = 0;

	void * m_httpPenetrate = nullptr;

public:
	// 浩沙http返回的code跟msg.
	int32 GetServerCode();

	FString GetServerMsg();

	// 获取http返回的json对象.
	TSharedPtr<FJsonObject> GetHttpContent();

	Byte* GetByteContent();

	int32 GetByteLenght();

	// 序列化.
	void Encode(TSharedPtr<FJsonObject> & _content,int32 _pageIndex,int32 _pageSize);

	// 反序列化.
	void Decode(FHttpRequestPtr & inReq, FHttpResponsePtr & inResp, bool inSuccess);

public:
	MsgHttp();

	virtual ~MsgHttp();

	MsgHttp(int32 _unChannelID,int32 _msgId);

	// 发送http构造函数.
	MsgHttp(int32 _unChannelID, int32 _unMsgID,const FString & _token,const FString & _operator,bool _inIsReqest, TSharedPtr<FJsonObject> & _content,const FString & _urlAppend=TEXT(""));

    // 发送http构造函数.
    MsgHttp(int32 _unChannelID, int32 _unMsgID, const FString & _httpContent,const FString & _httpUrl);
    
	// 不带任何内容发送http.
	MsgHttp(int32 _unChannelID, int32 _unMsgID, const FString & _token, const FString & _operator, bool _inIsReqest);

	// 发送指定的URl下载方法.
	MsgHttp(int32 _unChannelID, int32 _unMsgID, const FString & _urlAppend, void * _httpPenetrate, bool _inIsReqest);

	// http下载的回调.
	MsgHttp(int32 _unChannelID, int32 _unMsgID, Byte* _content,int32 _size, void * _httpPenetrate);

	// http 下载string回调.
	MsgHttp(int32 _unChannelID, int32 _unMsgID, FString & _content, void * _httpPenetrate);
};

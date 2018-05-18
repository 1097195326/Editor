#pragma once

#include "CoreMinimal.h"
#include "BaseT.h"
/**
*
*/
class EDITOR_API HomeInfoT :public BaseT
{
public:

	FString m_modelContentName;// : "软件设计工具-首页-轮播2",
	FString	m_appConnectUrl;
	int32	m_modelId;// : 1006,
	int32	m_createUserCode;// " : "1000",
	FString m_updateUserName;// : "sysadmin",
	FString	m_contentId;// " : 0,
	FString	m_contentText;// " : "SOFT_SERIES_2",
	FString m_modelContentCode;// " : "ITEMS - 20D81B6A60BC43FBBF156AAC9B60C1C7",
	FString	m_createUserName;//" : "sysadmin",
	int32	m_updateTime;// " : 1520501148000,
	int32	m_modelContentId;// " : 1021,
	FString  m_type;// " : "cat",
	FString m_contentImg;// " : "http://fafa-static.oss-cn-beijing.aliyuncs.com/images/cms/2018/3/8/17/658f586c-b0a7-4ace-86c8-7ee9e64bfcae.jpg",
	FString m_contentUrl;// " : "",
	int32 m_updateUserCode;// " : "1000",
	int32 m_createTime;// " : 1519807307000,
	int32 m_sorder;// " : 0

public:

	HomeInfoT();

	virtual~HomeInfoT();

public:
	FString GetName();
	virtual void OnLoad() override;

	virtual void OnRelease() override;

protected:
	virtual void OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj);

};
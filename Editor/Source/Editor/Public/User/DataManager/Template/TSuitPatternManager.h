// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TSuitPatternManager.h"
#include "GBaseTemplateManager.h"
#include "MsgCenter.h"
#include "SuitPatternT.h"

struct LoadPatternStruct
{
	int32 m_pageIndex = 0;
	int32 m_pageSize = 0;
	int32 m_recordCount = 0;
	int32 m_totalPage = 0;

	TArray<SuitPatternT*> m_patternArray;
};

/**
 * 
 */
class EDITOR_API TSuitPatternManager :public GBaseTemplateManager
{
public:
	const static int32 m_downloadPatternEvent = 4025;
private:
	// 单利. 
	static TSuitPatternManager *  m_pInstance;

	// 每页请求的数量.
	int32 m_getPatternCount = 16;

	// 存储花型.
	TArray<SuitPatternT*> m_patternArray;

	// 花型比较另类，id是字符串.
	TMap<FString, SuitPatternT*> m_patternMgr;

public:
	static TSuitPatternManager * GetInstance();

public:
	TSuitPatternManager();
	
	virtual ~TSuitPatternManager();

public:

	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void EncodeData(TSharedPtr<FJsonObject> & _content) override;

private:
	void AddPatternData(const FString & _key,SuitPatternT * _suitPatternT);

	void OnLoadPatternFinish(msg_ptr _msg);

public:
	SuitPatternT * GetPatternData(const FString & _key);

public:

	// 花型分页获取.
	TArray<SuitPatternT*> GetPatternArray(int32 _pageIndex);

	void GetPatternByIndex(int32 _pageIndex,int32 _pageSize,TArray<FString> & _patternIds,int32 _fabricId=0,const FString & _sn = TEXT(""),const FString & _ownerType = TEXT(""));
	
	// 从款详情数据里获取花型数据.
	void EncodePatternFromStyleDetailData(TSharedPtr<FJsonObject> & _content);

	// 检查花型是否可以用当前面料时候返回的花型数据.
	void EncodePatternFromCheckInFabric(TSharedPtr<FJsonObject> & _content);
};

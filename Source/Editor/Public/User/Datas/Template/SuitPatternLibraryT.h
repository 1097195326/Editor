// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseT.h"
#include "MsgCenter.h"

enum EnumPatternExtern
{
	CAN_EXTERN_PATTERN=1, //可以继承花型.
	CANT_NOT_EXTERN_PATTERN, // 不可以继承花型.
};

/**
 * 花型库.
 */
class EDITOR_API SuitPatternLibraryT :public BaseT
{
private:

	// 单利. 
	static SuitPatternLibraryT *   m_pInstance;

	// 二位矩阵判断是否继承花型.
	TMap<int32, TMap<FString, FString>> m_checkPatternData;

	int32 m_fabricId = 0;

	FString m_patternId;

public:
	SuitPatternLibraryT();

	virtual ~SuitPatternLibraryT();

public:

	virtual void OnLoad() override;

	virtual void OnRelease() override;

	static SuitPatternLibraryT * GetInstance();

public:
	// 请求服务器验证是否继承.
	void RequestPatternUseInFabric(int32 _fabricId, const FString & _patternId);

	// 根据本地数据来验证并返回.
	FString GetPatternId(int32 _fabricId,const FString & _patternId);

private:
	void OnCheckPatternExtern(msg_ptr _msg);


	void SendLocalMsg(EnumPatternExtern _msgCode);

	void AddPatternData(int32 _fabricId, const FString & _patternId);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseC.h"

struct ChildPartStruct
{
public:
	int32 m_stylePartId = 0;

	int32 m_suitPartId = 0;
};

/**
 * 
 */
class EDITOR_API StyleC:public BaseC
{

public:

	// icon.
	FString m_iconUrl;

	// 描述.
	FString m_commont;

	// 是否推荐款.
	int32 m_isRecommend = 0;

	// R部位ID数组.
	TArray<ChildPartStruct*> m_PartIdArray;

public:
	StyleC();

	virtual ~StyleC();

protected:
	// 子类可以重写.
	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void OnInitData(int32 _cid, int32 _rid, int32 _tid) override;
public:
	virtual void AddJsonC(TSharedPtr<FJsonObject> & _jsonObj) override;
};

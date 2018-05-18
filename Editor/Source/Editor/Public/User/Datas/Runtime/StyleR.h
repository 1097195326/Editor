// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseR.h"

/**
 * 
 */
class EDITOR_API StyleR:public BaseR
{
public:

	// icon.
	FString m_iconUrl;

	// 描述.
	FString m_commont;

	// 是否推荐款.
	int32 m_isRecommend = 1;

	// R部位ID数组.
	TArray<int32> m_stylePartIdArray;

public:
	
	StyleR();

	virtual ~StyleR();

public:

	virtual void OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj) override;

	virtual void OnLoad() override;

	virtual void OnRelease() override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseR.h"
/**
 * 
 */
class EDITOR_API StyleComponentR :public BaseR
{
public:
	TArray<int32> m_styleAccessoryIds;

	TArray<int32> m_styleSliceIds;

	TArray<int32> m_styleStitchsuitIds;


public:
	
	StyleComponentR();

	virtual ~StyleComponentR();

public:

	virtual void OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj) override;

	virtual void OnLoad() override;

	virtual void OnRelease() override;
};

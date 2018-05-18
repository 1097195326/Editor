// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseT.h"
/**
 * 
 */
class EDITOR_API SuitFabricColorLibraryT:public BaseT
{

public:

	FString m_iconUrl;

	// 花色Id数组.
	TArray<int32> m_fabricColorIds;

public:
	
	SuitFabricColorLibraryT();

	virtual ~SuitFabricColorLibraryT();

public:

	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj) override;
};

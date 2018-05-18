// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseT.h"
/*
 * 
 */
class EDITOR_API SuitFabricLibraryT:public BaseT
{
public:
	SuitFabricLibraryT();
	~SuitFabricLibraryT();
	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj) override;

public:
	FString m_IconUrl;
	TArray<int32> m_Fabrics;
};

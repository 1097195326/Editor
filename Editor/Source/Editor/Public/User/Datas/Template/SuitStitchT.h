// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseT.h"
/**
 * 
 */
class EDITOR_API SuitStitchT:public BaseT
{
public:
	SuitStitchT();
	~SuitStitchT();
	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj) override;

public:
	int32 m_Status;
	FString m_IconUrl;
	int32 m_IsMainPart;
	TArray<int32> m_SuitComponentIds;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseT.h"
/**
 * 
 */
class EDITOR_API SuitPartT :public BaseT
{
public:
	SuitPartT();
	~SuitPartT();
	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj) override;

public:
	FString m_IconUrl;
	int32 m_IsMainPart;
	int32 m_Status;
	TArray<int32> m_SuitComponentIds;
	int32 m_SuitComponentLibraryId;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseT.h"
/**
 * 
 */
class EDITOR_API SuitPatternT:public BaseT
{
public:
	SuitPatternT();
	~SuitPatternT();
	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj) override;

public:
	bool m_buy;
	int32 m_height = 0;
	FString m_iconUrl;
	int32 m_ownerId = 0;
	FString m_ownerType;
	FString m_Sn;
	int32 m_SupplierId = 0;
	FString m_SystemId;
	int32 m_Width = 0;

	// 主键Id.
	FString m_patternId = TEXT("0");

	FString m_baseTextureUrl;

};

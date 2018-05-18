// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseT.h"
/**
 * 
 */
class EDITOR_API SuitFabricColorT:public BaseT
{
public:
	SuitFabricColorT();

	virtual ~SuitFabricColorT();

public:

	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj) override;

public:
	FString m_GoodsId;
	bool m_IsBuy;
	FString m_PantoneCode;
	FString m_ProductCode;
	FLinearColor m_RGB;
	FString m_sn;
	FString m_icon;
	FString m_hexRgb;
	// 贴图数组.
	TArray<FString> m_textureUrlArray;

	// 材质.
	FString m_materialUrl = TEXT("");

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseT.h"
/**
 * 
 */
class EDITOR_API SuitFabricT:public BaseT
{
public:
	SuitFabricT();
	~SuitFabricT();
	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj) override;

public:
	FString m_BasePatternUrl;
	bool m_CanPattern;
	bool m_CanShowDetails;
	int32 m_ColorRepositoryId;
	FString m_IconUrl;
	FString m_MaskUrl;
	FString m_MaterialTemplateUrl;
	FString m_MetalUrl;

	FString m_NormalUrl;
	FString m_PrimaryPantoneCode;
	FString m_RoughnessUrl;
	//!!！询问传的是不是String.
	FVector2D m_UVScale = FVector2D::ZeroVector;
	// 法线贴图的uv缩放.
	float m_normalOriginalSize = 0;
};

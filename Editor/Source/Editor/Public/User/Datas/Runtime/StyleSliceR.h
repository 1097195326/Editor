// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseR.h"
/**
 * 
 */
class EDITOR_API StyleSliceR :public BaseR
{
public:
	// 辅料集合.
	TArray<int32> m_styleAccessoryIds;

	// 面料ID.
	int32 m_suitFabricId = 0;

	// 花型Id.
	FString m_suitPatternId;

	// 颜色id.
	int32 m_suitColorId = 0;

	// 旋转角度.
	float m_angle = 0.0f;

	// 偏移.
	FVector2D m_offset;

	// uv缩放.
	FVector2D m_uVScaleUser;



public:
	StyleSliceR();
	virtual ~StyleSliceR();

public:

	virtual void OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj) override;

	virtual void OnLoad() override;

	virtual void OnRelease() override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseC.h"
/**
 * 
 */
class EDITOR_API StyleSliceC :public BaseC
{
public:
	// 面料ID.
	int32 m_suitFabricId = 0;

	// 花型Id.
	FString m_suitPatternId;

	// 颜色id.
	int32 m_suitColorId = 0;

	// 旋转角度.
	float m_angle = 0.0f;

	// 偏移.
	FVector2D m_offset = FVector2D::ZeroVector;

	// uv缩放.
	FVector2D m_uVScaleUser = FVector2D::ZeroVector;

public:
	StyleSliceC();

	virtual ~StyleSliceC();

protected:
	// 子类可以重写.
	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void OnInitData(int32 _cid, int32 _rid, int32 _tid) override;
public:
	virtual void AddJsonC(TSharedPtr<FJsonObject> & _jsonObj) override;
};

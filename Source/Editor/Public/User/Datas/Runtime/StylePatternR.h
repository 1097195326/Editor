// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseR.h"
/**
 * 
 */
class EDITOR_API StylePatternR :public BaseR
{
public:
	// 旋转角度.
	float m_angle = 0.0f;

	// 偏移.
	FVector2D m_offset = FVector2D::ZeroVector;

	// uv缩放.
	FVector2D m_uVScaleUser = FVector2D::ZeroVector;

public:
	StylePatternR();
	virtual ~StylePatternR();

public:

	virtual void OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj) override;

	virtual void OnLoad() override;

	virtual void OnRelease() override;
};  

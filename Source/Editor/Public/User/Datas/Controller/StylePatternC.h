// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseC.h"
/**
 * 
 */
class EDITOR_API StylePatternC :public BaseC
{

public:
	StylePatternC();
	
	virtual ~StylePatternC();

public:
	FString m_patternTid;

	// 旋转角度.
	float m_angle = 0.0f;

	// 偏移.
	FVector2D m_offset;

	// uv缩放.
	FVector2D m_uVScaleUser;

protected:
	// 子类可以重写.
	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void OnInitData(int32 _cid, int32 _rid, int32 _tid) override;
public:
	virtual void AddJsonC(TSharedPtr<FJsonObject> & _jsonObj) override;

	void ChangePatternTid(const FString & _patternTid);
};

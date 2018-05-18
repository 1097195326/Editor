// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseC.h"

/**
 * 
 */
class EDITOR_API StyleStitchC :public BaseC
{
public:
	// .
	int32 m_StyleComponentId = 0;

public:
	int32 m_SuitComponentId = 0;

public:
	StyleStitchC();

	virtual ~StyleStitchC();

protected:
	// 子类可以重写.
	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void OnInitData(int32 _cid, int32 _rid, int32 _tid) override;
public:
	virtual void AddJsonC(TSharedPtr<FJsonObject> & _jsonObj) override;
};

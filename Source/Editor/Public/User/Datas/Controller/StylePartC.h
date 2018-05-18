// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseC.h"
/**
 * 
 */
class EDITOR_API StylePartC :public BaseC
{

public: // R.
	int32 m_styleComponentId = 0;

public: // T.
	int32 m_suitComponentId = 0;

public:
	StylePartC();

	virtual ~StylePartC();

protected:
	// 子类可以重写.
	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void OnInitData(int32 _cid, int32 _rid, int32 _tid) override;
public:
	virtual void AddJsonC(TSharedPtr<FJsonObject> & _jsonObj) override;
};

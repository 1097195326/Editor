// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseC.h"
/**
 * 
 */
class EDITOR_API StyleFabricC :public BaseC
{
public:
	int32 m_styleColorId = 0;

public:
	int32 m_suitColorId = 0;

public:
	StyleFabricC();

	virtual ~StyleFabricC();

protected:
	// 子类可以重写.
	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void OnInitData(int32 _cid, int32 _rid, int32 _tid) override;
public:
	virtual void AddJsonC(TSharedPtr<FJsonObject> & _jsonObj) override;
};

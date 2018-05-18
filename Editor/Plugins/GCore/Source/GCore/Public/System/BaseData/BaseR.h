// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseT.h"
#include "ILoad.h"
#include "GObject.h"


/**
 * 
 */
class GCORE_API BaseR : public ILoad
{  
	
protected:
	int32 m_Rid = 0;
	
	int32 m_Tid = 0;

	FString m_name;

public:
	// set get方法.
	int32 GetRid();

	int32 GetTid();

public:
	BaseR();
	virtual ~BaseR();

public:
	// 子类不允许重写.
	virtual void GLoad() final;

	virtual void GRelease() final;

	virtual void InitWithJson(const TSharedPtr<FJsonObject> & _jsonObj) final;

protected:

	// 子类可以重写.
	virtual void OnLoad() = 0;

	virtual void OnRelease() = 0;

	virtual void OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj);

};

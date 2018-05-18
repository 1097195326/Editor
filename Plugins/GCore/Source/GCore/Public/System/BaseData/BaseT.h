// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ILoad.h"
#include "GObject.h"
#include "tinyxml.h"
#include "tinystr.h"
#include "Serialization/JsonSerializer.h"

/**
 * 
 */
class GCORE_API BaseT : public GObject, public ILoad
{

public:

	int32	    	m_Tid = 0;

	FString			m_name;

public:
	BaseT();
	virtual ~BaseT();

public:
	// set get方法.
	int32 GetTid();

public:
	// 子类不允许重写.
	virtual void GLoad() final;

	virtual void GRelease() final;
	
	virtual void InitWithJson(const TSharedPtr<FJsonObject> & _jsonObj) final;

protected:

	virtual void OnLoad() = 0;

	virtual void OnRelease() = 0;
	
	virtual void OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj);
};

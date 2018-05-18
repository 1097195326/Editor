// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseR.h"
/**
 * 
 */
class EDITOR_API StyleAccessoriesR :public BaseR
{
public:
	// 片集合.
	int32 m_styleComponentId;

public:
	
	StyleAccessoriesR();

	virtual ~StyleAccessoriesR();

public:

	virtual void OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj) override;

	virtual void OnLoad() override;

	virtual void OnRelease() override;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseR.h"
/**
 * 
 */
class EDITOR_API StyleFabricR :public BaseR
{
public:
	int32 m_styleColorId = 0;

public:
	
	StyleFabricR();
	
	virtual ~StyleFabricR();

public:

	virtual void OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj) override;

	virtual void OnLoad() override;

	virtual void OnRelease() override;

};

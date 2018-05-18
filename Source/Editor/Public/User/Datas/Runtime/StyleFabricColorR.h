// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseR.h"
/**
 * 
 */
class EDITOR_API StyleFabricColorR :public BaseR
{
public:


public:
	
	StyleFabricColorR();

	virtual ~StyleFabricColorR();

public:

	virtual void OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj) override;

	virtual void OnLoad() override;

	virtual void OnRelease() override;
};

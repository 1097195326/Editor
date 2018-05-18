// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GBaseRuntimeManager.h"
/**
 * 
 */
class EDITOR_API BaseRStyleManager :public GBaseRuntimeManager
{
public:
	
	BaseRStyleManager();
	
	virtual ~BaseRStyleManager();

protected:

	virtual void OnLoad() override;

	virtual void OnRelease() override;

protected:

	// 解析style数据.
	virtual void EncodeStyleData(TSharedPtr<FJsonObject> & _jsonObject, TArray<BaseR*> & _arrayData);
	virtual void EncodeStyleDataById(TSharedPtr<FJsonObject> & _jsonObject, TArray<BaseR*> & _arrayData);

};

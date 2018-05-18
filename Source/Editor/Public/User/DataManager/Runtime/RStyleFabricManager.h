// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GBaseRuntimeManager.h"
/**
 * 
 */
class EDITOR_API RStyleFabricManager :public GBaseRuntimeManager
{
private:
	// 单利. 
	static RStyleFabricManager *   m_pInstance;

public:
	static RStyleFabricManager * GetInstance();

public:
	
	RStyleFabricManager();

	virtual ~RStyleFabricManager();

public:

	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void EncodeData(TSharedPtr<FJsonObject> & _content) override;
};

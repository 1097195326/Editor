// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GBaseRuntimeManager.h"
/**
 * 
 */
class EDITOR_API RStyleFabricColorManager :public GBaseRuntimeManager
{

private:
	// 单利. 
	static RStyleFabricColorManager *   m_pInstance;

public:
	static RStyleFabricColorManager * GetInstance();

public:
	
	RStyleFabricColorManager();

	virtual ~RStyleFabricColorManager();

public:

	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void EncodeData(TSharedPtr<FJsonObject> & _content) override;
};

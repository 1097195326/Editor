// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GBaseRuntimeManager.h"
/**
 * 
 */
class EDITOR_API RStyleStitchManager :public GBaseRuntimeManager
{
private:
	// 单利. 
	static RStyleStitchManager *   m_pInstance;

public:
	static RStyleStitchManager * GetInstance();

public:
	
	RStyleStitchManager();
	
	virtual ~RStyleStitchManager();

public:

	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void EncodeData(TSharedPtr<FJsonObject> & _content) override;
};

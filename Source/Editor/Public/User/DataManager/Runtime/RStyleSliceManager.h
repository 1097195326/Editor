// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GBaseRuntimeManager.h"
/**
 * 
 */
class EDITOR_API RStyleSliceManager :public GBaseRuntimeManager
{
private:
	// 单利. 
	static RStyleSliceManager *   m_pInstance;

public:
	static RStyleSliceManager * GetInstance();

public:
	
	RStyleSliceManager();
	
	virtual ~RStyleSliceManager();

public:

	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void EncodeData(TSharedPtr<FJsonObject> & _content) override;
};

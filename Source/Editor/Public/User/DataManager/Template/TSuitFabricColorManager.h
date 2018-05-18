// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SuitFabricColorT.h"
#include "GBaseTemplateManager.h"
/**
 * 
 */
class EDITOR_API TSuitFabricColorManager :public GBaseTemplateManager
{
private:
	// 单利. 
	static TSuitFabricColorManager *   m_pInstance;

public:
	static TSuitFabricColorManager * GetInstance();

public:
	
	TSuitFabricColorManager();

	virtual ~TSuitFabricColorManager();

public:

	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void EncodeData(TSharedPtr<FJsonObject> & _content) override;

};

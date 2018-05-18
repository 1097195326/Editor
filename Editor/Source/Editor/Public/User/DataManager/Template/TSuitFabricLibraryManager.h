// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GBaseTemplateManager.h"
/**
 * 
 */
class EDITOR_API TSuitFabricLibraryManager :public GBaseTemplateManager
{

private:
	// 单利. 
	static TSuitFabricLibraryManager *   m_pInstance;

public:
	static TSuitFabricLibraryManager * GetInstance();

public:
	TSuitFabricLibraryManager();
	
	virtual ~TSuitFabricLibraryManager();

public:

	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void EncodeData(TSharedPtr<FJsonObject> & _content) override;
};

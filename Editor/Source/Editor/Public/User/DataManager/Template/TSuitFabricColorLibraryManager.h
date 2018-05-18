// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GBaseTemplateManager.h"
/**
 * 
 */
class EDITOR_API TSuitFabricColorLibraryManager :public GBaseTemplateManager
{
private:
	// 单利. 
	static TSuitFabricColorLibraryManager *   m_pInstance;

public:
	static TSuitFabricColorLibraryManager * GetInstance();

public:
	
	TSuitFabricColorLibraryManager();

	virtual ~TSuitFabricColorLibraryManager();

public:

	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void EncodeData(TSharedPtr<FJsonObject> & _content) override;

	bool CheckFabricColorInLibrary(const FString & _sn, int32 _colorLibTid);
};

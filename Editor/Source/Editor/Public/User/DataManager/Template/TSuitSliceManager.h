﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GBaseTemplateManager.h"
/**
 * 
 */
class EDITOR_API TSuitSliceManager :public GBaseTemplateManager
{
private:
	// 单利. 
	static TSuitSliceManager *   m_pInstance;

public:
	static TSuitSliceManager * GetInstance();

public:
	TSuitSliceManager();
	
	virtual ~TSuitSliceManager();

public:

	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void EncodeData(TSharedPtr<FJsonObject> & _content) override;
};
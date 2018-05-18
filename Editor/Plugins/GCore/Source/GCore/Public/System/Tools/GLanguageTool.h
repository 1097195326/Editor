// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ILoad.h"

/**
*
*/
class GCORE_API GLanguageTool :public ILoad
{

private:

	// key-value.
	TMap<FString, FString> m_values;

	// 单利.
	static GLanguageTool *  m_pInstance;

public:

	GLanguageTool();

	virtual ~GLanguageTool();

	static GLanguageTool * GetInstance();

public:
	virtual void GLoad() override;

	virtual void GRelease() override;

public:
	FString GetLanguage(const FString & _key);

	FString GetLanguageTexture2D(const FString & _key);
};

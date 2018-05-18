// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "castle_std.h"
#include "BasePrototype.h"
/**
 * 
 */
class GCORE_API LanguagePrototype :public BasePrototype
{
private:

	// 文字对应的key value.
	TMap<FString, FString> m_values;

	// 图片对应的key value.
	TMap<FString, FString> m_assetValues;

public:
	LanguagePrototype();

	virtual ~LanguagePrototype();

protected:

	virtual void OnLoadXMLData(TiXmlElement * _elem) override;

	virtual void OnLoad() override;

	virtual void OnRelease() override;

public:
	FString GetLanguage(const FString & _key);

	FString GetLanguageTexture2D(const FString & _key);
};

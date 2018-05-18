// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "castle_std.h"
#include "BasePrototype.h"
/**
 * 
 */
class GCORE_API UIPrototype :public BasePrototype
{
private:

	// key-value.
	TMap<FString, FString> m_values;

public:

	int32 m_screenWidth = 1224;

	int32 m_scrrenHeight = 2204;

public:
	UIPrototype();
	virtual ~UIPrototype();

protected:

	virtual void OnLoadXMLData(TiXmlElement * _elem) override;

	virtual void OnLoad() override;

	virtual void OnRelease() override;

public:

	// 根据key获取UI的PrototypeID _key是分辨率的比例.
	int32 GetUIPrototypeId(const FString & _key);
};

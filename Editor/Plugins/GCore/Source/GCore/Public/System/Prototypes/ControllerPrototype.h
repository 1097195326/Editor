// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GController.h"
#include "BasePrototype.h"



/**
 * 
 */
class GCORE_API ControllerPrototype:public BasePrototype
{
private:

	// key-value.
	TMap<FString,FString> m_values;
	
public:
	ControllerPrototype();
	virtual ~ControllerPrototype();

protected:

	virtual void OnLoadXMLData(TiXmlElement * _elem) override;

	virtual void OnLoad() override;

	virtual void OnRelease() override;

public:
	
	// 根据key获取value.
	const FString GetValueByKey(const FString & _key);

	// 是否debug模式.
	bool IsDebug();

	bool IsAdmin();

	bool IsAdaptation();
	// 获取平台.
	ENUM_PLATFORM GetPlatform();

	// 获取语言.
	ENUM_LANGUAGE GetLanguage();

	// 获取appVersion.
	FString GetAppVersion();
};

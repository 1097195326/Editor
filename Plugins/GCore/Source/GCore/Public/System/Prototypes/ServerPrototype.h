// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "castle_std.h"
#include "BasePrototype.h"
/**
 * 
 */
class GCORE_API ServerPrototype :public BasePrototype
{
private:

	// key-value.
	TMap<FString, FString> m_values;

public:
	ServerPrototype();
	virtual ~ServerPrototype();

protected:

	virtual void OnLoadXMLData(TiXmlElement * _elem) override;

	virtual void OnLoad() override;

	virtual void OnRelease() override;

public:

	// 根据key获取value.
	const FString GetValueByKey(const FString & _key);
};

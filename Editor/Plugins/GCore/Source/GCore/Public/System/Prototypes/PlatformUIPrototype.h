// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "castle_std.h"
#include "BasePrototype.h"
/**
 * 
 */
class GCORE_API PlatformUIPrototype :public BasePrototype
{
private:

	// key-value.
	TMap<FString, FString> m_values;

	// child-parent.
	TMap<FString, FString> m_parentUI;

public:
	PlatformUIPrototype();
	virtual ~PlatformUIPrototype();

protected:

	virtual void OnLoadXMLData(TiXmlElement * _elem) override;

	virtual void OnLoad() override;

	virtual void OnRelease() override;

public:

	// 根据key获取value.
	FString GetUMGPath(const FString & _key);

	// 获取父级UI.
	FString GetParentUIName(const FString & _key);
};

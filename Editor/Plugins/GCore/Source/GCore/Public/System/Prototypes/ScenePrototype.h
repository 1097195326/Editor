// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AARefClasses.h"
#include "castle_std.h"
#include "BasePrototype.h"
/**
 * 
 */
class GCORE_API ScenePrototype :public BasePrototype
{
private:
	// key-value.
	TMap<FString, FString> m_values;

public:
	ScenePrototype();
	virtual ~ScenePrototype();

protected:

	virtual void OnLoadXMLData(TiXmlElement * _elem) override;

	virtual void OnLoad() override;

	virtual void OnRelease() override;

public:
	// 根据平台获取场景资源路径.
	FString GetLevelName(const FString & _levelName);
};

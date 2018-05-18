// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AARefClasses.h"
#include "castle_std.h"
#include "BasePrototype.h"
/**
 * 
 */
class GCORE_API SchedulerMapping :public BasePrototype
{
private:
	// key-value.
	TMap<FString, FString> m_values;

public:
	SchedulerMapping();

	virtual ~SchedulerMapping();

protected:

	virtual void OnLoadXMLData(TiXmlElement * _elem) override;

	virtual void OnLoad() override;

	virtual void OnRelease() override;

public:
	FString GetClassName(const FString & _className);

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ILoad.h"
#include "GCore/Public/AAReflection/AARefClasses.h"
/**
 * 
 */
class GCORE_API BaseScheduler:public ILoad
{
public:
	BaseScheduler();

	virtual ~BaseScheduler();

public:
	virtual void GLoad() final;

	virtual void GRelease() final;

protected:

	virtual void OnLoad() = 0;

	virtual void OnRelease() = 0;

public:
	virtual void Excute(TMap<FString,FString> & _params) = 0;
};

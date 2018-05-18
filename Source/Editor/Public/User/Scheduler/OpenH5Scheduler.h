// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseScheduler.h"
/**
 * 
 */
class EDITOR_API OpenH5Scheduler:public BaseScheduler
{
public:
	OpenH5Scheduler();

	virtual ~OpenH5Scheduler();

protected:

	virtual void OnLoad() override;

	virtual void OnRelease() override;

public:
	virtual void Excute(TMap<FString, FString> & _params) override;
};

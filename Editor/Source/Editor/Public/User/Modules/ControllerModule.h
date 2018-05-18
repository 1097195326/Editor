// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/BaseData/BaseModule.h"

/**
 * 
 */
class EDITOR_API ControllerModule : public BaseModule
{
public:
	ControllerModule();
	virtual ~ControllerModule();
public:
	virtual void OnLoad() override;
	virtual void OnRelease() override;

};

// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Modules/ModuleInterface.h"
#include "CoreMinimal.h"

class OSSMODULE_API IOSSModule : public IModuleInterface
{
public:
	static IOSSModule& Get();

	virtual void StartupModule() override;

	virtual void ShutdownModule() override;
};

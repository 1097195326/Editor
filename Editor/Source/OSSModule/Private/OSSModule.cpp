// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#include "OSSModule.h"
#include "Misc/Paths.h"
#include "Modules/ModuleManager.h"
IMPLEMENT_MODULE(IOSSModule, OSSModule)


IOSSModule& IOSSModule::Get()
{
	return FModuleManager::LoadModuleChecked< IOSSModule >("OSSModule");
}

void IOSSModule::StartupModule()
{

}

void IOSSModule::ShutdownModule()
{

}

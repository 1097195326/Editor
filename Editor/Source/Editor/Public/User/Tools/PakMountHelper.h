// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
namespace PakMountHelper
{
	bool MountPak(const FString &inPakFilePath, const FString &inMountPath);
	bool GetFiles(const FString& FullPathOfBaseDir, TArray<FString>& FilenamesOut, bool inRecursive = false, const FString& inFilterByExtension = "");
}

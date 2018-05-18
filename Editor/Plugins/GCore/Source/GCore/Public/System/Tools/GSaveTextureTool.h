// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "GSaveTextureTool.generated.h"

/**
 * 
 */
UCLASS()
class GCORE_API UGSaveTextureTool : public UObject
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "GSaveTextureTool")
	static void SaveTexture(UTexture2D * InRenderTarget, FString Filename, bool isConvertJpg);
	
	
};

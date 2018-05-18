// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GStaticFunction.generated.h"

/**
 * 
 */
UCLASS()
class EDITOR_API UGStaticFunction : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "StaticFunction")
	static void GetCameraPhotoCover(float & _cameraDistance,FVector & _cameraOffset,float & _rotateAngle,int32 & _isDisplayModel);
	
	UFUNCTION(BlueprintCallable, Category = "StaticFunction")
	static void GetCameraDesignCover(float & _cameraDistance, FVector & _cameraOffset, float & _rotateAroundYawDegree, float &_RotateSelfPitchDegree);

	UFUNCTION(BlueprintCallable, Category = "StaticFunction")
	static int32 GetGPlatform();

	UFUNCTION(BlueprintCallable, Category = "StaticFunction")
	static void OnCapture(bool _isShow);
};

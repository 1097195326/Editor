// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/StaticMeshComponent.h"
#include "ComponentStruct.generated.h"


USTRUCT(BlueprintType)
struct FMeshComponentStruct
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
		TArray<UStaticMeshComponent*> m_meshComponentArray;

	UPROPERTY(BlueprintReadWrite)
		float m_CameraRotate = 0.0f;

	UPROPERTY(BlueprintReadWrite)
		float m_cameraPitching = 0.0f;

	UPROPERTY(BlueprintReadWrite)
		float m_cameraDistance = 0.0f;

	UPROPERTY(BlueprintReadWrite)
		float m_cameraHeight = 0.0f;

};

/**
 * 
 */
UCLASS()
class EDITOR_API UComponentStruct : public UObject
{
	GENERATED_BODY()
	
	
	
	
};

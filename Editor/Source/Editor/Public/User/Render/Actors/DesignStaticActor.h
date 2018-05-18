// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/Render/BaseActor.h"
#include "Components/StaticMeshComponent.h"
#include "DesignStaticActor.generated.h"

/**
 * 
 */
UCLASS()
class EDITOR_API ADesignStaticActor : public ABaseActor
{
	GENERATED_UCLASS_BODY()
	
public:

	virtual void InitComponent();
	
	virtual void OnBeginPlay() override;

	virtual void OnEndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, Category = DesignActor)
	void HidePillar();
	UFUNCTION(BlueprintCallable, Category = DesignActor)
	void HideTable();
	UFUNCTION(BlueprintCallable, Category = DesignActor)
	void HidePillarAndTable();

	UFUNCTION(BlueprintCallable, Category = DesignActor)
	void InitTable();

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Pawn.h"
#include "Walker.generated.h"

UCLASS()
class EDITOR_API AWalker : public APawn
{ 
	GENERATED_BODY()
public:
	AWalker();
protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;	
public:

	UFUNCTION(BlueprintCallable, Category = Walk)
	void GoWalk(class USkeletalMeshComponent *inBaseSkeletalMesh);

	void AddSkeletalMeshComponent(USkeletalMeshComponent * _meshComponent);

public:
	UPROPERTY(Transient)
	TArray<class USkeletalMeshComponent*> SkeletalMeshComponents;
};

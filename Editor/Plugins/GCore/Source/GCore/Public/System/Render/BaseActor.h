// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseActor.generated.h"

UCLASS()
class GCORE_API ABaseActor : public AActor
{
	GENERATED_UCLASS_BODY()

private:
	// 自增的ComponentCid
	int32 m_currentComponentId = 0;

public: // 子类不要重写.

	virtual void GRelease();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected: // 子类需要重写这个.


	virtual void OnBeginPlay();

	virtual void OnEndPlay(const EEndPlayReason::Type EndPlayReason);

public:

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

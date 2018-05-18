// Fill out your copyright notice in the Description page of Project Settings.

#include "GCore.h"
#include "BaseActor.h"
#include "BaseComponent.h"


ABaseActor::ABaseActor(const FObjectInitializer & ObjectInitializer) :Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
}



void ABaseActor::GRelease()
{

	// 移除渲染.
	this->Destroy();
}

// Called when the game starts or when spawned
void ABaseActor::BeginPlay()
{
	Super::BeginPlay();

	OnBeginPlay();
}

void ABaseActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	OnEndPlay(EndPlayReason);
}

void ABaseActor::OnBeginPlay()
{

}

void ABaseActor::OnEndPlay(const EEndPlayReason::Type EndPlayReason)
{

}

// Called every frame
void ABaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


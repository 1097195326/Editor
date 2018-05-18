// Fill out your copyright notice in the Description page of Project Settings.

#include "GCore.h"
#include "GBaseGameMode.h"




AGBaseGameMode::AGBaseGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void AGBaseGameMode::BeginPlay()
{
	Super::BeginPlay();

	OnBeginPlay();
}


void AGBaseGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	OnEndPlay(EndPlayReason);
}

void AGBaseGameMode::OnBeginPlay()
{

}

void AGBaseGameMode::OnEndPlay(const EEndPlayReason::Type EndPlayReason)
{

}

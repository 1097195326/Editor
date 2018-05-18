// Fill out your copyright notice in the Description page of Project Settings.


#include "Walker.h"

// Sets default values
AWalker::AWalker()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}
void AWalker::BeginPlay()
{
	Super::BeginPlay();
}

void AWalker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWalker::GoWalk(USkeletalMeshComponent *inBaseSkeletalMesh)
{
	if (inBaseSkeletalMesh == nullptr)
	{
		return;
	}

	for (int index = 0; index < SkeletalMeshComponents.Num(); ++index)
	{
		SkeletalMeshComponents[index]->SetMasterPoseComponent(inBaseSkeletalMesh);
		SkeletalMeshComponents[index]->AttachToComponent(inBaseSkeletalMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		SkeletalMeshComponents[index]->bUseAttachParentBound = true;
	}
}

void AWalker::AddSkeletalMeshComponent(USkeletalMeshComponent * _meshComponent)
{
	SkeletalMeshComponents.Add(_meshComponent);
}

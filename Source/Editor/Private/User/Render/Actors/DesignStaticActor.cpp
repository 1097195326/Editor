// Fill out your copyright notice in the Description page of Project Settings.

#include "DesignStaticActor.h"


ADesignStaticActor::ADesignStaticActor(const FObjectInitializer & ObjectInitializer) :Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
}

void ADesignStaticActor::InitComponent()
{
	
}

void ADesignStaticActor::OnBeginPlay()
{
	this->SetActorLocation(FVector(0.0f, 0.0f, 0.0f));
}

void ADesignStaticActor::OnEndPlay(const EEndPlayReason::Type EndPlayReason)
{

}

void ADesignStaticActor::HidePillar()
{
	TArray<UActorComponent*> SourceComps = GetComponentsByClass(UStaticMeshComponent::StaticClass());

	for (int32 i=0;i<SourceComps.Num();i++)
	{
		UStaticMeshComponent* mComponent = (UStaticMeshComponent*)SourceComps[i];

		FString mComponentName = mComponent->GetName();

		if (mComponentName.Equals(TEXT("zhuzi")))
		{
			mComponent->SetVisibility(false);
		}
	}
}

void ADesignStaticActor::HideTable()
{
	TArray<UActorComponent*> SourceComps = GetComponentsByClass(UStaticMeshComponent::StaticClass());

	for (int32 i = 0; i < SourceComps.Num(); i++)
	{
		UStaticMeshComponent* mComponent = (UStaticMeshComponent*)SourceComps[i];

		FString mComponentName = mComponent->GetName();

		if (mComponentName.Equals(TEXT("Table")))
		{
			mComponent->SetVisibility(false);
		}
	}
}

void ADesignStaticActor::HidePillarAndTable()
{
	TArray<UActorComponent*> SourceComps = GetComponentsByClass(UStaticMeshComponent::StaticClass());

	for (int32 i = 0; i < SourceComps.Num(); i++)
	{
		UStaticMeshComponent* mComponent = (UStaticMeshComponent*)SourceComps[i];

		FString mComponentName = mComponent->GetName();

		if (mComponentName.Equals(TEXT("Table")))
		{
			mComponent->SetVisibility(false);
		}
		if (mComponentName.Equals(TEXT("zhuzi")))
		{
			mComponent->SetVisibility(false);
		}
	}
}

void ADesignStaticActor::InitTable()
{
	TArray<UActorComponent*> SourceComps = GetComponentsByClass(UStaticMeshComponent::StaticClass());

	for (int32 i = 0; i < SourceComps.Num(); i++)
	{
		UStaticMeshComponent* mComponent = (UStaticMeshComponent*)SourceComps[i];
		
		FString mComponentName = mComponent->GetName();

		if (mComponentName.Equals(TEXT("Table")))
		{
			mComponent->SetVisibility(true);
		}
		if (mComponentName.Equals(TEXT("zhuzi")))
		{
			mComponent->SetVisibility(true);
		}
	}
}

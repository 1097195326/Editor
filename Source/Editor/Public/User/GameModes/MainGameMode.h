#pragma once
#include "CoreMinimal.h"
#include "System/GameMode/GBaseGameMode.h"
#include "MainGameMode.generated.h"

UCLASS()
class EDITOR_API AMainGameMode : public AGBaseGameMode
{
	GENERATED_BODY()


protected:
	// 子类需要重写. 
	virtual void OnBeginPlay() override;

	virtual void OnEndPlay(const EEndPlayReason::Type EndPlayReason) override;

};
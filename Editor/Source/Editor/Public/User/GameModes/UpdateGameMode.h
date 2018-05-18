#pragma once
#include "CoreMinimal.h"
#include "System/GameMode/GBaseGameMode.h"
#include "UpdateGameMode.generated.h"

UCLASS()
class EDITOR_API AUpdateGameMode : public AGBaseGameMode
{
	GENERATED_BODY()


protected:
	// 子类需要重写. 
	virtual void OnBeginPlay() override;

	virtual void OnEndPlay(const EEndPlayReason::Type EndPlayReason) override;

};

#include "UpdateGameMode.h"
#include "GameDirector.h"

void AUpdateGameMode::OnBeginPlay()
{
	UGameDirector::GetInstance()->OpenLogicScene(TEXT("LogicUpdateScene"), this);

	GSchedulerManager::GetInstance()->AddCondition(TEXT("gameMode"), TEXT("UpdateGameMode"));
}

void AUpdateGameMode::OnEndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameDirector::GetInstance()->CloseCurrentLogicScene();
}


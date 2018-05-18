#include "MainGameMode.h"
#include "GameDirector.h"

void AMainGameMode::OnBeginPlay()
{
	UGameDirector::GetInstance()->OpenLogicScene(TEXT("LogicMainScene"), this);

	GSchedulerManager::GetInstance()->AddCondition(TEXT("gameMode"), TEXT("MainGameMode"));
}

void AMainGameMode::OnEndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameDirector::GetInstance()->CloseCurrentLogicScene();
}

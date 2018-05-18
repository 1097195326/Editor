#include "WalkingUIController.h"
#include "EditorDirector.h"

GREGISTER(WalkingUIController)

WalkingUIController::WalkingUIController()
{

}

WalkingUIController::~WalkingUIController()
{

}

void WalkingUIController::OnLoad()
{
	SetRootView(UUIManager::GetInstance()->OpenUI(TEXT("WalkingUI")));
	GetRootUI()->AddToViewport();
	UEditorDirector::GetInstance()->ShowMouseCursor();


}

void WalkingUIController::OnRelease()
{

}

void WalkingUIController::OnBackstageUI()
{
}

void WalkingUIController::OnActivationUI()
{
	SetRootView(UUIManager::GetInstance()->OpenUI(TEXT("WalkingUI")));
	GetRootUI()->AddToViewport();
}


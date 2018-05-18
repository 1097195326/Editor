#include "DesignUIController.h"
#include "EditorDirector.h"
#include "DesignUMG.h"
#include "GModuleManager.h"
#include "GameDirector.h"
#include "LogicDesignScene.h"
#include "Win_DesignUMG.h"
GREGISTER(DesignUIController)
DesignUIController::DesignUIController()
{

}

void DesignUIController::ChangePageTo(Change_PAGE _type)
{
	switch (_type)
	{
	case Change_PAGE::SHARED_PAGE:
		m_SharedPage = UUIManager::GetInstance()->OpenUI(TEXT("UI_SharedPage"));

		m_SharedPage->AddToViewport();

		break;
	}
}

DesignUIController::~DesignUIController()
{

}

void DesignUIController::OnLoad()
{
	SetRootView(UUIManager::GetInstance()->OpenUI(TEXT("DesignUMG")));
	GetRootUI()->AddToViewport();
	UGameDirector::GetInstance()->ShowMouseCursor();

}

void DesignUIController::OnRelease()
{

}

void DesignUIController::OnBackstageUI()
{
	GetRootUI()->RemoveFromViewport();
}

void DesignUIController::OnActivationUI()
{
	SetRootView(UUIManager::GetInstance()->OpenUI(TEXT("DesignUMG")));
	GetRootUI()->AddToViewport();
}

void DesignUIController::InitMainCamera()
{
	DesignModule *mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");
	int32 mSuitTid = mDesignModule->GetDesignData()->m_designTID;

	LogicDesignScene *mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();
#if PLATFORM_WINDOWS
	((UWin_DesignUMG*)GetRootUI())->InitCamera(mLogicDesignScene->GetComponentStructByTid(mSuitTid, EnumComponentType::STYLE_COMPONENT));
#else
	((UDesignUMG*)GetRootUI())->InitCamera(mLogicDesignScene->GetComponentStructByTid(mSuitTid,EnumComponentType::STYLE_COMPONENT));
#endif
}


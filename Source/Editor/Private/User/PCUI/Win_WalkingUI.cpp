#include "Win_WalkingUI.h"

#include "UIManager.h"
#include "MsgCenter.h"
#include "LogicDesignScene.h"




UWin_WalkingUI::UWin_WalkingUI(const FObjectInitializer& ObjectInitializer)
{
	m_BackButton = nullptr;
}

bool UWin_WalkingUI::OnStart()
{

	return true;
}

void UWin_WalkingUI::OnInit()
{

	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("BackButton")))
	{
		m_BackButton = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(100, m_BackButton, this, &UWin_WalkingUI::OnButtonClick);
	}

}

void UWin_WalkingUI::OnRelease()
{
	Super::OnRelease();
	m_BackButton = nullptr;
	UUIManager::GetInstance()->RemoveButtonClick(m_BackButton);
}

void UWin_WalkingUI::OnSetParams(int32 _pid, FString & _content, ...)
{

}

void UWin_WalkingUI::OnTick(float InDeltaTimeSecond)
{

}

void UWin_WalkingUI::OnButtonClick(int32 _Param)
{
	msg_ptr mLocalMsg(new MsgLocal(MessageTypeLocal, LogicDesignScene::m_rebackDesignSceneMsg, nullptr));

	MsgCenter::Instance()->SendMsg(mLocalMsg);
}

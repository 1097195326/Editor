#include "WalkingUI.h"

#include "UIManager.h"
#include "MsgCenter.h"
#include "LogicDesignScene.h"




UWalkingUI::UWalkingUI(const FObjectInitializer& ObjectInitializer)
{
	m_BackButton = nullptr;
}

bool UWalkingUI::OnStart()
{

	return true;
}

void UWalkingUI::OnInit()
{

	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("BackButton")))
	{
		m_BackButton = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(100, m_BackButton, this, &UWalkingUI::OnButtonClick);
	}

}

void UWalkingUI::OnRelease()
{
	Super::OnRelease();
	m_BackButton = nullptr;
	UUIManager::GetInstance()->RemoveButtonClick(m_BackButton);
}

void UWalkingUI::OnSetParams(int32 _pid, FString & _content, ...)
{

}

void UWalkingUI::OnTick(float InDeltaTimeSecond)
{

}

void UWalkingUI::OnButtonClick(int32 _Param)
{
	msg_ptr mLocalMsg(new MsgLocal(MessageTypeLocal, LogicDesignScene::m_rebackDesignSceneMsg, nullptr));

	MsgCenter::Instance()->SendMsg(mLocalMsg);
}

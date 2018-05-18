
#include "Win_StyleSpecialSign.h"
#include "UIManager.h"
#include "GameDirector.h"
#include "LogicMainScene.h"
#include "Button.h"
#include "Runtime/SlateCore/Public/Styling/SlateStyle.h"


UWin_StyleSpecialSign::UWin_StyleSpecialSign(const FObjectInitializer& ObjectInitializer)
{

}


bool UWin_StyleSpecialSign::OnStart()
{
	if (UButton * mButton = (UButton *)GetWidgetFromName(TEXT("Sign")))
	{
		m_Button = mButton;
	}

	return true;
}

void UWin_StyleSpecialSign::OnInit()
{

}

void UWin_StyleSpecialSign::OnRelease()
{
	Super::OnRelease();
}
void UWin_StyleSpecialSign::SetNormal()
{
	m_Button->SetIsEnabled(false);
}
void UWin_StyleSpecialSign::SetSelect()
{
	//m_Button->WidgetStyle.SetPressed(m_Button->WidgetStyle.Pressed);
	m_Button->SetIsEnabled(true);
}

#include "StyleSpecialSign.h"
#include "UIManager.h"
#include "GameDirector.h"
#include "LogicMainScene.h"
#include "Button.h"
#include "Runtime/SlateCore/Public/Styling/SlateStyle.h"


UStyleSpecialSign::UStyleSpecialSign(const FObjectInitializer& ObjectInitializer)
{

}


bool UStyleSpecialSign::OnStart()
{
	if (UButton * mButton = (UButton *)GetWidgetFromName(TEXT("Sign")))
	{
		m_Button = mButton;
	}

	return true;
}

void UStyleSpecialSign::OnInit()
{

}

void UStyleSpecialSign::OnRelease()
{
	Super::OnRelease();
}
void UStyleSpecialSign::SetNormal()
{
	m_Button->SetIsEnabled(false);
}
void UStyleSpecialSign::SetSelect()
{
	//m_Button->WidgetStyle.SetPressed(m_Button->WidgetStyle.Pressed);
	m_Button->SetIsEnabled(true);
}
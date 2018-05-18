// Fill out your copyright notice in the Description page of Project Settings.

#include "Popup_Window.h"
#include "UIManager.h"

UPopup_Window::UPopup_Window(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{

}

bool UPopup_Window::OnStart()
{
	//	提示框按钮.
	if (UButton* BtnClose = Cast<UButton>(GetWidgetFromName("Button_Cancel")))
	{
		m_BtnCancel = BtnClose;

		UUIManager::GetInstance()->RegisterButtonClick(31, m_BtnCancel, this, &UPopup_Window::OnButtonCancel);
	}

	if (UButton* BtnConfirm = Cast<UButton>(GetWidgetFromName("Button_Confirm")))
	{
		m_BtnConfirm = BtnConfirm;

		UUIManager::GetInstance()->RegisterButtonClick(32, m_BtnConfirm, this, &UPopup_Window::OnButtonConfirm);
	}

	if (UButton* BtnClose = Cast<UButton>(GetWidgetFromName("Button_Close")))
	{
		m_ButtonClose = BtnClose;

		UUIManager::GetInstance()->RegisterButtonClick(33, m_ButtonClose, this, &UPopup_Window::OnButtonClose);
	}

	//	提示框内容
	if (UTextBlock* TextMsg = Cast<UTextBlock>(GetWidgetFromName("Text_Msg")))
	{
		m_TextContent = TextMsg;
	}

	return true;
}

void UPopup_Window::OnInit()
{
	
}

void UPopup_Window::OnRelease()
{
	
	UUIManager::GetInstance()->RemoveButtonClick(m_BtnCancel);

	UUIManager::GetInstance()->RemoveButtonClick(m_BtnConfirm);
}

void UPopup_Window::OnSetParams(int32 _pid, FString & _content, ...)
{

}

void UPopup_Window::OnTick(float InDeltaTimeSecond)
{

}

void UPopup_Window::OnInitAlertUI()
{
	if (nullptr != m_TextContent)
	{
		m_TextContent->SetText(FText::FromString(m_content));
	}
}

void UPopup_Window::OnButtonCancel(int32 _pid)
{
	ClickBtnCancel();
}

void UPopup_Window::OnButtonConfirm(int32 _pid)
{
	ClickBtnConfirm();
}

void UPopup_Window::OnButtonClose(int32 _pid)
{
	ClickBtnClose();
	//UUIManager::GetInstance()->CloseTrueOrFalseAlert();
}


// Fill out your copyright notice in the Description page of Project Settings.

#include "LoginUIController.h"
#include "Win_LoginPageCell.h"
#include "GameDirector.h"
// 反射.
GREGISTER(LoginUIController)

LoginUIController::LoginUIController()
{
}

LoginUIController::~LoginUIController()
{
}

void LoginUIController::OnLoad()
{
	// 初始化要打开的根UI.
	SetRootView(UUIManager::GetInstance()->OpenUI(TEXT("LoginPageCellView")));
	GetRootUI()->AddToViewport();
	UGameDirector::GetInstance()->ShowMouseCursor();
}

void LoginUIController::OnRelease()
{
	// 需要把子view管理.rootView已经在父类释放掉了.
}

void LoginUIController::OnBackstageUI()
{
	
}

void LoginUIController::OnActivationUI()
{

}

// void LoginUIController::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
// {
// 	check(PlayerInputComponent);
// 
// 	PlayerInputComponent->BindAction("Login", IE_Pressed, this, &LoginUIController::Login);
// }
// 
// void LoginUIController::Login()
// {
// 	UWin_LoginPageCell * mLoginPage = (UWin_LoginPageCell*)GetRootUI();
// 
// 	if (nullptr != mLoginPage)
// 	{
// 		if (mLoginPage->m_Login->IsVisible())
// 		{
// 			mLoginPage->HandlePassWordLogin(1);
// 		}
// 	}
// }

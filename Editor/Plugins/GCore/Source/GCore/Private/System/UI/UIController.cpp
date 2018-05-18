// Fill out your copyright notice in the Description page of Project Settings.

#include "GCore.h"
#include "UIController.h"
#include "UIPrototype.h"
#include "GController.h"
#include "PrototypeManager.h"


UIController::UIController()
{
}

UIController::~UIController()
{
}

void UIController::GLoad()
{
	m_textureManager = NewObject<UUITextureManager>();

	m_textureManager->GLoad();

	OnLoad();
}

void UIController::GRelease()
{
	OnRelease();

	if (nullptr != m_textureManager)
	{
		m_textureManager->GRelease();
	}
	if (this->m_rootView->IsValidLowLevel())
	{
		this->m_rootView->RemoveFromViewport();
	}
	// 先把rootview卸载掉.
	/*if(nullptr != this->m_rootView)
		this->m_rootView->RemoveFromViewport();
*/
	//this->m_rootView = nullptr;

	delete this;
}

UUITextureManager * UIController::GetTextureManager()
{
	return m_textureManager;
}

void UIController::OnLoad()
{

}


void UIController::OnRelease()
{

}

void UIController::OnBackstageUI()
{

}

void UIController::OnActivationUI()
{

}

void UIController::SetRootView(UBaseUI * _rootView)
{
	if (GController::m_isadaptation)
	{
		/*UIPrototype * mUIPrototype = (UIPrototype *)(PrototypeManager::GetInstance()->GetPrototypeByPID(PrototypeConst::P_UIPROTOTYPE_ID));

		float mScaleX = (float)GController::m_screenWidth / (float)mUIPrototype->m_screenWidth;

		float mScaleY = (float)GController::m_screenHeight / (float)mUIPrototype->m_scrrenHeight;

		FVector2D mVector2D(mScaleX, mScaleY);
		_rootView->SetRenderScale(mVector2D);*/
	}
	m_rootView = _rootView;
}

UBaseUI * UIController::GetRootUI()
{
	return m_rootView;
}

void UIController::BackstageUI()
{
	OnBackstageUI();

	m_rootView = nullptr;
}

void UIController::ActivationUI()
{
	OnActivationUI();
}

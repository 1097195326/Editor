	// Fill out your copyright notice in the Description page of Project Settings.

#include "Win_UserHead.h"
#include "MsgCenter.h"
#include "UIManager.h"
#include "GameDirector.h"
#include "GModuleManager.h"
#include "MainUIController.h"

UWin_UserHead::UWin_UserHead(const FObjectInitializer& ObjectInitializer)
{
	m_BtnUserHead = nullptr;

	m_ImageUserHead = nullptr;
}

bool UWin_UserHead::OnStart()
{
	Super::OnStart();

	LoginModule *mLoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	//	头像按钮--点击事件.
	if (UButton* BtnUserHead = Cast<UButton>(GetWidgetFromName("Button_UserHead")))
	{
		m_BtnUserHead = BtnUserHead;

		UUIManager::GetInstance()->RegisterButtonClick(1, m_BtnUserHead, this, &UWin_UserHead::OpenUserHead);
	}

	//	用户头像图片.
	if (UImage* ImageUserHead = Cast<UImage>(GetWidgetFromName("Image_UserHead")))
	{
		m_ImageUserHead = ImageUserHead;

		FString mImageUrl = mLoginModule->GetUserData()->m_member.avatar;

		if (mImageUrl.Len() > 0)
		{
			if (m_DownloadImageTool != nullptr)
			{
				m_DownloadImageTool->CancelDownload();
			}
			m_DownloadImageTool = UDownloadImageTool::DownloadImage(1, mImageUrl, this, &UWin_UserHead::OnDownloadImage);
		}
	}
	return true;
}

void UWin_UserHead::OnDownloadImage(bool _isScuccess, int32 _penetrateId, FString _url, UTexture2D * _texture2d,UDownloadImageTool * _downloadTool)
{
	m_ImageUserHead->GetDynamicMaterial()->SetTextureParameterValue("headTexture", _texture2d);

	m_ImageUserHead->Brush.ImageSize = FVector2D(120.0f, 120.0f);
}

void UWin_UserHead::OnInit()
{
	Super::OnInit();

}

void UWin_UserHead::OnRelease()
{
	Super::OnRelease();
	if (nullptr != m_DownloadImageTool)
	{
		m_DownloadImageTool->CancelDownload();

		m_DownloadImageTool = nullptr;
	}

	UUIManager::GetInstance()->RemoveButtonClick(m_BtnUserHead);

	if (nullptr != m_ImageUserHead && nullptr != m_ImageUserHead->Brush.GetResourceObject())
	{
		UTexture2D * mTexture = (UTexture2D *)m_ImageUserHead->Brush.GetResourceObject();

		mTexture->ReleaseResource();
	}
}

void UWin_UserHead::OnSetParams(int32 _pid, FString & _content, ...)
{

}

void UWin_UserHead::OnTick(float InDeltaTimeSecond)
{

}

void UWin_UserHead::OpenUserHead(int32 _pid)
{
	//	打开个人设置页面
	//LogicMainScene* m_MainScene = (LogicMainScene *)UGameDirector::GetInstance()->GetCurrentLogicScene();

	//MainUIController*  MainCon = (MainUIController*)(m_MainScene->GetCurrentUIController());

	//MainCon->ChangePageTo(PAGE_TYPE::PersonInfo_PAGE);
}

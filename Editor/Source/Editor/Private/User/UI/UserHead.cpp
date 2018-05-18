// Fill out your copyright notice in the Description page of Project Settings.

#include "UserHead.h"
#include "MsgCenter.h"
#include "UIManager.h"
#include "GameDirector.h"
#include "GModuleManager.h"
#include "MainUIController.h"

UUserHead::UUserHead(const FObjectInitializer& ObjectInitializer)
{
	m_BtnUserHead = nullptr;

	m_ImageUserHead = nullptr;
}

bool UUserHead::OnStart()
{
	LoginModule *mLoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	//	头像按钮--点击事件.
	if (UButton* BtnUserHead = Cast<UButton>(GetWidgetFromName("Button_UserHead")))
	{
		m_BtnUserHead = BtnUserHead;

		UUIManager::GetInstance()->RegisterButtonClick(1, m_BtnUserHead, this, &UUserHead::OpenUserHead);
	}

	//	用户头像图片.
	if (UImage* ImageUserHead = Cast<UImage>(GetWidgetFromName("Image_UserHead")))
	{
		m_ImageUserHead = ImageUserHead;
	}

	//	本地监听获取用户头像和昵称消息回调.
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, mLoginModule->m_GetNickNameSuccess, this, &UUserHead::OnGetUserHeadResponseMsg);

	return true;
}

void UUserHead::OnDownloadImage(bool _isScuccess, int32 _penetrateId, FString _url, UTexture2D * _texture2d,UDownloadImageTool * _downloadTool)
{
	m_ImageUserHead->GetDynamicMaterial()->SetTextureParameterValue("headTexture", _texture2d);

	m_ImageUserHead->Brush.ImageSize = FVector2D(120.0f, 120.0f);

	BaseLogicScene * mScene = UGameDirector::GetInstance()->GetCurrentLogicScene();

	UIController*  mController = mScene->GetCurrentUIController();

	mController->GetTextureManager()->AddTexture2D(_url, _texture2d);
}

void UUserHead::OnInit()
{

}

void UUserHead::OnRelease()
{
	if (nullptr != m_DownloadImageTool)
	{
		m_DownloadImageTool->CancelDownload();

		m_DownloadImageTool = nullptr;
	}

	UUIManager::GetInstance()->RemoveButtonClick(m_BtnUserHead);

	LoginModule *mLoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, mLoginModule->m_GetNickNameSuccess, this, &UUserHead::OnGetUserHeadResponseMsg);
}

void UUserHead::OnSetParams(int32 _pid, FString & _content, ...)
{

}

void UUserHead::OnTick(float InDeltaTimeSecond)
{

}

void UUserHead::OpenUserHead(int32 _pid)
{
	UE_LOG(LogTemp, Log, TEXT("！！！！点击头像"));

	//	打开个人设置页面
	LogicMainScene* m_MainScene = (LogicMainScene *)UGameDirector::GetInstance()->GetCurrentLogicScene();

	MainUIController*  MainCon = (MainUIController*)(m_MainScene->GetCurrentUIController());

	MainCon->ChangePageTo(PAGE_TYPE::PersonInfo_PAGE);
}

void UUserHead::OnGetUserHeadResponseMsg(msg_ptr _msg)
{
	LoginModule *mLoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	FString mImageUrl = mLoginModule->GetUserData()->GetUserAvatar();

	UE_LOG(LogTemp, Log, TEXT("MainUMG头像图片：=%s"), *mImageUrl);
	
	//	判断缓存中是否存在.
	BaseLogicScene * mScene = UGameDirector::GetInstance()->GetCurrentLogicScene();

	UIController*  mController = mScene->GetCurrentUIController();

	UTexture2D * mTexture = mController->GetTextureManager()->GetTexture2d(mImageUrl);

	if (mTexture == nullptr)
	{
		if (mImageUrl.Len() > 0)
		{
			if (m_DownloadImageTool != nullptr)
			{
				m_DownloadImageTool->CancelDownload();
			}
			m_DownloadImageTool = UDownloadImageTool::DownloadImage(1, mImageUrl, this, &UUserHead::OnDownloadImage);
		}
	}
	else
	{
		m_ImageUserHead->GetDynamicMaterial()->SetTextureParameterValue("headTexture", mTexture);

		m_ImageUserHead->Brush.ImageSize = FVector2D(120.0f, 120.0f);
	}
}

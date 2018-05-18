// Fill out your copyright notice in the Description page of Project Settings.

#include "SelectCell.h"
#include "UIManager.h"
#include "Editor.h"
#include "GameDirector.h"
#include "LogicMainScene.h"

USelectCell::USelectCell(const FObjectInitializer& ObjectInitializer)
{
	m_Btn_Select=nullptr;
	m_Image_ICON=nullptr;
	m_DownloadImageTool = nullptr;
	m_IconUrl = NULL;
}

bool USelectCell::OnStart()
{
	if (!m_downloadSuccess)
	{
		DownloadIconImage();
	}

	return true;
}

void USelectCell::OnInit()
{
	m_isInit = true;
	m_downloadSuccess = false;
	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("Btn_Select")))
	{
		m_Btn_Select = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_Select, this, &USelectCell::OnButtonClick);
	}
	if (UImage *mImage = Cast<UImage>(GetWidgetFromName("ICON")))
	{
		m_Image_ICON = mImage;
	}

	if (m_IconUrl.Len() > 0)
		DownloadIconImage();
}

void USelectCell::OnRelease()
{
	m_isInit = false;
	if (m_downloadSuccess && nullptr != m_Image_ICON && nullptr != m_Image_ICON->Brush.GetResourceObject())
	{
		UTexture2D * mTexture = (UTexture2D *)m_Image_ICON->Brush.GetResourceObject();

		m_downloadSuccess = false;

		mTexture->ReleaseResource();
	}

	m_Btn_Select = nullptr;
	m_Image_ICON = nullptr;
	
	
	m_IconUrl = nullptr;
	if (m_DownloadImageTool!=nullptr)
	{
		m_downloadSuccess = false;
		m_DownloadImageTool->CancelDownload();
	}
	m_DownloadImageTool = nullptr;
}

void USelectCell::OnSetParams(int32 _pid, FString & _content, ...)
{

}

void USelectCell::OnTick(float InDeltaTimeSecond)
{

}

void USelectCell::SetIconURL(const FString _Url)
{
	m_IconUrl = _Url;
}

void USelectCell::SetTid(int32 _Tid)
{
	m_Tid = _Tid;
}

void USelectCell::SetRid(int32 _Rid)
{
	m_Rid = _Rid;
}

void USelectCell::SetDesignType(EnumDesignType _type)
{
	m_EnumDesignType = _type;
}

void USelectCell::DownloadIconImage()
{
	if (m_DownloadImageTool != nullptr)
	{
		m_downloadSuccess = false;
		m_DownloadImageTool->CancelDownload();
	}
	m_DownloadImageTool = UDownloadImageTool::DownloadImage(1, m_IconUrl, this, &USelectCell::OnDownoloadFinished);
}

void USelectCell::OnDownoloadFinished(bool _isScuccess, int32 _penetrateId, FString _url, UTexture2D * _texture2d, UDownloadImageTool * _downloadTool)
{
	if (_texture2d->IsValidLowLevel() && m_Image_ICON->IsValidLowLevel())
	{
		m_downloadSuccess = true;

		if (nullptr != m_Image_ICON)
		{
			m_Image_ICON->SetBrushFromTexture(_texture2d);
		}
	}
}

void USelectCell::OnButtonClick(int32 _Param)
{
	LogicMainScene *mLogicMainScene=(LogicMainScene *)UGameDirector::GetInstance()->GetCurrentLogicScene();

	mLogicMainScene->BeginJoinDesignLevel(m_Rid,m_Tid,m_EnumDesignType);
}

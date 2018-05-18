#include "StyleSpecialIcon.h"
#include "UIManager.h"
#include "GameDirector.h"
#include "GSchedulerManager.h"
#include "LogicMainScene.h"
#include "MainModule.h"
#include "GModuleManager.h"
#include "HomeInfoManager.h"
#include "StyleSpecialItem.h"


UStyleSpecialIcon::UStyleSpecialIcon(const FObjectInitializer& ObjectInitializer)
{
	m_Btn_Select = nullptr;
	m_Image_ICON = nullptr;
	m_DownloadImageTool = nullptr;
	m_IconUrl = NULL;
	m_DesignWidth = 1224.f;
}

bool UStyleSpecialIcon::OnStart()
{

	return true;
}

void UStyleSpecialIcon::OnInit()
{
	
	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("Btn_Select")))
	{
		m_Btn_Select = mButton;

		m_ButtnSlot = (UCanvasPanelSlot *)m_Btn_Select->Slot;
		

		UUIManager::GetInstance()->RegisterButtonClick(100, m_Btn_Select, this, &UStyleSpecialIcon::OnButtonClick);
	}
	if (UImage *mImage = Cast<UImage>(GetWidgetFromName("ICON")))
	{
		m_Image_ICON = mImage;
	}
	
	if (UCanvasPanel * mCanvasPanel = Cast<UCanvasPanel>(GetWidgetFromName("IconPanel")))
	{
		m_CanvasPanelSlot = (UCanvasPanelSlot *)mCanvasPanel->Slot;
	}
}

void UStyleSpecialIcon::OnRelease()
 {
	Super::OnRelease();
	
	//m_Image_ICON = nullptr;
	UUIManager::GetInstance()->RemoveButtonClick(m_Btn_Select);
	m_Btn_Select = nullptr;

	m_IconUrl = NULL;
	if (m_DownloadImageTool != nullptr)
	{
		m_DownloadImageTool->CancelDownload();
	}
	m_DownloadImageTool = nullptr;
}

void UStyleSpecialIcon::OnSetParams(int32 _pid, FString & _content, ...)
{

}

void UStyleSpecialIcon::OnTick(float InDeltaTimeSecond)
{

}
void UStyleSpecialIcon::OnClick(int32 index)
{
}


void UStyleSpecialIcon::SetIconURL(const FString _Url)
{
	m_IconUrl = _Url;
}
void UStyleSpecialIcon::SetContentId(FString _id)
{
	m_ContentId = _id;
}

void UStyleSpecialIcon::SetDesignType(EnumDesignType _type)
{
	m_EnumDesignType = _type;
}
void UStyleSpecialIcon::SetDesignWidth(float _width)
{
	m_DesignWidth = _width;
}
void UStyleSpecialIcon::DownloadIconImage()
{
	if (m_DownloadImageTool != nullptr)
	{
		m_DownloadImageTool->CancelDownload();
	}
	m_DownloadImageTool = UDownloadImageTool::DownloadImage(1, m_IconUrl, this, &UStyleSpecialIcon::OnDownoloadFinished);
}

void UStyleSpecialIcon::OnDownoloadFinished(bool _isScuccess, int32 _penetrateId, FString _url, UTexture2D * _texture2d, UDownloadImageTool * _downloadTool)
{
	if (_texture2d->IsValidLowLevel())
	{
		float mWidth = _texture2d->GetSizeX();
		float mHigh = _texture2d->GetSizeY();
		const float mScale = m_DesignWidth / mWidth;
		mWidth = mWidth * mScale;
		mHigh = mHigh * mScale;

		m_CanvasPanelSlot->SetOffsets(FMargin(0, 0, mWidth, mHigh));
		m_ButtnSlot->SetOffsets(FMargin(0, 0, mWidth, mHigh));

		m_Image_ICON->SetBrushFromTexture(_texture2d);
	}
}
void UStyleSpecialIcon::OnButtonClick(int32 _Param)
{
	if(!m_ContentId.IsEmpty())
		GSchedulerManager::GetInstance()->AddSchedulerInfo(m_ContentId);
}

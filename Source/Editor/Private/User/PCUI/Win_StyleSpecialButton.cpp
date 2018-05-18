

#include "Win_StyleSpecialButton.h"
#include "UIManager.h"
#include "GameDirector.h"
#include "LogicMainScene.h"
#include "Win_StyleSpecialItem.h"
#include "Win_RecommendStyleUMG.h"


UWin_StyleSpecialButton::UWin_StyleSpecialButton(const FObjectInitializer& ObjectInitializer)
{

}

bool UWin_StyleSpecialButton::OnStart()
{


	return true;
}

void UWin_StyleSpecialButton::OnInit()
{
	if (UButton * mButton = (UButton *)GetWidgetFromName(TEXT("ShowButton")) )
	{
		m_ShowButton = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(1, m_ShowButton, this, &UWin_StyleSpecialButton::OnButtonClick);
	}
	if (UImage * mImage = (UImage *)GetWidgetFromName(TEXT("ShowImage")))
	{
		m_ShowImage = mImage;
	}

	m_DownloadImageTool = nullptr;
	m_IsResponsing = false;


}

void UWin_StyleSpecialButton::OnRelease()
{
	Super::OnRelease();

	UUIManager::GetInstance()->RemoveButtonClick(m_ShowButton);

	if (m_DownloadImageTool != nullptr)
	{
		m_DownloadImageTool->CancelDownload();
	}
	m_DownloadImageTool = nullptr;

}

void UWin_StyleSpecialButton::OnSetParams(int32 _pid, FString & _content, ...)
{

}

void UWin_StyleSpecialButton::OnTick(float InDeltaTimeSecond)
{

}
void UWin_StyleSpecialButton::OnButtonClick(int32 _index)
{
	//UE_LOG(LogTemp, Log, TEXT("zhx : Style Special Button on Click"));
	if (!m_IsResponsing)
	{
		m_IsResponsing = true;
		HomeInfoManager::GetInstance()->GetDataFromURL(m_HomeInfo->m_contentText, this, &UWin_StyleSpecialButton::GetDataFinish);
	}
	
}
void UWin_StyleSpecialButton::SetLogicParent(UWin_RecommendStyleUMG * parent)
{
	m_Parent = parent;
}
void UWin_StyleSpecialButton::GetDataFinish(bool _finish)
{
	//UUIManager::GetInstance()->OpenUI(TEXT(""));
	m_IsResponsing = false;
	if (_finish)
	{
		m_Parent->RemoveFromViewport();
		UWin_StyleSpecialItem * mStyleSpecialItem = (UWin_StyleSpecialItem *)UUIManager::GetInstance()->OpenUI(TEXT("StyleSpecialItem"));
		mStyleSpecialItem->AddToViewport();
		
	}
	else
	{

	}

}
void UWin_StyleSpecialButton::InitWithData(HomeInfoT * _data)
{
	m_HomeInfo = _data;
}

void UWin_StyleSpecialButton::DownloadImage()
{
	FString mUrl = m_HomeInfo->m_contentImg;
	if (m_DownloadImageTool != nullptr)
	{
		m_DownloadImageTool->CancelDownload();
	}
	m_DownloadImageTool = UDownloadImageTool::DownloadImage(1, mUrl, this, &UWin_StyleSpecialButton::OnDownoloadFinished);
}

void UWin_StyleSpecialButton::OnDownoloadFinished(bool _isScuccess, int32 _penetrateId, FString _url, UTexture2D * _texture2d, UDownloadImageTool * _downloadTool)
{
	if (_texture2d->IsValidLowLevel())
	{
		m_ShowImage->SetBrushFromTexture(_texture2d);
	}
}
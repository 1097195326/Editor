

#include "StyleSpecialButton.h"
#include "UIManager.h"
#include "GameDirector.h"
#include "LogicMainScene.h"
#include "StyleSpecialItem.h"
#include "RecommendStyleUMG.h"


UStyleSpecialButton::UStyleSpecialButton(const FObjectInitializer& ObjectInitializer)
{

}

bool UStyleSpecialButton::OnStart()
{


	return true;
}

void UStyleSpecialButton::OnInit()
{
	if (UButton * mButton = (UButton *)GetWidgetFromName(TEXT("ShowButton")) )
	{
		m_ShowButton = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(1, m_ShowButton, this, &UStyleSpecialButton::OnButtonClick);
	}
	if (UImage * mImage = (UImage *)GetWidgetFromName(TEXT("ShowImage")))
	{
		m_ShowImage = mImage;
	}

	m_DownloadImageTool = nullptr;
	m_IsResponsing = false;


}

void UStyleSpecialButton::OnRelease()
{
	Super::OnRelease();

	UUIManager::GetInstance()->RemoveButtonClick(m_ShowButton);

	if (m_DownloadImageTool != nullptr)
	{
		m_DownloadImageTool->CancelDownload();
	}
	m_DownloadImageTool = nullptr;

}

void UStyleSpecialButton::OnSetParams(int32 _pid, FString & _content, ...)
{

}

void UStyleSpecialButton::OnTick(float InDeltaTimeSecond)
{

}
void UStyleSpecialButton::OnButtonClick(int32 _index)
{
	//UE_LOG(LogTemp, Log, TEXT("zhx : Style Special Button on Click"));
	if (!m_IsResponsing)
	{
		m_IsResponsing = true;
		HomeInfoManager::GetInstance()->GetDataFromURL(m_HomeInfo->m_contentText, this, &UStyleSpecialButton::GetDataFinish);
	}
	
}
void UStyleSpecialButton::SetLogicParent(URecommendStyleUMG * parent)
{
	m_Parent = parent;
}
void UStyleSpecialButton::GetDataFinish(bool _finish)
{
	//UUIManager::GetInstance()->OpenUI(TEXT(""));
	m_IsResponsing = false;
	if (_finish)
	{
		m_Parent->RemoveFromViewport();
		UStyleSpecialItem * mStyleSpecialItem = (UStyleSpecialItem *)UUIManager::GetInstance()->OpenUI(TEXT("StyleSpecialItem"));
		mStyleSpecialItem->AddToViewport();
		
	}
	else
	{

	}

}
void UStyleSpecialButton::InitWithData(HomeInfoT * _data)
{
	m_HomeInfo = _data;
}

void UStyleSpecialButton::DownloadImage()
{
	FString mUrl = m_HomeInfo->m_contentImg;
	if (m_DownloadImageTool != nullptr)
	{
		m_DownloadImageTool->CancelDownload();
	}
	m_DownloadImageTool = UDownloadImageTool::DownloadImage(1, mUrl, this, &UStyleSpecialButton::OnDownoloadFinished);
}

void UStyleSpecialButton::OnDownoloadFinished(bool _isScuccess, int32 _penetrateId, FString _url, UTexture2D * _texture2d, UDownloadImageTool * _downloadTool)
{
	if (_texture2d->IsValidLowLevel())
	{
		m_ShowImage->SetBrushFromTexture(_texture2d);
	}
}
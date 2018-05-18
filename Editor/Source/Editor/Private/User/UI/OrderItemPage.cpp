// Fill out your copyright notice in the Description page of Project Settings.

#include "OrderItemPage.h"
#include "HaoshaSDKBPLibrary.h"
#include "GModuleManager.h"
#include "GLanguageTool.h"
#include "UIManager.h"

UOrderItemPage::UOrderItemPage(const FObjectInitializer& ObjectInitializer)
{
	//	初始化.
	m_BtnOrder = nullptr;
	m_TextOrder = nullptr;
	m_ImagePiece = nullptr;

	m_ImageUrl = nullptr;
	m_ShowText = nullptr;
	m_OrderUrl = nullptr;
}

bool UOrderItemPage::OnStart()
{
	Super::OnStart();

	m_LoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	//	下单生产按钮--点击事件.
	if (UButton* BtnOrder = Cast<UButton>(GetWidgetFromName("Button_OrderItem")))
	{
		m_BtnOrder = BtnOrder;

		UUIManager::GetInstance()->RegisterButtonClick(1, m_BtnOrder, this, &UOrderItemPage::OrderItem);
	}

	//	获取汉字.
	if (UTextBlock* TextOrder = Cast<UTextBlock>(GetWidgetFromName("Text_Order")))
	{
		m_TextOrder = TextOrder;
	}

	//	订单图片.
	if (UImage * ImgOrder = Cast<UImage>(GetWidgetFromName("Image_Piece")))
	{
		m_ImagePiece = ImgOrder;
	}

	return true;
}

void UOrderItemPage::OnInit()
{

}

void UOrderItemPage::OnRelease()
{
	UUIManager::GetInstance()->RemoveButtonClick(m_BtnOrder);

	if (nullptr != m_DownloadImageTool)
	{
		m_DownloadImageTool->CancelDownload();

		m_DownloadImageTool = nullptr;
	}

	if (nullptr != m_ImagePiece && nullptr != m_ImagePiece->Brush.GetResourceObject())
	{
		UTexture2D * mTexture = (UTexture2D *)m_ImagePiece->Brush.GetResourceObject();

		mTexture->ReleaseResource();
	}
}

void UOrderItemPage::OnSetParams(int32 _pid, FString & _content, ...)
{

}

void UOrderItemPage::OnTick(float InDeltaTimeSecond)
{

}

void UOrderItemPage::SetImageUrl(FString _ImageUrl)
{
	m_ImageUrl = _ImageUrl;
}

void UOrderItemPage::SetButtonText(FString _ShowText)
{
	m_TextOrder->SetText(FText::FromString(_ShowText));
}

void UOrderItemPage::SetExtra(FString _Extra)
{
	m_OrderUrl = _Extra;
}

void UOrderItemPage::DownLoadIconImage()
{
	if (m_DownloadImageTool!=nullptr)
	{
		m_DownloadImageTool->CancelDownload();
	}
	m_DownloadImageTool = UDownloadImageTool::DownloadImage(1, m_ImageUrl, this, &UOrderItemPage::OnDownoloadFinished);
}

void UOrderItemPage::OnDownoloadFinished(bool _isScuccess, int32 _penetrateId, FString _url, UTexture2D * _texture2d, UDownloadImageTool * _downloadTool)
{
	if (_texture2d->IsValidLowLevel())
	{
		m_ImagePiece->SetBrushFromTexture(_texture2d);
	}
}

void UOrderItemPage::OrderItem(int32 _pid)
{
	if (m_OrderUrl.IsEmpty())
		UUIManager::GetInstance()->OpenMiddleTip(TEXT("下单请致电客服4000667088"), 2);

	else
		UHaoshaSDKBPLibrary::OpenWebview(EditorFunctionalTool::GetTicketUrl(m_OrderUrl));
}
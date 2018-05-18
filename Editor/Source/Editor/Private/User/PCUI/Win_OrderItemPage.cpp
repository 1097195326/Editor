// Fill out your copyright notice in the Description page of Project Settings.

#include "Win_OrderItemPage.h"
#include "HaoshaSDKBPLibrary.h"
#include "GModuleManager.h"
#include "GLanguageTool.h"
#include "DesignModule.h"
#include "UIManager.h"

UWin_OrderItemPage::UWin_OrderItemPage(const FObjectInitializer& ObjectInitializer)
{
	//	初始化.
	m_BtnOrder = nullptr;
	m_TextOrder = nullptr;
	m_ImagePiece = nullptr;

	m_ImageUrl = nullptr;
	m_ShowText = nullptr;
	m_OrderUrl = nullptr;
}

bool UWin_OrderItemPage::OnStart()
{
	m_LoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	//	下单生产按钮--点击事件.
	if (UButton* BtnOrder = Cast<UButton>(GetWidgetFromName("Button_OrderItem")))
	{
		m_BtnOrder = BtnOrder;

		UUIManager::GetInstance()->RegisterButtonClick(1, m_BtnOrder, this, &UWin_OrderItemPage::OrderItem);
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

	//	选择图片.
	if (UImage * ImgSelect = Cast<UImage>(GetWidgetFromName("Image_Select")))
	{
		m_ImageSelect = ImgSelect;
	}

	return true;
}

void UWin_OrderItemPage::OnInit()
{

}

void UWin_OrderItemPage::OnRelease()
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

void UWin_OrderItemPage::OnSetParams(int32 _pid, FString & _content, ...)
{

}

void UWin_OrderItemPage::OnTick(float InDeltaTimeSecond)
{

}

void UWin_OrderItemPage::SetImageUrl(FString _ImageUrl)
{
	m_ImageUrl = _ImageUrl;
}

void UWin_OrderItemPage::SetButtonText(FString _ShowText)
{
	m_TextOrder->SetText(FText::FromString(_ShowText));
}

void UWin_OrderItemPage::SetExtra(FString _Extra)
{
	m_OrderUrl = _Extra;
}

void UWin_OrderItemPage::DownLoadIconImage()
{
	if (m_DownloadImageTool!=nullptr)
	{
		m_DownloadImageTool->CancelDownload();
	}
	m_DownloadImageTool = UDownloadImageTool::DownloadImage(1, m_ImageUrl, this, &UWin_OrderItemPage::OnDownoloadFinished);
}

void UWin_OrderItemPage::OnDownoloadFinished(bool _isScuccess, int32 _penetrateId, FString _url, UTexture2D * _texture2d, UDownloadImageTool * _downloadTool)
{
	if (_texture2d->IsValidLowLevel())
	{
		m_ImagePiece->SetBrushFromTexture(_texture2d);
	}
}

void UWin_OrderItemPage::OrderItem(int32 _pid)
{
	//UHaoshaSDKBPLibrary::OpenWebview(EditorFunctionalTool::GetTicketUrl(m_OrderUrl));

	if (m_WorkStyle == false)
	{
		DesignModule * mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

		for (int32 i = 0; i < mDesignModule->m_OrderItemContent.Num(); ++i)
		{
			if (i == m_OrderIndex)
			{
				mDesignModule->m_SelectOrderIndex = m_OrderIndex;

				m_ImageSelect->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				mDesignModule->m_OrderItemContent[i]->m_ImageSelect->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}
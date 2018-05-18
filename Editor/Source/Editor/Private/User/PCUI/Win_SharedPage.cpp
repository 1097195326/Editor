// Fill out your copyright notice in the Description page of Project Settings.

#include "Win_SharedPage.h"
#include "UIManager.h"
#include "Win_WebPage.h"
#include "GameDirector.h"
#include "Win_DesignUMG.h"
#include "EditorDirector.h"
#include "GLanguageTool.h"
#include "GModuleManager.h"
#include "LogicDesignScene.h"
#include "Win_OrderItemPage.h"
#include "DesignUIController.h"

UWin_SharedPage::UWin_SharedPage(const FObjectInitializer& ObjectInitializer)
{
	m_BtnReturn = nullptr;
	m_BtnPage = nullptr;
	m_BtnDesign = nullptr;
	m_BtnFriend = nullptr;
	m_BtnCircle = nullptr;
	m_BtnProduce = nullptr;
	m_BtnBuyPiece = nullptr;

	m_BorderWork = nullptr;
	m_BorderPiece = nullptr;

	m_TextShare = nullptr;
	m_TextFriend = nullptr;
	m_TextCircle = nullptr;
	m_TextOrder = nullptr;
}

bool UWin_SharedPage::OnStart()
{
	//	返回设计页面.
	if (UButton* BtnReturn = Cast<UButton>(GetWidgetFromName("Button_Return")))
	{
		m_BtnReturn = BtnReturn;

		UUIManager::GetInstance()->RegisterButtonClick(1, m_BtnReturn, this, &UWin_SharedPage::SharedPage);
	}

	//	返回首页.
	if (UButton* BtnPage = Cast<UButton>(GetWidgetFromName("Button_Page")))
	{
		m_BtnPage = BtnPage;

		UUIManager::GetInstance()->RegisterButtonClick(2, m_BtnPage, this, &UWin_SharedPage::SharedPage);
	}

	//	作品image.
	if (UImage * ImgWork = Cast<UImage>(GetWidgetFromName("Image_Work")))
	{
		m_ImageWork = ImgWork;
	}

	//	继续设计.
	if (UButton* BtnDesign = Cast<UButton>(GetWidgetFromName("Button_Continue")))
	{
		m_BtnDesign = BtnDesign;

		UUIManager::GetInstance()->RegisterButtonClick(3, m_BtnDesign, this, &UWin_SharedPage::SharedPage);
	}

	//	分享给朋友.
	if (UButton* BtnFriend = Cast<UButton>(GetWidgetFromName("Button_Friend")))
	{
		m_BtnFriend = BtnFriend;

		UUIManager::GetInstance()->RegisterButtonClick(4, m_BtnFriend, this, &UWin_SharedPage::SharedPage);
	}

	//	分享到朋友圈.
	if (UButton* BtnCircle = Cast<UButton>(GetWidgetFromName("Button_Circle")))
	{
		m_BtnCircle = BtnCircle;

		UUIManager::GetInstance()->RegisterButtonClick(5, m_BtnCircle, this, &UWin_SharedPage::SharedPage);
	}

	//	下单列表.
	if (UBorder* BorderWork = Cast<UBorder>(GetWidgetFromName("Border_Work")))
	{
		m_BorderWork = BorderWork;
	}
	if (UGridPanel* GridPanelOrder = Cast<UGridPanel>(GetWidgetFromName("GridPanel_Work")))
	{
		m_GridPanelOrder = GridPanelOrder;

		m_GridPanelOrder->ClearChildren();
	}
	if (UBorder* BorderPiece = Cast<UBorder>(GetWidgetFromName("Border_Piece")))
	{
		m_BorderPiece = BorderPiece;
	}
	if (UGridPanel* GridPanelOrder = Cast<UGridPanel>(GetWidgetFromName("GridPanel_Piece")))
	{
		m_GridPanelPiece = GridPanelOrder;

		m_GridPanelPiece->ClearChildren();
	}

	//	下单生产按钮.
	if (UButton * mTemp = Cast<UButton>(GetWidgetFromName("Button_BuyWork")))
	{
		m_BtnProduce = mTemp;

		UUIManager::GetInstance()->RegisterButtonClick(31, m_BtnProduce, this, &UWin_SharedPage::OnWorkProduce);
	}

	//	立即购买按钮.
	if (UButton * mTemp = Cast<UButton>(GetWidgetFromName("Button_BuyPiece")))
	{
		m_BtnBuyPiece = mTemp;

		UUIManager::GetInstance()->RegisterButtonClick(31, m_BtnBuyPiece, this, &UWin_SharedPage::OnBuyPiece);
	}

	// 获取文字.
	if (UTextBlock* TextShare = Cast<UTextBlock>(GetWidgetFromName("Text_SharedContent")))
	{
		m_TextShare = TextShare;

		FString mTextShare = GLanguageTool::GetInstance()->GetLanguage(TEXT("key121"));

		m_TextShare->SetText(FText::FromString(mTextShare));
	}
	if (UTextBlock* TextFriend = Cast<UTextBlock>(GetWidgetFromName("Text_Friend")))
	{
		m_TextFriend = TextFriend;

		FString mTextFriend = GLanguageTool::GetInstance()->GetLanguage(TEXT("key122"));

		m_TextFriend->SetText(FText::FromString(mTextFriend));
	}
	if (UTextBlock* TextCircle = Cast<UTextBlock>(GetWidgetFromName("Text_Circle")))
	{
		m_TextCircle = TextCircle;

		FString mTextCircle = GLanguageTool::GetInstance()->GetLanguage(TEXT("key123"));

		m_TextCircle->SetText(FText::FromString(mTextCircle));
	}
	if (UTextBlock* TextOrder = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_OrderList"))))
	{
		m_TextOrder = TextOrder;

		FString mTextOrder = GLanguageTool::GetInstance()->GetLanguage(TEXT("key124"));

		m_TextOrder->SetText(FText::FromString(mTextOrder));
	}

	//	请求服务器--分享数据.
	LogicDesignScene * mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();

	int32 _StyleID = mLogicDesignScene->m_savedStyleRid;

	DesignModule *mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

	mDesignModule->RequestShareFriendServer(_StyleID);

	//	监听本地事件--分享给朋友.
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, mDesignModule->m_ShareFriendSuccess, this, &UWin_SharedPage::OnShareFriendResponseMsg);

	//	监听本地事件--订单.
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, mDesignModule->m_OrderItemSuccess, this, &UWin_SharedPage::OnGetOrderListResponseMsg);

	return true;
}

void UWin_SharedPage::OnInit()
{

}

void UWin_SharedPage::OnRelease()
{
	UUIManager::GetInstance()->RemoveButtonClick(m_BtnReturn);
	UUIManager::GetInstance()->RemoveButtonClick(m_BtnPage);
	UUIManager::GetInstance()->RemoveButtonClick(m_BtnDesign);
	UUIManager::GetInstance()->RemoveButtonClick(m_BtnFriend);
	UUIManager::GetInstance()->RemoveButtonClick(m_BtnCircle);
	UUIManager::GetInstance()->RemoveButtonClick(m_BtnProduce);
	UUIManager::GetInstance()->RemoveButtonClick(m_BtnBuyPiece);

	DesignModule *mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, mDesignModule->m_ShareFriendSuccess, this, &UWin_SharedPage::OnShareFriendResponseMsg);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, mDesignModule->m_OrderItemSuccess, this, &UWin_SharedPage::OnGetOrderListResponseMsg);

	if (nullptr != m_DownloadImageTool)
		m_DownloadImageTool->CancelDownload();

	m_DownloadImageTool = nullptr;

	if (nullptr != m_DownloadImageTool1)
		m_DownloadImageTool1->CancelDownload();

	m_DownloadImageTool1 = nullptr;

	if (nullptr != m_ImageWork && nullptr != m_ImageWork->Brush.GetResourceObject())
	{
		UTexture2D * mTexture = (UTexture2D *)m_ImageWork->Brush.GetResourceObject();

		mTexture->ReleaseResource();
	}
}

void UWin_SharedPage::OnSetParams(int32 _pid, FString & _content, ...)
{

}

void UWin_SharedPage::OnTick(float InDeltaTimeSecond)
{

}

void UWin_SharedPage::SharedPage(int32 _pid)
{
	if (_pid == 1)
	{
		//	打开设计页面
		this->RemoveFromViewport();
	}

	if (_pid == 2)
	{
		this->RemoveFromViewport();

		//	挑到首页.
		LogicDesignScene * mDesignScene = (LogicDesignScene *)UGameDirector::GetInstance()->GetCurrentLogicScene();

		mDesignScene->FinishDesign();
	}

	if (_pid == 3)
	{
		//	打开设计页面.
		this->RemoveFromViewport();
	}

	if (_pid == 4)
	{
		//	分享到微信好友.
		OpenWechatPage(m_Title, m_Description, m_IconUrl, m_LinkUrl);
	}

	if (_pid == 5)
	{
		//	分享到微信朋友圈.
		OpenCirclePage(m_Title, m_Description, m_IconUrl, m_LinkUrl);
	}
}

//	收到服务器发送给本地分享朋友成功的消息.
void UWin_SharedPage::OnShareFriendResponseMsg(msg_ptr _msg)
{
	//换成本地消息.
	MsgLocal * mMsgLocal = _msg->ConvertMsg<MsgLocal>();

	//获取原数据.
	TSharedPtr<FJsonObject> *t_jsonObject = (TSharedPtr<FJsonObject>*)mMsgLocal->Content();

	FString mLink = t_jsonObject->ToSharedRef()->GetStringField("linkUrl");

	FString mLeft, mRight;

	mLink.Split("?", &mLeft, &mRight);

	FString mUrl = ((UEditorDirector*)(UGameDirector::GetInstance()))->GetServerUrl();
	
	m_LinkUrl = FString::Printf(TEXT("%ssoft/qrcode/share.jhtml?%s"), *mUrl, *mRight);

	m_Description = t_jsonObject->ToSharedRef()->GetStringField("description");

	m_IconUrl = t_jsonObject->ToSharedRef()->GetStringField("iconUrl");

	m_Title = t_jsonObject->ToSharedRef()->GetStringField("title");

	DownLoadIconImage();
}

//	打开微信.
void UWin_SharedPage::OpenWechatPage(FString _title, FString _description, FString _iconurl, FString _linkurl)
{
	//UHaoshaSDKBPLibrary::WeChatFavorite(_title, _description, _iconurl, _linkurl);
// 	UWin_WebPage * mTemp = (UWin_WebPage *)UUIManager::GetInstance()->OpenUI(TEXT("Web_Shared"));
// 
// 	mTemp->AddToViewport();
// 
// 	mTemp->m_WebBuy->LoadURL(m_LinkUrl);
	DownLoadIconImage1(m_LinkUrl);
}

void UWin_SharedPage::OpenCirclePage(FString _title, FString _description, FString _iconurl, FString _linkurl)
{
	//UHaoshaSDKBPLibrary::WeChatMoments(_title, _description, _iconurl, _linkurl);
// 	UWin_WebPage * mTemp = (UWin_WebPage *)UUIManager::GetInstance()->OpenUI(TEXT("Web_Shared"));
// 
// 	mTemp->AddToViewport();
// 
// 	mTemp->m_WebBuy->LoadURL(m_LinkUrl);
	DownLoadIconImage1(m_LinkUrl);
}

//	下载作品Image.
void UWin_SharedPage::DownLoadIconImage()
{
	if (m_DownloadImageTool != nullptr)
	{
		m_DownloadImageTool->CancelDownload();
	}
	m_DownloadImageTool = UDownloadImageTool::DownloadImage(1, m_IconUrl, this, &UWin_SharedPage::OnDownoloadFinished);
}

void UWin_SharedPage::DownLoadIconImage1(FString _url)
{
	if (m_DownloadImageTool1 != nullptr)
	{
		m_DownloadImageTool1->CancelDownload();
	}
	m_DownloadImageTool1 = UDownloadImageTool::DownloadImage(1, _url, this, &UWin_SharedPage::OnDownoloadFinished1);

}

void UWin_SharedPage::OnDownoloadFinished(bool _isScuccess, int32 _penetrateId, FString _url, UTexture2D * _texture2d, UDownloadImageTool * _downloadTool)
{
	if (_texture2d->IsValidLowLevel())
	{
		m_ImageWork->SetBrushFromTexture(_texture2d);
	}

	if (_isScuccess == false)
		DownLoadIconImage();
}

void UWin_SharedPage::OnDownoloadFinished1(bool _isScuccess, int32 _penetrateId, FString _url, UTexture2D * _texture2d, UDownloadImageTool * _downloadTool)
{
	if (_texture2d->IsValidLowLevel())
	{
		UWin_WebPage * mTemp = (UWin_WebPage *)UUIManager::GetInstance()->OpenUI(TEXT("Web_Shared"));

		mTemp->AddToViewport();

		mTemp->m_ImageWeb->SetBrushFromTexture(_texture2d);
	}
}

//	创建订单界面.
void UWin_SharedPage::CreateOrderItem(int32 _num,bool _canBuy)
{
	m_GridPanelOrder->ClearChildren();

	m_GridPanelPiece->ClearChildren();

	m_OrderItemContent.Empty();

	m_PieceItemContent.Empty();

	bool mCanBuy = _canBuy;

	for (int32 i = 0; i < _num; ++i)
	{
		if (mCanBuy == true)
		{
			if (i == 0)
			{
				UWin_OrderItemPage *mTemp = (UWin_OrderItemPage *)UUIManager::GetInstance()->OpenUI(TEXT("UI_OrderItem"));

				m_GridPanelOrder->AddChildToGrid(mTemp);

				m_OrderItemContent.Add(mTemp);

				mCanBuy = false;

				mTemp->m_WorkStyle = true;	//	设置是成衣.

				mTemp->m_ImageSelect->SetVisibility(ESlateVisibility::Visible);	//将选中图显示.
			}
		}
		else
		{
			UWin_OrderItemPage *mTemp = (UWin_OrderItemPage *)UUIManager::GetInstance()->OpenUI(TEXT("UI_OrderItem"));

			UGridSlot * mGridSlot = m_GridPanelPiece->AddChildToGrid(mTemp);

			mGridSlot->SetRow(0);

			mGridSlot->SetColumn(i);

			mTemp->SetPadding(FMargin(0.0f, 0.0f, ORDER_ITEM_CONTENT_RIGHT_PADDING, 0.0f));

			m_OrderItemContent.Add(mTemp);

			m_PieceItemContent.Add(mTemp);

			if ((_canBuy == true && i == 1) || _canBuy == false && i == 0)
				mTemp->m_ImageSelect->SetVisibility(ESlateVisibility::Visible);
		}
	}
	DesignModule * mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

	mDesignModule->m_OrderItemContent = m_PieceItemContent;
}

void UWin_SharedPage::OnWorkProduce(int32 _pid)
{
	if (m_BuyWorkUrl.IsEmpty())
	{
		FString mText = GLanguageTool::GetInstance()->GetLanguage(TEXT("key151"));

		UUIManager::GetInstance()->OpenMiddleTip2(mText, 2);
	}
	else
	{
// 		UWin_WebPage * mTemp = (UWin_WebPage *)UUIManager::GetInstance()->OpenUI(TEXT("Web_Shared"));
// 
// 		mTemp->AddToViewport();
// 
// 		mTemp->m_WebBuy->LoadURL(m_BuyWorkUrl);
		DownLoadIconImage1(m_BuyWorkUrl);
	}
}

void UWin_SharedPage::OnBuyPiece(int32 _pid)
{
// 	UWin_WebPage * mTemp = (UWin_WebPage *)UUIManager::GetInstance()->OpenUI(TEXT("Web_Shared"));
// 
// 	mTemp->AddToViewport();
// 
// 	DesignModule * mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");
// 
// 	mTemp->m_WebBuy->LoadURL(m_OrderUrlArray[mDesignModule->m_SelectOrderIndex]);
	DesignModule * mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

	DownLoadIconImage1(m_OrderUrlArray[mDesignModule->m_SelectOrderIndex]);
}

//	收到服务器发给本地的订单.
void UWin_SharedPage::OnGetOrderListResponseMsg(msg_ptr _msg)
{
	//换成本地消息.
	MsgLocal * mMsgLocal = _msg->ConvertMsg<MsgLocal>();

	//获取原数据.
	TArray<OrderStruct> * mStructArray = (TArray<OrderStruct> *)mMsgLocal->Content();

	bool mCanBuyStyle = true;

	if (!(*mStructArray)[0].m_buttonText.Equals(TEXT("下单生产")))
	{
		mCanBuyStyle = false;

		m_BorderWork->SetVisibility(ESlateVisibility::Hidden);
	}

	else
	{
		m_BorderWork->SetVisibility(ESlateVisibility::Visible);
	}

	CreateOrderItem(mStructArray->Num(),mCanBuyStyle);

	for (int32 i = 0; i < mStructArray->Num(); i++)
	{
		OrderStruct mStruct = (*mStructArray)[i];

		m_OrderItemContent[i]->SetImageUrl(mStruct.m_extra);

		m_OrderItemContent[i]->DownLoadIconImage();

		m_OrderItemContent[i]->SetButtonText(mStruct.m_buttonText);

		//	设置购买成品链接.
		if (mCanBuyStyle == true && i == 0)
		{
			FString mLeft, mRight;

			mStruct.m_iconUrl.Split("?", &mLeft, &mRight);

			FString mUrl = ((UEditorDirector*)(UGameDirector::GetInstance()))->GetServerUrl();

			m_BuyWorkUrl = FString::Printf(TEXT("%ssoft/qrcode/style.jhtml?%s"), *mUrl, *mRight);

			//m_BuyWorkUrl = "http://test-h5.fafaapp.com/soft/qrcode/style.jhtml?" + mRight;
		}

		else
		{
			FString mLeft, mRight, mOrderUrl;

			mStruct.m_iconUrl.Split("?", &mLeft, &mRight);

			FString mUrl = ((UEditorDirector*)(UGameDirector::GetInstance()))->GetServerUrl();

			mOrderUrl = FString::Printf(TEXT("%ssoft/qrcode/goods.jhtml?%s"), *mUrl, *mRight);

			//mOrderUrl = "http://test-h5.fafaapp.com/soft/qrcode/goods.jhtml?" + mRight;

			m_OrderItemContent[i]->SetExtra(mOrderUrl);

			if (mCanBuyStyle == true)
				m_OrderItemContent[i]->m_OrderIndex = i - 1;
			else
				m_OrderItemContent[i]->m_OrderIndex = i;

			m_OrderUrlArray.Add(mOrderUrl);
		}
	}
}

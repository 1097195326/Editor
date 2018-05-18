// Fill out your copyright notice in the Description page of Project Settings.

#include "SharedPage.h"
#include "UIManager.h"
#include "GameDirector.h"
#include "OrderItemPage.h"
#include "GLanguageTool.h"
#include "GModuleManager.h"
#include "LogicDesignScene.h"
#include "DesignUIController.h"

USharedPage::USharedPage(const FObjectInitializer& ObjectInitializer)
{
	m_BtnReturn = nullptr;
	m_BtnPage = nullptr;
	m_BtnDesign = nullptr;
	m_BtnFriend = nullptr;
	m_BtnCircle = nullptr;

	m_TextShare = nullptr;
	m_TextFriend = nullptr;
	m_TextCircle = nullptr;
	m_TextOrder = nullptr;
}

bool USharedPage::OnStart()
{
	Super::OnStart();


	//	返回设计页面.
	if (UButton* BtnReturn = Cast<UButton>(GetWidgetFromName("Button_Return")))
	{
		m_BtnReturn = BtnReturn;

		UUIManager::GetInstance()->RegisterButtonClick(1, m_BtnReturn, this, &USharedPage::SharedPage);
	}

	//	返回首页.
	if (UButton* BtnPage = Cast<UButton>(GetWidgetFromName("Button_Page")))
	{
		m_BtnPage = BtnPage;

		UUIManager::GetInstance()->RegisterButtonClick(2, m_BtnPage, this, &USharedPage::SharedPage);
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

		UUIManager::GetInstance()->RegisterButtonClick(3, m_BtnDesign, this, &USharedPage::SharedPage);
	}

	//	分享给朋友.
	if (UButton* BtnFriend = Cast<UButton>(GetWidgetFromName("Button_Friend")))
	{
		m_BtnFriend = BtnFriend;

		UUIManager::GetInstance()->RegisterButtonClick(4, m_BtnFriend, this, &USharedPage::SharedPage);
	}

	//	分享到朋友圈.
	if (UButton* BtnCircle = Cast<UButton>(GetWidgetFromName("Button_Circle")))
	{
		m_BtnCircle = BtnCircle;

		UUIManager::GetInstance()->RegisterButtonClick(5, m_BtnCircle, this, &USharedPage::SharedPage);
	}

	//	下单列表.
	if (UGridPanel* GridPanelOrder = Cast<UGridPanel>(GetWidgetFromName("GridPanel_OrderList")))
	{
		m_GridPanelOrder = GridPanelOrder;

		m_GridPanelOrder->ClearChildren();
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
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, mDesignModule->m_ShareFriendSuccess, this, &USharedPage::OnShareFriendResponseMsg);

	//	监听本地事件--订单.
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, mDesignModule->m_OrderItemSuccess, this, &USharedPage::OnGetOrderListResponseMsg);

	return true;
}

void USharedPage::OnInit()
{

}

void USharedPage::OnRelease()
{
	UUIManager::GetInstance()->RemoveButtonClick(m_BtnReturn);
	UUIManager::GetInstance()->RemoveButtonClick(m_BtnPage);
	UUIManager::GetInstance()->RemoveButtonClick(m_BtnDesign);
	UUIManager::GetInstance()->RemoveButtonClick(m_BtnFriend);
	UUIManager::GetInstance()->RemoveButtonClick(m_BtnCircle);

	DesignModule *mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, mDesignModule->m_ShareFriendSuccess, this, &USharedPage::OnShareFriendResponseMsg);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, mDesignModule->m_OrderItemSuccess, this, &USharedPage::OnGetOrderListResponseMsg);

	if (nullptr != m_DownloadImageTool)
		m_DownloadImageTool->CancelDownload();

	m_DownloadImageTool = nullptr;

	if (nullptr != m_ImageWork && nullptr != m_ImageWork->Brush.GetResourceObject())
	{
		UTexture2D * mTexture = (UTexture2D *)m_ImageWork->Brush.GetResourceObject();

		mTexture->ReleaseResource();
	}
}

void USharedPage::OnSetParams(int32 _pid, FString & _content, ...)
{

}

void USharedPage::OnTick(float InDeltaTimeSecond)
{

}

void USharedPage::SharedPage(int32 _pid)
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
void USharedPage::OnShareFriendResponseMsg(msg_ptr _msg)
{
	//换成本地消息.
	MsgLocal * mMsgLocal = _msg->ConvertMsg<MsgLocal>();

	//获取原数据.
	TSharedPtr<FJsonObject> *t_jsonObject = (TSharedPtr<FJsonObject>*)mMsgLocal->Content();

	m_LinkUrl = t_jsonObject->ToSharedRef()->GetStringField("linkUrl");

	m_Description = t_jsonObject->ToSharedRef()->GetStringField("description");

	m_IconUrl = t_jsonObject->ToSharedRef()->GetStringField("iconUrl");

	m_Title = t_jsonObject->ToSharedRef()->GetStringField("title");

	DownLoadIconImage();
}

//	打开微信.
void USharedPage::OpenWechatPage(FString _title, FString _description, FString _iconurl, FString _linkurl)
{
	UHaoshaSDKBPLibrary::WeChatFavorite(_title, _description, _iconurl, _linkurl);
}

void USharedPage::OpenCirclePage(FString _title, FString _description, FString _iconurl, FString _linkurl)
{
	UHaoshaSDKBPLibrary::WeChatMoments(_title, _description, _iconurl, _linkurl);
}

//	下载作品Image.
void USharedPage::DownLoadIconImage()
{
	if (m_DownloadImageTool != nullptr)
	{
		m_DownloadImageTool->CancelDownload();
	}
	m_DownloadImageTool = UDownloadImageTool::DownloadImage(1, m_IconUrl, this, &USharedPage::OnDownoloadFinished);
}

void USharedPage::OnDownoloadFinished(bool _isScuccess, int32 _penetrateId, FString _url, UTexture2D * _texture2d, UDownloadImageTool * _downloadTool)
{
	if (_texture2d->IsValidLowLevel())
	{
		m_ImageWork->SetBrushFromTexture(_texture2d);
	}
}

//	创建订单界面.
void USharedPage::CreateOrderItem(int32 _num)
{
	m_GridPanelOrder->ClearChildren();

	m_OrderItemContent.Empty();

	for (int32 i = 0; i < _num; ++i)
	{
		UOrderItemPage *mTemp = (UOrderItemPage *)UUIManager::GetInstance()->OpenUI(TEXT("UI_OrderItem"));

		UGridSlot * mGridSlot = m_GridPanelOrder->AddChildToGrid(mTemp);

		mGridSlot->SetRow(i / 2);

		mGridSlot->SetColumn(i % 2);

		mTemp->SetPadding(FMargin(0.0f, 0.0f, ORDER_ITEM_CONTENT_RIGHT_PADDING, ORDER_ITEM_CONTENT_RIGHT_PADDING));

		m_OrderItemContent.Add(mTemp);
	}
}

//	收到服务器发给本地的订单.
void USharedPage::OnGetOrderListResponseMsg(msg_ptr _msg)
{
	//换成本地消息.
	MsgLocal * mMsgLocal = _msg->ConvertMsg<MsgLocal>();

	//获取原数据.
	TArray<OrderStruct> * mStructArray = (TArray<OrderStruct> *)mMsgLocal->Content();

	CreateOrderItem(mStructArray->Num());

	for (int32 i = 0; i < mStructArray->Num(); i++)
	{
		OrderStruct mStruct = (*mStructArray)[i];

		m_OrderItemContent[i]->SetImageUrl(mStruct.m_extra);

		m_OrderItemContent[i]->SetButtonText(mStruct.m_buttonText);

		m_OrderItemContent[i]->SetExtra(mStruct.m_iconUrl);

		m_OrderItemContent[i]->DownLoadIconImage();
	}
}

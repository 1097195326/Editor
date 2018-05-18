#include "MainUMG.h"
#include "UIManager.h"
#include "GameDirector.h"
#include "GModuleManager.h"
#include "MainUIController.h"
#include "GLanguageTool.h"
#include "SelectCell.h"
#include "MainUIController.h"


UMainUMG::UMainUMG(const FObjectInitializer& ObjectInitializer)
{
	m_Btn_Start=nullptr;
	m_Btn_Sport = nullptr;
	m_Btn_Yoga = nullptr;
	m_Btn_Shop = nullptr;
	m_Btn_Order = nullptr;
	m_Btn_PCDownload = nullptr;
	m_Btn_ShowAllMyStyle = nullptr;
	m_LogicMainScene = nullptr;
	m_Canv_Tips = nullptr;
}
bool UMainUMG::OnStart()
{
	bool mRet = false;
	
	m_LogicMainScene = (LogicMainScene *)UGameDirector::GetInstance()->GetCurrentLogicScene();

	LoginModule* mLoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	MainModule * mMainModule = GModuleManager::GetInstance()->GetModule<MainModule>("MainModule");

	//	本地监听消息回调(我的作品).
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, mMainModule->m_MyWorks_Content_LocalPid, this, &UMainUMG::OnMyWorksContentDataFinished);

	//	本地监听消息回调.
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, mLoginModule->m_GetNickNameSuccess, this, &UMainUMG::OnGetNickNameResponseMsg);

	//	本地监听消息回调.
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, mMainModule->m_PersonInfo_Back, this, &UMainUMG::OnPersonInfoBackFinished);

	//	请求服务器获取昵称和头像.
	mLoginModule->RequestNickName();

	OnActivationUI();

	return true;
}

void UMainUMG::OnInit()
{
	//初始化UI.
	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("Btn_Start")))
	{
		m_Btn_Start = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_Start, this, &UMainUMG::OnButtonStart);
	}
	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("Btn_Sport")))
	{
		m_Btn_Sport = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_Sport, this, &UMainUMG::OnButtonSport);
	}
	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("Btn_Yoga")))
	{
		m_Btn_Yoga = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_Yoga, this, &UMainUMG::OnButtonYoga);
	}

	//	商城.
	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("Btn_Shop")))
	{
		m_Btn_Shop = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_Shop, this, &UMainUMG::OnButtonShop);
	}
	//订单.
	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("Btn_Order")))
	{
		m_Btn_Order = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_Order, this, &UMainUMG::OnButtonOrder);
	}
	//PC版下载.
	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("Btn_PCDownload")))
	{
		m_Btn_PCDownload = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_PCDownload, this, &UMainUMG::OnButtonPCDownload);
	}
	//全部作品.
	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("Btn_ShowAllMyStyle")))
	{
		m_Btn_ShowAllMyStyle = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_ShowAllMyStyle, this, &UMainUMG::OnButtonShowAllMyStyle);
	}
	//提示.
	if (UCanvasPanel *mCanvas = Cast<UCanvasPanel>(GetWidgetFromName("Canv_Tip")))
	{
		m_Canv_Tips = mCanvas;
	}
	if (UTextBlock *mTextBlock = Cast<UTextBlock>(GetWidgetFromName("Text_Tip")))
	{
		m_Text_Tip = mTextBlock;
	}
	//昵称.
	if (UTextBlock * mTextUserName = Cast<UTextBlock>(GetWidgetFromName("Text_UserName")))
	{
		m_TextUserName = mTextUserName;
	}
	//我的作品.
	if (UHorizontalBox *mTemp = Cast<UHorizontalBox>(GetWidgetFromName("MyWorks_Content")))
	{
		m_HBox_MyWorksContent = mTemp;
	}
}

void UMainUMG::OnRelease()
{
	m_Btn_Start = nullptr;
	m_Btn_Sport = nullptr;
	m_Btn_Yoga = nullptr;
	m_Btn_Shop = nullptr;
	m_Btn_Order = nullptr;
	m_Btn_PCDownload = nullptr;
	m_Btn_ShowAllMyStyle = nullptr;
	m_LogicMainScene = nullptr;
	m_Canv_Tips = nullptr;
	m_TextUserName = nullptr;
	m_HBox_MyWorksContent = nullptr;

	LoginModule* mLoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	MainModule *mMainModule = GModuleManager::GetInstance()->GetModule<MainModule>("MainModule");

	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, mLoginModule->m_GetNickNameSuccess, this, &UMainUMG::OnGetNickNameResponseMsg);

	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, mMainModule->m_MyWorks_Content_LocalPid, this, &UMainUMG::OnMyWorksContentDataFinished);

	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, mMainModule->m_PersonInfo_Back, this, &UMainUMG::OnPersonInfoBackFinished);

}

void UMainUMG::OnSetParams(int32 _pid, FString & _content, ...)
{

}

void UMainUMG::OnTick(float InDeltaTimeSecond)
{
	Super::OnTick(InDeltaTimeSecond);
}

void UMainUMG::OnActivationUI()
{
	m_HBox_MyWorksContent->ClearChildren();
	GModuleManager::GetInstance()->GetModule<MainModule>("MainModule")->LoadMyWorksContentStyleData(2);
}

void UMainUMG::OnButtonStart(int32 _param)
{
	//进入推荐款界面.
	LogicMainScene* m_MainScene = (LogicMainScene *)UGameDirector::GetInstance()->GetCurrentLogicScene();

	MainUIController*  MainCon = (MainUIController*)(m_MainScene->GetCurrentUIController());

	MainCon->ChangePageTo(PAGE_TYPE::Recommend_PAGE);
}

void UMainUMG::OnButtonSport(int32 _param)
{
	//FString mMsgInfo = GLanguageTool::GetInstance()->GetLanguage(TEXT("key145"));

	//UUIManager::GetInstance()->OpenMiddleTip2(mMsgInfo, 1.5);
	LogicMainScene* m_MainScene = (LogicMainScene *)UGameDirector::GetInstance()->GetCurrentLogicScene();
	MainUIController*  MainCon = (MainUIController*)(m_MainScene->GetCurrentUIController());
	MainCon->ChangePageTo(PAGE_TYPE::Sport_Page);
}

void UMainUMG::OnButtonYoga(int32 _param)
{
	LoginModule* mLoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	if (nullptr != mLoginModule->GetUserData())
		UHaoshaSDKBPLibrary::OpenWebview(EditorFunctionalTool::GetTicketUrl(mLoginModule->GetUserData()->GetUseCourseURL()));
}

//	跳转商城H5.
void UMainUMG::OnButtonShop(int32 _param)
{
	LoginModule* mLoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	if (nullptr != mLoginModule->GetUserData())
		UHaoshaSDKBPLibrary::OpenWebview(EditorFunctionalTool::GetTicketUrl(mLoginModule->GetUserData()->GetShopURL()));
}

//	跳转订单H5.
void UMainUMG::OnButtonOrder(int32 _param)
{
	LoginModule* mLoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	if (nullptr != mLoginModule->GetUserData())
		UHaoshaSDKBPLibrary::OpenWebview(EditorFunctionalTool::GetTicketUrl(mLoginModule->GetUserData()->GetOrderURL()));
}

//	跳转下载H5.
void UMainUMG::OnButtonPCDownload(int32 _param)
{
	LoginModule* mLoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	if (nullptr != mLoginModule->GetUserData())
		UHaoshaSDKBPLibrary::OpenWebview(EditorFunctionalTool::GetTicketUrl(mLoginModule->GetUserData()->GetDownloadURL()));
}

//	进入全部作品界面.
void UMainUMG::OnButtonShowAllMyStyle(int32 _param)
{
	LogicMainScene* m_MainScene = (LogicMainScene *)UGameDirector::GetInstance()->GetCurrentLogicScene();

	MainUIController*  m_MainController = (MainUIController*)(m_MainScene->GetCurrentUIController());

	m_MainController->ChangePageTo(PAGE_TYPE::MyWork_PAGE);
}

void UMainUMG::ShowTip()
{
	//获得tip内容.
	FString mConfirm = GLanguageTool::GetInstance()->GetLanguage(TEXT("key145"));

	m_Text_Tip->SetText(FText::FromString(mConfirm));

	//显示tip.
	m_Canv_Tips->SetVisibility(ESlateVisibility::Visible);

	//设置定时器两秒后隐藏.
	GetWorld()->GetTimerManager().ClearTimer(TipTimerHandle);

	GetWorld()->GetTimerManager().SetTimer(TipTimerHandle, this, &UMainUMG::HiddenTip,2.0f, false);
}

void UMainUMG::HiddenTip()
{
	m_Canv_Tips->SetVisibility(ESlateVisibility::Hidden);
}

//	收到服务器发给本地的获取用户昵称的消息.
void UMainUMG::OnGetNickNameResponseMsg(msg_ptr _msg)
{
	LoginModule* mLoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	FString mNickName = mLoginModule->GetUserData()->GetVisualUserName();

	if (mNickName.Len() > 11)
	{
		int32 mLen = mNickName.Len() - 11;

		m_TextUserName->SetText(FText::FromString(mNickName.LeftChop(mLen) + "..."));
	}
	else
	{
		if (mNickName.Len() == 0)
			m_TextUserName->SetText(FText::FromString(mLoginModule->GetUserData()->GetNickName().LeftChop(3) + "..."));

		else
			m_TextUserName->SetText(FText::FromString(mNickName));
	}
}

//	创建我的作品cell.
void UMainUMG::CreateSelectCell(int32 _num)
{
	m_MyWorksContent.Empty();

	if (nullptr != this->m_HBox_MyWorksContent)
	{
		this->m_HBox_MyWorksContent->ClearChildren();

		for (int32 i = 0; i < _num; ++i)
		{
			USelectCell *mTemp = (USelectCell*)UUIManager::GetInstance()->OpenUI(TEXT("SelectCell_2"));

			this->m_HBox_MyWorksContent->AddChild(mTemp);

			mTemp->SetPadding(FMargin(0.0, 0.0f, 20.f, 0.0f));

			m_MyWorksContent.Add(mTemp);
		}
	}
}

void UMainUMG::OnMyWorksContentDataFinished(msg_ptr _msg)
{
	MainModule *mMainModule = GModuleManager::GetInstance()->GetModule<MainModule>("MainModule");

	TArray<SelectCellData> mSelectCellDatas = mMainModule->GetSelectCellDatas();

	//清空已有SelectCell.
	m_MyWorksContent.Empty();

	//创建新的数量的Cell.
	CreateSelectCell(mSelectCellDatas.Num());

	if (m_MyWorksContent.Num() >= mSelectCellDatas.Num())
	{
		for (int i = 0; i < mSelectCellDatas.Num(); ++i)
		{
			USelectCell *mSelectCell = m_MyWorksContent[i];

			mSelectCell->SetIconURL(mSelectCellDatas[i].m_IconUrl);

			mSelectCell->SetTid(mSelectCellDatas[i].m_Tid);

			mSelectCell->SetRid(mSelectCellDatas[i].m_Rid);

			mSelectCell->SetDesignType(EnumDesignType::DESIGN_BY_USER_STYLE);

			if (mSelectCell->m_IconUrl.Len() > 0)

				mSelectCell->DownloadIconImage();
		}
	}
}

void UMainUMG::OnPersonInfoBackFinished(msg_ptr _msg)
{
	LoginModule *mLoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	//	请求服务器获取昵称和头像.
	mLoginModule->RequestNickName();
}


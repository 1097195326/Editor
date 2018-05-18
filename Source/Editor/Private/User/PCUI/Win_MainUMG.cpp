#include "Win_MainUMG.h"
#include "Win_WebPage.h"
#include "UIManager.h"
#include "GameDirector.h"
#include "GLanguageTool.h"
#include "Win_DesignUMG.h"
#include "Win_SelectCell.h"
#include "GModuleManager.h"
#include "MainUIController.h"
#include "LogicDesignScene.h"
#include "MainUIController.h"


UWin_MainUMG::UWin_MainUMG(const FObjectInitializer& ObjectInitializer)
{
	m_Btn_Start=nullptr;
	m_Btn_Sport = nullptr;
	m_Btn_Yoga = nullptr;
	m_Btn_Shop = nullptr;
	m_Btn_Order = nullptr;
	m_Btn_Account = nullptr;
	m_Btn_PCDownload = nullptr;
	m_Btn_ShowAllMyStyle = nullptr;
	m_Btn_LoginOut = nullptr;
	m_LogicMainScene = nullptr;
	m_Canv_Tips = nullptr;
	m_TextUserName = nullptr;
	m_Grid_MyWorks = nullptr;
}
bool UWin_MainUMG::OnStart()
{
	bool mRet = false;

	m_LogicMainScene = (LogicMainScene *)UGameDirector::GetInstance()->GetCurrentLogicScene();

	LoginModule* mLoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	MainModule * mMainModule = GModuleManager::GetInstance()->GetModule<MainModule>("MainModule");

	//	本地监听消息回调(我的作品).
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, mMainModule->m_MyWorks_Content_LocalPid, this, &UWin_MainUMG::OnMyWorksContentDataFinished);

	//	本地监听消息回调(获取昵称).
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, mLoginModule->m_GetNickNameSuccess, this, &UWin_MainUMG::OnGetNickNameResponseMsg);

	//	请求服务器获取昵称和头像.
	mLoginModule->RequestNickName();

	OnActivationUI();

	return true;
}

void UWin_MainUMG::OnInit()
{
	//初始化UI.
	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("Btn_Start")))
	{
		m_Btn_Start = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_Start, this, &UWin_MainUMG::OnButtonStart);
	}
	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("Btn_Sport")))
	{
		m_Btn_Sport = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_Sport, this, &UWin_MainUMG::OnButtonSport);
	}
	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("Btn_Yoga")))
	{
		m_Btn_Yoga = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_Yoga, this, &UWin_MainUMG::OnButtonYoga);
	}
	//	商城.
	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("Btn_Shop")))
	{
		m_Btn_Shop = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_Shop, this, &UWin_MainUMG::OnButtonShop);
	}
	//订单.
	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("Btn_Order")))
	{
		m_Btn_Order = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_Order, this, &UWin_MainUMG::OnButtonOrder);
	}
	//账单.
	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("Btn_Account")))
	{
		m_Btn_Account = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_Account, this, &UWin_MainUMG::OnButtonAccount);
	}
	//PC版下载.
	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("Btn_PCDownload")))
	{
		m_Btn_PCDownload = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_PCDownload, this, &UWin_MainUMG::OnButtonPCDownload);
	}
	//所有作品.
	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("Btn_ShowAllMyStyle")))
	{
		m_Btn_ShowAllMyStyle = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_ShowAllMyStyle, this, &UWin_MainUMG::OnButtonShowAllMyStyle);
	}
	//注销.
	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("Button_LoginOut")))
	{
		m_Btn_LoginOut = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_LoginOut, this, &UWin_MainUMG::OnButtonLoginOut);
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
	if (UGridPanel *mTemp = Cast<UGridPanel>(GetWidgetFromName("GridPanel_MyStyles")))
	{
		m_Grid_MyWorks = mTemp;
	}
}

void UWin_MainUMG::OnRelease()
{
	m_Btn_Start = nullptr;
	m_Btn_Sport = nullptr;
	m_Btn_Yoga = nullptr;
	m_Btn_Shop = nullptr;
	m_Btn_Order = nullptr;
	m_Btn_PCDownload = nullptr;
	m_Btn_ShowAllMyStyle = nullptr;
	m_Btn_LoginOut = nullptr;
	m_LogicMainScene = nullptr;
	m_Canv_Tips = nullptr;
	m_TextUserName = nullptr;
	m_Grid_MyWorks = nullptr;

	UUIManager::GetInstance()->RemoveButtonClick(m_Btn_Start);
	UUIManager::GetInstance()->RemoveButtonClick(m_Btn_Sport);
	UUIManager::GetInstance()->RemoveButtonClick(m_Btn_Yoga);
	UUIManager::GetInstance()->RemoveButtonClick(m_Btn_Shop);
	UUIManager::GetInstance()->RemoveButtonClick(m_Btn_Order);
	UUIManager::GetInstance()->RemoveButtonClick(m_Btn_PCDownload);
	UUIManager::GetInstance()->RemoveButtonClick(m_Btn_ShowAllMyStyle);
	UUIManager::GetInstance()->RemoveButtonClick(m_Btn_LoginOut);

	LoginModule* mLoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	MainModule *mMainModule = GModuleManager::GetInstance()->GetModule<MainModule>("MainModule");

	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, mLoginModule->m_GetNickNameSuccess, this, &UWin_MainUMG::OnGetNickNameResponseMsg);

	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, mMainModule->m_MyWorks_Content_LocalPid, this, &UWin_MainUMG::OnMyWorksContentDataFinished);
}

void UWin_MainUMG::OnSetParams(int32 _pid, FString & _content, ...)
{
}

void UWin_MainUMG::OnTick(float InDeltaTimeSecond)
{
}

void UWin_MainUMG::OnActivationUI()
{
	m_Grid_MyWorks->ClearChildren();

	GModuleManager::GetInstance()->GetModule<MainModule>("MainModule")->LoadMyWorksContentStyleData(6);
}

void UWin_MainUMG::OnButtonStart(int32 _param)
{
	//进入推荐款界面.
	LogicMainScene* m_MainScene = (LogicMainScene *)UGameDirector::GetInstance()->GetCurrentLogicScene();

	MainUIController*  MainCon = (MainUIController*)(m_MainScene->GetCurrentUIController());

	MainCon->ChangePageTo(PAGE_TYPE::Recommend_PAGE);
}

void UWin_MainUMG::OnButtonSport(int32 _param)
{
	//FString mMsgInfo = GLanguageTool::GetInstance()->GetLanguage(TEXT("key145"));

	//UUIManager::GetInstance()->OpenMiddleTip2(mMsgInfo, 1.5);
 	LogicMainScene* m_MainScene = (LogicMainScene *)UGameDirector::GetInstance()->GetCurrentLogicScene();
 	MainUIController*  MainCon = (MainUIController*)(m_MainScene->GetCurrentUIController());
 	MainCon->ChangePageTo(PAGE_TYPE::Sport_Page);
}

void UWin_MainUMG::OnButtonYoga(int32 _param)
{
	LoginModule* mLoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	if (nullptr != mLoginModule->GetUserData())
		UHaoshaSDKBPLibrary::OpenWebview(EditorFunctionalTool::GetTicketUrl(mLoginModule->GetUserData()->GetUseCourseURL()));
}

//	跳转商城H5.
void UWin_MainUMG::OnButtonShop(int32 _param)
{
 	LoginModule* mLoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	if (nullptr != mLoginModule->GetUserData())
	{
		UWin_WebPage * mTemp = (UWin_WebPage *)UUIManager::GetInstance()->OpenUI(TEXT("Web_Fabric"));

		mTemp->AddToViewport();

		LogicDesignScene * mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();

		UWin_DesignUMG * mDesignUMG = nullptr;

		if (mLogicDesignScene != nullptr)
			mDesignUMG = (UWin_DesignUMG *)(mLogicDesignScene->GetCurrentUIController()->GetRootUI());

		if (nullptr != mDesignUMG)
		{
			mTemp->m_WebBuy->LoadURL(EditorFunctionalTool::GetTicketUrl(mLoginModule->GetUserData()->GetGoodsListUrl()));
		}
	}
}

//	跳转订单H5.
void UWin_MainUMG::OnButtonOrder(int32 _param)
{
// 	LoginModule* mLoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");
// 
// 	if (nullptr != mLoginModule->GetUserData())
// 		UHaoshaSDKBPLibrary::OpenWebview(EditorFunctionalTool::GetTicketUrl(mLoginModule->GetUserData()->GetOrderURL()));
}

void UWin_MainUMG::OnButtonAccount(int32 _param)
{
	FString mMsgInfo = GLanguageTool::GetInstance()->GetLanguage(TEXT("key145"));

	UUIManager::GetInstance()->OpenMiddleTip2(mMsgInfo, 1.5);
}

//	跳转下载H5.
void UWin_MainUMG::OnButtonPCDownload(int32 _param)
{
	LoginModule* mLoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	if (nullptr != mLoginModule->GetUserData())
		UHaoshaSDKBPLibrary::OpenWebview(EditorFunctionalTool::GetTicketUrl(mLoginModule->GetUserData()->GetDownloadURL()));
}

//	进入全部作品界面.
void UWin_MainUMG::OnButtonShowAllMyStyle(int32 _param)
{
	LogicMainScene* m_MainScene = (LogicMainScene *)UGameDirector::GetInstance()->GetCurrentLogicScene();

	MainUIController*  m_MainController = (MainUIController*)(m_MainScene->GetCurrentUIController());

	m_MainController->ChangePageTo(PAGE_TYPE::MyWork_PAGE);
}

void UWin_MainUMG::OnButtonLoginOut(int32 _param)
{
	LoginModule *mLoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	mLoginModule->LogOut();
}

void UWin_MainUMG::ShowTip()
{
	//获得tip内容.
	FString mConfirm = GLanguageTool::GetInstance()->GetLanguage(TEXT("key145"));

	m_Text_Tip->SetText(FText::FromString(mConfirm));

	//显示tip.
	m_Canv_Tips->SetVisibility(ESlateVisibility::Visible);

	//设置定时器两秒后隐藏.
	GetWorld()->GetTimerManager().ClearTimer(TipTimerHandle);

	GetWorld()->GetTimerManager().SetTimer(TipTimerHandle, this, &UWin_MainUMG::HiddenTip,2.0f, false);
}

void UWin_MainUMG::HiddenTip()
{
	m_Canv_Tips->SetVisibility(ESlateVisibility::Hidden);
}

//	收到服务器发给本地的获取用户昵称的消息.
void UWin_MainUMG::OnGetNickNameResponseMsg(msg_ptr _msg)
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
void UWin_MainUMG::CreateSelectCell(int32 _num)
{
	m_MyWorksContent.Empty();

	if (nullptr != this->m_Grid_MyWorks)
	{
		this->m_Grid_MyWorks->ClearChildren();

		for (int32 i = 0; i < _num; ++i)
		{
			UWin_SelectCell *mTemp = (UWin_SelectCell*)UUIManager::GetInstance()->OpenUI(TEXT("SelectCell_2"));

			mTemp->SetPadding(FMargin(0.0, 0.0f, 9.0f, 5.0f));

			UGridSlot * mGridSlot = this->m_Grid_MyWorks->AddChildToGrid(mTemp);

			mGridSlot->SetRow(i / 3);

			mGridSlot->SetColumn(i % 3);

			m_MyWorksContent.Add(mTemp);
		}
	}
}

//	收到服务器发给本地的获取我的作品的消息.
void UWin_MainUMG::OnMyWorksContentDataFinished(msg_ptr _msg)
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
			UWin_SelectCell *mSelectCell = m_MyWorksContent[i];

			mSelectCell->SetIconURL(mSelectCellDatas[i].m_IconUrl);

			mSelectCell->SetTid(mSelectCellDatas[i].m_Tid);

			mSelectCell->SetRid(mSelectCellDatas[i].m_Rid);

			mSelectCell->SetDesignType(EnumDesignType::DESIGN_BY_USER_STYLE);

			if (mSelectCell->m_IconUrl.Len() > 0)

				mSelectCell->DownloadIconImage();
		}
	}
}

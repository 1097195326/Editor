// Fill out your copyright notice in the Description page of Project Settings.

#include "LoginPageCell.h"
#include "TimerManager.h"
#include "UIManager.h"
#include "MsgCenter.h"
#include "Editor.h"
#include "GModuleManager.h"
#include "LoginModule.h"
#include "Userdata.h"
#include "UserInfo.h"
#include "EditorDirector.h"
#include "LogicLoginScene.h"
#include "HaoshaSDKBPLibrary.h"
#include "Editor/Public/User/Tools/EditorFunctionalTool.h"

ULoginPageCell::ULoginPageCell(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
		m_TextTip = nullptr;
		m_Login = nullptr;
		m_BtnLogin = nullptr;
		m_TextBoxPhone = nullptr;
		m_TextSecrte = nullptr;
	    m_BtnForgetPassWord = nullptr;
	    m_LoginTextCheck = nullptr;

	    m_FastRegister = nullptr;
		m_TextBoxPhoneReg = nullptr;
		m_TextBoxCheckNumber = nullptr;
	    m_BtnGetCheckReg = nullptr;
		m_BtnFastRegistLogin = nullptr;
		m_BtnPassWordLogin = nullptr;
        m_TextBlock_CountDownReg = nullptr;

												
		m_PassWordReset = nullptr;
		m_TextBlock_CountDownReset = nullptr;
	    m_EditableTextBox_PhoneReset = nullptr;
	    m_EditableTextBox_CheckedReset = nullptr;
		m_Button_GetCheckReset = nullptr;
	    m_EditableTextBox_PassWordReset = nullptr;
		m_Button_PassWordReset_Sure = nullptr;
		m_PassWordResetTextCheck = nullptr;

		m_FirstRegister = nullptr;

		m_AutoLogin = nullptr;
		CurrentPage = FAST_REGIST;
		//CurrentPage = LOGIN;
		CountDownReset = 0;
}

bool ULoginPageCell::OnStart()
{
	Super::OnStart();

	m_LoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	//	请求服务器获取h5路径.
	m_LoginModule->RequestGetHtmlUrlServer();

	//	本地监听消息回调.
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, m_LoginModule->m_GetHtmlUrlSuccess, this, &ULoginPageCell::OnGetHtmlResponseMsg);


	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, m_LoginModule->m_loginSucess, this, &ULoginPageCell::OnLoginResponseMsg);
	
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, m_LoginModule->m_FastLoginSucess, this, &ULoginPageCell::OnFastResponseMsg);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, m_LoginModule->m_CheckNumberSucess, this, &ULoginPageCell::OnCheckResponseMsg);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, m_LoginModule->m_PassWordCheckSucess, this, &ULoginPageCell::OnPassWordCheckResponseMsg);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, m_LoginModule->m_PassWordResetSucess, this, &ULoginPageCell::OnPassWordResponseMsg);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, 106, this, &ULoginPageCell::OnFirstLoginResponseMsg);

	if (UButton * BtnProtocol = Cast<UButton>(GetWidgetFromName("ButtonProtocol")))
	{
		m_Protocol = BtnProtocol;

		UUIManager::GetInstance()->RegisterButtonClick(31, m_Protocol, this, &ULoginPageCell::GetProtocol);
	}
	
	if (UTextBlock* TextTip = Cast<UTextBlock>(GetWidgetFromName("TipText")))
	{
		m_TextTip = TextTip;
	}
		
	if (UButton* BtnBack = Cast<UButton>(GetWidgetFromName("BtnBack")))
	{
		m_BtnBack = BtnBack;
	}
	m_BtnBack->OnClicked.AddDynamic(this, &ULoginPageCell::SendChangePage);
	//快速注册

	if (UCanvasPanel* FastRegister = Cast<UCanvasPanel>(GetWidgetFromName("FastRegister")))
	{
		m_FastRegister = FastRegister;
	}
	if (UEditableTextBox* TextBoxPhoneReg = Cast<UEditableTextBox>(GetWidgetFromName("EditableTextBox_PhoneReg")))
	{
		m_TextBoxPhoneReg = TextBoxPhoneReg;
	}
	if (UEditableTextBox* TextBoxCheckNumber = Cast<UEditableTextBox>(GetWidgetFromName("EditableTextBox_CheckedReg")))
	{
		m_TextBoxCheckNumber = TextBoxCheckNumber;
	}
	if (UButton* BtnGetCheckReg = Cast<UButton>(GetWidgetFromName("Button_GetCheckReg")))
	{
		m_BtnGetCheckReg = BtnGetCheckReg;
	}
	if (UButton* BtnFastRegistLogin = Cast<UButton>(GetWidgetFromName("FastRegistLogin")))
	{
		m_BtnFastRegistLogin = BtnFastRegistLogin;
	}
	if (UButton* BtnPassWordLogin = Cast<UButton>(GetWidgetFromName("PassWordLogin")))
	{
		m_BtnPassWordLogin = BtnPassWordLogin;
	}
	if (UTextBlock* TextBlock_CountDownReg = Cast<UTextBlock>(GetWidgetFromName("TextBlock_CountDownReg")))
	{
		m_TextBlock_CountDownReg = TextBlock_CountDownReg;
	}
	if (UCanvasPanel* ProtocolCanvasPanel = Cast<UCanvasPanel>(GetWidgetFromName("ProtocolCanvasPanel")))
	{
		m_ProtocolCanvasPanel = ProtocolCanvasPanel;
	}
	
	UUIManager::GetInstance()->RegisterButtonClick(1, m_BtnGetCheckReg, this, &ULoginPageCell::ClientButtonEvent);
	UUIManager::GetInstance()->RegisterButtonClick(2, m_BtnFastRegistLogin, this, &ULoginPageCell::ClientButtonEvent);
	
	m_BtnPassWordLogin->OnClicked.AddDynamic(this, &ULoginPageCell::LoginBtnCallback);
	
	//注冊第一次登陸.

	if (UEditableTextBox* EditableTextBox_Password0First = Cast<UEditableTextBox>(GetWidgetFromName("EditableTextBox_Password0First")))
	{
		m_EditableTextBox_Password0First = EditableTextBox_Password0First;
	}
	if (UEditableTextBox* EditableTextBox_Password1First = Cast<UEditableTextBox>(GetWidgetFromName("EditableTextBox_Password1First")))
	{
		m_EditableTextBox_Password1First = EditableTextBox_Password1First;
	}
	if (UButton* Button_CommitFirst = Cast<UButton>(GetWidgetFromName("Button_CommitFirst")))
	{
		m_Button_CommitFirst = Button_CommitFirst;
	}

	 m_Button_CommitFirst->OnClicked.AddDynamic(this, &ULoginPageCell::HandleFirstLogin);







	//密码登录.
	if (UCanvasPanel* Login = Cast<UCanvasPanel>(GetWidgetFromName("Login")))
	{
		m_Login = Login;
	}
	if (UEditableTextBox* Phone = Cast<UEditableTextBox>(GetWidgetFromName("EditableTextBox_PhoneLogin")))
	{
		m_TextBoxPhone = Phone;
	}
	if (UEditableTextBox* Password = Cast<UEditableTextBox>(GetWidgetFromName("EditableTextBox_PasswordLogin")))
	{
		m_TextSecrte = Password;
	}
	if (UButton* BtnLogin = Cast<UButton>(GetWidgetFromName("Button_2")))
	{
		m_BtnLogin = BtnLogin;
	}
	if (UButton* BtnForgetPassWord = Cast<UButton>(GetWidgetFromName("Button_Forget")))
	{
		m_BtnForgetPassWord = BtnForgetPassWord;
	}
	if (UTextBlock* LoginTextCheck = Cast<UTextBlock>(GetWidgetFromName("LoginTextCheck")))
	{
		m_LoginTextCheck = LoginTextCheck;
	}
	m_BtnLogin->OnClicked.AddDynamic(this, &ULoginPageCell::HandlePassWordLogin);
	m_BtnForgetPassWord->OnClicked.AddDynamic(this, &ULoginPageCell::GoPagePassWordReset);


	//UUIManager::GetInstance()->RegisterButtonClick(3, m_BtnLogin, this, &ULoginPageCell::ClientButtonEvent);

	//自动登录.
	if (UCanvasPanel* AutoLogin = Cast<UCanvasPanel>(GetWidgetFromName("AutoLogin")))
	{
		m_AutoLogin = AutoLogin;
	}

	//第一次注册.
	if (UCanvasPanel* FirstRegister = Cast<UCanvasPanel>(GetWidgetFromName("FirstRegister")))
	{
		m_FirstRegister = FirstRegister;
	}
	
	//忘记密码.
	if (UCanvasPanel* PassWordReset = Cast<UCanvasPanel>(GetWidgetFromName("PassWordReset")))
	{
		m_PassWordReset = PassWordReset;
	}
	if (UTextBlock* TextBlock_CountDownReset = Cast<UTextBlock>(GetWidgetFromName("TextBlock_CountDownReset")))
	{
		m_TextBlock_CountDownReset = TextBlock_CountDownReset;
	}
	if (UEditableTextBox* EditableTextBox_PhoneReset = Cast<UEditableTextBox>(GetWidgetFromName("EditableTextBox_PhoneReset")))
	{
		m_EditableTextBox_PhoneReset = EditableTextBox_PhoneReset;
	}
	if (UEditableTextBox* EditableTextBox_CheckedReset = Cast<UEditableTextBox>(GetWidgetFromName("EditableTextBox_CheckedReset")))
	{
		m_EditableTextBox_CheckedReset = EditableTextBox_CheckedReset;
	}
	if (UEditableTextBox* EditableTextBox_PassWordReset = Cast<UEditableTextBox>(GetWidgetFromName("EditableTextBox_PassWordReset")))
	{
		m_EditableTextBox_PassWordReset = EditableTextBox_PassWordReset;
	}
	if (UButton* Button_GetCheckReset = Cast<UButton>(GetWidgetFromName("Button_GetCheckReset")))
	{
		m_Button_GetCheckReset = Button_GetCheckReset;
	}
	if (UButton* Button_PassWordReset_Sure = Cast<UButton>(GetWidgetFromName("Button_PassWordReset_Sure")))
	{
		m_Button_PassWordReset_Sure = Button_PassWordReset_Sure;
	}
	if (UTextBlock* PassWordResetTextCheck = Cast<UTextBlock>(GetWidgetFromName("PassWordResetTextCheck")))
	{
		m_PassWordResetTextCheck = PassWordResetTextCheck;
	}
	m_Button_PassWordReset_Sure->OnClicked.AddDynamic(this, &ULoginPageCell::HandlePassWordReset);
	
	m_Button_GetCheckReset->OnClicked.AddDynamic(this, &ULoginPageCell::HandleCheckRest);
	

	AutoLogin();
	return true;
}

void ULoginPageCell::OnLoginResponseMsg(msg_ptr _msg)
{
	// 收到用户信息.开始自行处理.

	// 第一种方法.
	MsgLocal * mMsgLocal =  _msg->ConvertMsg<MsgLocal>();

	//mMsgLocal->Content();

	// 第二种那数据的方法.
	//FString mUserInfo = m_LoginModule->m_userInfo;

	//UE_LOG(GEditorLog, Error, TEXT("LoginPageCell userinfo= %s"), *mUserInfo);
	
	//FString mString = MsgJsonToString(_msg);

	FString msgString =  GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule")->GetMsgString();
	int32 msgCode = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule")->GetMsgCode();
	
	
	if (msgCode == MESSAGESUCCESS)
	{
		SetTip();   //todo  这里应该添加 登录成功创建下载界面

		UEditorDirector * mEditorDirector = ((UEditorDirector*)UGameDirector::GetInstance());

		mEditorDirector->InitExption();

		LogicLoginScene * currS = (LogicLoginScene*)UEditorDirector::GetInstance()->GetCurrentLogicScene();
		currS->JoinMainScene();
	}
	else
	{
       
		FString mIphone;
		FString mPassWord;
		if (UserInfo::Get()->ReadLocalData(mIphone, mPassWord))
		{
			ChangePage(EnumLoginPage::FAST_REGIST);
			SetTip(msgString);
			//SetTip(TEXT("自动登录失败"));
		}
		else
		{
			ChangePage(EnumLoginPage::LOGIN);
			SetTip(msgString);
		}
	}
	

	

}

void ULoginPageCell::OnCheckResponseMsg(msg_ptr _msg)
{

	m_BtnGetCheckReg->SetIsEnabled(true);

	
	FString MString = MsgJsonToString(_msg);
	FString mTip;
	if (EditorFunctionalTool::IdentifyingCodeCheck(_msg))
	{
		m_BtnGetCheckReg->SetIsEnabled(false);
		m_BtnGetCheckReg->SetVisibility(ESlateVisibility::Hidden);
		StartCoundDownFast();
		SetTip();
	}
	else
	{
		SetTip(mTip);
		m_BtnGetCheckReg->SetIsEnabled(true);
	}
}

void ULoginPageCell::OnFastResponseMsg(msg_ptr _msg)
{
	MsgLocal * mLocalMsg = _msg->ConvertMsg<MsgLocal>();
    bool Res = *((bool*)mLocalMsg->Content());
    
	bool check = false;
	bool newOrOld = false;
	if (GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule")->GetMsgCode() == MESSAGESUCCESS)
	{
		if (GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule")->GetUserData()->m_member.ticket == "")
		{
			newOrOld = true;
		}
		else
		{
			newOrOld = false;
		}
		check = true;
	}
	else
	{
		SetTip(TEXT(""));
	}

	if (Res)
	{
		SetTip();
        if (newOrOld)
        {
            ChangePage(FIRST_REGIST);
        }
        else
		{
			LogicLoginScene * currS = (LogicLoginScene*)UEditorDirector::GetInstance()->GetCurrentLogicScene();
			currS->JoinMainScene();
		}
	}
	else
	{
		FString mString = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule")->GetMsgString();

		SetTip(mString);
	}
}

void ULoginPageCell::OnPassWordCheckResponseMsg(msg_ptr _msg)
{
	FString MString  =  MsgJsonToString(_msg);
	FString mTip;
	if (EditorFunctionalTool::IdentifyingCodeCheck(_msg))
	{
		m_Button_GetCheckReset->SetIsEnabled(false);
		m_Button_GetCheckReset->SetVisibility(ESlateVisibility::Hidden);
		StartCoundDownReset();
		SetTip();
	}
	else
	{
		SetTip(mTip);
		m_Button_GetCheckReset->SetIsEnabled(true);
	}
	
}

void ULoginPageCell::OnPassWordResponseMsg(msg_ptr _msg)
{
	
	FString msgString = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule")->GetMsgString();
	int32 msgCode = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule")->GetMsgCode();


	if (msgCode == MESSAGESUCCESS)
	{
		SetPassWordTip();
		ChangePage(EnumLoginPage::LOGIN);
	}
	else
	{
		SetPassWordTip(msgString);
	}
}

void ULoginPageCell::OnFirstLoginResponseMsg(msg_ptr _msg)
{
	FString msgString = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule")->GetMsgString();
	int32 msgCode = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule")->GetMsgCode();


	if (msgCode == MESSAGESUCCESS)
	{
		SetTip();   //todo  这里应该添加 登录成功创建下载界面
        LogicLoginScene * currS = (LogicLoginScene*)UEditorDirector::GetInstance()->GetCurrentLogicScene();
        currS->JoinMainScene();

	}
	else
	{
		SetTip(msgString);
	}
}

void ULoginPageCell::OnGetHtmlResponseMsg(msg_ptr _msg)
{
	//换成本地消息.
	MsgLocal * mMsgLocal = _msg->ConvertMsg<MsgLocal>();

	//获取原数据.
	TSharedPtr<TMap<FString,FString>> *mTMap = (TSharedPtr<TMap<FString, FString>>*)mMsgLocal->Content();

	m_LoginModule->GetUserData()->SetUseCourseURL(m_LoginModule->GetUrlByKey("tutorialUrl"));

	m_LoginModule->GetUserData()->SetFeedbackURL(m_LoginModule->GetUrlByKey("feedbackUrl"));

	m_LoginModule->GetUserData()->SetContactURL(m_LoginModule->GetUrlByKey("contactUrl"));

	m_LoginModule->GetUserData()->SetShopURL(m_LoginModule->GetUrlByKey("mallUrl"));

	m_LoginModule->GetUserData()->SetOrderURL(m_LoginModule->GetUrlByKey("orderUrl"));

	m_LoginModule->GetUserData()->SetDownloadURL(m_LoginModule->GetUrlByKey("pcdownloadUrl"));

	m_LoginModule->GetUserData()->SetProtocolUrl(m_LoginModule->GetUrlByKey("protocolUrl"));
}

void ULoginPageCell::OnInit()
{
	Super::OnInit();

	m_LoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");
}

void ULoginPageCell::OnRelease()
{
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, m_LoginModule->m_loginSucess, this, &ULoginPageCell::OnLoginResponseMsg);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, m_LoginModule->m_GetHtmlUrlSuccess, this, &ULoginPageCell::OnGetHtmlResponseMsg);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, m_LoginModule->m_FastLoginSucess, this, &ULoginPageCell::OnFastResponseMsg);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, m_LoginModule->m_CheckNumberSucess, this, &ULoginPageCell::OnCheckResponseMsg);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, m_LoginModule->m_PassWordCheckSucess, this, &ULoginPageCell::OnPassWordCheckResponseMsg);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, m_LoginModule->m_PassWordResetSucess, this, &ULoginPageCell::OnPassWordResponseMsg);

	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, 106, this, &ULoginPageCell::OnFirstLoginResponseMsg);
}

void ULoginPageCell::OnSetParams(int32 _pid, FString & _content, ...)
{
	
}

void ULoginPageCell::OnTick(float InDeltaTimeSecond)
{
	Super::OnTick(InDeltaTimeSecond);
}

void ULoginPageCell::ClientButtonEvent(int32 _pid)
{
	switch (_pid)
	{
	case BtnPid_Login:
		//m_LoginModule->RequestLoginServer();
		break;
	case BtnPid_FastLogin:
		HandleFastLogin();
		break;
	case BtnPid_CheckNumber:
		HandleCheckNumber();
	}
	
}

void ULoginPageCell::HandleFastLogin()
{
	
	if (EditorFunctionalTool::CheckPhone(m_TextBoxPhoneReg->GetText().ToString()))
	{
		if (EditorFunctionalTool::CheckIdentifyingCode(m_TextBoxCheckNumber->GetText().ToString()))
		{
			m_LoginModule->RequestFastLogin(m_TextBoxPhoneReg->GetText().ToString(), m_TextBoxCheckNumber->GetText().ToString());
			SetTip();
		}
		else
		{
			SetTip(FString(TEXT("请输入正确的验证码")));
		}
		
	}
	else
	{
		SetTip(FString(TEXT("请输入正确的手机号")));
	}
}



void ULoginPageCell::HandlePassWordLogin()
{
	if (EditorFunctionalTool::CheckPhone(m_TextBoxPhone->GetText().ToString()))
	{
		FString mTip;
		if (true)//(EditorFunctionalTool::CheckPassword(m_TextSecrte->GetText().ToString(), mTip))
		{
			
			FString mPassWord = EditorFunctionalTool::Str2Md5(m_TextSecrte->GetText().ToString());
			m_LoginModule->RequestLoginServer(m_TextBoxPhone->GetText().ToString(), mPassWord);
			SetLoginTip();
		}
		else
		{
			SetLoginTip(mTip);
		}
	}
	else
	{
		//SetLoginTip(FString(TEXT("请输入正确是手机号")));
		SetTip(FString(TEXT("请输入正确的手机号")));
	}
	
}


void ULoginPageCell::HandleCheckNumber()
{
	FString PhoneString = m_TextBoxPhoneReg->GetText().ToString();
	
	if (EditorFunctionalTool::CheckPhone(PhoneString))
	{
		
		
		m_LoginModule->RequestCheckNumber(PhoneString);
		m_BtnGetCheckReg->SetIsEnabled(false);
	}
	else
	{
		SetTip(FString(TEXT("请输入正确的手机号")));
	}
	
}

void ULoginPageCell::AutoLogin()
{
	

	FString mIphone;
	FString mPassWord;
	if (UserInfo::Get()->ReadLocalData(mIphone, mPassWord))
	{
		UE_LOG(LogTemp, Log, TEXT("zhx : Auto Login Func1"));
		m_LoginModule->RequestLoginServer(mIphone, mPassWord);
		SetLoginTip();
		ChangePage(EnumLoginPage::AUTO_LOGIN);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("zhx : Auto Login Func2"));
		ChangePage(EnumLoginPage::FAST_REGIST);
	}


}
void ULoginPageCell::HandleCheckRest()
{
	if (EditorFunctionalTool::CheckPhone(m_EditableTextBox_PhoneReset->GetText().ToString()))
	{
		m_LoginModule->RequestPassWordResetCheckNumber(m_EditableTextBox_PhoneReset->GetText().ToString());
		m_Button_GetCheckReset->SetIsEnabled(false);
	}
	else
	{
		SetPassWordTip(FString(TEXT("请输入正确的手机号")));
	}
}
 
void ULoginPageCell::HandlePassWordReset()
{
	if (EditorFunctionalTool::CheckPhone(m_EditableTextBox_PhoneReset->GetText().ToString()))
	{
		if (EditorFunctionalTool::CheckIdentifyingCode(m_EditableTextBox_CheckedReset->GetText().ToString()))
		{
			if (m_EditableTextBox_PassWordReset->GetText().ToString().Len() > 0)
			{
				FString mLocalTip;
				if (EditorFunctionalTool::CheckPassword(m_EditableTextBox_PassWordReset->GetText().ToString(), mLocalTip))
				{
					FString mMD5 = EditorFunctionalTool::Str2Md5(m_EditableTextBox_PassWordReset->GetText().ToString()); 
					
					m_LoginModule->RequestPassWordReset(m_EditableTextBox_PhoneReset->GetText().ToString(), mMD5, m_EditableTextBox_CheckedReset->GetText().ToString());
				}
				else
				{
					SetPassWordTip(mLocalTip);
				}
				

			}
			else
			{
				SetPassWordTip(FString(TEXT("密码为空")));
			}
		}
		else
		{
			SetPassWordTip(FString(TEXT("请输验证码")));
		}
		//m_LoginModule->RequestPassWordResetCheckNumber(m_EditableTextBox_PhoneReset->GetText().ToString());
		m_Button_GetCheckReset->SetIsEnabled(false);
	}
	else
	{
		SetPassWordTip(FString(TEXT("请输入正确的手机号")));
	}
}

void ULoginPageCell::HandleFirstLogin()
{
	UE_LOG(LogTemp, Log, TEXT("zhx : login fisrt "));

	if (m_EditableTextBox_Password0First->GetText().EqualTo(m_EditableTextBox_Password1First->GetText()))
	{
		if (!m_EditableTextBox_Password0First->GetText().IsEmpty())
		{
			FString mTip;
			if (EditorFunctionalTool::CheckPassword(m_EditableTextBox_Password0First->GetText().ToString(), mTip))
			{
                FString md5PassWord = EditorFunctionalTool::Str2Md5(m_EditableTextBox_Password0First->GetText().ToString());
                
                m_LoginModule->RequestFirstLogin(m_TextBoxPhoneReg->GetText().ToString(), m_TextBoxCheckNumber->GetText().ToString(), md5PassWord);
			}
			else
			{
				SetTip(mTip);
			}
		}
		else
		{
			SetTip(TEXT("密码为空"));
		}
	}
	else
	{
		SetTip(TEXT("两次输入密码不一致"));
	}
}

void ULoginPageCell::SetTip(FString Tip)
{
	if (m_TextTip)
	{
		m_TextTip->SetText(FText::FromString(Tip));
	}
}

void ULoginPageCell::SetLoginTip(FString _tip)
{
	m_LoginTextCheck->SetText(FText::FromString(_tip));
}

void ULoginPageCell::SetPassWordTip(FString _tip )
{
	m_PassWordResetTextCheck->SetText(FText::FromString(_tip));
}

void ULoginPageCell::ChangePage(EnumLoginPage _Page)
{
	m_AutoLogin->SetVisibility(ESlateVisibility::Hidden);
	m_Login->SetVisibility(ESlateVisibility::Hidden);
	m_FastRegister->SetVisibility(ESlateVisibility::Hidden);
	m_PassWordReset->SetVisibility(ESlateVisibility::Hidden);
	m_FirstRegister->SetVisibility(ESlateVisibility::Hidden);
	m_ProtocolCanvasPanel->SetVisibility(ESlateVisibility::Hidden);
	CurrentPage = _Page;
	if (CurrentPage == FAST_REGIST)
	{
		m_BtnBack->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		m_BtnBack->SetVisibility(ESlateVisibility::Visible);

	}
	SetTip();
	switch (_Page)
	{
	case EnumLoginPage::FAST_REGIST:
		m_FastRegister->SetVisibility(ESlateVisibility::Visible);
		m_TextBoxCheckNumber->SetText(FText::FromString(FString("")));
		m_TextBoxPhoneReg->SetText(FText::FromString(FString("")));
		m_TextBlock_CountDownReg->SetText(FText::FromString(FString("")));
		m_BtnGetCheckReg->SetVisibility(ESlateVisibility::Visible);
		m_BtnGetCheckReg->SetIsEnabled(true);
		m_ProtocolCanvasPanel->SetVisibility(ESlateVisibility::Visible);
		
		break;
	case EnumLoginPage::LOGIN:
		m_Login->SetVisibility(ESlateVisibility::Visible);
		m_TextBoxPhone->SetText(FText::FromString(FString("")));
		m_TextSecrte->SetText(FText::FromString(FString("")));
		m_LoginTextCheck->SetText(FText::FromString(FString("")));
		break;
	case EnumLoginPage::PASSWORD_RESET:
		m_PassWordReset->SetVisibility(ESlateVisibility::Visible);
		m_EditableTextBox_PhoneReset->SetText(FText::FromString(FString("")));
		m_EditableTextBox_CheckedReset->SetText(FText::FromString(FString("")));
		m_EditableTextBox_PassWordReset->SetText(FText::FromString(FString("")));
		m_TextBlock_CountDownReset->SetText(FText::FromString(FString("")));
		m_PassWordResetTextCheck->SetText(FText::FromString(FString("")));
		m_Button_GetCheckReset->SetVisibility(ESlateVisibility::Visible);
		break;
	case EnumLoginPage::FIRST_REGIST:
		m_FirstRegister->SetVisibility(ESlateVisibility::Visible);
		m_EditableTextBox_Password0First->SetText(FText::FromString(FString("")));
		m_EditableTextBox_Password1First->SetText(FText::FromString(FString("")));
		break;
	case EnumLoginPage::AUTO_LOGIN:
		m_AutoLogin->SetVisibility(ESlateVisibility::Visible);
		break;
	}

	
}

void ULoginPageCell::LoginBtnCallback()
{
	ChangePage(EnumLoginPage::LOGIN);
}

void ULoginPageCell::GoPagePassWordReset()
{
	ChangePage(EnumLoginPage::PASSWORD_RESET);
}

void ULoginPageCell::SendChangePage()
{
	
	switch (CurrentPage)
	{
	case FAST_REGIST:
		break;
	case LOGIN:
		ChangePage(EnumLoginPage::FAST_REGIST);
		break;
	case PASSWORD_RESET:
		ChangePage(EnumLoginPage::LOGIN);
		break;
	case FIRST_REGIST:
		ChangePage(EnumLoginPage::PASSWORD_RESET);
		break;
	}
	
}

FString ULoginPageCell::MsgJsonToString(msg_ptr _msg)
{
	MsgLocal * mMsgLocal = _msg->ConvertMsg<MsgLocal>();

	TSharedPtr<FJsonObject> * t_jsonObject = (TSharedPtr<FJsonObject>*)mMsgLocal->Content();
	

	FString mContent;

	TSharedRef<TJsonWriter<TCHAR>> t_writer = TJsonWriterFactory<>::Create(&mContent);

	FJsonSerializer::Serialize((*t_jsonObject).ToSharedRef(), t_writer);

	UE_LOG(GEditorLog, Error, TEXT("LoginPageCell userinfo= %s"), *mContent);

	return mContent;
}

void ULoginPageCell::StartCoundDownReset()
{
	CountDownReset = 60;

	GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &ULoginPageCell::CountDownResetCheck, 1.0f, true);
}

void ULoginPageCell::CountDownResetCheck()
{
	CountDownReset -= 1;
	if (CountDownReset < 1)
	{
		GWorld->GetTimerManager().ClearTimer(CountdownTimerHandle);
		m_Button_GetCheckReset->SetVisibility(ESlateVisibility::Visible);
		m_Button_GetCheckReset->SetIsEnabled(true);
		m_TextBlock_CountDownReset->SetText(FText::FromString(FString(TEXT(""))));
	}
	FString mTimeCount = FString::FromInt(CountDownReset) + FString(TEXT("S"));
	m_TextBlock_CountDownReset->SetText(FText::FromString(mTimeCount));
	UE_LOG(GEditorLog, Error, TEXT("time left !!!!!!!!!!!!!!!!= %d"), CountDownReset);
}

void ULoginPageCell::StartCoundDownFast()
{
	CountDownReset = 60;

	GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &ULoginPageCell::CountDownnFastCheck, 1.0f, true);
}

void ULoginPageCell::CountDownnFastCheck()
{
	CountDownReset -= 1;
	if (CountDownReset < 1)
	{
		GWorld->GetTimerManager().ClearTimer(CountdownTimerHandle);
		m_BtnGetCheckReg->SetVisibility(ESlateVisibility::Visible);
		m_BtnGetCheckReg->SetIsEnabled(true);
		m_TextBlock_CountDownReg->SetText(FText::FromString(FString(TEXT(""))));
	}
	FString mTimeCount = FString::FromInt(CountDownReset) + FString(TEXT("s"));
	m_TextBlock_CountDownReg->SetText(FText::FromString(mTimeCount));
	UE_LOG(GEditorLog, Error, TEXT("time left !!!!!!!!!!!!!!!!= %d"), CountDownReset);
}

void ULoginPageCell::GetProtocol(int32 _pid)
{
	if (nullptr != m_LoginModule->GetUserData())
		UHaoshaSDKBPLibrary::OpenWebview(EditorFunctionalTool::GetTicketUrl(m_LoginModule->GetUserData()->GetProtocolUrl()));
}


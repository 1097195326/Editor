// Fill out your copyright notice in the Description page of Project Settings.

#include "Win_LoginPageCell.h"
#include "Editor.h"
#include "Userdata.h"
#include "UserInfo.h"
#include "UIManager.h"
#include "MsgCenter.h"
#include "LoginModule.h"
#include "LoginModule.h"
#include "Win_WebPage.h"
#include "TimerManager.h"
#include "Win_DesignUMG.h"
#include "GModuleManager.h"
#include "EditorDirector.h"
#include "LogicLoginScene.h"
#include "LogicDesignScene.h"
#include "HaoshaSDKBPLibrary.h"
#include "Editor/Public/User/Tools/EditorFunctionalTool.h"

UWin_LoginPageCell::UWin_LoginPageCell(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
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
		CountDownReset = 0;
}

bool UWin_LoginPageCell::OnStart()
{
	m_LoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	//	本地监听消息回调.
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, m_LoginModule->m_loginSucess, this, &UWin_LoginPageCell::OnLoginResponseMsg);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, m_LoginModule->m_FastLoginSucess, this, &UWin_LoginPageCell::OnFastResponseMsg);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, m_LoginModule->m_CheckNumberSucess, this, &UWin_LoginPageCell::OnCheckResponseMsg);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, m_LoginModule->m_PassWordCheckSucess, this, &UWin_LoginPageCell::OnPassWordCheckResponseMsg);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, m_LoginModule->m_PassWordResetSucess, this, &UWin_LoginPageCell::OnPassWordResponseMsg);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, m_LoginModule->m_FirstLoginSucess, this, &UWin_LoginPageCell::OnFirstLoginResponseMsg);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, m_LoginModule->m_GetHtmlUrlSuccess, this, &UWin_LoginPageCell::OnGetHtmlResponseMsg);
	

	//	请求服务器获取h5路径.
	m_LoginModule->RequestGetHtmlUrlServer();

	//	账号密码登陆.
	UUIManager::GetInstance()->RegisterButtonClick(1, m_BtnLogin, this, &UWin_LoginPageCell::HandlePassWordLogin);

	//	返回.
	UUIManager::GetInstance()->RegisterButtonClick(1, m_BtnBack, this, &UWin_LoginPageCell::SendChangePage);

	//	用户协议.
	UUIManager::GetInstance()->RegisterButtonClick(1, m_Protocol, this, &UWin_LoginPageCell::GetProtocol);

	//	验证码.
	UUIManager::GetInstance()->RegisterButtonClick(1, m_BtnGetCheckReg, this, &UWin_LoginPageCell::ClientButtonEvent);
	
	//	快速注册.
	UUIManager::GetInstance()->RegisterButtonClick(2, m_BtnFastRegistLogin, this, &UWin_LoginPageCell::ClientButtonEvent);
	
	//	账号密码登陆界面按钮.
	UUIManager::GetInstance()->RegisterButtonClick(1, m_BtnPassWordLogin, this, &UWin_LoginPageCell::LoginBtnCallback);

	//	
	UUIManager::GetInstance()->RegisterButtonClick(1, m_Button_CommitFirst, this, &UWin_LoginPageCell::HandleFirstLogin);


	//	忘记密码.
	UUIManager::GetInstance()->RegisterButtonClick(1, m_BtnForgetPassWord, this, &UWin_LoginPageCell::GoPagePassWordReset);

	UUIManager::GetInstance()->RegisterButtonClick(1, m_Button_PassWordReset_Sure, this, &UWin_LoginPageCell::HandlePassWordReset);

	UUIManager::GetInstance()->RegisterButtonClick(1, m_Button_GetCheckReset, this, &UWin_LoginPageCell::HandleCheckRest);

	AutoLogin();
	return true;
}

void UWin_LoginPageCell::OnInit()
{
	//	用户协议.
	if (UCanvasPanel* ProtocolCanvasPanel = Cast<UCanvasPanel>(GetWidgetFromName("ProtocolCanvasPanel")))
	{
		m_ProtocolCanvasPanel = ProtocolCanvasPanel;
	}
	if (UButton * BtnProtocol = Cast<UButton>(GetWidgetFromName("ButtonProtocol")))
	{
		m_Protocol = BtnProtocol;
	}

	//	提示信息.
	if (UTextBlock* TextTip = Cast<UTextBlock>(GetWidgetFromName("TipText")))
	{
		m_TextTip = TextTip;
	}

	//	返回.
	if (UButton* BtnBack = Cast<UButton>(GetWidgetFromName("BtnBack")))
	{
		m_BtnBack = BtnBack;
	}

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
	if (UTextBlock* TextBlock_CountDownReg = Cast<UTextBlock>(GetWidgetFromName("TextBlock_CountDownReg")))
	{
		m_TextBlock_CountDownReg = TextBlock_CountDownReg;
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

	//第一次注册，设置密码.
	if (UCanvasPanel* FirstRegister = Cast<UCanvasPanel>(GetWidgetFromName("FirstRegister")))
	{
		m_FirstRegister = FirstRegister;
	}
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

	//自动登录.
	if (UCanvasPanel* AutoLogin = Cast<UCanvasPanel>(GetWidgetFromName("AutoLogin")))
	{
		m_AutoLogin = AutoLogin;
	}
}

void UWin_LoginPageCell::OnRelease()
{
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, m_LoginModule->m_loginSucess, this, &UWin_LoginPageCell::OnLoginResponseMsg);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, m_LoginModule->m_GetHtmlUrlSuccess, this, &UWin_LoginPageCell::OnGetHtmlResponseMsg);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, m_LoginModule->m_FastLoginSucess, this, &UWin_LoginPageCell::OnFastResponseMsg);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, m_LoginModule->m_CheckNumberSucess, this, &UWin_LoginPageCell::OnCheckResponseMsg);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, m_LoginModule->m_PassWordCheckSucess, this, &UWin_LoginPageCell::OnPassWordCheckResponseMsg);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, m_LoginModule->m_PassWordResetSucess, this, &UWin_LoginPageCell::OnPassWordResponseMsg);

	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, 106, this, &UWin_LoginPageCell::OnFirstLoginResponseMsg);
}

//	本地监听到服务器账号密码登陆信息.
void UWin_LoginPageCell::OnLoginResponseMsg(msg_ptr _msg)
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

void UWin_LoginPageCell::OnCheckResponseMsg(msg_ptr _msg)
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

void UWin_LoginPageCell::OnFastResponseMsg(msg_ptr _msg)
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

void UWin_LoginPageCell::OnPassWordCheckResponseMsg(msg_ptr _msg)
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

void UWin_LoginPageCell::OnPassWordResponseMsg(msg_ptr _msg)
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

void UWin_LoginPageCell::OnFirstLoginResponseMsg(msg_ptr _msg)
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

void UWin_LoginPageCell::OnGetHtmlResponseMsg(msg_ptr _msg)
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

	m_LoginModule->GetUserData()->SetGoodsListUrl(m_LoginModule->GetUrlByKey("goodsListUrl"));
}



void UWin_LoginPageCell::OnSetParams(int32 _pid, FString & _content, ...)
{
	
}

void UWin_LoginPageCell::OnTick(float InDeltaTimeSecond)
{
	Super::OnTick(InDeltaTimeSecond);
}

void UWin_LoginPageCell::ClientButtonEvent(int32 _pid)
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

void UWin_LoginPageCell::HandleFastLogin()
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


//	账号密码登陆.
void UWin_LoginPageCell::HandlePassWordLogin(int32 _pid)
{
	if (EditorFunctionalTool::CheckPhone(m_TextBoxPhone->GetText().ToString()))
	{
		FString mPassWord = EditorFunctionalTool::Str2Md5(m_TextSecrte->GetText().ToString());

		m_LoginModule->RequestLoginServer(m_TextBoxPhone->GetText().ToString(), mPassWord);

		SetLoginTip();
	}
	else
	{
		FString mText = GLanguageTool::GetInstance()->GetLanguage(TEXT("key155"));

		SetTip(mText);
	}
}


void UWin_LoginPageCell::HandleCheckNumber()
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

void UWin_LoginPageCell::AutoLogin()
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
void UWin_LoginPageCell::HandleCheckRest(int32 _pid)
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
 
void UWin_LoginPageCell::HandlePassWordReset(int32 _pid)
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

void UWin_LoginPageCell::HandleFirstLogin(int32 _pid)
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

void UWin_LoginPageCell::SetTip(FString Tip)
{
	if (m_TextTip)
	{
		m_TextTip->SetText(FText::FromString(Tip));
	}
}

void UWin_LoginPageCell::SetLoginTip(FString _tip)
{
	m_LoginTextCheck->SetText(FText::FromString(_tip));
}

void UWin_LoginPageCell::SetPassWordTip(FString _tip )
{
	m_PassWordResetTextCheck->SetText(FText::FromString(_tip));
}

void UWin_LoginPageCell::ChangePage(EnumLoginPage _Page)
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

void UWin_LoginPageCell::LoginBtnCallback(int32 _pid)
{
	ChangePage(EnumLoginPage::LOGIN);
}

void UWin_LoginPageCell::GoPagePassWordReset(int32 _pid)
{
	ChangePage(EnumLoginPage::PASSWORD_RESET);
}

void UWin_LoginPageCell::SendChangePage(int32 _pid)
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

FString UWin_LoginPageCell::MsgJsonToString(msg_ptr _msg)
{
	MsgLocal * mMsgLocal = _msg->ConvertMsg<MsgLocal>();

	TSharedPtr<FJsonObject> * t_jsonObject = (TSharedPtr<FJsonObject>*)mMsgLocal->Content();
	

	FString mContent;

	TSharedRef<TJsonWriter<TCHAR>> t_writer = TJsonWriterFactory<>::Create(&mContent);

	FJsonSerializer::Serialize((*t_jsonObject).ToSharedRef(), t_writer);

	UE_LOG(GEditorLog, Error, TEXT("LoginPageCell userinfo= %s"), *mContent);

	return mContent;
}

void UWin_LoginPageCell::StartCoundDownReset()
{
	CountDownReset = 60;

	GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &UWin_LoginPageCell::CountDownResetCheck, 1.0f, true);
}

void UWin_LoginPageCell::CountDownResetCheck()
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

void UWin_LoginPageCell::StartCoundDownFast()
{
	CountDownReset = 60;

	GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &UWin_LoginPageCell::CountDownnFastCheck, 1.0f, true);
}

void UWin_LoginPageCell::CountDownnFastCheck()
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

void UWin_LoginPageCell::GetProtocol(int32 _pid)
{
	//	注册协议.
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
			mTemp->m_WebBuy->LoadURL(EditorFunctionalTool::GetTicketUrl(mLoginModule->GetUserData()->GetProtocolUrl()));
		}
	}
}


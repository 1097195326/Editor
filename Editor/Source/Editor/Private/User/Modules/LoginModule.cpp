// Fill out your copyright notice in the Description page of Project Settings.

#include "LoginModule.h"
#include "UserInfo.h"
#include "GLanguageTool.h"
#include "GModuleManager.h"
#include "Serialization/JsonSerializer.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "Editor.h"
#include "GFileHelper.h"
#include "TDesignTypeManager.h"
#include "UIManager.h"
#include "LoadDataModule.h"
#include "GameDirector.h"
#include "Private/User/Const/MsgConst.h"

// 反射.
GREGISTER_MODULE(LoginModule)

LoginModule::LoginModule()
{
	FString location = GFileHelper::ProjectPersistentDownloadDir();

	m_UserData = new FUserData();
}

LoginModule::~LoginModule()
{
	delete m_UserData;
}

FString LoginModule::GetUrlByKey(const FString & _key)
{
	if (m_h5UrlMap.Contains(_key))
	{
		return m_h5UrlMap[_key];
	}
	return TEXT("");
}

void LoginModule::OnLoad()
{
	// 注册监听消息.
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeHttpResponse, 1001, this, &LoginModule::OnLoginResponseMsg);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeHttpResponse, MsgConst::S_CHECK_NUMBER, this, &LoginModule::OnCheckNumberResponseMsg);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeHttpResponse, MsgConst::S_FAST_LOGIN, this, &LoginModule::OnFastLoginResponseMsg);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeHttpResponse, 1005, this, &LoginModule::OnResetPassWordCheckNumberResponseMsg);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeHttpResponse, 1006, this, &LoginModule::OnrResetPassWordResponseMsg);
    MsgCenter::Instance()->RegisterMsgHandler(MessageTypeHttpResponse, 10022, this, &LoginModule::OnrFirstLoginResponseMsg);

	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeHttpResponse, 2001, this, &LoginModule::OnResetUserName);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeHttpResponse, 2009, this, &LoginModule::OnGetHtmlUrlResponseMsg);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeHttpResponse, 2005, this, &LoginModule::OnGetNickName);

	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, BaseMsgConst::SYSTEM_HTTP_RESPONSE_ERROR, this, &LoginModule::OnLoginTimeout);
}

void LoginModule::OnRelease()
{
	// 移除监听消息.
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeHttpResponse, MsgConst::S_LOGIN, this, &LoginModule::OnLoginResponseMsg);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeHttpResponse, MsgConst::S_CHECK_NUMBER, this, &LoginModule::OnCheckNumberResponseMsg);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeHttpResponse, MsgConst::S_FAST_LOGIN, this, &LoginModule::OnFastLoginResponseMsg);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeHttpResponse, 1005, this, &LoginModule::OnResetPassWordCheckNumberResponseMsg);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeHttpResponse, 1006, this, &LoginModule::OnrResetPassWordResponseMsg);
    MsgCenter::Instance()->RemoveMsgHandler(MessageTypeHttpResponse, 10022, this, &LoginModule::OnrFirstLoginResponseMsg);

	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeHttpResponse, 2001, this, &LoginModule::OnResetUserName);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeHttpResponse, 2009, this, &LoginModule::OnGetHtmlUrlResponseMsg);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeHttpResponse, 2005, this, &LoginModule::OnGetNickName);


	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, BaseMsgConst::SYSTEM_HTTP_RESPONSE_ERROR, this, &LoginModule::OnLoginTimeout);
}

// 请求账号密码登录.
void LoginModule::RequestLoginServer(const FString& _Phone, const FString& _PassWord)
{
	// 1.打包发送消息的内容json格式.
	TSharedPtr<FJsonObject> t_jsonObject = MakeShareable(new FJsonObject);
	
	t_jsonObject->SetStringField(TEXT("phone"), _Phone);

	t_jsonObject->SetStringField(TEXT("password"), _PassWord);

	// 2.封装消息体 channelId,消息号，token，_operator，是否Get或Post,发送的消息内容.
	FString mToken("token"); 

	FString mOperator("operator");

	msg_ptr mMsg(new MsgHttp(MessageTypeHttpRequest, 1001, mToken, mOperator, false, t_jsonObject));
	
	// 3.通过消息系统发送消息.
	MsgCenter::Instance()->SendMsg(mMsg);   
}

//	监听到账号密码登录信息.
void LoginModule::OnLoginResponseMsg(msg_ptr _msg)
{
	MsgHttp *mHttpMsg = _msg->ConvertMsg<MsgHttp>();

	// 1.先判断消息是否正常接收.
	if (mHttpMsg->GetMsgErrorCode() == EnumMsgResponseErrorCode::MessageTrue)
	{
		// 2.消息正常,获取内容.
		TSharedPtr<FJsonObject> mjsonObject = mHttpMsg->GetHttpContent();

		TSharedPtr<FJsonObject> t_jsonObject = mjsonObject->GetObjectField(TEXT("data"));

		GetUserData()->InitData(t_jsonObject);

		GetUserInfo()->UpdateData(*m_UserData);

		mMsgCode = mHttpMsg->GetServerCode();
		mTempString = mHttpMsg->GetServerMsg();

		// 发送登录成功的广播.
		msg_ptr mBrodcastLoginSucess(new MsgLocal(MessageTypeLocal, m_loginSucess, &t_jsonObject));

		MsgCenter::Instance()->SendMsg(mBrodcastLoginSucess);
	}
	else
	{
		// 消息异常,自行根据逻辑判断是否重发.

		// 获取异常内容.
		TSharedPtr<FJsonObject> mjsonObject = mHttpMsg->GetHttpContent();

		mTempString = mHttpMsg->GetServerMsg();
		mMsgCode = 1;

		msg_ptr mBrodcastLoginSucess(new MsgLocal(MessageTypeLocal, m_loginSucess, &mjsonObject));

		MsgCenter::Instance()->SendMsg(mBrodcastLoginSucess);
	}
}

//验证码快速登录.
void LoginModule::RequestFastLogin(FString _Phone, FString _CheckNumber)
{
	
	TSharedPtr<FJsonObject> t_jsonObject = MakeShareable(new FJsonObject);

	t_jsonObject->SetStringField(TEXT("phone"), TCHAR_TO_ANSI(*_Phone));

	t_jsonObject->SetStringField(TEXT("smsAuthCode"), TCHAR_TO_ANSI(*_CheckNumber));

	// 2.封装消息体 channelId,消息号，token，_operator，是否Get或Post,发送的消息内容.
	FString mToken(TEXT("token"));

	FString mOperator(TEXT("operator"));

	
	

	msg_ptr mMsg(new MsgHttp(MessageTypeHttpRequest, MsgConst::S_FAST_LOGIN, mToken, mOperator, false, t_jsonObject));

	// 3.通过消息系统发送消息.
	MsgCenter::Instance()->SendMsg(mMsg);
}

//	请求http重置用户昵称.
void LoginModule::RequestUserNameReset(FString _Avatar,FString _UserName)
{
	TSharedPtr<FJsonObject> t_jsonObject = MakeShareable(new FJsonObject);

	t_jsonObject->SetStringField(TEXT("avatar"), _Avatar);

	t_jsonObject->SetStringField(TEXT("nickName"),_UserName);

	FString mToken = this->GetUserData()->GetToken();

	FString mOperator = this->GetUserData()->GetUserId();

	msg_ptr mMsg(new MsgHttp(MessageTypeHttpRequest, 2001, mToken, mOperator, false, t_jsonObject));
	
	MsgCenter::Instance()->SendMsg(mMsg);
}

//	请求htpp获取H5地址.
void LoginModule::RequestGetHtmlUrlServer()
{
	TSharedPtr<FJsonObject> t_jsonObject = MakeShareable(new FJsonObject);

	FString mToken = this->GetUserData()->GetToken();

	FString mOperator = this->GetUserData()->GetUserId();

	msg_ptr mMsg(new MsgHttp(MessageTypeHttpRequest, 2009, mToken, mOperator, false, t_jsonObject));

	MsgCenter::Instance()->SendMsg(mMsg);
}

//	请求htpp获取用户昵称和头像.
void LoginModule::RequestNickName()
{
	TSharedPtr<FJsonObject> t_jsonObject = MakeShareable(new FJsonObject);

	FString mToken = this->GetUserData()->GetToken();

	FString mOperator = this->GetUserData()->GetUserId();

	msg_ptr mMsg(new MsgHttp(MessageTypeHttpRequest, 2005, mToken, mOperator, false, t_jsonObject));

	MsgCenter::Instance()->SendMsg(mMsg);
}

void LoginModule::RequestFirstLogin(FString PhoneNumber, FString CheckCode, FString PassWord)
{
	TSharedPtr<FJsonObject> t_jsonObject = MakeShareable(new FJsonObject);

	t_jsonObject->SetStringField(TEXT("phone"), PhoneNumber);
    t_jsonObject->SetStringField(TEXT("smsAuthCodeType"), TEXT("REGISTER_SMS_AUTH_CODE_"));
	t_jsonObject->SetStringField(TEXT("smsAuthCode"), CheckCode);//驗證碼.

	t_jsonObject->SetStringField(TEXT("password"), PassWord);
	t_jsonObject->SetStringField(TEXT("roleCode"), FString(TEXT("")));
	t_jsonObject->SetStringField(TEXT("twoRoleCode"), FString(TEXT("")));

	FString mToken(TEXT("token"));

	FString mOperator(TEXT("operator"));

	msg_ptr mMsg(new MsgHttp(MessageTypeHttpRequest, 10022, mToken, mOperator, false, t_jsonObject));

	MsgCenter::Instance()->SendMsg(mMsg);
}

void LoginModule::RequestPassWordReset(FString _Phone, FString _PassWord, FString _CheckNumber)
{
	TSharedPtr<FJsonObject> t_jsonObject = MakeShareable(new FJsonObject);

	t_jsonObject->SetStringField(TEXT("phone"), _Phone);

	t_jsonObject->SetStringField(TEXT("newPassword"), _PassWord);

	t_jsonObject->SetStringField(TEXT("smsAuthCode"), _CheckNumber);

	FString mToken("token");

	FString mOperator("operator");

	msg_ptr mMsg(new MsgHttp(MessageTypeHttpRequest, 1006, mToken, mOperator, false, t_jsonObject));

	MsgCenter::Instance()->SendMsg(mMsg);
}

void LoginModule::LoginComplete()
{
	
}

void LoginModule::LogOut()
{
	// 1.把本地缓存的用户数据删除.
	UserInfo::Get()->ClearUserData();
	// 2.用户数据清理.
	LoadDataModule * mLoadDataModule = GModuleManager::GetInstance()->GetModule<LoadDataModule>("LoadDataModule");

	mLoadDataModule->ClearStyleDetailData();

	mLoadDataModule->ClearSuitDetailData();

	// 直接跳转到登录场景.
	UGameDirector::GetInstance()->OpenLevel(TEXT("LoginLevel"));
}

// 请求验证码.
void LoginModule::RequestCheckNumber(FString PhoneNumber)
{
	TSharedPtr<FJsonObject> t_jsonObject = MakeShareable(new FJsonObject);

	t_jsonObject->SetStringField(TEXT("phone"), TCHAR_TO_ANSI(*PhoneNumber));
	t_jsonObject->SetStringField(TEXT("smsAuthCodeType"), TEXT("REGISTER_SMS_AUTH_CODE_"));

	FString mPhone = t_jsonObject->GetStringField(TEXT("phone"));

	FString mToken("token");

	FString mOperator("operator");


	// 获取验证码.
	msg_ptr mYanzhengma(new MsgHttp(MessageTypeHttpRequest, MsgConst::S_CHECK_NUMBER, mToken, mOperator, false, t_jsonObject));

//	msg_ptr mMsg(new MsgHttp(MessageTypeHttpRequest, MsgConst::S_LOGIN, mToken, mOperator, false, t_jsonObject));

	// 3.通过消息系统发送消息.
	MsgCenter::Instance()->SendMsg(mYanzhengma);
}




void LoginModule::RequestPassWordResetCheckNumber(FString PhoneNumber)
{
	TSharedPtr<FJsonObject> t_jsonObject = MakeShareable(new FJsonObject);

	t_jsonObject->SetStringField(TEXT("phone"), TCHAR_TO_ANSI(*PhoneNumber));
	t_jsonObject->SetStringField(TEXT("smsAuthCodeType"), "REGISTER_NEW_PASSWORD_SMS_AUTH_CODE_");

	FString mToken("token");

	FString mOperator("operator");


	// 获取验证码.
	msg_ptr mYanzhengma(new MsgHttp(MessageTypeHttpRequest, MsgConst::S_PASSWORD_CHECK_NUMBER, mToken, mOperator, false, t_jsonObject));

	//	msg_ptr mMsg(new MsgHttp(MessageTypeHttpRequest, MsgConst::S_LOGIN, mToken, mOperator, false, t_jsonObject));

	// 3.通过消息系统发送消息.
	MsgCenter::Instance()->SendMsg(mYanzhengma);
}



void LoginModule::OnCheckNumberResponseMsg(msg_ptr _msg)
{
	MsgHttp *mHttpMsg = _msg->ConvertMsg<MsgHttp>();

	// 1.先判断消息是否正常接收.
	if (mHttpMsg->GetMsgErrorCode() == EnumMsgResponseErrorCode::MessageTrue)
	{
		// 2.消息正常,获取内容.
		TSharedPtr<FJsonObject> t_jsonObject = mHttpMsg->GetHttpContent();

		// 只是转换成字符串打印.
		FString mContent;

		TSharedRef<TJsonWriter<TCHAR>> t_writer = TJsonWriterFactory<>::Create(&mContent);

		FJsonSerializer::Serialize(t_jsonObject.ToSharedRef(), t_writer);

		msg_ptr mBrodcasCheckSucess(new MsgLocal(MessageTypeLocal, m_CheckNumberSucess, &t_jsonObject));

		MsgCenter::Instance()->SendMsg(mBrodcasCheckSucess);


	}
	else
	{
		// 消息异常,自行根据逻辑判断是否重发.

		// 获取异常内容.
		FString mErrorInfo = mHttpMsg->GetMsgErrorInfo();

		UE_LOG(GEditorLog, Error, TEXT("http response Error %s"), *mErrorInfo);
	}
}

void LoginModule::OnFastLoginResponseMsg(msg_ptr _msg)
{
	MsgHttp *mHttpMsg = _msg->ConvertMsg<MsgHttp>();

	
	if (mHttpMsg->GetMsgErrorCode() == EnumMsgResponseErrorCode::MessageTrue)
	{
		
		TSharedPtr<FJsonObject> t_jsonObject = mHttpMsg->GetHttpContent();

        TSharedPtr<FJsonObject> t_jsonData = t_jsonObject->GetObjectField(TEXT("data"));
		FString mContent;

		TSharedRef<TJsonWriter<TCHAR>> t_writer = TJsonWriterFactory<>::Create(&mContent);

		FJsonSerializer::Serialize(t_jsonObject.ToSharedRef(), t_writer);

		UE_LOG(LogTemp, Log, TEXT("zhx : OnFastLoginResponseMsg : %s"), *mContent);

		mMsgCode = mHttpMsg->GetServerCode();
		mTempString = mHttpMsg->GetServerMsg();


		GetUserData()->InitData(t_jsonData);
		GetUserInfo()->UpdateData(*m_UserData);
        
        bool Res = true;
        
		msg_ptr mBrodcastLoginSucess(new MsgLocal(MessageTypeLocal, m_FastLoginSucess, &Res));

		MsgCenter::Instance()->SendMsg(mBrodcastLoginSucess);

		

		//UE_LOG(GEditorLog, Error, TEXT("http response content %s"), *mContent);
	}
	else
	{
		// 消息异常,自行根据逻辑判断是否重发.

		// 获取异常内容.
		FString mErrorInfo = mHttpMsg->GetMsgErrorInfo();

		UE_LOG(GEditorLog, Error, TEXT("http response Error %s"), *mErrorInfo);
		mTempString = mHttpMsg->GetMsgErrorInfo();
        bool Res = false;
		msg_ptr mBrodcastLoginFail(new MsgLocal(MessageTypeLocal, m_FastLoginSucess, &Res));

		MsgCenter::Instance()->SendMsg(mBrodcastLoginFail);
	}
}

void LoginModule::OnResetPassWordCheckNumberResponseMsg(msg_ptr _msg)
{
	MsgHttp *mHttpMsg = _msg->ConvertMsg<MsgHttp>();

	// 1.先判断消息是否正常接收.
	if (mHttpMsg->GetMsgErrorCode() == EnumMsgResponseErrorCode::MessageTrue)
	{
		// 2.消息正常,获取内容.
		TSharedPtr<FJsonObject> t_jsonObject = mHttpMsg->GetHttpContent();

		// 只是转换成字符串打印.
		FString mContent;

		TSharedRef<TJsonWriter<TCHAR>> t_writer = TJsonWriterFactory<>::Create(&mContent);

		FJsonSerializer::Serialize(t_jsonObject.ToSharedRef(), t_writer);

		msg_ptr mBrodcasCheckSucess(new MsgLocal(MessageTypeLocal, m_PassWordCheckSucess, &t_jsonObject));

		MsgCenter::Instance()->SendMsg(mBrodcasCheckSucess);

	}
	else
	{
		// 消息异常,自行根据逻辑判断是否重发.

		// 获取异常内容.
		FString mErrorInfo = mHttpMsg->GetMsgErrorInfo();

		UE_LOG(GEditorLog, Error, TEXT("http response Error %s"), *mErrorInfo);
	}
}

void LoginModule::OnrResetPassWordResponseMsg(msg_ptr _msg)
{
	MsgHttp *mHttpMsg = _msg->ConvertMsg<MsgHttp>();

	mMsgCode = mHttpMsg->GetServerCode();
	mTempString = mHttpMsg->GetServerMsg();

	TSharedPtr<FJsonObject> t_jsonObject = mHttpMsg->GetHttpContent();

	if (mHttpMsg->GetMsgErrorCode() == EnumMsgResponseErrorCode::MessageTrue)
	{
		TSharedPtr<FJsonObject> t_jsonData = t_jsonObject->GetObjectField(TEXT("data"));
		GetUserData()->InitData(t_jsonData);

		GetUserInfo()->UpdateData(*m_UserData);

		FString mContent;
		TSharedRef<TJsonWriter<TCHAR>> t_writer = TJsonWriterFactory<>::Create(&mContent);
		FJsonSerializer::Serialize(t_jsonObject.ToSharedRef(), t_writer);
		UE_LOG(LogTemp, Log, TEXT("zhx : reset pass word : %s"), *mContent);
	}
	else
	{
		FString mErrorInfo = mHttpMsg->GetMsgErrorInfo();

		UE_LOG(GEditorLog, Error, TEXT("http response Error %s"), *mErrorInfo);
	}
	msg_ptr mBrodcasCheckSucess(new MsgLocal(MessageTypeLocal, m_PassWordResetSucess, &t_jsonObject));

	MsgCenter::Instance()->SendMsg(mBrodcasCheckSucess);
}




void LoginModule::OnrFirstLoginResponseMsg(msg_ptr _msg)
{
	MsgHttp *mHttpMsg = _msg->ConvertMsg<MsgHttp>();

	if (mHttpMsg->GetMsgErrorCode() == EnumMsgResponseErrorCode::MessageTrue)
	{

		TSharedPtr<FJsonObject> t_jsonObject = mHttpMsg->GetHttpContent();
		mMsgCode = mHttpMsg->GetServerCode();
		mTempString = mHttpMsg->GetServerMsg();

		TSharedPtr<FJsonObject> t_jsonData = t_jsonObject->GetObjectField(TEXT("data"));
		GetUserData()->InitData(t_jsonData);

		GetUserInfo()->UpdateData(*m_UserData);


		msg_ptr mBrodcastLoginSucess(new MsgLocal(MessageTypeLocal, 106, &t_jsonObject));

		MsgCenter::Instance()->SendMsg(mBrodcastLoginSucess);
	}
	else
	{
		FString mErrorInfo = mHttpMsg->GetMsgErrorInfo();

		UE_LOG(GEditorLog, Error, TEXT("http response Error %s"), *mErrorInfo);
	}
}



//	修改用户昵称--监听事件.
void LoginModule::OnResetUserName(msg_ptr _msg)
{
	MsgHttp *mHttpMsg = _msg->ConvertMsg<MsgHttp>();

	//	判断消息是否正常接收.
	if (mHttpMsg->GetMsgErrorCode() == EnumMsgResponseErrorCode::MessageTrue)
	{
		//	消息正常,获取内容.
		TSharedPtr<FJsonObject> t_jsonObject = mHttpMsg->GetHttpContent();

		TSharedPtr<FJsonObject> mjsonObject = t_jsonObject->GetObjectField(TEXT("data"));

		//	本地广播.
		msg_ptr mBrodcasCheckSucess(new MsgLocal(MessageTypeLocal, m_EditNameSuccess, &mjsonObject));

		MsgCenter::Instance()->SendMsg(mBrodcasCheckSucess);
	}
	else
	{
		// 消息异常,自行根据逻辑判断是否重发.

		// 获取异常内容.
		FString mErrorInfo = mHttpMsg->GetMsgErrorInfo();

		UE_LOG(GEditorLog, Error, TEXT("http response Error %s"), *mErrorInfo);
	}
}

//	获取H5路径--监听事件.
void LoginModule::OnGetHtmlUrlResponseMsg(msg_ptr _msg)
{
	MsgHttp *mHttpMsg = _msg->ConvertMsg<MsgHttp>();

	//	判断消息是否正常接收.
	if (mHttpMsg->GetMsgErrorCode() == EnumMsgResponseErrorCode::MessageTrue)
	{
		//	消息正常,获取内容.
		TSharedPtr<FJsonObject> t_jsonObject = mHttpMsg->GetHttpContent();

		TSharedPtr<FJsonObject> mjsonObject = t_jsonObject->GetObjectField(TEXT("data"));

		m_h5UrlMap.Add(TEXT("protocolUrl"), mjsonObject->GetStringField(TEXT("protocolUrl")));

		m_h5UrlMap.Add(TEXT("mallUrl"), mjsonObject->GetStringField(TEXT("mallUrl")));

		m_h5UrlMap.Add(TEXT("orderUrl"), mjsonObject->GetStringField(TEXT("orderUrl")));

		m_h5UrlMap.Add(TEXT("pcdownloadUrl"), mjsonObject->GetStringField(TEXT("pcdownloadUrl")));

		m_h5UrlMap.Add(TEXT("tutorialUrl"), mjsonObject->GetStringField(TEXT("tutorialUrl")));

		m_h5UrlMap.Add(TEXT("feedbackUrl"), mjsonObject->GetStringField(TEXT("feedbackUrl")));

		m_h5UrlMap.Add(TEXT("contactUrl"), mjsonObject->GetStringField(TEXT("contactUrl")));

		m_h5UrlMap.Add(TEXT("goodsListUrl"), mjsonObject->GetStringField(TEXT("goodsListUrl")));

		//	本地广播.
		msg_ptr mBrodcasCheckSucess(new MsgLocal(MessageTypeLocal, m_GetHtmlUrlSuccess, &m_h5UrlMap));

		MsgCenter::Instance()->SendMsg(mBrodcasCheckSucess);
	}
	else
	{
		// 消息异常,自行根据逻辑判断是否重发.

		// 获取异常内容.
		FString mErrorInfo = mHttpMsg->GetMsgErrorInfo();

		UE_LOG(GEditorLog, Error, TEXT("http response Error %s"), *mErrorInfo);
	}
}

//	获取昵称.
void LoginModule::OnGetNickName(msg_ptr _msg)
{
	MsgHttp *mHttpMsg = _msg->ConvertMsg<MsgHttp>();

	//	判断消息是否正常接收.
	if (mHttpMsg->GetMsgErrorCode() == EnumMsgResponseErrorCode::MessageTrue)
	{
		//	消息正常,获取内容.
		TSharedPtr<FJsonObject> t_jsonObject = mHttpMsg->GetHttpContent();

		TSharedPtr<FJsonObject> mjsonObject = t_jsonObject->GetObjectField(TEXT("data"));

		//TSharedPtr<FJsonObject> mjson = mjsonObject->GetObjectField(TEXT("stylistModel"));

		//TSharedPtr<FJsonObject> mjson2 = mjsonObject->GetObjectField(TEXT("memberModel"));

		FString m_userName = mjsonObject->GetStringField(TEXT("name"));

		FString m_avatarUrl = mjsonObject->GetStringField(TEXT("avatar"));

 		LoginModule* mLoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

		mLoginModule->GetUserData()->SetNickName(m_userName);

		mLoginModule->GetUserData()->SetAvatar(m_avatarUrl);

		//	本地广播.
		msg_ptr mBrodcasCheckSucess(new MsgLocal(MessageTypeLocal, m_GetNickNameSuccess, &mjsonObject));

		MsgCenter::Instance()->SendMsg(mBrodcasCheckSucess);
	}
	else
	{
		// 消息异常,自行根据逻辑判断是否重发.

		// 获取异常内容.
		FString mErrorInfo = mHttpMsg->GetMsgErrorInfo();

		UE_LOG(GEditorLog, Error, TEXT("http response Error %s"), *mErrorInfo);
	}
}

void LoginModule::OnLoginTimeout(msg_ptr _msg)
{
	// 
	MsgLocal *mHttpMsg = _msg->ConvertMsg<MsgLocal>();


	switch (mHttpMsg->m_serverCode)
	{
		case 9000001:// 登录超时自动登录.
		{
			if (!GetUserData()->GetToken().IsEmpty())
			{
				/*FString mUserName;

				FString mUserPasswork;

				UserInfo::Get()->ReadLocalData(mUserName, mUserPasswork);

				RequestLoginServer(mUserName, mUserPasswork);*/
				LoadDataModule * mLoadDataModule = GModuleManager::GetInstance()->GetModule<LoadDataModule>("LoadDataModule");

				mLoadDataModule->ClearStyleDetailData();

				mLoadDataModule->ClearSuitDetailData();

				TDesignTypeManager::GetInstance()->ReleaseAllData();

				// 直接到登录界面.
				UUIManager::GetInstance()->RemoveAllAlertUI();
				   
				UGameDirector::GetInstance()->CloseCurrentLogicScene();

				UGameDirector::GetInstance()->OpenLevel(TEXT("UpdateLevel"));
				break;
			}
			else
			{
				break;
			}
		}
		case 0:
		{
			m_errorMsgInfo = mHttpMsg->m_presentMsg;

			FString mErrorInfo = GLanguageTool::GetInstance()->GetLanguage(TEXT("key138"));
			//// 弹出异常信息.
			UUIManager::GetInstance()->OpenTrueOrFalseAlert(mErrorInfo, this, &LoginModule::OnHttpErrorRepeat, &LoginModule::OnHttpErrorCancel);
			break;
		}
		default:
		{
			
			break;
		}
	}

	
}

void LoginModule::OnHttpErrorCancel()
{
	UUIManager::GetInstance()->RemoveAllAlertUI();

	UGameDirector::GetInstance()->CloseAllLogicScene();

	// 直接跳转到更新界面.
	UGameDirector::GetInstance()->OpenLevel(TEXT("UpdateLevel"));
}

void LoginModule::OnHttpErrorRepeat()
{
	UUIManager::GetInstance()->RemoveAllAlertUI();

	MsgCenter::Instance()->SendMsg(m_errorMsgInfo);
}


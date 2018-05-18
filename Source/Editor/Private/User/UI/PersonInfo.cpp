// Fill out your copyright notice in the Description page of Project Settings.

#include "PersonInfo.h"
#include "UIManager.h"
#include "MsgCenter.h"
#include "GFileHelper.h"
#include "GController.h"
#include "GameDirector.h"
#include "GLanguageTool.h"
#include "LogicMainScene.h"
#include "GModuleManager.h"
#include "LogicLoginScene.h"
#include "LogicDesignScene.h"
#include "MainUIController.h"
#include "DesignUIController.h"
#include "GPlatformFileManager.h"
#include "EditableTextBox.h"
#include "Tools/EditorFunctionalTool.h"

UPersonInfo::UPersonInfo(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	//初始化变量.
	m_BtnBack = nullptr;
	m_BtnAvatar = nullptr;
	m_BtnSetting = nullptr;
	m_BtnLanguage = nullptr;
	m_BtnUseCourse = nullptr;
	m_BtnFeedBack = nullptr;
	m_BtnAbout = nullptr;
	m_BtnUserExit = nullptr;

	m_TextPerson = nullptr;
	m_TextUserName = nullptr;
	m_TextSetting = nullptr;
	m_TextSettingMsg = nullptr;
	m_TextLanguage = nullptr;
	m_TextLanguageMsg = nullptr;
	m_TextCourse = nullptr;
	m_TextSuggest = nullptr;
	m_TextAbout = nullptr;
	m_TextVersion = nullptr;
	m_TextExit = nullptr;

	m_EditName = false;
}

bool UPersonInfo::OnStart()
{
	LoginModule *mLoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	//	用户头像图片.
	if (UImage* ImageHead = Cast<UImage>(GetWidgetFromName("Image_Head")))
	{
		m_ImageHead = ImageHead;	
	}



	//	用户昵称字段.
	if (UEditableTextBox* TextUserName = Cast<UEditableTextBox>(GetWidgetFromName("EditableTextBox_UserName")))
	{
		m_TextUserName = TextUserName;
	}
	//	修改昵称成功--回调函数.
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, mLoginModule->m_EditNameSuccess, this, &UPersonInfo::OnUserNameResponseMsg);





	//	修改用户头像按钮.
	if (UButton* BtnAvatar = Cast<UButton>(GetWidgetFromName("Button_Head")))
	{
		m_BtnAvatar = BtnAvatar;

		UUIManager::GetInstance()->RegisterButtonClick(1, m_BtnAvatar, this, &UPersonInfo::OpenPageCell);
	}
	//	修改头像成功--回调函数.
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, mLoginModule->m_GetUserHeadSuccess, this, &UPersonInfo::OnUserHeadResponseMsg);




	//	本地监听获取用户头像和昵称消息回调.
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, mLoginModule->m_GetNickNameSuccess, this, &UPersonInfo::OnGetUserHeadResponseMsg);
	
	//	请求服务器获取昵称和头像.
	mLoginModule->RequestNickName();




	//	返回上一页面按钮.
	if (UButton* BtnBack = Cast<UButton>(GetWidgetFromName("Button_Back")))
	{
		m_BtnBack = BtnBack;

		//	返回上一页面按钮--点击事件.
		UUIManager::GetInstance()->RegisterButtonClick(2, m_BtnBack, this, &UPersonInfo::OpenPageCell);
	}

	//	画质设置按钮.
	if (UButton* BtnSetting = Cast<UButton>(GetWidgetFromName("Button_Setting")))
	{
		m_BtnSetting = BtnSetting;

		//	画质设置按钮--点击事件.
		UUIManager::GetInstance()->RegisterButtonClick(3, m_BtnSetting, this, &UPersonInfo::OpenPageCell);
 	}

	//	语言设置按钮.
	if (UButton* BtnLanguage = Cast<UButton>(GetWidgetFromName("Button_Language")))
	{
		m_BtnLanguage = BtnLanguage;

		//	语言设置按钮--点击事件.
		UUIManager::GetInstance()->RegisterButtonClick(4, m_BtnLanguage, this, &UPersonInfo::OpenPageCell);
	}

	//	使用教程按钮.
	if (UButton* BtnCourse = Cast<UButton>(GetWidgetFromName("Button_Course")))
	{
		m_BtnUseCourse = BtnCourse;

		//	使用教程按钮--点击事件.
		UUIManager::GetInstance()->RegisterButtonClick(5, m_BtnUseCourse, this, &UPersonInfo::OpenPageCell);
	}

	//	建议反馈按钮.
	if (UButton* BtnSuggest = Cast<UButton>(GetWidgetFromName("Button_Suggest")))
	{
		m_BtnFeedBack = BtnSuggest;

		//	建议反馈按钮--点击事件.
		UUIManager::GetInstance()->RegisterButtonClick(6, m_BtnFeedBack, this, &UPersonInfo::OpenPageCell);
	}

	//	关于我们按钮.
	if (UButton* BtnAbout = Cast<UButton>(GetWidgetFromName("Button_About")))
	{
		m_BtnAbout = BtnAbout;

		//	关于我们按钮--点击事件.
		UUIManager::GetInstance()->RegisterButtonClick(7, m_BtnAbout, this, &UPersonInfo::OpenPageCell);
	}

	//	清理缓存.
	if (UButton* BtnClear = Cast<UButton>(GetWidgetFromName("Button_Clear")))
	{
		m_BtnClear = BtnClear;

		//	清理缓存--点击事件.
		UUIManager::GetInstance()->RegisterButtonClick(8, m_BtnClear, this, &UPersonInfo::OpenPageCell);
	}

	//	退出按钮.
	if (UButton* BtnExit = Cast<UButton>(GetWidgetFromName("Button_Exit")))
	{
		m_BtnUserExit = BtnExit;

		//	退出按钮--点击事件.
		UUIManager::GetInstance()->RegisterButtonClick(9, m_BtnUserExit, this, &UPersonInfo::OpenPageCell);
	}

	//	我的字段.
	if (UTextBlock* TextPerson = Cast<UTextBlock>(GetWidgetFromName("Text_Person")))
	{
		m_TextPerson = TextPerson;

		// 获取文字.
		FString mPerson = GLanguageTool::GetInstance()->GetLanguage(TEXT("key114"));
		m_TextPerson->SetText(FText::FromString(mPerson));
	}

	//	画质设置信息字段.
	if (UTextBlock* TextSetting = Cast<UTextBlock>(GetWidgetFromName("Text_Setting")))
	{
		m_TextSetting = TextSetting;

		// 获取文字.
		FString mSetting = GLanguageTool::GetInstance()->GetLanguage(TEXT("key107"));
		m_TextSetting->SetText(FText::FromString(mSetting));
	}
	//	画质设置内容.
	if (UTextBlock* TextSettingMsg = Cast<UTextBlock>(GetWidgetFromName("Text_SettingMsg")))
	{
		m_TextSettingMsg = TextSettingMsg;

		EQualityLevel mDisplayLevel = mLoginModule->GetUserData()->GetSettingDisplayLevel();

		if (EQualityLevel::E_Low == mDisplayLevel)
		{
			// 获取文字.
			FString mSetting = GLanguageTool::GetInstance()->GetLanguage(TEXT("key125"));
			m_TextSettingMsg->SetText(FText::FromString(mSetting));
		}

		else if (EQualityLevel::E_High == mDisplayLevel) 
		{
			// 获取文字.
			FString mSetting = GLanguageTool::GetInstance()->GetLanguage(TEXT("key126"));
			m_TextSettingMsg->SetText(FText::FromString(mSetting));
		}

		else if (EQualityLevel::E_Highest == mDisplayLevel)
		{
			// 获取文字.
			FString mSetting = GLanguageTool::GetInstance()->GetLanguage(TEXT("key127"));
			m_TextSettingMsg->SetText(FText::FromString(mSetting));
		}
	}

	//	语言设置信息.
	if (UTextBlock* TextLanguage = Cast<UTextBlock>(GetWidgetFromName("Text_Language")))
	{
		m_TextLanguage = TextLanguage;

		// 获取文字.
		FString mLanguage = GLanguageTool::GetInstance()->GetLanguage(TEXT("key108"));
		m_TextLanguage->SetText(FText::FromString(mLanguage));

	}
	//	语言设置内容.
	if (UTextBlock* TextLanguageMsg = Cast<UTextBlock>(GetWidgetFromName("Text_LanguageMsg")))
	{
		m_TextLanguageMsg = TextLanguageMsg;

		if (ENUM_LANGUAGE::GCHINESE == GController::m_language)
		{
			FString mLanguageMsg = GLanguageTool::GetInstance()->GetLanguage(TEXT("key118"));
			m_TextLanguageMsg->SetText(FText::FromString(mLanguageMsg));
		}

		if (ENUM_LANGUAGE::GENGLISH == GController::m_language)
		{
			FString mLanguageMsg = GLanguageTool::GetInstance()->GetLanguage(TEXT("key119"));
			m_TextLanguageMsg->SetText(FText::FromString(mLanguageMsg));
		}
	}

	//	使用教程信息.
	if (UTextBlock* TextCourse = Cast<UTextBlock>(GetWidgetFromName("Text_Course")))
	{
		m_TextCourse = TextCourse;

		// 获取文字.
		FString mCourse = GLanguageTool::GetInstance()->GetLanguage(TEXT("key109"));
		m_TextCourse->SetText(FText::FromString(mCourse));
	}

	//	建议反馈信息.
	if (UTextBlock* TextSuggest = Cast<UTextBlock>(GetWidgetFromName("Text_Suggest")))
	{
		m_TextSuggest = TextSuggest;

		// 获取文字.
		FString mSuggest = GLanguageTool::GetInstance()->GetLanguage(TEXT("key110"));
		m_TextSuggest->SetText(FText::FromString(mSuggest));
	}

	//	关于我们信息.
	if (UTextBlock* TextAbout = Cast<UTextBlock>(GetWidgetFromName("Text_About")))
	{
		m_TextAbout = TextAbout;

		// 获取文字.
		FString mAbout = GLanguageTool::GetInstance()->GetLanguage(TEXT("key111"));
		m_TextAbout->SetText(FText::FromString(mAbout));
	}

	//	清理缓存信息.
	if (UTextBlock* TextClear = Cast<UTextBlock>(GetWidgetFromName("Text_Clear")))
	{
		m_TextClear = TextClear;

		// 获取文字.
		FString mClear = GLanguageTool::GetInstance()->GetLanguage(TEXT("key136"));
		m_TextClear->SetText(FText::FromString(mClear));
	}

	//	版本信息.
	if (UTextBlock* TextVersion = Cast<UTextBlock>(GetWidgetFromName("Text_Version")))
	{
		m_TextVersion = TextVersion;

		// 获取文字.
		FString mVersion = GLanguageTool::GetInstance()->GetLanguage(TEXT("key112"));
		m_TextVersion->SetText(FText::FromString(mVersion));
	}
	//	版本信息内容.
	if (UTextBlock* TextVersionMsg = Cast<UTextBlock>(GetWidgetFromName("Text_VersionMsg")))
	{
		m_TextVersionMsg = TextVersionMsg;

		m_TextVersionMsg->SetText(FText::FromString("V" + GController::m_appVersion));
	}

	//	退出字段.
	if (UTextBlock* TextEixt = Cast<UTextBlock>(GetWidgetFromName("Text_Exit")))
	{
		m_TextExit = TextEixt;

		// 获取文字.
		FString mExit = GLanguageTool::GetInstance()->GetLanguage(TEXT("key113"));
		m_TextExit->SetText(FText::FromString(mExit));
	}
	return true;
}

void UPersonInfo::OnInit()
{

}

void UPersonInfo::OnRelease()
{
	LoginModule *mLoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, mLoginModule->m_EditNameSuccess, this, &UPersonInfo::OnUserNameResponseMsg);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, mLoginModule->m_GetUserHeadSuccess, this, &UPersonInfo::OnUserHeadResponseMsg);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, mLoginModule->m_GetNickNameSuccess, this, &UPersonInfo::OnGetUserHeadResponseMsg);

	UUIManager::GetInstance()->RemoveButtonClick(m_BtnBack);
	UUIManager::GetInstance()->RemoveButtonClick(m_BtnAvatar);
	UUIManager::GetInstance()->RemoveButtonClick(m_BtnSetting);
	UUIManager::GetInstance()->RemoveButtonClick(m_BtnLanguage);
	UUIManager::GetInstance()->RemoveButtonClick(m_BtnUseCourse);
	UUIManager::GetInstance()->RemoveButtonClick(m_BtnFeedBack);
	UUIManager::GetInstance()->RemoveButtonClick(m_BtnAbout);
	UUIManager::GetInstance()->RemoveButtonClick(m_BtnClear);
	UUIManager::GetInstance()->RemoveButtonClick(m_BtnUserExit);

	if (nullptr != m_DownloadTool)
		m_DownloadTool->CancelDownload();

	m_DownloadTool = nullptr;
}

void UPersonInfo::OnSetParams(int32 _pid, FString & _content, ...)
{

}

void UPersonInfo::OnTick(float InDeltaTimeSecond)
{

}

void UPersonInfo::OnDownloadImage(bool _isScuccess, int32 _penetrateId, FString _url, UTexture2D * _texture2d, UDownloadImageTool * _downloadTool)
{
	m_ImageHead->GetDynamicMaterial()->SetTextureParameterValue("headTexture", _texture2d);

	m_ImageHead->Brush.ImageSize = FVector2D(220.0f, 220.0f);

	BaseLogicScene * mScene = UGameDirector::GetInstance()->GetCurrentLogicScene();

	UIController*  mController = mScene->GetCurrentUIController();

	mController->GetTextureManager()->AddTexture2D(_url, _texture2d);
}

//	收到服务器发给本地的修改用户昵称成功的消息.
void UPersonInfo::OnUserNameResponseMsg(msg_ptr _msg)
{
	LoginModule *mLoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	//FString mUserName = m_TextUserName->GetText().ToString();
	
	FString mUserName = m_UserNick;

	FString mNickName = mUserName.TrimStartAndEnd(); 

	mLoginModule->GetUserData()->SetNickName(mNickName);

	if (mNickName.Len() <= 11)
	{
		m_TextUserName->SetText(FText::FromString(mNickName));
	}
	else
	{
		int32 mLen = mNickName.Len() - 11;
		m_TextUserName->SetText(FText::FromString(mNickName.LeftChop(mLen)+"..."));
	}
}

//	收到服务器发给本地修改头像昵称成功的消息.
void UPersonInfo::OnUserHeadResponseMsg(msg_ptr _msg)
{
	UUIManager::GetInstance()->RemoveAllAlertUI();

	LoginModule *mLoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	//	请求服务器获取昵称和头像.
	mLoginModule->RequestNickName();
}

//	收到服务器发给本地获取头像和昵称成功的消息.
void UPersonInfo::OnGetUserHeadResponseMsg(msg_ptr _msg)
{
	LoginModule *mLoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	//	获取头像.
	FString mImageUrl = mLoginModule->GetUserData()->GetUserAvatar();

	UE_LOG(LogTemp, Log, TEXT("PersonInfo监听的头像图片：=%s"), *mImageUrl);

	//	判断缓存是否存在.
	BaseLogicScene * mScene = UGameDirector::GetInstance()->GetCurrentLogicScene();

	UIController*  mController = mScene->GetCurrentUIController();

	UTexture2D * mDetailTexture = mController->GetTextureManager()->GetTexture2d(mImageUrl);

	if (mDetailTexture == nullptr)
	{
		if (mImageUrl.Len() > 0)
		{
			if (m_DownloadTool != nullptr)
			{
				m_DownloadTool->CancelDownload();
			}
			m_DownloadTool = UDownloadImageTool::DownloadImage(1, mImageUrl, this, &UPersonInfo::OnDownloadImage);
		}
	}
	else
	{
		if (nullptr != m_ImageHead)
		{
			m_ImageHead->GetDynamicMaterial()->SetTextureParameterValue("headTexture", mDetailTexture);

			m_ImageHead->Brush.ImageSize = FVector2D(220.0f, 220.0f);
		}
	}

	//	获取昵称.
	FString mNickName = mLoginModule->GetUserData()->GetVisualUserName();

	UE_LOG(LogTemp, Log, TEXT("PersonInfo监听的昵称：=%s"), *mNickName);

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

//	修改用户昵称.
void UPersonInfo::EditUserName(FText InText)
{
	FString mUserName = InText.ToString();

	m_UserNick = mUserName;

	FString mNickName = mUserName.TrimStartAndEnd();

	if (mNickName.Len() > 0)
	{
		LoginModule *mLoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

		FString mAvatar = mLoginModule->GetUserData()->GetUserAvatar();

		mLoginModule->RequestUserNameReset(mAvatar, mNickName);
	}
	else
	{
		LogicDesignScene * mDesignScene = (LogicDesignScene *)UGameDirector::GetInstance()->GetCurrentLogicScene();

		FString mAlertText = GLanguageTool::GetInstance()->GetLanguage(TEXT("key137"));

		UUIManager::GetInstance()->OpenMiddleTip2(mAlertText, 2.0f);

		OnEditNameError();
	}
}

void UPersonInfo::OpenPageCell(int32 _pid)
{
	LoginModule *mLoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	DesignModule *mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

	//	修改用户头像按钮.
	if (_pid == 1)
	{
		//	调用照相机.
#if PLATFORM_ANDROID
		
		mDesignModule->m_UploadPattern = false;

		m_PhotoPage = (UPhotoPage *)UUIManager::GetInstance()->OpenUI(TEXT("UI_Photo"));

		m_PhotoPage->AddToViewport();

#elif PLATFORM_IOS
		m_PhotoPage = (UPhotoPage *)UUIManager::GetInstance()->OpenUI(TEXT("UI_Photo"));

		m_PhotoPage->AddToViewport();

		m_PhotoPage->SetVisibility(ESlateVisibility::Hidden);

		m_PhotoPage->GetUploadPicture();
#endif

#if PLATFORM_WINDOWS
		m_PhotoPage = (UPhotoPage *)UUIManager::GetInstance()->OpenUI(TEXT("UI_Photo"));

		m_PhotoPage->AddToViewport();

		m_PhotoPage->SetVisibility(ESlateVisibility::Hidden);

		m_PhotoPage->GetUploadPicture();
#endif
	}

	//	返回上一页面按钮.
	if (_pid == 2)
	{
		this->RemoveFromViewport();

		LogicMainScene * mMainScene = (LogicMainScene *)UGameDirector::GetInstance()->GetCurrentLogicScene();

		MainUIController*  mMainController = (MainUIController*)(mMainScene->GetCurrentUIController());

		mMainController->ChangePageTo(PAGE_TYPE::MAIN_PAGE);

		//	本地广播.
// 		MainModule *mMainModule = GModuleManager::GetInstance()->GetModule<MainModule>("MainModule");
// 
// 		TSharedPtr<FJsonObject> mjson = nullptr;
// 
// 		msg_ptr mBrodcasCheckSucess(new MsgLocal(MessageTypeLocal, mMainModule->m_PersonInfo_Back, &mjson));
// 
// 		MsgCenter::Instance()->SendMsg(mBrodcasCheckSucess);
	}

	//	点击画质设置.
	if (_pid == 3)
	{
		LogicMainScene * mMainScene = (LogicMainScene *)UGameDirector::GetInstance()->GetCurrentLogicScene();

		MainUIController*  mMainController = (MainUIController*)(mMainScene->GetCurrentUIController());

		mMainController->ChangePageTo(PAGE_TYPE::DisplaySetting_PAGE);
	}

	//	点击语言设置.
	if (_pid == 4)
	{
		LogicMainScene * mMainScene = (LogicMainScene *)UGameDirector::GetInstance()->GetCurrentLogicScene();

		MainUIController*  mMainController = (MainUIController*)(mMainScene->GetCurrentUIController());

		mMainController->ChangePageTo(PAGE_TYPE::LanguageSetting_PAGE);
	}

	//	使用教程.
	if (_pid == 5)
	{	
		if (nullptr != mLoginModule->GetUserData())
			UHaoshaSDKBPLibrary::OpenWebview(EditorFunctionalTool::GetTicketUrl(mLoginModule->GetUserData()->GetUseCourseURL()));
	}

	//	建议反馈.
	if (_pid == 6)
	{
		if (nullptr != mLoginModule->GetUserData())
			UHaoshaSDKBPLibrary::OpenWebview(EditorFunctionalTool::GetTicketUrl(mLoginModule->GetUserData()->GetFeedbackURL()));
	}

	//	联系我们.
	if (_pid == 7)
	{
		if (nullptr != mLoginModule->GetUserData())
			UHaoshaSDKBPLibrary::OpenWebview(EditorFunctionalTool::GetTicketUrl(mLoginModule->GetUserData()->GetContactURL()));
	}

	//	清理缓存.
	if (_pid == 8)
	{
		FString mPersistenDir = GFileHelper::ProjectPersistentDownloadDir();

		FString mDeletePath = FString::Printf(TEXT("%s/Pattern"), *mPersistenDir);

		GPlatformFileManager::DeleteDirectory(*mDeletePath);

		mDeletePath = FString::Printf(TEXT("%s/styleIcon"), *mPersistenDir);

		GPlatformFileManager::DeleteDirectory(*mDeletePath);

		FString mTipContent = GLanguageTool::GetInstance()->GetLanguage(TEXT("key143"));

		UUIManager::GetInstance()->OpenMiddleTip(mTipContent, 2.0f);
	}

	//	退出登录.
	else if (_pid == 9)
	{
 		this->RemoveFromViewport();

		mLoginModule->LogOut();
	}
}

//	修改昵称失败.
void UPersonInfo::OnEditNameError()
{
	LoginModule *mLoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	FString mNickName = mLoginModule->GetUserData()->GetVisualUserName();

	if (mNickName.Len() > 11)
	{
		int32 mLen = mNickName.Len() - 11;

		m_TextUserName->SetText(FText::FromString(mNickName.LeftChop(mLen) + "..."));
	}

	else
	{
		m_TextUserName->SetText(FText::FromString(mNickName));
	}
}


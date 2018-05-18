#include "DesignModule.h"
#include "SuitT.h"
#include "OSSHelper.h"
#include "GFileHelper.h"
#include "LoginModule.h"
#include "IOSSDownload.h"
#include "TSuitManager.h"
#include "GameDirector.h"
#include "GModuleManager.h"
#include "TSuitPartManager.h"
#include "LogicDesignScene.h"
#include "TSuitSliceManager.h"
#include "HaoshaSDKBPLibrary.h"
#include "TSuitStitchManager.h"
#include "GPlatformFileManager.h"
#include "TSuitPatternManager.h"
#include "ImageMagickBPLibrary.h"
#include "TSuitComponentManager.h"
#include "TSuitAccessoriesManager.h"

GREGISTER_MODULE(DesignModule)

void DesignData::ClearData()
{
	m_designRID = 0;

	m_designTID = 0;
}

DesignModule::DesignModule()
{
}

DesignModule::~DesignModule()
{

}
void DesignModule::SetDesignData(int32 _designRID, int32 _desginTID, EnumDesignType _designType)
{
	m_designData->m_designRID = _designRID;

	m_designData->m_designTID = _desginTID;

	m_designData->m_DesignType = _designType;

	// 临时测试写死从模板数据渲染.
	//m_designData->m_DesignType = EnumDesignType::DESIGN_BY_SUIT;
}

FString DesignModule::GetOssDateUrl(FString _inNativeFullUrl)
{
	FString mName = GPlatformFileManager::GetFilePathNamePart(*_inNativeFullUrl);
	FString mExt = GPlatformFileManager::GetFilePathExtPart(*_inNativeFullUrl);
	FString mNamePointExt = mName + "." + mExt;
	return FString("Pattern/UserPattern/") + GPlatformFileManager::CreateDateUrl2() + mNamePointExt;
}

FString DesignModule::GetOssDateUrl2(FString _inNativeFullUrl)
{
	FString mName = GPlatformFileManager::GetFilePathNamePart(*_inNativeFullUrl);
	FString mExt = GPlatformFileManager::GetFilePathExtPart(*_inNativeFullUrl);
	FString mNamePointExt = mName + "." + mExt;
	return FString("UserPattern/") + GPlatformFileManager::CreateDateUrl2() + mNamePointExt;
}

FString DesignModule::GetDateUrl(TCHAR * _inFilePath)
{
	FString mRet;
	FString fileName = GPlatformFileManager::GetFileName(_inFilePath);
	FString fileExt = GPlatformFileManager::GetFilePathExtPart(_inFilePath);
	mRet = GPlatformFileManager::CreateDateUrl() + "/" + fileName + "." + fileExt;
	return mRet;
}

FString DesignModule::GetHeadOssDateUrl(FString _inNativeFullUrl)
{
	FString mName = GPlatformFileManager::GetFilePathNamePart(*_inNativeFullUrl);
	FString mExt = GPlatformFileManager::GetFilePathExtPart(*_inNativeFullUrl);
	FString mNamePointExt = mName + "." + mExt;
	return FString("images/avatar/") + GPlatformFileManager::CreateDateUrl2() + mNamePointExt;
}

FString DesignModule::GetHeadOssDateUrl2(FString _inNativeFullUrl)
{
	FString mName = GPlatformFileManager::GetFilePathNamePart(*_inNativeFullUrl);
	FString mExt = GPlatformFileManager::GetFilePathExtPart(*_inNativeFullUrl);
	FString mNamePointExt = mName + "." + mExt;
	return FString("GameResource/images/avatar/") + GPlatformFileManager::CreateDateUrl2() + mNamePointExt;
}

FString DesignModule::GetHeadDateUrl(TCHAR * _inFilePath)
{
	FString mRet;
	FString fileName = GPlatformFileManager::GetFileName(_inFilePath);
	FString fileExt = GPlatformFileManager::GetFilePathExtPart(_inFilePath);
	mRet = GPlatformFileManager::CreateDateUrl() + "/" + fileName + "." + fileExt;
	return mRet;
}

DesignData * DesignModule::GetDesignData()
{
	return m_designData;
}

TArray<SuitPartT*> DesignModule::GetSuitPartArray()
{
	TArray<SuitPartT*> mPartArray;

	int32 mSuitTid = m_designData->m_designTID;

	SuitT * mCurrentSuitT = TSuitManager::GetInstance()->GetTemplateByPID<SuitT>(mSuitTid);

	TArray<int32> mPartIds = mCurrentSuitT->m_ArrayPartId;

	for (int32 i=0;i<mPartIds.Num();i++)
	{
		SuitPartT * mPartT = TSuitPartManager::GetInstance()->GetTemplateByPID<SuitPartT>(mPartIds[i]);

		mPartArray.Add(mPartT);
	}

	return mPartArray;
}

TArray<SuitComponentT*> DesignModule::GetSuitComponentArray(int32 _SuitPartTid)
{
	TArray<SuitComponentT*> mSuitComponentArray;

	SuitPartT * mCurrentSuitPartT = TSuitPartManager::GetInstance()->GetTemplateByPID<SuitPartT>(_SuitPartTid);

	TArray<int32> mSuitComponentIds = mCurrentSuitPartT->m_SuitComponentIds;

	for (int32 i = 0; i < mSuitComponentIds.Num(); i++)
	{
		SuitComponentT * mTemp = TSuitComponentManager::GetInstance()->GetTemplateByPID<SuitComponentT>(mSuitComponentIds[i]);
		mSuitComponentArray.Add(mTemp);
	}
	return mSuitComponentArray;
}

TArray<SuitSliceT*> DesignModule::GetSuitSliceArray(int32 _SuitComponentTid)
{
	TArray<SuitSliceT*> mSuitSliceTArray;

	SuitComponentT * mCurrentSuitComponentT = TSuitComponentManager::GetInstance()->GetTemplateByPID<SuitComponentT>(_SuitComponentTid);

	TArray<int32> mSuitSliceIds = mCurrentSuitComponentT->m_SuitSliceIds;

	for (int32 i = 0; i < mSuitSliceIds.Num(); i++)
	{
		SuitSliceT * mTemp = TSuitSliceManager::GetInstance()->GetTemplateByPID<SuitSliceT>(mSuitSliceIds[i]);
		mSuitSliceTArray.Add(mTemp);
	}
	return mSuitSliceTArray;
}

TArray<SuitAccessoriesT*> DesignModule::GetSuitAccessoriesArray(int32 _SuitComponentTid)
{
	TArray<SuitAccessoriesT*> mSuitAccessoriesTArray;

	SuitComponentT * mCurrentSuitComponentT = TSuitComponentManager::GetInstance()->GetTemplateByPID<SuitComponentT>(_SuitComponentTid);

	TArray<int32> mSuitAccessoriesIds = mCurrentSuitComponentT->m_SuitAccessoriesIds;

	for (int32 i = 0; i < mSuitAccessoriesIds.Num(); i++)
	{
		SuitAccessoriesT * mTemp = TSuitAccessoriesManager::GetInstance()->GetTemplateByPID<SuitAccessoriesT>(mSuitAccessoriesIds[i]);
		mSuitAccessoriesTArray.Add(mTemp);
	}
	return mSuitAccessoriesTArray;
}

TArray<SuitStitchT*> DesignModule::GetSuitStitchArray(int32 _SuitComponentTid)
{
	TArray<SuitStitchT*> mSuitStitchTArray;

	SuitComponentT * mCurrentSuitComponentT = TSuitComponentManager::GetInstance()->GetTemplateByPID<SuitComponentT>(_SuitComponentTid);

	TArray<int32> mSuitStitchIds = mCurrentSuitComponentT->m_SuitStitchsuitIds;

	for (int32 i = 0; i < mSuitStitchIds.Num(); i++)
	{
		SuitStitchT * mTemp = TSuitStitchManager::GetInstance()->GetTemplateByPID<SuitStitchT>(mSuitStitchIds[i]);
		mSuitStitchTArray.Add(mTemp);
	}
	return mSuitStitchTArray;
}

void DesignModule::OnLoad()
{
	m_designData = new DesignData();

	m_LoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	//	注册监听消息.
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeHttpResponse, 2002, this, &DesignModule::OnShareFriendResponseMsg);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeHttpResponse, 2006, this, &DesignModule::OnReqUploadPatternJson);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeHttpResponse, 2001, this, &DesignModule::OnUploadHeadSuccess);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeHttpResponse, 2007, this, &DesignModule::OnBuyInfoResponseMsg); 
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeHttpResponse, 2008, this, &DesignModule::OnShopOrderResponseMsg);
}

void DesignModule::OnRelease()
{
	delete m_designData;

	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeHttpResponse, 2002, this, &DesignModule::OnShareFriendResponseMsg);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeHttpResponse, 2006, this, &DesignModule::OnReqUploadPatternJson);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeHttpResponse, 2001, this, &DesignModule::OnUploadHeadSuccess);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeHttpResponse, 2007, this, &DesignModule::OnBuyInfoResponseMsg);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeHttpResponse, 2008, this, &DesignModule::OnShopOrderResponseMsg);
}
void DesignModule::RequestShopOrder(int32 _StyleID)
{
	TSharedPtr<FJsonObject> t_jsonObject = MakeShareable(new FJsonObject);

	t_jsonObject->SetNumberField(TEXT("styleId"), _StyleID);

	FString mToken = m_LoginModule->GetUserData()->GetToken();

	FString mOperator = m_LoginModule->GetUserData()->GetUserId();

	msg_ptr mMsg(new MsgHttp(MessageTypeHttpRequest, 2008, mToken, mOperator, false, t_jsonObject));

	MsgCenter::Instance()->SendMsg(mMsg);
}

//	请求服务器分享给朋友.
void DesignModule::RequestShareFriendServer(int32 _StyleID)
{
	TSharedPtr<FJsonObject> t_jsonObject = MakeShareable(new FJsonObject);

	t_jsonObject->SetNumberField(TEXT("styleId"), _StyleID);

	FString mToken = m_LoginModule->GetUserData()->GetToken();

	FString mOperator = m_LoginModule->GetUserData()->GetUserId();

	msg_ptr mMsg(new MsgHttp(MessageTypeHttpRequest, 2002, mToken, mOperator, false, t_jsonObject));

	MsgCenter::Instance()->SendMsg(mMsg);
}

//	请求服务器发送自定义花型到服务器.
void DesignModule::ReqUploadPatternJson(FString ossIconUrl, FString nativeIconUrl, float ossHeight, float ossWidth)
{
	FString m_OssIconUrl = GetOssDateUrl2(ossIconUrl);

	FString m_NativeIconUrl = GetOssDateUrl2(nativeIconUrl);

	m_LoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	TSharedPtr<FJsonObject> t_jsonObject = MakeShareable(new FJsonObject);

	t_jsonObject->SetStringField(TEXT("OwnerId"), m_LoginModule->GetUserData()->GetUserId());
	t_jsonObject->SetStringField(TEXT("PropertyId1"), "");
	t_jsonObject->SetStringField(TEXT("PropertyValueId1"), "");
	t_jsonObject->SetStringField(TEXT("PropertyId2"), "");
	t_jsonObject->SetStringField(TEXT("PropertyValueId2"), "");
	t_jsonObject->SetStringField(TEXT("IconUrl"), m_OssIconUrl);
	t_jsonObject->SetStringField(TEXT("ImageUrl"), m_NativeIconUrl);
	t_jsonObject->SetNumberField(TEXT("Height"), ossHeight);
	t_jsonObject->SetNumberField(TEXT("Width"), ossWidth);

	FString mToken = m_LoginModule->GetUserData()->GetToken();

	FString mOperator = m_LoginModule->GetUserData()->GetUserId();

	msg_ptr mMsg(new MsgHttp(MessageTypeHttpRequest, 2006, mToken, mOperator, false, t_jsonObject));

	UE_LOG(GEditorLog, Error, TEXT("zhoujia:ReqUploadPatternJson"));

	MsgCenter::Instance()->SendMsg(mMsg);
}

//	请求上传花型到OSS.
void DesignModule::UploadPatternToOss(FString ossIconUrl, FString nativeIconUrl, FString ossPatternUrl, FString nativePatternUrl, float width, float height)
{
	m_UploadInfoStruct.m_IconOssUrl = ossIconUrl;

	m_UploadInfoStruct.m_PatternOssUrl = ossPatternUrl;

	m_UploadInfoStruct.m_Width = width;

	m_UploadInfoStruct.m_Height = height;
	
	m_UploadInfoStruct.m_IconNativeUrl = nativeIconUrl;

	m_UploadInfoStruct.m_TextureNativeUrl = nativePatternUrl;

	FString mIconOssUrl = GetOssDateUrl(nativeIconUrl);

	FString mPatternOssUrl = GetOssDateUrl(nativePatternUrl);

	if (GPlatformFileManager::FileExists(*nativeIconUrl) && GPlatformFileManager::FileExists(*nativePatternUrl))
	{
		FOssUploadQueueParameter inUploadParameters;

		//路径. 
		TArray<FOssReqeustParameter> FilePaths;

		//icon.
		FilePaths.Add(FOssReqeustParameter(mIconOssUrl, nativeIconUrl));

		//texture.
		FilePaths.Add(FOssReqeustParameter(mPatternOssUrl, nativePatternUrl));

		//完成回调,回调函数必须指明类.

		FOnOSSRequestFinishedDelegate OnOSSRequestFinished(this, &DesignModule::OnUploadPatternToOssSuccess);

		FOnOSSFileUploadProgressDelegate OnOSSFileUploadProgressDelegate(this, &DesignModule::OnUploadUserPatternToOssProgress);

		inUploadParameters.OnOSSRequestFinished = OnOSSRequestFinished;

		inUploadParameters.OnOSSFileUploadProgress = OnOSSFileUploadProgressDelegate;

		inUploadParameters.FilePaths = FilePaths;

		//开始上传.
		OSSHelper::GetInstance()->GetOSSTool()->Upload(inUploadParameters);
	}
}

//	上传花型json服务器回调
void DesignModule::OnReqUploadPatternJson(msg_ptr _msg)
{
	MsgHttp *mHttpMsg = _msg->ConvertMsg<MsgHttp>();

	// 1.先判断消息是否正常接收.
	if (mHttpMsg->GetMsgErrorCode() == EnumMsgResponseErrorCode::MessageTrue)
	{
		// 2.消息正常,获取内容.
		TSharedPtr<FJsonObject> t_jsonObject = mHttpMsg->GetHttpContent();

		FString mNewPatternId;
		t_jsonObject->TryGetStringField(TEXT("data"), mNewPatternId);

		// 发送上传花型成功的广播.
		msg_ptr mBrodcastShareFriendSucess(new MsgLocal(MessageTypeLocal, m_Http_UploadPattern_Pid, &mNewPatternId));

		UE_LOG(LogTemp, Log, TEXT("zhoujia:OnReqUploadPatternJson"));

		MsgCenter::Instance()->SendMsg(mBrodcastShareFriendSucess);
	}
	else
	{
		// 消息异常,自行根据逻辑判断是否重发.

		// 获取异常内容.
		FString mErrorInfo = mHttpMsg->GetMsgErrorInfo();

		UE_LOG(GEditorLog, Error, TEXT("http response Error %s"), *mErrorInfo);
	}
}

//	上传Oss成功的回调.
void DesignModule::OnUploadPatternToOssSuccess(bool res)
{
	/*if (GPlatformFileManager::FileExists(*m_UploadInfoStruct.m_IconNativeUrl))
	{
		GPlatformFileManager::DeleteFile(m_UploadInfoStruct.m_IconNativeUrl);
	}
	if (GPlatformFileManager::FileExists(*m_UploadInfoStruct.m_TextureNativeUrl))
	{
		GPlatformFileManager::DeleteFile(m_UploadInfoStruct.m_TextureNativeUrl);
	}*/
	
	UE_LOG(GEditorLog, Error, TEXT("zhoujia:OnUploadPatternToOssSuccess"));

	//发送json数据到服务器.
	ReqUploadPatternJson(m_UploadInfoStruct.m_IconOssUrl, m_UploadInfoStruct.m_PatternOssUrl, m_UploadInfoStruct.m_Height, m_UploadInfoStruct.m_Width);
}

//	请求上传头像到服务器.
void DesignModule::ReqUploadHeadJson(FString ossIconUrl, FString nativeIconUrl, float ossHeight, float ossWidth)
{
	FString m_OssIconUrl = GetHeadOssDateUrl2(ossIconUrl);

	TSharedPtr<FJsonObject> t_jsonObject = MakeShareable(new FJsonObject);

	t_jsonObject->SetStringField(TEXT("avatar"), m_OssIconUrl);

	t_jsonObject->SetStringField(TEXT("nickName"), m_LoginModule->GetUserData()->GetNickName());

	FString mToken = m_LoginModule->GetUserData()->GetToken();

	FString mOperator = m_LoginModule->GetUserData()->GetUserId();

	msg_ptr mMsg(new MsgHttp(MessageTypeHttpRequest, 2001, mToken, mOperator, false, t_jsonObject));

	MsgCenter::Instance()->SendMsg(mMsg);
}

void DesignModule::OnUploadHeadSuccess(msg_ptr _msg)
{
	MsgHttp *mHttpMsg = _msg->ConvertMsg<MsgHttp>();

	//	判断消息是否正常接收.
	if (mHttpMsg->GetMsgErrorCode() == EnumMsgResponseErrorCode::MessageTrue)
	{
		//	消息正常,获取内容.
		TSharedPtr<FJsonObject> t_jsonObject = mHttpMsg->GetHttpContent();

		TSharedPtr<FJsonObject> mjsonObject = t_jsonObject->GetObjectField(TEXT("data"));

		//	本地广播.
		msg_ptr mBrodcasCheckSucess(new MsgLocal(MessageTypeLocal, m_LoginModule->m_GetUserHeadSuccess, &mjsonObject));

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

void DesignModule::UploadPictureToOss(FString ossIconUrl, FString nativeIconUrl, FString ossPatternUrl, FString nativePatternUrl, float width, float height)
{
	m_UploadInfoStruct.m_IconOssUrl = ossIconUrl;

	m_UploadInfoStruct.m_PatternOssUrl = ossPatternUrl;

	m_UploadInfoStruct.m_Width = width;

	m_UploadInfoStruct.m_Height = height;

	m_UploadInfoStruct.m_IconNativeUrl = nativeIconUrl;

	m_UploadInfoStruct.m_TextureNativeUrl = nativePatternUrl;

	FString mIconOssUrl = GetHeadOssDateUrl(nativeIconUrl);

	FString mPatternOssUrl = GetHeadOssDateUrl(nativePatternUrl);

	if (GPlatformFileManager::FileExists(*nativeIconUrl) && GPlatformFileManager::FileExists(*nativePatternUrl))
	{
		FOssUploadQueueParameter inUploadParameters;

		//路径. 
		TArray<FOssReqeustParameter> FilePaths;

		UE_LOG(LogTemp, Log, TEXT("zhoujia:UploadPictureToOss mIconOssUrl=%s, nativeIconUrl=%s"),  *mIconOssUrl, *nativeIconUrl);

		//icon.
		FilePaths.Add(FOssReqeustParameter(mIconOssUrl, nativeIconUrl));

		UE_LOG(LogTemp, Log, TEXT("zhoujia:UploadPictureToOss mPatternOssUrl=%s, nativePatternUrl=%s"), *mPatternOssUrl, *nativePatternUrl);

		//texture.
		FilePaths.Add(FOssReqeustParameter(mPatternOssUrl, nativePatternUrl));

		//完成回调,回调函数必须指明类.

		FOnOSSRequestFinishedDelegate OnOSSRequestFinished(this, &DesignModule::OnUploadPictureToOssSuccess);

		FOnOSSFileUploadProgressDelegate OnOSSFileUploadProgressDelegate(this, &DesignModule::OnUploadUserPictureToOssProgress);

		inUploadParameters.OnOSSRequestFinished = OnOSSRequestFinished;

		inUploadParameters.OnOSSFileUploadProgress = OnOSSFileUploadProgressDelegate;

		inUploadParameters.FilePaths = FilePaths;

		//开始上传.
		OSSHelper::GetInstance()->GetOSSTool()->Upload(inUploadParameters);
	}
}

void DesignModule::OnUploadPictureToOssSuccess(bool res)
{
	if (GPlatformFileManager::FileExists(*m_UploadInfoStruct.m_IconNativeUrl))
	{
		GPlatformFileManager::DeleteFile(m_UploadInfoStruct.m_IconNativeUrl);
	}
	if (GPlatformFileManager::FileExists(*m_UploadInfoStruct.m_TextureNativeUrl))
	{
		GPlatformFileManager::DeleteFile(m_UploadInfoStruct.m_TextureNativeUrl);
	}
	UE_LOG(GEditorLog, Error, TEXT("zhoujia:OnUploadPictureToOssSuccess"));

	ReqUploadHeadJson(m_UploadInfoStruct.m_IconOssUrl, m_UploadInfoStruct.m_PatternOssUrl, m_UploadInfoStruct.m_Height, m_UploadInfoStruct.m_Width);
}

void DesignModule::OnUploadUserPatternToOssProgress(FString _filename, int32 _currentsize, int32 _totalsize)
{

}

void DesignModule::OnUploadUserPictureToOssProgress(FString _filename, int32 _currentsize, int32 _totalsize)
{

}

//	订单列表服务器消息回调.
void DesignModule::OnShopOrderResponseMsg(msg_ptr _msg)
{
	MsgHttp *mHttpMsg = _msg->ConvertMsg<MsgHttp>();

	// 1.先判断消息是否正常接收.
	if (mHttpMsg->GetMsgErrorCode() == EnumMsgResponseErrorCode::MessageTrue)
	{
		// 2.消息正常,获取内容.
		TSharedPtr<FJsonObject> t_jsonObject = mHttpMsg->GetHttpContent();

		TSharedPtr<FJsonObject> mjsonObject = t_jsonObject->GetObjectField(TEXT("data"));

		TArray<TSharedPtr<FJsonValue>> mOrderInfo = mjsonObject->GetArrayField(TEXT("orderInfo"));

		OrderStruct mStruct;

		for (int32 i = 0; i < mOrderInfo.Num(); i++)
		{
			TSharedPtr<FJsonObject> mJsonObject = mOrderInfo[i]->AsObject();
			FString type = mJsonObject->GetStringField(TEXT("type"));
			if (type.Equals(FString(TEXT("STYLE"))))
			{
				mStruct.m_iconUrl = mJsonObject->GetStringField(TEXT("url"));

				mStruct.m_buttonText = mJsonObject->GetStringField(TEXT("buttonText"));

				mStruct.m_extra = mJsonObject->GetStringField(TEXT("extra"));
				mStruct.m_Type = type;
			}
		}

		// 发送分享成功的广播.
		msg_ptr mBrodcastShareFriendSucess(new MsgLocal(MessageTypeLocal, 20081, &mStruct));

		MsgCenter::Instance()->SendMsg(mBrodcastShareFriendSucess);
	}
	else
	{
		// 消息异常,自行根据逻辑判断是否重发.

		// 获取异常内容.
		FString mErrorInfo = mHttpMsg->GetMsgErrorInfo();

		UE_LOG(GEditorLog, Error, TEXT("http response Error %s"), *mErrorInfo);
	}
}

//	分享消息服务器回调.
void DesignModule::OnShareFriendResponseMsg(msg_ptr _msg)
{
	MsgHttp *mHttpMsg = _msg->ConvertMsg<MsgHttp>();

	// 1.先判断消息是否正常接收.
	if (mHttpMsg->GetMsgErrorCode() == EnumMsgResponseErrorCode::MessageTrue)
	{
		// 2.消息正常,获取内容.
		TSharedPtr<FJsonObject> t_jsonObject = mHttpMsg->GetHttpContent();

		TSharedPtr<FJsonObject> mjsonObject = t_jsonObject->GetObjectField(TEXT("data"));

		TSharedPtr<FJsonObject> mSharjson = mjsonObject->GetObjectField(TEXT("shareInfo"));

		TArray<TSharedPtr<FJsonValue>> mOrderInfo = mjsonObject->GetArrayField(TEXT("orderInfo"));

		TArray<OrderStruct> mStructArray;
		
		for (int32 i=0;i<mOrderInfo.Num();i++)
		{
			TSharedPtr<FJsonObject> mJsonObject = mOrderInfo[i]->AsObject();

			OrderStruct mStruct;

			mStruct.m_iconUrl = mJsonObject->GetStringField(TEXT("url"));

			mStruct.m_buttonText = mJsonObject->GetStringField(TEXT("buttonText"));

			mStruct.m_extra = mJsonObject->GetStringField(TEXT("extra"));

			mStructArray.Add(mStruct);
		}

		// 发送分享成功的广播.
		msg_ptr mBrodcastShareFriendSucess(new MsgLocal(MessageTypeLocal, m_ShareFriendSuccess, &mSharjson));

		MsgCenter::Instance()->SendMsg(mBrodcastShareFriendSucess);

		// 发送订单的广播.
		msg_ptr mBrodcastShareFriendSucess1(new MsgLocal(MessageTypeLocal, m_OrderItemSuccess, &mStructArray));

		MsgCenter::Instance()->SendMsg(mBrodcastShareFriendSucess1);
	}
	else
	{
		// 消息异常,自行根据逻辑判断是否重发.

		// 获取异常内容.
		FString mErrorInfo = mHttpMsg->GetMsgErrorInfo();

		UE_LOG(GEditorLog, Error, TEXT("http response Error %s"), *mErrorInfo);
	}
}

//	请求面板信息.
void DesignModule::RequestBuyServer(FString _buyId, FString _tab)
{
	TSharedPtr<FJsonObject> t_jsonObject = MakeShareable(new FJsonObject);

	t_jsonObject->SetStringField(TEXT("goodsId"), _buyId);
	
	t_jsonObject->SetStringField(TEXT("tab"), _tab);

	FString mToken = m_LoginModule->GetUserData()->GetToken();

	FString mOperator = m_LoginModule->GetUserData()->GetUserId();

	msg_ptr mMsg(new MsgHttp(MessageTypeHttpRequest, 2007, mToken, mOperator, false, t_jsonObject));

	MsgCenter::Instance()->SendMsg(mMsg);
}

//	服务器返回面板消息的回调.
void DesignModule::OnBuyInfoResponseMsg(msg_ptr _msg)
{
	MsgHttp *mHttpMsg = _msg->ConvertMsg<MsgHttp>();

	// 1.先判断消息是否正常接收.
	if (mHttpMsg->GetMsgErrorCode() == EnumMsgResponseErrorCode::MessageTrue)
	{
		GoodInfoStruct mStruct;
		// 2.消息正常,获取内容.
		TSharedPtr<FJsonObject> t_jsonObject = mHttpMsg->GetHttpContent();

		TSharedPtr<FJsonObject> mjsonObject = t_jsonObject->GetObjectField(TEXT("data"));

		mjsonObject->TryGetStringField(TEXT("name"), mStruct.m_goodsName);

		mjsonObject->TryGetStringField(TEXT("sn"), mStruct.m_goodsSn);

		mjsonObject->TryGetStringField(TEXT("color"), mStruct.m_goodsColor);

		mjsonObject->TryGetStringField(TEXT("panTongCode"), mStruct.m_goodsPanTongCode);

		mjsonObject->TryGetStringField(TEXT("imageUrl"), mStruct.m_imageUrl);

		mjsonObject->TryGetBoolField(TEXT("isBuy"), mStruct.m_isBuy);

		mjsonObject->TryGetStringField(TEXT("orderUrl"), mStruct.m_orderUrl);

		if (mStruct.m_isBuy == true)
		{
			mStruct.m_GoodsPriceStruct.Empty();

			TArray<TSharedPtr<FJsonValue>> mjson = mjsonObject->GetArrayField(TEXT("sku"));

			for (int32 i = 0; i < mjson.Num(); ++i)
			{
				TSharedPtr<FJsonObject> mBuyJson = mjson[i]->AsObject();

				GoodsPriceStruct mGoodsStruct;

				mBuyJson->TryGetNumberField(TEXT("price"), mGoodsStruct.m_goodsPrice);

				mBuyJson->TryGetNumberField(TEXT("promotionPrice"), mGoodsStruct.m_goodsPromotionPrice);

				mBuyJson->TryGetStringField(TEXT("title"), mGoodsStruct.m_title);

				mBuyJson->TryGetStringField(TEXT("unit"), mGoodsStruct.m_unit);

				mStruct.m_GoodsPriceStruct.Add(mGoodsStruct);
			}
		}

		// 发送获取面板消息的广播.
		msg_ptr mBrodcastShareFriendSucess(new MsgLocal(MessageTypeLocal, m_Http_BuyInfo, &mStruct));

		MsgCenter::Instance()->SendMsg(mBrodcastShareFriendSucess);
	}
	else
	{
		// 消息异常,自行根据逻辑判断是否重发.

		// 获取异常内容.
		FString mErrorInfo = mHttpMsg->GetMsgErrorInfo();

		UE_LOG(GEditorLog, Error, TEXT("http response Error %s"), *mErrorInfo);
	}
}

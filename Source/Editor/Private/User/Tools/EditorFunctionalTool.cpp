// Fill out your copyright notice in the Description page of Project Settings.

#include "EditorFunctionalTool.h"
#include "Internationalization/Regex.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include <regex>
#include "LoginModule.h"

#include "Engine.h"
#include "PrototypeManager.h"
#include "GController.h"
#include "ServerPrototype.h"
#include "UserInfo.h"
#include "UserData.h"
#include "GModuleManager.h"
#include "LoginModule.h"

EditorFunctionalTool::EditorFunctionalTool()
{
}

EditorFunctionalTool::~EditorFunctionalTool()
{
}
bool EditorFunctionalTool::CanAutoLogin(FString& phone, FString& password) {
	return UserInfo::Get()->ReadLocalData(phone, password);
}
bool EditorFunctionalTool::CheckStringIsValid(const FString& str, const FString& Reg)
{
	FRegexPattern Pattern(Reg);
	FRegexMatcher regMatcher(Pattern, str);
	regMatcher.SetLimits(0, str.Len());
	return regMatcher.FindNext();
}


bool EditorFunctionalTool::CheckPhone(const FString& str)
{
	if (str.Len() != 11) return false;
	FString reg = TEXT("^1\\d{10}$");
	return EditorFunctionalTool::CheckStringIsValid(str, reg);
}

bool EditorFunctionalTool::CheckIdentifyingCode(const FString& str)
{
	if (str.Len() < 1)return false;
	return true;
}

bool EditorFunctionalTool::CheckPassword(const FString& str, FString& tip)
{
	if (str.Len() < 6) {
		tip = TEXT("密码为6-20位数字和字母组合");
		return false;
	}
	if (str.Len() > 20) {
		tip = TEXT("密码为6-20位数字和字母组合");
		return false;
	}
	FString reg = TEXT("^(?![0-9]+$)(?![a-zA-Z]+$)[0-9A-Za-z]{6,20}$");
	if (!CheckStringIsValid(str, reg)) 
	{
		tip = TEXT("密码为6-20位数字和字母组合");
		return false;
	}
	return true;
}

FString EditorFunctionalTool::Str2Md5(const FString& str)
{
	FString md5str = FMD5::HashAnsiString(*str).ToUpper();
	return md5str;

}

bool EditorFunctionalTool::IdentifyingCodeCheck(const FString &json, FString& tip)
{
	// mHttpMsg->m_serverCode  == code
	//mHttpMsg->m_serverMsg = msg
 	MsgHttp message;
 	tip = "";
	

//  	if (!FJsonObjectConverter::JsonObjectStringToUStruct(json, &message, 0, 0))
//  	{
//  		//UE_LOG(ClothEditor, Warning, TEXT("properties matched but failed to deserialize."));
//  		return false;
//  	}
//  	if (message.GetMsgErrorCode() == EnumMsgResponseErrorCode::MessageTrue) 
//  	{
//  		return true;
//  	}
 	return true;

// 	TSharedPtr<FJsonObject> t_jsonObject = message.GetHttpContent();
// 
// 	// 只是转换成字符串打印.
// 	FString mContent;
// 
// 	TSharedRef<TJsonWriter<TCHAR>> t_writer = TJsonWriterFactory<>::Create(&mContent);
// 
// 	FJsonSerializer::Serialize(t_jsonObject.ToSharedRef(), t_writer);
// 
// 	tip = mContent;
	
}

bool EditorFunctionalTool::IdentifyingCodeCheck(msg_ptr _msg)
{
if (_msg->GetMsgErrorCode() == EnumMsgResponseErrorCode::MessageTrue)
{
	return true;
}
return false;
}

bool EditorFunctionalTool::LoginCheck(msg_ptr _msg,FString& _Tip)
{
	MsgHttp *mHttpMsg = _msg->ConvertMsg<MsgHttp>();

	//_Tip = mHttpMsg->GetServerMsg();
	if (_msg->GetMsgErrorCode() == EnumMsgResponseErrorCode::MessageTrue)
	{
		
		return true;
	}
	return false;
	
}

bool EditorFunctionalTool::FindNewPasswordHttpCheck(const FString &json, FString& tip)
{
	
	return true;
}

//	画质设置
void EditorFunctionalTool::SetQualityByShortcut(EQualityLevel _Level)
{
	Scalability::FQualityLevels  levels;
	levels.SetFromSingleQualityLevel((uint8)_Level);
	Scalability::SetQualityLevels(levels);
}

FString EditorFunctionalTool::GetURLTicketStr()
{
	int32 mServerPrototypeId = PrototypeConst::P_HTTP_WINDOWS_SERVER_ID;

	if (GController::m_platform == ENUM_PLATFORM::GANDROID)
	{
		mServerPrototypeId = PrototypeConst::P_HTTP_ANDROID_SERVER_ID;
	}
	else if (GController::m_platform == ENUM_PLATFORM::GIOS)
	{
		mServerPrototypeId = PrototypeConst::P_HTTP_IOS_SERVER_ID;
	}

	ServerPrototype * mServerPrototype = PrototypeManager::GetInstance()->GetPrototypeByPID<ServerPrototype>(mServerPrototypeId);

	FString mSource = mServerPrototype->GetValueByKey(TEXT("source"));

	FString mAppType = mServerPrototype->GetValueByKey(TEXT("appType"));

	FString mChannel = mServerPrototype->GetValueByKey(TEXT("channel"));

	LoginModule *mLoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	FString r = TEXT("ticket=") + mLoginModule->GetUserData()->GetToken();

	r += TEXT("&channel=") + mChannel;

	r += TEXT("&source=") + mSource;

	r += TEXT("&appType=") + mAppType;

 	return r;
}

FString EditorFunctionalTool::GetTicketUrl(FString &url)
{
 	FString ret;
 	if (url.Contains("?"))
 	{
 		ret = url + "&" + GetURLTicketStr();
 	}
	else
	{
		ret = url + "?" + GetURLTicketStr();
	}
 	return ret;
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "UserData.h"
#include "Serialization/JsonSerializer.h"



void FUserData::InitData(TSharedPtr<FJsonObject> &_jsonObj)
{
	GSchedulerManager::GetInstance()->AddCondition(TEXT("login"), TEXT("1"));
	TSharedPtr<FJsonObject> jsonObj = _jsonObj->GetObjectField(TEXT("member"));
	FString jsonObj_ticket = _jsonObj->GetStringField(TEXT("ticket"));
	

	FString phone = jsonObj->GetStringField(TEXT("phone"));
	m_member.phone = jsonObj->GetStringField(TEXT("phone"));

	m_member.ticket = jsonObj_ticket;
	m_member.address = jsonObj->GetStringField(TEXT("address"));
	m_member.age = jsonObj->GetIntegerField(TEXT("address"));
	m_member.avatar = jsonObj->GetStringField(TEXT("avatar"));
	m_member.channel = jsonObj->GetStringField(TEXT("channel"));
	m_member.city = jsonObj->GetStringField(TEXT("city"));
	m_member.comments = jsonObj->GetStringField(TEXT("nickName"));
	m_member.createby = jsonObj->GetStringField(TEXT("createby"));
	m_member.createdat = jsonObj->GetIntegerField(TEXT("createdat"));
	m_member.id = jsonObj->GetIntegerField(TEXT("id"));
	m_member.loginname = jsonObj->GetStringField(TEXT("loginname"));
	m_member.mail = jsonObj->GetStringField(TEXT("mail"));
	m_member.name = jsonObj->GetStringField(TEXT("name"));
	m_member.password = jsonObj->GetStringField(TEXT("password"));
	m_member.phone = jsonObj->GetStringField(TEXT("phone"));
	m_member.province = jsonObj->GetStringField(TEXT("province"));
	m_member.qq = jsonObj->GetStringField(TEXT("qq"));
	m_member.roleCode = jsonObj->GetStringField(TEXT("roleCode"));
	m_member.sex = jsonObj->GetIntegerField(TEXT("sex"));
	m_member.source = jsonObj->GetStringField(TEXT("source"));
	m_member.status = jsonObj->GetStringField(TEXT("status"));
	m_member.tworolecode = jsonObj->GetStringField(TEXT("tworolecode"));
	m_member.updateby = jsonObj->GetStringField(TEXT("updateby"));
	m_member.updatedat = jsonObj->GetIntegerField(TEXT("updatedat"));
}


//	获取Token和Operate.
const FString & FUserData::GetToken()
{
	return m_member.ticket;
}

FString FUserData::GetUserId()
{
	return FString::FromInt(m_member.id);
}

//	设置用户昵称.
void FUserData::SetNickName(FString nickName)
{
	m_member.comments = nickName;
}

FString FUserData::GetVisualUserName()
{
	FString mUserName = GetNickName();

	if (mUserName.Len() <= 0)
	{
		mUserName = m_member.phone;
	}

	return mUserName;
}

FString& FUserData::GetNickName()
{
	return m_member.comments;
}

//	设置建议反馈H5.
void FUserData::SetFeedbackURL(FString url)
{
	m_FeedBackUrl = url;
}

FString& FUserData::GetFeedbackURL()
{
	return m_FeedBackUrl;
}

//	设置关于我们H5.
void FUserData::SetContactURL(FString url)
{
	m_ContactUrl = url;
}

FString& FUserData::GetContactURL()
{
	return m_ContactUrl;
}

//	设置使用教程H5.
void FUserData::SetUseCourseURL(FString url)
{
	m_UseCourseUrl = url;
}

FString& FUserData::GetUseCourseURL()
{
	return m_UseCourseUrl;
}

//	设置商城H5.
void FUserData::SetShopURL(FString url)
{
	m_ShopUrl = url;
}

FString & FUserData::GetShopURL()
{
	return m_ShopUrl;
}

//	设置订单H5.
void FUserData::SetOrderURL(FString url)
{
	m_OrderUrl = url;
}

FString & FUserData::GetOrderURL()
{
	return m_OrderUrl;
}

//	设置下载H5.
void FUserData::SetDownloadURL(FString url)
{
	m_DownloadUrl = url;
}

FString & FUserData::GetDownloadURL()
{
	return m_DownloadUrl;
}





//	画质.
EQualityLevel FUserData::GetSettingDisplayLevel()
{
	if (m_SaveData.settingdisplaylevel < 0)
		return EQualityLevel::E_High;

	return (EQualityLevel)m_SaveData.settingdisplaylevel;

}

void FUserData::SetProtocolUrl(FString url)
{
	m_protocolUrl = url;
}

FString & FUserData::GetProtocolUrl()
{
	return m_protocolUrl;
}

void FUserData::SetAvatar(FString avatar)
{
	m_member.avatar = avatar;
}

FString & FUserData::GetUserAvatar()
{
	return m_member.avatar;
}

void FUserData::SetGoodsListUrl(FString url)
{
	m_GoodsListUrl = url;
}

FString & FUserData::GetGoodsListUrl()
{
	return m_GoodsListUrl;
}

void SaveSettingDisplayLevel(EQualityLevel level)
{
//	SetSettingDisplayLevel(level);
//	SaveLocalData();
}

void SetSettingDisplayLevel(EQualityLevel level)
{
//	m_SaveData.settingdisplaylevel = (int)level;
}

bool SaveLocalData()
{
// 	FString json;
// 	if (!FJsonObjectConverter::UStructToJsonObjectString(data, json, 0, 0)) 
// 	{
// 		return false;
// 	}
// 
// 	FString path = FPaths::Combine(MutableRootDir(), GetLocalFile());
// 	json = ToolUtil::Encrypt(json, EncryptKey);
// 
// 	if (!FFileHelper::SaveStringToFile(json, *path))
// 	{
// 		return false;
// 	}
	return true;
}

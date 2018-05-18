// Fill out your copyright notice in the Description page of Project Settings.

#include "UserInfo.h"
#include "Misc/SecureHash.h"
#include "Runtime/JsonUtilities/Public/JsonUtilities.h"
#include "GFileHelper.h"
#include <iostream>
#include <ctime>

#include "EncryptDecryptTool.h"


using namespace std;

// --- 
// -------- 
std::time_t getTimeStamp()
{
	std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
	auto tmp = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
	std::time_t timestamp = tmp.count();
	return timestamp;
}
/// -----------------

void UserInfo::UpdateDataByJson(const FString& jsondata)
{
	if (!FJsonObjectConverter::JsonObjectStringToUStruct(jsondata, &data, 0, 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("properties matched but failed to deserialize."));
		return;
	}
	data.m_SaveData.phone = this->data.m_member.phone;
	data.m_SaveData.password = this->data.m_member.password;
	data.m_SaveData.updatedat = getTimeStamp();
	SaveLocalData();
}

void UserInfo::UpdateData(const FUserData& indata)
{
	this->data = indata;
	this->data.m_member = indata.m_member;
	data.m_SaveData.phone = this->data.m_member.phone;
	data.m_SaveData.password = this->data.m_member.password;
	data.m_SaveData.updatedat = getTimeStamp();
	SaveLocalData();
}
bool UserInfo::SaveLocalData()
{
	FString json;
	if (!FJsonObjectConverter::UStructToJsonObjectString(data.m_SaveData, json, 0, 0)) {
		return false;
	}
	FString path = FPaths::Combine(GFileHelper::ProjectPersistentDownloadDir(), GetLocalFile());
	json = EncryptAndDecryptTool::Encrypt(json, EncryptKey);


	if (!GFileHelper::SaveStringToFile(json, *path))
	{
		return false;
	}
	return true;
}
bool UserInfo::ReadLocalData(FString& phone, FString& password) {
	
	FString json;
	phone = "";
	password = "";
	FString path = FPaths::Combine(GFileHelper::ProjectPersistentDownloadDir(), GetLocalFile());
	 
	if (!GFileHelper::LoadFileToString(json, *path))
	{
		return false;
	}
	json = EncryptAndDecryptTool::Decrypt(json, EncryptKey);
	data.m_SaveData.Clear();

	if (FJsonObjectConverter::JsonObjectStringToUStruct(json, &data.m_SaveData, 0, 0)) {
		if (data.m_SaveData.phone != "" && data.m_SaveData.password != "")
		{
			phone = data.m_SaveData.phone;
			password = data.m_SaveData.password;
			return true;
		}
	}
	return false;
}
void UserInfo::ClearLocalData() {
	data.m_SaveData.phone = TEXT("");
	data.m_SaveData.password = TEXT("");
	data.m_SaveData.updatedat = getTimeStamp();
	SaveLocalData();
}
void UserInfo::ClearUserData() {
	data.Clear();
	stylistModelData.Clear();
	ClearLocalData();

}
bool UserInfo::GetLocalData(FSaveUserData& indata) {
	FString s;
	FString a;
	if (ReadLocalData(a, s)) {
		indata = this->data.m_SaveData;
		return true;
	}
	return false;
}
FString UserInfo::GetTicket() {
	return data.ticket;
}
int32 UserInfo::GetUserId() {
	return data.m_member.id;
}
EQualityLevel UserInfo::GetSettingDisplayLevel()
{
	if (data.m_SaveData.settingdisplaylevel < 0) return EQualityLevel::E_High;
	return (EQualityLevel)data.m_SaveData.settingdisplaylevel;
}
void UserInfo::SetSettingDisplayLevel(EQualityLevel level)
{
	data.m_SaveData.settingdisplaylevel = (int)level;
}
FString UserInfo::GetUserName() {
	return data.m_member.loginname;
}

FString UserInfo::GetVisualUserName() {
	FString result = GetNickName();
	if (result == "") {
		result = data.m_member.phone;
		//Hide cell phone number four in the center-- tmsh
		/*for (size_t i = 3; i < result.Len() && i < 7; i++)
		{
		result[i] = '*';
		}
		*/
	}
	return result;
}
void UserInfo::SetUserInfoUrl(const FString & url)
{
	userInfoUrl = url;
}
const FString & UserInfo::GetUserInfoUrl()
{
	return	userInfoUrl;
}
FString UserInfo::GetLocalFile() {
	FString path;
	if (GConfig)
	{
		GConfig->GetString(*UserFile, *UserFilePath, path, GGameIni);
	}
	return path;
}
void UserInfo::SaveUserPassword(const FString& str) {
	FString md5str = FMD5::HashAnsiString(*str).ToUpper();
	data.m_member.password = md5str;
	data.m_SaveData.password = md5str;
	SaveLocalData();

}
void UserInfo::SaveMD5UserPassword(const FString& str) {
	data.m_member.password = str;
	data.m_SaveData.password = str;
	SaveLocalData();

}
void UserInfo::SaveSettingDisplayLevel(EQualityLevel level)
{
	SetSettingDisplayLevel(level);
	SaveLocalData();
}
void UserInfo::UpdatePerfectInfo(const FString& nickName, const FString& briefIntroduction, const FString& positionAddress, const FString&  name, const int32 position, const int32 sex, const int32 age) {
	stylistModelData.nickName = nickName;
	stylistModelData.memberId = data.m_member.id;
	stylistModelData.briefIntroduction = briefIntroduction;
	stylistModelData.postionAddress = positionAddress;
	data.m_member.name = name;
	stylistModelData.position = position;
	data.m_member.sex = sex;
	data.m_member.age = age;
}

void UserInfo::SetPcdownloadURL(const FString& url)
{
	pcdownloadUrl = url;
}

const FString& UserInfo::GetPcdownloadURL()
{
	return pcdownloadUrl;
}

void UserInfo::SetmallURL(const FString& url)
{
	mallUrl = url;
}

const FString& UserInfo::GetmallURL()
{
	return mallUrl;
}

void UserInfo::SetOrderURL(const FString& url)
{
	orderUrl = url;
}

void UserInfo::SetProtocolURL(const FString& url)
{
	protocolUrl = url;
}

void UserInfo::SetContactURL(const FString & url)
{
	contactUrl = url;
}

void UserInfo::SetFeedbackURL(const FString & url)
{
	feedbackUrl = url;
}

const FString& UserInfo::GetOrderURL()
{
	return orderUrl;
}

const FString& UserInfo::GetProtocol()
{
	return protocolUrl;
}

const FString & UserInfo::GetContactURL()
{
	return contactUrl;
}

const FString & UserInfo::GetFeedbackURL()
{
	return feedbackUrl;
}
const FString& UserInfo::GetHeadURL() {
	return data.m_member.avatar;
}

const FString & UserInfo::GetNickName()
{
	return data.m_member.comments;
}

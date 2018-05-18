// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserData.h"
#include "ILoad.h"
#include "EncryptDecryptTool.h"
#include "UserInfo.generated.h"
/**
 * 
 */





USTRUCT(BlueprintType)
struct FPrefectStylistModel
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY()
		FString nickName;
	UPROPERTY()
		int32 position;
	UPROPERTY()
		FString postionAddress;
	UPROPERTY()
		int32 roleId;
	UPROPERTY()
		FString briefIntroduction;
	UPROPERTY()
		int32 memberId;

	void Clear() {
		nickName = "";
		position = 0;
		postionAddress = "";
		roleId = 0;
		briefIntroduction = "";
		memberId = 0;
	}
};

static const FString UserFile = TEXT("USERFILE");
static const FString UserFilePath = TEXT("Path");
class UserInfo 
{
public:
	UserInfo() { data.m_SaveData.Clear(); };
public:
	static inline UserInfo * Get() {
		static UserInfo *instance = new UserInfo();
		return instance;
	}
private:
	FUserData data;
	FPrefectStylistModel stylistModelData;
	FString userInfoUrl;
	FString pcdownloadUrl;
	FString mallUrl;
	FString orderUrl;
	FString protocolUrl;
	FString contactUrl;
	FString feedbackUrl;


public:

	void UpdateDataByJson(const FString& jsondata);
	void UpdateData(const FUserData& data);
	FString GetLocalFile();
	bool SaveLocalData();
	bool ReadLocalData(FString& phone, FString& password);
	void ClearLocalData();
	bool GetLocalData(FSaveUserData& data);
	void ClearUserData();
	FString GetTicket();
	int32 GetUserId();
	EQualityLevel GetSettingDisplayLevel();
	void SetSettingDisplayLevel(EQualityLevel level);
	FString GetUserName();
	FString GetVisualUserName();
	void SetUserInfoUrl(const FString& url);
	const FString& GetUserInfoUrl();
	void SaveUserPassword(const FString& str);
	void SaveMD5UserPassword(const FString& str);
	void SaveSettingDisplayLevel(EQualityLevel level);
	void UpdatePerfectInfo(const FString& nickName, const FString& briefIntroduction, const FString& positionAddress, const FString&  name, const int32 position, const int32 sex, const int32 age);
	/*  */

	void SetPcdownloadURL(const FString& url);
	void SetmallURL(const FString& url);
	void SetOrderURL(const FString& url);
	void SetProtocolURL(const FString& url);
	void SetContactURL(const FString& url);
	void SetFeedbackURL(const FString& url);

	const FString& GetPcdownloadURL();
	const FString& GetmallURL();
	const FString& GetOrderURL();
	const FString& GetProtocol();
	const FString& GetContactURL();
	const FString& GetFeedbackURL();
	const FString& GetHeadURL();
	const FString& GetNickName();
};

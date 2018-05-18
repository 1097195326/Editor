// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Tools/EditorFunctionalTool.h"
#include "GSchedulerManager.h"
#include "UserData.generated.h"

USTRUCT(BlueprintType)
struct FUserDataMermber
{
	GENERATED_USTRUCT_BODY()
		FUserDataMermber() :id(0) { Clear(); }

public:
	UPROPERTY()
		FString ticket;//ticket
	UPROPERTY()
		FString address;
	UPROPERTY()
		int32 age;
	UPROPERTY()
		FString avatar;
	UPROPERTY()
		FString channel;
	UPROPERTY()
		FString city;
	UPROPERTY()
		FString comments;
	UPROPERTY()
		FString createby;
	UPROPERTY()
		int64 createdat;
	UPROPERTY()
		int32 id;
	UPROPERTY()
		FString loginname;
	UPROPERTY()
		FString mail;
	UPROPERTY()
		FString name;
	UPROPERTY()
		FString password;
	UPROPERTY()
		FString phone;
	UPROPERTY()
		FString province;
	UPROPERTY()
		FString qq;
	UPROPERTY()
		FString roleCode;
	UPROPERTY()
		int32 sex;
	UPROPERTY()
		FString source;
	UPROPERTY()
		FString status;
	UPROPERTY()
		FString tworolecode;
	UPROPERTY()
		FString updateby;
	UPROPERTY()
		int64 updatedat;

	void Clear() {
		GSchedulerManager::GetInstance()->AddCondition(TEXT("login"), TEXT("0"));
		ticket = "";
		address = "";
		age = 0;
		avatar = "";
		channel = "";
		city = "";
		comments = "";
		createby = "";
		createdat = 0;
		id = 0;
		loginname = "";
		mail = "";
		name = "";
		password = "";
		phone = "";
		province = "";
		qq = "";
		roleCode = "";
		sex = 0;
		source = "";
		status = "";
		tworolecode = "";
		updateby = "";
		updatedat = 0;
	}
};

USTRUCT(BlueprintType)
struct FSaveUserData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY()
		FString phone;
	UPROPERTY()
		FString password;
	UPROPERTY()
		int64 updatedat;

	UPROPERTY()//EQualityLevel
		int32 settingdisplaylevel = -1;

	void Clear() {
		phone = "";
		password = "";
		updatedat = 0;
		settingdisplaylevel = (int32)EQualityLevel::E_High;
	}
};

/**
 * 
 */
USTRUCT(BlueprintType)
struct EDITOR_API FUserData //: public UObject
{
	GENERATED_BODY()

public:
	FUserData():ticket("") { m_member.Clear(); };

	//FUserData & operator = (const UUserData & data);
private:
	//	H5页面跳转.
	FString m_ShopUrl;

	FString m_OrderUrl;

	FString m_DownloadUrl;

	FString m_UseCourseUrl;

	FString m_FeedBackUrl;

	FString m_ContactUrl;

	FString m_protocolUrl;

	FString m_GoodsListUrl;

public:
	void InitData(TSharedPtr<FJsonObject> &_jsonObj);

	UPROPERTY()
		FString ticket;
	UPROPERTY()
		FUserDataMermber m_member;

	UPROPERTY()
		FSaveUserData m_SaveData;

	void Clear() {
		ticket = "";
		m_member.Clear();
	}
	
public:

	EQualityLevel GetSettingDisplayLevel();

	FString GetUserId();

	FString GetVisualUserName();

	FString  & GetUserAvatar();

	const FString & GetToken();

	FString & GetShopURL();				//	商城URL.

	FString & GetOrderURL();			//	订单URL.

	FString & GetDownloadURL();			//	下载URL.

	FString & GetUseCourseURL();

	FString & GetContactURL();

	FString & GetFeedbackURL();

	FString & GetNickName();

	FString & GetProtocolUrl();

	FString & GetGoodsListUrl();

	void SetAvatar(FString avatar);		//	设置头像.

	void SetNickName(FString nickName);	//	设置用户昵称.

	void SetShopURL(FString url);		//	商城URL.

	void SetOrderURL(FString url);		//	订单URL.

	void SetDownloadURL(FString url);	//	下载URL.

	void SetUseCourseURL(FString url);

	void SetFeedbackURL(FString url);

	void SetContactURL(FString url);

	void SetProtocolUrl(FString url);

	void SetGoodsListUrl(FString url);

	void SaveSettingDisplayLevel(EQualityLevel level);

	void SetSettingDisplayLevel(EQualityLevel level);

	bool SaveLocalData();
};



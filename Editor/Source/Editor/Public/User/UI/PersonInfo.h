// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LoginModule.h"
#include "LogicMainScene.h"
#include "DisplaySetting.h"
#include "LanguageSetting.h"
#include "EditableTextBox.h"
#include "DownloadImageTool.h"
#include "HaoshaSDKBPLibrary.h"
#include "PhotoPage.h"
#include "PersonInfo.generated.h"

/**
 * 
 */
UCLASS()
class EDITOR_API UPersonInfo : public UBaseUI
{
	GENERATED_BODY()
	
private:
	FString m_UserNick;
	
public:
	UPersonInfo(const FObjectInitializer& ObjectInitializer);

	//	复写BaseUI.
	virtual bool OnStart() override;

	virtual void OnInit();

	virtual void OnRelease();

	virtual void OnSetParams(int32 _pid, FString & _content, ...);

	virtual void OnTick(float InDeltaTimeSecond);

	//	修改用户昵称事件.
	UFUNCTION(BlueprintCallable, Category = PersonInfo)
		void EditUserName(FText InText);

	//	收到服务器发送给本地修改用户名成功的消息.
	void OnUserNameResponseMsg(msg_ptr _msg);

	//	收到服务器发送给本地修改头像成功的消息.
	void OnUserHeadResponseMsg(msg_ptr _msg);

	//	收到服务器发送给本地获取头像昵称成功的消息.
	void OnGetUserHeadResponseMsg(msg_ptr _msg);

	//	打开其他页面.
	void OpenPageCell(int32 _pid);

	//	用户昵称不能为空.
	void OnEditNameError();

	// 图片异步下载完成的回调.
	void OnDownloadImage(bool _isScuccess, int32 _penetrateId, FString _url, UTexture2D * _texture2d, UDownloadImageTool * _downloadTool);

protected:

	UButton* m_BtnBack;			//返回.

	UButton* m_BtnAvatar;		//用户头像按钮.

	UButton* m_BtnSetting;		//画质设置按钮.

	UButton* m_BtnLanguage;		//语言设置按钮.

	UButton* m_BtnUseCourse;	//使用教程.

	UButton* m_BtnFeedBack;		//建议反馈.

	UButton* m_BtnAbout;		//关于我们.

	UButton* m_BtnClear;		//清理缓存.

	UButton* m_BtnUserExit;		//退出.

	UTextBlock* m_TextPerson;	//我的.

	UEditableTextBox* m_TextUserName;	//用户昵称字段.

	UTextBlock* m_TextSetting;			//画质设置.
	UTextBlock* m_TextSettingMsg;		//画质设置字段.

	UTextBlock* m_TextLanguage;			//语言设置.
	UTextBlock* m_TextLanguageMsg;		//语言设置字段.

	UTextBlock* m_TextCourse;			//使用教程.

	UTextBlock* m_TextSuggest;			//建议反馈.

	UTextBlock* m_TextAbout;			//关于我们.

	UTextBlock* m_TextClear;			//清理缓存.

	UTextBlock* m_TextVersion;			//版本信息.
	UTextBlock* m_TextVersionMsg;		//版本信息字段.

	UTextBlock* m_TextExit;				//退出.

	UImage* m_ImageHead;				//	用户头像图片.

	bool m_EditName;					//	判断是否修改了昵称.

	UPhotoPage * m_PhotoPage;			//	拍照.

	UDownloadImageTool * m_DownloadTool;
};

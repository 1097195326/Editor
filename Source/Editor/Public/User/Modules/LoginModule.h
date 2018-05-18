// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserData.h"
#include "UserInfo.h"
#include "MsgCenter.h"
#include "System/BaseData/BaseModule.h"

#define MESSAGESUCCESS 1000000

/**
 * 登录逻辑类，负责跟服务器交互.
 */
class EDITOR_API LoginModule : public BaseModule
{
	
public:

	// 临时保存用户数据.
	FString m_userInfo;

	FString mTempString;//临时传递字符串消息用的.

	int32 mMsgCode;

private:

	// 网络请求失败的消息体临时保存.
	msg_ptr m_errorMsgInfo;

	TMap<FString, FString> m_h5UrlMap;

public:

	// 广播登录成功的消息.
	int32 m_loginSucess = 101;		//	账号密码登陆成功.
	int32   m_CheckNumberSucess = 102;
	int32   m_FastLoginSucess = 103;//快速登录.
	int32   m_PassWordCheckSucess = 104; //重置密码验证码.
	int32   m_PassWordResetSucess = 105; //重置密码.
	int32   m_FirstLoginSucess = 106; //第一次登陆.

	int32	m_EditNameSuccess = 2001;		//	修改用户昵称.
	int32   m_GetHtmlUrlSuccess = 2009;		//	获取H5地址.
	int32	m_GetNickNameSuccess = 2005;	//	获取昵称.
	int32	m_GetUserHeadSuccess = 2010;	//	获取头像.

public:

	LoginModule();
	virtual ~LoginModule();

	FString GetUrlByKey(const FString & _key);

	inline class UserInfo* GetUserInfo() { return UserInfo::Get(); };
	inline struct FUserData* GetUserData() { return m_UserData; };
	inline FString GetMsgString() {
		return mTempString;
	};
	inline int32 GetMsgCode() {
		return mMsgCode;
	};
	virtual void OnLoad() override;
	virtual void OnRelease() override;

public:

	// 往服务器发送消息的区域.
	void RequestLoginServer(const FString& _Phone,const FString& _PassWord);

    //验证码请求.
	void RequestCheckNumber(FString PhoneNumber);

	//密码重置验证码
	void RequestPassWordResetCheckNumber(FString PhoneNumber);

	//验证码快速登录.
	void RequestFastLogin(FString _Phone,FString _CheckNumber);

	//修改密码请求.
	void RequestPassWordReset(FString _Phone, FString _PassWord, FString _CheckNumber);

	//修改用户昵称.
	void RequestUserNameReset(FString _Avatar, FString _UserName);

	//获取h5路径.
	void RequestGetHtmlUrlServer();

	//获取昵称.
	void RequestNickName();

	void RequestFirstLogin(FString PhoneNumber, FString CheckCode, FString PassWord);

	void LoginComplete();

	// 用户退出方法.
	void LogOut();

public:

	// 收到服务器消息的区域.
	void OnLoginResponseMsg(msg_ptr _msg);
	void OnCheckNumberResponseMsg(msg_ptr _msg);
	void OnFastLoginResponseMsg(msg_ptr _msg);

	void OnResetPassWordCheckNumberResponseMsg(msg_ptr _msg);	//密码修改验证码收到回调函数.
	void OnrResetPassWordResponseMsg(msg_ptr _msg);				//密码重置完成的消息回调.
	void OnrFirstLoginResponseMsg(msg_ptr _msg);

	void OnResetUserName(msg_ptr _msg);				//	修改用户昵称完成的消息回调.
	void OnGetHtmlUrlResponseMsg(msg_ptr _msg);		//	获得h5路径的消息回调.
	void OnGetNickName(msg_ptr _msg);				//	获取昵称的消息回调.

private:
	void OnLoginTimeout(msg_ptr _msg);		//	登录超时.

	void OnHttpErrorRepeat();   // 服务器连接失败重试事件.

	void OnHttpErrorCancel();   // 服务器连接失败不重试.

protected:
	FUserData* m_UserData;
};

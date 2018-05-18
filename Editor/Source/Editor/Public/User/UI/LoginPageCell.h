// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/UI/BaseUI.h"
#include "LoginModule.h"
#include "LoginPageCell.generated.h"



/**
 * 
 */
UCLASS()
class EDITOR_API ULoginPageCell : public UBaseUI
{
	GENERATED_BODY()
	
private:
	LoginModule * m_LoginModule;
	
public: 
	static const int32 BtnPid_CheckNumber = 1;
	static const int32  BtnPid_FastLogin = 2;
	static const int32 BtnPid_PassWordLogin = 3;
	static const int32 BtnPid_Login = 4;
	
	enum EnumLoginPage
	{
		FAST_REGIST = 0,
		LOGIN = 1,
		PASSWORD_RESET =2,
		FIRST_REGIST =3,
		AUTO_LOGIN =4,

	};
public:

	ULoginPageCell(const FObjectInitializer& ObjectInitializer);
	virtual bool OnStart() override;

	// 收到服务器登录成功的消息.
	void OnLoginResponseMsg(msg_ptr _msg);

	//收到验证码成功后的消息.
	void OnCheckResponseMsg(msg_ptr _msg);

	void OnFastResponseMsg(msg_ptr _msg);
	//密码重置验证码.
	void OnPassWordCheckResponseMsg(msg_ptr _msg);
	void OnPassWordResponseMsg(msg_ptr _msg);
	void OnFirstLoginResponseMsg(msg_ptr _msg);

	//	收到服务器发送给本地H5的路径.
	void OnGetHtmlResponseMsg(msg_ptr _msg);

	virtual void OnInit();

	virtual void OnRelease();

	
	virtual void OnSetParams(int32 _pid, FString & _content, ...);


	virtual void OnTick(float InDeltaTimeSecond);

	//
	void ClientButtonEvent(int32 _pid);//按钮点击事件.

	void HandleFastLogin();//点击快速登录按钮要处理的逻辑.

	void AutoLogin();

	UFUNCTION()
	void HandlePassWordLogin();//账号密码登录.
	void HandleCheckNumber();

	UFUNCTION()
	void HandleCheckRest(); //重置密码的验证码操作.

	UFUNCTION()
	void HandlePassWordReset(); //重置密码的验证码操作.

	UFUNCTION()
		void HandleFirstLogin(); //第一次登陸輸入密碼.

	void SetTip(FString Tip = FString(TEXT("")));

	//UI相关.
	void SetLoginTip(FString _tip = FString(TEXT("")));
	void SetPassWordTip(FString _tip = FString(TEXT("")));
	void ChangePage(EnumLoginPage _Page);
	
	UFUNCTION()
	void LoginBtnCallback();

	UFUNCTION()
	void GoPagePassWordReset();
	UFUNCTION()
	void SendChangePage();

	//工具相关.
	FString MsgJsonToString(msg_ptr _msg);
	void StartCoundDownReset();//验证码倒计时.
	void CountDownResetCheck();

	void StartCoundDownFast();//验证码倒计时.
	void CountDownnFastCheck();

	void GetProtocol(int32 _pid);

	FTimerHandle        CountdownTimerHandle;
	UTextBlock*			m_TextTip;
	UButton*			m_BtnBack;
	//密码登录.
	UCanvasPanel*		m_Login;
	UButton*			m_BtnLogin;
	UEditableTextBox*	m_TextBoxPhone; //电话.
	UEditableTextBox*	m_TextSecrte;//密码.
	UButton*            m_BtnForgetPassWord;
	UTextBlock*         m_LoginTextCheck;
	//快速注册.
	UCanvasPanel*       m_FastRegister;
	UEditableTextBox*	m_TextBoxPhoneReg;  //phone number.
	UEditableTextBox*	m_TextBoxCheckNumber;//验证码.
	UButton*		    m_BtnGetCheckReg;
	UButton*			m_BtnFastRegistLogin;
	UButton*			m_BtnPassWordLogin;
	UTextBlock*         m_TextBlock_CountDownReg; //倒计时.
	UCanvasPanel*       m_ProtocolCanvasPanel;
	UButton * m_Protocol;		//	协议.

	//忘记密码.
	UCanvasPanel*		m_PassWordReset;
	UTextBlock*			m_TextBlock_CountDownReset;
	UEditableTextBox*   m_EditableTextBox_PhoneReset;
	UEditableTextBox*   m_EditableTextBox_CheckedReset;
	UButton*			m_Button_GetCheckReset;
	UEditableTextBox*   m_EditableTextBox_PassWordReset;
	UButton*			m_Button_PassWordReset_Sure;
	UTextBlock*         m_PassWordResetTextCheck;
	
	//第一次注册.
	UCanvasPanel*		m_FirstRegister;
	UEditableTextBox*   m_EditableTextBox_Password0First;
	UEditableTextBox*   m_EditableTextBox_Password1First;
	UButton*            m_Button_CommitFirst;
	//自动登录
	UCanvasPanel*		m_AutoLogin;

private:
	int32 CountDownReset ;
	EnumLoginPage CurrentPage ;
};

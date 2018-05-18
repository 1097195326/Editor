#pragma once

#include "CoreMinimal.h"
#include "System/UI/BaseUI.h"
#include "LoginModule.h"
#include "MainModule.h"
#include "LogicMainScene.h"
#include "Win_SelectCell.h"
#include "Win_MainUMG.generated.h"


UCLASS()
class EDITOR_API UWin_MainUMG : public UBaseUI
{
	GENERATED_BODY()

public:
	UWin_MainUMG(const FObjectInitializer& ObjectInitializer);

	virtual bool OnStart() override;

	virtual void OnInit() override;

	virtual void OnRelease() override;

	virtual void OnSetParams(int32 _pid, FString & _content, ...) override;

	virtual void OnTick(float InDeltaTimeSecond) override;

	virtual void OnActivationUI() override;

	void OnButtonStart(int32 _param);
	void OnButtonSport(int32 _param);
	void OnButtonYoga(int32 _param);
	void OnButtonShop(int32 _param);
	void OnButtonOrder(int32 _param);
	void OnButtonAccount(int32 _param);
	void OnButtonPCDownload(int32 _param);
	void OnButtonShowAllMyStyle(int32 _param);
	void OnButtonLoginOut(int32 _param);
	void ShowTip();
	void HiddenTip();
	
	//	收到服务器发送给本地获取昵称成功的消息.
	void OnGetNickNameResponseMsg(msg_ptr _msg);

	//	收到服务器发给本地的获取我的作品的消息.
	void OnMyWorksContentDataFinished(msg_ptr _msg);

	//	创建我的作品cell.
	void CreateSelectCell(int32 _num);

public:
	LogicMainScene *m_LogicMainScene;

	FTimerHandle  TipTimerHandle;

	UButton* m_Btn_Start;

	UButton* m_Btn_Sport;

	UButton* m_Btn_Yoga;

	UButton* m_Btn_Shop;			//	商城按钮.

	UButton* m_Btn_Order;			//	订单按钮.

	UButton* m_Btn_Account;			//	账户.

	UButton* m_Btn_PCDownload;		//	PC下载按钮.

	UButton* m_Btn_ShowAllMyStyle;	//	全部作品.

	UButton* m_Btn_LoginOut;		//	注销.

	UTextBlock* m_Text_Tip;			//	提示内容.

	UTextBlock* m_TextUserName;		//	用户名.

	UCanvasPanel* m_Canv_Tips;		//	提示.

	UGridPanel* m_Grid_MyWorks;		//	主界面显示我的作品.

	TArray<UWin_SelectCell *> m_MyWorksContent;		//	储存我的作品.
};
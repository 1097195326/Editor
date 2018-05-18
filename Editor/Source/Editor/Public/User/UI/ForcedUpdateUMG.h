#pragma once
#include "CoreMinimal.h"
#include "BaseUI.h"
#include "UpdateResourceModule.h"
#include "ForcedUpdateUMG.generated.h"

UCLASS()
class EDITOR_API UForcedUpdateUMG :public UBaseUI
{
	GENERATED_BODY()

public:
	virtual bool OnStart() override;

	virtual void OnInit() override;

	virtual void OnRelease() override;

	// 被传入参数的列表.
	virtual void OnSetParams(int32 _pid, FString & _content, ...) override;

	// update.
	virtual void OnTick(float InDeltaTimeSecond) override;

	UForcedUpdateUMG(const FObjectInitializer& ObjectInitializer);

	void OnForceUpdateResponseMsg(msg_ptr _msg);
	//蓝图逻辑入口.
	void ExecuteLogic();
	//打开本地浏览器.
	void OpenURL(const FString &_Url);
	FString MsgJsonToString(msg_ptr _msg);

	virtual void OnActivationUI()override;
	virtual void OnBackstageUI()override;

	void QuitGame();
	void GoNext();
	void OnButtonCancel(int32 param);
	void OnButtonConfirm(int32 param);
	void SetDisplayText();
	//BP实现.
	void DoForcedUpdate();

	//BP实现.
	void DoCanUpdate();

	//BP实现.
	void HidenMe();
	



private:
	
	UButton *m_BtnCancel;
	UButton *m_BtnConfirm;
	UTextBlock *m_Text_Msg;
	UTextBlock *m_Text_Btn_Cancel;
	UTextBlock *m_Text_Btn_Confirm;
	UCanvasPanel *m_Canv_Main;
	FString m_Url;
	FString m_Content;
	int m_Forced;
};
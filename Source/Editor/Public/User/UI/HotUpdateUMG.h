#pragma once
#include "CoreMinimal.h"
#include "BaseUI.h"
#include "UpdateResourceModule.h"
//#include "LogicUpdateScene.h"
#include "HotUpdateUMG.generated.h"

UCLASS()
class EDITOR_API UHotUpdateUMG :public UBaseUI
{
	GENERATED_BODY()
public: // 重写以下的类.
	virtual bool OnStart() override;

	virtual void OnInit() override;

	virtual void OnRelease() override;

	// 被传入参数的列表.
	virtual void OnSetParams(int32 _pid, FString & _content, ...) override;
	// update.
	virtual void OnTick(float InDeltaTimeSecond) override;
	 
	virtual void OnActivationUI()override;
	virtual void OnBackstageUI()override;
	//重置界面.
	void Reset();
	//设置进度.
	void SetProgress(struct DownlaodInfoStruct *_DownloadInfoStruct);
private:

	UTextBlock *m_Text_Tip;
	UTextBlock *m_Text_This;
	UTextBlock *m_Text_Var_Size;
	UTextBlock *m_Text_Speed;
	UTextBlock *m_Text_Var_Speed;
	UTextBlock *m_Text_Progress;
	UProgressBar *m_Bar_Progress;
};
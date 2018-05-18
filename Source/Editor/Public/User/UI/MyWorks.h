// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainModule.h"
#include "SelectCell.h"
#include "ButtonSuitType.h"
#include "System/UI/BaseUI.h"
#include "MyWorks.generated.h"

#define MY_WORKS_CONTENT_RIGHT_PADDING 20
/**
 * 
 */
UCLASS()
class EDITOR_API UMyWorks : public UBaseUI
{
	GENERATED_BODY()

private:
	MainModule * m_MainModule;

	TArray<USelectCell *> m_MyWorksContent;	//	我的作品.

	bool m_CanChagePage;

public:
	UMyWorks(const FObjectInitializer& ObjectInitializer);

	//	复写BaseUI.
	virtual bool OnStart() override;

	virtual void OnInit();

	virtual void OnRelease();

	virtual void OnSetParams(int32 _pid, FString & _content, ...);

	virtual void OnTick(float InDeltaTimeSecond);

	//	打开其他页面.
	void OpenPageCell(int32 _pid);

	//	创建类别.
	UButtonSuitType* CreateNewHead(int32 _typeId, FString _name, bool _Last);

	//	更新类型事件.
	void UpdateHead();

	//	加载我的作品分页事件.
	UFUNCTION(BlueprintCallable,Category = ChangePage)
		void LoadMyWorks(int _page);

	//	获取所有的作品.
	void CreateSelectCell(int32 _num);

	//	获取所有作品成功的回调.
	void OnMyWorksContentDataFinished(msg_ptr _msg);

public:
	UButton* m_BtnReturn;			//	返回首页.
	
	UScrollBox* m_ScrollBoxHead;	//	类别.

	UScrollBox* m_ScrollBoxWorks;	//	作品滑动页.

	UGridPanel* m_GridPanelWorks;	//	作品.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = ChangePage)
		int32 m_PageIndex;

	int32 m_TotalPage;
	bool CheckCanLoad();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = ChangePage)
		bool m_Isloading;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhotoPage.h"
#include "DesignModule.h"
#include "BaseComponent.h"
#include "UMGDetailCell.h"
#include "System/UI/BaseUI.h"
#include "DesignActorRender.h"
#include "DetailPage.generated.h"

/*
 *
 */
UCLASS()
class EDITOR_API UDetailPage : public UBaseUI
{
	GENERATED_BODY()
	
private:
	DesignModule * m_DesignModule;

	TMap<int32, UUMGDetailCell *> m_DetailCell_Map;	//	花型，颜色，面料.

	TArray<SuitPatternT*> m_patternArray;

public:
	UDetailPage(const FObjectInitializer& ObjectInitializer);

	//	复写BaseUI.
	virtual bool OnStart() override;

	virtual void OnInit();

	virtual void OnRelease();

	virtual void OnSetParams(int32 _pid, FString & _content, ...);

	virtual void OnTick(float InDeltaTimeSecond);

	virtual void OnBackstageUI();

	//	选择花型按钮事件.
	void OnSelectPattern(int32 _pid);

	//	选择颜色按钮事件.
	void OnSelectColor(int32 _pid);

	//	选择面料按钮事件.
	void OnSelectMaterial(int32 _pid);

	//	修改花型高亮.
	void GetSelectPatternNum(int32 _select);

	//	修改颜色高亮.
	void GetSelectColorNum(int32 _select);

	//	修改面料高亮.
	void GetSelectMaterialNum(int32 _select);

	//	刷新面料列表.
	void InitFabricData(EnumDataType _Type, UsedFabricStruct & _usedFabricStruct);

	//	刷新颜色列表.
	void InitColorData(EnumDataType _Type, UsedFabricStruct & _usedFabricStruct);

	//	刷新花型列表.
	void InitPatternData(EnumDataType _Type, int32 _SliceTid, UsedFabricStruct & _usedFabricStruct);

	//	上传花型后的监听事件.
	void OnReloadPattern(msg_ptr _msg);

	//	加载花型.
	void OnAddPattern(msg_ptr _msg);

	void OnSetDetailShop();

	void HiddenTip();

	//换完面料完成通知.
	void OnResponseChangeFabric(msg_ptr _msg);

	//	请求花型分页.
	UFUNCTION(BlueprintCallable, Category = ChangePage)
		void AddPatternPage(int32 _PageIndex);

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = DetailPage)
		int32 m_PageIndex;		//	页数.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = DetailPage)
		bool m_ChangePage;		//	可以加载页.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = DetailPage)
		bool m_ACCESSORIES;		//	判断是否是辅料.

	int32 m_Style_Tid = -1;				//	片Tid.

	int32 m_SelectPatternId = -1;		//	选中的花型ID.

	int32 m_SelectColorId = -1;			//	选中的颜色ID.

	int32 m_SelectMaterialId = -1;		//	选中的面料ID.

	FLinearColor m_RGB;					//	当前颜色.

	bool m_UserPattern = false;			//	判断能否有花型.

	bool m_FirstBatch = false;

	UsedFabricStruct m_usedFabricStruct;	//	存储当前花型，颜色，面料.

	EnumDataType m_Type;		//	存储类型.

	UButton * m_BtnPattern;		//	花型.
	
	UButton * m_BtnColor;		//	颜色.

	UButton * m_BtnMaterial;	//	面料.

	UButton * m_BtnUpload;		//	上传花型.

	UButton * m_BtnOperate;		//	编辑花型.

	UTextBlock * m_TextPattern;	//	花型.

	UTextBlock * m_TextColor;	//	颜色.

	UTextBlock * m_TextMaterial;//	面料.

	UScrollBox * m_ScrollPattern;	//	花型ScrollBox.

	UScrollBox * m_ScrollColor;		//	颜色ScrollBox.

	UScrollBox * m_ScrollMaterial;	//	面料ScrollBox.

	UGridPanel * m_GridPattern;		//	花型GridPattern.

	UGridPanel * m_GridColor;		//	颜色GridColor.

	UGridPanel * m_GridMaterial;	//	面料GridMaterial.

	UCanvasPanel * m_CanvasPattern;	//	花型显示Panel.

	UCircularThrobber * m_CircularLoading;	//	加载图标.

	UUMGDetailCell *m_CurrentChangeFabricCell=nullptr;

	UPhotoPage * m_PhotoPage;		//	拍照.

	FTimerHandle m_TimerHandle;
};

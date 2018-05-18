// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WebBrowser.h"
#include "DesignModule.h"
#include "BaseComponent.h"
#include "UMGDetailCell.h"
#include "Win_PhotoPage.h"
#include "System/UI/BaseUI.h"
#include "Win_UMGDetailCell.h"
#include "Win_DetailPage.generated.h"

/*
 *
 */
UCLASS()
class EDITOR_API UWin_DetailPage : public UBaseUI
{
	GENERATED_BODY()
	
private:
	DesignModule * m_DesignModule;

	TMap<int32, UWin_UMGDetailCell *> m_DetailCell_Map;	//	花型，颜色，面料.

	TArray<SuitPatternT*> m_patternArray;

public:
	UWin_DetailPage(const FObjectInitializer& ObjectInitializer);

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

	//	设置购买消息.
	void SetBuyInfo(msg_ptr _msg);

	//	点击购买事件.
	void OnBuyGoods(int32 _pid);

	void OnSetDetailShop();

	void HiddenTip();

	//	下载图片.
	void DownloadIconImage(FString _imageUrl);

	//	下载图片成功的回调.
	void OnDownoloadFinished(bool _isScuccess, int32 _penetrateId, FString _url, UTexture2D * _texture2d, UDownloadImageTool * _downloadTool);

	//	请求花型分页.
	UFUNCTION(BlueprintCallable, Category = ChangePage)
		void AddPatternPage(int32 _PageIndex);

	//换完面料完成通知.
	void OnResponseChangeFabric(msg_ptr _msg);

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = DetailPage)
		int32 m_PageIndex;		//	页数.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = DetailPage)
		bool m_ChangePage;		//	可以加载页.

	int32 m_Style_Tid = -1;				//	片Tid.

	int32 m_SelectPatternId = -1;		//	选中的花型ID.

	int32 m_SelectColorId = -1;			//	选中的颜色ID.

	int32 m_SelectMaterialId = -1;		//	选中的面料ID.

	FLinearColor m_RGB;					//	当前颜色.

	bool m_UserPattern = false;			//	判断能否有花型.

	bool m_FirstBatch = false;

	UsedFabricStruct m_usedFabricStruct;	//	存储当前花型，颜色，面料.

	UWin_UMGDetailCell *m_CurrentChangeFabricCell = nullptr;

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

	UCanvasPanel * m_CanvasDetail;	//	detailCanvas.

	UCircularThrobber * m_CircularLoading;	//	加载图标.

	UWin_PhotoPage * m_PhotoPage;		//	拍照.

	FTimerHandle m_TimerHandle;

	UDownloadImageTool *m_DownloadImageTool;	//	异步下载.

	//	购买信息.
	UCanvasPanel * m_Cav_Buy;
	UTextBlock * m_Text_Good;		//	商品名称.
	UTextBlock * m_Text_SN;			//	货号.
	UTextBlock * m_Text_Color;		//	颜色/花型.
	UTextBlock * m_Text_Panton;		//	潘通色号.
	UTextBlock * m_Text_Price1_Name;//	样布.
	UTextBlock * m_Text_Price2_Name;//	大货期货.
	UTextBlock * m_Text_Price3_Name;//	大货现货.
	UTextBlock * m_Text_PromotionPrice1;		//	样布价格.
	UTextBlock * m_Text_Price1;
	UTextBlock * m_Text_PromotionPrice2;		//	大货期货价格.
	UTextBlock * m_Text_Price2;
	UTextBlock * m_Text_PromotionPrice3;		//	大货现货价格.
	UTextBlock * m_Text_Price3;	
	UImage * m_Img_One;
	UImage * m_Img_Two;
	UImage * m_Img_Three;
	UImage * m_Good_Icon;			//	商品图片.
	UVerticalBox * m_VerBox_One;
	UVerticalBox * m_VerBox_Two;
	UVerticalBox * m_VerBox_Three;
	UButton * m_Btn_Buy;			//	购买.
	UWebBrowser * m_Web_Icon;		//	商品icon.
};

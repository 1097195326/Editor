#pragma once
#include "CoreMinimal.h"
#include "System/UI/BaseUI.h"
#include "LoginModule.h"
#include "UpdateResourceModule.h"
#include "LogicDesignScene.h"
#include "Win_UMGSuitPartMenu.h"
#include "Win_UMGSuitComponentMenu.h"
#include "Win_UMGSuitSliceMenu.h"
#include "DesignModule.h"
#include "Win_UMGSuitComponentDetail.h"
#include "Win_DetailPage.h"
#include "BaseComponent.h"
#include "Win_DesignUMG.generated.h"

UCLASS()
class EDITOR_API UWin_DesignUMG : public UBaseUI
{
	GENERATED_BODY()

	UWin_DesignUMG(const FObjectInitializer& ObjectInitializer);
public:

	virtual bool OnStart() override;

	virtual void OnInit() override;

	virtual void OnRelease() override;

	virtual void OnLoad() override;

	virtual void OnSetParams(int32 _pid, FString & _content, ...) override;

	virtual void OnTick(float InDeltaTimeSecond) override;

	void OnButtonWalker(int32 param);
	void OnButtonHideRight(int32 param);
	void OnButtonShowRight(int32 param);

	void OnButtonShop(int32 param);
	void OnShopButtonRequestCallBack(msg_ptr _msg);
	void OnShopButtonSaveCallBack(msg_ptr _msg);

	void OnButtonHideSlide(int32 param);
	void OnButtonShowSilde(int32 param);
	void OnButtonSave(int32 param);
	void OnButtonPieceClose(int32 param);
	void OnButtonCancel(int32 param);
	void OnButtonSliceMenu(int32 param);
	void OnButtonDetailClose(int32 param);
	void OnButtonDetailShop(int32 param);
	void OnConfirmSaveEvent();
	void OnCancelSaveEvent();

	void InitSuitPartMenu();
	void InitSuitPartBatchMenu();
	void InitSuitCompMenu(int32 _SuitPartTid);
	void InitSuitCompBatchMenu();
	void InitSuitCompDetail(TArray<SuitComponentT* > &_CompTidArray);
	void InitSuitSliceMenu(int32 _SuitComponentTid);
	void InitDetailPage(EnumDataType _Type, int32 _Tid, UsedFabricStruct & _usedFabricStruct);

	void ShowSuiComponentMenu();
	void ShowSuitSliceMenu();
	void ShowSuitPartMenu();
	void ShowComponentDetail();
	void ShowSliceMenuButton();
	void ShowDetailPage();

	void HidenSuiComponentMenu();
	void HidenSuitPartMenu();
	void HidenSuitSliceMenu();
	void HidenComponentDetail();
	void HidenSliceMenuButton();
	void HidenDetailPage();
	void HidenFabricDetail();
	void SetBuyUrl(FString _buyUrl);
	void SetDetailShopView(bool _CanShop);
	void DoSave();

	int32 GetStyleSliceTid();

	//处理相机,BP实现.
	UFUNCTION(BlueprintNativeEvent)
	void OnChangeSuitComponent(FMeshComponentStruct _MeshCompStruct);
	void OnChangeSuitComponent_Implementation(FMeshComponentStruct _MeshCompStruct);

	//处理相机,BP实现.
	UFUNCTION(BlueprintNativeEvent)
	void OnChangeSuitSlice(FMeshComponentStruct _MeshCompStruct);
	void OnChangeSuitSlice_Implementation(FMeshComponentStruct _MeshCompStruct);

	//处理相机,BP实现.
	UFUNCTION(BlueprintNativeEvent)
	void OnReturnToSuitPartMenu();
	void OnReturnToSuitPartMenu_Implementation();

	//处理相机,BP实现.
	UFUNCTION(BlueprintNativeEvent)
	void OnReturnToCompMenu();
	void OnReturnToCompMenu_Implementation();

	//处理相机,BP实现.
	UFUNCTION(BlueprintNativeEvent)
	void InitCamera(FMeshComponentStruct _MeshCompStruct);
	void InitCamera_Implementation(FMeshComponentStruct _MeshCompStruct);

	//保存按钮click.
	UFUNCTION(BlueprintNativeEvent)
	void OnSaveButtonClick();
	void OnSaveButtonClick_Implementation();

	//处理相机,BP实现.
	UFUNCTION(BlueprintCallable)
		FMeshComponentStruct GetStyleMeshComponentStruct();
	//延迟获取静态模型数据.
	void GetCamCompMeshData();

	//刷新DetailPage 片操作.
	void UpdateSliceDetailPage();

	//刷新DetailPage 部件操作.
	void UpdateComponentDetailPage(UsedFabricStruct _UsedFabricStruct, bool isInitDetailPage=true);

	//刷新DetailPage 整体操作.
	void UpdatePartDetailPage(UsedFabricStruct _UsedFabricStruct,bool isInitDetailPage=true);

	//设置DetailTexture;
	void SetDetailTexture(UTexture2D * _texture2D);

	//设置DetailColor;
	void SetDetailColor(FLinearColor _RGB);

	//设置DetailMaterial;
	void SetDetailMaterial(FLinearColor _RGB, UTexture2D * _texture2D);

	//	设置购买失败.
	void OnShopError();

	//渲染完CompMesh.
	void OnCompMeshShow(msg_ptr _msg);
	
	//设置显示片操作按钮.
	void OnSetPieceShow();

public:
	UButton* m_Btn_Walker;
	UButton* m_Btn_HideRight;
	UButton* m_Btn_ShowRight;
	UButton* m_Btn_Shop;
	UButton* m_Btn_HideSlide;
	UButton* m_Btn_ShowSlide;
	UButton* m_Btn_Cancel;
	UButton* m_Btn_Save;
	UButton* m_Btn_PieceClose;
	UButton* m_Btn_SliceMenu;		//	片操作按钮.
	UButton* m_Btn_DetailClose;		//	花型，颜色，面料关闭按钮.
	UButton* m_Btn_DetailShop;		//	花型，颜色，面料购物车按钮.
	
	UCanvasPanel* m_Canv_Left;
	UCanvasPanel* m_Canv_Right;
	UCanvasPanel* m_Canv_Bottom;

	UCanvasPanel* m_Canv_Menus;
	UCanvasPanel* m_Canv_Detail;	//	花型，颜色，面料Panel.

	FTimerHandle m_TimerHandle;

	UWin_UMGSuitComponentDetail *m_ComponentDetail;
	UWin_UMGSuitPartMenu* m_UMG_SuitPartMenu;
	UWin_UMGSuitComponentMenu* m_UMG_SuitComponentMenu;
	UWin_UMGSuitSliceMenu* m_UMG_SuitSliceMenu;
	UWin_DetailPage * m_DetailPage;			//	花型，颜色，面料UI.

	UImage * m_ImgDetailTexture;		//	Detail贴图.
	UCanvasPanel * m_Canv_DetailShop;	//	Detail购物车.

	UCanvasPanel * m_Canv_Tip;			//	提示.
	UTextBlock * m_TextTip;				//	提示内容.
	FString m_BuyUrl;					//	购买Url.

	bool m_CanBuy = false;				//	可以购买.
	int32 m_ComponentTid;				//	部件Tid;
	UsedFabricStruct m_UsedFabric_Struct;	//	batchTid;
	bool m_IsDetailCompCellClicked=false;

};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DesignModule.h"
#include "BaseComponent.h"
#include "System/UI/BaseUI.h"
#include "DownloadImageTool.h"
#include "UMGDetailCell.generated.h"


/**
 * 
 */
enum EnumDataType
{
	TYPE_STYLE_ALL = 0,
	TYPE_STYLE_COMPONENT,
	TYPE_STYLE_SLICE
};

UCLASS()
class EDITOR_API UUMGDetailCell : public UBaseUI
{
	GENERATED_BODY()

private:
	DesignModule *mDesignModule;

	bool m_downloadSuccess = false;
	
public:
	UUMGDetailCell(const FObjectInitializer& ObjectInitializer);

	//复写BaseUI.
	virtual bool OnStart() override;

	virtual void OnInit();

	virtual void OnRelease();

	virtual void OnSetParams(int32 _pid, FString & _content, ...);

	virtual void OnTick(float InDeltaTimeSecond);

	

	//	点击事件.
	void OnButtonClick(int32 _param);

	//windsows平台点击事件.
	void OnWindowsButtonClickEvent();

	//moblie平台点击事件.
	void OnMobileButtonClickEvent();

	//	设置颜色.
	void SetColor(FLinearColor _RGB);

	//	设置材质.
	void SetMaterial(FString _iconUrl);

	//	设置掩图..
	void SetMaterialMask(FString _iconUrl);

	//	设置图片.
	UTexture2D * SetTexture2D(FString _iconUrl);

	//	下载图片.
	void DownloadIconImage();
	void DownloadIconImage2();

	//	设置购买消息.
	void SetBuyInfo(msg_ptr _msg);

	//	下载图片成功的回调.
	void OnDownoloadFinished(bool _isScuccess, int32 _penetrateId, FString _url, UTexture2D * _texture2d, UDownloadImageTool * _downloadTool);
	void OnDownoloadFinished2(bool _isScuccess, int32 _penetrateId, FString _url, UTexture2D * _texture2d, UDownloadImageTool * _downloadTool);

	//	设置点击面料高亮.
	void OnSetMaterialView();

	//更换面料.
	void ChangeFabric();
public:
	int32 m_Style_Tid = -1;					//	片Tid.

	UsedFabricStruct m_usedFabricStruct;	//	存储当前花型，颜色，面料.

	EnumDataType m_Type = EnumDataType::TYPE_STYLE_SLICE;	//	存储类型.

	FString m_patternId;					//	花型的Tid.

	FString m_OwnerType;					//	花型类别.

	FString m_ColorGoodsId;					//	颜色GoodsId.

	int32 m_ColorId;						//	颜色Tid.

	int32 m_MaterialId;					//	面料Tid.

	int32 m_SelectId;						//	选中的ID.

	int32 m_SelectType=1;						//	选中的类型.

	bool m_IsBuy;							//	是否可以购买.

	bool m_IsMetal = false;					//	是否金属.

	UTexture2D * m_PatternTexture;			//	花型贴图.

	UTexture2D * m_ColorTexture;			//	颜色贴图.

	UTexture2D * m_DetailMaterial;			//	面料.

	FLinearColor m_RGB;						//	颜色.

	UImage* m_Image_Back;					//	背景.

	UImage* m_Image_select;

	UImage* m_Image_Icon;

	UImage* m_Image_Material;

	UImage* m_Image_MaterialIcon;

	UButton* m_Btn_Select;

	UBorder * m_Border;
	
	FString m_IconUrl;

	UDownloadImageTool *m_DownloadImageTool;

	FTimerHandle m_CountDownTimerHander;
};

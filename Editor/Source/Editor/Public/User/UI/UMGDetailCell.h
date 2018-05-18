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

	//��дBaseUI.
	virtual bool OnStart() override;

	virtual void OnInit();

	virtual void OnRelease();

	virtual void OnSetParams(int32 _pid, FString & _content, ...);

	virtual void OnTick(float InDeltaTimeSecond);

	

	//	����¼�.
	void OnButtonClick(int32 _param);

	//windsowsƽ̨����¼�.
	void OnWindowsButtonClickEvent();

	//moblieƽ̨����¼�.
	void OnMobileButtonClickEvent();

	//	������ɫ.
	void SetColor(FLinearColor _RGB);

	//	���ò���.
	void SetMaterial(FString _iconUrl);

	//	������ͼ..
	void SetMaterialMask(FString _iconUrl);

	//	����ͼƬ.
	UTexture2D * SetTexture2D(FString _iconUrl);

	//	����ͼƬ.
	void DownloadIconImage();
	void DownloadIconImage2();

	//	���ù�����Ϣ.
	void SetBuyInfo(msg_ptr _msg);

	//	����ͼƬ�ɹ��Ļص�.
	void OnDownoloadFinished(bool _isScuccess, int32 _penetrateId, FString _url, UTexture2D * _texture2d, UDownloadImageTool * _downloadTool);
	void OnDownoloadFinished2(bool _isScuccess, int32 _penetrateId, FString _url, UTexture2D * _texture2d, UDownloadImageTool * _downloadTool);

	//	���õ�����ϸ���.
	void OnSetMaterialView();

	//��������.
	void ChangeFabric();
public:
	int32 m_Style_Tid = -1;					//	ƬTid.

	UsedFabricStruct m_usedFabricStruct;	//	�洢��ǰ���ͣ���ɫ������.

	EnumDataType m_Type = EnumDataType::TYPE_STYLE_SLICE;	//	�洢����.

	FString m_patternId;					//	���͵�Tid.

	FString m_OwnerType;					//	�������.

	FString m_ColorGoodsId;					//	��ɫGoodsId.

	int32 m_ColorId;						//	��ɫTid.

	int32 m_MaterialId;					//	����Tid.

	int32 m_SelectId;						//	ѡ�е�ID.

	int32 m_SelectType=1;						//	ѡ�е�����.

	bool m_IsBuy;							//	�Ƿ���Թ���.

	bool m_IsMetal = false;					//	�Ƿ����.

	UTexture2D * m_PatternTexture;			//	������ͼ.

	UTexture2D * m_ColorTexture;			//	��ɫ��ͼ.

	UTexture2D * m_DetailMaterial;			//	����.

	FLinearColor m_RGB;						//	��ɫ.

	UImage* m_Image_Back;					//	����.

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

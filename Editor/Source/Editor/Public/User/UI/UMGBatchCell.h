// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/UI/BaseUI.h"
#include "BaseComponent.h"
#include "DownloadImageTool.h"
#include "LogicDesignScene.h"
#include "UMGBatchCell.generated.h"

/**
*
*/
enum EnumBatchCellType
{
	BATCH_CELL_PART=0,
	BATCH_CELL_COMPONENT
};
UCLASS()
class EDITOR_API UUMGBatchCell : public UBaseUI
{
	GENERATED_BODY()

public:
	UUMGBatchCell(const FObjectInitializer& ObjectInitializer);

	//复写BaseUI.
	virtual bool OnStart() override;

	virtual void OnInit();

	virtual void OnRelease();

	virtual void OnSetParams(int32 _pid, FString & _content, ...);

	virtual void OnTick(float InDeltaTimeSecond);

	void OnButtonClick(int32 _param);
	void SetColor();
	void SetTexture2D();
	void SetUsedFabricStruct(UsedFabricStruct _inStruct);
	void ShowSelectedIcon();
	void HiddenSelectedIcon();
	void DownloadIconImage(const FString &_IconHttpPath);
	void OnDownoloadFinished(bool _isScuccess, int32 _penetrateId, FString _url, UTexture2D * _texture2d, UDownloadImageTool * _downloadTool);

	UsedFabricStruct GetUsedFabricStruct();
	
	void SetCellType(EnumBatchCellType _type);
	void SetParent(UBaseUI *_parent);
public:
	UsedFabricStruct m_UsedFabric_Struct;
private:
	UImage* m_Image_select;
	UImage* m_Image_Icon;
	UButton* m_Btn_Select;
	EnumBatchCellType m_Type;
	UBaseUI * m_Parent;
	UDownloadImageTool *m_DownloadImageTool;
};
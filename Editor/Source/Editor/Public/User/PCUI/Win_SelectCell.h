// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/UI/BaseUI.h"
#include "DownloadImageTool.h"
#include "DesignModule.h"
#include "SelectCell.h"
#include "Win_SelectCell.generated.h"


/**
 * 
 */
// struct SelectCellData
// {
// public:
// 	SelectCellData(){}
// 	FString m_IconUrl;
// 	int32 m_Tid;
// 	int32 m_Rid;
// };
UCLASS()
class EDITOR_API UWin_SelectCell : public UBaseUI
{
	GENERATED_BODY()
	
		UWin_SelectCell(const FObjectInitializer& ObjectInitializer);
public:

	virtual bool OnStart() override;

	virtual void OnInit() override;

	virtual void OnRelease() override;

	virtual void OnSetParams(int32 _pid, FString & _content, ...) override;

	virtual void OnTick(float InDeltaTimeSecond) override;
	void SetIconURL(const FString _Url);
	void SetTid(int32 _Tid);
	void SetRid(int32 _Rid);
	void SetDesignType(EnumDesignType _type);
	void DownloadIconImage();
	void OnDownoloadFinished(bool _isScuccess, int32 _penetrateId, FString _url, UTexture2D * _texture2d, UDownloadImageTool * _downloadTool);
	void OnButtonClick(int32 _Param);
public:
	UButton* m_Btn_Select;
	UImage* m_Image_ICON = nullptr;
	FString m_IconUrl;
	int32 m_Rid;
	int32 m_Tid;
	UDownloadImageTool *m_DownloadImageTool;
	EnumDesignType m_EnumDesignType= EnumDesignType::DESIGN_BY_SUIT;
	bool m_isInit = false;
	bool m_downloadSuccess = false;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/UI/BaseUI.h"
#include "Win_UMGMenuCell.h"
#include "DesignModule.h"
#include "LogicDesignScene.h"
#include "Win_UMGSuitComponentDetail.generated.h"

/**
 * 
 */
#define  MAX_CONTNET_COLUME_NUM 5

UCLASS()
class EDITOR_API UWin_UMGSuitComponentDetail : public UBaseUI
{
	GENERATED_BODY()

public:
	UWin_UMGSuitComponentDetail(const FObjectInitializer& ObjectInitializer);

	//复写BaseUI.
	virtual bool OnStart() override;

	virtual void OnInit();

	virtual void OnRelease();

	virtual void OnTick(float InDeltaTimeSecond);

	virtual void OnActivationUI()override;

	virtual void OnBackstageUI()override;

	void InitData(TArray<SuitComponentT* > &_CompTidArray);

	void SetUniqueSelected(int32 _CompTid);

	void SetParentTid(int32 _Tid);

	void OnReturnDetail(int32 _pid);

	int32 GetParentTid();
                       
public:
	UGridPanel  *m_Grid_Content;

	UButton * m_BtnReturn;

	int32  m_CurSelectedTid;

private:
	TMap<int32, UWin_UMGMenuCell *> m_MenuCell_Map;

	int32 m_Parent;
};
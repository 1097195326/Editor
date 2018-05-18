// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/UI/BaseUI.h"
#include "UMGMenuCell.h"
#include "DesignModule.h"
#include "UMGMenuCell.h"
#include "LogicDesignScene.h"
#include "UMGSuitComponentDetail.generated.h"

/**
 * 
 */
#define  MAX_CONTNET_COLUME_NUM 5
UCLASS()
class EDITOR_API UUMGSuitComponentDetail : public UBaseUI
{
	GENERATED_BODY()

public:
	UUMGSuitComponentDetail(const FObjectInitializer& ObjectInitializer);

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
	int32 GetParentTid();

	                            
public:
	UGridPanel  *m_Grid_Content;
	int32  m_CurSelectedTid;
private:
	TMap<int32, UUMGMenuCell *> m_MenuCell_Map;
	int32 m_Parent;
};
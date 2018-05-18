// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/UI/BaseUI.h"
#include "DesignModule.h"
#include "Win_UMGMenuCell.h"
#include "Win_UMGSuitSliceMenu.generated.h"

/**
 * 
 */
UCLASS()
class EDITOR_API UWin_UMGSuitSliceMenu : public UBaseUI
{
	GENERATED_BODY()
	
	UWin_UMGSuitSliceMenu(const FObjectInitializer& ObjectInitializer);
public:

	virtual bool OnStart() override;

	virtual void OnInit() override;

	virtual void OnRelease() override;

	virtual void OnSetParams(int32 _pid, FString & _content, ...) override;

	virtual void OnTick(float InDeltaTimeSecond) override;

	virtual void OnActivationUI()override;
	virtual void OnBackstageUI()override;
	void SetUniqueSelected(int32 _SliceTid);
	void SetUniqueHiddenSelected(int32 _SliceTid);
	void InitData(int32 _SuitCompTid);
	int32 GetComponentTid();
	int32 GetCurSelectedSliceTid();



private:
	void OnReturnButtonClick(int32 _param);
private:
	TMap<int32, UWin_UMGMenuCell*> m_MenuCell_Map;
	UVerticalBox *m_VBox_SuitSlice_Content;
	UButton* m_Btn_Return;
	int32 m_SuitComp_Tid;
	int32 m_CurrentSelectedSlice_Tid=-1;
};

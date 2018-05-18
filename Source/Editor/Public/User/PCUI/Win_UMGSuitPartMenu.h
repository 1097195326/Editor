#pragma once
#include "CoreMinimal.h"
#include "System/UI/BaseUI.h"
#include "LogicDesignScene.h"
#include "DesignModule.h"
#include "Win_UMGMenuCell.h"
#include "Win_UMGBatchCell.h"
#include "BaseComponent.h"
#include "Win_UMGSuitPartMenu.generated.h"

UCLASS()
class EDITOR_API UWin_UMGSuitPartMenu : public UBaseUI
{
	GENERATED_BODY()

		UWin_UMGSuitPartMenu(const FObjectInitializer& ObjectInitializer);
public:

	virtual bool OnStart() override;

	virtual void OnInit() override;

	virtual void OnRelease() override;

	virtual void OnLoad() override;

	virtual void OnSetParams(int32 _pid, FString & _content, ...) override;

	virtual void OnTick(float InDeltaTimeSecond) override;

	virtual void OnActivationUI()override;

	virtual void OnBackstageUI()override;
	void InitData();
	void InitBatchData();
	void OnReturnButtonClick(int32 _param);
	void SetUniqueSelectedBatchIcon(UsedFabricStruct _UsedFabricStruct);

	void SetUniqueHiddenSelectedBatchIcon(UsedFabricStruct _UsedFabricStruct);
public:
	UVerticalBox * m_VBox_Batch_Content;
	UVerticalBox * m_VBox_SuitPart_Content;
	UButton* m_Btn_Return;
	TArray<UWin_UMGBatchCell *> m_BatchCell_Array;
	
};
#pragma once
#include "CoreMinimal.h"
#include "System/UI/BaseUI.h"
#include "LoginModule.h"
#include "UpdateResourceModule.h"
#include "LogicDesignScene.h"
#include "UMGBatchCell.h"
#include "UMGSuitComponentMenu.generated.h"

UCLASS()
class EDITOR_API UUMGSuitComponentMenu : public UBaseUI
{
	GENERATED_BODY()
	UUMGSuitComponentMenu(const FObjectInitializer& ObjectInitializer);
public:

	virtual bool OnStart() override;

	virtual void OnInit() override;

	virtual void OnRelease() override;

	virtual void OnSetParams(int32 _pid, FString & _content, ...) override;

	virtual void OnTick(float InDeltaTimeSecond) override;

	virtual void OnActivationUI()override;
	
	virtual void OnBackstageUI()override;
	void InitData(int32 _SuitPartTid);
	void InitBatchData();
	void OnReturnButtonClick(int32 _param);
	int32 GetSuitPartTid();
	void SetUniqueSelectedIcon(int32 _Tid);

	void SetUniqueHiddenSelectedIcon(int32 _Tid);
	
	void SetUniqueSelectedBatchIcon(UsedFabricStruct _UsedFabricStruct);

	void SetUniqueHiddenSelectedBatchIcon(UsedFabricStruct _UsedFabricStruct);

public:
	TArray<UUMGBatchCell *> m_BatchCell_Array;

private:
	UVerticalBox * m_VBox_Batch_Content;
	UVerticalBox * m_VBox_SuitComponent_Content;
	UButton* m_Btn_Return;
	TArray<UUMGMenuCell*> m_MenuCell_Array;
	int32 m_SuitPart_Tid=-1;
	UImage* m_Image_ICON;
	
};
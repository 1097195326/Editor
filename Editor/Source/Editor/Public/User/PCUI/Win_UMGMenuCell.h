#pragma once
#include "CoreMinimal.h"
#include "DesignModule.h"
#include "BaseComponent.h"
#include "System/UI/BaseUI.h"
#include "DownloadImageTool.h"
#include "ComponentStruct.h"
#include "UMGMenuCell.h"
#include "Win_UMGMenuCell.generated.h"

/**
*
*/
// enum EnumCellType
// {
// 	T_SUIT_PART_MENU_CELL = 1,
// 	T_SUIT_COMPONENT_MENU_CELL,
// 	T_SUIT_ACCESSORIES_MENU_CELL,  
// 	T_SUIT_STITCH_MENU_CELL,
// 	T_SUIT_COMPONENT_DETAIL_CELL,
// 	T_SUIT_SLICE_MENU_CELL
// };

UCLASS()
class EDITOR_API UWin_UMGMenuCell : public UBaseUI
{
	GENERATED_BODY()

public:
	UWin_UMGMenuCell(const FObjectInitializer& ObjectInitializer);

	//复写BaseUI.
	virtual bool OnStart() override;

	virtual void OnInit();

	virtual void OnRelease();

	virtual void OnSetParams(int32 _pid, FString & _content, ...);

	virtual void OnTick(float InDeltaTimeSecond);
	
	virtual void OnActivationUI()override;

	void OnButtonClick(int32 _param);

	void OnButtonSliceClick(int32 _param);

	void SetCellType(EnumCellType _type);

	void SetIconUrl(const FString &_url);

	void SetTid(int32 _Tid);

	void SetParent(UBaseUI *_ui);

	void SetIconTexture2D();

	void ShowSelectedIcon();

	void HidenSelectedIcon();

	const FString &GetIconUrl();

	int32 GetTid();

	UBaseUI *GetParent();

	EnumCellType GetCellType();

public:
	EnumCellType m_Cell_Type=EnumCellType::T_SUIT_PART_MENU_CELL;

	UImage* m_Image_select;

	UImage* m_Image_ICON;

	UButton* m_Btn_Select;

	UButton* m_Btn_Slice;

	int32 m_Tid;

	FString m_IconUrl;

	UBaseUI *m_Parent;

	UsedFabricStruct _usedFabricStruct;
};
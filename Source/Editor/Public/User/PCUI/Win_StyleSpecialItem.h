#pragma once



#include "CoreMinimal.h"
#include "System/UI/BaseUI.h"
#include "DownloadImageTool.h"
#include "DesignModule.h"
#include "Win_StyleSpecialIcon.h"
#include "Win_StyleSpecialItem.generated.h"


/**
*
*/

UCLASS()
class EDITOR_API UWin_StyleSpecialItem : public UBaseUI
{
	GENERATED_BODY()

		UWin_StyleSpecialItem(const FObjectInitializer& ObjectInitializer);

private:
	UButton * m_BackButton;
	UScrollBox *	 m_ScrollBoxItem;
	UGridPanel * m_GridPanelSuitItem;

	UTextBlock * m_ShowTitle;

	TArray<UWin_StyleSpecialIcon *> m_AllSyles;
public:

	void OnBackClick(int32 index);
	virtual bool OnStart() override;

	virtual void OnInit() override;

	virtual void OnRelease() override;

	virtual void OnSetParams(int32 _pid, FString & _content, ...) override;

	virtual void OnTick(float InDeltaTimeSecond) override;

	void CreateSelectCell();
	void ClickCallBack(msg_ptr _msg);
};
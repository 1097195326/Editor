#pragma once


#include "CoreMinimal.h"
#include "System/UI/BaseUI.h"
#include "DownloadImageTool.h"
#include "DesignModule.h"
#include "Win_StyleSpecialSign.generated.h"


/**
*
*/

UCLASS()
class EDITOR_API UWin_StyleSpecialSign : public UBaseUI
{
	GENERATED_BODY()

		UWin_StyleSpecialSign(const FObjectInitializer& ObjectInitializer);
public:

	UButton * m_Button;

	virtual bool OnStart() override;

	virtual void OnInit() override;

	virtual void OnRelease() override;

	void SetNormal();
	void SetSelect();

};
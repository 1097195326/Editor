#pragma once


#include "CoreMinimal.h"
#include "System/UI/BaseUI.h"
#include "DownloadImageTool.h"
#include "DesignModule.h"
#include "StyleSpecialSign.generated.h"


/**
*
*/

UCLASS()
class EDITOR_API UStyleSpecialSign : public UBaseUI
{
	GENERATED_BODY()

		UStyleSpecialSign(const FObjectInitializer& ObjectInitializer);
public:

	UButton * m_Button;

	virtual bool OnStart() override;

	virtual void OnInit() override;

	virtual void OnRelease() override;

	void SetNormal();
	void SetSelect();

};
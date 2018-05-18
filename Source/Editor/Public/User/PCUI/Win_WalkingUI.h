#pragma once


#include "CoreMinimal.h"
#include "System/UI/BaseUI.h"
#include "Win_WalkingUI.generated.h"



UCLASS()
class EDITOR_API UWin_WalkingUI : public UBaseUI
{
	GENERATED_BODY()
public:

	UWin_WalkingUI(const FObjectInitializer& ObjectInitializer);
public:

	virtual bool OnStart() override;

	virtual void OnInit() override;

	virtual void OnRelease() override;

	virtual void OnSetParams(int32 _pid, FString & _content, ...) override;

	virtual void OnTick(float InDeltaTimeSecond) override;

	void OnButtonClick(int32 _Param);

public:
	UButton* m_BackButton;

};
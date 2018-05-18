#pragma once


#include "CoreMinimal.h"
#include "System/UI/BaseUI.h"
#include "WalkingUI.generated.h"



UCLASS()
class EDITOR_API UWalkingUI : public UBaseUI
{
	GENERATED_BODY()
public:

	UWalkingUI(const FObjectInitializer& ObjectInitializer);
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
#pragma once


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/UI/BaseUI.h"
#include "DownloadImageTool.h"
#include "DesignModule.h"
#include "HomeInfoManager.h"
#include "Win_StyleSpecialButton.generated.h"


/**
*
*/

UCLASS()
class EDITOR_API UWin_StyleSpecialButton : public UBaseUI
{
	GENERATED_BODY()

		UWin_StyleSpecialButton(const FObjectInitializer& ObjectInitializer);
private:
	HomeInfoT * m_HomeInfo;

	void OnDownoloadFinished(bool _isScuccess, int32 _penetrateId, FString _url, UTexture2D * _texture2d, UDownloadImageTool * _downloadTool);
	void OnButtonClick(int32 _Param);

	UDownloadImageTool * m_DownloadImageTool;

	UButton * m_ShowButton;
	UImage *	m_ShowImage;
	bool		m_IsResponsing;
	class UWin_RecommendStyleUMG *	m_Parent;
public:

	virtual bool OnStart() override;

	virtual void OnInit() override;

	virtual void OnRelease() override;

	virtual void OnSetParams(int32 _pid, FString & _content, ...) override;

	virtual void OnTick(float InDeltaTimeSecond) override;

	void InitWithData(HomeInfoT *);

	void SetLogicParent(class UWin_RecommendStyleUMG * parent);

	void DownloadImage();

	void GetDataFinish(bool _finish);

};
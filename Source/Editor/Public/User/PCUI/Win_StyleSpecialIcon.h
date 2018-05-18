#pragma once

#include "CoreMinimal.h"
#include "System/UI/BaseUI.h"
#include "DownloadImageTool.h"
#include "DesignModule.h"
#include "Win_StyleSpecialIcon.generated.h"



UCLASS()
class EDITOR_API UWin_StyleSpecialIcon : public UBaseUI
{
	GENERATED_BODY()
public:

	UWin_StyleSpecialIcon(const FObjectInitializer& ObjectInitializer);
public:

	void OnClick(int32 index);
	virtual bool OnStart() override;

	virtual void OnInit() override;

	virtual void OnRelease() override;

	virtual void OnSetParams(int32 _pid, FString & _content, ...) override;

	virtual void OnTick(float InDeltaTimeSecond) override;

	void SetIconURL(const FString _Url);
	void SetContentId(FString _id);
	void SetDesignType(EnumDesignType _type);
	void DownloadIconImage();
	void OnDownoloadFinished(bool _isScuccess, int32 _penetrateId, FString _url, UTexture2D * _texture2d, UDownloadImageTool * _downloadTool);
	void OnButtonClick(int32 _Param);
	void SetDesignWidth(float _width);

public:
	UButton* m_Btn_Select;
	UImage* m_Image_ICON;
	UCanvasPanelSlot * m_CanvasPanelSlot;
	UCanvasPanelSlot * m_ButtnSlot;

	FString m_IconUrl;
	FString m_ContentId;
	int32 m_Tid;
	float m_DesignWidth;
	UDownloadImageTool *m_DownloadImageTool;
	EnumDesignType m_EnumDesignType = EnumDesignType::DESIGN_BY_SUIT;

};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DesignModule.h"
#include "System/UI/BaseUI.h"
#include "PhotoPage.generated.h"

/**
 * 
 */
UCLASS()
class EDITOR_API UPhotoPage : public UBaseUI
{
	GENERATED_BODY()

private:
	DesignModule * m_DesignModule;

public:
	UPhotoPage(const FObjectInitializer& ObjectInitializer);

	//	复写BaseUI.
	virtual bool OnStart() override;

	virtual void OnInit();

	virtual void OnRelease();

	virtual void OnSetParams(int32 _pid, FString & _content, ...);

	virtual void OnTick(float InDeltaTimeSecond);


	void PhotoPicture(int32 _pid);

	//打开系统相册.
	void GetUploadPattern();

	//选择图片后的回调.
	void CBOnOpenPattern(bool isSuccess, FString inIconFilePath, FString inFilePath, FVector2D inDPI, FVector2D inRes);

	//打开系统相册.
	void GetUploadPicture();

	//选择图片后的回调.
	void CBOnOpenPicture(bool isSuccess, FString inIconFilePath, FString inFilePath, FVector2D inDPI, FVector2D inRes);

protected:
	UButton* m_BtnPhoto;	//	拍照按钮.
	UButton* m_BtnPicture;	//	从相册中选择按钮.
	UButton* m_BtnCancel;	//	取消按钮.

	UTextBlock* m_TextPhoto;	//	拍照字段.
	UTextBlock* m_TextPicture;	//	从相册中选择字段.
	UTextBlock* m_TextCancel;	//	取消字段.

};

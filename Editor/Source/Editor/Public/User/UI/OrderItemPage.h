// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LoginModule.h"
#include "System/UI/BaseUI.h"
#include "DownloadImageTool.h"
#include "OrderItemPage.generated.h"

/**
 * 
 */
struct OrderItemData
{
public:
	OrderItemData() {}
	
	FString m_ItemId;

	FString m_Type;

	FString m_ImageUrl;

	FString m_ButtonText;

	FString m_Extra;
};

UCLASS()
class EDITOR_API UOrderItemPage : public UBaseUI
{
	GENERATED_BODY()

private:
	LoginModule * m_LoginModule;

public:
	UOrderItemPage(const FObjectInitializer& ObjectInitializer);

	virtual bool OnStart() override;

	virtual void OnInit();

	virtual void OnRelease();

	virtual void OnSetParams(int32 _pid, FString & _content, ...);

	virtual void OnTick(float InDeltaTimeSecond);

	void SetImageUrl(FString _ImageUrl);

	void SetButtonText(FString _ShowText);

	void SetExtra(FString _Extra);

	void DownLoadIconImage();

	void OnDownoloadFinished(bool _isScuccess, int32 _penetrateId, FString _url, UTexture2D * _texture2d, UDownloadImageTool * _downloadTool);

	//	下单生产点击事件
	void OrderItem(int32 _pid);
	
protected:
	UButton* m_BtnOrder;		//	下单生产按钮.

	UTextBlock* m_TextOrder;	//	下单生产字段.
	
	UImage* m_ImagePiece;		//	订单图片.

	FString m_ImageUrl;			//	图片路径.

	FString m_ShowText;			//	下单字段.

	FString m_OrderUrl;			//	图片后缀.

	UDownloadImageTool * m_DownloadImageTool;	//	图片下载工具.
};

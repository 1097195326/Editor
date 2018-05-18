// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DesignModule.h"
#include "OrderItemPage.h"
#include "System/UI/BaseUI.h"
#include "HaoshaSDKBPLibrary.h"
#include "SharedPage.generated.h"

#if PLATFORM_WINDOWS
#define ORDER_ITEM_CONTENT_RIGHT_PADDING 5.0f
#else
#define ORDER_ITEM_CONTENT_RIGHT_PADDING 20.0f
#endif
/**
 * 
 */
UCLASS()
class EDITOR_API USharedPage : public UBaseUI
{
	GENERATED_BODY()
	
private:

	TArray<UOrderItemPage *> m_OrderItemContent;

public:
	USharedPage(const FObjectInitializer& ObjectInitializer);

	virtual bool OnStart() override;

	virtual void OnInit();

	virtual void OnRelease();

	virtual void OnSetParams(int32 _pid, FString & _content, ...);

	virtual void OnTick(float InDeltaTimeSecond);

	//	返回设计界面.
	void SharedPage(int32 _pid);

	//	收到服务器发回本地的分享给微信好友成功的消息.
	void OnShareFriendResponseMsg(msg_ptr _msg);

	//	收到服务器保存款下单列表的消息.
	void OnGetOrderListResponseMsg(msg_ptr _msg);

	//	打开微信.
	void OpenWechatPage(FString _title, FString _description, FString _iconurl, FString _linkurl);

	//	打开朋友圈.
	void OpenCirclePage(FString _title, FString _description, FString _iconurl, FString _linkurl);

	//	创建订单UI.
	void CreateOrderItem(int32 _num);

	//	异步下载图片.
	void DownLoadIconImage();

	void OnDownoloadFinished(bool _isScuccess, int32 _penetrateId, FString _url, UTexture2D * _texture2d, UDownloadImageTool * _downloadTool);


protected:
	UButton* m_BtnReturn;		//	返回设计界面.
	UButton* m_BtnPage;			//	打开首页.
	UButton* m_BtnDesign;		//	继续设计.
	UButton* m_BtnFriend;		//	分享给好友.
	UButton* m_BtnCircle;		//	分享到朋友圈.

	UTextBlock* m_TextShare;	//	分享.
	UTextBlock* m_TextFriend;	//	分享给好友.
	UTextBlock* m_TextCircle;	//	分享给朋友圈.
	UTextBlock* m_TextOrder;	//	下单.

	UImage * m_ImageWork;		//	作品image.

	FString m_LinkUrl;
	FString m_Description;
	FString m_IconUrl;
	FString m_Title;

	UGridPanel* m_GridPanelOrder;	//	下单列表.

	UOrderItemPage * m_OrderItem;	//	下单项UI.

	UDownloadImageTool * m_DownloadImageTool;	//	图片下载工具.
};

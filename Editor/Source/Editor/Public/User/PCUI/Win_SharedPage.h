// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DesignModule.h"
#include "Win_OrderItemPage.h"
#include "System/UI/BaseUI.h"
#include "HaoshaSDKBPLibrary.h"
#include "SharedPage.h"
#include "Win_SharedPage.generated.h"

/**
 * 
 */
UCLASS()
class EDITOR_API UWin_SharedPage : public UBaseUI
{
	GENERATED_BODY()
	
private:
	TArray<UWin_OrderItemPage *> m_OrderItemContent;

	TArray<UWin_OrderItemPage *> m_PieceItemContent;

public:
	TArray<FString> m_OrderUrlArray;

public:
	UWin_SharedPage(const FObjectInitializer& ObjectInitializer);

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
	void CreateOrderItem(int32 _num,bool _canBuy);

	//	下单生产按钮事件.
	void OnWorkProduce(int32 _pid);

	//	立即购买按钮事件.
	void OnBuyPiece(int32 _pid);

	//	异步下载图片.
	void DownLoadIconImage();
	void DownLoadIconImage1(FString _url);

	void OnDownoloadFinished(bool _isScuccess, int32 _penetrateId, FString _url, UTexture2D * _texture2d, UDownloadImageTool * _downloadTool);
	void OnDownoloadFinished1(bool _isScuccess, int32 _penetrateId, FString _url, UTexture2D * _texture2d, UDownloadImageTool * _downloadTool);

protected:
	UButton* m_BtnReturn;		//	返回设计界面.
	UButton* m_BtnPage;			//	打开首页.
	UButton* m_BtnDesign;		//	继续设计.
	UButton* m_BtnFriend;		//	分享给好友.
	UButton* m_BtnCircle;		//	分享到朋友圈.
	UButton* m_BtnProduce;		//	下单生产.
	UButton* m_BtnBuyPiece;		//	立即购买.

	UTextBlock* m_TextShare;	//	分享.
	UTextBlock* m_TextFriend;	//	分享给好友.
	UTextBlock* m_TextCircle;	//	分享给朋友圈.
	UTextBlock* m_TextOrder;	//	下单.

	UImage * m_ImageWork;		//	作品image.

	//	分享信息.
	FString m_LinkUrl;
	FString m_Description;
	FString m_IconUrl;
	FString m_Title;

	//	购买信息.
	FString m_BuyWorkUrl;
	FString m_BuyPieceUrl;

	UBorder * m_BorderWork;
	UBorder * m_BorderPiece;

	UGridPanel* m_GridPanelOrder;	//	下单列表.
	UGridPanel* m_GridPanelPiece;	//	下单列表.

	UWin_OrderItemPage * m_OrderItem;	//	下单项UI.

	UDownloadImageTool * m_DownloadImageTool;	//	图片下载工具.

	UDownloadImageTool * m_DownloadImageTool1;	//	二维码下载工具.
};

#pragma once
#include "CoreMinimal.h"
#include "UserData.h"
#include "MsgCenter.h"
#include "SuitPartT.h"
#include "SuitSliceT.h"
#include "LoginModule.h"
#include "SuitStitchT.h"
#include "SuitPatternT.h"
#include "OrderItemPage.h"
#include "SuitComponentT.h"
#include "SuitFabricColorT.h"
#include "SuitAccessoriesT.h"
#include "Win_OrderItemPage.h"
#include "System/BaseData/BaseModule.h"

// 设计类型.1 套件开始设计 2 推荐款设计 3用户款开始设计.
enum EnumDesignType
{
	DESIGN_BY_SUIT=1,
	DESIGN_BY_SYS_STYLE,
	DESIGN_BY_USER_STYLE
};

struct OrderStruct
{
	FString m_iconUrl;

	FString m_buttonText;

	FString m_extra;

	FString m_Type;
};

struct GoodsPriceStruct
{
	double m_goodsPrice;
	double m_goodsPromotionPrice;
	FString m_title;
	FString m_unit;
};

struct GoodInfoStruct
{
	FString m_goodsName;
	FString m_goodsSn;
	FString m_goodsColor;
	FString m_goodsPanTongCode;
	FString m_imageUrl;
	bool m_isBuy = false;
	FString m_orderUrl;
	TArray<GoodsPriceStruct> m_GoodsPriceStruct;
};

/**
* 设计场景需要的结构体.
*/
class DesignData
{

public:

	int32 m_designRID = 0;

	int32 m_designTID = 0;

	EnumDesignType m_DesignType = EnumDesignType::DESIGN_BY_SYS_STYLE;

public:

	void ClearData();
};

struct UploadInfoStruct
{
	FString m_IconOssUrl;
	FString m_PatternOssUrl;
	FString m_IconNativeUrl;
	FString m_TextureNativeUrl;
	float m_Width;
	float m_Height;
	UploadInfoStruct(FString _IconOssUrl,FString _PatternOssUrl,float _Width,float _Height,FString _iconnativeURl,FString _TextureNativeUrl) {
		m_IconOssUrl = _IconOssUrl;
		m_PatternOssUrl = _PatternOssUrl;
		m_IconNativeUrl = _iconnativeURl;
		m_TextureNativeUrl = _TextureNativeUrl;
		m_Width = _Width;
		m_Height = _Height;
	};
	UploadInfoStruct() {};

};
/**
*.
*/
class EDITOR_API DesignModule : public BaseModule
{

private:
	//	LoginModule.
	LoginModule * m_LoginModule;

	DesignData * m_designData;

public:

	// 临时，广播登录成功的消息.
	int32	m_ShareFriendSuccess = 2002;		//	分享给微信好友.
		
	int32	m_OrderItemSuccess = 2004;			//	订单页.

	int32   m_Http_UploadPattern_Pid = 2006;	//	上传花型Pid.

	int32	m_Http_BuyInfo = 2007;				//	购买消息.

public:

	DesignModule();

	virtual ~DesignModule(); 

	virtual void OnLoad() override;

	virtual void OnRelease() override;

public:
	//	请求订单信息
	void RequestShopOrder(int32 _StyleID);

	//	服务器订单信息回调.
	void OnShopOrderResponseMsg(msg_ptr _msg);

	//	请求分享消息.
	void RequestShareFriendServer(int32 _StyleID);

	//	分享消息服务器回调.
	void OnShareFriendResponseMsg(msg_ptr _msg);

	//	请求面板信息.
	void RequestBuyServer(FString _buyId, FString _tab);

	//	请求面板服务器回调.
	void OnBuyInfoResponseMsg(msg_ptr _msg);

	//	请求上传花型Json.
	void ReqUploadPatternJson(FString ossIconUrl, FString nativeIconUrl, float ossHeight, float ossWidth);

	//	上传花型Json回调.
	void OnReqUploadPatternJson(msg_ptr _msg);

	//	请求上传花型到OSS服务器.
	void UploadPatternToOss(FString ossIconUrl, FString nativeIconUrl, FString ossPatternUrl, FString nativePatternUrl,float width,float height);

	//	上传花型到OSS服务器回调.
	void OnUploadPatternToOssSuccess(bool res = true);

	//	请求上传头像到服务器
	void ReqUploadHeadJson(FString ossIconUrl, FString nativeIconUrl, float ossHeight, float ossWidth);

	//	上传头像服务器回调.
	void OnUploadHeadSuccess(msg_ptr _msg);

	//	请求上传头像到OSS服务器.
	void UploadPictureToOss(FString ossIconUrl, FString nativeIconUrl, FString ossPatternUrl, FString nativePatternUrl, float width, float height);

	//	上传头像到OSS服务器回调.
	void OnUploadPictureToOssSuccess(bool res = true);
	
	void OnUploadUserPatternToOssProgress(FString _filename, int32 _currentsize, int32 _totalsize);

	void OnUploadUserPictureToOssProgress(FString _filename, int32 _currentsize, int32 _totalsize);

	// 设置要设计的款&套件.
	void SetDesignData(int32 _designRID, int32 _desginTID, EnumDesignType _designType);

	//获取本地路径.
	FString GetOssDateUrl(FString _inNativeFullUrl);

	FString GetOssDateUrl2(FString _inNativeFullUrl);

	FString GetDateUrl(TCHAR * _inFilePath);

	//	获取上传头像路径.
	FString GetHeadOssDateUrl(FString _inNativeFullUrl);

	FString GetHeadOssDateUrl2(FString _inNativeFullUrl);

	FString GetHeadDateUrl(TCHAR * _inFilePath);

	DesignData * GetDesignData();

	// 获取当前设计的部位数组.
	TArray<SuitPartT*> GetSuitPartArray();
	TArray<SuitComponentT*> GetSuitComponentArray(int32 _SuitPartTid);
	TArray<SuitSliceT*> GetSuitSliceArray(int32 _SuitComponentTid);
	TArray<SuitAccessoriesT*> GetSuitAccessoriesArray(int32 _SuitComponentTid);
	TArray<SuitStitchT*> GetSuitStitchArray(int32 _SuitComponentTid);

public:
	bool m_CanServer = false;

	bool m_UploadPattern = true;

	int32 m_SelectOrderIndex = 0;

	UploadInfoStruct m_UploadInfoStruct;

	TArray<UWin_OrderItemPage *> m_OrderItemContent;	//订单信息.
};

//
//  LLShareSDKTool.h
//  LLShareSDKTool
//
//  Created by 李龙 on 16/4/7.
//  Copyright © 2016年 李龙. All rights reserved.
//
#pragma once

#if PLATFORM_IOS


#import <Foundation/Foundation.h>
#import <ShareSDK/ShareSDK.h>

//分享功能
static NSString *const ShareAppKey = @"2314290489057";

//微信开放平台,申请地址:https://open.weixin.qq.com/
static NSString *const WXShareAppID = @"wx4d6039ecd93593d0";
static NSString *const WXShareAppSecret = @"9b3c71aef8b56e10187fdafe860d3d34";

//static NSString *const WXShareAppID = @"wx63383dfb47aa5873";
//static NSString *const WXShareAppSecret = @"e3e9f51d5d35878505787a2b56af4efe";



//新浪分享:申请地址http://open.weibo.com/apps/192207424/info/basic
static NSString *const SinaShareAppKey = @"192207424";
static NSString *const SinaShareAppSecret = @"1ea33dd254dbcb2579cffc670a1336c5";
static NSString *const SinaOAuthWebAddress = @"http://www.lilongcnc.cc";

/**
 *  分享的内容类型
 */
typedef NS_ENUM(NSUInteger, HMShareContentType){
    /**
     *  自动适配类型，视传入的参数来决定
     */
    HMShareContentTypeAuto         = 0,

    /**
     *  文本
     */
    HMShareContentTypeText         = 1,

    /**
     *  图片
     */
    HMShareContentTypeImage        = 2,

    /**
     *  网页
     */
    HMShareContentTypeWebPage      = 3,

    /**
     *  应用
     */
    HMShareContentTypeApp          = 4,

    /**
     *  音频
     */
    HMShareContentTypeAudio        = 5,

    /**
     *  视频
     */
    HMShareContentTypeVideo        = 6,

    /**
     *  文件类型(暂时仅微信可用)
     */
    HMShareContentTypeFile         = 7

};

/**
 *  平台类型
 */
typedef NS_ENUM(NSUInteger, HMSSDKPlatformType){
    /**
     *  未知
     */
    HMSSDKPlatformTypeUnknown             = 0,
    /**
     *  新浪微博
     */
    HMSSDKPlatformTypeSinaWeibo           = 1,
    /**
     *  腾讯微博
     */
    HMSSDKPlatformTypeTencentWeibo        = 2,
    /**
     *  豆瓣
     */
    HMSSDKPlatformTypeDouBan              = 5,
    /**
     *  QQ空间
     */
    HMSSDKPlatformSubTypeQZone            = 6,
    /**
     *  人人网
     */
    HMSSDKPlatformTypeRenren              = 7,
    /**
     *  开心网
     */
    HMSSDKPlatformTypeKaixin              = 8,
    /**
     *  Facebook
     */
    HMSSDKPlatformTypeFacebook            = 10,
    /**
     *  Twitter
     */
    HMSSDKPlatformTypeTwitter             = 11,
    /**
     *  印象笔记
     */
    HMSSDKPlatformTypeYinXiang            = 12,
    /**
     *  Google+
     */
    HMSSDKPlatformTypeGooglePlus          = 14,
    /**
     *  Instagram
     */
    HMSSDKPlatformTypeInstagram           = 15,
    /**
     *  LinkedIn
     */
    HMSSDKPlatformTypeLinkedIn            = 16,
    /**
     *  Tumblr
     */
    HMSSDKPlatformTypeTumblr              = 17,
    /**
     *  邮件
     */
    HMSSDKPlatformTypeMail                = 18,
    /**
     *  短信
     */
    HMSSDKPlatformTypeSMS                 = 19,
    /**
     *  打印
     */
    HMSSDKPlatformTypePrint               = 20,
    /**
     *  拷贝
     */
    HMSSDKPlatformTypeCopy                = 21,
    /**
     *  微信好友
     */
    HMSSDKPlatformSubTypeWechatSession    = 22,
    /**
     *  微信朋友圈
     */
    HMSSDKPlatformSubTypeWechatTimeline   = 23,
    /**
     *  QQ好友
     */
    HMSSDKPlatformSubTypeQQFriend         = 24,
    /**
     *  Instapaper
     */
    HMSSDKPlatformTypeInstapaper          = 25,
    /**
     *  Pocket
     */
    HMSSDKPlatformTypePocket              = 26,
    /**
     *  有道云笔记
     */
    HMSSDKPlatformTypeYouDaoNote          = 27,
    /**
     *  Pinterest
     */
    HMSSDKPlatformTypePinterest           = 30,
    /**
     *  Flickr
     */
    HMSSDKPlatformTypeFlickr              = 34,
    /**
     *  Dropbox
     */
    HMSSDKPlatformTypeDropbox             = 35,
    /**
     *  VKontakte
     */
    HMSSDKPlatformTypeVKontakte           = 36,
    /**
     *  微信收藏
     */
    HMSSDKPlatformSubTypeWechatFav        = 37,
    /**
     *  易信好友
     */
    HMSSDKPlatformSubTypeYiXinSession     = 38,
    /**
     *  易信朋友圈
     */
    HMSSDKPlatformSubTypeYiXinTimeline    = 39,
    /**
     *  易信收藏
     */
    HMSSDKPlatformSubTypeYiXinFav         = 40,
    /**
     *  明道
     */
    HMSSDKPlatformTypeMingDao             = 41,
    /**
     *  Line
     */
    HMSSDKPlatformTypeLine                = 42,
    /**
     *  WhatsApp
     */
    HMSSDKPlatformTypeWhatsApp            = 43,
    /**
     *  KaKao Talk
     */
    HMSSDKPlatformSubTypeKakaoTalk        = 44,
    /**
     *  KaKao Story
     */
    HMSSDKPlatformSubTypeKakaoStory       = 45,
    /**
     *  Facebook Messenger
     */
    HMSSDKPlatformTypeFacebookMessenger   = 46,
    /**
     *  支付宝好友
     */
    HMSSDKPlatformTypeAliPaySocial        = 50,
    /**
     *  易信
     */
    HMSSDKPlatformTypeYiXin               = 994,
    /**
     *  KaKao
     */
    HMSSDKPlatformTypeKakao               = 995,
    /**
     *  印象笔记国际版
     */
    HMSSDKPlatformTypeEvernote            = 996,
    /**
     *  微信平台,
     */
    HMSSDKPlatformTypeWechat              = 997,
    /**
     *  QQ平台
     */
    HMSSDKPlatformTypeQQ                  = 998,
    /**
     *  任意平台
     */
    HMSSDKPlatformTypeAny                 = 999
};



@interface LLShareSDKTool : NSObject

/**
 初始化管理类

 @return 返回管理类对象
 */
+ (instancetype)defaultManager;

+(void)registerShare;
/**
 分享

 @param shareContentType         content类型
 @param contentTitle             分享标题
 @param contentDescription       分享描述
 @param contentImage             分享缩略图
 @param contentURL               分享URL
 @param showInView               列表承载页面
 @param success                  分享成功callback
 @param failure                  分享失败callback
 @param otherResponseStatus      其他原因callback
 */
- (void)shareContentWithShareContentType:(HMShareContentType)shareContentType
            contentTitle:(NSString *)contentTitle
          contentDescription:(NSString *)contentDescription
          contentImage:(id)contentImage
                  contentURL:(NSString *)contentURL
                  showInView:(UIView *)showInView
                     success:(void (^)())success
                     failure:(void (^)(NSString *failureInfo))failure
         OtherResponseStatus:(void (^)(SSDKResponseState state))otherResponseStatus;



/**
 指定平台分享

 @param shareContentType            分享内容格式
 @param sharePlatformType           分享平台
 @param contentTitle                分享标题
 @param contentDescription          分享描述
 @param contentImage                分享缩略图
 @param contentURL                  分享URL
 @param success                     分享成功callBack
 @param failure                     分享失败callBack
 @param otherResponseStatus         其他原因callBack
 */
- (void)shareContentWithShareContentType:(HMShareContentType)shareContentType
                       SharePlatformType:(SSDKPlatformType)sharePlatformType
                            contentTitle:(NSString *)contentTitle
                      contentDescription:(NSString *)contentDescription
                            contentImage:(id)contentImage
                              contentURL:(NSString *)contentURL
                                 success:(void (^)())success
                                 failure:(void (^)(NSString *))failure
                     OtherResponseStatus:(void (^)(SSDKResponseState))otherResponseStatus;


//- (BOOL)application:(UIApplication *)app openURL:(NSURL *)url options:(NSDictionary<UIApplicationOpenURLOptionsKey,id> *)options;
//
//- (void) onResp:(BaseResp*)resp;


@end


#endif

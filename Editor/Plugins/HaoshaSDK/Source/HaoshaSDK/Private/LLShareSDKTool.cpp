//
//  LLShareSDKTool.m
//  LLShareSDKTool
//
//  Created by 李龙 on 16/4/7.
//  Copyright © 2016年 李龙. All rights reserved.
//



#include "LLShareSDKTool.h"

#if PLATFORM_IOS
//
#import <ShareSDK/ShareSDK.h>
#import <ShareSDKConnector/ShareSDKConnector.h>
#import <ShareSDKUI/ShareSDKUI.h>

//微信SDK头文件
#import "WXApi.h"

//新浪微博SDK头文件
//新浪微博SDK需要在项目Build Settings中的Other Linker Flags添加"-ObjC"
//#import "WeiboSDK.h"

@implementation LLShareSDKTool

+ (instancetype)defaultManager
{
    static LLShareSDKTool *manager;
    
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        manager = [LLShareSDKTool new];
        
        
    });
    
    return manager;
}

- (instancetype)init
{
    self = [super init];
    if (self)
    {
        
    }
    return self;
}


+ (void)initialize
{
//    [LLShareSDKTool registerShare];
}

#pragma mark - ShareSDK 注册
+(void)registerShare
{
//    NSLog(@"%s",__FUNCTION__);
    
    //registerApp 初始化SDK并且初始化第三方平台
    
    [WXApi registerApp:@"wx4d6039ecd93593d0"];
    
    [ShareSDK registerActivePlatforms:@[@(SSDKPlatformSubTypeWechatSession),
                            @(SSDKPlatformSubTypeWechatTimeline)]
                 onImport:^(SSDKPlatformType platformType) {
                     switch (platformType)
                     {
                         case SSDKPlatformTypeWechat:{
                             [ShareSDKConnector connectWeChat:[WXApi class]];
                             break;
                         }
                         default:
                             break;
                     }

                 } onConfiguration:^(SSDKPlatformType platformType, NSMutableDictionary *appInfo) {
                     switch (platformType)
                     {
                         case SSDKPlatformTypeWechat:
                             [appInfo SSDKSetupWeChatByAppId:WXShareAppID appSecret:WXShareAppSecret];
                             break;
                         default:
                             break;
                     }
                 }
     ];
}


#pragma mark- 分享代码
- (void)shareContentWithShareContentType:(HMShareContentType)shareContentType
                            contentTitle:(NSString *)contentTitle
                      contentDescription:(NSString *)contentDescription
                            contentImage:(id)contentImage
                              contentURL:(NSString *)contentURL
                              showInView:(UIView *)showInView
                                 success:(void (^)())success
                                 failure:(void (^)(NSString *))failure
                     OtherResponseStatus:(void (^)(SSDKResponseState))otherResponseStatus
{
    //0.区分分享类型
    SSDKContentType type;
    switch (shareContentType) {
        case HMShareContentTypeAuto: // 自动适配类型，视传入的参数来决定
            type = SSDKContentTypeAuto;
            break;
        case HMShareContentTypeText: //文本
            type = SSDKContentTypeText;
            break;
            
        case HMShareContentTypeImage://图片
            type = SSDKContentTypeImage;
            break;
            
        case HMShareContentTypeWebPage://网页
            type = SSDKContentTypeWebPage;
            break;
            
        case HMShareContentTypeApp: //应用
            type = SSDKContentTypeApp;
            break;
            
        case HMShareContentTypeAudio://音频
            type = SSDKContentTypeAudio;
            break;
            
        case HMShareContentTypeVideo://视频
            type = SSDKContentTypeVideo;
            break;
        case HMShareContentTypeFile://文件类型(暂时仅微信可用)
            type = SSDKContentTypeFile;
            break;
            
        default:
            break;
    }
    
    //1. 创建分享参数
    NSMutableDictionary *shareParams = [NSMutableDictionary dictionary];
    [shareParams SSDKEnableUseClientShare]; //这个参数控制是"应用内分享(网页分享)" 还是客户端分享. 不加的话是'应用内分享(网页分享)'
    
    [shareParams SSDKSetupShareParamsByText:contentDescription
                                     images:contentImage
                                        url:[NSURL URLWithString:contentURL]
                                      title:contentTitle
                                       type:type];
    
    //2. 分享,显示分享view
    SSUIShareActionSheetController *sheet =[ShareSDK showShareActionSheet:nil
                             items:nil
                       shareParams:shareParams
               onShareStateChanged:^(SSDKResponseState state, SSDKPlatformType platformType, NSDictionary *userData, SSDKContentEntity *contentEntity, NSError *error, BOOL end) {
                   
                   NSLog(@"SSDKResponseState state:%lu",(unsigned long)state);
                   switch (state) {
                           
                       case SSDKResponseStateBegin:
                       {
                           //这里不填写
                           break;
                       }
                       case SSDKResponseStateSuccess:
                       {
                           NSLog(@"0");
                           success();
                           break;
                           
                       }
                       case SSDKResponseStateFail:
                       {
                           
                           if (platformType == SSDKPlatformTypeSMS && [error code] == 201)
                           {
                               
                               NSLog(@"1");
                               
                               failure(@"失败原因可能是：1、短信应用没有设置帐号；2、设备不支持短信应用；3、短信应用在iOS 7以上才能发送带附件的短                                                     信。");
                               break;
                               
                           }
                           else if(platformType == SSDKPlatformTypeMail && [error code] == 201)
                           {
                             
                               NSLog(@"2");
                               failure(@"失败原因可能是：1、邮件应用没有设置帐号；2、设备不支持邮件应用；");
                               break;
                               
                           }
                           else
                           {
                               
                               NSLog(@"3");
                               failure([NSString stringWithFormat:@"%@",error]);
                               break;
                               
                           }
                           break;
                       }
                       case SSDKResponseStateCancel:
                       {
                           
                           NSLog(@"4");
//                           otherResponseStatus(SSDKResponseStateCancel); //捕获不准确
                           break;
                           
                       }
                       default:
                           break;
                   }
                   
                   if (state != SSDKResponseStateBegin)
                   {
                       NSLog(@"5");
//                         failure([NSString stringWithFormat:@"%@",error]);
                       
                   }
                   
               }];
    
    
       [sheet.directSharePlatforms addObject:@(SSDKPlatformTypeSinaWeibo)]; //这句话是取消掉 sahreSDK自带的 分享内容编辑界面
}


- (void)shareContentWithShareContentType:(HMShareContentType)shareContentType
                       SharePlatformType:(SSDKPlatformType)sharePlatformType
                            contentTitle:(NSString *)contentTitle
                      contentDescription:(NSString *)contentDescription
                            contentImage:(id)contentImage
                              contentURL:(NSString *)contentURL
                                 success:(void (^)())success
                                 failure:(void (^)(NSString *))failure
                     OtherResponseStatus:(void (^)(SSDKResponseState))otherResponseStatus
{
    //0.区分分享类型
    SSDKContentType type;
//    SSDKContentType type =SSDKContentTypeAuto;
    switch (shareContentType) {
        case HMShareContentTypeAuto: // 自动适配类型，视传入的参数来决定
            type = SSDKContentTypeAuto;
            break;
        case HMShareContentTypeText: //文本
            type = SSDKContentTypeText;
            break;
            
        case HMShareContentTypeImage://图片
            type = SSDKContentTypeImage;
            break;
            
        case HMShareContentTypeWebPage://网页
            type = SSDKContentTypeWebPage;
            break;
            
        case HMShareContentTypeApp: //应用
            type = SSDKContentTypeApp;
            break;
            
        case HMShareContentTypeAudio://音频
            type = SSDKContentTypeAudio;
            break;
            
        case HMShareContentTypeVideo://视频
            type = SSDKContentTypeVideo;
            break;
        case HMShareContentTypeFile://文件类型(暂时仅微信可用)
            type = SSDKContentTypeFile;
            break;
            
        default:
            break;
    }
    
    //1. 创建分享参数
    NSMutableDictionary *shareParams = [NSMutableDictionary dictionary];
    [shareParams SSDKEnableUseClientShare]; //这个参数控制是"应用内分享(网页分享)" 还是客户端分享. 不加的话是'应用内分享(网页分享)'
    
    [shareParams SSDKSetupShareParamsByText:contentDescription
                                     images:contentImage
                                        url:[NSURL URLWithString:contentURL]
                                      title:contentTitle
                                       type:type];
    //进行分享
    [ShareSDK share:sharePlatformType
         parameters:shareParams
     onStateChanged:^(SSDKResponseState state, NSDictionary *userData, SSDKContentEntity *contentEntity, NSError *error) {
         //         KDLOG(@"%@", error);
         
         //         self.superVC = nil;
         
         switch (state) {
             case SSDKResponseStateSuccess:
             {
                 
                 //                 NSLog(@"msgParam==%@",self.messageParam);
                 //                [KDAlertView showMessage:@"分享成功！" title:@"" cancelBtnTitle:@"确定" buttonBlock:^(NSInteger buttonIndex) {
                 //
                 //                 }];
                 //                 [MBProgressHUD showSuccess:@"分享成功！"];
                 
                 break;
             }
             case SSDKResponseStateFail:
             {
                 
                 //                 [KDAlertView showMessage:@"分享失败！" title:@"" cancelBtnTitle:@"确定" buttonBlock:^(NSInteger buttonIndex) {
                 //
                 //                 }];
                 //                 [MBProgressHUD showSuccess:@"分享失败！"];
                 
                 break;
             }
             case SSDKResponseStateCancel:
             {
                 //分享成功，选择留在QQ时，回来这里，所以去掉他
                 //                 [KDAlertView showMessage:@"分享取消！" title:@"" cancelBtnTitle:@"确定" buttonBlock:^(NSInteger buttonIndex) {
                 //
                 //                 }];
                 //                 [MBProgressHUD showSuccess:@"分享取消！"];
                 
                 break;
             }
             default:
                 break;
         }
     }];

    
    
}

//- (BOOL)application:(UIApplication *)app openURL:(NSURL *)url options:(NSDictionary<UIApplicationOpenURLOptionsKey,id> *)options
//{
//    return [WXApi handleOpenURL:url delegate:self];
//}
//
//- (void) onResp:(BaseResp*)resp
//{
//    NSLog(@"Appdelegate callback  ----->   %@",resp);
//}


@end

#endif

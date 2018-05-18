// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#if PLATFORM_IOS


#import <Foundation/Foundation.h>

#import <UIKit/UIKit.h>

@interface HMWebViewManager : NSObject
/**
 初始化Manager管理类
 
 @return 返回管理类对象
 */
+ (instancetype)defaultManager;


/**
 添加自定义协议处理方式
 
 @param protocolName            协议名称
 @param actionName              需要调用的方法名
 */
- (void)addCustomProtocol:(NSString *)protocolName actionName:(NSString *)actionName;


/**
 调用WebView方法
 
 @param viewController              当前控制器ViewController
 @param urlString                   URL
 @param imageName                   返回图片名称
 @param cancelButtonImageName       取消图片名称
 @param callback                    为做处理的协议进行统一的处理
 protocolString为协议内容
 */
- (void)webViewInitWithcontroller:(UIViewController *)viewController urlString:(NSString *)urlString prama:(NSString *)pramaString   backButtonImage:(NSString *)imageName cancelButtonImage:(NSString *)cancelButtonImageName protocolCallBack:(void(^)(NSString *protocolString))callback;

@end


#endif



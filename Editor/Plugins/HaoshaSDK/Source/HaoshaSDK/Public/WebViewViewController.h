// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#if PLATFORM_IOS

#import <UIKit/UIKit.h>
#define Scheme @"webView"

typedef void(^WebViewCallBack)(NSString *protocolString);

@interface WebViewViewController : UIViewController

@property (copy ,nonatomic) NSString *urlString;

@property (copy, nonatomic) NSString *imageName;

@property (copy, nonatomic) NSString *cancelImageName;

@property (copy, nonatomic) WebViewCallBack callBack;

@property (copy, nonatomic) NSString *pramaString;

-(void)payForAli;

@end

#endif

// Fill out your copyright notice in the Description page of Project Settings.

#include "HMWebViewManager.h"

// Fill out your copyright notice in the Description page of Project Settings.

#if PLATFORM_IOS

#import "WebViewViewController.h"

@interface HMWebViewManager()

//@property (strong, nonatomic) WebViewViewController *webViewViewController;

@end

@implementation HMWebViewManager

+ (instancetype)defaultManager
{
    static HMWebViewManager *manager;
    
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        manager = [HMWebViewManager new];
        
        if (![[NSUserDefaults standardUserDefaults] valueForKey:@"Protocol"])
        {
            NSMutableDictionary *dic = @{@"cancel":@"cancelAction"}.mutableCopy;
            
            [[NSUserDefaults standardUserDefaults] setObject:dic forKey:@"Protocol"];
        }
    });
    
    return manager;
}

- (instancetype)init
{
    self = [super init];
    if (self) {
        
    }
    return self;
}

- (void)addCustomProtocol:(NSString *)protocolName actionName:(NSString *)actionName
{
    if ([[NSUserDefaults standardUserDefaults] valueForKey:@"CustomProtocol"])
    {
        NSMutableDictionary *customDic = [[NSUserDefaults standardUserDefaults] valueForKey:@"CustomProtocol"];
        
        [customDic setObject:protocolName forKey:actionName];
        
        [[NSUserDefaults standardUserDefaults] setObject:customDic forKey:@"CustomProtocol"];
        
    }
    else
    {
        NSMutableDictionary *dic = [NSMutableDictionary dictionary];
        
        [dic setObject:protocolName forKey:actionName];
        
        [[NSUserDefaults standardUserDefaults] setObject:dic forKey:@"CustomProtocol"];
        
    }
}


- (void)webViewInitWithcontroller:(UIViewController *)viewController urlString:(NSString *)urlString prama:(NSString *)pramaString backButtonImage:(NSString *)imageName cancelButtonImage:(NSString *)cancelButtonImageName protocolCallBack:(void(^)(NSString *protocolString))callback
{
    WebViewViewController * webViewViewController = [WebViewViewController new];
    
    webViewViewController.urlString = urlString;
    
    webViewViewController.imageName = imageName;
    
    webViewViewController.cancelImageName = cancelButtonImageName;
    
    webViewViewController.pramaString = pramaString;
    
    webViewViewController.callBack = ^(NSString *protocolString)
    {
        callback(protocolString);
    };
    
    [viewController presentModalViewController:webViewViewController animated:YES];
    
}


@end



#endif



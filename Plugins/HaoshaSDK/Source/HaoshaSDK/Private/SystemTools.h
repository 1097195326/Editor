//
//  SystemTools.h
//  FAFA
//
//  Created by 门皓 on 2017/8/30.
//  Copyright © 2017年 门皓. All rights reserved.
//
#pragma once

#if PLATFORM_IOS


#import <Foundation/Foundation.h>

@interface SystemTools : NSObject

+ (NSString *)getCPUType;

+(NSString *)getTotalMemorySize;

+(NSString *)getAvailableMemorySize;

+(NSString *)fileSizeToString:(unsigned long long)fileSize;

+ (NSString*)deviceVersion;

+ (NSString *)iOSVersion;

+ (void)setUserID:(NSString *)userID setApiVersion:(NSString *)Api setAppVersion:(NSString *)AppVersion;

+ (NSString *)getErrorLog;

+ (void)cleanErrorLog;

@end
#endif

//
//  UncaughtExceptionHandler.m
//  UncaughtExceptions
//
//  Created by Matt Gallagher on 2010/05/25.
//  Copyright 2010 Matt Gallagher. All rights reserved.
//
//  Permission is given to use this source code file, free of charge, in any
//  project, commercial or otherwise, entirely at your risk, with the condition
//  that any redistribution (in part or whole) of source code must retain
//  this copyright and permission notice. Attribution in compiled projects is
//  appreciated but not required.
//

#include "UncaughtExceptionHandler.h"
#if PLATFORM_IOS
#include <libkern/OSAtomic.h>
#include <execinfo.h>
#include "SystemTools.h"

NSString * const UncaughtExceptionHandlerSignalExceptionName = @"UncaughtExceptionHandlerSignalExceptionName";
NSString * const UncaughtExceptionHandlerSignalKey = @"UncaughtExceptionHandlerSignalKey";
NSString * const UncaughtExceptionHandlerAddressesKey = @"UncaughtExceptionHandlerAddressesKey";

volatile int32_t UncaughtExceptionCount = 0;
const int32_t UncaughtExceptionMaximum = 10;

const NSInteger UncaughtExceptionHandlerSkipAddressCount = 4;
const NSInteger UncaughtExceptionHandlerReportAddressCount = 5;

@implementation UncaughtExceptionHandler

+ (NSArray *)backtrace
{
    void* callstack[128];
    int frames = backtrace(callstack, 128);
    char **strs = backtrace_symbols(callstack, frames);
    
    int i;
    NSMutableArray *backtrace = [NSMutableArray arrayWithCapacity:frames];
    for (
         i = UncaughtExceptionHandlerSkipAddressCount;
         i < UncaughtExceptionHandlerSkipAddressCount +
         UncaughtExceptionHandlerReportAddressCount;
         i++)
    {
        [backtrace addObject:[NSString stringWithUTF8String:strs[i]]];
    }
    free(strs);
    
    return backtrace;
}

- (void)alertView:(UIAlertView *)anAlertView clickedButtonAtIndex:(NSInteger)anIndex
{
    if (anIndex == 0)
    {
        dismissed = YES;
    }
}

- (void)validateAndSaveCriticalApplicationData
{
    
}

- (void)handleException:(NSException *)exception
{
    [self validateAndSaveCriticalApplicationData];
    
    //    [SystemTools networkStateChange];
    
    //获取电池状态
    //    NSLog(@"%@",[SystemTools getCurrentBatteryLevel]);
    //获取总内存大小
    NSLog(@"%@",[SystemTools getTotalMemorySize]);
    //获取当前可用内存
    NSLog(@"%@",[SystemTools getAvailableMemorySize]);
    //获取CUP
    [SystemTools getCPUType];
    
    
    NSDictionary *fattributes = [[NSFileManager defaultManager] attributesOfFileSystemForPath:NSHomeDirectory() error:nil];
    //获取存储总量
    NSLog(@"%@",[SystemTools fileSizeToString:[[fattributes objectForKey:NSFileSystemSize] integerValue]]);
    //获取剩余存储
    NSLog(@"%@",[SystemTools fileSizeToString:[[fattributes objectForKey:NSFileSystemFreeSize] integerValue]]);
    
    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
    //设定时间格式,这里可以设置成自己需要的格式
    [dateFormatter setDateFormat:@"yyyy-MM-dd HH:mm:ss"];
    
    NSString *timeSp = [dateFormatter stringFromDate: [NSDate date]];
    
    NSMutableDictionary *dic = [NSMutableDictionary dictionary];
    
    [dic setValue:@"FAFA_SOFT_APP_IOS" forKey:@"source"];
    
    [dic setValue:@"Error" forKey:@"level"];
    
    
    [dic setValue:[NSString stringWithFormat:NSLocalizedString(@"%@,%@", nil),[exception reason],[[exception userInfo] objectForKey:UncaughtExceptionHandlerAddressesKey]] forKey:@"exceptionInfo"];
    
    [dic setValue:timeSp forKey:@"occurTime"];
    
    [dic setValue:USERID forKey:@"account"];
    
    [dic setValue:@"IOS" forKey:@"os"];
    
    [dic setValue:[[UIDevice currentDevice] systemVersion] forKey:@"version"];
    
    [dic setValue:[SystemTools getCPUType] forKey:@"cpuType"];
    
    [dic setValue:[SystemTools getTotalMemorySize] forKey:@"memory"];
    
    [dic setValue:[SystemTools fileSizeToString:[[fattributes objectForKey:NSFileSystemSize] integerValue]] forKey:@"storage"];
    
    [dic setValue:[SystemTools getAvailableMemorySize] forKey:@"memoryFree"];
    
    [dic setValue:[SystemTools fileSizeToString:[[fattributes objectForKey:NSFileSystemFreeSize] integerValue]] forKey:@"storageFree"];
    
    [dic setValue:[SystemTools deviceVersion] forKey:@"model"];
    
    [dic setValue:[[[UIDevice currentDevice] identifierForVendor] UUIDString] forKey:@"deviceCode"];
    
    [dic setValue:AppVersion forKey:@"appVersion"];
    
    [dic setValue:APIVersion forKey:@"apiVersion"];
    
    [dic setValue:@"FAFA_SOFT_APP" forKey:@"appType"];
    
    
    
    //    NSDictionary *dic = [NSDictionary dictionaryWithObjects:@[@"FAFA_SHOP_APP_IOS",
    //                                                              @"Error",
    //                                                              [NSString stringWithFormat:NSLocalizedString(@"%@,%@", nil),[exception reason],[[exception userInfo] objectForKey:UncaughtExceptionHandlerAddressesKey]],
    //                                                              timeSp,
    //                                                              UserPhone,
    //                                                              @"IOS",
    //                                                              [[UIDevice currentDevice] systemVersion],
    //                                                              [SystemTools getCPUType],
    //                                                              [SystemTools getTotalMemorySize],
    //                                                              [SystemTools fileSizeToString:[[fattributes objectForKey:NSFileSystemSize] integerValue]],
    //                                                              [SystemTools getAvailableMemorySize],
    //                                                              [SystemTools fileSizeToString:[[fattributes objectForKey:NSFileSystemFreeSize] integerValue]],
    //                                                              [SystemTools deviceVersion],
    //                                                              [[[UIDevice currentDevice] identifierForVendor] UUIDString],
    //                                                              @"1.0.0",
    //                                                              @"FAFA_SHOP_APP"] forKeys:@[@"source",@"level",@"exceptionInfo",@"occurTime",@"account",@"os",@"version",@"cpuType",@"memory",@"storage",@"memoryFree",@"storageFree",@"model",@"deviceCode",@"appVersion",@"apiVersion",@"appType"]];
    ////
    
    NSLog(@"%@",dic);
    
    NSMutableArray *array = [NSMutableArray array];
    
    [array addObjectsFromArray:ErrorLog];
    
    [array addObject:dic];
    
    [[NSUserDefaults standardUserDefaults] setObject:array forKey:@"ErrorLog"];
    
    
    //	UIAlertView *alert =
    //		[[UIAlertView alloc]
    //			initWithTitle:NSLocalizedString(@"Unhandled exception", nil)
    //			message:[NSString stringWithFormat:NSLocalizedString(
    //				@"You can try to continue but the application may be unstable.\n\n"
    //				@"Debug details follow:\n%@\n%@", nil),
    //				[exception reason],
    //				[[exception userInfo] objectForKey:UncaughtExceptionHandlerAddressesKey]]
    //			delegate:self
    //			cancelButtonTitle:NSLocalizedString(@"Quit", nil)
    //			otherButtonTitles:NSLocalizedString(@"Continue", nil), nil]
    //		;
    //	[alert show];
    
    CFRunLoopRef runLoop = CFRunLoopGetCurrent();
    CFArrayRef allModes = CFRunLoopCopyAllModes(runLoop);
    
    while (!dismissed)
    {
        for (NSString *mode in (__bridge NSArray *)allModes)
        {
            CFRunLoopRunInMode((CFStringRef)mode, 0.001, false);
        }
    }
    
    CFRelease(allModes);
    
    NSSetUncaughtExceptionHandler(NULL);
    signal(SIGABRT, SIG_DFL);
    signal(SIGILL, SIG_DFL);
    signal(SIGSEGV, SIG_DFL);
    signal(SIGFPE, SIG_DFL);
    signal(SIGBUS, SIG_DFL);
    signal(SIGPIPE, SIG_DFL);
    
    if ([[exception name] isEqual:UncaughtExceptionHandlerSignalExceptionName])
    {
        kill(getpid(), [[[exception userInfo] objectForKey:UncaughtExceptionHandlerSignalKey] intValue]);
    }
    else
    {
        [exception raise];
    }
}

@end

void HandleException(NSException *exception)
{
    int32_t exceptionCount = OSAtomicIncrement32(&UncaughtExceptionCount);
    if (exceptionCount > UncaughtExceptionMaximum)
    {
        return;
    }
    
    NSArray *callStack = [UncaughtExceptionHandler backtrace];
    NSMutableDictionary *userInfo =
    [NSMutableDictionary dictionaryWithDictionary:[exception userInfo]];
    [userInfo
     setObject:callStack
     forKey:UncaughtExceptionHandlerAddressesKey];
    
    [[[UncaughtExceptionHandler alloc] init]
     performSelectorOnMainThread:@selector(handleException:)
     withObject:
     [NSException
      exceptionWithName:[exception name]
      reason:[exception reason]
      userInfo:userInfo]
     waitUntilDone:YES];
}

void SignalHandler(int signal)
{
    int32_t exceptionCount = OSAtomicIncrement32(&UncaughtExceptionCount);
    if (exceptionCount > UncaughtExceptionMaximum)
    {
        return;
    }
    NSMutableDictionary *userInfo =
    [NSMutableDictionary
     dictionaryWithObject:[NSNumber numberWithInt:signal]
     forKey:UncaughtExceptionHandlerSignalKey];
    NSArray *callStack = [UncaughtExceptionHandler backtrace];
    [userInfo
     setObject:callStack
     forKey:UncaughtExceptionHandlerAddressesKey];
    [[[UncaughtExceptionHandler alloc] init]
     performSelectorOnMainThread:@selector(handleException:)
     withObject:
     [NSException
      exceptionWithName:UncaughtExceptionHandlerSignalExceptionName
      reason:
      [NSString stringWithFormat:
       NSLocalizedString(@"Signal %d was raised.", nil),
       signal]
      userInfo:
      [NSDictionary
       dictionaryWithObject:[NSNumber numberWithInt:signal]
       forKey:UncaughtExceptionHandlerSignalKey]]
     waitUntilDone:YES];
}

void InstallUncaughtExceptionHandler(void)
{
    NSSetUncaughtExceptionHandler(&HandleException);
    signal(SIGABRT, SignalHandler);
    signal(SIGILL, SignalHandler);
    signal(SIGSEGV, SignalHandler);
    signal(SIGFPE, SignalHandler);
    signal(SIGBUS, SignalHandler);
    signal(SIGPIPE, SignalHandler);
    
}
#endif

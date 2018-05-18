//
//  SystemTools.m
//  FAFA
//
//  Created by 门皓 on 2017/8/30.
//  Copyright © 2017年 门皓. All rights reserved.
//

#include "SystemTools.h"
#if PLATFORM_IOS
#include <mach/mach_host.h>
#include <sys/socket.h>
#import <arpa/inet.h>
#import <ifaddrs.h>
#include <netdb.h>
#import <SystemConfiguration/SystemConfiguration.h>
#import "sys/utsname.h"
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "UncaughtExceptionHandler.h"

@implementation SystemTools

//+(double)getCurrentBatteryLevel
//{

//    //Returns a blob of Power Source information in an opaque CFTypeRef.
//    CFTypeRef blob = IOPSCopyPowerSourcesInfo();
//    
//    //Returns a CFArray of Power Source handles, each of type CFTypeRef.
//    CFArrayRef sources = IOPSCopyPowerSourcesList(blob);
//    
//    CFDictionaryRef pSource = NULL;
//    const void *psValue;
//    
//    //Returns the number of values currently in an array.
//    int numOfSources = CFArrayGetCount(sources);
//    
//    //Error in CFArrayGetCount
//    if (numOfSources == 0)
//    {
//        NSLog(@"Error in CFArrayGetCount");
//        return -1.0f;
//    }
//    
//    //Calculating the remaining energy
//    for (int i = 0 ; i < numOfSources ; i++)
//    {
//        //Returns a CFDictionary with readable information about the specific power source.
//        pSource = IOPSGetPowerSourceDescription(blob, CFArrayGetValueAtIndex(sources, i));
//        if (!pSource)
//        {
//            NSLog(@"Error in IOPSGetPowerSourceDescription");
//            return -1.0f;
//        }
//        psValue = (CFStringRef)CFDictionaryGetValue(pSource, CFSTR(kIOPSNameKey));
//        
//        int curCapacity = 0;
//        int maxCapacity = 0;
//        double percent;
//        
//        psValue = CFDictionaryGetValue(pSource, CFSTR(kIOPSCurrentCapacityKey));
//        CFNumberGetValue((CFNumberRef)psValue, kCFNumberSInt32Type, &curCapacity);
//        
//        psValue = CFDictionaryGetValue(pSource, CFSTR(kIOPSMaxCapacityKey));
//        CFNumberGetValue((CFNumberRef)psValue, kCFNumberSInt32Type, &maxCapacity);
//        
//        percent = ((double)curCapacity/(double)maxCapacity * 100.0f);
//        
//        return percent;
//    }
//    return -1.0f;
//}


+(NSString *)getTotalMemorySize
{
    return [self fileSizeToString:[NSProcessInfo processInfo].physicalMemory];
}

+(NSString *)getAvailableMemorySize
{
    vm_statistics_data_t vmStats;
    mach_msg_type_number_t infoCount = HOST_VM_INFO_COUNT;
    kern_return_t kernReturn = host_statistics(mach_host_self(), HOST_VM_INFO, (host_info_t)&vmStats, &infoCount);
    if (kernReturn != KERN_SUCCESS)
    {
        return [NSString stringWithFormat:@"%ld",NSNotFound];
    }
    
    return [self fileSizeToString:((vm_page_size * vmStats.free_count + vm_page_size * vmStats.inactive_count))];
}

//-(long long)getTotalDiskSize
//{
//    struct statfs buf;
//    unsigned long long freeSpace = -1;
//    if (statfs("/var", &buf) >= 0)
//    {
//        freeSpace = (unsigned long long)(buf.f_bsize * buf.f_blocks);
//    }
//    return freeSpace;
//}

//-(long long)getAvailableDiskSize
//{
//    struct statfs buf;
//    unsigned long long freeSpace = -1;
//    if (statfs("/var", &buf) >= 0)
//    {
//        freeSpace = (unsigned long long)(buf.f_bsize * buf.f_bavail);
//    }
//    return freeSpace;
//}

+(NSString *)fileSizeToString:(unsigned long long)fileSize
{
    NSInteger KB = 1024;
    NSInteger MB = KB*KB;
    NSInteger GB = MB*KB;
    
    if (fileSize < 10)
    {
        return @"0 B";
        
    }else if (fileSize < KB)
    {
        return @"< 1 KB";
        
    }else if (fileSize < MB)
    {
        return [NSString stringWithFormat:@"%.1f KB",((float)fileSize)/KB];
        
    }else if (fileSize < GB)
    {
        return [NSString stringWithFormat:@"%.1f MB",((float)fileSize)/MB];
        
    }else
    {
        return [NSString stringWithFormat:@"%.1f GB",((float)fileSize)/GB];
    }
}

+ (NSString *)getCPUType
{
    host_basic_info_data_t hostInfo;
    mach_msg_type_number_t infoCount;
    
    infoCount = HOST_BASIC_INFO_COUNT;
    host_info(mach_host_self(), HOST_BASIC_INFO, (host_info_t)&hostInfo, &infoCount);
    
    switch (hostInfo.cpu_type) {
        case CPU_TYPE_ARM:
            return @"CPU_TYPE_ARM";
            break;
            
        case CPU_TYPE_ARM64:
            return @"CPU_TYPE_ARM64";
            break;
            
        case CPU_TYPE_X86:
            return @"CPU_TYPE_X86";
            break;
            
        case CPU_TYPE_X86_64:
            return @"CPU_TYPE_X86_64";
            break;
            
        default:
            break;
    }
    
    return nil;
}

+ (NSString*)deviceVersion
{
    // 需要#import "sys/utsname.h"
    struct utsname systemInfo;
    uname(&systemInfo);
    NSString * deviceString = [NSString stringWithCString:systemInfo.machine encoding:NSUTF8StringEncoding];
    //iPhone
    if ([deviceString isEqualToString:@"iPhone1,1"])    return @"iPhone 1G";
    if ([deviceString isEqualToString:@"iPhone1,2"])    return @"iPhone 3G";
    if ([deviceString isEqualToString:@"iPhone2,1"])    return @"iPhone 3GS";
    if ([deviceString isEqualToString:@"iPhone3,1"])    return @"iPhone 4";
    if ([deviceString isEqualToString:@"iPhone3,2"])    return @"Verizon iPhone 4";
    if ([deviceString isEqualToString:@"iPhone4,1"])    return @"iPhone 4S";
    if ([deviceString isEqualToString:@"iPhone5,1"])    return @"iPhone 5";
    if ([deviceString isEqualToString:@"iPhone5,2"])    return @"iPhone 5";
    if ([deviceString isEqualToString:@"iPhone5,3"])    return @"iPhone 5C";
    if ([deviceString isEqualToString:@"iPhone5,4"])    return @"iPhone 5C";
    if ([deviceString isEqualToString:@"iPhone6,1"])    return @"iPhone 5S";
    if ([deviceString isEqualToString:@"iPhone6,2"])    return @"iPhone 5S";
    if ([deviceString isEqualToString:@"iPhone7,1"])    return @"iPhone 6 Plus";
    if ([deviceString isEqualToString:@"iPhone7,2"])    return @"iPhone 6";
    if ([deviceString isEqualToString:@"iPhone8,1"])    return @"iPhone 6s";
    if ([deviceString isEqualToString:@"iPhone8,2"])    return @"iPhone 6s Plus";
    if ([deviceString isEqualToString:@"iPhone9,1"])    return @"iPhone 7";
    if ([deviceString isEqualToString:@"iPhone9,2"])    return @"iPhone 7 Plus";
    if ([deviceString isEqualToString:@"iPhone9,3"])    return @"iPhone 7";
    if ([deviceString isEqualToString:@"iPhone9,4"])    return @"iPhone 7 Plus";
    if ([deviceString isEqualToString:@"iPhone10,1"])   return @"iPhone 8";
    if ([deviceString isEqualToString:@"iPhone10,4"])   return @"iPhone 8";
    if ([deviceString isEqualToString:@"iPhone10,2"])   return @"iPhone 8 Plus";
    if ([deviceString isEqualToString:@"iPhone10,5"])   return @"iPhone 8 Plus";
    if ([deviceString isEqualToString:@"iPhone10,3"])   return @"iPhone X";
    if ([deviceString isEqualToString:@"iPhone10,6"])   return @"iPhone X";
    
    
    return deviceString;
}

+ (NSString *)iOSVersion
{
    return [[UIDevice currentDevice] systemVersion];
}

+ (void)setUserID:(NSString *)userID setApiVersion:(NSString *)Api setAppVersion:(NSString *)AppHaosaVersion
{
    [[NSUserDefaults standardUserDefaults] setObject:userID forKey:@"UserID"];
    
    [[NSUserDefaults standardUserDefaults] setObject:Api forKey:@"APIVersion"];
    
    [[NSUserDefaults standardUserDefaults] setObject:AppHaosaVersion forKey:@"AppVersion"];
}

+ (NSString *)getErrorLog
{
    if([[NSUserDefaults standardUserDefaults] objectForKey:@"ErrorLog"] && [ErrorLog count])
    {
    NSData *jsonData = [NSJSONSerialization dataWithJSONObject:ErrorLog
                                                       options:kNilOptions
                                                         error:nil];
    
    return [[NSString alloc] initWithData:jsonData
                                 encoding:NSUTF8StringEncoding];
    }
    return @"";
}

+ (void)cleanErrorLog
{
    [[NSUserDefaults standardUserDefaults] removeObjectForKey:ErrorLog];
}


@end
#endif

// Fill out your copyright notice in the Description page of Project Settings.

#include "GCore.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "Public/System/Tools/GFileHelper.h"

FString GFileHelper::ProjectHotloadDownloadDir()
{
#if PLATFORM_WINDOWS
    static FString persistentDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir(), TEXT("Datas"));
#elif PLATFORM_ANDROID
    static FString persistentDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectPersistentDownloadDir(), TEXT("Datas"));
#else
        IPlatformFile &  pf = FPlatformFileManager::Get().GetPlatformFile();
        FString AppendPath = FPaths::Combine(FPaths::ProjectDir(),TEXT("Datas"));
        static FString persistentDir = pf.ConvertToAbsolutePathForExternalAppForWrite(*AppendPath);
#endif
    return persistentDir;
}

FString GFileHelper::ProjectPersistentDownloadDir()
{
#if PLATFORM_WINDOWS
    static FString persistentDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir(), TEXT("Datas"));
#elif PLATFORM_ANDROID
    static FString persistentDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectPersistentDownloadDir(), TEXT("Datas"));
#else
    NSArray *mPath = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    
    NSString *cachesDir = [mPath objectAtIndex : 0];
    
    static FString persistentDir([cachesDir UTF8String]);
    
    //    IPlatformFile &  pf = FPlatformFileManager::Get().GetPlatformFile();
    //    FString AppendPath = FPaths::Combine(FPaths::ProjectDir(),TEXT("Datas"));
    //    static FString persistentDir = pf.ConvertToAbsolutePathForExternalAppForWrite(*AppendPath);
#endif
    return persistentDir;
}

bool GFileHelper::LoadFileToArray(TArray<uint8>& _outArray, const TCHAR* _texPath, uint32 Flags /*= 0*/)
{
    
#if PLATFORM_IOS
    NSString * mPath = [NSString stringWithFormat : @"%s",TCHAR_TO_UTF8(_texPath)];
    
    NSData * mData = [NSData dataWithContentsOfFile : mPath];
    
    if (NULL != mData)
    {
        _outArray.Append((const uint8*)[mData bytes], [mData length]);
        
        return true;
    }
    else
    {
        return false;
    }
#else
    if (FFileHelper::LoadFileToArray(_outArray, _texPath))
    {
        return true;
    }
    return false;
#endif
    //    return false;
}

FString & MutableRootDir()
{
#if PLATFORM_WINDOWS
    static FString persistentDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectPersistentDownloadDir(), TEXT("Datas"));
#elif PLATFORM_ANDROID
    static FString persistentDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectPersistentDownloadDir(), TEXT("Datas"));
#else
    //    IPlatformFile &  pf = FPlatformFileManager::Get().GetPlatformFile();
    //    FString AppendPath = FPaths::Combine(FPaths::ProjectDir(),TEXT("Datas"));
    //    static FString persistentDir = pf.ConvertToAbsolutePathForExternalAppForWrite(*AppendPath);
    //
    NSArray *mPath = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    
    NSString *cachesDir = [mPath objectAtIndex : 0];
    
    static FString persistentDir([cachesDir UTF8String]);
#endif
    return persistentDir;
}



bool GFileHelper::SaveArrayToFile(TArrayView<const uint8> Array, const TCHAR* Filename, IFileManager* _FileManager /*= &IFileManager::Get()*/, uint32 WriteFlags /*= 0*/)
{
#if PLATFORM_IOS || PLATFORM_MAC
    NSData * mData = [NSData dataWithBytes : (void*)Array.GetData() length : Array.Num()];
    
    NSString * mPath = [[NSString alloc]initWithUTF8String:TCHAR_TO_UTF8(Filename)];
    
    BOOL ret = [mData writeToFile : mPath atomically : YES];
    
    return ret;
#else
    return FFileHelper::SaveArrayToFile(Array, Filename, _FileManager, WriteFlags);
#endif
}

bool GFileHelper::LoadFileToString(FString& Result, const TCHAR* Filename, uint32 VerifyFlags /*= 0*/)
{
#if PLATFORM_IOS
    NSString *nsPath = [NSString stringWithFormat : @"%s",TCHAR_TO_UTF8(Filename)];
    
    NSError *error;
    
    NSString *str = [NSString stringWithContentsOfFile : nsPath encoding : NSUTF8StringEncoding error : &error];
    
    Result = UTF8_TO_TCHAR([str UTF8String]);
    
    if (error) {
        
        return true;
    }
    else {
        
        return false;
    }
#else
    return FFileHelper::LoadFileToString(Result, Filename);
#endif
    return false;
}

bool GFileHelper::SaveStringToFile(const FString& String, const TCHAR* Filename, FFileHelper::EEncodingOptions EncodingOptions /*= EEncodingOptions::AutoDetect*/, IFileManager* FileManager /*= &IFileManager::Get()*/, uint32 WriteFlags /*= 0*/)
{
#if PLATFORM_IOS
    //    NSString *str = [NSString stringWithFormat : @"%s",TCHAR_TO_UTF8(*String)];
    NSString *str = String.GetNSString();
    
    //文件不存在会自动创建，文件夹不存在则不会自动创建会报错.
    NSString *nsPath = [NSString stringWithFormat : @"%s",TCHAR_TO_UTF8(Filename)];
    
    NSError *error;
    
    [str writeToFile : nsPath atomically : YES encoding : NSUTF8StringEncoding error : &error];
    
    if (error)
    {
        return true;
    }
    else
    {
        return false;
    }
#else
    return FFileHelper::SaveStringToFile(String, Filename, EncodingOptions, FileManager, WriteFlags);
#endif
    return false;
}

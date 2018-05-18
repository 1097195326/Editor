// Fill out your copyright notice in the Description page of Project Settings.

#include "PakMountHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "IPlatformFilePak.h"
#include "Misc/CoreDelegates.h"
#include "Runtime/CoreUObject/Public/Misc/PackageName.h"
#include "Engine/Engine.h"
#include "GPlatformFileManager.h"


DEFINE_LOG_CATEGORY_STATIC(LogAsset,Log,All)

namespace PakMountHelper
{
    bool MountPak(const FString &inPakFilePath, const FString &inMountPath)
    {
        UE_LOG(LogAsset, Log, TEXT("zhx Begin to mount pak file : %s"), *inPakFilePath);
        
        IPlatformFile* PreviousPlatformFile = NULL;
        FPakPlatformFile *PlatformFile = NULL;
        if (FString(FPlatformFileManager::Get().GetPlatformFile().GetName()).Equals(FString(TEXT("PakFile"))))
        {
            PlatformFile = static_cast<FPakPlatformFile*>(&FPlatformFileManager::Get().GetPlatformFile());
        }
        else
        {
            PlatformFile = new FPakPlatformFile;
            
            if (!PlatformFile->Initialize(&FPlatformFileManager::Get().GetPlatformFile(), TEXT("")))
            {
                UE_LOG(LogAsset, Error, TEXT("zhx FPakPlatformFile failed to initialize"));
                return false;
            }
            PreviousPlatformFile = &FPlatformFileManager::Get().GetPlatformFile();
            //FPlatformFileManager::Get().SetPlatformFile(*PlatformFile);
        }
        
        FString PakMountPoint = FPaths::Combine(FPaths::ProjectContentDir(), inMountPath);
        
        if (PlatformFile->Mount(*inPakFilePath, 0, *PakMountPoint))
        {
            FString VirtualMountPoint(FString::Printf(TEXT("/Game/%s/"), *inMountPath));
            FPackageName::RegisterMountPoint(VirtualMountPoint, PakMountPoint);
            UE_LOG(LogAsset, Warning, TEXT("zhx %s mount to %s success"), *inPakFilePath, *VirtualMountPoint);
        }
        else
        {
            UE_LOG(LogAsset, Error, TEXT("zhx %s mount failed"), *inPakFilePath);
        }
        if (FCoreDelegates::OnMountPak.IsBound())
        {
            FCoreDelegates::OnMountPak.Execute(inPakFilePath, 0, nullptr);
            UE_LOG(LogAsset, Warning, TEXT("zhx FCoreDelegates::OnMountPak excuted!"));
        }
        else
        {
            UE_LOG(LogAsset, Warning, TEXT("zhx FCoreDelegates::OnMountPak.IsBound return false"));
        }
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *FPaths::GamePersistentDownloadDir());
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *inPakFilePath);
        UE_LOG(LogAsset, Log, TEXT("zhx Finish to mount pak file : %s"), *inPakFilePath);
        return true;
    }
    
    template <class FunctorType>
    class PlatformFileFunctor : public IPlatformFile::FDirectoryVisitor    //GenericPlatformFile.h
    {
    public:
        
        virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override
        {
            return Functor(FilenameOrDirectory, bIsDirectory);
        }
        
        PlatformFileFunctor(FunctorType&& FunctorInstance)
        : Functor(MoveTemp(FunctorInstance))
        {
        }
        
    private:
        FunctorType Functor;
    };
    
    template <class Functor>
    PlatformFileFunctor<Functor> MakeDirectoryVisitor(Functor&& FunctorInstance)
    {
        return PlatformFileFunctor<Functor>(MoveTemp(FunctorInstance));
    }
    
    bool GetFiles(const FString& FullPathOfBaseDir, TArray<FString>& FilenamesOut, bool inRecursive, const FString& inFilterByExtension)
    {
        //#if PLATFORM_IOS
        //        @autoreleasepool {
        //            NSString *mFullPathOfBaseDir = [NSString stringWithFormat:@"%s",TCHAR_TO_UTF8(*FullPathOfBaseDir)];
        //
        //            NSString *mInFilterByExtension = [NSString stringWithFormat:@"%s",TCHAR_TO_UTF8(*inFilterByExtension)];
        //
        //            NSFileManager *manager = [NSFileManager defaultManager];
        //
        //            NSArray *allPath =[manager subpathsAtPath:mFullPathOfBaseDir];
        //            //4.遍历所有的子路径
        //            for (NSString *subPath in allPath) {
        //                if([subPath containsString:mInFilterByExtension])
        //                {
        //                    NSString * ffpath = [NSString stringWithFormat:@"%@/%@",mFullPathOfBaseDir,subPath];
        //                    NSLog(@"===%@",ffpath);
        //                    FilenamesOut.Push(UTF8_TO_TCHAR([ffpath UTF8String]));
        //                }
        //            }
        //            return true;
        //        }
        //#else
        //Format File Extension, remove the "." if present
        const FString FileExt = inFilterByExtension.Replace(TEXT("."), TEXT("")).ToLower();
        FString iosFullPathOfBaseDir = GPlatformFileManager::ExtractIOSDir(*FullPathOfBaseDir);
        FString Str;
        auto FilenamesVisitor = MakeDirectoryVisitor(
                                                     [&](const TCHAR* FilenameOrDirectory, bool bIsDirectory)
                                                     {
                                                         //Files
                                                         if (!bIsDirectory)
                                                         {
                                                             //Filter by Extension
                                                             if (FileExt != "")
                                                             {
                                                                 Str = FPaths::GetCleanFilename(FilenameOrDirectory);
                                                                 
                                                                 //Filter by Extension
                                                                 if (FPaths::GetExtension(Str).ToLower() == FileExt)
                                                                 {
                                                                     if (inRecursive)
                                                                     {
                                                                         FilenamesOut.Push(FilenameOrDirectory); //need whole path for recursive
                                                                     }
                                                                     else
                                                                     {
                                                                         FilenamesOut.Push(FilenameOrDirectory);
                                                                     }
                                                                 }
                                                             }
                                                             
                                                             //Include All Filenames!
                                                             else
                                                             {
                                                                 //Just the Directory
                                                                 Str = FPaths::GetCleanFilename(FilenameOrDirectory);
                                                                 
                                                                 if (inRecursive)
                                                                 {
                                                                     FilenamesOut.Push(FilenameOrDirectory); //need whole path for recursive
                                                                 }
                                                                 else
                                                                 {
                                                                     FilenamesOut.Push(FilenameOrDirectory);
                                                                 }
                                                             }
                                                         }
                                                         return true;
                                                     }
                                                     );
        if (inRecursive)
        {
            return FPlatformFileManager::Get().GetPlatformFile().IterateDirectoryRecursively(*iosFullPathOfBaseDir, FilenamesVisitor);
        }
        else
        {
            return FPlatformFileManager::Get().GetPlatformFile().IterateDirectory(*iosFullPathOfBaseDir, FilenamesVisitor);
        }
        //#endif
        
    }
    
}

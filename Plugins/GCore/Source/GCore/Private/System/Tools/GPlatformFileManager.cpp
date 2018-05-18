// Fill out your copyright notice in the Description page of Project Settings.

#include "GCore.h"
#include <iostream>
#include <fstream>
#include "Public/System/Tools/GPlatformFileManager.h"
#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"
#endif



GPlatformFileManager::GPlatformFileManager()
{
}

GPlatformFileManager::~GPlatformFileManager()
{
}


FString GPlatformFileManager::ExtractIOSDir(const TCHAR * DirPath)
{
    string cDirPath = TCHAR_TO_UTF8(DirPath);
    string cSource = GPlatformFileManager::ExtractIOSDir(cDirPath);
    
    return FString(UTF8_TO_TCHAR(cSource.c_str()));
}
string GPlatformFileManager::ExtractIOSDir(const string &DirPath)
{
#if PLATFORM_IOS
    FString rootDir = IFileManager::Get().ConvertToAbsolutePathForExternalAppForWrite(TEXT(""));
    std::string sRootDir(TCHAR_TO_UTF8(*rootDir));
    int len = DirPath.length();
    string folder = DirPath.substr(0, len);
    if(string::npos != DirPath.find(sRootDir))
    {
        int flen = sRootDir.length();
        folder = DirPath.substr(flen,len);
    }
    return folder;
#else
    return DirPath;
#endif
}
bool GPlatformFileManager::FileExists(const TCHAR* Filename)
{
    //    IPlatformFile& mPlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    //    string IosStr = GPlatformFileManager::ExtractIOSDir(TCHAR_TO_UTF8(Filename));
    //    return mPlatformFile.FileExists(UTF8_TO_TCHAR(IosStr.c_str()));
#if PLATFORM_IOS
    @autoreleasepool
    {
        NSString *mPath = [NSString stringWithFormat : @"%s",TCHAR_TO_UTF8(Filename)];
        
        NSFileManager *fileManager = [NSFileManager defaultManager];
        
        BOOL isExist = [fileManager fileExistsAtPath : mPath];
        
        return isExist;
    }
#else
    IPlatformFile& mPlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    return mPlatformFile.FileExists(Filename);
#endif
}

bool GPlatformFileManager::DeleteFile(const TCHAR* Filename)
{
    //    string IosStr = GPlatformFileManager::ExtractIOSDir(TCHAR_TO_UTF8(Filename));
    //    return IFileManager::Get().DeleteDirectory(UTF8_TO_TCHAR(IosStr.c_str()));
#if PLATFORM_IOS
    @autoreleasepool
    {
        NSString *mPath = [NSString stringWithFormat : @"%s",TCHAR_TO_UTF8(Filename)];
        
        NSFileManager *fileManager = [NSFileManager defaultManager];
        
        BOOL res = [fileManager removeItemAtPath : mPath error : nil];
        
        return res;
    }
#else
    return IFileManager::Get().DeleteDirectory(Filename);
#endif
}

bool GPlatformFileManager::DeleteFile(const FString & _path)
{
    //    string IosStr = GPlatformFileManager::ExtractIOSDir(TCHAR_TO_UTF8(*_path));
    //    return  FPlatformFileManager::Get().GetPlatformFile().DeleteFile(UTF8_TO_TCHAR(IosStr.c_str()));
#if PLATFORM_IOS
    
    NSString *mPath = [NSString stringWithFormat : @"%s",TCHAR_TO_UTF8(*_path)];
    
    NSFileManager *fileManager = [NSFileManager defaultManager];
    
    BOOL res = [fileManager removeItemAtPath : mPath error : nil];
    
    return res ? true : false;
#else
    bool ret = FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*_path);
    return ret;
#endif
}

FString GPlatformFileManager::GetAndroidOssUrl(FString InFilePath, FString fileName)
{
    FString ret = FPaths::Combine(fileName);
    ret.Append(".");
    ret.Append(GetFilePathExtPart(*InFilePath));
    return ret;
}

FString GPlatformFileManager::GetAndroidNativeFullUrl(FString inFilePath, FString fileName)
{
    FString ret = FPaths::Combine(MutableRootDir(), FString("Pattern"), fileName);
    ret.Append(".");
    ret.Append(GetFilePathExtPart(*inFilePath));
    return ret;
}

FString GPlatformFileManager::GetWindowsOssUrl(FString InFilePath)
{
    return InFilePath;
}

FString GPlatformFileManager::GetWindowsNativeFullUrl(FString inFilePath)
{
    FString ret = FPaths::Combine(GetFilePathNamePart(*inFilePath));
    ret.Append(".");
    ret.Append(GetFilePathExtPart(*inFilePath));
    return ret;
}

FString GPlatformFileManager::GetIOSNativeFullUrl(FString inFilePath, FString fileName)
{
    FString ret = FPaths::Combine(MutableRootDir(), FString("Pattern"), fileName);
    ret.Append(".");
    ret.Append(GetFilePathExtPart(*inFilePath));
    return ret;
}

FString GPlatformFileManager::GetIOSOssUrl(FString InFilePath, FString fileName)
{
    FString ret = FPaths::Combine(fileName);
    ret.Append(".");
    ret.Append(GetFilePathExtPart(*InFilePath));
    return ret;
}

void GPlatformFileManager::ScanDirectory(TArray<FString>& Files, const FString & FilePath, const FString& Extension)
{
    FString SearchedFiles = FilePath + Extension;
    TArray<FString> FindedFiles;
    
    IFileManager::Get().FindFiles(FindedFiles, *SearchedFiles, true, false);
    
    FString SearchFile = "";
    
    for (int i = 0; i < FindedFiles.Num(); i++)
    {
        SearchFile = FilePath + FindedFiles[i];
        Files.Add(SearchFile);
    }
}



bool GPlatformFileManager::MoveFile(const TCHAR* To, const TCHAR* From)
{
    //#if PLATFORM_IOS
    //    @autoreleasepool
    //    {
    //        NSString *mFromPath = [NSString stringWithFormat : @"%s",TCHAR_TO_UTF8(From)];
    //
    //        NSString *mToPath = [NSString stringWithFormat : @"%s",TCHAR_TO_UTF8(To)];
    //
    //        NSFileManager *fileManager = [NSFileManager defaultManager];
    //
    //        BOOL res = [fileManager moveItemAtPath: mFromPath mToPath: movePath error: nil];
    //
    //        return res;
    //    }
    //#else
    return FPlatformFileManager::Get().GetPlatformFile().MoveFile(To, From);
    //#endif
}

bool GPlatformFileManager::CopyFile(const TCHAR* To, const TCHAR* From)
{
#if PLATFORM_ANDROID
    JNIEnv *Env = FAndroidApplication::GetJavaEnv();
    if (Env)
    {
        jclass Class = FAndroidApplication::FindJavaClass("com/epicgames/ue4/GameActivity");
        if (nullptr != Class)
        {
            static jmethodID Method = Env->GetStaticMethodID(Class, "Android_CopyTo", "(Ljava/lang/String;Ljava/lang/String;)Z");
            jstring src = Env->NewStringUTF(TCHAR_TO_UTF8(From));
            jstring tar = Env->NewStringUTF(TCHAR_TO_UTF8(To));
            Env->CallStaticBooleanMethod(Class, Method, src, tar);
            Env->DeleteLocalRef(src);
            Env->DeleteLocalRef(tar);
        }
        Env->DeleteLocalRef(Class);
    }
#endif
#if PLATFORM_IOS
    @autoreleasepool
    {
        NSString *mInpath = [NSString stringWithFormat : @"%s",TCHAR_TO_UTF8(From)];
        
        NSString *mToPath = [NSString stringWithFormat : @"%s",TCHAR_TO_UTF8(To)];
        
        // 文件管理者
        NSFileManager * manager = [NSFileManager defaultManager];
        
        if ([manager copyItemAtPath : mInpath toPath : mToPath error : nil])
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    
#endif
#if PLATFORM_WINDOWS
    std::ifstream in;
    std::ofstream out;
    in.open(TCHAR_TO_UTF8(From), ios::binary);//打开源文件.
    if (in.fail())//打开源文件失败.
    {
        cout << "Error 1: Fail to open the source file." << endl;
        in.close();
        out.close();
        return false;
    }
    out.open(TCHAR_TO_UTF8(To), ios::binary);//创建目标文件 .
    if (out.fail())//创建文件失败.
    {
        cout << "Error 2: Fail to create the new file." << endl;
        out.close();
        in.close();
        return false;
    }
    else//复制文件.
    {
        out << in.rdbuf();
        out.close();
        in.close();
        return true;
    }
    
#endif
    return false;
}

bool GPlatformFileManager::DirectoryExists(const TCHAR* Directory)
{
    //    IPlatformFile& mPlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    //    string IosStr = GPlatformFileManager::ExtractIOSDir(TCHAR_TO_UTF8(Directory));
    //    return mPlatformFile.DirectoryExists(UTF8_TO_TCHAR(IosStr.c_str()));
    
#if PLATFORM_IOS
    @autoreleasepool
    {
        NSString *mPath = [NSString stringWithFormat : @"%s",TCHAR_TO_UTF8(Directory)];
        
        NSFileManager *fileManager = [NSFileManager defaultManager];
        
        BOOL isExist = [fileManager fileExistsAtPath : mPath];
        
        return isExist;
    }
#else
    IPlatformFile& mPlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	bool mRet = mPlatformFile.DirectoryExists(Directory);
	FString mLog = mRet == true ? "true" : "false";
	UE_LOG(LogTemp, Log, TEXT("DirectoryExists : %s"), *mLog);
    return mRet;
#endif
}

bool GPlatformFileManager::CreateDirectory(const TCHAR* Directory)
{
    //    IPlatformFile& mPlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    //    string IosStr = GPlatformFileManager::ExtractIOSDir(TCHAR_TO_UTF8(Directory));
    //    return  mPlatformFile.CreateDirectory(UTF8_TO_TCHAR(IosStr.c_str()));
#if PLATFORM_IOS
    
    @autoreleasepool
    {
        NSString *mPath = [NSString stringWithFormat : @"%s",TCHAR_TO_UTF8(Directory)];
        
        NSFileManager *fileManager = [NSFileManager defaultManager];
        
        BOOL ret = [fileManager createDirectoryAtPath : mPath withIntermediateDirectories : YES attributes : nil error : nil];
        
        return ret;
    }
#else
    IPlatformFile& mPlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	bool mRet = mPlatformFile.CreateDirectory(Directory);

    return  mRet;
#endif
}

bool GPlatformFileManager::DeleteDirectory(const TCHAR* Directory)
{
    //    string IosStr = GPlatformFileManager::ExtractIOSDir(TCHAR_TO_UTF8(Directory));
    //    return FPlatformFileManager::Get().GetPlatformFile().DeleteDirectoryRecursively(UTF8_TO_TCHAR(IosStr.c_str()));
    
#if PLATFORM_IOS
    @autoreleasepool
    {
        NSString *mPath = [NSString stringWithFormat : @"%s",TCHAR_TO_UTF8(Directory)];
        
        NSFileManager *fileManager = [NSFileManager defaultManager];
        
        BOOL ret = [fileManager createDirectoryAtPath : mPath withIntermediateDirectories : YES attributes : nil error : nil];
        
        return ret;
    }
#else
    return FPlatformFileManager::Get().GetPlatformFile().DeleteDirectoryRecursively(Directory);
#endif
}

void GPlatformFileManager::DeleteDirectory(const FString & FilePath, const FString& Extension)
{
    TArray<FString> mFileArray;
    
    ScanDirectory(mFileArray, FilePath, Extension);
    
    for (int32 i=0; i<mFileArray.Num();i++)
    {
        GPlatformFileManager::DeleteFile(mFileArray[i]);
    }
}

int64 GPlatformFileManager::GetFileSize(const FString & _filePath)
{
    
    if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*_filePath))
    {
        return 0;
    }
    return FPlatformFileManager::Get().GetPlatformFile().FileSize(*_filePath);
}

int64 GPlatformFileManager::GetDirectorySize(const FString & FilePath)
{
    TArray<FString> mFileArray;
    
    ScanDirectory(mFileArray, FilePath, TEXT("*"));
    
    int64 mAllSize = 0;
    
    for (int32 i = 0; i < mFileArray.Num(); i++)
    {
        mAllSize = mAllSize+GetFileSize(mFileArray[i]);
    }
    return mAllSize;
}

FString GPlatformFileManager::ExtractFolder(const TCHAR * _inFilePath)
{
    FString urlPath = _inFilePath;
    
    FString right;
    
    if (urlPath.Split(".", &right, nullptr, ESearchCase::CaseSensitive, ESearchDir::FromEnd))
    {
        return ParentFolder(*right);
    }
    else
    {
        return urlPath;
    }
}

FString GPlatformFileManager::ParentFolder(const TCHAR * _inFilePath)
{
    FString urlPath = _inFilePath;
    
    FString right;
    
    if (urlPath.Split("/", &right, nullptr, ESearchCase::CaseSensitive, ESearchDir::FromEnd))
    {
        return right;
    }
    else
    {
        return urlPath;
    }
}

FString GPlatformFileManager::CreateDateUrl()
{
    FDateTime now = FDateTime::Now();
    FString newIconFolder = FPaths::Combine(TEXT(""), FString::FromInt(now.GetYear()));
    newIconFolder = FPaths::Combine(newIconFolder, FString::FromInt(now.GetMonth()));
    newIconFolder = FPaths::Combine(newIconFolder, FString::FromInt(now.GetDay()));
    newIconFolder = FPaths::Combine(newIconFolder, FString::FromInt(now.GetHour()));
    newIconFolder = FPaths::Combine(newIconFolder, FString::FromInt(now.GetMinute()));
    newIconFolder = FPaths::Combine(newIconFolder, FString::FromInt(now.GetSecond()));
    return newIconFolder;
}

FString GPlatformFileManager::CreateDateUrl2()
{
    FDateTime now = FDateTime::Now();
    FString newIconFolder = FPaths::Combine(TEXT(""), FString::FromInt(now.GetYear()));
    newIconFolder = FPaths::Combine(newIconFolder, FString::FromInt(now.GetMonth()));
    newIconFolder = FPaths::Combine(newIconFolder, FString::FromInt(now.GetDay()));
    newIconFolder = FPaths::Combine(newIconFolder, FString::FromInt(now.GetHour()));
    newIconFolder = FPaths::Combine(newIconFolder, FString::FromInt(now.GetMinute()));
    return newIconFolder;
}

FString GPlatformFileManager::GetFileName(const TCHAR * _inFilePath)
{
    FString urlPath = _inFilePath;
    
    FString namepart;
    FString pathpart;
    FString extpart;
    if (FPaths::ValidatePath(urlPath))
    {
        FPaths::Split(urlPath, pathpart, namepart, extpart);
    }
    return namepart;
}

FString GPlatformFileManager::GetFilePathExtPart(const TCHAR * _inFilePath)
{
    FString urlPath = _inFilePath;
    
    FString namepart;
    FString pathpart;
    FString extpart;
    if (FPaths::ValidatePath(urlPath))
    {
        FPaths::Split(urlPath, pathpart, namepart, extpart);
    }
    return extpart;
}

FString GPlatformFileManager::GetFilePathNamePart(const TCHAR * _inFilePath)
{
    FString urlPath = _inFilePath;
    
    FString namepart;
    FString pathpart;
    FString extpart;
    if (FPaths::ValidatePath(urlPath))
    {
        FPaths::Split(urlPath, pathpart, namepart, extpart);
    }
    return namepart;
}

bool GPlatformFileManager::RecursiveCreateDirectory(const TCHAR* Directory)
{
#if PLATFORM_IOS
    @autoreleasepool
    {
        FString mDownloadPath = GPlatformFileManager::ExtractFolder(Directory);
        
        NSString *mPath = [NSString stringWithFormat : @"%s",TCHAR_TO_UTF8(*mDownloadPath)];
        
        NSFileManager *fileManager = [NSFileManager defaultManager];
        
        BOOL ret = [fileManager createDirectoryAtPath : mPath withIntermediateDirectories : YES attributes : nil error : nil];
        
        return ret;
    }
#else
    FString mFolder = GPlatformFileManager::ExtractFolder(Directory);
    
	return PrecursiveCreateDirectory(*mFolder);
#endif
}

bool GPlatformFileManager::PrecursiveCreateDirectory(const TCHAR* Directory)
{
	// 判断目录是否存在.
	if (GPlatformFileManager::DirectoryExists(Directory))
	{

		UE_LOG(LogTemp, Log, TEXT("RecursiveCreateDirectory : DirectoryExists  true"));
		return true;
	}
	else
	{
		// 获取父级目录.
		FString mParentFolder = GPlatformFileManager::ParentFolder(Directory);

		// 开始递归创建目录.
		GPlatformFileManager::PrecursiveCreateDirectory(*mParentFolder);

		// 真正的创建目录.
		return GPlatformFileManager::CreateDirectory(Directory);
	}
}

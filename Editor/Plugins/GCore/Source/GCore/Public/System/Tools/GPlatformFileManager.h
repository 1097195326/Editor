// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Paths.h"

#include <string>
using namespace std;

#include "FileManagerGeneric.h"
/**
 *
 */
class GCORE_API GPlatformFileManager
{
public:
    GPlatformFileManager();
    ~GPlatformFileManager();
private:
	static bool PrecursiveCreateDirectory(const TCHAR* Directory);
public:
    /** Return true if the file exists. **/
    static bool        FileExists(const TCHAR* Filename);
    
    /** Delete a file and return true if the file exists. Will not delete read only files. **/
    static bool        DeleteFile(const TCHAR* Filename) ;
    
    /** Attempt to move a file. Return true if successful. Will not overwrite existing files. **/
    static bool        MoveFile(const TCHAR* To, const TCHAR* From);
    
    static bool        CopyFile(const TCHAR* To, const TCHAR* From);
    
    static bool DirectoryExists(const TCHAR* Directory);
    
    static bool CreateDirectory(const TCHAR* Directory);
    
    static bool DeleteDirectory(const TCHAR* Directory);
    
    // 递归创建目录.
    static bool RecursiveCreateDirectory(const TCHAR* Directory);
    
    // 根据.分割目录.
    static FString ExtractFolder(const TCHAR * _inFilePath);
    
    // 根据 / 分割字符串.
    static FString ParentFolder(const TCHAR * _inFilePath);
    
    //创建时间路径.
    static FString CreateDateUrl();
    
    static FString CreateDateUrl2();
    //获取文件名.
    static FString GetFileName(const TCHAR * _inFilePath);
    //获取文件后缀.
    static FString GetFilePathExtPart(const TCHAR * _inFilePath);
    //获取文件名字.
    static     FString GetFilePathNamePart(const TCHAR * _inFilePath);
    //删除文件.
    static bool DeleteFile(const FString & _path);
    
    //获取安卓本地OssUrl.
    static FString GetAndroidOssUrl(FString InFilePath, FString fileName);
    //获取安卓本地绝对路径.
    static FString GetAndroidNativeFullUrl(FString inFilePath, FString fileName);
    //获取Windows OssUrl.
    static FString GetWindowsOssUrl(FString InFilePath);
    //获取Windows本地绝对路径.
    static FString GetWindowsNativeFullUrl(FString inFilePath);
    //获取IOS本地绝对路径.
    static FString GetIOSNativeFullUrl(FString inFilePath, FString fileName);
    //获取IOS Oss路径.
    static FString GetIOSOssUrl(FString InFilePath, FString fileName);
    
    static string ExtractIOSDir(const string &DirPath);
    static FString ExtractIOSDir(const TCHAR * DirPath);
    // 遍历文件夹下指定类型文件
    // Files 保存遍例到的所有文件
    // FilePath 文件夹路径  如 "D:\\MyCodes\\LearnUE4Cpp\\Source\\LearnUE4Cpp\\"
    // Extension 扩展名(文件类型) 如 "*.cpp"
    static void ScanDirectory(TArray<FString>& Files, const FString & FilePath, const FString& Extension);
    
    // 遍历文件下的所有文件删除.
    static void DeleteDirectory(const FString & FilePath, const FString& Extension);
    
    // 获取文件大小.
    static int64 GetFileSize(const FString & _filePath);
    
    // 获取目录大小.
    static int64 GetDirectorySize(const FString & FilePath);
};

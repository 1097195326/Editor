#include "OSSModule.h"
#include "OSSHelper.h"
#if PLATFORM_WINDOWS || PLATFORM_MAC
#include "FOSSDownload.h"
#elif PLATFORM_ANDROID
#include "AndroidOSSDownload.h"
#elif PLATFORM_IOS
#include "iOSOSSDownload.h"
#endif

//静态变量必须初始化.
OSSHelper *OSSHelper::Instance = nullptr;


OSSHelper::OSSHelper()
{

	OSSDownload = nullptr;
}

OSSHelper::~OSSHelper()
{

}

OSSHelper * OSSHelper::GetInstance()
{

	if (Instance == nullptr)
	{
		Instance = new OSSHelper();

	}
	return Instance;
}

IOSSDownload * OSSHelper::GetOSSTool()
{
	if (OSSDownload==nullptr)
	{		
#if PLATFORM_WINDOWS || PLATFORM_MAC
		OSSDownload = FOSSDownload::Get();
#elif PLATFORM_ANDROID
		OSSDownload = FAndroidOSSDownload::Get();
#elif PLATFORM_IOS
		OSSDownload = FiOSOSSDownload::Get();
#endif
	}
	return OSSDownload;
}

void OSSHelper::Init(FString _AosEndPoint, FString _AosAccessKeyID, FString _AosAccessKeySecret, FString _AosBucket)
{
	OSSDownload = GetOSSTool();
	OSSDownload->Init(_AosEndPoint, _AosAccessKeyID, _AosAccessKeySecret, _AosBucket);
}

void OSSHelper::Release()
{
	Instance = nullptr;
	OSSDownload->GRelease();
	OSSDownload = nullptr;
	delete this;
}

void OSSHelper::ExitGame()
{
	Instance = nullptr;
	OSSDownload->ExitGame();
	OSSDownload = nullptr;
	delete this;
}

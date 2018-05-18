#include "ServerLogTool.h"



//FHttpRequestCompleteDelegate::CreateRaw(this, &FResourceManagerRepository::CallbackStyleRecommentBatch)
void UServerLogToolBlueprintFunctionLibrary::SendLogToServer(const FString &exceptionInfo,const FString &userName)
{
	//FSeverLogTool::Get()->SendLogToServer(exceptionInfo,userName);
}



FString clientIp("");
FString model("iphonex");
FString apiVersion("1.0.0");
FString version("10.1.1");
FString occurTime("2018 - 01 - 13 01:34 : 26");
//FString exceptionInfo("");
FString appType("FAFA_DESIGNER");
FString source("FAFA_DESIGNER_IOS");
FString os("IOS");
FString storageFree("34.3 GB");
FString level("Error");
FString memory("3.0 GB");
FString memoryFree("");
FString storage("113.6 GB");
FString appVersion("1.4.0");
FString account("10086");
FString deviceCode("216FA314-1DDF-42B3-B87D-D8B2C680C501");
FString cpuType("CPU_TYPE_ARM64");
void FSeverLogTool::NewRequest(const FString &inUrl, bool inIsReqest, const FString &exceptionInfo, FHttpRequestCompleteDelegate inDelegate)
{
	//UE_LOG(ClothHttp, Log, TEXT("Send Http ： url(%s),parameter : %s"), *inUrl, *exceptionInfo);

	FHttpModule::Get().SetHttpTimeout(120);
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	//json header 必须加..
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	HttpRequest->SetURL(inUrl);
	HttpRequest->SetVerb(inIsReqest ? TEXT("GET") : TEXT("POST"));
	HttpRequest->SetContentAsString(exceptionInfo);
	HttpRequest->OnProcessRequestComplete() = inDelegate;
	HttpRequest->ProcessRequest();

}

FString FSeverLogTool::FormatParameter(const FString &exceptionInfo,const FString &userName)
{
	FDateTime now = FDateTime::Now();
	occurTime = now.ToString();
	account = userName;
	return FString::Printf(TEXT("[{\"model\":\"%s\",\"apiVersion\":\"%s\",\"version\":\"%s\",\"occurTime\":\"%s\",\"exceptionInfo\":\"%s\",\"appType\":\"%s\",\"source\":\"%s\",\"os\":\"%s\",\"storageFree\":\"%s\",\"level\":\"%s\",\"memory\":\"%s\",\"memoryFree\":\"%s\",\"storage\":\"%s\",\"appVersion\":\"%s\",\"account\":\"%s\",\"deviceCode\":\"%s\",\"cpuType\":\"%s\"}]"),
		*model, *apiVersion, *version, *occurTime, *exceptionInfo, *appType, *source, *os, *storageFree, *level, *memory, *memoryFree, *storage, *appVersion, *account, *deviceCode, *cpuType);
}

void FSeverLogTool::SendLogToServer(const FString &exceptionInfo, const FString &userName)
{
	//FString Content = FormatParameter(exceptionInfo,userName);
	//SendPost("http://trace.fafaapp.com/exception/log", Content, FHttpRequestCompleteDelegate::CreateRaw(this,&FSeverLogTool::CallbackSendLogToServer));
}

void FSeverLogTool::SendPost(const FString &inUrl, const FString &inContent, FHttpRequestCompleteDelegate inDelegate)
{
	//NewRequest(inUrl, false, inContent, inDelegate);
}
void FSeverLogTool::CallbackSendLogToServer(FHttpRequestPtr inReq, FHttpResponsePtr inResp, bool inSuccess)
{
	FString retMsg;
	if (inSuccess)
	{
		if (inResp.IsValid())
		{
			retMsg = inResp->GetContentAsString();
		//	UE_LOG(ClothHttp, Log, TEXT("CallbackSendLogToServer:%s"), *retMsg);
		}
		else
		{
		//	UE_LOG(ClothHttp, Error, TEXT("CallbackSendLogToServer, Failed"));
		}
	}
	else
	{
		//UE_LOG(ClothHttp, Error, TEXT("CallbackSendLogToServer failed, Failed"));

	}
}

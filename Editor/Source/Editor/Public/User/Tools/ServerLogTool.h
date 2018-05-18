#pragma once
#include "CoreMinimal.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "ServerLogTool.generated.h"
UCLASS()
class EDITOR_API UServerLogToolBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	

public:
	UFUNCTION(BlueprintCallable, Category = "FAFAServerLog")
	static void SendLogToServer(const FString &exceptionInfo,const FString &userName);
	 
};


class FSeverLogTool
{

public:
	FSeverLogTool(){}
	static inline FSeverLogTool * Get() {
		static FSeverLogTool *instance = new FSeverLogTool();
		return instance;
	}
	void SendLogToServer(const FString &exceptionInfo, const FString &userName);

private:
	FString FormatParameter(const FString &exceptionInfo, const FString &userName);
	void CallbackSendLogToServer(FHttpRequestPtr inReq, FHttpResponsePtr inResp, bool inSuccess);
	void NewRequest(const FString &inUrl, bool inIsReqest, const FString &inContent, FHttpRequestCompleteDelegate inDelegate);
	void SendPost(const FString &inUrl, const FString &inContent, FHttpRequestCompleteDelegate inDelegate);
};
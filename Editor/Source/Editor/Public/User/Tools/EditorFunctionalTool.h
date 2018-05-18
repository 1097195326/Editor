//AUTHOR:XINGLONG DATE 2018/2/7

#pragma once

#include "CoreMinimal.h"
#include "MsgCenter.h"
/**
 * 
 */
UENUM(BlueprintType)
enum class EQualityLevel :uint8
{
	E_Low = 0 UMETA(DisplayName = "Low"),
	E_Medium UMETA(DisplayName = "Medium"),
	E_High UMETA(DisplayName = "High"),
	E_Higher UMETA(DisplayName = "Higher"),
	E_Highest UMETA(DisplayName = "Highest")
};

class EDITOR_API EditorFunctionalTool
{
public:
	EditorFunctionalTool();
	~EditorFunctionalTool();

	static bool CheckStringIsValid(const FString& str, const FString& Reg);
	static bool CheckPhone(const FString& str);
	static bool CheckIdentifyingCode(const FString& str);
	static bool CheckPassword(const FString& str, FString& tip);
	static FString Str2Md5(const FString& str);
	static bool IdentifyingCodeCheck(const FString &json, FString& tip);
	static bool IdentifyingCodeCheck(msg_ptr _msg);
	static bool LoginCheck(msg_ptr _msg,FString& _Tip);
	static bool FindNewPasswordHttpCheck(const FString &json, FString& tip);

	static bool CanAutoLogin(FString& phone, FString& password);

	// ª≠÷ …Ë÷√
	void SetQualityByShortcut(EQualityLevel _Level);

	static FString GetURLTicketStr();

	static FString GetTicketUrl(FString &url);

	/*static char* ltrim_lc(char* s);

	static char* rtrim_lc(char* s);*/

	//FString Encrypt(FString src, WORD key);
};

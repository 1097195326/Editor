#include "Win_ForcedUpdateUMG.h"
#include "GModuleManager.h"
#include "MsgCenter.h"
#include "Editor.h"
#include "HaoshaSDKBPLibrary.h"
#include "UIManager.h"
#include "GLanguageTool.h"
#include "LogicUpdateScene.h"
#include "GameDirector.h"
#include "UpdateUIController.h"
#include "GameDirector.h"

UWin_ForcedUpdateUMG::UWin_ForcedUpdateUMG(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	m_BtnCancel = nullptr;
	m_BtnConfirm = nullptr;
	m_Text_Msg = nullptr;
	m_Text_Btn_Cancel = nullptr;
	m_Text_Btn_Confirm = nullptr;
	m_Forced = -1000;
}

bool UWin_ForcedUpdateUMG::OnStart()
{
	//Super::OnStart();
	bool mRet = false;

	UpdateResourceModule *mUpdateResourceModule = GModuleManager::GetInstance()->GetModule<UpdateResourceModule>("UpdateResourceModule");

	//监听本地事件.
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, mUpdateResourceModule->m_ForceUpdate_Response_LocalPid, this, &UWin_ForcedUpdateUMG::OnForceUpdateResponseMsg);
	
	mRet = true;
	return mRet;
}

void UWin_ForcedUpdateUMG::OnInit()
{
	//Super::OnInit();
	//初始化UI.
	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("Button_Cancel")))
	{
		m_BtnCancel = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_BtnCancel, this, &UWin_ForcedUpdateUMG::OnButtonCancel);

	}
	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("Button_Confirm")))
	{
		m_BtnConfirm = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_BtnConfirm, this, &UWin_ForcedUpdateUMG::OnButtonConfirm);
	}
	//提示框内容.
	if (UTextBlock *mTextBlock = Cast<UTextBlock>(GetWidgetFromName("Text_Msg")))
	{
		m_Text_Msg = mTextBlock;
	}
	if (UTextBlock *mTextBlock = Cast<UTextBlock>(GetWidgetFromName("Text_Btn_Confirm")))
	{
		m_Text_Btn_Confirm = mTextBlock;
	}
	if (UTextBlock *mTextBlock = Cast<UTextBlock>(GetWidgetFromName("Text_Btn_Cancel")))
	{
		m_Text_Btn_Cancel = mTextBlock;
	}
	if (UCanvasPanel *mTemp = Cast<UCanvasPanel>(GetWidgetFromName("Main")))
	{
		m_Canv_Main = mTemp;
	}
}

void UWin_ForcedUpdateUMG::OnRelease()
{
	Super::OnRelease();
	UpdateResourceModule *mUpdateResourceModule = GModuleManager::GetInstance()->GetModule<UpdateResourceModule>("UpdateResourceModule");
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, mUpdateResourceModule->m_ForceUpdate_Response_LocalPid, this, &UWin_ForcedUpdateUMG::OnForceUpdateResponseMsg);
	UUIManager::GetInstance()->RemoveButtonClick(m_BtnCancel);
	UUIManager::GetInstance()->RemoveButtonClick(m_BtnConfirm);
	m_BtnCancel = nullptr;
	m_BtnConfirm = nullptr;
	m_Text_Msg = nullptr;
	m_Text_Btn_Cancel = nullptr;
	m_Text_Btn_Confirm = nullptr;
	m_Forced = -1000;
	
}

void UWin_ForcedUpdateUMG::OnSetParams(int32 _pid, FString & _content, ...)
{
	Super::OnSetParams(_pid,_content);
}

void UWin_ForcedUpdateUMG::OnTick(float InDeltaTimeSecond)
{
	Super::OnTick(InDeltaTimeSecond);


}


void UWin_ForcedUpdateUMG::OnForceUpdateResponseMsg(msg_ptr _msg)
{
	//装换成本地消息.
	MsgLocal * mMsgLocal = _msg->ConvertMsg<MsgLocal>();
	//获取原数据.
	TSharedPtr<FJsonObject> *t_jsonObject = (TSharedPtr<FJsonObject>*)mMsgLocal->Content();
	const TSharedPtr<FJsonObject> &mJsonObj= t_jsonObject->ToSharedRef()->GetObjectField("data");
	m_Forced = mJsonObj->GetIntegerField("forced");
	m_Content = mJsonObj->GetStringField("content");
	m_Url = mJsonObj->GetStringField("url");
	ExecuteLogic();
}

void UWin_ForcedUpdateUMG::ExecuteLogic()
{
	//设置文字内容.
	SetDisplayText();
	if (m_Forced==-1)
	{
		GoNext();
	}
	else if (m_Forced==0)
	{
		DoCanUpdate();
	}
	else if(m_Forced==1)
	{
		DoForcedUpdate();
	}
}

void UWin_ForcedUpdateUMG::OpenURL(const FString &_Url)
{
	UHaoshaSDKBPLibrary::OpenURL(_Url);
}


FString UWin_ForcedUpdateUMG::MsgJsonToString(msg_ptr _msg)
{
	MsgLocal * mMsgLocal = _msg->ConvertMsg<MsgLocal>();

	TSharedPtr<FJsonObject> * t_jsonObject = (TSharedPtr<FJsonObject>*)mMsgLocal->Content();


	FString mContent;

	TSharedRef<TJsonWriter<TCHAR>> t_writer = TJsonWriterFactory<>::Create(&mContent);

	FJsonSerializer::Serialize((*t_jsonObject).ToSharedRef(), t_writer);

	UE_LOG(GEditorLog, Error, TEXT("Forced Update Response:%s"), *mContent);

	return mContent;
}

void UWin_ForcedUpdateUMG::OnActivationUI()
{
	this->SetVisibility(ESlateVisibility::Visible);
}

void UWin_ForcedUpdateUMG::OnBackstageUI()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UWin_ForcedUpdateUMG::QuitGame()
{
	UGameDirector::GetInstance()->QuitGame();
}

void UWin_ForcedUpdateUMG::GoNext()
{
	//隐藏强制更新UI.
	HidenMe();
	//执行热更新逻辑.
	


	LogicUpdateScene* mLogicDesignScene = (LogicUpdateScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();
	UpdateUIController* mUpdateUIController =(UpdateUIController*)mLogicDesignScene->GetCurrentUIController();

	mUpdateUIController->ShowProgressUI();
	mLogicDesignScene->OnFinishedForcedUpdate();

}

void UWin_ForcedUpdateUMG::OnButtonCancel(int32 param)
{
	if (m_Forced==0)
	{
		GoNext();
	}
	else if(m_Forced==1)
	{
		QuitGame();
	}
}

void UWin_ForcedUpdateUMG::OnButtonConfirm(int32 param)
{
	UE_LOG(GEditorLog, Error, TEXT("Forced Update URL:%s"), *m_Url);

	OpenURL(m_Url);
}

void UWin_ForcedUpdateUMG::SetDisplayText()
{
	FString mCancel = GLanguageTool::GetInstance()->GetLanguage(TEXT("key102"));
	FString mConfirm = GLanguageTool::GetInstance()->GetLanguage(TEXT("key103"));
	m_Text_Msg->SetText(FText::FromString(m_Content));
	m_Text_Btn_Cancel->SetText(FText::FromString(mCancel));
	m_Text_Btn_Confirm->SetText(FText::FromString(mConfirm));
}

void UWin_ForcedUpdateUMG::DoForcedUpdate()
{
	m_Canv_Main->SetVisibility(ESlateVisibility::Visible);
	m_BtnCancel->SetVisibility(ESlateVisibility::Hidden);
	FWidgetTransform mTransform;
	mTransform.Translation = FVector2D(-168,0);
	mTransform.Scale = FVector2D(1.0,1.0);
	
	m_BtnConfirm->SetRenderTransform(mTransform);

}

void UWin_ForcedUpdateUMG::DoCanUpdate()
{
	m_Canv_Main->SetVisibility(ESlateVisibility::Visible);
}

void UWin_ForcedUpdateUMG::HidenMe()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "Win_MyWorks.h"
#include "UIManager.h"
#include "MainModule.h"
#include "Win_SelectCell.h"
#include "GameDirector.h"
#include "GLanguageTool.h"
#include "GModuleManager.h"

UWin_MyWorks::UWin_MyWorks(const FObjectInitializer& ObjectInitializer)
{
	
}

bool UWin_MyWorks::OnStart()
{
	MainModule * mMainModule = GModuleManager::GetInstance()->GetModule<MainModule>("MainModule");
	
	//	返回上一页面按钮--点击事件.
	UUIManager::GetInstance()->RegisterButtonClick(1, m_BtnReturn, this, &UWin_MyWorks::OpenPageCell);

	//	监听消息(我的作品).
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, mMainModule->m_AllWorks_Content_LocalPid, this, &UWin_MyWorks::OnMyWorksContentDataFinished);

	//	更新类别.
	//UpdateHead();
	
	//	获取第一个类别的作品.
	LoadMyWorks(0);
	
	return true;
}

void UWin_MyWorks::OnInit()
{
	//	返回上一页面按钮.
	if (UButton* BtnReturn = Cast<UButton>(GetWidgetFromName("Button_Return")))
	{
		m_BtnReturn = BtnReturn;
	}

	//	类别.
	if (UScrollBox* ScrollBoxHead = Cast<UScrollBox>(GetWidgetFromName("ScrollBox_Head")))
	{
		m_ScrollBoxHead = ScrollBoxHead;
	}

	//	作品滑动页.
	if (UScrollBox* ScrollBoxWorks = Cast<UScrollBox>(GetWidgetFromName("ScrollBox_MyWorks")))
	{
		m_ScrollBoxWorks = ScrollBoxWorks;
	}

	//	作品列表.
	if (UGridPanel* GridPanelWorks = Cast<UGridPanel>(GetWidgetFromName("GridPanel_MyWorks")))
	{
		m_GridPanelWorks = GridPanelWorks;
	}

	m_PageIndex = 0;

	m_Isloading = false;
}

void UWin_MyWorks::OnRelease()
{
	m_MyWorksContent.Empty();

	UUIManager::GetInstance()->RemoveButtonClick(m_BtnReturn);

	MainModule *mMainModule = GModuleManager::GetInstance()->GetModule<MainModule>("MainModule");

	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, mMainModule->m_AllWorks_Content_LocalPid, this, &UWin_MyWorks::OnMyWorksContentDataFinished);
}

void UWin_MyWorks::OnSetParams(int32 _pid, FString & _content, ...)
{
}

void UWin_MyWorks::OnTick(float InDeltaTimeSecond)
{	
}

void UWin_MyWorks::OpenPageCell(int32 _pid)
{
	this->RemoveFromViewport();

	UGameDirector::GetInstance()->GetCurrentLogicScene()->GetCurrentUIController()->GetRootUI()->OnActivationUI();
}

// UWin_ButtonSuitType* UWin_MyWorks::CreateNewHead(int32 _typeId, FString _name, bool _Last)
// {
// 	float mDefaultPadingRight = 60.0f;
// 
// 	UWin_ButtonSuitType * mSuitType = Cast<UWin_ButtonSuitType>(UUIManager::GetInstance()->OpenUI(FString(TEXT("Btn_SuitType"))));
// 
// 	mSuitType->SetData(_name, _typeId);
// 
// 	mSuitType->SetPadding(FMargin(0.0f, 0.0f, 0.0f, mDefaultPadingRight));
// 
// 	UUIManager::GetInstance()->RegisterButtonClick(mSuitType->GetID(), mSuitType->m_ButtonSuit, this, &UWin_MyWorks::LoadMyWorks);
// 
// 	return mSuitType;
// }
// 
// void UWin_MyWorks::UpdateHead()
// {
// 	m_ScrollBoxHead->ClearChildren();
// 
// 	m_ScrollBoxHead->ScrollToStart();
// 
// 	/*FString mPattern = GLanguageTool::GetInstance()->GetLanguage(TEXT("key134"));
// 
// 	UButtonSuitType* mSuitType = CreateNewHead(0, mPattern, true);
// 
// 	m_ScrollBoxHead->AddChild(mSuitType);*/
// }

//	检查是否可以加载.
bool UWin_MyWorks::CheckCanLoad()
{
	bool Res = false;
	if (m_Isloading)
	{
		return false;
	}
	if (m_PageIndex == 0)
	{
		return true;
	}
	if (m_PageIndex < m_TotalPage)
	{
		return true;
	}
	return false;
}

//	加载作品.
void UWin_MyWorks::LoadMyWorks(int _id)
{
	if (CheckCanLoad())
	{
		m_Isloading = true;

		/*if (UButtonSuitType * mSuitType = Cast<UButtonSuitType>(m_ScrollBoxHead->GetChildAt(_id)))
		{
			mSuitType->SetSelect();
		}*/
		MainModule * mMainModule = GModuleManager::GetInstance()->GetModule<MainModule>("MainModule");

		mMainModule->GetUserStyleData(m_PageIndex + 1, 20);

	}
}

//	创建我的作品cell.
void UWin_MyWorks::CreateSelectCell(int32 _num)
{
	int mIndex = m_MyWorksContent.Num();

	for (int32 i = 0; i < _num; ++i)
	{
		UWin_SelectCell * mTemp = (UWin_SelectCell*)UUIManager::GetInstance()->OpenUI(TEXT("SelectCell"));
		
		UGridSlot * mGridSlot = m_GridPanelWorks->AddChildToGrid(mTemp);

		int index = mIndex + i;

		mGridSlot->SetRow(index % 2);

		mGridSlot->SetColumn(index / 2);

		mTemp->SetPadding(FMargin(0.0f, 0.0f, 15.0f, 10.0f));

		m_MyWorksContent.Add(mTemp);
	}
}

//	收到服务器发给本地的获取我的作品成功的消息.
void UWin_MyWorks::OnMyWorksContentDataFinished(msg_ptr _msg)
{
	MsgLocal * lMsg = _msg->ConvertMsg<MsgLocal>();

	ResHttp Rh = *((ResHttp*)lMsg->Content());

	m_PageIndex = Rh.m_pageIndex;

	m_TotalPage = Rh.m_totalPage;

	MainModule *mMainModule = GModuleManager::GetInstance()->GetModule<MainModule>("MainModule");

	TArray<SelectCellData> mSelectCellDatas = mMainModule->GetMyWorksSelectCellDatas();

	int32 mWorkIndex = m_MyWorksContent.Num();

	CreateSelectCell(mSelectCellDatas.Num());

	for (int32 i = mWorkIndex; i < m_MyWorksContent.Num(); ++i)
	{
// 		UWin_SelectCell * mTemp = (UWin_SelectCell*)UUIManager::GetInstance()->OpenUI(TEXT("SelectCell"));
// 
// 		if (mTemp && mTemp->IsValidLowLevel())
// 		{
// 			UGridSlot * mGridSlot = m_GridPanelWorks->AddChildToGrid(mTemp);
// 
// 			mGridSlot->SetRow(i / 10);
// 
// 			mGridSlot->SetColumn((m_PageIndex - 1) * 10 + i % 10);
// 
// 			mTemp->SetPadding(FMargin(0.0f, 0.0f, MY_WORKS_CONTENT_RIGHT_PADDING, MY_WORKS_CONTENT_RIGHT_PADDING));

			m_MyWorksContent[i]->SetIconURL(mSelectCellDatas[i- mWorkIndex].m_IconUrl);

			m_MyWorksContent[i]->SetTid(mSelectCellDatas[i - mWorkIndex].m_Tid);

			m_MyWorksContent[i]->SetRid(mSelectCellDatas[i - mWorkIndex].m_Rid);

			m_MyWorksContent[i]->SetDesignType(EnumDesignType::DESIGN_BY_USER_STYLE);

			m_MyWorksContent[i]->DownloadIconImage();

			//m_MyWorksContent.Add(mTemp);
		//}
	}
	m_Isloading = false;
}


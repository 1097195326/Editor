// Fill out your copyright notice in the Description page of Project Settings.

#include "MyWorks.h"
#include "UIManager.h"
#include "MainModule.h"
#include "SelectCell.h"
#include "GameDirector.h"
#include "GLanguageTool.h"
#include "GModuleManager.h"

UMyWorks::UMyWorks(const FObjectInitializer& ObjectInitializer)
{
	
}

bool UMyWorks::OnStart()
{
	//	返回上一页面按钮--点击事件.
	UUIManager::GetInstance()->RegisterButtonClick(1, m_BtnReturn, this, &UMyWorks::OpenPageCell);
	MainModule * mMainModule = GModuleManager::GetInstance()->GetModule<MainModule>("MainModule");
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, mMainModule->m_AllWorks_Content_LocalPid, this, &UMyWorks::OnMyWorksContentDataFinished);

	//	更新类别.
	UpdateHead();

	
	//	获取第一个类别的作品.
	LoadMyWorks(0);

	
	return true;
}

void UMyWorks::OnInit()
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

void UMyWorks::OnRelease()
{
	m_MyWorksContent.Empty();

	UUIManager::GetInstance()->RemoveButtonClick(m_BtnReturn);

	MainModule *mMainModule = GModuleManager::GetInstance()->GetModule<MainModule>("MainModule");
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, mMainModule->m_AllWorks_Content_LocalPid, this, &UMyWorks::OnMyWorksContentDataFinished);

}

void UMyWorks::OnSetParams(int32 _pid, FString & _content, ...)
{

}

void UMyWorks::OnTick(float InDeltaTimeSecond)
{
	
}

void UMyWorks::OpenPageCell(int32 _pid)
{
	this->RemoveFromViewport();

	UGameDirector::GetInstance()->GetCurrentLogicScene()->GetCurrentUIController()->GetRootUI()->OnActivationUI();
}

UButtonSuitType* UMyWorks::CreateNewHead(int32 _typeId, FString _name, bool _Last)
{	float mDefaultPadingRight = 60.0f;

	UButtonSuitType * mSuitType = Cast<UButtonSuitType>(UUIManager::GetInstance()->OpenUI(FString(TEXT("Btn_SuitType"))));

	mSuitType->SetData(_name, _typeId);

	mSuitType->SetPadding(FMargin(0.0f, 0.0f, 0.0f, mDefaultPadingRight));

	UUIManager::GetInstance()->RegisterButtonClick(mSuitType->GetID(), mSuitType->m_ButtonSuit, this, &UMyWorks::LoadMyWorks);

	return mSuitType;
}

void UMyWorks::UpdateHead()
{
	m_ScrollBoxHead->ClearChildren();

	m_ScrollBoxHead->ScrollToStart();

	/*FString mPattern = GLanguageTool::GetInstance()->GetLanguage(TEXT("key134"));

	UButtonSuitType* mSuitType = CreateNewHead(0, mPattern, true);

	m_ScrollBoxHead->AddChild(mSuitType);*/
}
bool UMyWorks::CheckCanLoad()
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
void UMyWorks::LoadMyWorks(int _id)
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

void UMyWorks::CreateSelectCell(int32 _num)
{
	int mIndex = m_MyWorksContent.Num();

	for (int32 i = 0; i < _num; ++i)
	{
		USelectCell * mTemp = (USelectCell*)UUIManager::GetInstance()->OpenUI(TEXT("SelectCell"));
		
		UGridSlot * mGridSlot = m_GridPanelWorks->AddChildToGrid(mTemp);
		int index = mIndex + i;
		mGridSlot->SetRow(index / 2);

		mGridSlot->SetColumn(index % 2);

		mTemp->SetPadding(FMargin(20.0, 0.0f, MY_WORKS_CONTENT_RIGHT_PADDING, MY_WORKS_CONTENT_RIGHT_PADDING));

		m_MyWorksContent.Add(mTemp);
	}
}

void UMyWorks::OnMyWorksContentDataFinished(msg_ptr _msg)
{
	MsgLocal * lMsg = _msg->ConvertMsg<MsgLocal>();

	ResHttp Rh = *((ResHttp*)lMsg->Content());
	m_PageIndex = Rh.m_pageIndex;
	m_TotalPage = Rh.m_totalPage;

	MainModule *mMainModule = GModuleManager::GetInstance()->GetModule<MainModule>("MainModule");
	TArray<SelectCellData> mSelectCellDatas = mMainModule->GetMyWorksSelectCellDatas();

	int mIndex = m_MyWorksContent.Num();

	for (int i = 0; i < mSelectCellDatas.Num(); ++i)
	{
		int index = mIndex + i;

		USelectCell * mTemp = (USelectCell*)UUIManager::GetInstance()->OpenUI(TEXT("SelectCell"));
		if (mTemp && mTemp->IsValidLowLevel())
		{
			UGridSlot * mGridSlot = m_GridPanelWorks->AddChildToGrid(mTemp);
			mGridSlot->SetRow(index / 2);
			mGridSlot->SetColumn(index % 2);

			mTemp->SetPadding(FMargin(20.0, 0.0f, MY_WORKS_CONTENT_RIGHT_PADDING, MY_WORKS_CONTENT_RIGHT_PADDING));
			mTemp->SetIconURL(mSelectCellDatas[i].m_IconUrl);
			mTemp->SetTid(mSelectCellDatas[i].m_Tid);
			mTemp->SetRid(mSelectCellDatas[i].m_Rid);
			mTemp->SetDesignType(EnumDesignType::DESIGN_BY_USER_STYLE);
			mTemp->DownloadIconImage();

			m_MyWorksContent.Add(mTemp);
		}
	}
	
	//if(i == mSelectCellDatas.Num())
	m_Isloading = false;
}


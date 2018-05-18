// Fill out your copyright notice in the Description page of Project Settings.

#include "Win_RecommendStyleUMG.h"
#include "UIManager.h"
#include "GameDirector.h"
#include "GModuleManager.h"
#include "MsgCenter.h"
#include "HomeInfoManager.h"
#include "TDesignTypeManager.h"
#include "Win_StyleSpecialShow.h"
#include "HotLoadManager.h"

static int PageSize = 20;

int32 UWin_RecommendStyleUMG::m_ParentId = 1006;

bool UWin_RecommendStyleUMG::OnStart()
{
    TArray<HomeInfoT*> homeInfoArr;
    if(m_ParentId == 1006)
    {
        homeInfoArr = HomeInfoManager::GetInstance()->GetBikiniTypes();
    }else if (m_ParentId == 1025)
    {
        homeInfoArr = HomeInfoManager::GetInstance()->GetSportTypes();
    }
    
	if (homeInfoArr.Num() > 0)
	{
		UpdateShow();
	}

	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, 109, this, &UWin_RecommendStyleUMG::OnRecommendSuitIconFinished);

	UpdateHead();

	EnterRecommend();

	return true;
}

void UWin_RecommendStyleUMG::OnInit()
{

	if (UScrollBox* Head = Cast<UScrollBox>(GetWidgetFromName("Head")))
	{
		m_Head = Head;
	}
	if (UGridPanel* GridPanel = Cast<UGridPanel>(GetWidgetFromName("HeadGrid")))
	{
		m_HeadGrid = GridPanel;
	}
	if (UButton* _BackButton = Cast<UButton>(GetWidgetFromName("BackButton")))
	{
		m_BackButton = _BackButton;
		UUIManager::GetInstance()->RegisterButtonClick(1, m_BackButton, this, &UWin_RecommendStyleUMG::BackToMainPage);
	}

	if (UGridPanel* GridPanelSuit = Cast<UGridPanel>(GetWidgetFromName("GridPanelSuit")))
	{
		m_GridPanelSuit = GridPanelSuit;
	}
	
	if (UScrollBox* ScrollBoxItem = Cast<UScrollBox>(GetWidgetFromName("ScrollBoxItem1")))
	{
		m_ScrollBoxItem1 = ScrollBoxItem;
	}
	if (UGridPanel* GridPanelSuitItem = Cast<UGridPanel>(GetWidgetFromName("GridPanelSuitItem1")))
	{
		m_GridPanelSuitItem1 = GridPanelSuitItem;
	}
	if (UScrollBox* ScrollBoxItem = Cast<UScrollBox>(GetWidgetFromName("ScrollBoxItem2")))
	{
		m_ScrollBoxItem2 = ScrollBoxItem;
	}
	if (UGridPanel* GridPanelSuitItem = Cast<UGridPanel>(GetWidgetFromName("GridPanelSuitItem2")))
	{
		m_GridPanelSuitItem2 = GridPanelSuitItem;
	}
	if (UCanvasPanel* SpecialShowView = Cast<UCanvasPanel>(GetWidgetFromName("SpecialShowView")))
	{			
		m_SpecialShowView = SpecialShowView;
	}

	mIsLoading = false;

	m_CurrentCatId = 0;
}

void UWin_RecommendStyleUMG::OnRelease()
{
	ArrButtonSuitType.Empty();

	UUIManager::GetInstance()->RemoveButtonClick(m_BackButton);

	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, 109, this, &UWin_RecommendStyleUMG::OnRecommendSuitIconFinished);
	
}

void UWin_RecommendStyleUMG::OnSetParams(int32 _pid, FString & _content, ...)
{

}

void UWin_RecommendStyleUMG::OnTick(float InDeltaTimeSecond)
{

}

void UWin_RecommendStyleUMG::BackToMainPage(int32 id)
{
	UE_LOG(LogTemp, Log, TEXT("Recommend style umg Back to Main Page"));

	this->RemoveFromViewport();

	UGameDirector::GetInstance()->GetCurrentLogicScene()->GetCurrentUIController()->GetRootUI()->OnActivationUI();
}

//	点击推荐款类型.
void UWin_RecommendStyleUMG::OnClick(int _id)
{
	UE_LOG(LogTemp, Log, TEXT("Catid : %d"), _id);
	
	//m_SpecialShowView->SetVisibility(ESlateVisibility::Hidden);

	//m_ScrollBoxItem1->SetVisibility(ESlateVisibility::Hidden);

	if (_id == m_ParentId)
	{
        TArray<HomeInfoT*> homeInfoArr;

        if(m_ParentId == 1006)
        {
            homeInfoArr = HomeInfoManager::GetInstance()->GetBikiniTypes();
        }
		else if (m_ParentId == 1025)
        {
            homeInfoArr = HomeInfoManager::GetInstance()->GetSportTypes();
        }
        if(homeInfoArr.Num() > 0)
        {
            //m_SpecialShowView->SetVisibility(ESlateVisibility::Visible);
            m_ScrollBoxItem = m_ScrollBoxItem2;
            m_GridPanelSuitItem = m_GridPanelSuitItem2;
        }
// 		else
//         {
//             m_ScrollBoxItem1->SetVisibility(ESlateVisibility::Visible);
//             m_ScrollBoxItem = m_ScrollBoxItem1;
//             m_GridPanelSuitItem = m_GridPanelSuitItem1;
//         }
	}
// 	else
// 	{
// 		m_ScrollBoxItem1->SetVisibility(ESlateVisibility::Visible);
// 		m_ScrollBoxItem = m_ScrollBoxItem1;
// 		m_GridPanelSuitItem = m_GridPanelSuitItem1;
// 	}

	m_AllSyles.Empty();

    m_CurrentCatId = _id;

	ShowTypeStyle(m_CurrentCatId);
	
	for (int32 i = 0;i < ArrButtonSuitType.Num();i++)
	{
		if (ArrButtonSuitType[i]->GetID() == _id)
		{
			ArrButtonSuitType[i]->SetSelect();
		}
		else
		{
			ArrButtonSuitType[i]->SetNormal();
		}
	}
}

void UWin_RecommendStyleUMG::UpdateShow()
{
// 	UWin_StyleSpecialShow * SpecialShow = (UWin_StyleSpecialShow*)UUIManager::GetInstance()->OpenUI(TEXT("StyleSpecialShow"));
// 	SpecialShow->SetLogicParent(this);
// 	m_SpecialShowView->AddChild(SpecialShow);
}

//	更新类别.
void UWin_RecommendStyleUMG::UpdateHead()
{
	m_HeadGrid->ClearChildren();

	m_Head->ScrollToStart();

	m_GridPanelSuit->ClearChildren();

 	UWin_ButtonSuitType* temp = CreateNewHead(FString(TEXT("全部")), true, m_ParentId);

	UGridSlot * mFSlot = m_HeadGrid->AddChildToGrid(temp);

	mFSlot->SetColumn(0);

	mFSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 0.f));

	ArrButtonSuitType.Add(temp);

	TArray<DesignType*> mArr = TDesignTypeManager::GetInstance()->GetAllDesignTypesByParentId(m_ParentId);

	if (mArr.Num() > 0)
	{
		for (int32 i = 0; i < mArr.Num(); ++i)
		{
			FString _String = mArr[i]->GetName();

			UWin_ButtonSuitType* temp2 = CreateNewHead(_String, (i == (mArr.Num() - 1)), mArr[i]->GetTid());

			ArrButtonSuitType.Add(temp2);

			UGridSlot * mSlot = m_HeadGrid->AddChildToGrid(temp2);

			mSlot->SetColumn(i + 1);

			mSlot->SetPadding(FMargin(60.f, 0.f, 0.f, 0.f));
		}
	}
}

//	创建类别.
UWin_ButtonSuitType* UWin_RecommendStyleUMG::CreateNewHead(FString Name, bool Last, int CategooryID)
{
	float _PadingLeft = 0.0f;

	float _DefaultPadingRight = 60.0f;

	UWin_ButtonSuitType * tempSuitType = Cast<UWin_ButtonSuitType>(UUIManager::GetInstance()->OpenUI(FString(TEXT("Btn_SuitType"))));

	tempSuitType->SetData(Name, CategooryID);
	if (Last)
	{
		_PadingLeft = 0.0f;
	}
	else
	{
		_PadingLeft = _DefaultPadingRight;
	}
	UUIManager::GetInstance()->RegisterButtonClick(tempSuitType->GetID(), tempSuitType->m_ButtonSuit, this, &UWin_RecommendStyleUMG::OnClick);

	return tempSuitType;
}

//	显示对应类别的推荐款.
void UWin_RecommendStyleUMG::ShowTypeStyle(int32 _id)
{
	m_GridPanelSuitItem2->ClearChildren();

	m_ScrollBoxItem2->ScrollToStart();

	MainModule * mModule = GModuleManager::GetInstance()->GetModule<MainModule>("MainModule");

	mModule->GetSystemStyleData(1, PageSize, { m_CurrentCatId });
}

void UWin_RecommendStyleUMG::EnterRecommend()
{
	OnClick(m_ParentId);
}

//	检查是否可以加载分页.
bool UWin_RecommendStyleUMG::CheckCanLoad()
{
	if (mIsLoading)
	{
		return false;
	}
	if (m_ResHttp.m_pageIndex == 0)
	{
		return true;
	}
	if (m_ResHttp.m_pageIndex < m_ResHttp.m_totalPage)
	{
		return true;
	}
	return false;
}

//	请求一页推荐款.
void UWin_RecommendStyleUMG::AddTypeStyle()
{
	if (CheckCanLoad())
	{
		UE_LOG(LogTemp, Log, TEXT("zhx: is loading index = %d"), m_ResHttp.m_pageIndex + 1);

		mIsLoading = true;

		MainModule * mModule = GModuleManager::GetInstance()->GetModule<MainModule>("MainModule");

		mModule->GetSystemStyleData(m_ResHttp.m_pageIndex + 1, PageSize, { m_CurrentCatId });
	}
}

//	收到服务器发给本地获取推荐款成功的消息.
void UWin_RecommendStyleUMG::OnRecommendSuitIconFinished(msg_ptr _msg)
{
	UE_LOG(LogTemp, Log, TEXT("zhx: Recommend get Suit  json finish"));

	if (_msg == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("zhx: _msg is null"));

		return;
	}

	MsgBase * mb = _msg.get(); // static_cast<MsgLocal*>(mb); 

	MsgLocal * ResMsg = _msg->ConvertMsg<MsgLocal>();

	ResHttp * resHttp = (ResHttp *)ResMsg->Content();

	m_ResHttp.m_pageIndex = resHttp->m_pageIndex;  

	m_ResHttp.m_pageSize = resHttp->m_pageSize;

	m_ResHttp.m_recordCount = resHttp->m_recordCount;

	m_ResHttp.m_totalPage = resHttp->m_totalPage;

	delete resHttp;

	CreateSelectCell();
	
	mIsLoading = false;
}

//	创建推荐款cell.
void UWin_RecommendStyleUMG::CreateSelectCell()
{
	MainModule * mMainModule = GModuleManager::GetInstance()->GetModule<MainModule>("MainModule");

	TArray<SelectCellData> mSelectCellDatas = mMainModule->GetSysSelectCellDatas_ToCreate();

	int _num = mSelectCellDatas.Num();

	int mIndex = m_AllSyles.Num();

	for (int i = 0; i < _num; ++i)
	{
		UWin_SelectCell * mTemp = (UWin_SelectCell*)UUIManager::GetInstance()->OpenUI(TEXT("SelectCell"));

		if(mTemp->IsValidLowLevel())
		{
			UGridSlot * mGridSlot = m_GridPanelSuitItem2->AddChildToGrid(mTemp);

			int index = mIndex + i;

			mGridSlot->SetRow(index % 2);

			mGridSlot->SetColumn(index / 2);

// 			mGridSlot->SetRow(i / 10);
// 
// 			mGridSlot->SetColumn((m_ResHttp.m_pageIndex - 1) * 10 + i % 10);

			mTemp->SetPadding(FMargin(0.0f, 0.0f, 15.0f, 10.f));

			mTemp->SetIconURL(mSelectCellDatas[i].m_IconUrl);

			mTemp->SetTid(mSelectCellDatas[i].m_Tid);

			mTemp->SetRid(mSelectCellDatas[i].m_Rid);

			mTemp->SetDesignType(EnumDesignType::DESIGN_BY_SYS_STYLE);

			mTemp->DownloadIconImage();

			m_AllSyles.Add(mTemp);
		}
	}
}

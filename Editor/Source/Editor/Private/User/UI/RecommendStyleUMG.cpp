// Fill out your copyright notice in the Description page of Project Settings.

#include "RecommendStyleUMG.h"
#include "UIManager.h"
#include "MsgCenter.h"
#include "GameDirector.h"
#include "GModuleManager.h"
#include "HotLoadManager.h"
#include "HomeInfoManager.h"
#include "StyleSpecialShow.h"
#include "TDesignTypeManager.h"

static int PageSize = 20;

int32 URecommendStyleUMG::m_ParentId = 1006;

bool URecommendStyleUMG::OnStart()
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
    
	if (homeInfoArr.Num() > 0)
	{
		UpdateShow();
	}

	UpdateHead();

	EnterRecommend();

	return true;
}

void URecommendStyleUMG::OnInit()
{
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, 109, this, &URecommendStyleUMG::OnRecommendSuitIconFinished);

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
		UUIManager::GetInstance()->RegisterButtonClick(1, m_BackButton, this, &URecommendStyleUMG::BackToMainPage);
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

void URecommendStyleUMG::OnRelease()
{
	ArrButtonSuitType.Empty();

	UUIManager::GetInstance()->RemoveButtonClick(m_BackButton);

	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, 109, this, &URecommendStyleUMG::OnRecommendSuitIconFinished);
}

void URecommendStyleUMG::OnSetParams(int32 _pid, FString & _content, ...)
{
}

void URecommendStyleUMG::OnTick(float InDeltaTimeSecond)
{
}

void URecommendStyleUMG::BackToMainPage(int32 id)
{
	UE_LOG(LogTemp, Log, TEXT("Recommend style umg Back to Main Page"));

	this->RemoveFromViewport();

	UGameDirector::GetInstance()->GetCurrentLogicScene()->GetCurrentUIController()->GetRootUI()->OnActivationUI();
}

void URecommendStyleUMG::OnClick(int _id)
{
	UE_LOG(LogTemp, Log, TEXT("Catid : %d"), _id);
	
	m_SpecialShowView->SetVisibility(ESlateVisibility::Hidden);

	m_ScrollBoxItem1->SetVisibility(ESlateVisibility::Hidden);

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
            m_SpecialShowView->SetVisibility(ESlateVisibility::Visible);
            m_ScrollBoxItem = m_ScrollBoxItem2;
            m_GridPanelSuitItem = m_GridPanelSuitItem2;
        }
		else
        {
            m_ScrollBoxItem1->SetVisibility(ESlateVisibility::Visible);
            m_ScrollBoxItem = m_ScrollBoxItem1;
            m_GridPanelSuitItem = m_GridPanelSuitItem1;
        }
	}
	else
	{
		m_ScrollBoxItem1->SetVisibility(ESlateVisibility::Visible);
		m_ScrollBoxItem = m_ScrollBoxItem1;
		m_GridPanelSuitItem = m_GridPanelSuitItem1;
	}

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

void URecommendStyleUMG::UpdateShow()
{
	UStyleSpecialShow * SpecialShow = (UStyleSpecialShow*)UUIManager::GetInstance()->OpenUI(TEXT("StyleSpecialShow"));
	SpecialShow->SetLogicParent(this);
	m_SpecialShowView->AddChild(SpecialShow);
}

void URecommendStyleUMG::UpdateHead()
{
	m_HeadGrid->ClearChildren();
	m_Head->ScrollToStart();
	m_GridPanelSuit->ClearChildren();

 	UButtonSuitType* temp = CreateNewHead(FString(TEXT("全部")), true, m_ParentId);
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
			
			UButtonSuitType* temp2 = CreateNewHead(_String, (i == (mArr.Num() - 1)), mArr[i]->GetTid());
				ArrButtonSuitType.Add(temp2);
				UGridSlot * mSlot = m_HeadGrid->AddChildToGrid(temp2);
				mSlot->SetColumn(i + 1);
				mSlot->SetPadding(FMargin(60.f,0.f,0.f,0.f));
		}
	}
}

UButtonSuitType* URecommendStyleUMG::CreateNewHead(FString Name, bool Last, int CategooryID)
{	float _PadingLeft = 0.0f;	float _DefaultPadingRight = 60.0f;	UButtonSuitType * tempSuitType = Cast<UButtonSuitType>(UUIManager::GetInstance()->OpenUI(FString(TEXT("Btn_SuitType"))));	
	tempSuitType->SetData(Name, CategooryID);		if (Last)
		{
			_PadingLeft = 0.0f;
		}
		else
		{
			_PadingLeft = _DefaultPadingRight;
		}
		//tempSuitType->SetPadding(FMargin(30.f, 0.0f, 0.f, 0.0f));
		UUIManager::GetInstance()->RegisterButtonClick(tempSuitType->GetID(), tempSuitType->m_ButtonSuit, this, &URecommendStyleUMG::OnClick);

	return tempSuitType;
		
}

void URecommendStyleUMG::ShowTypeStyle(int32 _id)
{
	m_GridPanelSuitItem->ClearChildren();
	m_ScrollBoxItem->ScrollToStart();
	MainModule * mModule = GModuleManager::GetInstance()->GetModule<MainModule>("MainModule");
	mModule->GetSystemStyleData(1, PageSize, { m_CurrentCatId });
}

void URecommendStyleUMG::EnterRecommend()
{
	OnClick(m_ParentId);
}

bool URecommendStyleUMG::CheckCanLoad()
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

void URecommendStyleUMG::AddTypeStyle()
{
	if (CheckCanLoad())
	{
		UE_LOG(LogTemp, Log, TEXT("zhx: is loading index = %d"), m_ResHttp.m_pageIndex + 1);
		mIsLoading = true;
		MainModule * mModule = GModuleManager::GetInstance()->GetModule<MainModule>("MainModule");
		mModule->GetSystemStyleData(m_ResHttp.m_pageIndex + 1, PageSize, { m_CurrentCatId });
	}
}

void URecommendStyleUMG::OnRecommendSuitIconFinished(msg_ptr _msg)
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

void URecommendStyleUMG::CreateSelectCell()
{
	

	MainModule * mMainModule = GModuleManager::GetInstance()->GetModule<MainModule>("MainModule");
	TArray<SelectCellData> mSelectCellDatas = mMainModule->GetSysSelectCellDatas_ToCreate();
	int _num = mSelectCellDatas.Num();

	int CurrentNum = m_AllSyles.Num();

	for (int i = 0; i < _num; ++i)
	{
		USelectCell * mTemp = (USelectCell*)UUIManager::GetInstance()->OpenUI(TEXT("SelectCell"));

		if(mTemp->IsValidLowLevel())
		{
			UGridSlot * mGridSlot = m_GridPanelSuitItem->AddChildToGrid(mTemp);
			int mIndex = CurrentNum + i;
			mGridSlot->SetRow(mIndex / 2);
			mGridSlot->SetColumn(mIndex % 2);
			if (mIndex % 2 > 0)
			{
				mTemp->SetPadding(FMargin(20.f, 0.0f, 0.f, 30.f));
			}
			else
			{
				mTemp->SetPadding(FMargin(0.f, 0.0f, 0.f, 30.f));
			}
			mTemp->SetIconURL(mSelectCellDatas[i].m_IconUrl);
			mTemp->SetTid(mSelectCellDatas[i].m_Tid);
			mTemp->SetRid(mSelectCellDatas[i].m_Rid);
			mTemp->SetDesignType(EnumDesignType::DESIGN_BY_SYS_STYLE);
			mTemp->DownloadIconImage();

			m_AllSyles.Add(mTemp);
		}
		
	}
}

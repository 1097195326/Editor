// Fill out your copyright notice in the Description page of Project Settings.

#include "Win_UMGSuitComponentDetail.h"
#include "UIManager.h"
#include "GameDirector.h"
#include "Win_DesignUMG.h"
#include "GModuleManager.h"
#include "Win_UMGMenuCell.h"
#include "TSuitComponentManager.h"

UWin_UMGSuitComponentDetail::UWin_UMGSuitComponentDetail(const FObjectInitializer& ObjectInitializer)
{
	m_Grid_Content = nullptr;

	m_BtnReturn = nullptr;
}

bool UWin_UMGSuitComponentDetail::OnStart()
{
	//UUIManager::GetInstance()->RegisterButtonClick(1, m_BtnReturn, this, &UWin_UMGSuitComponentDetail::OnReturnDetail);

	return true;
}

void UWin_UMGSuitComponentDetail::OnInit()
{
	if (UGridPanel *mTemp = Cast<UGridPanel>(GetWidgetFromName("Grid_Content")))
	{
		m_Grid_Content = mTemp;
	}
	if (UButton * mTemp = Cast<UButton>(GetWidgetFromName("Btn_Return")))
	{
		m_BtnReturn = mTemp;
	}
}

void UWin_UMGSuitComponentDetail::OnRelease()
{
	m_Grid_Content = nullptr;

	m_BtnReturn = nullptr;

	UUIManager::GetInstance()->RemoveButtonClick(m_BtnReturn);
}

void UWin_UMGSuitComponentDetail::OnTick(float InDeltaTimeSecond)
{

}

void UWin_UMGSuitComponentDetail::OnActivationUI()
{
	this->SetVisibility(ESlateVisibility::Visible);
}

void UWin_UMGSuitComponentDetail::OnBackstageUI()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UWin_UMGSuitComponentDetail::InitData(TArray<SuitComponentT* > &_CompTidArray)
{
	DesignModule *mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

	if (mDesignModule !=nullptr)
	{
		m_MenuCell_Map.Empty();

		m_Grid_Content->ClearChildren();

		for (size_t i = 0; i < _CompTidArray.Num(); ++i)
		{
			UWin_UMGMenuCell* mTemp = (UWin_UMGMenuCell*)UUIManager::GetInstance()->OpenUI(TEXT("UMG_MenuCell"));

			SuitComponentT *mSuitCompT=TSuitComponentManager::GetInstance()->GetTemplateByPID<SuitComponentT>(_CompTidArray[i]->GetTid());

			mTemp->SetCellType(EnumCellType::T_SUIT_COMPONENT_DETAIL_CELL);

			mTemp->SetTid(mSuitCompT->GetTid());

			mTemp->SetIconUrl(mSuitCompT->m_IconUrl);

			mTemp->SetParent(this);

			mTemp->SetIconTexture2D();

			m_MenuCell_Map.Add(mTemp->GetTid(), mTemp);

			UGridSlot * mGridSlot = this->m_Grid_Content->AddChildToGrid(mTemp);

			mGridSlot->SetRow(i / 2);

			mGridSlot->SetColumn(i % 2);
		}
	}
}

void UWin_UMGSuitComponentDetail::SetUniqueSelected(int32 _CompTid)
{
	TArray<UWin_UMGMenuCell*> mMenuCells ;

	m_MenuCell_Map.GenerateValueArray(mMenuCells);

	for (size_t i = 0; i < mMenuCells.Num(); ++i)
	{
		mMenuCells[i]->HidenSelectedIcon();

		mMenuCells[i]->m_Btn_Slice->SetVisibility(ESlateVisibility::Hidden);
	}

	UWin_UMGMenuCell *mCurrentTemp = nullptr;

	if (m_MenuCell_Map.Contains(_CompTid))
	{
		mCurrentTemp = m_MenuCell_Map[_CompTid];
	}

	if (mCurrentTemp!=nullptr)
	{
		mCurrentTemp->ShowSelectedIcon();

		//mCurrentTemp->m_Btn_Slice->SetVisibility(ESlateVisibility::Visible);

		m_CurSelectedTid = _CompTid;
	}
}

void UWin_UMGSuitComponentDetail::SetParentTid(int32 _Tid)
{
	m_Parent = _Tid;
}

void UWin_UMGSuitComponentDetail::OnReturnDetail(int32 _pid)
{
	LogicDesignScene *mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();
	
	UWin_DesignUMG *mDesignUMG = nullptr;
	
	if (mLogicDesignScene != nullptr)
	{
		mDesignUMG = (UWin_DesignUMG *)(mLogicDesignScene->GetCurrentUIController()->GetRootUI());
	}
	
	if (mDesignUMG != nullptr)
	{
		mDesignUMG->HidenDetailPage();
		
		mDesignUMG->ShowSuitPartMenu();

		//相机返回到SuitPartMenu.
		mDesignUMG->OnReturnToSuitPartMenu();
	}
}

int32 UWin_UMGSuitComponentDetail::GetParentTid()
{
	return m_Parent;
}

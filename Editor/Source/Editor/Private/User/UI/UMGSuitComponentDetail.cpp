// Fill out your copyright notice in the Description page of Project Settings.

#include "UMGSuitComponentDetail.h"
#include "GModuleManager.h"
#include "UMGMenuCell.h"
#include "TSuitComponentManager.h"
#include "UIManager.h"
#include "GameDirector.h"
#include "DesignUMG.h"


UUMGSuitComponentDetail::UUMGSuitComponentDetail(const FObjectInitializer& ObjectInitializer)
{
	m_Grid_Content = nullptr;
}

bool UUMGSuitComponentDetail::OnStart()
{
	//Super::OnStart();
	bool mRet = false;
	mRet = true;
	return mRet;
}

void UUMGSuitComponentDetail::OnInit()
{
	//Super::OnInit();
	if (UGridPanel *mTemp = Cast<UGridPanel>(GetWidgetFromName("Grid_Content")))
	{
		m_Grid_Content = mTemp;
	}
}

void UUMGSuitComponentDetail::OnRelease()
{
	//Super::OnRelease();
	m_Grid_Content = nullptr;
}

void UUMGSuitComponentDetail::OnTick(float InDeltaTimeSecond)
{

}

void UUMGSuitComponentDetail::OnActivationUI()
{
	this->SetVisibility(ESlateVisibility::Visible);
}

void UUMGSuitComponentDetail::OnBackstageUI()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UUMGSuitComponentDetail::InitData(TArray<SuitComponentT* > &_CompTidArray)
{
	DesignModule *mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

	if (mDesignModule !=nullptr)
	{
		m_MenuCell_Map.Empty();
		m_Grid_Content->ClearChildren();

		for (size_t i = 0; i < _CompTidArray.Num(); ++i)
		{
			UUMGMenuCell* mTemp = (UUMGMenuCell*)UUIManager::GetInstance()->OpenUI(TEXT("UMG_MenuCell"));
			SuitComponentT *mSuitCompT=TSuitComponentManager::GetInstance()->GetTemplateByPID<SuitComponentT>(_CompTidArray[i]->GetTid());
			mTemp->SetCellType(EnumCellType::T_SUIT_COMPONENT_DETAIL_CELL);
			mTemp->SetTid(mSuitCompT->GetTid());
			mTemp->SetIconUrl(mSuitCompT->m_IconUrl);
			mTemp->SetParent(this);
			mTemp->SetIconTexture2D();
			m_MenuCell_Map.Add(mTemp->GetTid(), mTemp);

			UGridSlot * mGridSlot = this->m_Grid_Content->AddChildToGrid(mTemp);
			mGridSlot->SetRow(i / MAX_CONTNET_COLUME_NUM);
			mGridSlot->SetColumn(i % MAX_CONTNET_COLUME_NUM);
		}
	}
}





void UUMGSuitComponentDetail::SetUniqueSelected(int32 _CompTid)
{
	TArray<UUMGMenuCell*> mMenuCells ;
	m_MenuCell_Map.GenerateValueArray(mMenuCells);
	for (size_t i = 0; i < mMenuCells.Num(); ++i)
	{
		mMenuCells[i]->HidenSelectedIcon();
	}
	UUMGMenuCell *mCurrentTemp = nullptr;
	if (m_MenuCell_Map.Contains(_CompTid))
	{
		mCurrentTemp = m_MenuCell_Map[_CompTid];
	}
	if (mCurrentTemp!=nullptr)
	{
		mCurrentTemp->ShowSelectedIcon();
		m_CurSelectedTid = _CompTid;
	}
}

void UUMGSuitComponentDetail::SetParentTid(int32 _Tid)
{
	m_Parent = _Tid;
}

int32 UUMGSuitComponentDetail::GetParentTid()
{
	return m_Parent;
}




// Fill out your copyright notice in the Description page of Project Settings.

#include "Win_UMGSuitSliceMenu.h"
#include "GModuleManager.h"
#include "SuitSliceT.h"
#include "UIManager.h"
#include "Win_UMGMenuCell.h"
#include "Win_DesignUMG.h"
UWin_UMGSuitSliceMenu::UWin_UMGSuitSliceMenu(const FObjectInitializer& ObjectInitializer)
{

}

bool UWin_UMGSuitSliceMenu::OnStart()
{
	//Super::OnStart();
	bool mRet = false;

	mRet = true;
	return mRet;
}

void UWin_UMGSuitSliceMenu::OnInit()
{
	if (UVerticalBox *mTemp = Cast<UVerticalBox>(GetWidgetFromName("SuitSlice_Content")))
	{
		m_VBox_SuitSlice_Content = mTemp;
	}
	if (UButton * mButton = Cast<UButton>(GetWidgetFromName("Btn_Return")))
	{
		m_Btn_Return = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_Return, this, &UWin_UMGSuitSliceMenu::OnReturnButtonClick);
	}
}

void UWin_UMGSuitSliceMenu::OnRelease()
{
	//Super::OnRelease();

}

void UWin_UMGSuitSliceMenu::OnSetParams(int32 _pid, FString & _content, ...)
{

}

void UWin_UMGSuitSliceMenu::OnTick(float InDeltaTimeSecond)
{
}

void UWin_UMGSuitSliceMenu::OnActivationUI()
{
	this->SetVisibility(ESlateVisibility::Visible);
}

void UWin_UMGSuitSliceMenu::OnBackstageUI()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UWin_UMGSuitSliceMenu::SetUniqueSelected(int32 _SliceTid)
{
	TArray<UWin_UMGMenuCell*> mMenuCells;
	m_MenuCell_Map.GenerateValueArray(mMenuCells);
	for (size_t i = 0; i < mMenuCells.Num(); ++i)
	{
		mMenuCells[i]->HidenSelectedIcon();
	}
	m_MenuCell_Map[_SliceTid]->ShowSelectedIcon();
	m_CurrentSelectedSlice_Tid = _SliceTid;
}

void UWin_UMGSuitSliceMenu::SetUniqueHiddenSelected(int32 _SliceTid)
{
	TArray<UWin_UMGMenuCell*> mMenuCells;
	m_MenuCell_Map.GenerateValueArray(mMenuCells);
	for (size_t i = 0; i < mMenuCells.Num(); ++i)
	{
		mMenuCells[i]->HidenSelectedIcon();
	}
}

void UWin_UMGSuitSliceMenu::InitData(int32 _SuitCompTid)
{
	
	DesignModule *mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

	if (mDesignModule != nullptr)
	{
		m_VBox_SuitSlice_Content->ClearChildren();
		m_MenuCell_Map.Empty();
		m_SuitComp_Tid = _SuitCompTid;
		TArray<SuitSliceT*> mSuitSliceArray = mDesignModule->GetSuitSliceArray(_SuitCompTid);
		for (size_t i = 0; i < mSuitSliceArray.Num(); ++i)
		{
			UWin_UMGMenuCell* mTemp = (UWin_UMGMenuCell*)UUIManager::GetInstance()->OpenUI(TEXT("UMG_MenuCell"));
			mTemp->SetCellType(EnumCellType::T_SUIT_SLICE_MENU_CELL);
			mTemp->SetTid(mSuitSliceArray[i]->GetTid());
			//设置ICON.
			mTemp->SetIconUrl(mSuitSliceArray[i]->m_IconUrl);
			mTemp->SetIconTexture2D();
			mTemp->SetParent(this);
			m_VBox_SuitSlice_Content->AddChild(mTemp);
			m_MenuCell_Map.Add(mTemp->GetTid(), mTemp);
		}
	}
}

int32 UWin_UMGSuitSliceMenu::GetComponentTid()
{
	return m_SuitComp_Tid;
}

int32 UWin_UMGSuitSliceMenu::GetCurSelectedSliceTid()
{
	return m_CurrentSelectedSlice_Tid;
}



void UWin_UMGSuitSliceMenu::OnReturnButtonClick(int32 _param)
{
	OnBackstageUI();
	LogicDesignScene *mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();
	UWin_DesignUMG *mDesignUMG = nullptr;
	if (mLogicDesignScene != nullptr)
	{
		mDesignUMG = (UWin_DesignUMG *)(mLogicDesignScene->GetCurrentUIController()->GetRootUI());
	}
	if (mDesignUMG != nullptr)
	{
		//重置片模型高亮.
		mLogicDesignScene->ResetAllSliceOutLine();
		mDesignUMG->ShowSuiComponentMenu();
		mDesignUMG->HidenDetailPage();

		//相机返回到CompMenu.
		mDesignUMG->OnReturnToCompMenu();
	}
}


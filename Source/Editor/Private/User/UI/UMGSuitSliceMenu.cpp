// Fill out your copyright notice in the Description page of Project Settings.

#include "UMGSuitSliceMenu.h"
#include "GModuleManager.h"
#include "SuitSliceT.h"
#include "UIManager.h"
#include "UMGMenuCell.h"
UUMGSuitSliceMenu::UUMGSuitSliceMenu(const FObjectInitializer& ObjectInitializer)
{

}

bool UUMGSuitSliceMenu::OnStart()
{
	//Super::OnStart();
	bool mRet = false;

	mRet = true;
	return mRet;
}

void UUMGSuitSliceMenu::OnInit()
{
	if (UVerticalBox *mTemp = Cast<UVerticalBox>(GetWidgetFromName("SuitSlice_Content")))
	{
		m_VBox_SuitSlice_Content = mTemp;
	}
	if (UButton * mButton = Cast<UButton>(GetWidgetFromName("Btn_Return")))
	{
		m_Btn_Return = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_Return, this, &UUMGSuitSliceMenu::OnReturnButtonClick);
	}
}

void UUMGSuitSliceMenu::OnRelease()
{
	//Super::OnRelease();

}

void UUMGSuitSliceMenu::OnSetParams(int32 _pid, FString & _content, ...)
{

}

void UUMGSuitSliceMenu::OnTick(float InDeltaTimeSecond)
{
}

void UUMGSuitSliceMenu::OnActivationUI()
{
	this->SetVisibility(ESlateVisibility::Visible);
}

void UUMGSuitSliceMenu::OnBackstageUI()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UUMGSuitSliceMenu::SetUniqueSelected(int32 _SliceTid)
{
	TArray<UUMGMenuCell*> mMenuCells;
	m_MenuCell_Map.GenerateValueArray(mMenuCells);
	for (size_t i = 0; i < mMenuCells.Num(); ++i)
	{
		mMenuCells[i]->HidenSelectedIcon();
	}
	m_MenuCell_Map[_SliceTid]->ShowSelectedIcon();
	m_CurrentSelectedSlice_Tid = _SliceTid;
}

void UUMGSuitSliceMenu::SetUniqueHiddenSelected(int32 _SliceTid)
{
	TArray<UUMGMenuCell*> mMenuCells;
	m_MenuCell_Map.GenerateValueArray(mMenuCells);
	for (size_t i = 0; i < mMenuCells.Num(); ++i)
	{
		mMenuCells[i]->HidenSelectedIcon();
	}
}

void UUMGSuitSliceMenu::InitData(int32 _SuitCompTid)
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
			UUMGMenuCell* mTemp = (UUMGMenuCell*)UUIManager::GetInstance()->OpenUI(TEXT("UMG_MenuCell"));
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

int32 UUMGSuitSliceMenu::GetComponentTid()
{
	return m_SuitComp_Tid;
}

int32 UUMGSuitSliceMenu::GetCurSelectedSliceTid()
{
	return m_CurrentSelectedSlice_Tid;
}



void UUMGSuitSliceMenu::OnReturnButtonClick(int32 _param)
{
	OnBackstageUI();
	LogicDesignScene *mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();
	UDesignUMG *mDesignUMG = nullptr;
	if (mLogicDesignScene != nullptr)
	{
		mDesignUMG = (UDesignUMG *)(mLogicDesignScene->GetCurrentUIController()->GetRootUI());
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


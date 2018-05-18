#include "Win_UMGSuitPartMenu.h"
#include "DesignModule.h"
#include "GModuleManager.h"
#include "SuitPartT.h"
#include "GameDirector.h"
#include "TSuitComponentManager.h"
#include "Win_UMGBatchCell.h"
UWin_UMGSuitPartMenu::UWin_UMGSuitPartMenu(const FObjectInitializer& ObjectInitializer)
{
	
}

bool UWin_UMGSuitPartMenu::OnStart()
{
	bool mRet = false;
	

	InitData();
	InitBatchData();
	mRet = true;
	return true;
}

void UWin_UMGSuitPartMenu::OnInit()
{
	//Super::OnInit();

	//初始化UI.
	if (UVerticalBox *mTemp = Cast<UVerticalBox>(GetWidgetFromName("Batch_Content")))
	{
		m_VBox_Batch_Content = mTemp;
	}
	if (UVerticalBox *mTemp = Cast<UVerticalBox>(GetWidgetFromName("SuitPart_Content")))
	{
		m_VBox_SuitPart_Content = mTemp;
	}
	if (UButton * mButton=Cast<UButton>(GetWidgetFromName("Btn_Return")))
	{
		m_Btn_Return = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_Return, this, &UWin_UMGSuitPartMenu::OnReturnButtonClick);
	}
}

void UWin_UMGSuitPartMenu::OnRelease()
{

}

void UWin_UMGSuitPartMenu::OnLoad()
{



}

void UWin_UMGSuitPartMenu::OnSetParams(int32 _pid, FString & _content, ...)
{

}

void UWin_UMGSuitPartMenu::OnTick(float InDeltaTimeSecond)
{

}

void UWin_UMGSuitPartMenu::OnActivationUI()
{
	this->SetVisibility(ESlateVisibility::Visible);
}

void UWin_UMGSuitPartMenu::OnBackstageUI()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UWin_UMGSuitPartMenu::InitData()
{
	LogicDesignScene *mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();
	DesignModule *mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

	mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

	mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();

		m_VBox_SuitPart_Content->ClearChildren();
		TArray<SuitPartT*> mSuitPartTArray = mDesignModule->GetSuitPartArray();
		for (size_t i = 0; i < mSuitPartTArray.Num(); ++i)
		{
			int32 mCompTid = mLogicDesignScene->GetDefaultComponentTid(mSuitPartTArray[i]->GetTid(), EnumComponentType::PART_COMPONENT);
			SuitComponentT * mTempComp = TSuitComponentManager::GetInstance()->GetTemplateByPID<SuitComponentT>(mCompTid);
			UWin_UMGMenuCell* mTempCell = (UWin_UMGMenuCell*)UUIManager::GetInstance()->OpenUI(TEXT("UMG_MenuCell"));
			mTempCell->SetCellType(EnumCellType::T_SUIT_PART_MENU_CELL);
			mTempCell->SetTid(mSuitPartTArray[i]->GetTid());
			mTempCell->SetIconUrl(mTempComp->m_IconUrl);
			mTempCell->SetParent(this);
			mTempCell->SetIconTexture2D();
			m_VBox_SuitPart_Content->AddChild(mTempCell);
		}
}

void UWin_UMGSuitPartMenu::InitBatchData()
{
	LogicDesignScene *mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();
	DesignModule *mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

	mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

	mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();
	//清空数据.
	m_VBox_Batch_Content->ClearChildren();
	m_BatchCell_Array.Empty();
	
	//初始化Batch Cell.
	TArray<UsedFabricStruct> mArray;
	mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

	mLogicDesignScene->GetAllFabricInfoByTid(mDesignModule->GetDesignData()->m_designTID,mArray,EnumComponentType::STYLE_COMPONENT);
	for (int32 i=0;i<mArray.Num();++i)
	{
		UWin_UMGBatchCell* mTempCell = (UWin_UMGBatchCell*)UUIManager::GetInstance()->OpenUI(TEXT("UMG_BatchCell"));
		mTempCell->SetUsedFabricStruct(mArray[i]);
		mTempCell->SetParent(this);
		mTempCell->SetCellType(EnumBatchCellType::BATCH_CELL_PART);
		if (mArray[i].m_colorTid!=0)
		{
			mTempCell->SetColor();
		}
		else
		{
			mTempCell->SetTexture2D();
		}
		m_VBox_Batch_Content->AddChild(mTempCell);
		m_BatchCell_Array.Add(mTempCell);
	}
}

void UWin_UMGSuitPartMenu::OnReturnButtonClick(int32 _param)
{

}

void UWin_UMGSuitPartMenu::SetUniqueSelectedBatchIcon(UsedFabricStruct _UsedFabricStruct)
{
	for (int32 i = 0; i < m_BatchCell_Array.Num(); ++i)
	{
		if (_UsedFabricStruct== m_BatchCell_Array[i]->m_UsedFabric_Struct)
		{
			m_BatchCell_Array[i]->ShowSelectedIcon();
		}
		else
		{
			m_BatchCell_Array[i]->HiddenSelectedIcon();
		}
	}
}

void UWin_UMGSuitPartMenu::SetUniqueHiddenSelectedBatchIcon(UsedFabricStruct _UsedFabricStruct)
{
	for (int32 i = 0; i < m_BatchCell_Array.Num(); ++i)
	{
		if (_UsedFabricStruct == m_BatchCell_Array[i]->m_UsedFabric_Struct)
		{
			m_BatchCell_Array[i]->HiddenSelectedIcon();
		}
	}
}

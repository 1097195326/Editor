#include "UMGSuitPartMenu.h"
#include "DesignModule.h"
#include "GModuleManager.h"
#include "SuitPartT.h"
#include "GameDirector.h"
#include "TSuitComponentManager.h"
#include "UMGBatchCell.h"
UUMGSuitPartMenu::UUMGSuitPartMenu(const FObjectInitializer& ObjectInitializer)
{
	
}

bool UUMGSuitPartMenu::OnStart()
{
	bool mRet = false;
	

	InitData();
	InitBatchData();
	mRet = true;
	return true;
}

void UUMGSuitPartMenu::OnInit()
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
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_Return, this, &UUMGSuitPartMenu::OnReturnButtonClick);
	}
}

void UUMGSuitPartMenu::OnRelease()
{

}

void UUMGSuitPartMenu::OnLoad()
{



}

void UUMGSuitPartMenu::OnSetParams(int32 _pid, FString & _content, ...)
{

}

void UUMGSuitPartMenu::OnTick(float InDeltaTimeSecond)
{

}

void UUMGSuitPartMenu::OnActivationUI()
{
	this->SetVisibility(ESlateVisibility::Visible);
}

void UUMGSuitPartMenu::OnBackstageUI()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UUMGSuitPartMenu::InitData()
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
			UUMGMenuCell* mTempCell = (UUMGMenuCell*)UUIManager::GetInstance()->OpenUI(TEXT("UMG_MenuCell"));
			mTempCell->SetCellType(EnumCellType::T_SUIT_PART_MENU_CELL);
			mTempCell->SetTid(mSuitPartTArray[i]->GetTid());
			mTempCell->SetIconUrl(mTempComp->m_IconUrl);
			mTempCell->SetParent(this);
			mTempCell->SetIconTexture2D();
			m_VBox_SuitPart_Content->AddChild(mTempCell);
		}
}

void UUMGSuitPartMenu::InitBatchData()
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
		UUMGBatchCell* mTempCell = (UUMGBatchCell*)UUIManager::GetInstance()->OpenUI(TEXT("UMG_BatchCell"));
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

void UUMGSuitPartMenu::OnReturnButtonClick(int32 _param)
{

}

void UUMGSuitPartMenu::SetUniqueSelectedBatchIcon(UsedFabricStruct _UsedFabricStruct)
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

void UUMGSuitPartMenu::SetUniqueHiddenSelectedBatchIcon(UsedFabricStruct _UsedFabricStruct)
{
	for (int32 i = 0; i < m_BatchCell_Array.Num(); ++i)
	{
		if (_UsedFabricStruct == m_BatchCell_Array[i]->m_UsedFabric_Struct)
		{
			m_BatchCell_Array[i]->HiddenSelectedIcon();
		}
	}
}

#include "UMGSuitComponentMenu.h"
#include "LogicDesignScene.h"
#include "DesignUMG.h"
#include "TSuitComponentManager.h"
#include "TSuitStitchManager.h"
#include "TSuitAccessoriesManager.h"
#include "SuitComponentT.h"
#include "GModuleManager.h"
#include "GameDirector.h"
#include "DesignModule.h"
#include "BaseComponent.h"

UUMGSuitComponentMenu::UUMGSuitComponentMenu(const FObjectInitializer& ObjectInitializer)
{

}

bool UUMGSuitComponentMenu::OnStart()
{
	//Super::OnStart();
	bool mRet = false;
	//获取module.

	mRet = true;
	return mRet;
	return true;
}

void UUMGSuitComponentMenu::OnInit()
{
	//Super::OnInit();

	//初始化UI.
	if (UVerticalBox *mTemp = Cast<UVerticalBox>(GetWidgetFromName("Batch_Content")))
	{
		m_VBox_Batch_Content = mTemp;
	}
	if (UVerticalBox *mTemp = Cast<UVerticalBox>(GetWidgetFromName("SuitComponent_Content")))
	{
		m_VBox_SuitComponent_Content = mTemp;
	}
	if (UButton * mButton = Cast<UButton>(GetWidgetFromName("Btn_Return")))
	{
		m_Btn_Return = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_Return, this, &UUMGSuitComponentMenu::OnReturnButtonClick);
	}
}

void UUMGSuitComponentMenu::OnRelease()
{
	Super::OnRelease();

}

void UUMGSuitComponentMenu::OnSetParams(int32 _pid, FString & _content, ...)
{

}

void UUMGSuitComponentMenu::OnTick(float InDeltaTimeSecond)
{

}

void UUMGSuitComponentMenu::OnActivationUI()
{
	this->SetVisibility(ESlateVisibility::Visible);
	
}

void UUMGSuitComponentMenu::OnBackstageUI()
{
	this->SetVisibility(ESlateVisibility::Hidden);
	
}



void UUMGSuitComponentMenu::InitData(int32 _SuitPartTid)
{
	//清空数据.
	m_VBox_SuitComponent_Content->ClearChildren();
	m_MenuCell_Array.Empty();
	m_SuitPart_Tid = _SuitPartTid;
	LogicDesignScene *mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();

	//获取默认compTid.
	int32 mCompTid = mLogicDesignScene->GetDefaultComponentTid(_SuitPartTid,EnumComponentType::PART_COMPONENT);
	//初始化ComponentCell.
	{
		SuitComponentT * mCompT = TSuitComponentManager::GetInstance()->GetTemplateByPID<SuitComponentT>(mCompTid);
		UUMGMenuCell* mCompMenuCell = (UUMGMenuCell*)UUIManager::GetInstance()->OpenUI(TEXT("UMG_MenuCell"));
		mCompMenuCell->SetCellType(EnumCellType::T_SUIT_COMPONENT_MENU_CELL);
		mCompMenuCell->SetTid(m_SuitPart_Tid);
		mCompMenuCell->SetIconUrl(mCompT->m_IconUrl);
		mCompMenuCell->SetParent(this);
		mCompMenuCell->SetIconTexture2D();

		mCompMenuCell->ShowSelectedIcon();

		
		m_VBox_SuitComponent_Content->AddChild(mCompMenuCell);
		m_MenuCell_Array.Add(mCompMenuCell);
	}

	DesignModule *mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

	//初始化Accessories.
	TArray<SuitAccessoriesT *> mAccT_Array= mDesignModule->GetSuitAccessoriesArray(mCompTid);

	for (int32 i = 0; i < mAccT_Array.Num(); ++i)
	{
		//获取默认辅料.
		int32 mDefaultCompTid = mLogicDesignScene->GetDefaultComponentTid(mAccT_Array[i]->GetTid(), EnumComponentType::ACCESSORIES_COMPONENT);
		SuitComponentT *mComponent=TSuitComponentManager::GetInstance()->GetTemplateByPID<SuitComponentT>(mDefaultCompTid);

		UUMGMenuCell* mCompMenuCell = (UUMGMenuCell*)UUIManager::GetInstance()->OpenUI(TEXT("UMG_MenuCell"));
		mCompMenuCell->SetCellType(EnumCellType::T_SUIT_ACCESSORIES_MENU_CELL);
		mCompMenuCell->SetTid(mAccT_Array[i]->GetTid());
		mCompMenuCell->SetIconUrl(mComponent->m_IconUrl);
		mCompMenuCell->SetParent(this);
		mCompMenuCell->SetIconTexture2D();
		m_VBox_SuitComponent_Content->AddChild(mCompMenuCell);
		m_MenuCell_Array.Add(mCompMenuCell);
	}

	//初始化Stitch.
	//TArray<SuitStitchT *> mStitch_Array = mDesignModule->GetSuitStitchArray(mCompTid);
	////int32 GetDefaultComponentTid(m_SuitPart_Tid);
	//for (int32 i = 0; i < mStitch_Array.Num(); ++i)
	//{
	//	//获取默认线迹.
	//	int32 mDefaultCompTid = mLogicDesignScene->GetDefaultComponentTid(mStitch_Array[i]->GetTid(), EnumComponentType::STITCH_COMPONENT);
	//	SuitComponentT *mComponent = TSuitComponentManager::GetInstance()->GetTemplateByPID<SuitComponentT>(mDefaultCompTid);

	//	UUMGMenuCell* mCompMenuCell = (UUMGMenuCell*)UUIManager::GetInstance()->OpenUI("UMG_MenuCell");
	//	mCompMenuCell->SetCellType(EnumCellType::T_SUIT_STITCH_MENU_CELL);
	//	mCompMenuCell->SetTid(mStitch_Array[i]->GetTid());
	//	mCompMenuCell->SetIconUrl(mComponent->m_IconUrl);
	//	mCompMenuCell->SetParent(this);
	//	mCompMenuCell->SetIconTexture2D();
	//	m_VBox_SuitComponent_Content->AddChild(mCompMenuCell);
	//	m_MenuCell_Array.Add(mCompMenuCell);
	//}
}

void UUMGSuitComponentMenu::InitBatchData()
{
	//清空数据.
	m_VBox_Batch_Content->ClearChildren();
	m_BatchCell_Array.Empty();
	LogicDesignScene *mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();

	//获取部位cid.
	UDesignUMG *mDesignUMG = (UDesignUMG *)(mLogicDesignScene->GetCurrentUIController()->GetRootUI());

	DesignModule *mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

	//初始化花型Batch Cell.
	TArray<UsedFabricStruct> mArray;
	mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

	mLogicDesignScene->GetAllFabricInfoByTid(m_SuitPart_Tid, mArray,EnumComponentType::PART_COMPONENT);
	for (int32 i = 0; i < mArray.Num(); ++i)
	{
		UUMGBatchCell* mTempCell = (UUMGBatchCell*)UUIManager::GetInstance()->OpenUI(TEXT("UMG_BatchCell"));
		mTempCell->SetUsedFabricStruct(mArray[i]);
		mTempCell->SetParent(this);
		mTempCell->SetCellType(EnumBatchCellType::BATCH_CELL_COMPONENT);
		if (mArray[i].m_colorTid != 0)
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

void UUMGSuitComponentMenu::OnReturnButtonClick(int32 _param)
{
	OnBackstageUI();
	LogicDesignScene *mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();
	UDesignUMG *mDesignUMG = nullptr;
	if (mLogicDesignScene!=nullptr)
	{
		mDesignUMG = (UDesignUMG *)(mLogicDesignScene->GetCurrentUIController()->GetRootUI());
	}
	if (mDesignUMG!=nullptr)
	{
		mDesignUMG->HidenDetailPage();
		mDesignUMG->ShowSuitPartMenu();

		//相机返回到SuitPartMenu.
		mDesignUMG->OnReturnToSuitPartMenu();
	}
}

int32 UUMGSuitComponentMenu::GetSuitPartTid()
{
	return m_SuitPart_Tid;
}

void UUMGSuitComponentMenu::SetUniqueSelectedIcon(int32 _Tid)
{
	for (size_t i = 0; i < m_MenuCell_Array.Num();++i)
	{
		if (m_MenuCell_Array[i]->GetTid()== _Tid)
		{
			m_MenuCell_Array[i]->ShowSelectedIcon();
		}
		else
		{
			m_MenuCell_Array[i]->HidenSelectedIcon();
		}
		
	}
}

void UUMGSuitComponentMenu::SetUniqueHiddenSelectedIcon(int32 _Tid)
{
	for (size_t i = 0; i < m_MenuCell_Array.Num(); ++i)
	{
		if (m_MenuCell_Array[i]->GetTid() == _Tid)
		{
			m_MenuCell_Array[i]->HidenSelectedIcon();
		}
	}
}

void UUMGSuitComponentMenu::SetUniqueSelectedBatchIcon(UsedFabricStruct _UsedFabricStruct)
{
	for (int32 i = 0; i < m_BatchCell_Array.Num(); ++i)
	{
		if (_UsedFabricStruct == m_BatchCell_Array[i]->m_UsedFabric_Struct)
		{
			m_BatchCell_Array[i]->ShowSelectedIcon();
		}
		else
		{
			m_BatchCell_Array[i]->HiddenSelectedIcon();
		}
	}
}

void UUMGSuitComponentMenu::SetUniqueHiddenSelectedBatchIcon(UsedFabricStruct _UsedFabricStruct)
{
	for (int32 i = 0; i < m_BatchCell_Array.Num(); ++i)
	{
		if (_UsedFabricStruct == m_BatchCell_Array[i]->m_UsedFabric_Struct)
		{
			m_BatchCell_Array[i]->HiddenSelectedIcon();
		}
	}
}



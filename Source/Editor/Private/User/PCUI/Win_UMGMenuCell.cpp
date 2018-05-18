#include "Win_UMGMenuCell.h"
#include "Win_UMGSuitPartMenu.h"
#include "Win_UMGSuitComponentDetail.h"
#include "Win_DesignUMG.h"
#include "LoadAssetTool.h"
#include "GModuleManager.h"
#include "SuitAccessoriesT.h"
#include "TSuitAccessoriesManager.h"
#include "TSuitComponentManager.h"
#include "TSuitStitchManager.h"
#include "SuitComponentT.h"
#include "SuitStitchT.h"

UWin_UMGMenuCell::UWin_UMGMenuCell(const FObjectInitializer& ObjectInitializer)
{
	m_Image_select = nullptr;
	m_Btn_Select = nullptr;
	m_Image_ICON = nullptr;
	m_Parent = nullptr;
}

bool UWin_UMGMenuCell::OnStart()
{
	bool mRet = false;

	mRet = true;

	return mRet;
}

void UWin_UMGMenuCell::OnInit()
{
	if (UImage *mTemp = Cast<UImage>(GetWidgetFromName("Image_select")))
	{
		m_Image_select = mTemp;
	}
	if (UImage *mTemp = Cast<UImage>(GetWidgetFromName("Image_ICON")))
	{
		m_Image_ICON = mTemp;
	}
	if (UButton *mTemp = Cast<UButton>(GetWidgetFromName("Btn_Select")))
	{
		m_Btn_Select = mTemp;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_Select, this, &UWin_UMGMenuCell::OnButtonClick);
	}
	if (UButton *mTemp = Cast<UButton>(GetWidgetFromName("Btn_Slice")))
	{
		m_Btn_Slice = mTemp;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_Slice, this, &UWin_UMGMenuCell::OnButtonSliceClick);
	}
}

void UWin_UMGMenuCell::OnRelease()
{
	m_Image_select = nullptr;

	m_Btn_Select = nullptr;

	m_Image_ICON = nullptr;

	m_Parent = nullptr;

	UUIManager::GetInstance()->RemoveButtonClick(m_Btn_Select);

	UUIManager::GetInstance()->RemoveButtonClick(m_Btn_Slice);

}

void UWin_UMGMenuCell::OnSetParams(int32 _pid, FString & _content, ...)
{

}

void UWin_UMGMenuCell::OnTick(float InDeltaTimeSecond)
{

}

void UWin_UMGMenuCell::OnActivationUI()
{

}

void UWin_UMGMenuCell::OnButtonClick(int32 _param)
{
	switch (m_Cell_Type)
	{
	case T_SUIT_PART_MENU_CELL:
	{
		if (m_Parent != nullptr)
		{
			m_Parent->OnBackstageUI();
		}

		LogicDesignScene *mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();

		UWin_DesignUMG *mDesignUMG = nullptr;

		if (mLogicDesignScene != nullptr)
		{
			mDesignUMG = (UWin_DesignUMG *)(mLogicDesignScene->GetCurrentUIController()->GetRootUI());
		}

		if (mDesignUMG != nullptr)
		{
			//隐藏DetailPage.
			mDesignUMG->HidenDetailPage();

			//初始化CompMenu.
			int32 mCompTid = mLogicDesignScene->GetDefaultComponentTid(m_Tid, EnumComponentType::PART_COMPONENT);
			mDesignUMG->InitSuitCompMenu(m_Tid);
			mDesignUMG->InitSuitCompBatchMenu();
			mDesignUMG->ShowSuiComponentMenu();

			DesignModule* mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

			//获取SuitCompArray.
			TArray<SuitComponentT* > mSuitCompTArray = mDesignModule->GetSuitComponentArray(m_Tid);
			//过滤AB款后的数组.
			TArray<SuitComponentT* > mSuitCompAdpterABTArray;
			//A,B款过滤.
			int32 mAdapterType = mLogicDesignScene->GetAdapterType(m_Tid);
			//等于0时不用过滤.
			if (mAdapterType != 0)
			{
				for (size_t i = 0; i < mSuitCompTArray.Num(); ++i)
				{
					//相等时显示出来.
					if (mSuitCompTArray[i]->m_AdapterType == mAdapterType)
					{
						mSuitCompAdpterABTArray.Add(mSuitCompTArray[i]);
					}
				}
			}
			else
			{
				mSuitCompAdpterABTArray = mSuitCompTArray;
			}

			mDesignUMG->m_ComponentTid = m_Tid;	//保存part点进去的部件的Tid,目的设置高亮.

			mDesignUMG->m_UMG_SuitPartMenu->SetUniqueHiddenSelectedBatchIcon(mDesignUMG->m_UsedFabric_Struct);

			//初始化CompDetail.
			mDesignUMG->InitSuitCompDetail(mSuitCompAdpterABTArray);
			//设置父对象.
			mDesignUMG->m_ComponentDetail->SetParentTid(m_Tid);
			//设置默认选择高亮.
			mDesignUMG->m_ComponentDetail->SetUniqueSelected(mCompTid);
			//显示CompDetail.
			mDesignUMG->ShowComponentDetail();

			//初始化SliceMenu.
			mDesignUMG->InitSuitSliceMenu(mCompTid);
		}
		break;
	}
	case T_SUIT_COMPONENT_MENU_CELL:
	{
		LogicDesignScene *mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();
		
		UWin_DesignUMG *mDesignUMG = nullptr;

		if (mLogicDesignScene != nullptr)
		{
			mDesignUMG = (UWin_DesignUMG *)(mLogicDesignScene->GetCurrentUIController()->GetRootUI());
		}
		if (mDesignUMG != nullptr)
		{
			//隐藏DetailPage.
			mDesignUMG->HidenDetailPage();

			//设置SuitComponentMenu高亮.
			mDesignUMG->m_UMG_SuitComponentMenu->SetUniqueSelectedIcon(m_Tid);

			mDesignUMG->m_ComponentTid = m_Tid;	//保存Component里点击的部件的Tid.

			mDesignUMG->m_UMG_SuitComponentMenu->SetUniqueHiddenSelectedBatchIcon(mDesignUMG->m_UsedFabric_Struct);

			//设置父对象.
			mDesignUMG->m_ComponentDetail->SetParentTid(m_Tid);

			DesignModule* mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

			//获取SuitCompArray.
			TArray<SuitComponentT* > mSuitCompTArray = mDesignModule->GetSuitComponentArray(m_Tid);
			//过滤AB款后的数组.
			TArray<SuitComponentT* > mSuitCompAdpterABTArray;
			//A,B款过滤.
			int32 mAdapterType = mLogicDesignScene->GetAdapterType(m_Tid);
			//等于0时不用过滤.
			if (mAdapterType != 0)
			{
				for (size_t i = 0; i < mSuitCompTArray.Num(); ++i)
				{
					//相等时显示出来.
					if (mSuitCompTArray[i]->m_AdapterType == mAdapterType)
					{
						mSuitCompAdpterABTArray.Add(mSuitCompTArray[i]);
					}
				}
			}
			else
			{
				mSuitCompAdpterABTArray = mSuitCompTArray;
			}

			//初始化DetailComp.
			mDesignUMG->InitSuitCompDetail(mSuitCompAdpterABTArray);

			//设置compDetail高亮.
			int32 mTid = mLogicDesignScene->GetDefaultComponentTid(m_Tid, EnumComponentType::PART_COMPONENT);

			mDesignUMG->m_ComponentDetail->SetUniqueSelected(mTid);

			mDesignUMG->ShowComponentDetail();

			//初始化SliceMenu.
			int32 mCompTid = mLogicDesignScene->GetDefaultComponentTid(m_Tid, EnumComponentType::PART_COMPONENT);
			mDesignUMG->InitSuitSliceMenu(mCompTid);
		}
	}
	break;
	case T_SUIT_COMPONENT_DETAIL_CELL:
	{
		((UWin_UMGSuitComponentDetail*)m_Parent)->SetUniqueSelected(m_Tid);

		LogicDesignScene *mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();

		UWin_DesignUMG *mDesignUMG = nullptr;

		if (mLogicDesignScene != nullptr)
		{
			mDesignUMG = (UWin_DesignUMG *)(mLogicDesignScene->GetCurrentUIController()->GetRootUI());
		}

		if (mDesignUMG != nullptr)
		{
			//初始化SuitSlice Menu.
			mDesignUMG->InitSuitSliceMenu(m_Tid);

			int32 mSuitPartTid = mDesignUMG->m_UMG_SuitComponentMenu->GetSuitPartTid();

			//获取部位或者线迹或者辅料Tid.
			int32 mTid = mDesignUMG->m_ComponentDetail->GetParentTid();

			//设置IsClicked.
			mDesignUMG->m_IsDetailCompCellClicked = true;

			//改变Component Mesh.
			mLogicDesignScene->ChangeComponent(mTid, m_Tid, EnumComponentType::PART_COMPONENT);

			//更新compMenu.
			mDesignUMG->InitSuitCompMenu(mSuitPartTid);

			//显示片操作的按钮.
 			//mDesignUMG->OnSetPieceShow();

// 			//m_Btn_Slice->SetVisibility(ESlateVisibility::Visible);
 			mDesignUMG->m_Btn_SliceMenu = m_Btn_Slice;

			//设置选择高亮.
			mDesignUMG->m_UMG_SuitComponentMenu->SetUniqueSelectedIcon(mDesignUMG->m_ComponentDetail->GetParentTid());
			
			//更新suitMenu.
			mDesignUMG->InitSuitPartMenu();

			//刷新Batch.
			mDesignUMG->m_UMG_SuitComponentMenu->InitBatchData();
			mDesignUMG->m_UMG_SuitPartMenu->InitBatchData();
		}
	}
	break;
	case T_SUIT_SLICE_MENU_CELL:
	{
		LogicDesignScene *mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();
		UWin_DesignUMG *mDesignUMG = nullptr;
		if (mLogicDesignScene != nullptr)
		{
			mDesignUMG = (UWin_DesignUMG *)(mLogicDesignScene->GetCurrentUIController()->GetRootUI());
		}
		if (mDesignUMG != nullptr)
		{
			//设置SliceMenu选择高亮.
			mDesignUMG->m_UMG_SuitSliceMenu->SetUniqueSelected(m_Tid);
			//重置所有片模型高亮.
			mLogicDesignScene->ResetAllSliceOutLine();
			//设置片模型高亮.
			mLogicDesignScene->SetOutLineBySliceTid(m_Tid);

			mDesignUMG->m_ComponentTid = m_Tid;	//保存part点进去的部件的Tid,目的设置高亮.

			//	显示detailPage.
			//mDesignUMG->SetDetailShopView(false);

			//mDesignUMG->m_DetailPage->m_FirstBatch = true;

			mDesignUMG->m_DetailPage->m_CanvasDetail->SetVisibility(ESlateVisibility::Visible);

			mDesignUMG->m_DetailPage->m_Cav_Buy->SetVisibility(ESlateVisibility::Hidden);

			mDesignUMG->ShowDetailPage();

			_usedFabricStruct.m_patternTid = mLogicDesignScene->GetDefaultPatternInSlice(m_Tid, EnumComponentType::SLICE_COMPONENT);

			_usedFabricStruct.m_fabricTid = mLogicDesignScene->GetDefaultFabricInSlice(m_Tid, EnumComponentType::SLICE_COMPONENT);

			_usedFabricStruct.m_colorTid = mLogicDesignScene->GetDefaultColorInSlice(m_Tid, EnumComponentType::SLICE_COMPONENT);

			mDesignUMG->InitDetailPage(EnumDataType::TYPE_STYLE_SLICE, m_Tid, _usedFabricStruct);

			//移动摄像机.
			mDesignUMG->OnChangeSuitSlice(mLogicDesignScene->GetComponentStructByTid(m_Tid, EnumComponentType::SLICE_COMPONENT));
		}
	}
	break;
	case T_SUIT_ACCESSORIES_MENU_CELL:
	{
		LogicDesignScene *mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();
		UWin_DesignUMG *mDesignUMG = nullptr;
		if (mLogicDesignScene != nullptr)
		{
			mDesignUMG = (UWin_DesignUMG *)(mLogicDesignScene->GetCurrentUIController()->GetRootUI());
		}
		if (mDesignUMG != nullptr)
		{
			//隐藏DetailPage.
			mDesignUMG->HidenDetailPage();

			//获取辅料里的CompT.
			SuitAccessoriesT * mAccessT = TSuitAccessoriesManager::GetInstance()->GetTemplateByPID<SuitAccessoriesT>(m_Tid);
			TArray<SuitComponentT*> mSuitComponentTArray;
			for (int32 i = 0; i < mAccessT->m_SuitComponent.Num(); ++i)
			{
				SuitComponentT *mTemp = TSuitComponentManager::GetInstance()->GetTemplateByPID<SuitComponentT>(mAccessT->m_SuitComponent[i]);
				mSuitComponentTArray.Add(mTemp);
			}
			mDesignUMG->InitSuitCompDetail(mSuitComponentTArray);
			//设置默认高亮.
			int32 mTid = mLogicDesignScene->GetDefaultComponentTid(m_Tid, EnumComponentType::ACCESSORIES_COMPONENT);
			mDesignUMG->m_ComponentDetail->SetUniqueSelected(mTid);
			//设置父对象.
			mDesignUMG->m_ComponentDetail->SetParentTid(m_Tid);
			//显示选中ICON.
			mDesignUMG->m_UMG_SuitComponentMenu->SetUniqueSelectedIcon(m_Tid);
			mDesignUMG->ShowComponentDetail();

			//初始化SuitSlice Menu.
			mDesignUMG->InitSuitSliceMenu(mTid);
			//mDesignUMG->ShowSliceMenuButton();
		}
	}
	break;
	case T_SUIT_STITCH_MENU_CELL:
	{
		LogicDesignScene *mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();
		UWin_DesignUMG *mDesignUMG = nullptr;
		if (mLogicDesignScene != nullptr)
		{
			mDesignUMG = (UWin_DesignUMG *)(mLogicDesignScene->GetCurrentUIController()->GetRootUI());
		}
		if (mDesignUMG != nullptr)
		{
			//隐藏DetailPage.
			mDesignUMG->HidenDetailPage();

			//获取线迹里的CompT.
			SuitStitchT * mSTitchT = TSuitStitchManager::GetInstance()->GetTemplateByPID<SuitStitchT>(m_Tid);
			TArray<SuitComponentT*> mSuitComponentTArray;
			for (int32 i = 0; i < mSTitchT->m_SuitComponentIds.Num(); ++i)
			{
				SuitComponentT *mTemp = TSuitComponentManager::GetInstance()->GetTemplateByPID<SuitComponentT>(mSTitchT->m_SuitComponentIds[i]);
				mSuitComponentTArray.Add(mTemp);
			}
			mDesignUMG->InitSuitCompDetail(mSuitComponentTArray);
			//设置默认高亮.
			int32 mTid = mLogicDesignScene->GetDefaultComponentTid(m_Tid, EnumComponentType::STITCH_COMPONENT);
			mDesignUMG->m_ComponentDetail->SetUniqueSelected(mTid);
			//设置父对象.
			mDesignUMG->m_ComponentDetail->SetParentTid(m_Tid);
			//显示选中ICON.
			mDesignUMG->m_UMG_SuitComponentMenu->SetUniqueSelectedIcon(m_Tid);

			mDesignUMG->ShowComponentDetail();

			mDesignUMG->m_ComponentTid = m_Tid;	//保存Component里点击的部件的Tid.

			mDesignUMG->m_UMG_SuitComponentMenu->SetUniqueHiddenSelectedBatchIcon(mDesignUMG->m_UsedFabric_Struct);

			//初始化SuitSlice Menu.
			mDesignUMG->InitSuitSliceMenu(mTid);
			//mDesignUMG->ShowSliceMenuButton();
		}
	}
	break;
	default:
		break;
	}
}

void UWin_UMGMenuCell::OnButtonSliceClick(int32 _param)
{
	LogicDesignScene *mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();

	UWin_DesignUMG *mDesignUMG = (UWin_DesignUMG *)(mLogicDesignScene->GetCurrentUIController()->GetRootUI());

	mDesignUMG->m_UMG_SuitComponentMenu->SetUniqueHiddenSelectedIcon(mDesignUMG->m_ComponentTid);

	mDesignUMG->ShowSuitSliceMenu();
}

EnumCellType UWin_UMGMenuCell::GetCellType()
{
	return m_Cell_Type;
}
void UWin_UMGMenuCell::SetCellType(EnumCellType _type)
{
	m_Cell_Type = _type;
}

const FString & UWin_UMGMenuCell::GetIconUrl()
{
	return m_IconUrl;
}
void UWin_UMGMenuCell::SetIconUrl(const FString &_url)
{
	m_IconUrl = _url;
}

int32 UWin_UMGMenuCell::GetTid()
{
	return m_Tid;
}
void UWin_UMGMenuCell::SetTid(int32 _Tid)
{
	m_Tid = _Tid;
}

UBaseUI * UWin_UMGMenuCell::GetParent()
{
	return m_Parent;
}
void UWin_UMGMenuCell::SetParent(UBaseUI *_ui)
{
	m_Parent = _ui;
}

void UWin_UMGMenuCell::SetIconTexture2D()
{
	if (m_IconUrl.Len() > 0)
	{
		FString mPath = GPathTool::GetSuitPath(m_IconUrl);


		UTexture2D *mTexture2D = ULoadAssetTool::GetInstance()->LoadUAsset<UTexture2D>(mPath);

		if (mTexture2D->IsValidLowLevel())
		{
			if (m_Image_ICON != nullptr)
			{
				m_Image_ICON->SetBrushFromTexture(mTexture2D);
			}
		}
	}
}

void UWin_UMGMenuCell::ShowSelectedIcon()
{
	m_Image_select->SetVisibility(ESlateVisibility::Visible);
}

void UWin_UMGMenuCell::HidenSelectedIcon()
{
	m_Image_select->SetVisibility(ESlateVisibility::Hidden);
}

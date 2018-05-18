#include "UMGBatchCell.h"
#include "UIManager.h"
#include "LoadAssetTool.h"
#include "TSuitPatternManager.h"
#include "TSuitFabricColorManager.h"
#include "SuitFabricColorT.h"
#include "SuitPartT.h"
#include "UMGSuitPartMenu.h"
#include "UMGSuitComponentMenu.h"
#include "UITextureManager.h"
#include "GameDirector.h"
//!!!DesignUIController.h 在.h包含会出现莫名其妙的错误.
#include "DesignUIController.h"
UUMGBatchCell::UUMGBatchCell(const FObjectInitializer& ObjectInitializer)
{
	m_Image_Icon = nullptr;
	m_Image_select = nullptr;
	m_Btn_Select = nullptr;
}

bool UUMGBatchCell::OnStart()
{
	//Super::OnStart();
	bool mRet = false;

	mRet = true;
	return mRet;
}

void UUMGBatchCell::OnInit()
{
	if (UImage *mTemp = Cast<UImage>(GetWidgetFromName("Image_select")))
	{
		m_Image_select = mTemp;
	}
	if (UImage *mTemp = Cast<UImage>(GetWidgetFromName("Image_Icon")))
	{
		m_Image_Icon = mTemp;
	}
	if (UButton *mTemp = Cast<UButton>(GetWidgetFromName("Btn_Select")))
	{
		m_Btn_Select = mTemp;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_Select, this, &UUMGBatchCell::OnButtonClick);
	}
}

void UUMGBatchCell::OnRelease()
{
	//Super::OnRelease();
	if (m_DownloadImageTool != nullptr)
	{
		m_DownloadImageTool->CancelDownload();
	}
	//m_Image_select = nullptr;
	//m_Btn_Select = nullptr;
	//m_Image_Icon = nullptr;

	//m_LogicDesignScene = nullptr;
}

void UUMGBatchCell::OnSetParams(int32 _pid, FString & _content, ...)
{

}

void UUMGBatchCell::OnTick(float InDeltaTimeSecond)
{

}

void UUMGBatchCell::OnButtonClick(int32 _param)
{
	LogicDesignScene *mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();

	switch (m_Type)
	{
	case BATCH_CELL_PART:
	{
		((UUMGSuitPartMenu*)m_Parent)->SetUniqueSelectedBatchIcon(m_UsedFabric_Struct);
		//初始化DetailPage.
		UDesignUMG *mDesignUMG = nullptr;
		mDesignUMG = (UDesignUMG *)(mLogicDesignScene->GetCurrentUIController()->GetRootUI());
		if (mDesignUMG != nullptr)
		{
			mDesignUMG->m_UsedFabric_Struct = m_UsedFabric_Struct;

			//显示DetailPage.
			mDesignUMG->SetDetailShopView(false);
			mDesignUMG->m_DetailPage->m_FirstBatch = true;
			mDesignUMG->ShowDetailPage();
			//初始化数据.
			mDesignUMG->InitDetailPage(EnumDataType::TYPE_STYLE_ALL,-1,m_UsedFabric_Struct);
		}
	}
	break;
	case BATCH_CELL_COMPONENT:
	{
		((UUMGSuitComponentMenu*)m_Parent)->SetUniqueSelectedBatchIcon(m_UsedFabric_Struct);
		//初始化DetailPage.
		UDesignUMG *mDesignUMG = nullptr;
		mDesignUMG = (UDesignUMG *)(mLogicDesignScene->GetCurrentUIController()->GetRootUI());
		if (mDesignUMG != nullptr)
		{
			mDesignUMG->m_UsedFabric_Struct = m_UsedFabric_Struct;

			mDesignUMG->m_UMG_SuitComponentMenu->SetUniqueHiddenSelectedIcon(mDesignUMG->m_ComponentTid);

			//隐藏CompDetail.
			mDesignUMG->HidenComponentDetail();
			mDesignUMG->SetDetailShopView(false);
			mDesignUMG->m_DetailPage->m_FirstBatch = true;
			//show Detail Page.
			mDesignUMG->ShowDetailPage();
			//初始化数据,传Comp Tid.
			mDesignUMG->InitDetailPage(EnumDataType::TYPE_STYLE_COMPONENT, mLogicDesignScene->GetDefaultComponentTid(mDesignUMG->m_UMG_SuitComponentMenu->GetSuitPartTid(), EnumComponentType::PART_COMPONENT),m_UsedFabric_Struct);
		}
	}
	break;
	default:
		break;
	}
}

void UUMGBatchCell::SetColor()
{
	SuitFabricColorT *mColor = TSuitFabricColorManager::GetInstance()->GetTemplateByPID<SuitFabricColorT>(m_UsedFabric_Struct.m_colorTid);
	if (mColor==nullptr)
	{
		return;
	}

	if (mColor->m_icon.Len()>0)
	{
		LogicDesignScene *mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();
		mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();
		DesignUIController *mDesignUIController = (DesignUIController*)mLogicDesignScene->GetCurrentUIController();


		//从缓存里取图片,缓存里没有则下载图片.
		UUITextureManager *mUITextureController = mDesignUIController->GetTextureManager();
		if (mUITextureController != nullptr)
		{
			UTexture2D *mTexture2d = mUITextureController->GetTexture2d(mColor->m_icon);
			if (mTexture2d->IsValidLowLevel())
			{
				m_Image_Icon->GetDynamicMaterial()->SetTextureParameterValue(FName(TEXT("ColorTexture")), mTexture2d);
			}
			else
			{
				SuitPatternT *mPattern = TSuitPatternManager::GetInstance()->GetPatternData(m_UsedFabric_Struct.m_patternTid);
				DownloadIconImage(mColor->m_icon);
			}
		}
	}
	else
	{
		//切换颜色.
		m_Image_Icon->GetDynamicMaterial()->SetScalarParameterValue(FName(TEXT("ColorOrTexture")), -1);
		//设置动态颜色.
		m_Image_Icon->GetDynamicMaterial()->SetVectorParameterValue(FName(TEXT("ColorBase")), mColor->m_RGB);
	}

	
}

void UUMGBatchCell::SetTexture2D()
{
	LogicDesignScene *mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();

	mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();
	SuitPatternT *mPattern = TSuitPatternManager::GetInstance()->GetPatternData(m_UsedFabric_Struct.m_patternTid);
	if (mPattern != nullptr)
	{
		//切换Texture.
		m_Image_Icon->GetDynamicMaterial()->SetScalarParameterValue(FName(TEXT("ColorOrTexture")), 1);
		//设置动态图片.
		DesignUIController *mDesignUIController = (DesignUIController*)mLogicDesignScene->GetCurrentUIController();

		UUITextureManager *mUITextureController = mDesignUIController->GetTextureManager();
		if (mUITextureController != nullptr)
		{
			UTexture2D *mTexture2d = mUITextureController->GetTexture2d(mPattern->m_iconUrl);
			if (mTexture2d->IsValidLowLevel())
			{
				m_Image_Icon->GetDynamicMaterial()->SetTextureParameterValue(FName(TEXT("ColorTexture")), mTexture2d);
			}
			else
			{
				SuitPatternT *mPattern = TSuitPatternManager::GetInstance()->GetPatternData(m_UsedFabric_Struct.m_patternTid);
				DownloadIconImage(mPattern->m_iconUrl);
			}
		}
	}
}
   
void UUMGBatchCell::SetUsedFabricStruct(UsedFabricStruct _inStruct)
{
	m_UsedFabric_Struct = _inStruct;
}

void UUMGBatchCell::ShowSelectedIcon()
{
	m_Image_select->SetVisibility(ESlateVisibility::Visible);
}

void UUMGBatchCell::HiddenSelectedIcon()
{
	m_Image_select->SetVisibility(ESlateVisibility::Hidden);
}

void UUMGBatchCell::DownloadIconImage(const FString &_IconHttpPath)
{

	if (m_DownloadImageTool != nullptr)
	{
		m_DownloadImageTool->CancelDownload();
	}
	m_DownloadImageTool = UDownloadImageTool::DownloadImage(1, _IconHttpPath, this, &UUMGBatchCell::OnDownoloadFinished);
}
	

void UUMGBatchCell::OnDownoloadFinished(bool _isScuccess, int32 _penetrateId, FString _url, UTexture2D * _texture2d, UDownloadImageTool * _downloadTool)
{
	if (_texture2d->IsValidLowLevel())
	{
		LogicDesignScene *mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();

		m_Image_Icon->GetDynamicMaterial()->SetTextureParameterValue(FName(TEXT("ColorTexture")), _texture2d);
		DesignUIController *mDesignUIController = (DesignUIController*)mLogicDesignScene->GetCurrentUIController();

		UUITextureManager *mUITextureController = mDesignUIController->GetTextureManager();
		if (mUITextureController != nullptr)
		{
			mUITextureController->AddTexture2D(_url, _texture2d);
		}
	}
}

UsedFabricStruct UUMGBatchCell::GetUsedFabricStruct()
{
	return m_UsedFabric_Struct;
}



void UUMGBatchCell::SetCellType(EnumBatchCellType _type)
{
	m_Type = _type;
}

void UUMGBatchCell::SetParent(UBaseUI *_parent)
{
	m_Parent = _parent;
}

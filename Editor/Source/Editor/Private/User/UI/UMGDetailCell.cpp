// Fill out your copyright notice in the Description page of Project Settings.

#include "UMGDetailCell.h"
#include "GPathTool.h"
#include "DesignUMG.h"
#include "SuitFabricT.h"
#include "GLanguageTool.h"
#include "GameDirector.h"
#include "DesignModule.h"
#include "LoadAssetTool.h"
#include "GModuleManager.h"
#include "LogicDesignScene.h"
#include "TSuitFabricManager.h"
#include "DesignUIController.h"
#include "SuitPatternLibraryT.h"
#include "Kismet/GameplayStatics.h"
#include "TSuitFabricColorLibraryManager.h"
#include "TSuitFabricColorManager.h"

UUMGDetailCell::UUMGDetailCell(const FObjectInitializer& ObjectInitializer)
{
	m_Image_Icon = nullptr;

	m_Image_Back = nullptr;

	m_Image_select = nullptr;

	m_Image_Material = nullptr;

	m_Image_MaterialIcon = nullptr;

	m_Btn_Select = nullptr;

	m_Border = nullptr;

	m_IconUrl = nullptr;

}

bool UUMGDetailCell::OnStart()
{
	mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

	if (mDesignModule == nullptr)
	{
		return false;
	}

	UUIManager::GetInstance()->RegisterButtonClick(1, m_Btn_Select, this, &UUMGDetailCell::OnButtonClick);

	//	本地监听消息回调.
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, mDesignModule->m_Http_BuyInfo, this, &UUMGDetailCell::SetBuyInfo);

	if (!m_downloadSuccess)
	{
		if (m_IconUrl.Contains(TEXT("http")))
		{
			if (m_SelectType==1)
			{
				DownloadIconImage();
			}
			else
			{
				DownloadIconImage2();
			}
		}
	}
	return true;
}

void UUMGDetailCell::OnInit()
{
	m_downloadSuccess = false;

	if (UButton * BtnSelect = Cast<UButton>(GetWidgetFromName("Btn_Select")))
	{
		m_Btn_Select = BtnSelect;
	}

	if (UImage * ImgBack = Cast<UImage>(GetWidgetFromName("Image_back")))
	{
		m_Image_Back = ImgBack;
	}

	if (UImage * ImgIcon = Cast<UImage>(GetWidgetFromName("Image_ICON")))
	{
		m_Image_Icon = ImgIcon;
	}

	if (UImage * ImgSelect = Cast<UImage>(GetWidgetFromName("Image_select")))
	{
		m_Image_select = ImgSelect;
	}

	if (UImage * ImgMaterial = Cast<UImage>(GetWidgetFromName("Image_Material")))
	{
		m_Image_Material = ImgMaterial;
	}

	if (UImage * ImgMaterialIcon = Cast<UImage>(GetWidgetFromName("Image_MaterialIcon")))
	{
		m_Image_MaterialIcon = ImgMaterialIcon;
	}

	if (UBorder * mBorder = Cast<UBorder>(GetWidgetFromName("Border_Color")))
	{
		m_Border = mBorder;
	}
}

void UUMGDetailCell::OnRelease()
{
	m_Image_Icon = nullptr;

	m_Image_Back = nullptr;

	m_Image_select = nullptr;

	m_Image_Material = nullptr;

	m_Image_MaterialIcon = nullptr;

	m_Btn_Select = nullptr;

	m_Border = nullptr;

	m_IconUrl = nullptr;

	UUIManager::GetInstance()->RemoveButtonClick(m_Btn_Select);

	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, mDesignModule->m_Http_BuyInfo, this, &UUMGDetailCell::SetBuyInfo);

	if (nullptr != m_DownloadImageTool)
	{
		m_DownloadImageTool->CancelDownload();

		m_DownloadImageTool = nullptr;
	}
}

void UUMGDetailCell::OnSetParams(int32 _pid, FString & _content, ...)
{

}

void UUMGDetailCell::OnTick(float InDeltaTimeSecond)
{

}


void UUMGDetailCell::OnButtonClick(int32 _param)
{
	DesignModule *mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

	LogicDesignScene * mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();

	UDesignUMG * mDesignUMG = nullptr;

	if (mLogicDesignScene != nullptr)
	{
		mDesignUMG = (UDesignUMG *)(mLogicDesignScene->GetCurrentUIController()->GetRootUI());
	}

	//	点击花型，判断购物车.
	if (m_SelectType == 1)
	{
		mDesignUMG->SetDetailShopView(false);

		mDesignModule->RequestBuyServer(m_patternId, "1");

		mDesignUMG->SetDetailTexture(m_PatternTexture);
	}

	//	点击颜色，判断购物车.
	else if (m_SelectType == 2)
	{
		mDesignUMG->SetDetailShopView(false);

		mDesignUMG->m_DetailPage->m_RGB = m_RGB;

		mDesignModule->RequestBuyServer(m_ColorGoodsId, "2");

		mDesignUMG->SetDetailColor(m_RGB, m_ColorTexture);
	}

	else if (m_SelectType == 3)
	{
		mDesignUMG->SetDetailShopView(false);

		mDesignModule->RequestBuyServer(FString::FromInt(m_MaterialId), "3");

		mDesignUMG->SetDetailMaterial(m_RGB, m_DetailMaterial);
	}

	//	判断是否是DetailCell.
	if (mDesignUMG->m_DetailPage->m_FirstBatch == false)
	{

#if  PLATFORM_WINDOWS
		OnWindowsButtonClickEvent();
#else
		OnMobileButtonClickEvent();
#endif 
	}
}

void UUMGDetailCell::OnWindowsButtonClickEvent()
{
#if WITH_EDITOR
	OnMobileButtonClickEvent();
#endif
}

void UUMGDetailCell::OnMobileButtonClickEvent()
{
	LogicDesignScene * mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();

	UDesignUMG * mDesignUMG = nullptr;

	if (mLogicDesignScene != nullptr)
	{
		mDesignUMG = (UDesignUMG *)(mLogicDesignScene->GetCurrentUIController()->GetRootUI());
	}

	//重置targetFabricTid.
	mDesignUMG->m_DetailPage->m_usedFabricStruct.m_targetFabricTid = 0;

	UsedFabricStruct mUsedFabricStruct = mDesignUMG->m_DetailPage->m_usedFabricStruct;

	mDesignUMG->m_DetailPage->m_CurrentChangeFabricCell = this;

	switch (m_Type)
	{
	case TYPE_STYLE_SLICE:
	{
		if (mDesignUMG != nullptr)
		{
			if (m_SelectType == 1)
			{
				m_Image_select->SetVisibility(ESlateVisibility::Visible);

				mDesignUMG->m_DetailPage->GetSelectPatternNum(m_SelectId);

				mLogicDesignScene->ChangePattern(m_Style_Tid, m_patternId, EnumComponentType::SLICE_COMPONENT);
			}

			if (m_SelectType == 2)
			{
				m_Image_select->SetVisibility(ESlateVisibility::Visible);

				mDesignUMG->m_DetailPage->GetSelectColorNum(m_SelectId);

				mLogicDesignScene->ChangeColor(m_Style_Tid, m_ColorId, EnumComponentType::SLICE_COMPONENT);
			}

			if (m_SelectType == 3)
			{
				m_Image_select->SetVisibility(ESlateVisibility::Visible);

				mDesignUMG->m_DetailPage->GetSelectMaterialNum(m_SelectId);

				//打开更换面料提示.
				FString mTipContent = GLanguageTool::GetInstance()->GetLanguage(TEXT("key147"));

				UUIManager::GetInstance()->OpenMiddleTip(mTipContent, 0.0f);

				mDesignUMG->m_DetailPage->m_usedFabricStruct.m_targetFabricTid = m_MaterialId;

				//请求服务器能否集成花型.
				SuitPatternLibraryT::GetInstance()->RequestPatternUseInFabric(m_MaterialId, mUsedFabricStruct.m_patternTid);
			}
		}

		if (m_SelectType != 3)
		{
			mDesignUMG->UpdateSliceDetailPage();
		}
	}
	break;

	case TYPE_STYLE_ALL:
	{
		if (mDesignUMG != nullptr)
		{
			if (m_SelectType == 1)
			{
				m_Image_select->SetVisibility(ESlateVisibility::Visible);

				mDesignUMG->m_DetailPage->GetSelectPatternNum(m_SelectId);

				mUsedFabricStruct.m_targetPatternTid = m_patternId;

				mLogicDesignScene->ChangeAllSliceChildComponentByTid(mDesignModule->GetDesignData()->m_designTID, mUsedFabricStruct, EnumComponentType::STYLE_COMPONENT);

				mUsedFabricStruct.m_targetFabricTid = mUsedFabricStruct.m_fabricTid;

				mUsedFabricStruct.m_targetColorTid = 0;
			}

			if (m_SelectType == 2)
			{
				m_Image_select->SetVisibility(ESlateVisibility::Visible);

				mDesignUMG->m_DetailPage->GetSelectColorNum(m_SelectId);

				mUsedFabricStruct.m_targetColorTid = m_ColorId;

				mLogicDesignScene->ChangeAllSliceChildComponentByTid(mDesignModule->GetDesignData()->m_designTID, mUsedFabricStruct, EnumComponentType::STYLE_COMPONENT);

				mUsedFabricStruct.m_targetFabricTid = mUsedFabricStruct.m_fabricTid;

				mUsedFabricStruct.m_targetPatternTid = TEXT("");
			}

			if (m_SelectType == 3)
			{
				m_Image_select->SetVisibility(ESlateVisibility::Visible);

				mDesignUMG->m_DetailPage->GetSelectMaterialNum(m_SelectId);

				//打开更换面料提示.
				FString mTipContent = GLanguageTool::GetInstance()->GetLanguage(TEXT("key147"));

				UUIManager::GetInstance()->OpenMiddleTip(mTipContent, 0.0f);

				mDesignUMG->m_DetailPage->m_usedFabricStruct.m_targetFabricTid = m_MaterialId;

				//请求服务器能否集成花型.
				SuitPatternLibraryT::GetInstance()->RequestPatternUseInFabric(m_MaterialId, mUsedFabricStruct.m_patternTid);
			}
		}

		if (m_SelectType != 3)
		{
			UsedFabricStruct mUFS;

			mUFS.m_colorTid = mUsedFabricStruct.m_targetColorTid;

			mUFS.m_fabricTid = mUsedFabricStruct.m_targetFabricTid;

			mUFS.m_patternTid = mUsedFabricStruct.m_targetPatternTid;

			mUFS.m_fabricTidArray = mUsedFabricStruct.m_fabricTidArray;

			mDesignUMG->UpdatePartDetailPage(mUFS);
		}
	}
	break;

	case BATCH_CELL_COMPONENT:
	{
		if (mDesignUMG != nullptr)
		{
			if (m_SelectType == 1)
			{
				m_Image_select->SetVisibility(ESlateVisibility::Visible);

				mDesignUMG->m_DetailPage->GetSelectPatternNum(m_SelectId);

				mUsedFabricStruct.m_targetPatternTid = m_patternId;

				mLogicDesignScene->ChangeAllSliceChildComponentByTid(m_Style_Tid, mUsedFabricStruct, EnumComponentType::PIECES_COMPONENT);

				mUsedFabricStruct.m_targetFabricTid = mUsedFabricStruct.m_fabricTid;

				mUsedFabricStruct.m_targetColorTid = 0;
			}

			if (m_SelectType == 2)
			{
				m_Image_select->SetVisibility(ESlateVisibility::Visible);

				mDesignUMG->m_DetailPage->GetSelectColorNum(m_SelectId);

				mUsedFabricStruct.m_targetColorTid = m_ColorId;

				mLogicDesignScene->ChangeAllSliceChildComponentByTid(m_Style_Tid, mUsedFabricStruct, EnumComponentType::PIECES_COMPONENT);

				mUsedFabricStruct.m_targetFabricTid = mUsedFabricStruct.m_fabricTid;

				mUsedFabricStruct.m_targetPatternTid = TEXT("");

			}

			if (m_SelectType == 3)
			{
				m_Image_select->SetVisibility(ESlateVisibility::Visible);

				mDesignUMG->m_DetailPage->GetSelectMaterialNum(m_SelectId);

				//打开更换面料提示.
				FString mTipContent = GLanguageTool::GetInstance()->GetLanguage(TEXT("key147"));

				UUIManager::GetInstance()->OpenMiddleTip(mTipContent, 0.0f);

				mDesignUMG->m_DetailPage->m_usedFabricStruct.m_targetFabricTid = m_MaterialId;

				//请求服务器能否集成花型.
				SuitPatternLibraryT::GetInstance()->RequestPatternUseInFabric(m_MaterialId, mUsedFabricStruct.m_patternTid);
			}

			if (m_SelectType != 3)
			{
				UsedFabricStruct mUFS;
				mUFS.m_colorTid = mUsedFabricStruct.m_targetColorTid;
				mUFS.m_fabricTid = mUsedFabricStruct.m_targetFabricTid;
				mUFS.m_patternTid = mUsedFabricStruct.m_targetPatternTid;
				mUFS.m_fabricTidArray = mUsedFabricStruct.m_fabricTidArray;

				mDesignUMG->UpdateComponentDetailPage(mUFS);
			}
		}
	}
	break;

	default:
		break;
	}
}

void UUMGDetailCell::SetColor(FLinearColor _RGB)
{
	m_Image_Back->SetVisibility(ESlateVisibility::Hidden);

	if (m_SelectType == 2)
	{
		m_Border->SetVisibility(ESlateVisibility::Visible);

		m_Image_Icon->SetVisibility(ESlateVisibility::Hidden);

		m_Image_Material->SetVisibility(ESlateVisibility::Hidden);

 		m_Border->GetDynamicMaterial()->SetScalarParameterValue("ColorOrTexture", -1);
 
 		m_Border->GetDynamicMaterial()->SetVectorParameterValue("Color", _RGB);	

		//m_Border->SetBrushColor(_RGB);

		m_RGB = _RGB;
	}

	if (m_SelectType == 3)
	{
		m_Border->SetVisibility(ESlateVisibility::Hidden);

		m_Image_Icon->SetVisibility(ESlateVisibility::Hidden);

		m_Image_Material->SetVisibility(ESlateVisibility::Visible);

		m_Image_Material->GetDynamicMaterial()->SetScalarParameterValue("ColorOrTexture", -1);

		m_Image_Material->GetDynamicMaterial()->SetVectorParameterValue("Color", _RGB);

		m_RGB = _RGB;
	}
}

void UUMGDetailCell::SetMaterial(FString _iconUrl)
{
	//	显示底图.
	m_Image_Back->SetVisibility(ESlateVisibility::Visible);

	UTexture2D * mTexture2D;

	if (!_iconUrl.Contains(TEXT("http")))
	{
		m_IconUrl = _iconUrl;

		FString mPath = GPathTool::GetFabricPath(m_IconUrl);

		mTexture2D = ULoadAssetTool::GetInstance()->LoadUAsset<UTexture2D>(mPath);

		m_Image_Back->SetVisibility(ESlateVisibility::Hidden);

		if (mTexture2D->IsValidLowLevel())
		{
			if (m_SelectType == 2)
			{
				m_Border->SetVisibility(ESlateVisibility::Visible);

				m_Image_Icon->SetVisibility(ESlateVisibility::Hidden);

				m_Image_Material->SetVisibility(ESlateVisibility::Hidden);

				if (m_IsMetal == false)
				{
					m_Image_MaterialIcon->SetVisibility(ESlateVisibility::Hidden);

					m_Border->GetDynamicMaterial()->SetTextureParameterValue("maskIcon", mTexture2D);
				}
				else
				{
					m_Border->SetVisibility(ESlateVisibility::Hidden);

					m_Image_MaterialIcon->SetVisibility(ESlateVisibility::Visible);

					m_Image_MaterialIcon->SetBrushFromTexture(mTexture2D);

					m_ColorTexture = mTexture2D;
				}
			}

			if (m_SelectType == 3)
			{
				m_Border->SetVisibility(ESlateVisibility::Hidden);

				m_Image_Icon->SetVisibility(ESlateVisibility::Hidden);

				m_Image_Material->SetVisibility(ESlateVisibility::Visible);

				if (m_IsMetal == false)
				{
					m_Image_MaterialIcon->SetVisibility(ESlateVisibility::Hidden);

					m_Image_Material->GetDynamicMaterial()->SetTextureParameterValue("maskIcon", mTexture2D);
				}
				else
				{
					m_Image_Material->SetVisibility(ESlateVisibility::Hidden);

					m_Image_MaterialIcon->SetVisibility(ESlateVisibility::Visible);

					m_Image_MaterialIcon->SetBrushFromTexture(mTexture2D);

					m_DetailMaterial = mTexture2D;
				}
			}
		}
	}

	else
	{
		LogicDesignScene * mDesignScene = (LogicDesignScene *)UGameDirector::GetInstance()->GetCurrentLogicScene();

		DesignUIController*  mDesignController = (DesignUIController *)(mDesignScene->GetCurrentUIController());

		UTexture2D * mDetailTexture = mDesignController->GetTextureManager()->GetTexture2d(_iconUrl);

		if (mDetailTexture == nullptr)
		{
			m_IconUrl = _iconUrl;

			DownloadIconImage2();
		}
		else
		{
			m_Image_Back->SetVisibility(ESlateVisibility::Hidden);

			if (m_SelectType == 2)
			{
				m_Border->SetVisibility(ESlateVisibility::Visible);

				m_Image_Icon->SetVisibility(ESlateVisibility::Hidden);

				m_Image_Material->SetVisibility(ESlateVisibility::Hidden);

				if (m_IsMetal == false)
				{
					m_Image_MaterialIcon->SetVisibility(ESlateVisibility::Hidden);

					m_Border->GetDynamicMaterial()->SetTextureParameterValue("maskIcon", mDetailTexture);
				}
				else
				{
					m_Border->SetVisibility(ESlateVisibility::Hidden);

					m_Image_MaterialIcon->SetVisibility(ESlateVisibility::Visible);

					m_Image_MaterialIcon->SetBrushFromTexture(mDetailTexture);
					
					m_ColorTexture = mDetailTexture;
				}
			}

			if (m_SelectType == 3)
			{
				m_Border->SetVisibility(ESlateVisibility::Hidden);

				m_Image_Icon->SetVisibility(ESlateVisibility::Hidden);

				m_Image_Material->SetVisibility(ESlateVisibility::Visible);

				if (m_IsMetal == false)
				{
					m_Image_MaterialIcon->SetVisibility(ESlateVisibility::Hidden);

					m_Image_Material->GetDynamicMaterial()->SetTextureParameterValue("maskIcon", mDetailTexture);
				}
				else
				{
					m_Image_Material->SetVisibility(ESlateVisibility::Hidden);

					m_Image_MaterialIcon->SetVisibility(ESlateVisibility::Visible);

					m_Image_MaterialIcon->SetBrushFromTexture(mDetailTexture);

					m_DetailMaterial = mDetailTexture;
				}
			}
		}
	}
}

void UUMGDetailCell::SetMaterialMask(FString _iconUrl)
{
	UTexture2D * mTexture2D;

	if (!_iconUrl.Contains(TEXT("http")))
	{
		m_IconUrl = _iconUrl;

		FString mPath = GPathTool::GetFabricPath(m_IconUrl);

		mTexture2D = ULoadAssetTool::GetInstance()->LoadUAsset<UTexture2D>(mPath);

		if (mTexture2D->IsValidLowLevel())
		{
			if (m_SelectType == 2)
			{
				m_Border->SetVisibility(ESlateVisibility::Visible);

				m_Image_Icon->SetVisibility(ESlateVisibility::Hidden);

				m_Image_Material->SetVisibility(ESlateVisibility::Hidden);

				m_Border->GetDynamicMaterial()->SetTextureParameterValue("maskIcon", mTexture2D);
			}

			if (m_SelectType == 3)
			{
				m_Border->SetVisibility(ESlateVisibility::Hidden);

				m_Image_Icon->SetVisibility(ESlateVisibility::Hidden);

				m_Image_Material->SetVisibility(ESlateVisibility::Visible);

				m_Image_Material->GetDynamicMaterial()->SetTextureParameterValue("maskIcon", mTexture2D);
			}
		}
	}

	else
	{
		m_IconUrl = _iconUrl;

		DownloadIconImage2();
	}
}

UTexture2D * UUMGDetailCell::SetTexture2D(FString _iconUrl)
{
	m_IconUrl = _iconUrl;

	//	显示底图.
	m_Image_Back->SetVisibility(ESlateVisibility::Visible);

	LogicDesignScene * mDesignScene = (LogicDesignScene *)UGameDirector::GetInstance()->GetCurrentLogicScene();

	DesignUIController*  mDesignController = (DesignUIController *)(mDesignScene->GetCurrentUIController());

	UTexture2D * mDetailTexture = mDesignController->GetTextureManager()->GetTexture2d(m_IconUrl);

	if (mDetailTexture == nullptr)
		DownloadIconImage();

	else if (mDetailTexture != nullptr)
	{
		m_Border->SetVisibility(ESlateVisibility::Hidden);

		m_Image_Icon->SetVisibility(ESlateVisibility::Visible);

		m_Image_Material->SetVisibility(ESlateVisibility::Hidden);

		m_Image_Icon->SetBrushFromTexture(mDetailTexture);

		m_PatternTexture = mDetailTexture;
	}

	return mDetailTexture;
}

//	下载图片.
void UUMGDetailCell::DownloadIconImage()
{
	if (m_DownloadImageTool != nullptr)
	{
		m_DownloadImageTool->CancelDownload();
	}
	m_DownloadImageTool = UDownloadImageTool::DownloadImage(1, m_IconUrl, this, &UUMGDetailCell::OnDownoloadFinished);
}

void UUMGDetailCell::DownloadIconImage2()
{
	if (m_DownloadImageTool != nullptr)
	{
		m_DownloadImageTool->CancelDownload();
	}
	m_DownloadImageTool = UDownloadImageTool::DownloadImage(1, m_IconUrl, this, &UUMGDetailCell::OnDownoloadFinished2);
}

void UUMGDetailCell::SetBuyInfo(msg_ptr _msg)
{
	//换成本地消息.
	MsgLocal * mMsgLocal = _msg->ConvertMsg<MsgLocal>();

	//获取原数据.
	GoodInfoStruct * mGoodInfo = (GoodInfoStruct *)mMsgLocal->Content();

	m_IsBuy = mGoodInfo->m_isBuy;

	//获取DesignUMG.
	LogicDesignScene * mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();

	UDesignUMG * mDesignUMG = nullptr;

	if (mLogicDesignScene != nullptr)
		mDesignUMG = (UDesignUMG *)(mLogicDesignScene->GetCurrentUIController()->GetRootUI());

	//显示购物信息.
	if (m_IsBuy)
	{
		mDesignUMG->m_CanBuy = true;

		if (!mGoodInfo->m_orderUrl.IsEmpty())
			mDesignUMG->SetBuyUrl(mGoodInfo->m_orderUrl);
		else
			m_IsBuy = false;
	}
	else
		mDesignUMG->m_CanBuy = false;

	mDesignUMG->SetDetailShopView(m_IsBuy);
}

//	下载图片成功回调.
void UUMGDetailCell::OnDownoloadFinished(bool _isScuccess, int32 _penetrateId, FString _url, UTexture2D * _texture2d, UDownloadImageTool * _downloadTool)
{
	if (_texture2d->IsValidLowLevel())
	{
		m_downloadSuccess = true;

		if (nullptr != m_Image_Back)
			m_Image_Back->SetVisibility(ESlateVisibility::Hidden);

		if (m_Border != nullptr)
		{
			m_Border->SetVisibility(ESlateVisibility::Hidden);
		}

		if (m_Image_Icon != nullptr)
		{
			m_Image_Icon->SetVisibility(ESlateVisibility::Visible);
		}

		if (m_Image_Material != nullptr)
		{
			m_Image_Material->SetVisibility(ESlateVisibility::Hidden);
		}

		LogicDesignScene * mDesignScene = (LogicDesignScene *)UGameDirector::GetInstance()->GetCurrentLogicScene();

		DesignUIController*  mDesignController = (DesignUIController *)(mDesignScene->GetCurrentUIController());

		mDesignController->GetTextureManager()->AddTexture2D(_url, _texture2d);

		if (m_Image_Icon != nullptr)
		{
			m_Image_Icon->SetBrushFromTexture(_texture2d);
		}

		m_PatternTexture = _texture2d;
	}
}

void UUMGDetailCell::OnDownoloadFinished2(bool _isScuccess, int32 _penetrateId, FString _url, UTexture2D * _texture2d, UDownloadImageTool * _downloadTool)
{
	if (_texture2d->IsValidLowLevel())
	{
		m_downloadSuccess = true;

		if (nullptr != m_Image_Back)
			m_Image_Back->SetVisibility(ESlateVisibility::Hidden);

		if (m_SelectType == 2)
		{
			m_Border->SetVisibility(ESlateVisibility::Visible);

			m_Image_Icon->SetVisibility(ESlateVisibility::Hidden);

			m_Image_Material->SetVisibility(ESlateVisibility::Hidden);

			if (m_IsMetal == false)
			{
				m_Image_MaterialIcon->SetVisibility(ESlateVisibility::Hidden);

				m_Border->GetDynamicMaterial()->SetTextureParameterValue("maskIcon", _texture2d);
			}
			else
			{
				m_Border->SetVisibility(ESlateVisibility::Hidden);

				m_Image_MaterialIcon->SetVisibility(ESlateVisibility::Visible);

				m_Image_MaterialIcon->SetBrushFromTexture(_texture2d);
				
				m_ColorTexture = _texture2d;
			}
		}

		if (m_SelectType == 3)
		{
			m_Border->SetVisibility(ESlateVisibility::Hidden);

			m_Image_Icon->SetVisibility(ESlateVisibility::Hidden);

			m_Image_Material->SetVisibility(ESlateVisibility::Visible);

			if (m_IsMetal == false)
			{
				m_Image_MaterialIcon->SetVisibility(ESlateVisibility::Hidden);

				m_Image_Material->GetDynamicMaterial()->SetTextureParameterValue("maskIcon", _texture2d);
			}
			else
			{
				m_Image_Material->SetVisibility(ESlateVisibility::Hidden);

				m_Image_MaterialIcon->SetVisibility(ESlateVisibility::Visible);

				m_Image_MaterialIcon->SetBrushFromTexture(_texture2d);

				m_DetailMaterial = _texture2d;
			}
		}

		LogicDesignScene * mDesignScene = (LogicDesignScene *)UGameDirector::GetInstance()->GetCurrentLogicScene();

		DesignUIController*  mDesignController = (DesignUIController *)(mDesignScene->GetCurrentUIController());

		mDesignController->GetTextureManager()->AddTexture2D(_url, _texture2d);
	}
}

void UUMGDetailCell::OnSetMaterialView()
{
	m_Image_select->SetVisibility(ESlateVisibility::Hidden);

	GetWorld()->GetTimerManager().ClearTimer(m_CountDownTimerHander);
}

void UUMGDetailCell::ChangeFabric()
{
	
	LogicDesignScene * mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();
	UDesignUMG * mDesignUMG = (UDesignUMG *)(mLogicDesignScene->GetCurrentUIController()->GetRootUI());
	UsedFabricStruct mUsedFabricStruct = mDesignUMG->m_DetailPage->m_usedFabricStruct;
	if (mUsedFabricStruct.m_targetFabricTid==mUsedFabricStruct.m_fabricTid)
	{
		return;
	}


	SuitFabricT *mFabricT = TSuitFabricManager::GetInstance()->GetTemplateByPID<SuitFabricT>(mUsedFabricStruct.m_targetFabricTid);
	if (mFabricT==nullptr)
	{   
		UE_LOG(LogTemp, Log, TEXT("ChangeFabric() : mFabricT is nullptr"));

		return;
	}

	if (mUsedFabricStruct.m_patternTid.Len()>0)
	{
		//获取可继承花型ID.
		FString mPatternId = SuitPatternLibraryT::GetInstance()->GetPatternId(mUsedFabricStruct.m_targetFabricTid, mUsedFabricStruct.m_patternTid);
		mUsedFabricStruct.m_targetPatternTid = mPatternId;
	}
	else
	{
		//可以继承时.
		mUsedFabricStruct.m_targetPatternTid = mUsedFabricStruct.m_patternTid;
	}
	

	if (mUsedFabricStruct.m_colorTid!= 0)
	{
		//获取主推颜色T数据.
		mUsedFabricStruct.m_targetColorTid =TSuitFabricManager::GetInstance()->CheckExternColor(mUsedFabricStruct.m_colorTid, mFabricT->GetTid());
		
	}



	UsedFabricStruct mUFS;
	mUFS.m_colorTid = mUsedFabricStruct.m_targetColorTid;
	mUFS.m_fabricTid = mUsedFabricStruct.m_targetFabricTid;
	mUFS.m_patternTid = mUsedFabricStruct.m_targetPatternTid;
	mUFS.m_fabricTidArray = mUsedFabricStruct.m_fabricTidArray;
	
	switch (m_Type)
	{
		case TYPE_STYLE_SLICE:
		{
			mLogicDesignScene->ChangeAllSliceChildComponentByTid(m_Style_Tid, mUsedFabricStruct, EnumComponentType::SLICE_COMPONENT);

			mDesignUMG->UpdateSliceDetailPage();
		}
		break;

		case TYPE_STYLE_ALL:
		{

			mLogicDesignScene->ChangeAllSliceChildComponentByTid(mDesignModule->GetDesignData()->m_designTID, mUsedFabricStruct, EnumComponentType::STYLE_COMPONENT);

			mDesignUMG->UpdatePartDetailPage(mUFS);
		}
		break;

		case BATCH_CELL_COMPONENT:
		{

			mLogicDesignScene->ChangeAllSliceChildComponentByTid(m_Style_Tid, mUsedFabricStruct, EnumComponentType::PIECES_COMPONENT);

			mDesignUMG->UpdateComponentDetailPage(mUFS);
		}
		break;
		default:
			break;
	}
}

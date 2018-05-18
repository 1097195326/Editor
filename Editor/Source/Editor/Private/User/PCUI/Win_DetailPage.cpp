// Fill out your copyright notice in the Description page of Project Settings.

#include "Win_DetailPage.h"
#include "UIManager.h"
#include "Win_WebPage.h"
#include "SuitFabricT.h"
#include "SuitPatternT.h"
#include "GameDirector.h"
#include "Win_PhotoPage.h"
#include "Win_DesignUMG.h"
#include "GLanguageTool.h"
#include "LoadAssetTool.h"
#include "GModuleManager.h"
#include "LogicDesignScene.h"
#include "SuitFabricColorT.h"
#include "Win_UMGDetailCell.h"
#include "TSuitSliceManager.h"
#include "TSuitFabricManager.h"
#include "SuitFabricLibraryT.h"
#include "TSuitPatternManager.h"
#include "DesignUIController.h"
#include "SuitFabricColorLibraryT.h"
#include "TSuitFabricColorManager.h"
#include "TSuitFabricLibraryManager.h"
#include "TSuitFabricColorLibraryManager.h"

UWin_DetailPage::UWin_DetailPage(const FObjectInitializer& ObjectInitializer)
{
	m_PageIndex = 1;

	m_BtnPattern = nullptr;

	m_GridPattern = nullptr;

	m_ChangePage = true;
}

bool UWin_DetailPage::OnStart()
{
	m_DesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

	if (UTextBlock* TextPattern = Cast<UTextBlock>(GetWidgetFromName("Text_Pattern")))
	{
		m_TextPattern = TextPattern;

		FString mTextPattern = GLanguageTool::GetInstance()->GetLanguage(TEXT("key147"));

		m_TextPattern->SetText(FText::FromString(mTextPattern));
	}

	if (UTextBlock* TextColor = Cast<UTextBlock>(GetWidgetFromName("Text_Color")))
	{
		m_TextColor = TextColor;

		FString mTextColor = GLanguageTool::GetInstance()->GetLanguage(TEXT("key148"));

		m_TextColor->SetText(FText::FromString(mTextColor));

	}

	if (UTextBlock* TextMaterial = Cast<UTextBlock>(GetWidgetFromName("Text_Material")))
	{
		m_TextMaterial = TextMaterial;

		FString mTextMaterial = GLanguageTool::GetInstance()->GetLanguage(TEXT("key149"));

		m_TextMaterial->SetText(FText::FromString(mTextMaterial));

	}

	//	花型按钮--点击事件.
	UUIManager::GetInstance()->RegisterButtonClick(1, m_BtnPattern, this, &UWin_DetailPage::OnSelectPattern);

	//	上传花型按钮--点击事件.
	UUIManager::GetInstance()->RegisterButtonClick(2, m_BtnUpload, this, &UWin_DetailPage::OnSelectPattern);

	//	编辑花型按钮--点击事件.
	UUIManager::GetInstance()->RegisterButtonClick(3, m_BtnOperate, this, &UWin_DetailPage::OnSelectPattern);

	//	颜色按钮--点击事件.
	UUIManager::GetInstance()->RegisterButtonClick(1, m_BtnColor, this, &UWin_DetailPage::OnSelectColor);

	//	面料按钮--点击事件.
	UUIManager::GetInstance()->RegisterButtonClick(1, m_BtnMaterial, this, &UWin_DetailPage::OnSelectMaterial);

	//	购买按钮--点击事件.
	UUIManager::GetInstance()->RegisterButtonClick(1, m_Btn_Buy, this, &UWin_DetailPage::OnBuyGoods);

	//	本地监听消息回调.
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, m_DesignModule->m_Http_UploadPattern_Pid, this, &UWin_DetailPage::OnReloadPattern);
	
	//	本地监听消息回调.
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, TSuitPatternManager::GetInstance()->m_downloadPatternEvent, this, &UWin_DetailPage::OnAddPattern);

	//	本地监听消息回调.
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, m_DesignModule->m_Http_BuyInfo, this, &UWin_DetailPage::SetBuyInfo);

	//	本地监听消息回调.
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, 4028, this, &UWin_DetailPage::OnResponseChangeFabric);

	return true;
}

void UWin_DetailPage::OnInit()
{
	//	DetailCanvas.
	if (UCanvasPanel* CanvDetail = Cast<UCanvasPanel>(GetWidgetFromName("MainPanel")))
	{
		m_CanvasDetail = CanvDetail;
	}

	//	花型按钮.
	if (UButton* BtnPattern = Cast<UButton>(GetWidgetFromName("Button_Pattern")))
	{
		m_BtnPattern = BtnPattern;
	}

	//	上传花型按钮.
	if (UButton* BtnUpload = Cast<UButton>(GetWidgetFromName("Button_UploadPattern")))
	{
		m_BtnUpload = BtnUpload;
	}

	//	编辑花型按钮.
	if (UButton* BtnOperate = Cast<UButton>(GetWidgetFromName("Button_OperatePattern")))
	{
		m_BtnOperate = BtnOperate;
	}

	//	颜色按钮.
	if (UButton* BtnColor = Cast<UButton>(GetWidgetFromName("Button_Color")))
	{
		m_BtnColor = BtnColor;
	}

	//	面料按钮.
	if (UButton* BtnMaterial = Cast<UButton>(GetWidgetFromName("Button_Material")))
	{
		m_BtnMaterial = BtnMaterial;
	}

	//	花型Canvas.
	if (UCanvasPanel * mTemp = Cast<UCanvasPanel>(GetWidgetFromName("CanvasPanel_Pattern")))
	{
		m_CanvasPattern = mTemp;
	}

	//	花型ScrollBox.
	if (UScrollBox * mTemp = Cast<UScrollBox>(GetWidgetFromName("ScrollBox_Pattern")))
	{
		m_ScrollPattern = mTemp;
	}

	//	花型列表.
	if (UGridPanel * mTemp = Cast<UGridPanel>(GetWidgetFromName("GridPanel_Pattern")))
	{
		m_GridPattern = mTemp;
	}

	//	颜色ScrollBox.
	if (UScrollBox * mTemp = Cast<UScrollBox>(GetWidgetFromName("ScrollBox_Color")))
	{
		m_ScrollColor = mTemp;
	}

	//	颜色列表.
	if (UGridPanel * mTemp = Cast<UGridPanel>(GetWidgetFromName("GridPanel_Color")))
	{
		m_GridColor = mTemp;
	}

	//	面料ScrollBox.
	if (UScrollBox * mTemp = Cast<UScrollBox>(GetWidgetFromName("ScrollBox_Material")))
	{
		m_ScrollMaterial = mTemp;
	}

	//	面料列表.
	if (UGridPanel * mTemp = Cast<UGridPanel>(GetWidgetFromName("GridPanel_Material")))
	{
		m_GridMaterial = mTemp;
	}

	//	加载图标.
	if (UCircularThrobber * mTemp = Cast<UCircularThrobber>(GetWidgetFromName("CircularThrobber_Loading")))
	{
		m_CircularLoading = mTemp;
	}

	//	购买消息.
	if (UCanvasPanel * mTemp = Cast<UCanvasPanel>(GetWidgetFromName("FabricDetail")))
	{
		m_Cav_Buy = mTemp;
	}
	if (UTextBlock * mTemp = Cast<UTextBlock>(GetWidgetFromName("Title")))
	{
		m_Text_Good = mTemp;
	}
	if (UTextBlock * mTemp = Cast<UTextBlock>(GetWidgetFromName("sn_text")))
	{
		m_Text_SN = mTemp;
	}
	if (UTextBlock * mTemp = Cast<UTextBlock>(GetWidgetFromName("color")))
	{
		m_Text_Color = mTemp;
	}
	if (UTextBlock * mTemp = Cast<UTextBlock>(GetWidgetFromName("panton")))
	{
		m_Text_Panton = mTemp;
	}
	if (UTextBlock * mTemp = Cast<UTextBlock>(GetWidgetFromName("priceTitle1")))
	{
		m_Text_Price1_Name = mTemp;
	}
	if (UTextBlock * mTemp = Cast<UTextBlock>(GetWidgetFromName("promotionPrice1")))
	{
		m_Text_PromotionPrice1 = mTemp;
	}
	if (UTextBlock * mTemp = Cast<UTextBlock>(GetWidgetFromName("price1")))
	{
		m_Text_Price1 = mTemp;
	}
	if (UTextBlock * mTemp = Cast<UTextBlock>(GetWidgetFromName("priceTitle2")))
	{
		m_Text_Price2_Name = mTemp;
	}
	if (UTextBlock * mTemp = Cast<UTextBlock>(GetWidgetFromName("promotionPrice2")))
	{
		m_Text_PromotionPrice2 = mTemp;
	}
	if (UTextBlock * mTemp = Cast<UTextBlock>(GetWidgetFromName("price2")))
	{
		m_Text_Price2 = mTemp;
	}
	if (UTextBlock * mTemp = Cast<UTextBlock>(GetWidgetFromName("priceTitle3")))
	{
		m_Text_Price3_Name = mTemp;
	}
	if (UTextBlock * mTemp = Cast<UTextBlock>(GetWidgetFromName("promotionPrice3")))
	{
		m_Text_PromotionPrice3 = mTemp;
	}
	if (UTextBlock * mTemp = Cast<UTextBlock>(GetWidgetFromName("price3")))
	{
		m_Text_Price3 = mTemp;
	}
	if (UImage * mTemp = Cast<UImage>(GetWidgetFromName("one-")))
	{
		m_Img_One = mTemp;
	}
	if (UImage * mTemp = Cast<UImage>(GetWidgetFromName("two-")))
	{
		m_Img_Two = mTemp;
	}
	if (UImage * mTemp = Cast<UImage>(GetWidgetFromName("three-")))
	{
		m_Img_Three = mTemp;
	}
	if (UVerticalBox * mTemp = Cast<UVerticalBox>(GetWidgetFromName(TEXT("样布"))))
	{
		m_VerBox_One = mTemp;
	}
	if (UVerticalBox * mTemp = Cast<UVerticalBox>(GetWidgetFromName(TEXT("大货期货"))))
	{
		m_VerBox_Two = mTemp;
	}
	if (UVerticalBox * mTemp = Cast<UVerticalBox>(GetWidgetFromName(TEXT("大货现货"))))
	{
		m_VerBox_Three = mTemp;
	}
	if (UButton * mTemp = Cast<UButton>(GetWidgetFromName("Btn_Buy")))
	{
		m_Btn_Buy = mTemp;
	}
	if (UWebBrowser * mTemp = Cast<UWebBrowser>(GetWidgetFromName("WebBrowser_Good_Icon")))
	{
		m_Web_Icon = mTemp;
	}
	if (UImage * mTemp = Cast<UImage>(GetWidgetFromName("WebBrowser_icon_Mask")))
	{
		m_Good_Icon = mTemp;
	}
}

void UWin_DetailPage::OnRelease()
{
	UUIManager::GetInstance()->RemoveButtonClick(m_BtnPattern);

	UUIManager::GetInstance()->RemoveButtonClick(m_BtnColor);

	UUIManager::GetInstance()->RemoveButtonClick(m_BtnMaterial);

	UUIManager::GetInstance()->RemoveButtonClick(m_BtnUpload);

	UUIManager::GetInstance()->RemoveButtonClick(m_BtnOperate);

	UUIManager::GetInstance()->RemoveButtonClick(m_Btn_Buy);

	//	本地监听消息回调.
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, m_DesignModule->m_Http_UploadPattern_Pid, this, &UWin_DetailPage::OnReloadPattern);
	
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, TSuitPatternManager::GetInstance()->m_downloadPatternEvent, this, &UWin_DetailPage::OnAddPattern);

	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, m_DesignModule->m_Http_BuyInfo, this, &UWin_DetailPage::SetBuyInfo);

	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, 4028, this, &UWin_DetailPage::OnResponseChangeFabric);

	if (nullptr != m_DownloadImageTool)
	{
		m_DownloadImageTool->CancelDownload();

		m_DownloadImageTool = nullptr;
	}
}

void UWin_DetailPage::OnSetParams(int32 _pid, FString & _content, ...)
{

}

void UWin_DetailPage::OnTick(float InDeltaTimeSecond)
{

}

void UWin_DetailPage::OnBackstageUI()
{
	
}

void UWin_DetailPage::OnSelectPattern(int32 _pid)
{
	LogicDesignScene * mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();

	UWin_DesignUMG * mDesignUMG = nullptr;

	if (mLogicDesignScene != nullptr)
	{
		mDesignUMG = (UWin_DesignUMG *)(mLogicDesignScene->GetCurrentUIController()->GetRootUI());
	}

	if (_pid == 1)
	{
		mDesignUMG->m_DetailPage->m_Cav_Buy->SetVisibility(ESlateVisibility::Hidden);

		InitPatternData(m_Type, m_Style_Tid, m_usedFabricStruct);
	}
	else if (_pid == 2)
	{
		//	调用照相机.
#if PLATFORM_ANDROID
		m_DesignModule->m_UploadPattern = true;

		m_PhotoPage = (UWin_PhotoPage *)UUIManager::GetInstance()->OpenUI(TEXT("UI_Photo"));

		m_PhotoPage->AddToViewport();

#elif PLATFORM_IOS
		m_PhotoPage = (UWin_PhotoPage *)UUIManager::GetInstance()->OpenUI(TEXT("UI_Photo"));

		m_PhotoPage->AddToViewport();

		m_PhotoPage->SetVisibility(ESlateVisibility::Hidden);

		m_PhotoPage->GetUploadPattern();
#endif

#if PLATFORM_WINDOWS
		m_PhotoPage = (UWin_PhotoPage *)UUIManager::GetInstance()->OpenUI(TEXT("UI_Photo"));

		m_PhotoPage->AddToViewport();

		m_PhotoPage->SetVisibility(ESlateVisibility::Hidden);

		m_PhotoPage->GetUploadPattern();
#endif
	}
	else if (_pid == 3)
	{
		//	编辑花型.
		FString mTipContent = GLanguageTool::GetInstance()->GetLanguage(TEXT("key145"));

		mDesignUMG->m_TextTip->SetText(FText::FromString(mTipContent));

		//显示tip.
		mDesignUMG->m_Canv_Tip->SetVisibility(ESlateVisibility::Visible);

		//设置定时器两秒后隐藏.
		GetWorld()->GetTimerManager().SetTimer(m_TimerHandle, this, &UWin_DetailPage::HiddenTip, 1.0f, false);
	}
}

void UWin_DetailPage::OnSelectColor(int32 _pid)
{
	LogicDesignScene * mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();

	UWin_DesignUMG * mDesignUMG = nullptr;

	if (mLogicDesignScene != nullptr)
	{
		mDesignUMG = (UWin_DesignUMG *)(mLogicDesignScene->GetCurrentUIController()->GetRootUI());
	}

	mDesignUMG->m_DetailPage->m_Cav_Buy->SetVisibility(ESlateVisibility::Hidden);

	m_CanvasPattern->SetVisibility(ESlateVisibility::Hidden);

	m_ScrollMaterial->SetVisibility(ESlateVisibility::Hidden);

	m_ScrollColor->SetVisibility(ESlateVisibility::Visible);

	m_BtnPattern->SetIsEnabled(true);

	m_BtnColor->SetIsEnabled(false);

	m_BtnMaterial->SetIsEnabled(true);

	InitColorData(m_Type, m_usedFabricStruct);
}

void UWin_DetailPage::OnSelectMaterial(int32 _pid)
{
	LogicDesignScene * mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();

	UWin_DesignUMG * mDesignUMG = nullptr;

	if (mLogicDesignScene != nullptr)
	{
		mDesignUMG = (UWin_DesignUMG *)(mLogicDesignScene->GetCurrentUIController()->GetRootUI());
	}

	mDesignUMG->m_DetailPage->m_Cav_Buy->SetVisibility(ESlateVisibility::Hidden);

	m_CanvasPattern->SetVisibility(ESlateVisibility::Hidden);

	m_ScrollColor->SetVisibility(ESlateVisibility::Hidden);

	m_ScrollMaterial->SetVisibility(ESlateVisibility::Visible);

	m_BtnPattern->SetIsEnabled(true);

	m_BtnColor->SetIsEnabled(true);

	m_BtnMaterial->SetIsEnabled(false);

	InitFabricData(m_Type, m_usedFabricStruct);
}

//	切换花型高亮.
void UWin_DetailPage::GetSelectPatternNum(int32 _select){
	if (m_SelectPatternId != _select)
	{
		if(m_SelectPatternId != -1 && m_SelectPatternId < m_DetailCell_Map.Num())
			m_DetailCell_Map[m_SelectPatternId]->m_Image_select->SetVisibility(ESlateVisibility::Hidden);

		m_SelectPatternId = _select;
	}
}

//	切换颜色高亮.
void UWin_DetailPage::GetSelectColorNum(int32 _select)
{
	if (m_SelectColorId != _select)
	{
		if(m_SelectColorId != -1 && m_SelectColorId < m_DetailCell_Map.Num())
			m_DetailCell_Map[m_SelectColorId]->m_Image_select->SetVisibility(ESlateVisibility::Hidden);

		m_SelectColorId = _select;
	}
}

void UWin_DetailPage::GetSelectMaterialNum(int32 _select)
{
	if (m_SelectMaterialId != _select)
	{
		if (m_SelectMaterialId != -1 && m_SelectMaterialId < m_DetailCell_Map.Num())
			m_DetailCell_Map[m_SelectMaterialId]->m_Image_select->SetVisibility(ESlateVisibility::Hidden);

		m_SelectMaterialId = _select;
	}
}

void UWin_DetailPage::InitFabricData(EnumDataType _Type, UsedFabricStruct & _usedFabricStruct)
{
	//	设置显示.
	m_SelectMaterialId = -1;

	m_DetailCell_Map.Empty();

	m_GridMaterial->ClearChildren();

	//	面料.
	SuitFabricT * mSuitFabric = TSuitFabricManager::GetInstance()->GetTemplateByPID<SuitFabricT>(_usedFabricStruct.m_fabricTid);

	//	判断是否有花型.
	if (nullptr != mSuitFabric && mSuitFabric->m_CanPattern == true)
	{
		//	显示花型按钮.
		UHorizontalBoxSlot * mSlot = (UHorizontalBoxSlot*)m_BtnPattern->Slot;

		mSlot->SetPadding(FMargin(0.0f, 0.0f, 10.0f, 0.0f));

		FSlateChildSize  mSlate;

		mSlate.Value = 1.0;

		mSlate.SizeRule = ESlateSizeRule::Automatic;

		mSlot->SetSize(mSlate);

		m_TextPattern->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else if (nullptr != mSuitFabric && mSuitFabric->m_CanPattern == false)
	{
		//	隐藏花型按钮.
		UHorizontalBoxSlot * mSlot = (UHorizontalBoxSlot*)m_BtnPattern->Slot;

		mSlot->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 0.0f));

		FSlateChildSize  mSlate;

		mSlate.Value = 0.0f;

		mSlate.SizeRule = ESlateSizeRule::Fill;
		
		mSlot->SetSize(mSlate);

		m_TextPattern->SetVisibility(ESlateVisibility::Hidden);
	}

	//OnSetDetailShop();

	//	面料Tid.
	TArray<int32> mFabricTidArray;

	if (EnumDataType::TYPE_STYLE_SLICE == _Type)
	{
		//	获取片的面料Tid.
		SuitSliceT * mSuitSlice = TSuitSliceManager::GetInstance()->GetTemplateByPID<SuitSliceT>(m_Style_Tid);

		SuitFabricLibraryT * mFabrics = TSuitFabricLibraryManager::GetInstance()->GetTemplateByPID<SuitFabricLibraryT>(mSuitSlice->m_FabricLibId);

		mFabricTidArray = mFabrics->m_Fabrics;
	}
	else if (EnumDataType::TYPE_STYLE_COMPONENT == _Type || EnumDataType::TYPE_STYLE_ALL == _Type)
	{
		mFabricTidArray = _usedFabricStruct.m_fabricTidArray;
	}

	//	面料库中的面料.
	TArray<SuitFabricT*> mSuitFabricTarray;

	for (size_t i = 0; i < mFabricTidArray.Num(); ++i)
	{
		SuitFabricT * mFabric = TSuitFabricManager::GetInstance()->GetTemplateByPID<SuitFabricT>(mFabricTidArray[i]);

		mSuitFabricTarray.Add(mFabric);

		//	显示面料.
		UWin_UMGDetailCell * mTemp = (UWin_UMGDetailCell *)UUIManager::GetInstance()->OpenUI(TEXT("UI_DetailCell"));

		mTemp->m_SelectType = 3;

		mTemp->m_Style_Tid = m_Style_Tid;

		mTemp->m_Type = _Type;

		mTemp->m_usedFabricStruct = _usedFabricStruct;

		mTemp->m_MaterialId = mFabricTidArray[i];

		mTemp->m_SelectId = m_DetailCell_Map.Num();

		//mTemp->SetMaterial(mFabric->m_MaskUrl);

		if (mFabric->m_IconUrl.Len() > 0)
		{
			mTemp->m_IsMetal = true;

			mTemp->SetMaterial(mFabric->m_IconUrl);
		}

		else if (mFabric->m_IconUrl.Len() == 0)
		{
			SuitFabricColorT* mFabricColor = TSuitFabricManager::GetInstance()->GetDefaultFabricColor(mFabric->GetTid());
			
			if (mFabricColor->m_icon.Len() > 0)
			{
				mTemp->m_IsMetal = true;

				mTemp->SetMaterial(mFabricColor->m_icon);
			}
			else
			{
				mTemp->m_IsMetal = false;

				mTemp->SetMaterial(mFabric->m_MaskUrl);

				mTemp->SetColor(mFabricColor->m_RGB);
			}
		}

		mTemp->SetPadding(FMargin(0.0f, 0.0f, 5.0f, 5.0f));

		m_DetailCell_Map.Add(m_DetailCell_Map.Num(), mTemp);

		if (m_SelectMaterialId == -1 && _usedFabricStruct.m_fabricTid == mFabricTidArray[i])
		{
			m_DetailCell_Map[i]->m_Image_select->SetVisibility(ESlateVisibility::Visible);

			m_SelectMaterialId = i;
		}

		UGridSlot * mGridSlot = m_GridMaterial->AddChildToGrid(mTemp);

		mGridSlot->SetRow(i / 2);

		mGridSlot->SetColumn(i % 2);
	}
	if (m_SelectMaterialId != -1 && _usedFabricStruct.m_fabricTid != 0 && m_SelectMaterialId < m_DetailCell_Map.Num())
	{
		m_DetailCell_Map[m_SelectMaterialId]->m_Image_select->SetVisibility(ESlateVisibility::Visible);
	}
}

void UWin_DetailPage::InitColorData(EnumDataType _Type, UsedFabricStruct & _usedFabricStruct)
{
	if (m_DesignModule != nullptr)
	{
		//	初始化.
		m_SelectColorId = -1;

		m_DetailCell_Map.Empty();

		m_GridColor->ClearChildren();

		//	获取颜色库.
		SuitFabricT * mSuitFabric = TSuitFabricManager::GetInstance()->GetTemplateByPID<SuitFabricT>(_usedFabricStruct.m_fabricTid);

		SuitFabricColorLibraryT * mColors = TSuitFabricColorLibraryManager::GetInstance()->GetTemplateByPID<SuitFabricColorLibraryT>(mSuitFabric->m_ColorRepositoryId);

		TArray<SuitFabricColorT*> mSuitColorTarray;

		OnSetDetailShop();

		for (int32 i = 0; i < mColors->m_fabricColorIds.Num(); ++i)
		{
			//	加载颜色.
			SuitFabricColorT * mSuitColor = TSuitFabricColorManager::GetInstance()->GetTemplateByPID<SuitFabricColorT>(mColors->m_fabricColorIds[i]);

			mSuitColorTarray.Add(mSuitColor);

			UWin_UMGDetailCell * mTemp = (UWin_UMGDetailCell *)UUIManager::GetInstance()->OpenUI(TEXT("UI_DetailCell"));

			mTemp->m_SelectType = 2;

			mTemp->m_Style_Tid = m_Style_Tid;

			mTemp->m_Type = _Type;

			mTemp->m_usedFabricStruct = _usedFabricStruct;

			mTemp->m_ColorGoodsId = FString::FromInt(mSuitColor->GetTid());

			mTemp->m_ColorId = mColors->m_fabricColorIds[i];

			mTemp->m_SelectId = m_DetailCell_Map.Num();

			//mTemp->SetMaterial(mSuitFabric->m_MaskUrl);

			if (mSuitColor->m_icon.Len() > 0)
			{
				mTemp->m_IsMetal = true;

				mTemp->SetMaterial(mSuitColor->m_icon);
			}

			else if (mSuitColor->m_icon.Len() == 0)
			{
				mTemp->m_IsMetal = false;

				mTemp->SetMaterial(mSuitFabric->m_MaskUrl);

				mTemp->SetColor(mSuitColor->m_RGB);
			}

			mTemp->SetPadding(FMargin(0.0f, 0.0f, 4.0f, 4.0f));

			m_DetailCell_Map.Add(m_DetailCell_Map.Num(), mTemp);

			if (m_SelectColorId == -1 && _usedFabricStruct.m_colorTid == mColors->m_fabricColorIds[i])
			{
				m_DetailCell_Map[i]->m_Image_select->SetVisibility(ESlateVisibility::Visible);

				m_SelectColorId = i;
			}

			UGridSlot * mGridSlot = m_GridColor->AddChildToGrid(mTemp);

			mGridSlot->SetRow(i / 2);

			mGridSlot->SetColumn(i % 2);
		}

		if (m_SelectColorId != -1 && _usedFabricStruct.m_colorTid != 0 && m_SelectColorId < m_DetailCell_Map.Num())
		{
			m_DetailCell_Map[m_SelectColorId]->m_Image_select->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UWin_DetailPage::InitPatternData(EnumDataType _Type, int32 _SliceTid, UsedFabricStruct & _usedFabricStruct)
{
	//	保存参数.
	m_SelectPatternId = -1;

	m_PageIndex = 1;

	m_usedFabricStruct = _usedFabricStruct;

	m_Type = _Type;

	m_Style_Tid = _SliceTid;

	//	面料.
	SuitFabricT * mSuitFabric = TSuitFabricManager::GetInstance()->GetTemplateByPID<SuitFabricT>(_usedFabricStruct.m_fabricTid);

	//	初始化RGB.
// 	SuitFabricColorLibraryT * mColors = TSuitFabricColorLibraryManager::GetInstance()->GetTemplateByPID<SuitFabricColorLibraryT>(mSuitFabric->m_ColorRepositoryId);
// 
// 	SuitFabricColorT * mSuitColor = TSuitFabricColorManager::GetInstance()->GetTemplateByPID<SuitFabricColorT>(mColors->m_fabricColorIds[0]);
// 
// 	m_RGB = mSuitColor->m_RGB;

	//	判断是否是辅料.
	LogicDesignScene * mDesignScene = (LogicDesignScene *)UGameDirector::GetInstance()->GetCurrentLogicScene();

	EnumComponentType mComponentType = mDesignScene->GetComponentTypeByFabricId(_usedFabricStruct.m_fabricTid);

	if (EnumComponentType::ACCESSORIES_COMPONENT == mComponentType)
	{
		FString mText = GLanguageTool::GetInstance()->GetLanguage(TEXT("key150"));

		m_TextMaterial->SetText(FText::FromString(mText));
	}

	else
	{
		FString mText = GLanguageTool::GetInstance()->GetLanguage(TEXT("key149"));

		m_TextMaterial->SetText(FText::FromString(mText));
	}

	//	判断是否有花型.
	if (nullptr != mSuitFabric && mSuitFabric->m_CanPattern == true)
	{
		//	显示花型按钮.
		UHorizontalBoxSlot * mSlot = (UHorizontalBoxSlot*)m_BtnPattern->Slot;

		mSlot->SetPadding(FMargin(0.0f, 0.0f, 10.0f, 0.0f));

		FSlateChildSize  mSlate;

		mSlate.Value = 1.0;

		mSlate.SizeRule = ESlateSizeRule::Automatic;

		mSlot->SetSize(mSlate);

		m_TextPattern->SetVisibility(ESlateVisibility::HitTestInvisible);

		m_CanvasPattern->SetVisibility(ESlateVisibility::Visible);

		m_ScrollMaterial->SetVisibility(ESlateVisibility::Hidden);

		m_ScrollColor->SetVisibility(ESlateVisibility::Hidden);

		m_BtnPattern->SetIsEnabled(false);

		m_BtnColor->SetIsEnabled(true);

		m_BtnMaterial->SetIsEnabled(true);

		m_DetailCell_Map.Empty();

		m_GridPattern->ClearChildren();

		m_ScrollPattern->ScrollToStart();

		TArray<FString> mPatternIds;

		LogicDesignScene * mScene = (LogicDesignScene *)UGameDirector::GetInstance()->GetCurrentLogicScene();

		mScene->GetAllUsedPatternIds(mPatternIds);

		TSuitPatternManager::GetInstance()->GetPatternByIndex(1, 16, mPatternIds, _usedFabricStruct.m_fabricTid);

		OnSetDetailShop();
	}
	else if (nullptr != mSuitFabric && mSuitFabric->m_CanPattern == false)
	{
		//	隐藏花型按钮.
		UHorizontalBoxSlot * mSlot = (UHorizontalBoxSlot*)m_BtnPattern->Slot;

		mSlot->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 0.0f));

		FSlateChildSize  mSlate;

		mSlate.Value = 0.0;

		mSlate.SizeRule = ESlateSizeRule::Fill;

		mSlot->SetSize(mSlate);

		m_TextPattern->SetVisibility(ESlateVisibility::Hidden);

		m_CanvasPattern->SetVisibility(ESlateVisibility::Hidden);

		OnSelectColor(1);
	}
}

//	上传自定义花型的本地消息回调.
void UWin_DetailPage::OnReloadPattern(msg_ptr _msg)
{
	UE_LOG(LogTemp, Log, TEXT("zhoujia:OnUploadPattern Json Success!"));

	MsgLocal * mLocal = _msg->ConvertMsg<MsgLocal>();

	UUIManager::GetInstance()->RemoveAllAlertUI();

	m_PageIndex = 1;

	m_DetailCell_Map.Empty();

	m_GridPattern->ClearChildren();
	   
	m_ScrollPattern->ScrollToStart();

	m_UserPattern = true;

	FString mNewPatternId = *(FString*)mLocal->Content();

	TArray<FString> mPatternIds;

	mPatternIds.Add(mNewPatternId);

	LogicDesignScene * mDesignScene = (LogicDesignScene *)UGameDirector::GetInstance()->GetCurrentLogicScene();

	mDesignScene->GetAllUsedPatternIds(mPatternIds);

	TSuitPatternManager::GetInstance()->GetPatternByIndex(1, 16, mPatternIds, m_usedFabricStruct.m_fabricTid);
}

//	加载花型，本地监听回调.
void UWin_DetailPage::OnAddPattern(msg_ptr _msg)
{
	//换成本地消息.
	MsgLocal * mMsgLocal = _msg->ConvertMsg<MsgLocal>();

	//获取原数据.
	LoadPatternStruct * mStruct = (LoadPatternStruct *)mMsgLocal->Content();

	if (mStruct->m_patternArray.Num() == 0)
		return;

	TArray<SuitPatternT*> mPatternArray = mStruct->m_patternArray;

	//追加新页起始位置.
	int32  mIndex=m_DetailCell_Map.Num();

	for (size_t i = 0; i < mPatternArray.Num(); ++i)
	{
		UWin_UMGDetailCell * mTemp = (UWin_UMGDetailCell *)UUIManager::GetInstance()->OpenUI(TEXT("UI_DetailCell"));

		mTemp->m_SelectType = 1;

		mTemp->m_Style_Tid = m_Style_Tid;

		mTemp->m_Type = m_Type;

		mTemp->m_usedFabricStruct = m_usedFabricStruct;

		mTemp->m_patternId = mPatternArray[i]->m_patternId;	//	花型Id,请求商品消息.

		mTemp->m_OwnerType = mPatternArray[i]->m_ownerType;	//	花型类型(系统还是自定义).

		mTemp->m_SelectId = m_DetailCell_Map.Num();			//	花型列表中的位置，设置高亮.

		mTemp->SetTexture2D(mPatternArray[i]->m_iconUrl);	//	设置花型.

		mTemp->SetPadding(FMargin(0.0f, 0.0f, 4.0f, 4.0f));

		m_DetailCell_Map.Add(m_DetailCell_Map.Num(), mTemp);

		if (m_SelectPatternId == -1 && m_usedFabricStruct.m_patternTid == mPatternArray[i]->m_patternId)
		{
			m_DetailCell_Map[i]->m_Image_select->SetVisibility(ESlateVisibility::Visible);

			m_SelectPatternId = m_DetailCell_Map.Num()-1;
		}
	}

	//添加一页花型.
	for (int32 i = mIndex; i < m_DetailCell_Map.Num(); ++i)
	{
		UGridSlot * mGridSlot = m_GridPattern->AddChildToGrid(m_DetailCell_Map[i]);

		mGridSlot->SetRow(i / 2);

		mGridSlot->SetColumn(i % 2);
	}

	if (m_SelectPatternId != -1 && (!m_usedFabricStruct.m_patternTid.IsEmpty()) && (m_SelectPatternId < m_DetailCell_Map.Num()) && (m_DetailCell_Map.Num() > 0))
	{
		m_DetailCell_Map[m_SelectPatternId]->m_Image_select->SetVisibility(ESlateVisibility::Visible);
	}

	m_ChangePage = true;	//	可以分页.

	//	如果是上传自定义花型，将它选中换上.
	if (m_UserPattern == true)
	{
		m_DetailCell_Map[0]->OnButtonClick(1);

		m_UserPattern = false;
	}
}

//	花型分页.
void UWin_DetailPage::AddPatternPage(int32 _PageIndex)
{
	m_ChangePage = false;

	TArray<FString> mPatternIds;

	LogicDesignScene * mDesignScene = (LogicDesignScene *)UGameDirector::GetInstance()->GetCurrentLogicScene();

	mDesignScene->GetAllUsedPatternIds(mPatternIds);

	TSuitPatternManager::GetInstance()->GetPatternByIndex(_PageIndex, 16, mPatternIds, m_usedFabricStruct.m_fabricTid);
}

void UWin_DetailPage::SetBuyInfo(msg_ptr _msg)
{
	//换成本地消息.
	MsgLocal * mMsgLocal = _msg->ConvertMsg<MsgLocal>();

	//获取原数据.
	GoodInfoStruct * mGoodInfo = (GoodInfoStruct *)mMsgLocal->Content();

	LogicDesignScene * mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();

	UWin_DesignUMG * mDesignUMG = nullptr;

	if (mLogicDesignScene != nullptr)
		mDesignUMG = (UWin_DesignUMG *)(mLogicDesignScene->GetCurrentUIController()->GetRootUI());

	if (mGoodInfo->m_isBuy)
	{
		mDesignUMG->m_CanBuy = true;

		if (!mGoodInfo->m_orderUrl.IsEmpty())
			mDesignUMG->SetBuyUrl(mGoodInfo->m_orderUrl);

		m_Good_Icon->SetBrushFromTexture(nullptr);

		//	设置购物消息.
		this->m_Img_One->SetVisibility(ESlateVisibility::Hidden);

		this->m_Img_Two->SetVisibility(ESlateVisibility::Hidden);

		this->m_Img_Three->SetVisibility(ESlateVisibility::Hidden);

		this->m_VerBox_One->SetVisibility(ESlateVisibility::Hidden);

		this->m_VerBox_Two->SetVisibility(ESlateVisibility::Hidden);

		this->m_VerBox_Three->SetVisibility(ESlateVisibility::Hidden);

		this->m_Cav_Buy->SetVisibility(ESlateVisibility::Visible);

		this->m_Text_Good->SetText(FText::FromString(mGoodInfo->m_goodsName));

		this->m_Text_SN->SetText(FText::FromString(mGoodInfo->m_goodsSn));

		this->m_Text_Color->SetText(FText::FromString(mGoodInfo->m_goodsColor));

		this->m_Text_Panton->SetText(FText::FromString(mGoodInfo->m_goodsPanTongCode));

		if (mGoodInfo->m_GoodsPriceStruct.Num() >= 1)
		{
			this->m_VerBox_One->SetVisibility(ESlateVisibility::Visible);

			this->m_Img_One->SetVisibility(ESlateVisibility::Visible);

			this->m_Text_Price1_Name->SetText(FText::FromString(mGoodInfo->m_GoodsPriceStruct[0].m_title + ":"));

			FString mPrice1 = FString::SanitizeFloat(mGoodInfo->m_GoodsPriceStruct[0].m_goodsPrice);

			FString mPromotionPrice1 = FString::SanitizeFloat(mGoodInfo->m_GoodsPriceStruct[0].m_goodsPromotionPrice);

			this->m_Text_PromotionPrice1->SetText(FText::FromString(TEXT("￥") + mPromotionPrice1 + mGoodInfo->m_GoodsPriceStruct[0].m_unit));
		
			this->m_Text_Price1->SetText(FText::FromString(TEXT("￥") + mPrice1));
		}

		if (mGoodInfo->m_GoodsPriceStruct.Num() >= 2)
		{
			this->m_VerBox_Two->SetVisibility(ESlateVisibility::Visible);

			this->m_Img_Two->SetVisibility(ESlateVisibility::Visible);

			this->m_Text_Price2_Name->SetText(FText::FromString(mGoodInfo->m_GoodsPriceStruct[1].m_title + ":"));

			FString mPrice2 = FString::SanitizeFloat(mGoodInfo->m_GoodsPriceStruct[1].m_goodsPrice);

			FString mPromotionPrice2 = FString::SanitizeFloat(mGoodInfo->m_GoodsPriceStruct[1].m_goodsPromotionPrice);

			this->m_Text_PromotionPrice2->SetText(FText::FromString(TEXT("￥") + mPromotionPrice2 + mGoodInfo->m_GoodsPriceStruct[1].m_unit));

			this->m_Text_Price2->SetText(FText::FromString(TEXT("￥") + mPrice2));
		}

		if (mGoodInfo->m_GoodsPriceStruct.Num() >= 3)
		{
			this->m_VerBox_Three->SetVisibility(ESlateVisibility::Visible);

			this->m_Img_Three->SetVisibility(ESlateVisibility::Visible);

			this->m_Text_Price3_Name->SetText(FText::FromString(mGoodInfo->m_GoodsPriceStruct[2].m_title + ":"));

			FString mPrice3 = FString::SanitizeFloat(mGoodInfo->m_GoodsPriceStruct[2].m_goodsPrice);

			FString mPromotionPrice3 = FString::SanitizeFloat(mGoodInfo->m_GoodsPriceStruct[2].m_goodsPromotionPrice);

			this->m_Text_PromotionPrice3->SetText(FText::FromString(TEXT("￥") + mPromotionPrice3 + mGoodInfo->m_GoodsPriceStruct[2].m_unit));

			this->m_Text_Price3->SetText(FText::FromString(TEXT("￥") + mPrice3));
		}

		LogicDesignScene * mDesignScene = (LogicDesignScene *)UGameDirector::GetInstance()->GetCurrentLogicScene();

		DesignUIController*  mDesignController = (DesignUIController *)(mDesignScene->GetCurrentUIController());

		UTexture2D * mDetailTexture = mDesignController->GetTextureManager()->GetTexture2d(mGoodInfo->m_imageUrl);

		if (mDetailTexture == nullptr)
			DownloadIconImage(mGoodInfo->m_imageUrl);
		else
			m_Good_Icon->SetBrushFromTexture(mDetailTexture);
		//this->m_Web_Icon->LoadURL(mGoodInfo->m_imageUrl);
	}
	else
		mDesignUMG->m_CanBuy = false;

	//mDesignUMG->SetDetailShopView(m_IsBuy);
}

void UWin_DetailPage::OnBuyGoods(int32 _pid)
{
	UWin_WebPage * mTemp = (UWin_WebPage *)UUIManager::GetInstance()->OpenUI(TEXT("Web_Fabric"));

	mTemp->AddToViewport();

	LogicDesignScene * mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();

	UWin_DesignUMG * mDesignUMG = nullptr;

	if (mLogicDesignScene != nullptr)
		mDesignUMG = (UWin_DesignUMG *)(mLogicDesignScene->GetCurrentUIController()->GetRootUI());

	if (nullptr != mDesignUMG)
	{
		mDesignUMG->m_DetailPage->m_Cav_Buy->SetVisibility(ESlateVisibility::Hidden);

		mTemp->m_WebBuy->LoadURL(EditorFunctionalTool::GetTicketUrl(mDesignUMG->m_BuyUrl));
	}
}

void UWin_DetailPage::OnResponseChangeFabric(msg_ptr _msg)
{
	if (m_CurrentChangeFabricCell != nullptr)
	{
		UUIManager::GetInstance()->CloseMiddleTip();
		m_CurrentChangeFabricCell->ChangeFabric();
	}
}

//	设置购物车.
void UWin_DetailPage::OnSetDetailShop()
{
	if (m_FirstBatch == true)
	{
		UWin_UMGDetailCell * mTemp = (UWin_UMGDetailCell *)UUIManager::GetInstance()->OpenUI(TEXT("UI_DetailCell"));

		//	显示颜色购物车.
		if (m_usedFabricStruct.m_colorTid != 0)
		{
			SuitFabricColorT * mSuitColor1 = TSuitFabricColorManager::GetInstance()->GetTemplateByPID<SuitFabricColorT>(m_usedFabricStruct.m_colorTid);

			if (nullptr != mSuitColor1)
			{
				mTemp->m_SelectType = 2;

				mTemp->m_ColorGoodsId = FString::FromInt(mSuitColor1->GetTid());

				mTemp->OnButtonClick(1);
			}
		}
		//	显示花型购物车.
		else if (!m_usedFabricStruct.m_patternTid.IsEmpty())
		{
			SuitPatternT * mSuitPattern = TSuitPatternManager::GetInstance()->GetPatternData(m_usedFabricStruct.m_patternTid);

			if (nullptr != mSuitPattern)
			{
				mTemp->m_SelectType = 1;

				mTemp->m_patternId = m_usedFabricStruct.m_patternTid;

				mTemp->m_OwnerType = mSuitPattern->m_ownerType;

				mTemp->OnButtonClick(1);
			}
		}
	}
	m_FirstBatch = false;
}

void UWin_DetailPage::HiddenTip()
{
	LogicDesignScene * mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();

	UWin_DesignUMG * mDesignUMG = nullptr;

	if (mLogicDesignScene != nullptr)
	{
		mDesignUMG = (UWin_DesignUMG *)(mLogicDesignScene->GetCurrentUIController()->GetRootUI());
	}

	//隐藏tip.
	mDesignUMG->m_Canv_Tip->SetVisibility(ESlateVisibility::Hidden);

	GetWorld()->GetTimerManager().ClearTimer(m_TimerHandle);
}

void UWin_DetailPage::DownloadIconImage(FString _imageUrl)
{
	if (m_DownloadImageTool != nullptr)
	{
		m_DownloadImageTool->CancelDownload();
	}
	m_DownloadImageTool = UDownloadImageTool::DownloadImage(1, _imageUrl, this, &UWin_DetailPage::OnDownoloadFinished);

}

void UWin_DetailPage::OnDownoloadFinished(bool _isScuccess, int32 _penetrateId, FString _url, UTexture2D * _texture2d, UDownloadImageTool * _downloadTool)
{
	if (_texture2d->IsValidLowLevel())
	{
		m_Good_Icon->SetBrushFromTexture(_texture2d);

		LogicDesignScene * mDesignScene = (LogicDesignScene *)UGameDirector::GetInstance()->GetCurrentLogicScene();

		DesignUIController*  mDesignController = (DesignUIController *)(mDesignScene->GetCurrentUIController());

		mDesignController->GetTextureManager()->AddTexture2D(_url, _texture2d);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "DetailPage.h"
#include "UIManager.h"
#include "PhotoPage.h"
#include "DesignUMG.h"
#include "SuitFabricT.h"
#include "SuitPatternT.h"
#include "GameDirector.h"
#include "GLanguageTool.h"
#include "UMGDetailCell.h"
#include "LoadAssetTool.h"
#include "GModuleManager.h"
#include "LogicDesignScene.h"
#include "SuitFabricColorT.h"
#include "TSuitSliceManager.h"
#include "TSuitFabricManager.h"
#include "SuitFabricLibraryT.h"
#include "TSuitPatternManager.h"
#include "SuitFabricColorLibraryT.h"
#include "TSuitFabricColorManager.h"
#include "TSuitFabricLibraryManager.h"
#include "TSuitFabricColorLibraryManager.h"
#include "SuitPatternLibraryT.h"

UDetailPage::UDetailPage(const FObjectInitializer& ObjectInitializer)
{
	m_PageIndex = 1;

	m_BtnPattern = nullptr;

	m_GridPattern = nullptr;

	m_ChangePage = true;
}

bool UDetailPage::OnStart()
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
	UUIManager::GetInstance()->RegisterButtonClick(1, m_BtnPattern, this, &UDetailPage::OnSelectPattern);

	//	上传花型按钮--点击事件.
	UUIManager::GetInstance()->RegisterButtonClick(2, m_BtnUpload, this, &UDetailPage::OnSelectPattern);

	//	编辑花型按钮--点击事件.
	UUIManager::GetInstance()->RegisterButtonClick(3, m_BtnOperate, this, &UDetailPage::OnSelectPattern);

	//	颜色按钮--点击事件.
	UUIManager::GetInstance()->RegisterButtonClick(1, m_BtnColor, this, &UDetailPage::OnSelectColor);

	//	面料按钮--点击事件.
	UUIManager::GetInstance()->RegisterButtonClick(1, m_BtnMaterial, this, &UDetailPage::OnSelectMaterial);

	//	本地监听消息回调.
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, m_DesignModule->m_Http_UploadPattern_Pid, this, &UDetailPage::OnReloadPattern);
	
	//	本地监听消息回调.
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, TSuitPatternManager::GetInstance()->m_downloadPatternEvent, this, &UDetailPage::OnAddPattern);

	//	本地监听消息回调.
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, 4028, this, &UDetailPage::OnResponseChangeFabric);

	return true;
}

void UDetailPage::OnInit()
{
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
}

void UDetailPage::OnRelease()
{
	UUIManager::GetInstance()->RemoveButtonClick(m_BtnPattern);

	UUIManager::GetInstance()->RemoveButtonClick(m_BtnColor);

	UUIManager::GetInstance()->RemoveButtonClick(m_BtnMaterial);

	UUIManager::GetInstance()->RemoveButtonClick(m_BtnUpload);

	UUIManager::GetInstance()->RemoveButtonClick(m_BtnOperate);

	//	本地监听消息回调.
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, m_DesignModule->m_Http_UploadPattern_Pid, this, &UDetailPage::OnReloadPattern);
	
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, TSuitPatternManager::GetInstance()->m_downloadPatternEvent, this, &UDetailPage::OnAddPattern);

	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, 4028, this, &UDetailPage::OnResponseChangeFabric);
}

void UDetailPage::OnSetParams(int32 _pid, FString & _content, ...)
{

}

void UDetailPage::OnTick(float InDeltaTimeSecond)
{

}

void UDetailPage::OnBackstageUI()
{
	
}

void UDetailPage::OnSelectPattern(int32 _pid)
{
	if (_pid == 1)
	{
		InitPatternData(m_Type, m_Style_Tid, m_usedFabricStruct);
	}
	else if (_pid == 2)
	{
		//	调用照相机.
#if PLATFORM_ANDROID
		m_DesignModule->m_UploadPattern = true;

		m_PhotoPage = (UPhotoPage *)UUIManager::GetInstance()->OpenUI(TEXT("UI_Photo"));

		m_PhotoPage->AddToViewport();

#elif PLATFORM_IOS
		m_PhotoPage = (UPhotoPage *)UUIManager::GetInstance()->OpenUI(TEXT("UI_Photo"));

		m_PhotoPage->AddToViewport();

		m_PhotoPage->SetVisibility(ESlateVisibility::Hidden);

		m_PhotoPage->GetUploadPattern();
#endif

#if PLATFORM_WINDOWS
		m_PhotoPage = (UPhotoPage *)UUIManager::GetInstance()->OpenUI(TEXT("UI_Photo"));

		m_PhotoPage->AddToViewport();

		m_PhotoPage->SetVisibility(ESlateVisibility::Hidden);

		m_PhotoPage->GetUploadPattern();
#endif
	}
	else if (_pid == 3)
	{
		//	编辑花型.
		FString mTipContent = GLanguageTool::GetInstance()->GetLanguage(TEXT("key145"));

		LogicDesignScene * mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();

		UDesignUMG * mDesignUMG = nullptr;

		if (mLogicDesignScene != nullptr)
		{
			mDesignUMG = (UDesignUMG *)(mLogicDesignScene->GetCurrentUIController()->GetRootUI());
		}

		mDesignUMG->m_TextTip->SetText(FText::FromString(mTipContent));

		//显示tip.
		mDesignUMG->m_Canv_Tip->SetVisibility(ESlateVisibility::Visible);

		//设置定时器两秒后隐藏.
		GetWorld()->GetTimerManager().SetTimer(m_TimerHandle, this, &UDetailPage::HiddenTip, 1.0f, false);
	}
}

void UDetailPage::OnSelectColor(int32 _pid)
{
	m_CanvasPattern->SetVisibility(ESlateVisibility::Hidden);

	m_ScrollMaterial->SetVisibility(ESlateVisibility::Hidden);

	m_ScrollColor->SetVisibility(ESlateVisibility::Visible);

	m_BtnPattern->SetIsEnabled(true);

	m_BtnColor->SetIsEnabled(false);

	m_BtnMaterial->SetIsEnabled(true);

	InitColorData(m_Type, m_usedFabricStruct);
}

void UDetailPage::OnSelectMaterial(int32 _pid)
{
	m_CanvasPattern->SetVisibility(ESlateVisibility::Hidden);

	m_ScrollColor->SetVisibility(ESlateVisibility::Hidden);

	m_ScrollMaterial->SetVisibility(ESlateVisibility::Visible);

	m_BtnPattern->SetIsEnabled(true);

	m_BtnColor->SetIsEnabled(true);

	m_BtnMaterial->SetIsEnabled(false);

	InitFabricData(m_Type, m_usedFabricStruct);
}

//	切换花型高亮.
void UDetailPage::GetSelectPatternNum(int32 _select){
	if (m_SelectPatternId != _select)
	{
		if(m_SelectPatternId != -1 && m_SelectPatternId < m_DetailCell_Map.Num())
			m_DetailCell_Map[m_SelectPatternId]->m_Image_select->SetVisibility(ESlateVisibility::Hidden);

		m_SelectPatternId = _select;
	}
}

//	切换颜色高亮.
void UDetailPage::GetSelectColorNum(int32 _select)
{
	if (m_SelectColorId != _select)
	{
		if(m_SelectColorId != -1 && m_SelectColorId < m_DetailCell_Map.Num())
			m_DetailCell_Map[m_SelectColorId]->m_Image_select->SetVisibility(ESlateVisibility::Hidden);

		m_SelectColorId = _select;
	}
}

void UDetailPage::GetSelectMaterialNum(int32 _select)
{
	if (m_SelectMaterialId != _select)
	{
		if (m_SelectMaterialId != -1 && m_SelectMaterialId < m_DetailCell_Map.Num())
			m_DetailCell_Map[m_SelectMaterialId]->m_Image_select->SetVisibility(ESlateVisibility::Hidden);

		m_SelectMaterialId = _select;
	}
}

void UDetailPage::InitFabricData(EnumDataType _Type, UsedFabricStruct & _usedFabricStruct)
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

	OnSetDetailShop();

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
		UUMGDetailCell * mTemp = (UUMGDetailCell *)UUIManager::GetInstance()->OpenUI(TEXT("UI_DetailCell"));

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
			SuitFabricColorT* mFabricColor = TSuitFabricManager::GetInstance()->GetDefaultFabricColor( mFabric->GetTid());

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

		mGridSlot->SetRow(i / 5);

		mGridSlot->SetColumn(i % 5);
	}
	if (m_SelectMaterialId != -1 && _usedFabricStruct.m_fabricTid != 0 && m_SelectMaterialId < m_DetailCell_Map.Num())
	{
		m_DetailCell_Map[m_SelectMaterialId]->m_Image_select->SetVisibility(ESlateVisibility::Visible);
	}
}

void UDetailPage::InitColorData(EnumDataType _Type, UsedFabricStruct & _usedFabricStruct)
{
	if (m_DesignModule != nullptr)
	{
		//	初始化.
		m_SelectColorId = -1;

		m_DetailCell_Map.Empty();

		m_GridColor->ClearChildren();

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

		//	获取颜色库.
		//SuitFabricT * mSuitFabric = TSuitFabricManager::GetInstance()->GetTemplateByPID<SuitFabricT>(_usedFabricStruct.m_fabricTid);

		SuitFabricColorLibraryT * mColors = TSuitFabricColorLibraryManager::GetInstance()->GetTemplateByPID<SuitFabricColorLibraryT>(mSuitFabric->m_ColorRepositoryId);

		TArray<SuitFabricColorT*> mSuitColorTarray;

		OnSetDetailShop();

		for (int32 i = 0; i < mColors->m_fabricColorIds.Num(); ++i)
		{
			//	加载颜色.
			SuitFabricColorT * mSuitColor = TSuitFabricColorManager::GetInstance()->GetTemplateByPID<SuitFabricColorT>(mColors->m_fabricColorIds[i]);

			mSuitColorTarray.Add(mSuitColor);

			UUMGDetailCell * mTemp = (UUMGDetailCell *)UUIManager::GetInstance()->OpenUI(TEXT("UI_DetailCell"));

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

			mGridSlot->SetRow(i / 5);

			mGridSlot->SetColumn(i % 5);
		}

		if (m_SelectColorId != -1 && _usedFabricStruct.m_colorTid != 0 && m_SelectColorId < m_DetailCell_Map.Num())
		{
			m_DetailCell_Map[m_SelectColorId]->m_Image_select->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UDetailPage::InitPatternData(EnumDataType _Type, int32 _SliceTid, UsedFabricStruct & _usedFabricStruct)
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

		mSlot->SetSize(mSlate);

		m_TextPattern->SetVisibility(ESlateVisibility::Hidden);

		m_CanvasPattern->SetVisibility(ESlateVisibility::Hidden);

		OnSelectColor(1);
	}
}

//	上传自定义花型的本地消息回调.
void UDetailPage::OnReloadPattern(msg_ptr _msg)
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
void UDetailPage::OnAddPattern(msg_ptr _msg)
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
		UUMGDetailCell * mTemp = (UUMGDetailCell *)UUIManager::GetInstance()->OpenUI(TEXT("UI_DetailCell"));

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

			m_SelectPatternId = i;
		}
	}

	//添加一页花型.
	for (int32 i = mIndex; i < m_DetailCell_Map.Num(); i++)
	{
		UGridSlot * mGridSlot = m_GridPattern->AddChildToGrid(m_DetailCell_Map[i]);

		mGridSlot->SetRow(i / 4);

		mGridSlot->SetColumn(i % 4);
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
void UDetailPage::AddPatternPage(int32 _PageIndex)
{
	m_ChangePage = false;

	TArray<FString> mPatternIds;

	LogicDesignScene * mDesignScene = (LogicDesignScene *)UGameDirector::GetInstance()->GetCurrentLogicScene();

	mDesignScene->GetAllUsedPatternIds(mPatternIds);

	TSuitPatternManager::GetInstance()->GetPatternByIndex(_PageIndex, 16, mPatternIds, m_usedFabricStruct.m_fabricTid);
}

//	设置购物车.
void UDetailPage::OnSetDetailShop()
{
	if (m_FirstBatch == true)
	{
		UUMGDetailCell * mTemp = (UUMGDetailCell *)UUIManager::GetInstance()->OpenUI(TEXT("UI_DetailCell"));

		//	显示颜色购物车.
		if (m_usedFabricStruct.m_colorTid != 0)
		{
			SuitFabricColorT * mSuitColor1 = TSuitFabricColorManager::GetInstance()->GetTemplateByPID<SuitFabricColorT>(m_usedFabricStruct.m_colorTid);

			mTemp->m_SelectType = 2;

			mTemp->m_ColorGoodsId = FString::FromInt(mSuitColor1->GetTid());

			if (mSuitColor1->m_icon.Len() <= 0)
				mTemp->SetColor(mSuitColor1->m_RGB);

			else
			{
				mTemp->m_IsMetal = true;

				mTemp->SetMaterial(mSuitColor1->m_icon);
			}

			mTemp->OnButtonClick(1);
		}
		//	显示花型购物车.
		else if (!m_usedFabricStruct.m_patternTid.IsEmpty())
		{
			SuitPatternT * mSuitPattern = TSuitPatternManager::GetInstance()->GetPatternData(m_usedFabricStruct.m_patternTid);

			mTemp->m_SelectType = 1;

			mTemp->m_patternId = m_usedFabricStruct.m_patternTid;

			mTemp->SetTexture2D(mSuitPattern->m_iconUrl);

			mTemp->OnButtonClick(1);
		}
	}
	m_FirstBatch = false;
}

void UDetailPage::HiddenTip()
{
	LogicDesignScene * mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();

	UDesignUMG * mDesignUMG = nullptr;

	if (mLogicDesignScene != nullptr)
	{
		mDesignUMG = (UDesignUMG *)(mLogicDesignScene->GetCurrentUIController()->GetRootUI());
	}

	//隐藏tip.
	mDesignUMG->m_Canv_Tip->SetVisibility(ESlateVisibility::Hidden);

	GetWorld()->GetTimerManager().ClearTimer(m_TimerHandle);
}

void UDetailPage::OnResponseChangeFabric(msg_ptr _msg)
{
	if (m_CurrentChangeFabricCell!=nullptr)
	{
		UUIManager::GetInstance()->CloseMiddleTip();
		m_CurrentChangeFabricCell->ChangeFabric();
	}
}


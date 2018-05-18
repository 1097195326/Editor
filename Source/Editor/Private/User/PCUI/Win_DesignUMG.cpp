
#include "Win_DesignUMG.h"
#include "UIManager.h"
#include "GameDirector.h"
#include "GModuleManager.h"
#include "GLanguageTool.h"
#include "LogicDesignScene.h"
#include "DesignUIController.h"
#include "UIManager.h"
#include "MsgCenter.h"


UWin_DesignUMG::UWin_DesignUMG(const FObjectInitializer& ObjectInitializer)
{
	m_Btn_Walker=nullptr;
	m_Btn_HideRight = nullptr;
	m_Btn_ShowRight = nullptr;
	m_Btn_Shop = nullptr;
	m_Btn_HideSlide = nullptr;
	m_Btn_ShowSlide = nullptr;
	m_Btn_Cancel = nullptr;
	m_Btn_Save = nullptr;
	m_Btn_PieceClose = nullptr;
	m_UMG_SuitPartMenu = nullptr;
	m_UMG_SuitComponentMenu = nullptr;
	m_ComponentDetail = nullptr;
	m_UMG_SuitSliceMenu = nullptr;
	m_DetailPage = nullptr;
	m_Canv_Left = nullptr;
	m_Canv_Bottom = nullptr;
	m_Canv_Tip = nullptr;

	m_Canv_Menus = nullptr;
	m_Canv_Right = nullptr;
	m_Btn_SliceMenu = nullptr;
	m_Canv_Detail = nullptr;

}

bool UWin_DesignUMG::OnStart()
{
	bool mRet = false;
	
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal,3020, this, &UWin_DesignUMG::OnCompMeshShow);

	mRet = true;
	return mRet;
}

void UWin_DesignUMG::OnInit()
{
	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("Btn_Walker")))
	{
		m_Btn_Walker = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_Walker, this, &UWin_DesignUMG::OnButtonWalker);
	}
	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("Btn_HideRight")))
	{
		m_Btn_HideRight = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_HideRight, this, &UWin_DesignUMG::OnButtonHideRight);
	}
	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("Btn_ShowRight")))
	{
		m_Btn_ShowRight = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_ShowRight, this, &UWin_DesignUMG::OnButtonShowRight);
		
	}
	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("Btn_Shop")))
	{
		m_Btn_Shop = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_Shop, this, &UWin_DesignUMG::OnButtonShop);
	}

	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("Btn_SliceMenu")))
	{
		m_Btn_SliceMenu = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_SliceMenu, this, &UWin_DesignUMG::OnButtonSliceMenu);
	}

	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("Btn_HideSlide")))
	{
		m_Btn_HideSlide = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_HideSlide, this, &UWin_DesignUMG::OnButtonHideSlide);
	}
	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("Btn_ShowSlide")))
	{
		m_Btn_ShowSlide = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_ShowSlide, this, &UWin_DesignUMG::OnButtonShowSilde);
	}
	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("Btn_Save")))
	{
		m_Btn_Save = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_Save, this, &UWin_DesignUMG::OnButtonSave);
	}
	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("Btn_PieceClose")))
	{
		m_Btn_PieceClose = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_PieceClose, this, &UWin_DesignUMG::OnButtonPieceClose);
	}
	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("Btn_Cancel")))
	{
		m_Btn_Cancel = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_Cancel, this, &UWin_DesignUMG::OnButtonCancel);
	}
	if (UWin_UMGSuitPartMenu *mTemp = Cast<UWin_UMGSuitPartMenu>(GetWidgetFromName("UMG_SuitPartMenu")))
	{
		m_UMG_SuitPartMenu = mTemp;
	}
	if (UWin_UMGSuitComponentMenu *mTemp = Cast<UWin_UMGSuitComponentMenu>(GetWidgetFromName("UMG_SuitComponentMenu")))
	{
		m_UMG_SuitComponentMenu = mTemp;
	}
	if (UWin_UMGSuitComponentDetail *mTemp = Cast<UWin_UMGSuitComponentDetail>(GetWidgetFromName("UMG_SuitComponentDetail")))
	{
		m_ComponentDetail = mTemp;
	}
	if (UWin_UMGSuitSliceMenu *mTemp = Cast<UWin_UMGSuitSliceMenu>(GetWidgetFromName("UMG_SuitSliceMenu")))
	{
		m_UMG_SuitSliceMenu = mTemp;
	}

	if (UCanvasPanel *mTemp = Cast<UCanvasPanel>(GetWidgetFromName("Left")))
	{
		m_Canv_Left = mTemp;
	}
	if (UCanvasPanel *mTemp = Cast<UCanvasPanel>(GetWidgetFromName("Right")))
	{
		m_Canv_Right = mTemp;
	}
	
	if (UCanvasPanel *mTemp = Cast<UCanvasPanel>(GetWidgetFromName("Bottom")))
	{
		m_Canv_Bottom = mTemp;
	}
	if (UCanvasPanel *mTemp = Cast<UCanvasPanel>(GetWidgetFromName("Menus")))
	{
		m_Canv_Menus = mTemp;
	}

	//	DetailPage.
	//	花型，颜色，面料.
	if (UCanvasPanel * mTemp = Cast<UCanvasPanel>(GetWidgetFromName("DetailPage")))
	{
		m_Canv_Detail = mTemp;
	}
	if (UWin_DetailPage * mTemp = Cast<UWin_DetailPage>(GetWidgetFromName("UI_Detail")))
	{
		m_DetailPage = mTemp;		
	}
	if (UButton * mTemp = Cast<UButton>(GetWidgetFromName("Button_DetailClose")))
	{
		m_Btn_DetailClose = mTemp;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_DetailClose, this, &UWin_DesignUMG::OnButtonDetailClose);
	}
	if (UCanvasPanel * mTemp = Cast<UCanvasPanel>(GetWidgetFromName("ShoppingCar")))
	{
		m_Canv_DetailShop = mTemp;
	}
	if (UButton * mTemp = Cast<UButton>(GetWidgetFromName("Button_DetailShop")))
	{
		m_Btn_DetailShop = mTemp;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_DetailShop, this, &UWin_DesignUMG::OnButtonDetailShop);
	}
	if (UImage * mTemp = Cast<UImage>(GetWidgetFromName("Texture")))
	{
		m_ImgDetailTexture = mTemp;
	}
	if (UCanvasPanel * mTemp = Cast<UCanvasPanel>(GetWidgetFromName("Canv_Tip")))
	{
		m_Canv_Tip = mTemp;
	}
	if (UTextBlock * mTemp = Cast<UTextBlock>(GetWidgetFromName("Text_Tip")))
	{
		m_TextTip = mTemp;
	}

	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, 2003, this, &UWin_DesignUMG::OnShopButtonSaveCallBack);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, 20081, this, &UWin_DesignUMG::OnShopButtonRequestCallBack);

}

void UWin_DesignUMG::OnRelease()
{

	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, 2003, this, &UWin_DesignUMG::OnShopButtonSaveCallBack);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, 20081, this, &UWin_DesignUMG::OnShopButtonRequestCallBack);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, 3020, this, &UWin_DesignUMG::OnCompMeshShow);

	m_Btn_Walker = nullptr;
	m_Btn_HideRight = nullptr;
	m_Btn_ShowRight = nullptr;
	m_Btn_Shop = nullptr;
	m_Btn_HideSlide = nullptr;
	m_Btn_ShowSlide = nullptr;
	m_Btn_Cancel = nullptr;
	m_Btn_Save = nullptr;
	m_Btn_PieceClose = nullptr;
	m_UMG_SuitPartMenu = nullptr;
	m_UMG_SuitComponentMenu = nullptr;
	m_ComponentDetail = nullptr;
	m_UMG_SuitSliceMenu = nullptr;
	m_DetailPage = nullptr;
	m_Canv_Left = nullptr;
	m_Canv_Bottom = nullptr;
	m_Canv_Tip = nullptr;
	m_Canv_Menus = nullptr;
	m_Canv_Right = nullptr;
	m_Canv_Detail = nullptr;
	m_DetailPage = nullptr;
	UUIManager::GetInstance()->RemoveButtonClick(m_Btn_DetailShop);
	UUIManager::GetInstance()->RemoveButtonClick(m_Btn_DetailClose);


}

void UWin_DesignUMG::OnLoad()
{
}

void UWin_DesignUMG::OnSetParams(int32 _pid, FString & _content, ...)
{
}

void UWin_DesignUMG::OnTick(float InDeltaTimeSecond)
{

}

void UWin_DesignUMG::OnButtonWalker(int32 param)
{
	LogicDesignScene * mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();

	mLogicDesignScene->JoinWalkingLevel();
}
void UWin_DesignUMG::OnButtonSliceMenu(int32 param)
{
	LogicDesignScene *mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();

	UWin_DesignUMG *mDesignUMG = (UWin_DesignUMG *)(mLogicDesignScene->GetCurrentUIController()->GetRootUI());

	mDesignUMG->m_UMG_SuitComponentMenu->SetUniqueHiddenSelectedIcon(mDesignUMG->m_ComponentTid);

	ShowSuitSliceMenu();
}


void UWin_DesignUMG::OnButtonDetailClose(int32 param)
{
	m_Canv_Detail->SetVisibility(ESlateVisibility::Hidden);

	m_Canv_Right->SetVisibility(ESlateVisibility::Visible);
 
 	LogicDesignScene *mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();
 
	UWin_DesignUMG *mDesignUMG = (UWin_DesignUMG *)(mLogicDesignScene->GetCurrentUIController()->GetRootUI());
 
 	mDesignUMG->m_UMG_SuitComponentMenu->SetUniqueHiddenSelectedIcon(mDesignUMG->m_ComponentTid);

	mDesignUMG->m_UMG_SuitComponentMenu->SetUniqueHiddenSelectedBatchIcon(mDesignUMG->m_UsedFabric_Struct);

	mDesignUMG->m_UMG_SuitPartMenu->SetUniqueHiddenSelectedBatchIcon(mDesignUMG->m_UsedFabric_Struct);

	mDesignUMG->m_UMG_SuitSliceMenu->SetUniqueHiddenSelected(mDesignUMG->m_ComponentTid);

}

void UWin_DesignUMG::OnButtonDetailShop(int32 param)
{
	UE_LOG(GEditorLog, Log, TEXT("OnButtonDetailShop click"));

	if (!m_BuyUrl.IsEmpty())
	{
		if (m_CanBuy == true)
		{
			UE_LOG(GEditorLog, Log, TEXT("Open Shop webview"));

			UHaoshaSDKBPLibrary::OpenWebview(EditorFunctionalTool::GetTicketUrl(m_BuyUrl));
		}
	}
}

void UWin_DesignUMG::OnConfirmSaveEvent()
{
	LogicDesignScene * mLogicDesignScene = (LogicDesignScene *)UGameDirector::GetInstance()->GetCurrentLogicScene();
	mLogicDesignScene->m_saveType = EnumSaveStyleType::SAVE_STYLE_FOR_CANCEL_BTN;
	DoSave();
}

void UWin_DesignUMG::OnCancelSaveEvent()
{
	LogicDesignScene * mLogicDesignScene = (LogicDesignScene *)UGameDirector::GetInstance()->GetCurrentLogicScene();
	mLogicDesignScene->FinishDesign();
}

void UWin_DesignUMG::InitSuitPartMenu()
{
	m_UMG_SuitPartMenu->InitData();
}

void UWin_DesignUMG::InitSuitPartBatchMenu()
{
	m_UMG_SuitPartMenu->InitBatchData();
}

void UWin_DesignUMG::InitSuitCompMenu(int32 _SuitPartTid)
{
	m_UMG_SuitComponentMenu->InitData(_SuitPartTid);
}

void UWin_DesignUMG::InitSuitCompBatchMenu()
{
	m_UMG_SuitComponentMenu->InitBatchData();
}

void UWin_DesignUMG::InitSuitCompDetail(TArray<SuitComponentT*> &_CompTidArray)
{
	m_ComponentDetail->InitData(_CompTidArray);
}

void UWin_DesignUMG::InitSuitSliceMenu(int32 _SuitComponentTid)
{
	//int32 mSuitCompTid = m_ComponentDetail->GetCurrentSelectedCompTid();
	m_UMG_SuitSliceMenu->InitData(_SuitComponentTid);
}

void UWin_DesignUMG::InitDetailPage(EnumDataType _Type,int32 _Tid, UsedFabricStruct & _usedFabricStruct)
{
	m_DetailPage->InitPatternData(_Type, _Tid, _usedFabricStruct);
}

void UWin_DesignUMG::OnButtonHideRight(int32 param)
{
	m_Btn_HideRight->SetVisibility(ESlateVisibility::Hidden);
	m_Btn_Shop->SetVisibility(ESlateVisibility::Hidden);
	m_Btn_Walker->SetVisibility(ESlateVisibility::Hidden);
	m_Btn_ShowRight->SetVisibility(ESlateVisibility::Visible);
}

void UWin_DesignUMG::OnButtonShowRight(int32 param)
{
	m_Btn_HideRight->SetVisibility(ESlateVisibility::Visible);
	m_Btn_Shop->SetVisibility(ESlateVisibility::Visible);
	m_Btn_Walker->SetVisibility(ESlateVisibility::Visible);
	m_Btn_ShowRight->SetVisibility(ESlateVisibility::Hidden);
}

void UWin_DesignUMG::OnButtonShop(int32 param)
{
	LogicDesignScene * mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();
	mLogicDesignScene->m_saveType = EnumSaveStyleType::SAVE_STYLE_FOR_SHOP;

	OnButtonSave(0);

}
void UWin_DesignUMG::OnShopButtonRequestCallBack(msg_ptr _msg)
{
	MsgLocal * mMsg = _msg->ConvertMsg<MsgLocal>();
	OrderStruct * order = (OrderStruct *)mMsg->Content();
	UHaoshaSDKBPLibrary::OpenWebview(EditorFunctionalTool::GetTicketUrl(order->m_iconUrl));
}
void UWin_DesignUMG::OnShopButtonSaveCallBack(msg_ptr _msg)
{
	DesignModule * mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");
	LogicDesignScene * mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();

	int32 _StyleID = mLogicDesignScene->m_savedStyleRid;

	mDesignModule->RequestShopOrder(_StyleID);
}
void UWin_DesignUMG::OnButtonHideSlide(int32 param)
{
	m_Btn_HideSlide->SetVisibility(ESlateVisibility::Hidden);
	m_Btn_ShowSlide->SetVisibility(ESlateVisibility::Visible);
	m_Canv_Menus->SetVisibility(ESlateVisibility::Hidden);
}

void UWin_DesignUMG::OnButtonShowSilde(int32 param)
{
	//m_Btn_HideSlide->SetVisibility(ESlateVisibility::Visible);
	m_Btn_ShowSlide->SetVisibility(ESlateVisibility::Hidden);
	m_Canv_Detail->SetVisibility(ESlateVisibility::Visible);
	this->m_DetailPage->m_CanvasDetail->SetVisibility(ESlateVisibility::Visible);
	this->m_DetailPage->m_Cav_Buy->SetVisibility(ESlateVisibility::Hidden);
}

void UWin_DesignUMG::OnButtonSave(int32 param)
{
	DoSave();
}
 
void UWin_DesignUMG::OnButtonPieceClose(int32 param)
{
	m_UMG_SuitComponentMenu->OnBackstageUI();
    ShowSuitPartMenu();

	//摄像机SuitPartMenu.
	OnReturnToSuitPartMenu();
}

void UWin_DesignUMG::OnButtonCancel(int32 param)
{
	LogicDesignScene * mLogicDesignScene = (LogicDesignScene *)UGameDirector::GetInstance()->GetCurrentLogicScene();
	
	if (mLogicDesignScene->IsUpdated())
	{
		FString mContent;

		DesignModule * mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

		if (mDesignModule->GetDesignData()->m_DesignType == DESIGN_BY_SYS_STYLE)
		{
			mContent = FString(TEXT("key142"));
		}
		else
		{
			mContent = FString(TEXT("key146"));
		}
		FString mMsg=GLanguageTool::GetInstance()->GetLanguage(mContent);
		//修改过款.
		UUIManager::GetInstance()->OpenTrueOrFalseAlert(mMsg,this,&UWin_DesignUMG::OnConfirmSaveEvent,&UWin_DesignUMG::OnCancelSaveEvent);
	}
	else
	{
		//未修改过款.
		mLogicDesignScene->FinishDesign();
	}
}

void UWin_DesignUMG::ShowSuiComponentMenu()
{
	m_UMG_SuitComponentMenu->OnBackstageUI();
	m_UMG_SuitComponentMenu->OnActivationUI();
	//m_Canv_Right->SetVisibility(ESlateVisibility::Hidden);
	//ShowComponentDetail();
}

void UWin_DesignUMG::ShowSuitPartMenu()
{
	m_UMG_SuitPartMenu->OnActivationUI();

	HidenComponentDetail();
}

void UWin_DesignUMG::ShowSuitSliceMenu()
{
	HidenSuiComponentMenu();
	HidenComponentDetail();
	m_UMG_SuitSliceMenu->OnActivationUI();
}

void UWin_DesignUMG::ShowComponentDetail()
{
	m_Canv_Bottom->SetVisibility(ESlateVisibility::Visible);

	this->m_Btn_ShowSlide->SetVisibility(ESlateVisibility::Hidden);
}

void UWin_DesignUMG::ShowSliceMenuButton()
{
	m_Btn_SliceMenu->SetVisibility(ESlateVisibility::Visible);
}

void UWin_DesignUMG::ShowDetailPage()
{
	if(!m_Canv_Detail->IsVisible())
		m_Canv_Detail->SetVisibility(ESlateVisibility::Visible);

	this->m_Btn_ShowSlide->SetVisibility(ESlateVisibility::Hidden);

	//m_Canv_Right->SetVisibility(ESlateVisibility::Hidden);
}

void UWin_DesignUMG::HidenSuiComponentMenu()
{
	m_UMG_SuitComponentMenu->OnBackstageUI();
}

void UWin_DesignUMG::HidenSuitPartMenu()
{
	m_UMG_SuitPartMenu->OnBackstageUI();
}

void UWin_DesignUMG::HidenSuitSliceMenu()
{
	m_UMG_SuitSliceMenu->OnActivationUI();
}

void UWin_DesignUMG::HidenComponentDetail()
{
	m_Canv_Right->SetVisibility(ESlateVisibility::Visible);
	m_Canv_Bottom->SetVisibility(ESlateVisibility::Hidden);
}

void UWin_DesignUMG::HidenSliceMenuButton()
{
	m_Btn_SliceMenu->SetVisibility(ESlateVisibility::Hidden);
}

void UWin_DesignUMG::HidenDetailPage()
{
	m_Canv_Detail->SetVisibility(ESlateVisibility::Hidden);
}

void UWin_DesignUMG::SetBuyUrl(FString _buyUrl)
{
	m_BuyUrl = _buyUrl;
}

void UWin_DesignUMG::SetDetailShopView(bool _CanShop)
{
	/*if(_CanShop)
		m_Canv_DetailShop->SetVisibility(ESlateVisibility::Visible);
	else
		m_Canv_DetailShop->SetVisibility(ESlateVisibility::Hidden);*/
}

void UWin_DesignUMG::DoSave()
{
	LogicDesignScene * mLogicDesignScene = (LogicDesignScene *)UGameDirector::GetInstance()->GetCurrentLogicScene();

	if (mLogicDesignScene->m_isSaveing)
	{
		return;
	}
	mLogicDesignScene->m_isSaveing = true;

 	FString mTip = GLanguageTool::GetInstance()->GetLanguage(TEXT("key139"));

	UUIManager::GetInstance()->OpenMiddleTip(mTip, 2);
	//UUIManager::GetInstance()->OpenFullTip(mTip);
	OnSaveButtonClick();
}

int32 UWin_DesignUMG::GetStyleSliceTid()
{
	return 0;
}

void UWin_DesignUMG::OnChangeSuitComponent_Implementation(FMeshComponentStruct _MeshCompStruct)
{

}

void UWin_DesignUMG::OnChangeSuitSlice_Implementation(FMeshComponentStruct _MeshCompStruct)
{

}

void UWin_DesignUMG::OnReturnToSuitPartMenu_Implementation()
{

}

void UWin_DesignUMG::OnReturnToCompMenu_Implementation()
{

}

void UWin_DesignUMG::InitCamera_Implementation(FMeshComponentStruct _MeshCompStruct)
{

}

void UWin_DesignUMG::OnSaveButtonClick_Implementation()
{

}



FMeshComponentStruct UWin_DesignUMG::GetStyleMeshComponentStruct()
{
	DesignModule *mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");
	int32 mSuitTid = mDesignModule->GetDesignData()->m_designTID;

	LogicDesignScene *mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();
	return mLogicDesignScene->GetComponentStructByTid(mSuitTid, EnumComponentType::STYLE_COMPONENT);
}

void UWin_DesignUMG::UpdateSliceDetailPage()
{
	
	//刷新DetailPage.
	int32 mSliceTid = m_UMG_SuitSliceMenu->GetCurSelectedSliceTid();
	UsedFabricStruct mStruct;
	LogicDesignScene *mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();
	mStruct.m_patternTid = mLogicDesignScene->GetDefaultPatternInSlice(mSliceTid, EnumComponentType::SLICE_COMPONENT);
	mStruct.m_fabricTid = mLogicDesignScene->GetDefaultFabricInSlice(mSliceTid,EnumComponentType::SLICE_COMPONENT);
	mStruct.m_colorTid = mLogicDesignScene->GetDefaultColorInSlice(mSliceTid, EnumComponentType::SLICE_COMPONENT);
	this->m_DetailPage->m_usedFabricStruct = mStruct;
	this->m_DetailPage->m_Type = EnumDataType::TYPE_STYLE_SLICE;
	this->m_DetailPage->m_Style_Tid = mSliceTid;

	UpdateComponentDetailPage(mStruct,false);
}

void UWin_DesignUMG::UpdateComponentDetailPage(UsedFabricStruct _UsedFabricStruct, bool isInitDetailPage)
{
	UpdatePartDetailPage(_UsedFabricStruct,false);
	//刷新CompMenu.
	m_UMG_SuitComponentMenu->InitBatchData();
	m_UMG_SuitComponentMenu->SetUniqueSelectedBatchIcon(_UsedFabricStruct);

	if (isInitDetailPage)
	{
		//刷新DetailPage.
		LogicDesignScene * mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();
		
		this->m_DetailPage->m_usedFabricStruct = _UsedFabricStruct;
		this->m_DetailPage->m_Type = EnumDataType::TYPE_STYLE_COMPONENT;
		this->m_DetailPage->m_Style_Tid = mLogicDesignScene->GetDefaultComponentTid(m_UMG_SuitComponentMenu->GetSuitPartTid(), EnumComponentType::PART_COMPONENT);
	}
	
}

void UWin_DesignUMG::UpdatePartDetailPage(UsedFabricStruct _UsedFabricStruct, bool isInitDetailPage)
{
	//刷新PartMenu.
	m_UMG_SuitPartMenu->InitBatchData();
	m_UMG_SuitPartMenu->SetUniqueSelectedBatchIcon(_UsedFabricStruct);

	if (isInitDetailPage)
	{
		//刷新DetailPage.
		this->m_DetailPage->m_usedFabricStruct = _UsedFabricStruct;
		this->m_DetailPage->m_Type = EnumDataType::TYPE_STYLE_ALL;
		this->m_DetailPage->m_Style_Tid = -1;
	}
}

//	设置Detail购物车.
void UWin_DesignUMG::SetDetailTexture(UTexture2D * _texture2D)
{
	//m_ImgDetailTexture->GetDynamicMaterial()->SetScalarParameterValue("A", 1);

	//m_ImgDetailTexture->GetDynamicMaterial()->SetTextureParameterValue("Texture", _texture2D);
}

void UWin_DesignUMG::SetDetailColor(FLinearColor _RGB)
{
	//m_ImgDetailTexture->GetDynamicMaterial()->SetScalarParameterValue("A", -1);

	//m_ImgDetailTexture->GetDynamicMaterial()->SetVectorParameterValue("Color", _RGB);
}

void UWin_DesignUMG::SetDetailMaterial(FLinearColor _RGB, UTexture2D * _texture2D)
{
// 	if (!_texture2D->IsValidLowLevel())
// 	{
// 		m_ImgDetailTexture->GetDynamicMaterial()->SetScalarParameterValue("A", -1);
// 
// 		m_ImgDetailTexture->GetDynamicMaterial()->SetVectorParameterValue("Color", _RGB);
// 	}
// 	else
// 	{
// 		m_ImgDetailTexture->GetDynamicMaterial()->SetScalarParameterValue("A", 1);
// 
// 		m_ImgDetailTexture->GetDynamicMaterial()->SetTextureParameterValue("Texture", _texture2D);
// 	}
}

void UWin_DesignUMG::OnShopError()
{

}

void UWin_DesignUMG::OnCompMeshShow(msg_ptr _msg)
{
	if (m_IsDetailCompCellClicked)
	{
		m_IsDetailCompCellClicked=false;
	
			//延迟移动摄像机.
			GetCamCompMeshData();
		
	}
}

void UWin_DesignUMG::OnSetPieceShow()
{
	if(nullptr != m_Btn_SliceMenu)
		m_Btn_SliceMenu->SetVisibility(ESlateVisibility::Hidden);
}

void UWin_DesignUMG::GetCamCompMeshData()
{
	LogicDesignScene *mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();
	if (mLogicDesignScene!=nullptr)
	{
		FMeshComponentStruct mCompStruct=mLogicDesignScene->GetComponentStructByTid(m_ComponentDetail->m_CurSelectedTid, EnumComponentType::PIECES_COMPONENT);
		if (mCompStruct.m_meshComponentArray.Num()>0)
		{
			//移动摄像机.
			OnChangeSuitComponent(mCompStruct);
			//重置部件选择摄像机激活状态.
	
		}
		else
		{
			//!!!正式包记得关掉打印.
			GEngine->AddOnScreenDebugMessage(0, 3000, FColor::Green, FString("FMeshComponentStruct is null , Camera can't move!"));
		}
		
	}
}


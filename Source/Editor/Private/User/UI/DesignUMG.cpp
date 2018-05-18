
#include "DesignUMG.h"
#include "UIManager.h"
#include "GameDirector.h"
#include "GModuleManager.h"
#include "GLanguageTool.h"
#include "LogicDesignScene.h"
#include "DesignUIController.h"
#include "UIManager.h"
#include "MsgCenter.h"


UDesignUMG::UDesignUMG(const FObjectInitializer& ObjectInitializer)
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

bool UDesignUMG::OnStart()
{
	//Super::OnStart();
	bool mRet = false;
	
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal,3020, this, &UDesignUMG::OnCompMeshShow);

	mRet = true;
	return mRet;
}

void UDesignUMG::OnInit()
{
	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("Btn_Walker")))
	{
		m_Btn_Walker = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_Walker, this, &UDesignUMG::OnButtonWalker);
	}
	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("Btn_HideRight")))
	{
		m_Btn_HideRight = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_HideRight, this, &UDesignUMG::OnButtonHideRight);
	}
	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("Btn_ShowRight")))
	{
		m_Btn_ShowRight = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_ShowRight, this, &UDesignUMG::OnButtonShowRight);
		
	}
	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("Btn_Shop")))
	{
		m_Btn_Shop = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_Shop, this, &UDesignUMG::OnButtonShop);
	}

	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("Btn_SliceMenu")))
	{
		m_Btn_SliceMenu = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_SliceMenu, this, &UDesignUMG::OnButtonSliceMenu);
	}

	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("Btn_HideSlide")))
	{
		m_Btn_HideSlide = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_HideSlide, this, &UDesignUMG::OnButtonHideSlide);
	}
	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("Btn_ShowSlide")))
	{
		m_Btn_ShowSlide = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_ShowSlide, this, &UDesignUMG::OnButtonShowSilde);
	}
	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("Btn_Save")))
	{
		m_Btn_Save = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_Save, this, &UDesignUMG::OnButtonSave);
	}
	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("Btn_PieceClose")))
	{
		m_Btn_PieceClose = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_PieceClose, this, &UDesignUMG::OnButtonPieceClose);
	}
	if (UButton *mButton = Cast<UButton>(GetWidgetFromName("Btn_Cancel")))
	{
		m_Btn_Cancel = mButton;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_Cancel, this, &UDesignUMG::OnButtonCancel);
	}
	if (UUMGSuitPartMenu *mTemp = Cast<UUMGSuitPartMenu>(GetWidgetFromName("UMG_SuitPartMenu")))
	{
		m_UMG_SuitPartMenu = mTemp;
	}
	if (UUMGSuitComponentMenu *mTemp = Cast<UUMGSuitComponentMenu>(GetWidgetFromName("UMG_SuitComponentMenu")))
	{
		m_UMG_SuitComponentMenu = mTemp;
	}
	if (UUMGSuitComponentDetail *mTemp = Cast<UUMGSuitComponentDetail>(GetWidgetFromName("UMG_SuitComponentDetail")))
	{
		m_ComponentDetail = mTemp;
	}
	if (UUMGSuitSliceMenu *mTemp = Cast<UUMGSuitSliceMenu>(GetWidgetFromName("UMG_SuitSliceMenu")))
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
	if (UDetailPage * mTemp = Cast<UDetailPage>(GetWidgetFromName("UI_Detail")))
	{
		m_DetailPage = mTemp;		
	}
	if (UButton * mTemp = Cast<UButton>(GetWidgetFromName("Button_DetailClose")))
	{
		m_Btn_DetailClose = mTemp;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_DetailClose, this, &UDesignUMG::OnButtonDetailClose);
	}
	if (UCanvasPanel * mTemp = Cast<UCanvasPanel>(GetWidgetFromName("ShoppingCar")))
	{
		m_Canv_DetailShop = mTemp;
	}
	if (UButton * mTemp = Cast<UButton>(GetWidgetFromName("Button_DetailShop")))
	{
		m_Btn_DetailShop = mTemp;
		UUIManager::GetInstance()->RegisterButtonClick(31, m_Btn_DetailShop, this, &UDesignUMG::OnButtonDetailShop);
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

	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, 2003, this, &UDesignUMG::OnShopButtonSaveCallBack);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, 20081, this, &UDesignUMG::OnShopButtonRequestCallBack);

}

void UDesignUMG::OnRelease()
{

	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, 2003, this, &UDesignUMG::OnShopButtonSaveCallBack);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, 20081, this, &UDesignUMG::OnShopButtonRequestCallBack);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, 3020, this, &UDesignUMG::OnCompMeshShow);

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
	m_Btn_SliceMenu = nullptr;
	m_Canv_Detail = nullptr;
	m_DetailPage = nullptr;
	UUIManager::GetInstance()->RemoveButtonClick(m_Btn_DetailShop);
	UUIManager::GetInstance()->RemoveButtonClick(m_Btn_DetailClose);


}

void UDesignUMG::OnLoad()
{

}

void UDesignUMG::OnSetParams(int32 _pid, FString & _content, ...)
{
}

void UDesignUMG::OnTick(float InDeltaTimeSecond)
{

}

void UDesignUMG::OnButtonWalker(int32 param)
{
	LogicDesignScene * mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();

	mLogicDesignScene->JoinWalkingLevel();
}
void UDesignUMG::OnButtonSliceMenu(int32 param)
{
	LogicDesignScene *mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();

	UDesignUMG *mDesignUMG = (UDesignUMG *)(mLogicDesignScene->GetCurrentUIController()->GetRootUI());

	mDesignUMG->m_UMG_SuitComponentMenu->SetUniqueHiddenSelectedIcon(mDesignUMG->m_ComponentTid);

	ShowSuitSliceMenu();
}


void UDesignUMG::OnButtonDetailClose(int32 param)
{
	m_Canv_Detail->SetVisibility(ESlateVisibility::Hidden);

	m_Canv_Right->SetVisibility(ESlateVisibility::Visible);
 
 	LogicDesignScene *mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();
 
 	UDesignUMG *mDesignUMG = (UDesignUMG *)(mLogicDesignScene->GetCurrentUIController()->GetRootUI());
 
 	mDesignUMG->m_UMG_SuitComponentMenu->SetUniqueHiddenSelectedIcon(mDesignUMG->m_ComponentTid);

	mDesignUMG->m_UMG_SuitComponentMenu->SetUniqueHiddenSelectedBatchIcon(mDesignUMG->m_UsedFabric_Struct);

	mDesignUMG->m_UMG_SuitPartMenu->SetUniqueHiddenSelectedBatchIcon(mDesignUMG->m_UsedFabric_Struct);

	mDesignUMG->m_UMG_SuitSliceMenu->SetUniqueHiddenSelected(mDesignUMG->m_ComponentTid);

}

void UDesignUMG::OnButtonDetailShop(int32 param)
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

void UDesignUMG::OnConfirmSaveEvent()
{
	LogicDesignScene * mLogicDesignScene = (LogicDesignScene *)UGameDirector::GetInstance()->GetCurrentLogicScene();
	mLogicDesignScene->m_saveType = EnumSaveStyleType::SAVE_STYLE_FOR_CANCEL_BTN;
	DoSave();
}

void UDesignUMG::OnCancelSaveEvent()
{
	LogicDesignScene * mLogicDesignScene = (LogicDesignScene *)UGameDirector::GetInstance()->GetCurrentLogicScene();
	mLogicDesignScene->FinishDesign();
}

void UDesignUMG::InitSuitPartMenu()
{
	m_UMG_SuitPartMenu->InitData();
}

void UDesignUMG::InitSuitPartBatchMenu()
{
	m_UMG_SuitPartMenu->InitBatchData();
}

void UDesignUMG::InitSuitCompMenu(int32 _SuitPartTid)
{
	m_UMG_SuitComponentMenu->InitData(_SuitPartTid);
}

void UDesignUMG::InitSuitCompBatchMenu()
{
	m_UMG_SuitComponentMenu->InitBatchData();
}

void UDesignUMG::InitSuitCompDetail(TArray<SuitComponentT*> &_CompTidArray)
{
	m_ComponentDetail->InitData(_CompTidArray);
}

void UDesignUMG::InitSuitSliceMenu(int32 _SuitComponentTid)
{
	//int32 mSuitCompTid = m_ComponentDetail->GetCurrentSelectedCompTid();
	m_UMG_SuitSliceMenu->InitData(_SuitComponentTid);
}

void UDesignUMG::InitDetailPage(EnumDataType _Type,int32 _Tid, UsedFabricStruct & _usedFabricStruct)
{
	m_DetailPage->InitPatternData(_Type, _Tid, _usedFabricStruct);
}

void UDesignUMG::OnButtonHideRight(int32 param)
{
	m_Btn_HideRight->SetVisibility(ESlateVisibility::Hidden);
	m_Btn_Shop->SetVisibility(ESlateVisibility::Hidden);
	m_Btn_Walker->SetVisibility(ESlateVisibility::Hidden);
	m_Btn_ShowRight->SetVisibility(ESlateVisibility::Visible);
}

void UDesignUMG::OnButtonShowRight(int32 param)
{
	m_Btn_HideRight->SetVisibility(ESlateVisibility::Visible);
	m_Btn_Shop->SetVisibility(ESlateVisibility::Visible);
	m_Btn_Walker->SetVisibility(ESlateVisibility::Visible);
	m_Btn_ShowRight->SetVisibility(ESlateVisibility::Hidden);
}

void UDesignUMG::OnButtonShop(int32 param)
{
	LogicDesignScene * mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();
	
	mLogicDesignScene->m_saveType = EnumSaveStyleType::SAVE_STYLE_FOR_SHOP;

	OnButtonSave(0);
}
void UDesignUMG::OnShopButtonRequestCallBack(msg_ptr _msg)
{
	MsgLocal * mMsg = _msg->ConvertMsg<MsgLocal>();

	OrderStruct * order = (OrderStruct *)mMsg->Content();

	if (order->m_iconUrl.IsEmpty())
	{
		FString mText = GLanguageTool::GetInstance()->GetLanguage(TEXT("key151"));

		UUIManager::GetInstance()->OpenMiddleTip2(mText, 2);
	}
	else
		UHaoshaSDKBPLibrary::OpenWebview(EditorFunctionalTool::GetTicketUrl(order->m_iconUrl));
}
void UDesignUMG::OnShopButtonSaveCallBack(msg_ptr _msg)
{
	DesignModule * mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");
	LogicDesignScene * mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();

	int32 _StyleID = mLogicDesignScene->m_savedStyleRid;

	mDesignModule->RequestShopOrder(_StyleID);
}
void UDesignUMG::OnButtonHideSlide(int32 param)
{
	m_Btn_HideSlide->SetVisibility(ESlateVisibility::Hidden);
	m_Btn_ShowSlide->SetVisibility(ESlateVisibility::Visible);
	m_Canv_Menus->SetVisibility(ESlateVisibility::Hidden);
}

void UDesignUMG::OnButtonShowSilde(int32 param)
{
	m_Btn_HideSlide->SetVisibility(ESlateVisibility::Visible);
	m_Btn_ShowSlide->SetVisibility(ESlateVisibility::Hidden);
	m_Canv_Menus->SetVisibility(ESlateVisibility::Visible);
}

void UDesignUMG::OnButtonSave(int32 param)
{
	DoSave();
}
 
void UDesignUMG::OnButtonPieceClose(int32 param)
{
	m_UMG_SuitComponentMenu->OnBackstageUI();
    ShowSuitPartMenu();

	//摄像机SuitPartMenu.
	OnReturnToSuitPartMenu();
}

void UDesignUMG::OnButtonCancel(int32 param)
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
		UUIManager::GetInstance()->OpenTrueOrFalseAlert(mMsg,this,&UDesignUMG::OnConfirmSaveEvent,&UDesignUMG::OnCancelSaveEvent);
	}
	else
	{
		//未修改过款.
		mLogicDesignScene->FinishDesign();
	}
}

void UDesignUMG::ShowSuiComponentMenu()
{
	m_UMG_SuitComponentMenu->OnBackstageUI();
	m_UMG_SuitComponentMenu->OnActivationUI();
	m_Canv_Right->SetVisibility(ESlateVisibility::Hidden);
	ShowComponentDetail();
}

void UDesignUMG::ShowSuitPartMenu()
{
	m_UMG_SuitPartMenu->OnActivationUI();
	HidenComponentDetail();
}

void UDesignUMG::ShowSuitSliceMenu()
{
	HidenSuiComponentMenu();
	HidenComponentDetail();
	m_UMG_SuitSliceMenu->OnActivationUI();
}

void UDesignUMG::ShowComponentDetail()
{
	m_Canv_Right->SetVisibility(ESlateVisibility::Hidden);
	m_Canv_Bottom->SetVisibility(ESlateVisibility::Visible);
}

void UDesignUMG::ShowSliceMenuButton()
{
	m_Btn_SliceMenu->SetVisibility(ESlateVisibility::Visible);
}

void UDesignUMG::ShowDetailPage()
{
	if(!m_Canv_Detail->IsVisible())
		m_Canv_Detail->SetVisibility(ESlateVisibility::Visible);

	m_Canv_Right->SetVisibility(ESlateVisibility::Hidden);
}

void UDesignUMG::HidenSuiComponentMenu()
{
	m_UMG_SuitComponentMenu->OnBackstageUI();
}

void UDesignUMG::HidenSuitPartMenu()
{
	m_UMG_SuitPartMenu->OnBackstageUI();
}

void UDesignUMG::HidenSuitSliceMenu()
{
	m_UMG_SuitSliceMenu->OnActivationUI();
}

void UDesignUMG::HidenComponentDetail()
{
	m_Canv_Right->SetVisibility(ESlateVisibility::Visible);
	m_Canv_Bottom->SetVisibility(ESlateVisibility::Hidden);
}

void UDesignUMG::HidenSliceMenuButton()
{
	m_Btn_SliceMenu->SetVisibility(ESlateVisibility::Hidden);
}

void UDesignUMG::HidenDetailPage()
{
	m_DetailPage->OnBackstageUI();
	m_Canv_Detail->SetVisibility(ESlateVisibility::Hidden);
}

void UDesignUMG::SetBuyUrl(FString _buyUrl)
{
	m_BuyUrl = _buyUrl;
}

void UDesignUMG::SetDetailShopView(bool _CanShop)
{
	if(_CanShop)
		m_Canv_DetailShop->SetVisibility(ESlateVisibility::Visible);
	else
		m_Canv_DetailShop->SetVisibility(ESlateVisibility::Hidden);
}

void UDesignUMG::DoSave()
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

int32 UDesignUMG::GetStyleSliceTid()
{
	return 0;
}

void UDesignUMG::OnChangeSuitComponent_Implementation(FMeshComponentStruct _MeshCompStruct)
{

}

void UDesignUMG::OnChangeSuitSlice_Implementation(FMeshComponentStruct _MeshCompStruct)
{

}

void UDesignUMG::OnReturnToSuitPartMenu_Implementation()
{

}

void UDesignUMG::OnReturnToCompMenu_Implementation()
{

}

void UDesignUMG::InitCamera_Implementation(FMeshComponentStruct _MeshCompStruct)
{

}


FMeshComponentStruct UDesignUMG::GetStyleMeshComponentStruct()
{
	DesignModule *mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");
	int32 mSuitTid = mDesignModule->GetDesignData()->m_designTID;

	LogicDesignScene *mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();
	return mLogicDesignScene->GetComponentStructByTid(mSuitTid, EnumComponentType::STYLE_COMPONENT);
}

void UDesignUMG::OnSaveButtonClick_Implementation()
{
	
}


void UDesignUMG::UpdateSliceDetailPage()
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

void UDesignUMG::UpdateComponentDetailPage(UsedFabricStruct _UsedFabricStruct, bool isInitDetailPage)
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

void UDesignUMG::UpdatePartDetailPage(UsedFabricStruct _UsedFabricStruct, bool isInitDetailPage)
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
void UDesignUMG::SetDetailTexture(UTexture2D * _texture2D)
{
	m_ImgDetailTexture->GetDynamicMaterial()->SetScalarParameterValue("A", 1);

	m_ImgDetailTexture->GetDynamicMaterial()->SetTextureParameterValue("Texture", _texture2D);
}

void UDesignUMG::SetDetailColor(FLinearColor _RGB, UTexture2D * _texture2D)
{
	if (!_texture2D->IsValidLowLevel())
	{
		m_ImgDetailTexture->GetDynamicMaterial()->SetScalarParameterValue("A", -1);

		m_ImgDetailTexture->GetDynamicMaterial()->SetVectorParameterValue("Color", _RGB);
	}
	else
	{
		m_ImgDetailTexture->GetDynamicMaterial()->SetScalarParameterValue("A", 1);

		m_ImgDetailTexture->GetDynamicMaterial()->SetTextureParameterValue("Texture", _texture2D);
	}
}

void UDesignUMG::SetDetailMaterial(FLinearColor _RGB, UTexture2D * _texture2D)
{
	if (!_texture2D->IsValidLowLevel())
	{
		m_ImgDetailTexture->GetDynamicMaterial()->SetScalarParameterValue("A", -1);

		m_ImgDetailTexture->GetDynamicMaterial()->SetVectorParameterValue("Color", _RGB);
	}
	else
	{
		m_ImgDetailTexture->GetDynamicMaterial()->SetScalarParameterValue("A", 1);

		m_ImgDetailTexture->GetDynamicMaterial()->SetTextureParameterValue("Texture", _texture2D);
	}
}

void UDesignUMG::OnShopError()
{

}

void UDesignUMG::OnCompMeshShow(msg_ptr _msg)
{
	if (m_IsDetailCompCellClicked)
	{
		m_IsDetailCompCellClicked=false;
		//延迟移动摄像机.
		GetCamCompMeshData();
	}
}

void UDesignUMG::GetCamCompMeshData()
{
	LogicDesignScene *mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();
	if (mLogicDesignScene!=nullptr)
	{
		FMeshComponentStruct mCompStruct=mLogicDesignScene->GetComponentStructByTid(m_ComponentDetail->m_CurSelectedTid, EnumComponentType::PIECES_COMPONENT);
		if (mCompStruct.m_meshComponentArray.Num()>0)
		{
			//移动摄像机.
			OnChangeSuitComponent(mCompStruct);
		}
		else
		{
			//!!!正式包记得关掉打印.
			GEngine->AddOnScreenDebugMessage(0, 3000, FColor::Green, FString("FMeshComponentStruct is null , Camera can't move!"));
		}
		
	}
}


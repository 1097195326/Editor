
#include "Win_StyleSpecialItem.h"
#include "UIManager.h"
#include "GameDirector.h"
#include "LogicMainScene.h"
#include "MainModule.h"
#include "GModuleManager.h"
#include "HomeInfoManager.h"




UWin_StyleSpecialItem::UWin_StyleSpecialItem(const FObjectInitializer& ObjectInitializer)
{

}

bool UWin_StyleSpecialItem::OnStart()
{
	FText tile = FText::FromString(HomeInfoManager::GetInstance()->GetItemTitle());
	m_ShowTitle->SetText(tile);

	CreateSelectCell();
	return true;
}

void UWin_StyleSpecialItem::OnInit()
{
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, 1022, this, &UWin_StyleSpecialItem::ClickCallBack);


	if (UButton* _BackButton = Cast<UButton>(GetWidgetFromName("BackButton")))
	{
		m_BackButton = _BackButton;
		UUIManager::GetInstance()->RegisterButtonClick(1, m_BackButton, this, &UWin_StyleSpecialItem::OnBackClick);
	}
	if (UTextBlock* mShowTitle = Cast<UTextBlock>(GetWidgetFromName("ShowItemText")))
	{
		m_ShowTitle = mShowTitle;
	}
	if (UScrollBox* ScrollBoxItem = Cast<UScrollBox>(GetWidgetFromName("ScrollBoxItem")))
	{
		m_ScrollBoxItem = ScrollBoxItem;
	}
	if (UGridPanel* GridPanelSuitItem = Cast<UGridPanel>(GetWidgetFromName("GridPanelSuitItem")))
	{
		m_GridPanelSuitItem = GridPanelSuitItem;
	}

}

void UWin_StyleSpecialItem::OnRelease()
{
	Super::OnRelease();

	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, 1022, this, &UWin_StyleSpecialItem::ClickCallBack);
}

void UWin_StyleSpecialItem::OnSetParams(int32 _pid, FString & _content, ...)
{

}

void UWin_StyleSpecialItem::OnTick(float InDeltaTimeSecond)
{

}
void UWin_StyleSpecialItem::OnBackClick(int32 index)
{
	RemoveFromParent();
	UBaseUI * RecommendStyle = UUIManager::GetInstance()->OpenUI(TEXT("RecommendStyle"));
	RecommendStyle->AddToViewport();
}
void UWin_StyleSpecialItem::CreateSelectCell()
{
	TArray<HomeInfoT*> mArr = HomeInfoManager::GetInstance()->GetCurrentDataList();

	UCanvasPanelSlot * mSlot = (UCanvasPanelSlot*)m_ScrollBoxItem->Slot;
	float m_Width = mSlot->GetSize().X;
	int _num = mArr.Num();

	for (int i = 0; i < _num; ++i)
	{
		UWin_StyleSpecialIcon * mTemp = (UWin_StyleSpecialIcon*)UUIManager::GetInstance()->OpenUI(TEXT("StyleSpecialIcon"));

		if (mTemp->IsValidLowLevel())
		{
			UGridSlot * mGridSlot = m_GridPanelSuitItem->AddChildToGrid(mTemp);

			mGridSlot->SetRow(i);
			mTemp->SetDesignWidth(m_Width);
			mTemp->SetIconURL(mArr[i]->m_contentImg);
			mTemp->SetContentId(mArr[i]->m_contentId);
			mTemp->SetDesignType(EnumDesignType::DESIGN_BY_SYS_STYLE);
			mTemp->DownloadIconImage();

			m_AllSyles.Add(mTemp);
		}

	}
	
}
void UWin_StyleSpecialItem::ClickCallBack(msg_ptr _msg)
{
	UE_LOG(LogTemp, Log, TEXT("ZHX : UWin_StyleSpecialIcon::ClickCallBack"));
	MainModule * mMainModule = GModuleManager::GetInstance()->GetModule<MainModule>("MainModule");

	TArray<SelectCellData> mArr = mMainModule->m_Sys_SelectCell_Dats_ById;
	SelectCellData data = mArr[0];

	LogicMainScene *mLogicMainScene = (LogicMainScene *)UGameDirector::GetInstance()->GetCurrentLogicScene();

	mLogicMainScene->BeginJoinDesignLevel(data.m_Rid, data.m_Tid, EnumDesignType::DESIGN_BY_SYS_STYLE);


}
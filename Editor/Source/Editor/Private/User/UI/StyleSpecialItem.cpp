
#include "StyleSpecialItem.h"
#include "UIManager.h"
#include "GameDirector.h"
#include "LogicMainScene.h"
#include "MainModule.h"
#include "GModuleManager.h"
#include "HomeInfoManager.h"




UStyleSpecialItem::UStyleSpecialItem(const FObjectInitializer& ObjectInitializer)
{

}

bool UStyleSpecialItem::OnStart()
{
	FText tile = FText::FromString(HomeInfoManager::GetInstance()->GetItemTitle());
	m_ShowTitle->SetText(tile);

	CreateSelectCell();
	return true;
}

void UStyleSpecialItem::OnInit()
{
	

	if (UButton* _BackButton = Cast<UButton>(GetWidgetFromName("BackButton")))
	{
		m_BackButton = _BackButton;
		UUIManager::GetInstance()->RegisterButtonClick(1, m_BackButton, this, &UStyleSpecialItem::OnBackClick);
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

void UStyleSpecialItem::OnRelease()
{
	Super::OnRelease();

}

void UStyleSpecialItem::OnSetParams(int32 _pid, FString & _content, ...)
{

}

void UStyleSpecialItem::OnTick(float InDeltaTimeSecond)
{

}
void UStyleSpecialItem::OnBackClick(int32 index)
{
	RemoveFromParent();
	UBaseUI * RecommendStyle = UUIManager::GetInstance()->OpenUI(TEXT("RecommendStyle"));
	RecommendStyle->AddToViewport();
}
void UStyleSpecialItem::CreateSelectCell()
{
	TArray<HomeInfoT*> mArr = HomeInfoManager::GetInstance()->GetCurrentDataList();

	UCanvasPanelSlot * mSlot = (UCanvasPanelSlot*)m_ScrollBoxItem->Slot;
	float m_Width = mSlot->GetSize().X;
	int _num = mArr.Num();

	for (int i = 0; i < _num; ++i)
	{
		UStyleSpecialIcon * mTemp = (UStyleSpecialIcon*)UUIManager::GetInstance()->OpenUI(TEXT("StyleSpecialIcon"));

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
void UStyleSpecialItem::ClickCallBack(msg_ptr _msg)
{
	UE_LOG(LogTemp, Log, TEXT("ZHX : UStyleSpecialIcon::ClickCallBack"));
	MainModule * mMainModule = GModuleManager::GetInstance()->GetModule<MainModule>("MainModule");

	TArray<SelectCellData> mArr = mMainModule->m_Sys_SelectCell_Dats_ById;
	SelectCellData data = mArr[0];

	LogicMainScene *mLogicMainScene = (LogicMainScene *)UGameDirector::GetInstance()->GetCurrentLogicScene();

	mLogicMainScene->BeginJoinDesignLevel(data.m_Rid, data.m_Tid, EnumDesignType::DESIGN_BY_SYS_STYLE);


}
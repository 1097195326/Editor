// Fill out your copyright notice in the Description page of Project Settings.

#include "Win_StyleSpecialShow.h"
#include "UIManager.h"
#include "GameDirector.h"
#include "LogicMainScene.h"
#include "Win_StyleSpecialButton.h"
#include "HomeInfoManager.h"
#include "GameDirector.h"
#include "Win_RecommendStyleUMG.h"

static int m_index = 0;

UWin_StyleSpecialShow::UWin_StyleSpecialShow(const FObjectInitializer& ObjectInitializer)
{

}

bool UWin_StyleSpecialShow::OnStart()
{
	m_index = 0;
	if(UScrollBox * mScrollBox = (UScrollBox *)GetWidgetFromName(TEXT("ScrollTable")))
	{
		m_ScrollBox = mScrollBox;
		
	}
	m_ScrollBox->OnUserScrolled.AddDynamic(this, &UWin_StyleSpecialShow::OnUserScrolledEvent);

	if (UCanvasPanel * mCanvasPanel = (UCanvasPanel *)GetWidgetFromName(TEXT("SignPanel")))
	{
		m_SignPanel = mCanvasPanel;
	}
	
	TArray<HomeInfoT*> AllInfo;
	if (UWin_RecommendStyleUMG::m_ParentId == 1006)
	{
		AllInfo = HomeInfoManager::GetInstance()->GetBikiniTypes();
	}
	else if (UWin_RecommendStyleUMG::m_ParentId == 1025)
	{
		AllInfo = HomeInfoManager::GetInstance()->GetSportTypes();
	}
	else
	{
		AllInfo = HomeInfoManager::GetInstance()->GetBikiniTypes();
	}

	UCanvasPanelSlot * mCps = (UCanvasPanelSlot*)m_ScrollBox->Slot;
	FVector2D mSize = mCps->GetSize();
	m_SizeNum = AllInfo.Num();
	int SignWidght = 70 + 20;

	for (int i = 0; i < m_SizeNum; ++i)
	{
		UWin_StyleSpecialSign * mShowSign = (UWin_StyleSpecialSign *)UUIManager::GetInstance()->OpenUI(TEXT("StyleSpecialSign"));
		UCanvasPanelSlot * mSlot =(UCanvasPanelSlot*) m_SignPanel->AddChild(mShowSign);
		m_AllShowSigns.Add(mShowSign);
		FVector2D SignSize = mSlot->GetSize();
		int StartPosX = (mSize.X  - (SignSize.X  * m_SizeNum)) / 2;
		mSlot->SetOffsets(FMargin(StartPosX + i * SignSize.X, 0, 70, 9));
	}
	for (HomeInfoT * info : AllInfo)
	{
		UWin_StyleSpecialButton * mShowButton = (UWin_StyleSpecialButton *)UUIManager::GetInstance()->OpenUI(TEXT("StyleSpecialButton"));
		m_ScrollBox->AddChild(mShowButton);
		
		mShowButton->SetLogicParent(m_Parent);
		mShowButton->InitWithData(info);
		mShowButton->DownloadImage();
		m_AllShowButtons.Add(mShowButton);
	}
	m_ScrollBox->ScrollToStart();
	
	UE_LOG(LogTemp, Log, TEXT("zhx : show button  = %d"), m_AllShowButtons.Num())
	UGameDirector::GetInstance()->GetTimerManager().SetTimer(m_TimerHandle, this, &UWin_StyleSpecialShow::TickByTimter, 5, true,0);

	return true;
}

void UWin_StyleSpecialShow::OnInit()
{
	
}
void UWin_StyleSpecialShow::SetLogicParent(UWin_RecommendStyleUMG * parent)
{
	m_Parent = parent;
}

void UWin_StyleSpecialShow::OnRelease()
{
	Super::OnRelease();
	UGameDirector::GetInstance()->GetTimerManager().ClearTimer(m_TimerHandle);
	//m_index = 0;
}
void UWin_StyleSpecialShow::OnUserScrolledEvent(float _OffSet)
{
	UCanvasPanelSlot * mCps = (UCanvasPanelSlot*)m_ScrollBox->Slot;
	FVector2D mSize = mCps->GetSize();
	
	m_index = (int)m_ScrollBox->GetScrollOffset() / (int)mSize.X + 1;

	if ((int)m_ScrollBox->GetScrollOffset() % (int)mSize.X > 0)
	{
		int index = (int)m_ScrollBox->GetScrollOffset() / (int)mSize.X;

		for (UWin_StyleSpecialSign * sign : m_AllShowSigns)
		{
			sign->SetNormal();
		}
		m_AllShowSigns[index]->SetSelect();
	}
}
void UWin_StyleSpecialShow::TickByTimter()
{
	m_index = m_index % m_SizeNum;
	m_ScrollBox->ScrollWidgetIntoView(m_AllShowButtons[m_index]);

	for (UWin_StyleSpecialSign * sign : m_AllShowSigns)
	{
		sign->SetNormal();
	}
	m_AllShowSigns[m_index]->SetSelect();

	++m_index;
}
void UWin_StyleSpecialShow::OnSetParams(int32 _pid, FString & _content, ...)
{

}

void UWin_StyleSpecialShow::OnTick(float InDeltaTimeSecond)
{

}

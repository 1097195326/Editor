// Fill out your copyright notice in the Description page of Project Settings.

#include "StylePatternC.h"
#include "RStylePatternManager.h"
#include "TSuitPatternManager.h"
#include "SuitPatternT.h"
#include "StylePatternR.h"

StylePatternC::StylePatternC()
{
}

StylePatternC::~StylePatternC()
{
}

void StylePatternC::OnLoad()
{

}

void StylePatternC::OnRelease()
{
}

void StylePatternC::OnInitData(int32 _cid, int32 _rid, int32 _tid)
{
	m_dataT = TSuitPatternManager::GetInstance()->GetPatternData(m_patternTid);
}

void StylePatternC::AddJsonC(TSharedPtr<FJsonObject> & _jsonObj)
{

}

void StylePatternC::ChangePatternTid(const FString & _patternTid)
{
	m_uVScaleUser.X = 1.0f;

	m_uVScaleUser.Y = 1.0f;

	m_patternTid = _patternTid;

	m_dataT = TSuitPatternManager::GetInstance()->GetPatternData(_patternTid);
}

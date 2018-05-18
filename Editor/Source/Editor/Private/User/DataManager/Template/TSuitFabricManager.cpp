// Fill out your copyright notice in the Description page of Project Settings.

#include "TSuitFabricManager.h"
#include "SuitFabricT.h"
#include "SuitFabricColorLibraryT.h"
#include "TSuitFabricColorManager.h"
#include "TSuitFabricColorLibraryManager.h"

TSuitFabricManager *   TSuitFabricManager::m_pInstance = nullptr;

TSuitFabricManager * TSuitFabricManager::GetInstance()
{
	if (m_pInstance == nullptr)  //判断是否第一次调用.
	{
		m_pInstance = new TSuitFabricManager();

		m_pInstance->GLoad();
	}
	return m_pInstance;
}

TSuitFabricManager::TSuitFabricManager()
{
}

TSuitFabricManager::~TSuitFabricManager()
{
}

void TSuitFabricManager::OnLoad()
{

}

void TSuitFabricManager::OnRelease()
{
	m_pInstance = nullptr;
}

void TSuitFabricManager::EncodeData(TSharedPtr<FJsonObject> & _content)
{
	TArray<TSharedPtr<FJsonValue>> mJsonData = _content->GetArrayField("data");

	// 3.解析数据.
	for (int32 i = 0; i < mJsonData.Num(); i++)
	{
		TSharedPtr<FJsonValue> mJson = mJsonData[i];

		TSharedPtr<FJsonObject> mContentJson = mJson->AsObject();

		SuitFabricT * mData = new SuitFabricT();

		mData->GLoad();

		mData->InitWithJson(mContentJson);

		// 添加到容器里.
		AddData(mData);
	}
}

int32 TSuitFabricManager::CheckExternColor(int32 _colorTid, int32 _fabricTid)
{
	SuitFabricT * mFabricT = GetTemplateByPID<SuitFabricT>(_fabricTid);

	SuitFabricColorT * mColorT = TSuitFabricColorManager::GetInstance()->GetTemplateByPID<SuitFabricColorT>(_colorTid);

	// 获取主推色.
	int32 mDefaultPantonTid = 0;

	SuitFabricColorLibraryT * mLibraryT = TSuitFabricColorLibraryManager::GetInstance()->GetTemplateByPID<SuitFabricColorLibraryT>(mFabricT->m_ColorRepositoryId);

	for (int32 i = 0; i < mLibraryT->m_fabricColorIds.Num(); i++)
	{
		SuitFabricColorT * mTargetColorT = TSuitFabricColorManager::GetInstance()->GetTemplateByPID<SuitFabricColorT>(mLibraryT->m_fabricColorIds[i]);

		if (mTargetColorT->m_PantoneCode.Equals(mColorT->m_PantoneCode))
		{
			return mTargetColorT->GetTid();
		}
		else if (mFabricT->m_PrimaryPantoneCode.Equals(mTargetColorT->m_PantoneCode))
		{
			// 检查一下推荐的潘通色号.
			mDefaultPantonTid = mTargetColorT->GetTid();
		}
		if (i==0)
		{
			mDefaultPantonTid = mTargetColorT->GetTid();
		}
	}

	return mDefaultPantonTid;
}

SuitFabricColorT * TSuitFabricManager::GetDefaultFabricColor(int32 _fabricTid)
{
	SuitFabricT * mFabricT = GetTemplateByPID<SuitFabricT>(_fabricTid);

	SuitFabricColorLibraryT * mLibraryT = TSuitFabricColorLibraryManager::GetInstance()->GetTemplateByPID<SuitFabricColorLibraryT>(mFabricT->m_ColorRepositoryId);

	SuitFabricColorT * mDefaultColorT = nullptr;

	for (int32 i = 0; i < mLibraryT->m_fabricColorIds.Num(); i++)
	{
		SuitFabricColorT * mTargetColorT = TSuitFabricColorManager::GetInstance()->GetTemplateByPID<SuitFabricColorT>(mLibraryT->m_fabricColorIds[i]);

		if (mFabricT->m_PrimaryPantoneCode.Equals(mTargetColorT->m_PantoneCode))
		{
			// 检查一下推荐的潘通色号.
			mDefaultColorT = mTargetColorT;
		}
		if (i == 0)
		{
			mDefaultColorT = mTargetColorT;
		}
	}

	return mDefaultColorT;
}


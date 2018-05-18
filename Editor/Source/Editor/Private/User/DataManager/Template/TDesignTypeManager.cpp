

#include "TDesignTypeManager.h"
#include "TSuitManager.h"
#include "SuitT.h"


TDesignTypeManager *   TDesignTypeManager::m_pInstance = nullptr;

TDesignTypeManager * TDesignTypeManager::GetInstance()
{
	if (m_pInstance == nullptr)  //判断是否第一次调用.
	{
		m_pInstance = new TDesignTypeManager();

		m_pInstance->GLoad();
	}
	return m_pInstance;
}


TDesignTypeManager::TDesignTypeManager()
{
}

TDesignTypeManager::~TDesignTypeManager()
{
}

void TDesignTypeManager::OnLoad()
{

}

void TDesignTypeManager::OnRelease()
{
	m_pInstance = nullptr;
}

void TDesignTypeManager::EncodeData(TSharedPtr<FJsonObject> & _content)
{
	const TArray<TSharedPtr<FJsonValue>>& mContentArray = _content->GetArrayField("data");

	for (int32 i = 0; i < mContentArray.Num(); i++)
	{
		TSharedPtr<FJsonValue> mStyleJson = mContentArray[i];

		const TSharedPtr<FJsonObject>& mStyleObject = mStyleJson->AsObject();

		DesignType * mData = new DesignType();

		mData->GLoad();

		mData->InitWithJson(mStyleObject);

		AddTypeData(mData);
		//// 添加到容器里.
		//AddData(mData);
	}
}

void TDesignTypeManager::AddTypeData(DesignType * _designType)
{
	for (int32 i = 0; i < m_AllTypesArr.Num(); i++)
	{
		DesignType * mType = m_AllTypesArr[i];

		if (mType->m_Tid == _designType->m_Tid)
		{
			m_AllTypesArr.RemoveAt(i);
			delete mType;
		}
	}
	m_AllTypesArr.Add(_designType);
}

int32 TDesignTypeManager::GetParentCatIdBySuitTid(int32 _suitTid)
{
	SuitT * mSuitT = TSuitManager::GetInstance()->GetTemplateByPID<SuitT>(_suitTid);

	int32 mSuitCatId = mSuitT->m_catId;

	for (int32 i=0;i<m_AllTypesArr.Num();i++)
	{
		DesignType * mType = m_AllTypesArr[i];

		if (mType->m_Tid == mSuitCatId)
		{
			return mType->m_ParentId;
		}
	}
	return 0;
}

int32 TDesignTypeManager::GetCatIdBySuitTid(int32 _suitTid)
{
	SuitT * mSuitT = TSuitManager::GetInstance()->GetTemplateByPID<SuitT>(_suitTid);

	int32 mSuitCatId = mSuitT->m_catId;

	for (int32 i = 0; i < m_AllTypesArr.Num(); i++)
	{
		DesignType * mType = m_AllTypesArr[i];

		if (mType->m_Tid == mSuitCatId)
		{
			return mType->m_Tid;
		}
	}
	return 0;
}

DesignType * TDesignTypeManager::GetDesignTypeById(int32 _id)
{
	return (DesignType*)m_templateMap[_id];
}
TArray<DesignType *> TDesignTypeManager::GetAllDesignTypesByParentId(int32 _parentId)
{
	TArray<DesignType*> Arr;
	for (auto dt : m_AllTypesArr)
	{
		if (dt->m_ParentId == _parentId)
		{
			Arr.Add(dt);
		}
	}
	Arr.Sort([](const DesignType & A, const DesignType & B)->bool {
		return A.m_Sort < B.m_Sort;
	});
	return Arr;
}


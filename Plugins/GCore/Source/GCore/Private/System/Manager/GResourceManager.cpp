// Fill out your copyright notice in the Description page of Project Settings.

#include "GCore.h"
#include "GResourceManager.h"


UGResourceManager *       UGResourceManager::m_instance = nullptr;

UGResourceManager::UGResourceManager()
{
	
}

UGResourceManager::~UGResourceManager()
{

}

UGResourceManager * UGResourceManager::GetInstance()
{
	if (nullptr == m_instance)
	{
		m_instance = NewObject<UGResourceManager>();

		m_instance->GLoad();
	}
	return m_instance;
}

void UGResourceManager::GLoad()
{
	AddToRoot();
}

void UGResourceManager::GRelease()
{
	this->ClearCache();

	this->RemoveFromRoot();

	m_instance = nullptr;
}

UClass* UGResourceManager::GetUClassFromCache(const FString & _assetPath)
{
	if (m_resourcesMap.Contains(*_assetPath))
		return m_resourcesMap[*_assetPath];

	return nullptr;
}

UObject* UGResourceManager::GetUObjectFromCache(const FString & _objectPath)
{
	if (m_objectMap.Contains(*_objectPath))
		return m_objectMap[*_objectPath];

	return nullptr;
}

UClass* UGResourceManager::LoadAssetToCache(const FString & _assetPath)
{
	UClass* loadClass = LoadClass<AActor>(NULL, *_assetPath);

	// 2. 要是没有就创建一个新的模型并且保存在map中.
	if (loadClass != nullptr)
		m_resourcesMap.Add(*_assetPath, loadClass);

	return loadClass;
}

void UGResourceManager::ClearCache()
{
	// 遍历删除.
	for (auto Iter = m_resourcesMap.CreateIterator(); Iter; ++Iter)
	{
		UClass* cls = Iter.Value();

	}
	m_resourcesMap.Empty();

	for (auto Iter = m_objectMap.CreateIterator(); Iter; ++Iter)
	{
		UObject* obj = Iter.Value();

	}
	m_objectMap.Empty();
}

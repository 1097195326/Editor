// Fill out your copyright notice in the Description page of Project Settings.

#include "GCore.h"
#include "UITextureManager.h"
#include "DownloadImageTool.h"
#include "LoadAssetTool.h"
#include "GFileHelper.h"



UUITextureManager::UUITextureManager()
{
}
UUITextureManager::~UUITextureManager()
{
}

void UUITextureManager::GLoad()
{
	AddToRoot();
}

void UUITextureManager::GRelease()
{
	Clear();

	RemoveFromRoot();
}

void UUITextureManager::Clear()
{
	for (auto Iter = m_resourceMap.CreateIterator(); Iter; ++Iter)
	{
		UTexture2D * mTexture2d = Iter->Value;

		mTexture2d->ReleaseResource();

		Iter.RemoveCurrent();
	}
	m_resourceMap.Empty();

	for (auto Iter = m_contentResourceMap.CreateIterator(); Iter; ++Iter)
	{
		/*UTexture2D * mTexture2d = Iter->Value;

		mTexture2d->ReleaseResource();*/

		Iter.RemoveCurrent();
	}
	m_contentResourceMap.Empty();

	for (auto Iter = m_resourceMapForModel.CreateIterator(); Iter; ++Iter)
	{
		FModelTextureStruct * mStruct = Iter->Value;

		Iter.RemoveCurrent();

		// 删除掉.
		mStruct->m_texture2d = nullptr;

		delete mStruct;
	}

	m_resourceMapForModel.Empty();
}

int32 UUITextureManager::ResourceCount()
{
	return m_resourceMap.Num();
}

void UUITextureManager::AddContentTexture2D(const FString & _key, UTexture2D * _texture2D)
{
	if (m_contentResourceMap.Contains(_key))
	{
		m_contentResourceMap.Remove(_key);

		m_contentResourceMap.Add(_key, _texture2D);
	}
	else
	{
		m_contentResourceMap.Add(_key, _texture2D);
	}
}

void UUITextureManager::AddTexture2D(const FString & _key, UTexture2D * _texture2D)
{
	if (m_resourceMap.Contains(_key))
	{
		m_resourceMap.Remove(_key);

		m_resourceMap.Add(_key, _texture2D);
	}
	else
	{
		m_resourceMap.Add(_key, _texture2D);
	}
}

UTexture2D * UUITextureManager::GetTexture2d(const FString & _key)
{
	if (m_resourceMap.Contains(_key))
	{
		return m_resourceMap[_key];
	}
	return nullptr;
}


UTexture2D * UUITextureManager::GetTexture2dByPath(const FString & _path)
{
	if (m_resourceMap.Contains(_path))
	{
		return m_resourceMap[_path];
	}
	else
	{
		TArray<uint8> CompressedData;

		if (!GFileHelper::LoadFileToArray(CompressedData, *_path))
		{
			return nullptr;
		}

		UTexture2D * mTexture2D = UDownloadImageTool::GetTexture2dByUnit8Array(CompressedData, CompressedData.Num());

		if (nullptr != mTexture2D)
		{
			AddTexture2D(_path, mTexture2D);

			return mTexture2D;
		}
	}
	return nullptr;
}

UTexture2D * UUITextureManager::GetTexture2dInContent(const FString & _path)
{
	if (m_contentResourceMap.Contains(_path))
	{
		return m_contentResourceMap[_path];
	}
	else
	{
		UTexture2D * mTexture2D = ULoadAssetTool::GetInstance()->LoadUAsset<UTexture2D>(_path);
		
		if (nullptr != mTexture2D)
		{
			AddContentTexture2D(_path, mTexture2D);

			return mTexture2D;
		}
	}
	return nullptr;
}

void UUITextureManager::AddTexture2DForModel(const FString & _key, FModelTextureStruct * _modelTextureStruct)
{
	if (m_resourceMapForModel.Contains(_key))
	{
		FModelTextureStruct * mStruct = m_resourceMapForModel[_key];

		m_resourceMapForModel.Remove(_key);

		// 释放内存.
		mStruct->m_texture2d = nullptr;

		delete mStruct;
	}
	m_resourceMapForModel.Add(_key, _modelTextureStruct);
}

FModelTextureStruct * UUITextureManager::GetTexture2dForModel(const FString & _key)
{
	if (m_resourceMapForModel.Contains(_key))
	{
		FModelTextureStruct * mStruct = m_resourceMapForModel[_key];

		mStruct->m_useCount = mStruct->m_useCount + 1;

		return mStruct;
	}
	return nullptr;
}


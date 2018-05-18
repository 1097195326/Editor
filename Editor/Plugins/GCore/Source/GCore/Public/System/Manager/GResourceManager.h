// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ILoad.h"
#include "GameDirector.h"
#include "LoadAssetTool.h"
#include "UObject/NoExportTypes.h"
#include "GResourceManager.generated.h"

/**
 * 资源管理.
 */
UCLASS()
class GCORE_API UGResourceManager : public UObject,public ILoad
{
	GENERATED_BODY()

private:
	// class资源列表.
	UPROPERTY()
	TMap<FString, UClass*>          m_resourcesMap;

	// object资源列表=包含mesh，animation，texture等等.
	UPROPERTY()
	TMap<FString, UObject*>          m_objectMap;

	static UGResourceManager *       m_instance;
	
public:
	UGResourceManager();

	virtual ~UGResourceManager();

	static UGResourceManager * GetInstance();
	
	virtual void GLoad();

	virtual void GRelease();

	// 从缓存里获取资源.
	UClass* GetUClassFromCache(const FString & _assetPath);

	// 从缓存里获取资源.
	UObject* GetUObjectFromCache(const FString & _objectPath);

	// 缓存资源.
	UClass* LoadAssetToCache(const FString & _assetPath);

	// 清空缓存.
	void ClearCache();

	// 加载资源同是缓存到内存当中.
	// 参数穿一个FString的引用.
	template<typename T>
	T* LoadAssetAndCache(const FString & _assetPath)
	{
		if (!_assetPath.IsEmpty())
		{
			// 1. 先判断map里有没有这个模型.
			UClass* loadClass = GetUClassFromCache(_assetPath);

			if (loadClass == nullptr)
			{
				// 2. 要是没有就创建一个新的模型并且保存在map中.
				loadClass = LoadAssetToCache(_assetPath);
			}

			if (loadClass != nullptr)
			{
				FVector dicLocation = FVector(0, 0, 0);

				FRotator dicRotator = FRotator(0, 0, 0);

				T* _assetObj = UGameDirector::GetInstance()->GetWorld()->SpawnActor<T>(loadClass, dicLocation, dicRotator);

				return _assetObj;
			}
			return nullptr;
		}
		return nullptr;
	}

	// 加载一个资源但是不缓存.
	// 参数穿一个FString的引用.
	template<typename T>
	T* LoadAssetDontCache(const FString & _assetPath)
	{
		if (!_assetPath.IsEmpty())
		{
			// 1. 先判断map里有没有这个模型.
			UClass* loadClass = GetUClassFromCache(_assetPath);

			if (loadClass == nullptr)
			{
				// 2.

				loadClass = LoadClass<AActor>(NULL, *_assetPath);
			}

			if (loadClass != nullptr)
			{
				FVector dicLocation = FVector(0, 0, 0);

				FRotator dicRotator = FRotator(0, 0, 0);

				T* _assetObj = UGameDirector::GetInstance()->GetWorld()->SpawnActor<T>(loadClass, dicLocation, dicRotator);

				return _assetObj;
			}
			return nullptr;
		}
		return nullptr;
	}

	// 加载资源同是缓存到内存当中.
	// 参数穿一个FString的引用.
	template<typename T>
	T* LoadObjectAndCache(const FString & _objPath)
	{
		if (!_objPath.IsEmpty())
		{
			// 1. 先判断map里有没有这个模型.
			UObject* loadClass = GetUObjectFromCache(_objPath);

			if (loadClass == nullptr)
			{
				// 2. 要是没有就创建一个新的模型并且保存在map中.
				loadClass = ULoadAssetTool::GetInstance()->LoadUAsset<T>(_objPath);

				if (nullptr != loadClass)
					m_objectMap.Add(*_objPath, loadClass);
			}

			if (loadClass != nullptr)
			{
				return Cast<T>(loadClass);
			}
			return nullptr;
		}
		return nullptr;
	}

	// 加载一个资源但是不缓存.
	// 参数穿一个FString的引用.
	template<typename T>
	T* LoadObjectDontCache(const FString & _objPath)
	{
		if (!_objPath.IsEmpty())
		{
			// 1. 先判断map里有没有这个模型.
			UObject* loadClass = GetUObjectFromCache(_objPath);

			if (loadClass == nullptr)
			{
				// 2. 要是没有就创建一个新的.
				loadClass = ULoadAssetTool::GetInstance()->LoadUAsset<T>(_objPath);
			}

			if (loadClass != nullptr)
			{
				return Cast<T>(loadClass);
			}
			return nullptr;
		}
		return nullptr;
	}
};

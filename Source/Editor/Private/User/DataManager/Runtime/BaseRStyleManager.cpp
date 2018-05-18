// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseRStyleManager.h"
#include "GModuleManager.h"
#include "LoginModule.h"
#include "MsgCenter.h"
#include "Editor.h"
#include "StyleR.h"

BaseRStyleManager::BaseRStyleManager()
{
}

BaseRStyleManager::~BaseRStyleManager()
{
}

void BaseRStyleManager::OnLoad()
{

}

void BaseRStyleManager::OnRelease()
{

}

void BaseRStyleManager::EncodeStyleData(TSharedPtr<FJsonObject> & _jsonObject, TArray<BaseR*> & _arrayData)
{
	// 反序列化数据并添加到管理器中.

	const TArray<TSharedPtr<FJsonValue>>& mContentArray = _jsonObject->GetArrayField("content");

	for (int32 i = 0; i < mContentArray.Num(); i++)
	{
		TSharedPtr<FJsonValue> mStyleJson = mContentArray[i];

		const TSharedPtr<FJsonObject>& mStyleObject = mStyleJson->AsObject();

		StyleR * mStyle = new StyleR();

		mStyle->GLoad();

		mStyle->InitWithJson(mStyleObject);

		// 添加到容器里.
		AddData(mStyle);

		_arrayData.Add(mStyle);
	}
}
void BaseRStyleManager::EncodeStyleDataById(TSharedPtr<FJsonObject> & _jsonObject, TArray<BaseR*> & _arrayData)
{
	// 反序列化数据并添加到管理器中.

		StyleR * mStyle = new StyleR();

		mStyle->GLoad();

		mStyle->InitWithJson(_jsonObject);

		// 添加到容器里.
		AddData(mStyle);

		_arrayData.Add(mStyle);
	
}




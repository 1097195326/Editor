// Fill out your copyright notice in the Description page of Project Settings.

#include "GCore.h"
#include "BaseComponent.h"

BaseComponent::BaseComponent()
{
}

BaseComponent::~BaseComponent()
{
}

void BaseComponent::SetRender(GBaseRender * _render)
{
	m_owner = _render;
}

void BaseComponent::SetComponentName(const FString & _componentName)
{
	m_componentName = _componentName;
}

FString BaseComponent::GetComponentName()
{
	return m_componentName;
}  

void BaseComponent::SetComponentTag(int32 _tag)
{
	m_componentTag = _tag;
}

int32 BaseComponent::GetComponentTag()
{
	return m_componentTag;
}

int32 BaseComponent::GetComponentCID()
{
	return m_cid;
}

GBaseRender * BaseComponent::GetOwner()
{
	return m_owner;
}

void BaseComponent::StartRenderChildComponent()
{
	for (int32 i = m_childComponents.Num() - 1; i >= 0; i--)
	{
		IComponent * mComponent = m_childComponents[i];

		mComponent->StartDesignRender();
	}
}

void BaseComponent::StartSkeletalRenderChildComponent()
{
	for (int32 i = m_childComponents.Num() - 1; i >= 0; i--)
	{
		IComponent * mComponent = m_childComponents[i];

		mComponent->StartWalkingRender();
	}
}

void BaseComponent::GLoad()
{
	OnLoad();
}

void BaseComponent::GLoad(GBaseRender * _render)
{
	m_owner = _render;

	OnLoad();
}

void BaseComponent::ReleaseComponents()
{
	if (nullptr != m_staticMeshComponent)
	{
		m_staticMeshComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

		m_staticMeshComponent->UnregisterComponent();

		m_staticMeshComponent->DestroyComponent();

		//m_staticMeshComponent->GetStaticMesh()->ReleaseResources();

		m_staticMeshComponent->SetStaticMesh(nullptr);

		m_staticMeshComponent = nullptr;
	}

	if (nullptr != m_skeletalMeshComponent)
	{
		m_skeletalMeshComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

		m_skeletalMeshComponent->UnregisterComponent();

		m_skeletalMeshComponent->DestroyComponent();

		m_skeletalMeshComponent = nullptr;
	}
}

bool BaseComponent::CheckTag(TArray<EnumComponentType> & _componentType)
{
	if(_componentType.Num()==0)
		return true;

	for (int32 i=0;i<_componentType.Num();i++)
	{
		if (GetComponentTag() == _componentType[i])
		{
			return true;
		}
	}
	return false;
}

void BaseComponent::ReleaseRender()
{
	// 在吧子级删除.
	for (int32 i = m_childComponents.Num() - 1; i >= 0; i--)
	{
		IComponent * mComponent = m_childComponents[i];

		mComponent->ReleaseRender();
	}

	OnReleaseRender();
	ReleaseComponents();
}

void BaseComponent::GRelease()
{
	// 先从父级删除.
	if (nullptr != m_parentComponent)
	{
		for (int32 i= m_parentComponent->GetChildComponents().Num()-1;i>=0;i--)
		{
			BaseComponent * mComponent = (BaseComponent *)m_parentComponent->GetChildComponents()[i];
			
			if (mComponent == this)
			{
				m_parentComponent->RemoveChildComponent(this);

				break;;
			}
		}
	}

	// 在吧子级删除.
	for (int32 i=m_childComponents.Num()-1;i>=0;i--)
	{
		IComponent * mComponent = m_childComponents[i];

		m_childComponents.RemoveAt(i);

		mComponent->GRelease();
	}

	OnRelease();

	m_dataC->GRelease();

	ReleaseComponents();

	delete this;
}

BaseC * BaseComponent::GetDataC()
{
	return m_dataC;
}

void BaseComponent::GetAllTexturePath(TArray<FString> & _pathArray)
{
	int32 _tag = EnumComponentType::PATTERN_COMPONENT;

	if (_tag == GetComponentTag())
	{
		BaseComponent * mPatternComponent = (BaseComponent *)this;

		FString mPatternPath = mPatternComponent->GetPatternPath();

		FString mNullStr = TEXT("");

		if(nullptr != mPatternComponent && !mPatternPath.IsEmpty() && !mNullStr.Equals(mPatternPath))
			_pathArray.Add(mPatternPath);
	}
	else
	{
		for (int32 i = m_childComponents.Num() - 1; i >= 0; i--)
		{
			((BaseComponent*)m_childComponents[i])->GetAllTexturePath(_pathArray);
		}
	}
}

void BaseComponent::GetAllColorTexture(TArray<FString> & _pathArray)
{
	int32 _tag = EnumComponentType::FABRICCOLOR_COMPONENT;

	if (_tag == GetComponentTag())
	{
		TArray<FString> mTextureArray = this->GetColorTexture();

		FString mNullStr = TEXT("");

		for (int32 i=0;i<mTextureArray.Num();i++)
		{
			if (!mTextureArray[i].IsEmpty() && !mNullStr.Equals(mTextureArray[i]))
				_pathArray.Add(mTextureArray[i]);
		}
	}
	else
	{
		for (int32 i = m_childComponents.Num() - 1; i >= 0; i--)
		{
			((BaseComponent*)m_childComponents[i])->GetAllColorTexture(_pathArray);
		}
	}
}

void BaseComponent::GetAllPatternIds(TArray<FString> & _patternIds)
{
	int32 _tag = EnumComponentType::SLICE_COMPONENT;

	if (_tag == GetComponentTag())
	{
		BaseComponent * mPatternComponent = (BaseComponent *)this;

		FString mPatternId = mPatternComponent->GetPatternId();

		FString mNullStr = TEXT("");

		if (nullptr != mPatternComponent && !mPatternId.IsEmpty() && !mNullStr.Equals(mPatternId))
		{
			if(!_patternIds.Contains(mPatternId))
				_patternIds.Add(mPatternId);
		}
	}
	else
	{
		for (int32 i = m_childComponents.Num() - 1; i >= 0; i--)
		{
			((BaseComponent*)m_childComponents[i])->GetAllPatternIds(_patternIds);
		}
	}
}

void BaseComponent::InitData(int32 _rid, int32 _tid, int32 _cid, IComponent * _parentComponent)
{
	m_parentComponent = _parentComponent;

	m_cid = _cid;
	
	OnInitData(_rid, _tid, _cid);
}

IComponent * BaseComponent::GetParentComponent()
{
	return m_parentComponent;
}

IComponent * BaseComponent::GetParentComponentByTag(int32 _tag)
{
	if (_tag == m_componentTag)
	{
		return this;
	}
	else
	{
		// 继续往上找.
		if(nullptr != m_parentComponent)
			return m_parentComponent->GetParentComponentByTag(_tag);

		return nullptr;
	}
}

TArray<IComponent*> & BaseComponent::GetChildComponents()
{
	return m_childComponents;
}

void BaseComponent::AddChildComponent(IComponent * _component)
{
	m_childComponents.Add(_component);
}

IComponent * BaseComponent::GetComponentByName(const FString & _componentName)
{
	if (_componentName.Equals(GetComponentName()))
	{
		return this;
	}
	else
	{
		for (int32 i = m_childComponents.Num() - 1; i >= 0; i--)
		{
			IComponent * mComponent = m_childComponents[i]->GetComponentByName(_componentName);

			if (nullptr != mComponent)
				return mComponent;
		}
	}
	return nullptr;
}

void BaseComponent::GetComponentByTag(int32 _tag, TArray<IComponent*> & _childComponentArray)
{
	if (_tag == GetComponentTag())
	{
		_childComponentArray.Add(this);
	}
	for (int32 i = 0; i <m_childComponents.Num(); i++)
	{
		m_childComponents[i]->GetComponentByTag(_tag, _childComponentArray);
	}
}

IComponent * BaseComponent::GetComponentByCID(int32 _cid)
{
	if (_cid == m_cid)
	{
		return this;
	}
	else
	{
		for (int32 i = m_childComponents.Num() - 1; i >= 0; i--)
		{
			IComponent * mComponent = m_childComponents[i]->GetComponentByCID(_cid);

			if (nullptr != mComponent)
				return mComponent;
		}
	}
	return nullptr;
}

void BaseComponent::RemoveChildComponent(IComponent * _component)
{
	if (m_childComponents.Num() > 0)
	{
		for (int32 i = m_childComponents.Num() - 1; i >= 0; i--)
		{
			IComponent * mComponent = m_childComponents[i];

			if (mComponent == _component)
			{
				m_childComponents.RemoveAt(i);
			}
		}
	}
}



void BaseComponent::StartDesignRender()
{
	StartRenderChildComponent();

	OnStartRender();
}

void BaseComponent::StartWalkingRender()
{
	StartSkeletalRenderChildComponent();

	OnStartSkeletalRender();
}

BaseComponent * BaseComponent::GetComponentByTid(int32 _tid, TArray<EnumComponentType> & _componentType)
{
	BaseC * mDataC = m_dataC;

	BaseT * mBaseT = nullptr;

	if (nullptr != mDataC)
		mBaseT = mDataC->GetBaseT();

	if (mBaseT != nullptr && mBaseT->GetTid() == _tid && CheckTag(_componentType))
	{
		return this;
	}
	else
	{
		for (int32 i = m_childComponents.Num() - 1; i >= 0; i--)
		{
			BaseComponent * mChildComponent = (BaseComponent *)m_childComponents[i];

			BaseComponent * mComponent = mChildComponent->GetComponentByTid(_tid, _componentType);

			if (nullptr != mComponent)
				return mComponent;
		}
	}
	return nullptr;
}

void BaseComponent::OnLoad()
{

}

void BaseComponent::OnRelease()
{

}

void BaseComponent::OnReleaseRender()
{

}

void BaseComponent::OnInitData(int32 _rid, int32 _tid, int32 _cid)
{

}

FString BaseComponent::GetPatternPath()
{
	return TEXT("");
}

TArray<FString> BaseComponent::GetColorTexture()
{
	TArray<FString> mVar;
	return mVar;
}

void BaseComponent::ChangePiecesCompont(int32 _targetTid)
{

}

void BaseComponent::ChangePatternComponent(FString _targetTid)
{

}

void BaseComponent::ChangeColorComponent(int32 _targetTid)
{

}

void BaseComponent::ChangeFabricComponent(int32 _targetTid)
{

}

void BaseComponent::GetAllPatternInChildComponent(TArray<UsedFabricStruct> & _array)
{

}


void BaseComponent::GetAllFabricInfoIn(TArray<UsedFabricStruct> & _array)
{

}

void BaseComponent::ChangeSliceChildComponent(UsedFabricStruct & _usedFabricStruct)
{

}

void BaseComponent::ChangeRenderTargetTid(UsedFabricStruct & _usedFabricStruct)
{

}

void BaseComponent::CreateJson(const TSharedPtr<FJsonObject> & _saveInfoObject)
{

}

FString BaseComponent::GetPatternId()
{
	return TEXT("");
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "GCore.h"
#include "GBaseRender.h"

GBaseRender::GBaseRender()
{
}

GBaseRender::~GBaseRender()
{
}

void GBaseRender::GLoad()
{
	m_textureManager = NewObject<UUITextureManager>();

	m_textureManager->GLoad();

	OnLoad();
}

void GBaseRender::GRelease()
{
	m_textureManager->GRelease();

	OnRelease();

	if (nullptr != m_rootComponent)
		m_rootComponent->GRelease();

	if (nullptr != m_actor)
	{
		m_actor->Destroy();

		m_actor = nullptr;
	}

	delete this;
}

ABaseActor * GBaseRender::GetActor()
{
	return m_actor;
}

IComponent * GBaseRender::GetRootComponent()
{
	return m_rootComponent;
}

int32 GBaseRender::CreateComponentID()
{
	return m_cid;
}

UUITextureManager * GBaseRender::GetTextureManager()
{
	return m_textureManager;
}

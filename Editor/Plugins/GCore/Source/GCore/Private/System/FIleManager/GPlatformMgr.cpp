// Fill out your copyright notice in the Description page of Project Settings.
#include "GCore.h"
#include "GPlatformMgr.h"

#include "Runtime/Core/Public/Misc/Paths.h"
GPlatformMgr* GPlatformMgr::m_pInstance = nullptr;//初始化在主线程之前.

GPlatformMgr::GPlatformMgr()
{
}

GPlatformMgr::~GPlatformMgr()
{
}

// 单利.
GPlatformMgr * GPlatformMgr::GetInstance()
{
	if (m_pInstance == nullptr)  //判断是否第一次调用.
	{
		m_pInstance = new GPlatformMgr();
	}
	return m_pInstance;
}

void GPlatformMgr::GLoad()
{

}

void GPlatformMgr::GRelease()
{
	delete m_pInstance;

	m_pInstance = nullptr;
}

FString GPlatformMgr::GetPersistentDownloadDir()
{
	return FPaths::ProjectPersistentDownloadDir();
}

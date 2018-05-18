// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ILoad.h"
/**
 * 
 */
class GCORE_API GPlatformMgr:public ILoad
{
public:
	GPlatformMgr();
	~GPlatformMgr();

private:
	// 单利.
	static GPlatformMgr *          m_pInstance;

public:
	// 单利. 
	static GPlatformMgr * GetInstance();

public:
	virtual void GLoad() override;
	virtual void GRelease() override;

public:
	// 返回可读写的下载路径.
	FString GetPersistentDownloadDir();

};

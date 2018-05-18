// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// 实体类型.
enum EnumEntityType
{
	EACTOR,		//actor类型.
	EMAT,		//材质.
	ECOLOR,		//颜色.
	ETEXTURE,	//纹理.
	EDATA,		//数据.
};

// 实体数据对象类型.
enum EnumCDataType
{
	ESUIT,// 套件.
		  // 部位.
		  // 部件.
		  // 部片.
		  // 面料.
		  // 花型.
		  // 花色.
		  // 贴图.
		  // 线迹.
		  // 辅料.
};
/**
 * 
 */
class GCORE_API EnumDatas
{
public:
	EnumDatas();
	~EnumDatas();
};

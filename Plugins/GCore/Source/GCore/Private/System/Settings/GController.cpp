// Fill out your copyright notice in the Description page of Project Settings.
#include "GCore.h"
#include "GController.h"

FString GController::m_proportion=TEXT("1.66");

FString GController::m_appVersion = TEXT("1.0.0");

// 是否debug.
bool   GController::m_isDebug = false;

bool GController::m_isAdmin = false;

// 正在运行的平台.
ENUM_PLATFORM GController::m_platform = ENUM_PLATFORM::GWINDOWS;

// 语言.
ENUM_LANGUAGE GController::m_language = ENUM_LANGUAGE::GCHINESE;

int32 GController::m_screenWidth = 0;

int32 GController::m_screenHeight = 0;

bool GController::m_isadaptation = false;

GController::GController()
{
}

GController::~GController()
{
}

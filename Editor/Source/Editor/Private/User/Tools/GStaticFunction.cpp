// Fill out your copyright notice in the Description page of Project Settings.

#include "GStaticFunction.h"
#include "GModuleManager.h"
#include "DesignModule.h"
#include "GController.h"
#include "TSuitManager.h"
#include "LogicDesignScene.h"
#include "SuitT.h"




void UGStaticFunction::GetCameraPhotoCover(float & _cameraDistance, FVector & _cameraOffset, float & _rotateAngle, int32 & _isDisplayModel)
{
	DesignModule * mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

	SuitT * mSuitT = TSuitManager::GetInstance()->GetTemplateByPID<SuitT>(mDesignModule->GetDesignData()->m_designTID);

	_cameraDistance = mSuitT->m_cameraPhotoCover.m_cameraDistance;

	_cameraOffset.Z = mSuitT->m_cameraPhotoCover.m_cameraHeight;

	_rotateAngle = mSuitT->m_cameraPhotoCover.m_cameraPitching;

	_isDisplayModel = mSuitT->m_cameraPhotoCover.m_isDisplayModel;
}

void UGStaticFunction::GetCameraDesignCover(float & _cameraDistance, FVector & _cameraOffset, float & _rotateAroundYawDegree,float & _RotateSelfPitchDegree)
{
	DesignModule * mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

	SuitT * mSuitT = TSuitManager::GetInstance()->GetTemplateByPID<SuitT>(mDesignModule->GetDesignData()->m_designTID);

	_cameraDistance = mSuitT->m_cameraDesignCover.m_cameraDistance;

	_cameraOffset.Z = mSuitT->m_cameraDesignCover.m_cameraHeight;

	//左右旋转.
	_rotateAroundYawDegree = mSuitT->m_cameraDesignCover.m_rotateAngle;
	
	//俯仰角.
	_RotateSelfPitchDegree = mSuitT->m_cameraDesignCover.m_cameraPitching;
}

int32 UGStaticFunction::GetGPlatform()
{
	return GController::m_platform;
}

void UGStaticFunction::OnCapture(bool _isShow)
{
	LogicDesignScene *mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();
	mLogicDesignScene->OnCapture(_isShow);
}

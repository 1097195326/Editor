// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenH5Scheduler.h"
GREGISTER(OpenH5Scheduler)
OpenH5Scheduler::OpenH5Scheduler()
{
}

OpenH5Scheduler::~OpenH5Scheduler()
{
}

void OpenH5Scheduler::OnLoad()
{

}

void OpenH5Scheduler::OnRelease()
{

}

void OpenH5Scheduler::Excute(TMap<FString, FString> & _params)
{
	if(_params.Contains(TEXT("url")))
		UHaoshaSDKBPLibrary::OpenWebview(EditorFunctionalTool::GetTicketUrl(_params[TEXT("url")]));

	GRelease();
}
  
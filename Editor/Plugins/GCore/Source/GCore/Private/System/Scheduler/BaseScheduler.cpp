// Fill out your copyright notice in the Description page of Project Settings.

#include "GCore.h"
#include "BaseScheduler.h"

BaseScheduler::BaseScheduler()
{
}

BaseScheduler::~BaseScheduler()
{
}

void BaseScheduler::GLoad()
{
	OnLoad();
}

void BaseScheduler::GRelease()
{
	OnRelease();

	delete this;
}

void BaseScheduler::OnLoad()
{

}

void BaseScheduler::OnRelease()
{

}

void BaseScheduler::Excute(TMap<FString, FString> & _params)
{

}

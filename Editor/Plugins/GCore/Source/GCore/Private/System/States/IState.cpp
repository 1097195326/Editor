// Fill out your copyright notice in the Description page of Project Settings.

#include "GCore.h"
#include "IState.h"

IState::IState()
{
}

IState::~IState()
{
}

void IState::GRelease()
{
	OnRelease();

	delete this;
}

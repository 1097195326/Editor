// Fill out your copyright notice in the Description page of Project Settings.

#include "GCore.h"
#include "BaseModule.h"


BaseModule::BaseModule()
{
    
}

BaseModule::~BaseModule()
{
}

void BaseModule::GLoad()
{
    OnLoad();
}

void BaseModule::GRelease()
{
   OnRelease();

   delete this;
}


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "ILoad.h"
#include "GObject.h"
#include "GCore/Public/AAReflection/AARefClasses.h"
/**
 * 
 */
class GCORE_API BaseModule : public ILoad, public GObject
{

public:
    BaseModule();
	virtual ~BaseModule();
    
    // 子类不可以重写.
    void GLoad() final;
    void GRelease() final;
    
    
    
protected:
  
    virtual void OnLoad()=0;
    virtual void OnRelease()=0;
};

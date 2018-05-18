// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class GCORE_API ILoad
{
public:
	ILoad();
	virtual ~ILoad();
    
	virtual void GLoad();
    virtual void GRelease();
    
};

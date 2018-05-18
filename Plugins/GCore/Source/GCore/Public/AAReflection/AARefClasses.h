// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "stdio.h"
#include <string>
#include <map>
using namespace std;
typedef void *(*PTRCreateObject)(void);

class GCORE_API AARefClasses
{
public:
	~AARefClasses();
private:
	// 普通C++类.
	map<string, PTRCreateObject> m_ClassMap;

	// 反射Module用的类.
	map<string, PTRCreateObject> m_moduleMap;

	AARefClasses();
	static AARefClasses *m_Instance;
public:
	void *CreateClassByName(string _ClassName);
	
	void RegistClass(string _Name, PTRCreateObject _Method);

	// 注册Module.
	void RegistModuleClass(string _Name, PTRCreateObject _Method);

	// 直接创建所有的Module.
	void LoadAllModule();

	void GRelease();

	static AARefClasses* GetInstance();
};

class RegisterAction
{
public:
	RegisterAction(string _Name, PTRCreateObject _Methor)
	{
		AARefClasses::GetInstance()->RegistClass(_Name, _Methor);
	}
};

class  RegisterModuleAction
{
public:
	RegisterModuleAction(string _Name, PTRCreateObject _Methor)
	{
		AARefClasses::GetInstance()->RegistModuleClass(_Name, _Methor);
	}
};

#define GREGISTER(ClassName)\
	ClassName* Create##ClassName(){ return new ClassName(); }; \
	RegisterAction CreateRegister##ClassName(#ClassName,(PTRCreateObject)Create##ClassName);\

// 注册Module用.
#define GREGISTER_MODULE(ClassName)\
	ClassName* Create##ClassName(){ return new ClassName(); }; \
	RegisterModuleAction CreateRegister##ClassName(#ClassName,(PTRCreateObject)Create##ClassName);\









































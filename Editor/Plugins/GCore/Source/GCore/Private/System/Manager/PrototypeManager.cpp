#include "GCore.h"
#include "PrototypeManager.h"
#include "AARefClasses.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "GCore/Public/System/FIleManager/GPlatformMgr.h"
#include "ControllerPrototype.h"
#include "EncryptDecryptTool.h"
#include "Runtime/Core/Public/Misc/Base64.h"
#include "GFileHelper.h"


PrototypeManager* PrototypeManager::m_pInstance = nullptr;//初始化在主线程之前.

PrototypeManager::PrototypeManager()
{
}

PrototypeManager::~PrototypeManager()
{
}

PrototypeManager * PrototypeManager::GetInstance()
{
	if (m_pInstance == nullptr)  //判断是否第一次调用.
	{
		m_pInstance = new PrototypeManager();
	}
	return m_pInstance;
}

//-- 通过pid获取prototype对象.
BasePrototype* PrototypeManager::GetPrototypeByPID(int32 _pid)
{
	XY_IF_FOUND_IN_MAP(_pid, it, m_protypeMap, PrototypePIDMap)
	{
		BasePrototype* mPrototype = it->second;

		return mPrototype;
	} 
	return nullptr;
}

// 加载所有系统设置.
void PrototypeManager::LoadAllSystemPrototype()
{
	FString mContentDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir());

	FString mLocalResourcePath = FString::Printf(TEXT("%s%s"), *mContentDir, TEXT("Resource/Config/"));

	LoadPrototype(FString::Printf(TEXT("%s%s"), *mLocalResourcePath, TEXT("SchedulerMappingPrototype.xml")));

	//-- 1.先加载ControllerPrototype.
	if (!LoadPrototype(FString::Printf(TEXT("%s%s"), *mLocalResourcePath, TEXT("ControllerPrototype.xml"))))
	{
		return;
	}

	// 必须要加载.
	LoadPrototype(FString::Printf(TEXT("%s%s"), *mLocalResourcePath, TEXT("UIPrototype.xml")));

	LoadPrototype(FString::Printf(TEXT("%s%s"), *mLocalResourcePath, TEXT("ScenePrototype.xml")));

	//-- 2.根据ControllerPrototype加载不同的配置文件.
	ControllerPrototype * mController = (ControllerPrototype*)GetPrototypeByPID(PrototypeConst::P_CONTROLLER_ID);


	if (mController)   
	{
		if (mController->IsDebug())
		{
			// debug 模式需要加载的配置文件.
			LoadPrototype(FString::Printf(TEXT("%s%s"), *mLocalResourcePath, TEXT("ServerDebugPrototype.xml")));
		}
		else       
		{
			// shipping 模式需要加载的配置文件.
			LoadPrototype(FString::Printf(TEXT("%s%s"), *mLocalResourcePath, TEXT("ServerShippingPrototype.xml")));
		}
		if (mController->GetPlatform() == ENUM_PLATFORM::GWINDOWS || mController->GetPlatform() == ENUM_PLATFORM::GMAC)
		{
			// windows 或 Mac平台下.
			LoadPrototype(FString::Printf(TEXT("%s%s"), *mLocalResourcePath, TEXT("WindowsUIPrototype.xml")));
		}
		else
		{
			// 移动平台.
			LoadPrototype(FString::Printf(TEXT("%s%s"), *mLocalResourcePath, TEXT("MobileUIPrototype.xml")));
		}
		if (mController->GetLanguage() == ENUM_LANGUAGE::GCHINESE)
		{
			// 
			LoadPrototype(FString::Printf(TEXT("%s%s"), *mLocalResourcePath, TEXT("ChinesePrototype.xml")));
		}
		else
		{
			LoadPrototype(FString::Printf(TEXT("%s%s"), *mLocalResourcePath, TEXT("EnglishPrototype.xml")));
		}
	}

	// 加载http映射表.
	LoadPrototype(FString::Printf(TEXT("%s%s"), *mLocalResourcePath, TEXT("HttpMappingPrototype.xml")));
}

bool PrototypeManager::LoadPrototype(const FString & _xmlPath)
{
	TiXmlDocument doc;

	FString mStrContent;

	if (!FFileHelper::LoadFileToString(mStrContent, *_xmlPath))
	{
		return false;
	}

	// 加密.
	if (!_xmlPath.Contains(TEXT("ChinesePrototype.xml")) && !_xmlPath.Contains(TEXT("EnglishPrototype.xml")))
	{
		FString mEncryptStr = EncryptAndDecryptTool::Decrypt(mStrContent, EncryptKey);
		FBase64::Decode(mEncryptStr, mStrContent);
	}

	FString mContent = mStrContent;

	std::string stdContent = TCHAR_TO_UTF8(*mContent);

	const char * xmlContent = stdContent.c_str();

	doc.Parse(xmlContent);

	TiXmlElement* root = doc.FirstChildElement();

	if (root == NULL)
	{
		doc.Clear();

		return false;
	}

	const char * nodeType = root->Attribute("type");

	for (TiXmlElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
	{
		BasePrototype * mPrototype;

		const char * pid = elem->Attribute("pid");

		//
		std::map<int32, BasePrototype*>::iterator it = m_protypeMap.find(atoi(pid));

		if (it != m_protypeMap.end())
		{
			mPrototype = it->second;

			mPrototype->LoadXMLData(elem);
		}
		else
		{
			mPrototype = (BasePrototype*)(AARefClasses::GetInstance()->CreateClassByName(nodeType));

			mPrototype->LoadXMLData(elem);

			m_protypeMap.insert(std::make_pair(atoi(pid), mPrototype));
		}
	}

	doc.Clear();

	return true;
}

void PrototypeManager::GLoad()
{

}

void PrototypeManager::GRelease()
{
	for (std::map<int32, BasePrototype*>::reverse_iterator rit = m_protypeMap.rbegin(); rit != m_protypeMap.rend(); rit++)
	{
		BasePrototype* prototype = rit->second;

		prototype->GRelease();
	}

	m_protypeMap.clear();

	PrototypeManager::m_pInstance = nullptr;

	delete this;
}


#include "DesignType.h"


DesignType::DesignType()
{
}
DesignType::~DesignType()
{
}

void DesignType::OnLoad()
{

}

void DesignType::OnRelease()
{

}

void DesignType::OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj)
{
	m_Image = _jsonObj->GetStringField(TEXT("image"));
	m_ParentId = _jsonObj->GetIntegerField(TEXT("parentId"));
	m_Path = _jsonObj->GetStringField(TEXT("path"));
	m_Sort = _jsonObj->GetIntegerField(TEXT("sort"));
	m_Type = _jsonObj->GetIntegerField(TEXT("type"));
}
FString DesignType::GetName()
{
	return m_name;
}
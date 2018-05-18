
#include "HomeInfoT.h"


HomeInfoT::HomeInfoT()
{
}
HomeInfoT::~HomeInfoT()
{
}

void HomeInfoT::OnLoad()
{

}

void HomeInfoT::OnRelease()
{

}

void HomeInfoT::OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj)
{
	m_modelContentName = _jsonObj->GetStringField(TEXT("modelContentName"));
	m_contentId = _jsonObj->GetStringField(TEXT("appConnectUrl"));
	m_modelId = _jsonObj->GetIntegerField(TEXT("modelId"));
	m_createUserCode = _jsonObj->GetIntegerField(TEXT("createUserCode"));
	m_updateUserName = _jsonObj->GetStringField(TEXT("updateUserName"));
	//m_contentId = _jsonObj->GetStringField(TEXT("contentId"));
	m_contentText = _jsonObj->GetStringField(TEXT("contentText"));
	m_modelContentCode = _jsonObj->GetStringField(TEXT("modelContentCode"));
	m_createUserName = _jsonObj->GetStringField(TEXT("createUserName"));
	m_updateTime = _jsonObj->GetIntegerField(TEXT("updateTime"));
	m_modelContentId = _jsonObj->GetIntegerField(TEXT("modelContentId"));
	m_type = _jsonObj->GetStringField(TEXT("type"));
	m_contentImg = _jsonObj->GetStringField(TEXT("contentImg"));
	m_contentUrl = _jsonObj->GetStringField(TEXT("contentUrl"));
	m_updateUserCode = _jsonObj->GetIntegerField(TEXT("updateUserCode"));
	m_createTime = _jsonObj->GetIntegerField(TEXT("createTime"));
	m_sorder = _jsonObj->GetIntegerField(TEXT("sorder"));

}
FString HomeInfoT::GetName()
{
	return m_name;
}
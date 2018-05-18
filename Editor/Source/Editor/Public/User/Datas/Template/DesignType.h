#pragma once


#include "CoreMinimal.h"
#include "BaseT.h"
/**
*
*/
class EDITOR_API DesignType :public BaseT
{
public:

	FString			m_Image;
	int32			m_ParentId;
	FString			m_Path;
	int8			m_Sort;
	int8			m_Type;

public:

	DesignType();

	virtual~DesignType();

public:
	FString GetName();
	virtual void OnLoad() override;

	virtual void OnRelease() override;

protected:
	virtual void OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj);

};

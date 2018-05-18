#pragma once

#include "CoreMinimal.h"
#include "GBaseTemplateManager.h"

#include "DesignType.h"

/**
*
*/
class EDITOR_API TDesignTypeManager :public GBaseTemplateManager
{
private:
	// 单利. 
	static TDesignTypeManager *   m_pInstance;

	TArray<DesignType*>			m_AllTypesArr;
public:
	static TDesignTypeManager * GetInstance();

public:
	TDesignTypeManager();

	virtual ~TDesignTypeManager();

public:
	TArray<DesignType*>  GetAllTypes() { return m_AllTypesArr; }

	DesignType * GetDesignTypeById(int32 _id);
	TArray<DesignType *> GetAllDesignTypesByParentId(int32 _parentId);

	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void EncodeData(TSharedPtr<FJsonObject> & _content) override;

	// 通过SuitTid获取父级分类ID.
	int32 GetParentCatIdBySuitTid(int32 _suitTid);

	int32 GetCatIdBySuitTid(int32 _suitTid);
	void AddTypeData(DesignType * _designType);
};
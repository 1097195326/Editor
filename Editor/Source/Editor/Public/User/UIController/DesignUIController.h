
#pragma once

#include "CoreMinimal.h"
#include "SharedPage.h"
#include "DesignUMG.h"
#include "UIController.h"

enum class Change_PAGE :uint8
{
	SHARED_PAGE = 0,
};

/**
*
*/
class EDITOR_API DesignUIController :public UIController
{
public:
	DesignUIController();
	virtual~DesignUIController();

public:
	void ChangePageTo(Change_PAGE _type);

protected: // 以下类必须重写.

	virtual void OnLoad() override;

	virtual void OnRelease() override;

	// rootview进入后台=隐藏UI.
	virtual void OnBackstageUI() override;

	// rootView 进入激活状态.
	virtual void OnActivationUI() override;
	
public:
	//初始化摄像机.
	void InitMainCamera();
	
private:

	UBaseUI * m_SharedPage;	//	分享UI.
};

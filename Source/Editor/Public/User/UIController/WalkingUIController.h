#pragma once


#include "CoreMinimal.h"
#include "UIController.h"
/**
*
*/

class EDITOR_API WalkingUIController :public UIController
{
public:
	WalkingUIController();

	virtual ~WalkingUIController();

protected: // 以下类必须重写.

	virtual void OnLoad() override;

	virtual void OnRelease() override;

	// rootview进入后台=隐藏UI.
	virtual void OnBackstageUI() override;

	// rootView 进入激活状态.
	virtual void OnActivationUI() override;


};
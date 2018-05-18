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

protected: // �����������д.

	virtual void OnLoad() override;

	virtual void OnRelease() override;

	// rootview�����̨=����UI.
	virtual void OnBackstageUI() override;

	// rootView ���뼤��״̬.
	virtual void OnActivationUI() override;


};
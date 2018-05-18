// Fill out your copyright notice in the Description page of Project Settings.

#include "GCore.h"
#include "ButtonScript.h"


UButtonScript * UButtonScript::Create()
{
	UButtonScript * mButtonScript = NewObject<UButtonScript>();
	mButtonScript->AddToRoot();
	return mButtonScript;
}


UButtonScript::~UButtonScript()
{
}
void UButtonScript::GLoad()
{

}

void UButtonScript::BindBtnEvent(BtnEnum _btnEnum, int32 _pid, UButton * _btn)
{
	m_pid = _pid;

	m_btn = _btn;

	switch (_btnEnum) {

	case BtnEnum::CLICKED:
	{
		m_btn->OnClicked.AddDynamic(this, &UButtonScript::ButtonClicked);
		break;
	}

	case BtnEnum::HOVER:
	{
		m_btn->OnClicked.AddDynamic(this, &UButtonScript::ButtonClicked);
		break;
	}

	default:
		break;
	}
}

// 按钮点击事件.
void UButtonScript::ButtonClicked()
{
	if (m_pid != 0)
	{
		/*BaseTask * mTask = GTaskManager::GetInstance()->GetTaskByPID<BaseTask>(m_pid);

		if (mTask)
		{
			mTask->Excute();
		}*/
		// 直接发送消息.
		button_event_function(m_pid);
	}
}

void UButtonScript::GRelease()
{
	this->RemoveFromRoot();
}
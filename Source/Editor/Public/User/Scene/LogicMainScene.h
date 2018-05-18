#pragma once
#include "CoreMinimal.h"
#include "MsgCenter.h"
#include "DesignModule.h"
#include "DesignModule.h"
#include "BaseLogicScene.h"

enum EnumCheckDesignState
{
	WAITING = 0,
	CHECK_R_DATA, // 检查R数据.
	CHECK_PART_T_DATA, // 检查T部位数据.
	CHECK_COMPONENT_T_DATA,//检查T 部件数据.
	CHECK_SLICE_T_DATA, //检查T部片数据.
	CHECK_ACCESSORIES_T_DATA,//检查 T辅料数据.
	CHECK_STITCH_T_DATA,// 检查T线迹数据.
	CHECK_SUIT_RESOURCE,// 检查套件模型资源.
	CHECK_FINISH,	//可以进入设计场景了.
	CHECK_ERROR		// 尝试3次以后仍不能加载只能退出.
};

class EDITOR_API LogicMainScene :public BaseLogicScene
{

private:
	
	EnumCheckDesignState m_checkDesignState = EnumCheckDesignState::WAITING;


	// 验证失败重试次数.
	int32 m_AllrepeatCount = 3;

	// 当前尝试的次数.
	int32 m_currentRepeatCount = 1;

public:
	
	LogicMainScene();

	virtual ~LogicMainScene();

protected:

	virtual void OnLoad() override;

	virtual void OnRelease() override;

public:

	// 准备开始进入设计场景.
	void BeginJoinDesignLevel(int32 _designRID, int32 _desginTID, EnumDesignType _designType);

private:// 进入设计关卡之前的验证方法.
	 
	// 状态判断.
	void DesignState(EnumCheckDesignState _checkDesignState);

	void JoinDesignLevel();

	// 检查R数据.
	void CheckRData();

	// 检查T数据.
	void CheckPartTData();

	void CheckComponentTData();

	void CheckSliceTData();

	void CheckAccessoriesTData();

	void CheckStitchTData();

	// 检查模型资源.
	void CheckSuitResource();

	// 尝试.
	void CheckRepeat();

	// 加载失败的处理.
	void OnCheckError();

private:// 收到加载完成回调的函数区.

	
	// 收到加载完毕款详情的回调.
	void OnLoadStyleDetailFinish(msg_ptr _msg);

	// 收到部位加载完毕的回调.
	void OnLoadPartFinish(msg_ptr _msg);

	// 收到部件加载完毕的回调.
	void OnLoadComponentFinish(msg_ptr _msg);

	// 收到片加载完毕的回调.
	void OnLoadSliceFinish(msg_ptr _msg);

	// 收到辅料加载完毕的回调.
	void OnLoadAccessoriesFinish(msg_ptr _msg);

	// 收到线迹加载完毕的回调.
	void OnLoadStitchFinish(msg_ptr _msg);

	// 热更新完成.
	void OnLoadSuitResourceFinish(msg_ptr _msg);

	// T资源加载完毕.
	void OnLoadTDataFinish(msg_ptr _msg);

public:
	// 数据加载提示用户点确认事件.
	void OnErrorLoadDataClickBtnTrue();
};
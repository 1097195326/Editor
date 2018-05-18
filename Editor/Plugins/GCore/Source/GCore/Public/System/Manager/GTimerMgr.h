// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Object.h"
#include "castle_std.h"
#include <functional>
#include "IUpdate.h"
#include "Containers/Ticker.h"

/**
 * 任务.
 */
typedef std::function<bool  (float _delay)> timer_run_function;

class GCORE_API TimerTask
{

private:
    float                               m_currentTime = 0.f;

public:

    /** 前两个参数是函数指针.
    * _delayTime : 延迟执行时间.
    * _isRepeat  : 是否重复.
    **/
    template <typename T>
	TimerTask(T *objectPointer,bool(T::*funcRunPointer)(float),float _delayTime=0,bool _isRepeat=false)
    {
        b_isRepeat = _isRepeat;
        
        m_delayTime = _delayTime;
        
        m_run_func = timer_run_function( std::bind(funcRunPointer, objectPointer,std::placeholders::_1) );
    }
    
	~TimerTask();
    
    // 函数指针.
    timer_run_function                      m_run_func;
    
    // 延迟时间.
    float                                   m_delayTime=0.f;
    
    // 是否重复.
    bool                                    b_isRepeat = false;
    
    bool                                    b_isFinish = false;
    
public:

    // 执行.
    bool Excute(float _delay);
    
};


typedef std::shared_ptr<TimerTask>		timer_task_ptr;

typedef	std::map<int32, timer_task_ptr> task_map;

typedef std::vector<IUpdate*>           update_handler_list;

/**
 * 游戏时间管理器.
 */
class GCORE_API GTimerMgr
{

private:

	// 单利.
	static GTimerMgr *						m_pInstance;

    //timmer执行队列.
    task_map                                m_Queue;
    
    // 任务编号.
    int32                                   m_taskID = 0;

	FTickerDelegate							m_tickDelegate;

	FDelegateHandle							m_tickDelegateHandle;

private:
	// receive network update.
	update_handler_list         m_receives_handler_list;

	// service update.
	update_handler_list         m_service_handler_list;

	// game update.
	update_handler_list         m_game_handler_list;

public:
	GTimerMgr();
	~GTimerMgr();
    
	// 单利. 
	static GTimerMgr * GetInstance();

public:

	void GLoad();
	
	void GRelease();

    bool Update(float _delay);
    
    // 添加任务.
    int32 RegisterTimer(timer_task_ptr _timerTask);
    
    // 移除任务.
    void RemoveTimer(int32 _taskID);

protected:
	void ClearHandler();

private:
	void UpdateReceive(float DeltaTime);

	void UpdateService(float DeltaTime);

	void UpdateGame(float DeltaTime);

public:

	void RegisterReceiveUpdateHandler(IUpdate * _handler);

	void RegisterServiceUpdateHandler(IUpdate * _handler);

	void RegisterGameUpdateHandler(IUpdate * _handler);

	void RemoveReceiveUpdateHandler(IUpdate * _handler);

	void RemoveServiceUpdateHandler(IUpdate * _handler);

	void RemoveGameUpdateHandler(IUpdate * _handler);

	// 由director 调用此函数，整个游戏的循环都是由director一个人来触发.
	void UpdateMe(float DeltaTime);
};

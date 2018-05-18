// Fill out your copyright notice in the Description page of Project Settings.

#include "GCore.h"
#include "GTimerMgr.h"

GTimerMgr* GTimerMgr::m_pInstance = nullptr;//初始化在主线程之前. 

TimerTask::~TimerTask()
{

}

bool TimerTask::Excute(float _delay)
{
    // 判断时间.
    if(b_isFinish)
    {
        return true;
    }
 
    m_currentTime = m_currentTime +_delay;
    
    bool isFinish = false;
    
    // 判断时间.
    if(m_currentTime>=m_delayTime)
    {
        m_currentTime = 0.f;
        
        // 执行函数返回true 就直接退出了.
        _delay = m_delayTime + _delay;
        
        isFinish = m_run_func(_delay);
        
        //  这里判断只执行一次.
        isFinish = b_isRepeat?isFinish:true;

		m_currentTime = 0.0f;
    }
    return isFinish;
}

GTimerMgr::GTimerMgr()
{
    m_taskID = 0;
}

GTimerMgr::~GTimerMgr()
{
    // 遍历删除素有线程.
    m_Queue.clear();
}

GTimerMgr * GTimerMgr::GetInstance()
{
	if (m_pInstance == nullptr)  //判断是否第一次调用.
	{
		m_pInstance = new GTimerMgr();

	}
	return m_pInstance;
}

void GTimerMgr::GLoad()
{
	// 开启Update.
	m_tickDelegate = FTickerDelegate::CreateRaw(this, &GTimerMgr::Update);
	m_tickDelegateHandle = FTicker::GetCoreTicker().AddTicker(m_tickDelegate);
}

void GTimerMgr::GRelease()
{
	ClearHandler();
	FTicker::GetCoreTicker().RemoveTicker(m_tickDelegateHandle);
	m_Queue.clear();
	delete m_pInstance;
	m_pInstance = nullptr;
}


// 负责回调.
bool GTimerMgr::Update(float _delay)
{
    // 遍历删除素有线程.
    std::map<int32, timer_task_ptr>::iterator it = m_Queue.begin();
    while(it != m_Queue.end())
    {
        timer_task_ptr task = it->second;
        
        if(nullptr != task && task->Excute(_delay))
        {
            it = m_Queue.erase(it);
        }
        else
        {
            it++;
        }
    }

	UpdateMe(_delay);

	return true;
}

// 添加任务.
int32 GTimerMgr::RegisterTimer(timer_task_ptr _timerTask)
{
    m_taskID += 1;
    
    XY_MAP_INSERT(m_taskID,_timerTask,m_Queue);
    
    return m_taskID;
}

void GTimerMgr::RemoveTimer(int32 _taskID)
{
    XY_IF_FOUND_IN_MAP(m_taskID,it,m_Queue,task_map)
    {
        
        XY_MAP_ERASE(m_taskID, m_Queue);
    }
}

void GTimerMgr::ClearHandler()
{
	m_receives_handler_list.clear();

	std::vector<IUpdate*>().swap(m_receives_handler_list);

	m_service_handler_list.clear();

	std::vector<IUpdate*>().swap(m_service_handler_list);

	m_game_handler_list.clear();

	std::vector<IUpdate*>().swap(m_game_handler_list);
}

void GTimerMgr::RegisterReceiveUpdateHandler(IUpdate * _handler)
{
	m_receives_handler_list.push_back(_handler);
}

void GTimerMgr::RegisterServiceUpdateHandler(IUpdate * _handler)
{
	m_service_handler_list.push_back(_handler);
}

void GTimerMgr::RegisterGameUpdateHandler(IUpdate * _handler)
{
	m_game_handler_list.push_back(_handler);
}


void GTimerMgr::RemoveReceiveUpdateHandler(IUpdate * _handler)
{
	if (m_receives_handler_list.empty())
		return;

	auto iter = m_receives_handler_list.begin();

	while (iter != m_receives_handler_list.end())
	{
		IUpdate* mhandler = *iter;

		if (mhandler == _handler)
		{
			iter = m_receives_handler_list.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void GTimerMgr::RemoveServiceUpdateHandler(IUpdate * _handler)
{
	if (m_service_handler_list.empty())
		return;

	auto iter = m_service_handler_list.begin();

	while (iter != m_service_handler_list.end())
	{
		IUpdate* mhandler = *iter;

		if (mhandler == _handler)
		{
			iter = m_service_handler_list.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void GTimerMgr::RemoveGameUpdateHandler(IUpdate * _handler)
{
	if (m_game_handler_list.empty() && m_game_handler_list.size() > 0)
		return;

	auto iter = m_game_handler_list.begin();

	while (iter != m_game_handler_list.end())
	{
		IUpdate* mhandler = *iter;

		if (mhandler == _handler)
		{
			iter = m_game_handler_list.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}


void GTimerMgr::UpdateReceive(float DeltaTime)
{
	if (m_receives_handler_list.empty() && m_receives_handler_list.size() > 0)
		return;

	auto iter = m_receives_handler_list.begin();

	while (iter != m_receives_handler_list.end())
	{
		IUpdate* mhandler = *iter;

		if (mhandler == nullptr)
		{
			iter = m_receives_handler_list.erase(iter);
		}
		else
		{
			mhandler->OnUpdateReceived(DeltaTime);
			++iter;
		}
	}
}

void GTimerMgr::UpdateService(float DeltaTime)
{
	if (m_service_handler_list.empty() && m_service_handler_list.size() > 0)
		return;

	auto iter = m_service_handler_list.begin();

	while (iter != m_service_handler_list.end())
	{
		IUpdate* mhandler = *iter;

		if (mhandler == nullptr)
		{
			iter = m_service_handler_list.erase(iter);
		}
		else
		{
			mhandler->OnUpdateService(DeltaTime);
			++iter;
		}
	}
}

void GTimerMgr::UpdateGame(float DeltaTime)
{
	if (m_game_handler_list.empty() && m_game_handler_list.size() > 0)
		return;

	auto iter = m_game_handler_list.begin();

	while (iter != m_game_handler_list.end())
	{
		IUpdate* mhandler = *iter;

		if (mhandler == nullptr)
		{
			iter = m_game_handler_list.erase(iter);
		}
		else
		{
			mhandler->OnUpdate(DeltaTime);
			++iter;
		}
	}
}


// 由director 调用此函数，整个游戏的循环都是由director一个人来触发.
void GTimerMgr::UpdateMe(float DeltaTime)
{
	UpdateReceive(DeltaTime);
	UpdateService(DeltaTime);
	UpdateGame(DeltaTime);
}
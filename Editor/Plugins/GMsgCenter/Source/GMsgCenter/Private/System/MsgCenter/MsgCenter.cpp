// Fill out your copyright notice in the Description page of Project Settings.

#include "GMsgCenter.h"
#include "MsgCenter.h"
#include "LocalChannel.h"
#include "GTimerMgr.h"



MsgCenter* MsgCenter:: m_pInstance  = nullptr;//初始化在主线程之前.

MsgCenter * MsgCenter::Instance()
{
    if(m_pInstance == nullptr)  //判断是否第一次调用.
        m_pInstance = new MsgCenter();
    return m_pInstance;  
}

void MsgCenter::GLoad()
{
	// 启动定时器.
	timer_task_ptr task(new TimerTask(this, &MsgCenter::OnUpdateReceived, 0.0f, true));

	m_updateID = GTimerMgr::GetInstance()->RegisterTimer(task);
}

void MsgCenter::GRelease()
{
    for(std::map<uint32, MsgChannel*>::reverse_iterator rit=map_channels.rbegin();rit!=map_channels.rend();rit++)
    {
        if(nullptr != rit->second)
            delete (MsgChannel*)(rit->second);
    }
    map_channels.clear();

	GTimerMgr::GetInstance()->RemoveTimer(m_updateID);
    
    m_pInstance = nullptr;
    
    delete this;
}

MsgCenter::MsgCenter()
{
    //内部消息.
    MsgChannel * channel = new LocalChannel((uint32)MessageTypeLocal);
    XY_MAP_INSERT((uint32)MessageTypeLocal,channel,map_channels);
	
}

MsgCenter::~MsgCenter()
{
    
}

/*
 *  消息转发.
 */
void MsgCenter::SendMsg(msg_ptr msg)
{
    uint32 mChannelID = msg->ChannelID();
    
    XY_IF_FOUND_IN_MAP(mChannelID,it,map_channels,MapMsgChannel)
    {
        MsgChannel * channel = it->second;
        
        channel->SendMsg(msg);
    }
}

/*
 *  检查消息频道是否存在.
 */
MsgChannel * MsgCenter::GetChannel(uint32 _channelID)
{
    XY_IF_FOUND_IN_MAP(_channelID,it,map_channels,MapMsgChannel)
    {
        MsgChannel * channel = it->second;
        
        return channel;
    }
    
    return nullptr;
}

/*
* 需要在子线程里直接处理的消息id列表(临时起名字叫白名单)..
*/
void MsgCenter::RegisterWhiteMsgID(int32 _msgID)
{
	m_vector_whiteMsgIds.push_back(_msgID);
}

// 检查消息ID是否白名单里的.
bool MsgCenter::CheckMsgIdInWhiteVector(int32 _msgID)
{
	 auto iter = m_vector_whiteMsgIds.begin();

	 while (iter != m_vector_whiteMsgIds.end())
	 {
		 if (_msgID == *iter)
		 {
			 return true;
		 }
		 ++iter;
	 }
	 return false;
}

/* 
 *  移除消息频道.
 */
void MsgCenter::EraseChannel(uint32 _channelID)
{
    XY_IF_FOUND_IN_MAP(_channelID,it,map_channels,MapMsgChannel)
    {
        XY_MAP_ERASE(_channelID, map_channels);
    }
}

/*
 *  添加一个消息频道 动态创建频道只能是本地消息.   
 */
void MsgCenter::AddLocalChannel(uint32 _channelID)
{
    XY_IF_FOUND_IN_MAP(_channelID,it,map_channels,MapMsgChannel)
    {
        
    }
    else
    {
        LocalChannel * channel = new LocalChannel(_channelID);
        
        XY_MAP_INSERT(_channelID, channel, map_channels);
    }
}


/*
* 注册自定义channel渠道.
*/
void MsgCenter::AddChannel(uint32 _channelID, MsgChannel * _channel)
{
	XY_IF_FOUND_IN_MAP(_channelID, it, map_channels, MapMsgChannel)
	{

	}
	else
	{
		XY_MAP_INSERT(_channelID, _channel, map_channels);
	}
}

bool MsgCenter::OnUpdateReceived(float DeltaTime)
{
	if (!m_received_queue.empty() && m_received_queue.size() > 0)
	{
		// 遍历vector 发送 .
		m_msg_received_mutex.lock();
		auto iter = m_received_queue.begin();

		while (iter != m_received_queue.end())
		{
			msg_ptr  msg = *iter;

			// 直接发送就行了.
			SendMsg(msg);

			++iter;
		}

		//  只需要释放数据不需要连内存都释放.

		m_received_queue.clear();
		m_msg_received_mutex.unlock();
	}
	return false;
}

void MsgCenter::AddReceiveMsg(msg_ptr _receiveMsg)
{
	// 判断消息是否放在主线程队列里处理.
	if (CheckMsgIdInWhiteVector(_receiveMsg->MsgID()))
	{
		SendMsg(_receiveMsg);
	}
	else
	{
		// 其他的都等主线程过来拿数据并发送.
		m_msg_received_mutex.lock();
		m_received_queue.push_back(_receiveMsg);
		m_msg_received_mutex.unlock();
	}
}






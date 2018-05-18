// Fill out your copyright notice in the Description page of Project Settings.

#include "GMsgCenter.h"
#include "MsgChannel.h"



MsgChannel::MsgChannel(uint32 _channelID)
{
    m_channelID = _channelID;
}

MsgChannel::~MsgChannel()
{
    for(std::map<int32, MsgFuncVector*>::reverse_iterator rit=map_handlers.rbegin();rit!=map_handlers.rend();rit++)
    {
        MsgFuncVector * mVector = rit->second;
        
        auto iter = mVector->begin();
        
        while (iter != mVector->end())
        {
            msg_function mHandler = *iter;
            
            iter = mVector->erase(iter);
            
        }
        
        mVector->clear();
        
        std::vector<msg_function>().swap(*mVector);
        
        delete mVector;
    }
    
    map_handlers.clear();
}

bool MsgChannel::RemoveObjectByMsgID(uint32 _msgID, void * obj)
{
	auto rit = map_objects.begin();

	// 这里有个坑.一个msgid只能对应一个对象的函数指针.
	bool ck = false;

	// 遍历过程中删除.
	while (rit != map_objects.end())
	{
		if (rit->first == _msgID)
		{
			MapObjects* mMap = rit->second;

			auto iter = mMap->begin();

			while (iter != mMap->end())
			{
				// 判断对象是否一样.
				if (obj == iter->second)
				{
					// 从map中移除.
					mMap->erase(iter++);

					ck = true;
				}
				else
				{
					++iter;
				}
			}
		}
		++rit;
	}
	return ck;
}

/*
 *  发送消息。子类去实现的.
 */
void MsgChannel::SendMsg(msg_ptr msg)
{
    // 子类实现。这个类不管 .

}

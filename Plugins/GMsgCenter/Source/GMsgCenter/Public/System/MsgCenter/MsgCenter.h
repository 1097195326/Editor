// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "MsgChannel.h"
#include "LocalChannel.h"
#include <mutex>
#include "castle_std.h"

//** include 一些常用的头文件.
#include "MsgLocal.h"
#include "MsgHttp.h"
#include "MsgSocket.h"

/**
 *      消息系统 光雄 2016 11.17.
 */

enum EnumMessageType:int32
{
    MessageTypeLocal = 0,
    MessageTypeHttpRequest,
    MessageTypeHttpResponse,
};

// 消息频道map定义.
typedef	std::map<uint32, MsgChannel*>                MapMsgChannel;


class GMSGCENTER_API MsgCenter
{
private:
    // 单利.
    static MsgCenter * m_pInstance;

	// 接受msg队列.
	vector<msg_ptr>                      m_received_queue;

	// 接受msg消息包队列用的互斥锁.
	mutex                                m_msg_received_mutex;

	// 需要在子线程里直接处理的消息id列表(临时起名字叫白名单).
	vector<int32>                        m_vector_whiteMsgIds;

	// updateid，为了取消驱动保存变量.
	int32								 m_updateID = 0;

protected:
    // map存储的 key=channelid，value=存储的channel对象.
    MapMsgChannel map_channels;

public:
    
	MsgCenter();
	~MsgCenter();
    

    void GLoad();
    void GRelease();
    
    // 单利.
    static MsgCenter * Instance();

public:
    /*
     *  消息转发.
     */ 
    void SendMsg(msg_ptr msg);
    
    /*
     *  检查消息频道是否存在.
     */
    MsgChannel * GetChannel(uint32 _channelID);
    
    /*
     *  移除消息频道.
     */
    void EraseChannel(uint32 _channelID);
    
    /*
     *  添加一个本地消息频道. 
     */
    void AddLocalChannel(uint32 _channelID);

	/*
	* 注册自定义channel渠道.
	*/
	void AddChannel(uint32 _channelID,MsgChannel * _channel);

	/*
	*  网络消息队列Update线程同步..
	*/
	bool OnUpdateReceived(float DeltaTime);

	/*
	* 从网络接受的消息放到队列里.
	*/
	void AddReceiveMsg(msg_ptr _msg); 

	
	/*
	* 需要在子线程里直接处理的消息id列表(临时起名字叫白名单)..
	*/
	void RegisterWhiteMsgID(int32 _msgID);



private:

	// 检查消息ID是否白名单里的.
	bool CheckMsgIdInWhiteVector(int32 _msgID);

	
public:

	// 注册消息.
	template<typename T>
	//_1:msg体.
	void RegisterMsgHandler(uint32 _channelID, uint32 _msgID, T* obj, void (T::*_func)(msg_ptr msg))
	{
		XY_IF_FOUND_IN_MAP(_channelID, it, map_channels, MapMsgChannel)
		{
			MsgChannel * channel = it->second;

			channel->RegisterMsgHandler(_msgID, obj, _func);
		}
		else
		{
			LocalChannel * channel = new LocalChannel(_channelID);

			channel->RegisterMsgHandler(_msgID, obj, _func);

			XY_MAP_INSERT(_channelID, channel, map_channels);
		}
	}

	// 移除消息.
	template<typename T>
	//_1:msg体.
	void RemoveMsgHandler(uint32 _channelID, uint32 _msgID, T* obj, void (T::*_func)(msg_ptr msg))
	{
		XY_IF_FOUND_IN_MAP(_channelID, it, map_channels, MapMsgChannel)
		{
			MsgChannel * channel = it->second;

			channel->RemoveMsgHandler(_msgID, obj, _func);
		}
	}
};

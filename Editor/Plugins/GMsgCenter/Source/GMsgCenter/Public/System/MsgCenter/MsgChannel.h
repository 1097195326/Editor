  // Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "MsgBase.h"
#include "castle_std.h"
#include "Object.h"
#include <iostream>
#include <stdint.h>
#include <string>
#include <functional>
typedef unsigned char Byte;




/**
 * 
 */
typedef std::function<void(msg_ptr msg)>          msg_function;

// 函数指针map定义 value 存储的是ue4 里面函数指针对象.
typedef std::vector<msg_function>                   MsgFuncVector;
typedef	std::map<int32, MsgFuncVector*>             MsgFuncMap;

// 保存对象的key.
typedef	std::map<int32, void*>                      MapObjects;

// 保存msgid于mapobj绑定.
typedef	std::map<int32, MapObjects*>                MapMsgIdObject;

class GMSGCENTER_API MsgChannel
{

protected:
    uint32              m_channelID = 0;
    
    int32               m_objectID = 0;
    
    // map存储的 key=msgID，value=存储的channel对象.
    MsgFuncMap          map_handlers;
    
    // map存储   key=m_objectID value=对象地址.
    MapMsgIdObject      map_objects;
    
protected:

    // 保存函数指针对象.
    void SaveObjectByMsgID(uint32 _msgID,void* obj)
    {
        XY_IF_FOUND_IN_MAP(_msgID,it,map_objects,MapMsgIdObject)
        {
            MapObjects * mMap = it->second;
            
            mMap->insert(std::make_pair(++m_objectID,obj));
        }
        else
        { 
            MapObjects *  mMap = new MapObjects();
            
            mMap->insert(std::make_pair(++m_objectID,obj));
            
            map_objects.insert(std::make_pair(_msgID,mMap));
        }
    }
    
    // 删除函数指针对象.
	bool RemoveObjectByMsgID(uint32 _msgID, void * obj);

public:

	MsgChannel(uint32 _channelID);

	virtual ~MsgChannel();
    
    /*
     *  发送消息.
     */
    virtual void SendMsg(msg_ptr msg);
    

    // 注册消息 .
    template<typename T>
    //_1:msg体.
    void RegisterMsgHandler(uint32 _msgID,T* obj, void (T::*_func)(msg_ptr msg))
    {
        XY_IF_FOUND_IN_MAP(_msgID,it,map_handlers,MsgFuncMap)
        {
            MsgFuncVector * mVector = it->second;
            
            msg_function mHandler;
            
            // 绑定.
            mHandler = msg_function( std::bind(_func, obj, std::placeholders::_1) );
            
            // 保存对象编号，为了remove消息时候用的.
            SaveObjectByMsgID(_msgID,obj);
            
            mVector->push_back(mHandler);
        }
        else
        {
            MsgFuncVector *  mVector = new MsgFuncVector();
            
            msg_function mHandler;
            
            // 绑定.
            mHandler = msg_function( std::bind(_func, obj, std::placeholders::_1) );
            
            mVector->push_back(mHandler);
            
            // 保存对象编号，为了remove消息时候用的.
            SaveObjectByMsgID(_msgID,obj);
            
            XY_MAP_INSERT(_msgID,mVector,map_handlers);
        }
    }
    
    // 注销消息.
    template<typename T>
    //_1:msg体. 
    void RemoveMsgHandler(uint32 _msgID,T* obj, void (T::*_func)(msg_ptr msg))
    {
        auto rit = map_handlers.begin();
        
        // 遍历过程中删除.
        while(rit != map_handlers.end())
        {
            if(rit->first == _msgID)
            {
                MsgFuncVector* mVector = rit->second;
                
                auto iter = mVector->begin();
            
                while (iter != mVector->end())
                {
                    msg_function mHandler = *iter;
                    
                    bool ck = RemoveObjectByMsgID(_msgID, obj);
                    
                    if (ck)
                    {
                       
                        iter = mVector->erase(iter);
                        
                        mHandler = nullptr;
                    }
                    else
                    {
                        ++iter;
                    }
                }
                
                if(mVector->size()>0)
                {
                    ++rit;
                }
                else
                {
                    // 从map中删除.
                    map_handlers.erase(rit++);
                    
                    delete mVector;
                }
            }
            else
            {
                ++rit;
            }
        }
    }
    
};

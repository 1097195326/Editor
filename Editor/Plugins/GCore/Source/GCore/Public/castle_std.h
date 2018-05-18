// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <iostream>
#include <stdio.h>
#include <memory>
#include <string>
#include <vector>
#include <map>
using namespace std;

typedef unsigned char                   Byte;

// Socket接受消息缓存区大小.
#define RECEIVE_BUFFER_SIZE 65535

// Socket发送消息缓存区大小.
#define BACKING_BUFFER_SIZE 65535

enum EnumGSocketState
{
	Connecting,         // 连接中.
	Connected,          // 连接已经建立.
	ConnectionFailed,   // 建立连接失败.
	Disconnecting,		// 准备断开连接.
	Disconnected        // 已经断开连接.
};

enum EnumGReciveState
{
	Head,				// 接受头文件.
	Body			    // 接受body.

};

inline int desToByte(unsigned char * bytes,int32 & _offset)
{
     int value;    
    value = (int) ((bytes[_offset] & 0xFF)
            | ((bytes[_offset+1] & 0xFF)<<8)
            | ((bytes[_offset+2] & 0xFF)<<16)
            | ((bytes[_offset+3] & 0xFF)<<24));
    
    _offset = _offset+4;
    return value;
}

//字符串分割函数.
inline std::vector<std::string> Gsplit(std::string str, std::string pattern)
{
	std::string::size_type pos;
	std::vector<std::string> result;
	str += pattern;//扩展字符串以方便操作. 
	int size = str.size();

	for (int i = 0; i < size; i++)
	{
		pos = str.find(pattern, i);
		if (pos < size)
		{
			std::string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}

inline string GBase64Encode(const unsigned char* Data, int DataByte)
{
	//编码表. 
	const char EncodeTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	//返回值.
	string strEncode;
	unsigned char Tmp[4] = { 0 };
	int LineLength = 0;
	for (int i = 0; i < (int)(DataByte / 3); i++)
	{
		Tmp[1] = *Data++;
		Tmp[2] = *Data++;
		Tmp[3] = *Data++;
		strEncode += EncodeTable[Tmp[1] >> 2];
		strEncode += EncodeTable[((Tmp[1] << 4) | (Tmp[2] >> 4)) & 0x3F];
		strEncode += EncodeTable[((Tmp[2] << 2) | (Tmp[3] >> 6)) & 0x3F];
		strEncode += EncodeTable[Tmp[3] & 0x3F];
		if (LineLength += 4, LineLength == 76) { strEncode += "\r\n"; LineLength = 0; }
	}
	//对剩余数据进行编码.
	int Mod = DataByte % 3;
	if (Mod == 1)
	{
		Tmp[1] = *Data++;
		strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
		strEncode += EncodeTable[((Tmp[1] & 0x03) << 4)];
		strEncode += "==";
	}
	else if (Mod == 2)
	{
		Tmp[1] = *Data++;
		Tmp[2] = *Data++;
		strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
		strEncode += EncodeTable[((Tmp[1] & 0x03) << 4) | ((Tmp[2] & 0xF0) >> 4)];
		strEncode += EncodeTable[((Tmp[2] & 0x0F) << 2)];
		strEncode += "=";
	}

	return strEncode;
}

inline string GBase64Decode(const unsigned char* Data, int DataByte, int& OutByte)
{
	//解码表.
	const char DecodeTable[] =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		62, // '+'  
		0, 0, 0,
		63, // '/'  
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'  
		0, 0, 0, 0, 0, 0, 0,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'  
		0, 0, 0, 0, 0, 0,
		26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
		39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'  
	};
	//返回值.
	string strDecode;
	int nValue;
	int i = 0;
	while (i < DataByte)
	{
		if (*Data != '\r' && *Data != '\n')
		{
			nValue = DecodeTable[*Data++] << 18;
			nValue += DecodeTable[*Data++] << 12;
			strDecode += (nValue & 0x00FF0000) >> 16;
			OutByte++;
			if (*Data != '=')
			{
				nValue += DecodeTable[*Data++] << 6;
				strDecode += (nValue & 0x0000FF00) >> 8;
				OutByte++;
				if (*Data != '=')
				{
					nValue += DecodeTable[*Data++];
					strDecode += nValue & 0x000000FF;
					OutByte++;
				}
			}
			i += 4;
		}
		else// 回车换行,跳过  
		{
			Data++;
			i++;
		}
	}
	return strDecode;
}

// 只能指针的string用法 gstring(new std::string("hello async world!"));.
typedef std::shared_ptr<std::string>		g_string;

#define GString(name, value)\
g_string name(new std::string(value));\


	//*MAP智能指针宏.
#define XY_MAP_PTR(indexType,className)\
typedef std::map<indexType, className::ptr>	Map##className##Ptr


	//*MAP宏.
#define XY_MAP_BEGIN(itName,mapName,mapType)\
mapType::iterator itName = mapName.begin()


	//*MAP宏 .
#define XY_MAP_WHILE(mapType,mapName,itName)\
	mapType::iterator itName = mapName.begin();while (itName!=mapName.end())	

#define XY_MAP_FOR(mapType,mapName,itName)\
	mapType::iterator itName = mapName.begin();for(;itName!=mapName.end();++it)	

	//*MAP insert宏 .
#define XY_MAP_INSERT(index,Value,mapName)\
	mapName.insert(std::make_pair(index,Value))

	//*MAP erase宏.
#define XY_MAP_ERASE(index,mapName)\
	mapName.erase(index)

	//*MAP find宏.
#define XY_IF_FOUND_IN_MAP(index,itName,mapName,mapType)\
	mapType::iterator itName;itName=mapName.find(index);if(itName!=mapName.end())


// set get
// PropertyBuilderByName 用于生成类的成员变量.
// 并生成set和get方法.
// type 为变量类型.
// access_permission 为变量的访问权限(public, priavte, protected).

#define PropertyBuilderByName(type, name, access_permission)\
public:\
inline void set_##name(type v) {\
    m_##name = v;\
}\
inline type get_##name() {\
    return m_##name;\
}\
    
// 只有Get方法.
#define PropertyGetByName(type, name, access_permission)\
public:\
inline type get_##name() {\
    return m_##name;\
}\
    

// 指针类型的setget方法.
#define PointerPropertyBuilderByName(type, name, access_permission)\
public:\
    inline void set_##name(type* v){\
        m_##name = v;\
    }\
    inline type* get_##name(){\
        return m_##name;\
    }\

// 指针类型的get方法.
#define PointerPropertyGetByName(type, name, access_permission)\
public:\
    inline type* get_##name(){\
        return m_##name;\
    }\
    
// 引用类型的get方法
#define ReferencePropertyGetByName(type, name, access_permission)\
public:\
    inline type* get_##name(){\
        return &m_##name;\
    }\

    

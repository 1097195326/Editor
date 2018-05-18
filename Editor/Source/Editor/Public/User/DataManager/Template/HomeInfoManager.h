#pragma once


#include "CoreMinimal.h"
#include "GBaseTemplateManager.h"
#include "MsgCenter.h"
#include "HomeInfoT.h"
#include "JsonSerializer.h"
#include <functional>

/**
*
*/
class EDITOR_API HomeInfoManager :public GBaseTemplateManager
{
private:
	// 单利. 
	static HomeInfoManager *   m_pInstance;

	TArray<HomeInfoT*>			m_BikiniTypesArr;
	TArray<HomeInfoT*>			m_SportTypesArr;
	TArray<HomeInfoT*>			m_SubTypesArr;

	FString						m_ItemTitle;

	typedef std::function<void(bool)> CallbackDelegate;
	CallbackDelegate m_CallbackFunc;

private:
	void RequestUrl(FString _Url);
	void GetDataArray(TSharedPtr<FJsonObject> & _content, TArray<HomeInfoT*> & Arr);
	void ResponseFinish(msg_ptr _msg);


public:
	static HomeInfoManager * GetInstance();

public:
	HomeInfoManager();
	virtual ~HomeInfoManager();

	template<class T>
	void GetDataFromURL(FString _url, T * _ob, void(T::*_func)(bool))
	{
		m_CallbackFunc = CallbackDelegate(std::bind(_func, _ob, std::placeholders::_1));

		RequestUrl(_url);
	}

public:
	TArray<HomeInfoT*>  GetBikiniTypes() { return m_BikiniTypesArr; }
	TArray<HomeInfoT*>  GetSportTypes() { return m_SportTypesArr; }
	TArray<HomeInfoT*>	GetCurrentDataList() { return m_SubTypesArr; }
	FString				GetItemTitle(){return m_ItemTitle;}

	virtual void OnLoad() override;
	virtual void OnRelease() override;
	virtual void EncodeData(TSharedPtr<FJsonObject> & _content) override;
	void	EncodeSportData(TSharedPtr<FJsonObject> & _content);
	
};

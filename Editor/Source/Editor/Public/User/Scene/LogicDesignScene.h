// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseLogicScene.h"
#include "BaseComponent.h"
#include "SuitT.h"
#include "MsgCenter.h"
#include "UITextureManager.h"
#include "DesignActorRender.h"
#include "ComponentStruct.h"

// 保存款类型.
enum EnumSaveStyleType:int32
{
	SAVE_STYLE_FOR_DEFAULT=0, // 默认保存完直接显示分享界面.
	SAVE_STYLE_FOR_SHOP,// 保存完自动跳转到商城h5.
	SAVE_STYLE_FOR_CANCEL_BTN //当用户点取消按钮保存时候.
};

/**
 * 
 */
class EDITOR_API LogicDesignScene:public BaseLogicScene
{
public:
	bool m_isSaveing = false;

	// 是否点击商城保存.
	EnumSaveStyleType m_saveType = EnumSaveStyleType::SAVE_STYLE_FOR_DEFAULT;

	// 判断是否修改过.
	bool m_isUpdated = false;

private:
	DesignActorRender * m_designRender = nullptr;

	// 文理管理器,最多保存20个文理超过就释放掉.
	UUITextureManager * m_textureManager = nullptr;

public:
	// 返回设计场景的通知.
	const static int32 m_rebackDesignSceneMsg = 4015;

	// 保存成功以后服务器返回的RStyleId.
	int32 m_savedStyleRid = 0;

	// 保存成功以后生成的日志Id.
	int32 m_savedLogId = 0;

public:
	LogicDesignScene();

	virtual ~LogicDesignScene();

protected:

	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void OnBackstageLevel(const FString & _levelName) override;

	virtual void OnLevelBeginPlay(const FString & _levelName) override;

public:
	// 初始化设计场景.
	void InitDesignScene();

	// 初始化走秀长青.
	void InitWalkingScene();

	// 切换走秀场景.
	void JoinWalkingLevel();

	// 下载花型资源并且渲染.
	void StartRenderStaticActor();

	// 保存事件.
	void SaveStyle(const FString & _iconUrl,TArray<FString> & _PromotionList);

	// 给摄像机用。获取componet的数组.
	FMeshComponentStruct GetComponentStructByTid(int32 _tid, EnumComponentType _componentTyp);

	// 完成设计退出设计场景.
	void FinishDesign();

	// 当渲染完成的回调.
	void OnRenderFinish();

	// 当拍封面图的事件.
	void OnCapture(bool _isShow);

public: // ui 相关功能.

	// 判断用户是否修改过.
	bool IsUpdated();

	// 获取默认选中的tid == 只适用于f部件辅料线迹.
	int32 GetDefaultComponentTid(int32 _parentTid, EnumComponentType _componentTyp);

	// 获取从片上默认选中的花型，面料，花色.
	FString GetDefaultPatternInSlice(int32 _parentTid, EnumComponentType _componentTyp);

	int32 GetDefaultFabricInSlice(int32 _parentTid, EnumComponentType _componentTyp);

	int32 GetDefaultColorInSlice(int32 _parentTid, EnumComponentType _componentTyp);

	// 获取用到的所有花型.
	void GetAllUsedPatternIds(TArray<FString> & _patternIds);

	// 当前设计的片显示高亮.
	void SetOutLineBySliceTid(int32 _sliceTid);

	// 重置所有的片不显示高亮.
	void ResetAllSliceOutLine();

	// 切换部件.
	void ChangeComponent(int32 _parentTid,int32 _targetComponentTid, EnumComponentType _componentTyp);

	// 切换花型.
	void ChangePattern(int32 _parentTid,FString _targetPaternTid, EnumComponentType _componentTyp);

	// 切换花色.
	void ChangeColor(int32 _parentTid,int32 _targetColorTid, EnumComponentType _componentTyp);

	// 切换面料.
	void ChangeFabric(int32  _parentTid,int32 _targetFabricTid, EnumComponentType _componentTyp);

	// 批量修改部件下的所有花型花色面料.
	void ChangeAllSliceChildComponentByTid(int32 _parentId,UsedFabricStruct & _usedFabricStruct, EnumComponentType _componentTyp);

	// 通过Tid、获取所有片上的不同花色花型面料的结构体.
	void GetAllFabricInfoByTid(int32 _parentTid, TArray<UsedFabricStruct> & _structArray,EnumComponentType _componentType);

	// 通过部位TID获取AdapterType,返回0是不用比对.
	int32 GetAdapterType(int32 _partTid);

	// 根据面料ID，获取部位的类型,成衣辅料线迹.
	EnumComponentType GetComponentTypeByFabricId(int32 _fabricId);

private:

	void CreateNewStyleJson(const FString & _iconUrl, TArray<FString> & _PromotionList, TSharedPtr<FJsonObject> & t_saveInfoObject);

	BaseComponent * GetRootComponent();

	BaseComponent * GetComponentByTid(int32 _parentTid, TArray<EnumComponentType> & _componentType);

	// 返回设计场景.
	void OnRebackDesignScene(msg_ptr _msg);

	// 创建styleIcon.
	void CreateStyleIcon(const TSharedPtr<FJsonObject> & _saveInfoObject, const FString & _iconUrl);

	// 创建12张图json.
	void CreateStylePromotionImage(const TSharedPtr<FJsonObject> & _saveInfoObject, TArray<FString> & _PromotionList);

	// 创建StyleInfo.
	void CreateStyleInfo(const TSharedPtr<FJsonObject> & _saveInfoObject, const FString & _iconUrl);

	// 新数据保存成功的回调.
	void OnSaveNewJsonMsg(msg_ptr _msg);

	void OnStartRender(msg_ptr _msg);

	// 保存失败的提示回调.
	void OnSaveErrorCallback();

	// 其他片都设置成半透明.
	void SetAllSliceAlpa();
};

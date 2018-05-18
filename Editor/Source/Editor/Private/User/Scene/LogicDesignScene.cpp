// Fill out your copyright notice in the Description page of Project Settings.

#include "LogicDesignScene.h"
#include "GameDirector.h"
#include "GModuleManager.h"
#include "GResourceManager.h"
#include "DesignModule.h"
#include "DesignStaticActor.h"
#include "StylePartC.h"
#include "StyleSliceC.h"
#include "PartComponent.h"
#include "TSuitManager.h"
#include "SuitT.h"
#include "StylePatternC.h"
#include "StyleStitchC.h"
#include "GLanguageTool.h"
#include "UIManager.h"
#include "GController.h"
#include "StyleAccessoriesC.h"
#include "StyleComponent.h"
#include "PatternComponent.h"
#include "DesignUIController.h"
#include "BaseComponent.h"
#include "TDesignTypeManager.h"
#include "SuitSliceT.h"
#include "LoadDataModule.h"
#include "GTimerMgr.h"
#include "LoginModule.h"
#include "SliceComponent.h"
#include "GameDirector.h"

GREGISTER(LogicDesignScene)

LogicDesignScene::LogicDesignScene()
{
}

LogicDesignScene::~LogicDesignScene()
{
}


void LogicDesignScene::OnLoad()
{
	m_isSaveing = false;

	m_saveType = EnumSaveStyleType::SAVE_STYLE_FOR_DEFAULT;

	m_isUpdated = false;

	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, m_rebackDesignSceneMsg, this, &LogicDesignScene::OnRebackDesignScene);

	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, 4029, this, &LogicDesignScene::OnStartRender);

	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeHttpResponse, 4016, this, &LogicDesignScene::OnSaveNewJsonMsg);

	m_designRender = new DesignActorRender();

	m_designRender->GLoad();
}

void LogicDesignScene::OnRelease()
{
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, m_rebackDesignSceneMsg, this, &LogicDesignScene::OnRebackDesignScene);

	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, 4029, this, &LogicDesignScene::OnStartRender);

	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeHttpResponse, 4016, this, &LogicDesignScene::OnSaveNewJsonMsg);

	if (nullptr != m_designRender)
	{
		m_designRender->GRelease();

		m_designRender = nullptr;
	}
}

void LogicDesignScene::OnBackstageLevel(const FString & _levelName)
{
	if (nullptr != m_designRender)
		m_designRender->RemoveRender();
}

void LogicDesignScene::OnLevelBeginPlay(const FString & _levelName)
{
	FString mDesignLevel = TEXT("DesignLevel");

	if (_levelName.Equals(mDesignLevel))
	{
		InitDesignScene();
	}
	else
	{
		InitWalkingScene();
	}
}

// 初始化设计场景.
void LogicDesignScene::InitDesignScene()
{
	if (nullptr != m_designRender)
		m_designRender->RemoveRender();

	if (GController::m_isDebug)
	{
		DesignModule * mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

		GEngine->AddOnScreenDebugMessage(-1, 120, FColor::Red, FString::Printf(TEXT("款id=%d,套件id=%d"), mDesignModule->GetDesignData()->m_designRID, mDesignModule->GetDesignData()->m_designTID));
	}

	// 8.打开UI.
	OpenUIController(TEXT("DesignUIController"));

	// 下载花型.
	StartRenderStaticActor();
}

void LogicDesignScene::InitWalkingScene()
{
	OpenUIController(TEXT("WalkingUIController"));
	m_designRender->StartSkeletalRender();
}

void LogicDesignScene::JoinWalkingLevel()
{
	UUIManager::GetInstance()->RemoveAllAlertUI();

	CloseAllUIController();

	DesignModule * mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

	SuitT * mSuitT = TSuitManager::GetInstance()->GetTemplateByPID<SuitT>(mDesignModule->GetDesignData()->m_designTID);

	UGameDirector::GetInstance()->LoadLevelAsync(mSuitT->m_catwalk);
}

void LogicDesignScene::StartRenderStaticActor()
{
	// 下载花型.
	m_designRender->StartRender();
}


FMeshComponentStruct LogicDesignScene::GetComponentStructByTid(int32 _tid, EnumComponentType _componentTyp)
{
	TArray<EnumComponentType> mComponentTypeArray;

	mComponentTypeArray.Add(_componentTyp);

	FMeshComponentStruct mStruct;

	BaseComponent * mParentComponent = GetComponentByTid(_tid, mComponentTypeArray);

	// 判断是否slice.
	if (mParentComponent->GetComponentTag() == EnumComponentType::SLICE_COMPONENT)
	{
		mStruct.m_meshComponentArray.Add(mParentComponent->m_staticMeshComponent);

		SuitSliceT * mSuitT = mParentComponent->GetDataC()->GetBaseT<SuitSliceT>();

		mStruct.m_CameraRotate = mSuitT->m_CameraRotate;
		mStruct.m_cameraDistance = mSuitT->m_cameraDistance;
		mStruct.m_cameraHeight = mSuitT->m_cameraHeight;
		mStruct.m_cameraPitching = mSuitT->m_cameraPitching;
	}
	else
	{
		// 查找所有的子component.
		TArray<IComponent*> mChildSliceComponentArray;
		
		mParentComponent->GetComponentByTag(EnumComponentType::SLICE_COMPONENT, mChildSliceComponentArray);

		for (int32 i=0;i<mChildSliceComponentArray.Num();i++)
		{
			BaseComponent * mComponent = (BaseComponent *)mChildSliceComponentArray[i];

			mStruct.m_meshComponentArray.Add(mComponent->m_staticMeshComponent);
		}
		if (mParentComponent->GetComponentTag() == EnumComponentType::PIECES_COMPONENT)
		{
			SuitComponentT * mComponentT = mParentComponent->GetDataC()->GetBaseT<SuitComponentT>();

			mStruct.m_CameraRotate = mComponentT->m_CameraRotate;
			mStruct.m_cameraDistance = mComponentT->m_cameraDistance;
			mStruct.m_cameraHeight = mComponentT->m_cameraHeight;
			mStruct.m_cameraPitching = mComponentT->m_cameraPitching;
		}
	}
	return mStruct;
}

void LogicDesignScene::FinishDesign()
{
	LoadDataModule * mLoadDataModule = GModuleManager::GetInstance()->GetModule<LoadDataModule>("LoadDataModule");

	mLoadDataModule->ClearStyleDetailData();

	mLoadDataModule->ClearSuitDetailData();

	UUIManager::GetInstance()->RemoveAllAlertUI();

	UGameDirector::GetInstance()->CloseCurrentLogicScene();

	UGameDirector::GetInstance()->OpenLevel(TEXT("MainLevel"));
}

void LogicDesignScene::OnRenderFinish()
{
	// 通知UIController.
	DesignUIController * mCurrentController = (DesignUIController *)GetCurrentUIController();

	mCurrentController->InitMainCamera();
}

void LogicDesignScene::OnCapture(bool _isShow)
{
	BaseComponent * mParentComponent = GetRootComponent();

	TArray<IComponent*> mAllSliceComponentArray;

	mParentComponent->GetComponentByTag(EnumComponentType::SLICE_COMPONENT, mAllSliceComponentArray);

	for (int32 i = 0; i < mAllSliceComponentArray.Num(); i++)
	{
		SliceComponent * mSliceComponent = (SliceComponent *)mAllSliceComponentArray[i];

		if (_isShow == true)
		{
			mSliceComponent->SetTrueLightChannel();
		}
		else
		{
			mSliceComponent->SetFalseLightChannel();
		}
	}
}

bool LogicDesignScene::IsUpdated()
{
	return m_isUpdated;
}

int32 LogicDesignScene::GetDefaultComponentTid(int32 _parentTid, EnumComponentType _componentTyp)
{
	TArray<EnumComponentType> mComponentTypeArray;

	mComponentTypeArray.Add(_componentTyp);

	BaseComponent * mParentComponent = GetComponentByTid(_parentTid, mComponentTypeArray);
	
	for (int32 i=0;i<mParentComponent->GetChildComponents().Num();i++)
	{
		BaseComponent * mChildComponent = (BaseComponent *)mParentComponent->GetChildComponents()[i];

		if(mChildComponent->GetComponentTag() == EnumComponentType::PIECES_COMPONENT)
			return mChildComponent->GetDataC()->GetTid();
	}
	return 0;
}


FString LogicDesignScene::GetDefaultPatternInSlice(int32 _parentTid, EnumComponentType _componentTyp)
{
	TArray<EnumComponentType> mComponentTypeArray;

	mComponentTypeArray.Add(_componentTyp);

	BaseComponent * mParentComponent = GetComponentByTid(_parentTid, mComponentTypeArray);

	for (int32 i = 0; i < mParentComponent->GetChildComponents().Num(); i++)
	{
		BaseComponent * mChildComponent = (BaseComponent *)mParentComponent->GetChildComponents()[i];

		if (mChildComponent->GetComponentTag() == EnumComponentType::PATTERN_COMPONENT)
		{
			PatternComponent * mPatternComponent = (PatternComponent *)mChildComponent;

			StylePatternC * mPatternC = (StylePatternC *)mPatternComponent->GetDataC();

			return mPatternC->m_patternTid;
		}
	}
	return TEXT("");
}

int32 LogicDesignScene::GetDefaultFabricInSlice(int32 _parentTid, EnumComponentType _componentTyp)
{
	TArray<EnumComponentType> mComponentTypeArray;

	mComponentTypeArray.Add(_componentTyp);

	BaseComponent * mParentComponent = GetComponentByTid(_parentTid, mComponentTypeArray);

	for (int32 i = 0; i < mParentComponent->GetChildComponents().Num(); i++)
	{
		BaseComponent * mChildComponent = (BaseComponent *)mParentComponent->GetChildComponents()[i];

		if (mChildComponent->GetComponentTag() == EnumComponentType::FABRIC_COMPONENT)
			return mChildComponent->GetDataC()->GetTid();
	}
	return 0;
}

int32 LogicDesignScene::GetDefaultColorInSlice(int32 _parentTid, EnumComponentType _componentTyp)
{
	TArray<EnumComponentType> mComponentTypeArray;

	mComponentTypeArray.Add(_componentTyp);

	BaseComponent * mParentComponent = GetComponentByTid(_parentTid, mComponentTypeArray);

	if (mParentComponent->GetComponentTag() == EnumComponentType::SLICE_COMPONENT)
	{
		StyleSliceC * mSliceC = (StyleSliceC *)mParentComponent->GetDataC();

		if (mSliceC)
			return mSliceC->m_suitColorId;
	}
	return 0;
}


void LogicDesignScene::GetAllUsedPatternIds(TArray<FString> & _patternIds)
{
	m_designRender->GetAllPatternIds(_patternIds);
}

void LogicDesignScene::SetOutLineBySliceTid(int32 _sliceTid)
{
	ResetAllSliceOutLine();

	TArray<EnumComponentType> mComponentTypeArray;

	mComponentTypeArray.Add(EnumComponentType::SLICE_COMPONENT);

	BaseComponent * mParentComponent = GetComponentByTid(_sliceTid, mComponentTypeArray);

	if (mParentComponent->GetComponentTag() == EnumComponentType::SLICE_COMPONENT)
	{
		SliceComponent * mSliceComponent = (SliceComponent *)mParentComponent;

		mSliceComponent->SetOutLine();
	}
}

void LogicDesignScene::ResetAllSliceOutLine()
{
	BaseComponent * mParentComponent = GetRootComponent();

	TArray<IComponent*> mAllSliceComponentArray;

	mParentComponent->GetComponentByTag(EnumComponentType::SLICE_COMPONENT, mAllSliceComponentArray);

	for (int32 i = 0; i < mAllSliceComponentArray.Num(); i++)
	{
		SliceComponent * mSliceComponent = (SliceComponent *)mAllSliceComponentArray[i];

		mSliceComponent->ResetOutLine();
	}
}

void LogicDesignScene::SetAllSliceAlpa()
{
	BaseComponent * mParentComponent = GetRootComponent();

	TArray<IComponent*> mAllSliceComponentArray;

	mParentComponent->GetComponentByTag(EnumComponentType::SLICE_COMPONENT, mAllSliceComponentArray);

	for (int32 i = 0; i < mAllSliceComponentArray.Num(); i++)
	{
		SliceComponent * mSliceComponent = (SliceComponent *)mAllSliceComponentArray[i];

		mSliceComponent->SetAlpa();
	}
}

void LogicDesignScene::ChangeComponent(int32 _parentTid, int32 _targetTid, EnumComponentType _componentTyp)
{
	TArray<EnumComponentType> mComponentTypeArray;

	mComponentTypeArray.Add(_componentTyp);

	mComponentTypeArray.Add(EnumComponentType::PART_COMPONENT);

	mComponentTypeArray.Add(EnumComponentType::ACCESSORIES_COMPONENT);

	mComponentTypeArray.Add(EnumComponentType::STITCH_COMPONENT);

	m_isUpdated = true;

	BaseComponent * mComponent = GetComponentByTid(_parentTid, mComponentTypeArray);

	mComponent->ChangePiecesCompont(_targetTid);

	StartRenderStaticActor();

}

void LogicDesignScene::ChangePattern(int32 _parentTid, FString _targetPaternTid, EnumComponentType _componentTyp)
{
	TArray<EnumComponentType> mComponentTypeArray;

	mComponentTypeArray.Add(_componentTyp);
	mComponentTypeArray.Add(EnumComponentType::SLICE_COMPONENT);

	m_isUpdated = true;

	BaseComponent * mComponent = GetComponentByTid(_parentTid, mComponentTypeArray);

	mComponent->ChangePatternComponent(_targetPaternTid);

	StartRenderStaticActor();
}

void LogicDesignScene::ChangeColor(int32 _parentTid, int32 _targetColorTid, EnumComponentType _componentTyp)
{
	TArray<EnumComponentType> mComponentTypeArray;

	mComponentTypeArray.Add(_componentTyp);
	mComponentTypeArray.Add(EnumComponentType::SLICE_COMPONENT);

	m_isUpdated = true;

	BaseComponent * mComponent = GetComponentByTid(_parentTid, mComponentTypeArray);

	mComponent->ChangeColorComponent(_targetColorTid);

	StartRenderStaticActor();
}

void LogicDesignScene::ChangeFabric(int32 _parentTid, int32 _targetFabricTid, EnumComponentType _componentTyp)
{
	TArray<EnumComponentType> mComponentTypeArray;

	mComponentTypeArray.Add(_componentTyp);
	mComponentTypeArray.Add(EnumComponentType::SLICE_COMPONENT);

	m_isUpdated = true;

	BaseComponent * mComponent = GetComponentByTid(_parentTid, mComponentTypeArray);

	mComponent->ChangeFabricComponent(_targetFabricTid);

	StartRenderStaticActor();
}

void LogicDesignScene::ChangeAllSliceChildComponentByTid(int32 _parentId, UsedFabricStruct & _usedFabricStruct, EnumComponentType _componentTyp)
{
	TArray<EnumComponentType> mComponentTypeArray;

	mComponentTypeArray.Add(_componentTyp);

	m_isUpdated = true;

	// 获取指定的Tid.
	BaseComponent * mPartComponent = GetComponentByTid(_parentId, mComponentTypeArray);

	TArray<IComponent*> mSliceComponentArray;

	mPartComponent->GetComponentByTag(EnumComponentType::SLICE_COMPONENT, mSliceComponentArray);

	for (int32 i = 0; i < mSliceComponentArray.Num(); i++)
	{
		BaseComponent * mSliceComponent = (BaseComponent *)mSliceComponentArray[i];

		// 过滤一下上一次是辅料或者线迹.
		IComponent * mParentStitchComponent = mSliceComponent->GetParentComponentByTag(EnumComponentType::STITCH_COMPONENT);

		IComponent * mParentAccessoriesComponent = mSliceComponent->GetParentComponentByTag(EnumComponentType::ACCESSORIES_COMPONENT);

		if (nullptr == mParentStitchComponent && nullptr == mParentAccessoriesComponent)
		{
			mSliceComponent->ChangeRenderTargetTid(_usedFabricStruct);
		}
	}
	StartRenderStaticActor();
}


void LogicDesignScene::GetAllFabricInfoByTid(int32 _parentTid, TArray<UsedFabricStruct> & _structArray, EnumComponentType _componentType)
{
	TArray<EnumComponentType> mComponentTypeArray;

	mComponentTypeArray.Add(_componentType);

	TArray<UsedFabricStruct> mStructArray;

	// 获取指定的Tid.
	BaseComponent * mPartComponent = GetComponentByTid(_parentTid, mComponentTypeArray);

	TArray<IComponent*> mSliceComponentArray;
	
	mPartComponent->GetComponentByTag(EnumComponentType::SLICE_COMPONENT, mSliceComponentArray);

	for (int32 i=0;i<mSliceComponentArray.Num();i++)
	{
		BaseComponent * mSliceComponent = (BaseComponent *)mSliceComponentArray[i];

		// 过滤一下上一次是辅料或者线迹.
		IComponent * mParentStitchComponent = mSliceComponent->GetParentComponentByTag(EnumComponentType::STITCH_COMPONENT);

		IComponent * mParentAccessoriesComponent = mSliceComponent->GetParentComponentByTag(EnumComponentType::ACCESSORIES_COMPONENT);

		if (nullptr == mParentStitchComponent && nullptr == mParentAccessoriesComponent)
		{
			mSliceComponent->GetAllFabricInfoIn(_structArray);
		}
	}
}

int32 LogicDesignScene::GetAdapterType(int32 _partTid)
{
	TArray<EnumComponentType> mComponentTypeArray;

	mComponentTypeArray.Add(EnumComponentType::PART_COMPONENT);

	PartComponent * mPartComponent = (PartComponent *)GetComponentByTid(_partTid, mComponentTypeArray);

	SuitPartT * mPartT = mPartComponent->GetDataC()->GetBaseT<SuitPartT>();

	if (mPartT->m_IsMainPart == 1)
		return 0;

	BaseComponent* mRootComponent = (BaseComponent*)m_designRender->GetRootComponent();

	// 获取所有部位.
	TArray<IComponent*> mPartComponentArray;
	mRootComponent->GetComponentByTag(EnumComponentType::PART_COMPONENT,mPartComponentArray);

	for (int32 i=0;i<mPartComponentArray.Num();i++)
	{
		mPartComponent = (PartComponent *)mPartComponentArray[i];

		mPartT = mPartComponent->GetDataC()->GetBaseT<SuitPartT>();

		if (mPartT->m_IsMainPart == 1)
		{
			TArray<IComponent*> mPieceComponentArray = mPartComponent->GetChildComponents();

			for (int32 j=0;j<mPieceComponentArray.Num();j++)
			{
				BaseComponent * mPieceComponent = (BaseComponent *)mPieceComponentArray[j];

				if (mPieceComponent->GetComponentTag() == EnumComponentType::PIECES_COMPONENT)
				{
					SuitComponentT * mSuitComponentT =  mPieceComponent->GetDataC()->GetBaseT<SuitComponentT>();

					return mSuitComponentT->m_AdapterType;
				}
			}
		}
	}
	return 0;
}

EnumComponentType LogicDesignScene::GetComponentTypeByFabricId(int32 _fabricId)
{
	return m_designRender->GetComponentTypeByFabricId(_fabricId);
}

BaseComponent * LogicDesignScene::GetRootComponent()
{
	BaseComponent* mRootComponent = (BaseComponent*)m_designRender->GetRootComponent();

	return mRootComponent;
}

BaseComponent * LogicDesignScene::GetComponentByTid(int32 _parentTid, TArray<EnumComponentType> & _componentType)
{
	BaseComponent * mComponent = GetRootComponent();

	BaseComponent * mParentComponent = mComponent->GetComponentByTid(_parentTid, _componentType);

	return mParentComponent;
}

void LogicDesignScene::OnRebackDesignScene(msg_ptr _msg)
{
	CloseAllUIController();

	UUIManager::GetInstance()->RemoveAllAlertUI();

	UGameDirector::GetInstance()->LoadLevelAsync(TEXT("DesignLevel"));
}

void LogicDesignScene::CreateStyleIcon(const TSharedPtr<FJsonObject> & _saveInfoObject,const FString & _iconUrl)
{
	DesignModule * mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

	TSharedPtr<FJsonObject> t_styleInfoObject = MakeShareable(new FJsonObject);

	t_styleInfoObject->SetNumberField(TEXT("styleId"), mDesignModule->GetDesignData()->m_designRID);

	t_styleInfoObject->SetNumberField(TEXT("height"), 1024);

	t_styleInfoObject->SetNumberField(TEXT("width"), 1024);

	t_styleInfoObject->SetStringField(TEXT("url"), _iconUrl);

	_saveInfoObject->SetObjectField(TEXT("styleIcon"), t_styleInfoObject);
}

void LogicDesignScene::CreateStylePromotionImage(const TSharedPtr<FJsonObject> & _saveInfoObject, TArray<FString> & _PromotionList)
{
	DesignModule * mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

	TSharedPtr<FJsonObject> t_styleLaunchObject = MakeShareable(new FJsonObject);

	t_styleLaunchObject->SetNumberField(TEXT("styleId"), mDesignModule->GetDesignData()->m_designRID);

	t_styleLaunchObject->SetNumberField(TEXT("height"), 512);

	t_styleLaunchObject->SetNumberField(TEXT("width"), 512);
}

void LogicDesignScene::CreateStyleInfo(const TSharedPtr<FJsonObject> & _saveInfoObject, const FString & _iconUrl)
{
	TSharedPtr<FJsonObject> t_styleInfoObject = MakeShareable(new FJsonObject);

	DesignModule * mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

	LoginModule * mLoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	FString mUserId = mLoginModule->GetUserData()->GetUserId();

	if (mDesignModule->GetDesignData()->m_DesignType == EnumDesignType::DESIGN_BY_SYS_STYLE)
	{
		t_styleInfoObject->SetStringField(TEXT("ownerType"), "USER");

		t_styleInfoObject->SetNumberField(TEXT("isRecommend"), 0);

		t_styleInfoObject->SetStringField(TEXT("flag"), TEXT("ADD"));

		mUserId = mLoginModule->GetUserData()->GetUserId();
	}
	else if (mDesignModule->GetDesignData()->m_DesignType == EnumDesignType::DESIGN_BY_USER_STYLE)
	{
		t_styleInfoObject->SetStringField(TEXT("ownerType"), "USER");

		t_styleInfoObject->SetNumberField(TEXT("isRecommend"), 0);

		t_styleInfoObject->SetStringField(TEXT("flag"), TEXT("UPDATE"));

		mUserId = mLoginModule->GetUserData()->GetUserId();
	}
	else
	{
		t_styleInfoObject->SetStringField(TEXT("ownerType"), "SYSTEM");

		t_styleInfoObject->SetNumberField(TEXT("isRecommend"), 1);

		t_styleInfoObject->SetStringField(TEXT("flag"), TEXT("ADD"));
	}
	t_styleInfoObject->SetStringField(TEXT("ownerId"), mUserId);

	t_styleInfoObject->SetNumberField(TEXT("suitId"), mDesignModule->GetDesignData()->m_designTID);

	t_styleInfoObject->SetStringField(TEXT("name"), TEXT(""));

	t_styleInfoObject->SetStringField(TEXT("commont"), TEXT(""));

	int32 mParentCatId = TDesignTypeManager::GetInstance()->GetCatIdBySuitTid(mDesignModule->GetDesignData()->m_designTID);

	t_styleInfoObject->SetNumberField(TEXT("parentId"), mParentCatId);

	t_styleInfoObject->SetNumberField(TEXT("height"), 512);

	t_styleInfoObject->SetNumberField(TEXT("width"), 512);

	t_styleInfoObject->SetStringField(TEXT("iconUrl"), _iconUrl);

	t_styleInfoObject->SetNumberField(TEXT("id"), mDesignModule->GetDesignData()->m_designRID);

	// 获取款信息.
	BaseComponent * mRootComponent = GetRootComponent();

	mRootComponent->CreateJson(t_styleInfoObject);

	_saveInfoObject->SetObjectField(TEXT("styleInfo"), t_styleInfoObject);
}


void LogicDesignScene::OnSaveNewJsonMsg(msg_ptr _msg)
{
	m_isSaveing = false;

	MsgHttp * mHttpMsg = _msg->ConvertMsg<MsgHttp>();

	//	判断消息是否正常接收.
	if (mHttpMsg->GetMsgErrorCode() == EnumMsgResponseErrorCode::MessageTrue)
	{
		m_isUpdated = false;

		//	消息正常,获取内容.
		TSharedPtr<FJsonObject> t_jsonObject = mHttpMsg->GetHttpContent();

		TSharedPtr<FJsonObject> t_dataObject = t_jsonObject->GetObjectField("data");

		t_dataObject->TryGetNumberField(TEXT("styleId"), m_savedStyleRid);

		t_dataObject->TryGetNumberField(TEXT("logId"), m_savedLogId);

		DesignUIController * mUIController = (DesignUIController *)GetCurrentUIController();

		UUIManager::GetInstance()->RemoveAllAlertUI();

		DesignModule * mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

		// 修改设计模式.从推荐款进来的一旦保存了就变成用户款,并且变成update.要是admin 就变成推荐款而且一直都是推荐款.
		mDesignModule->GetDesignData()->m_designRID = m_savedStyleRid;

		mDesignModule->GetDesignData()->m_DesignType = EnumDesignType::DESIGN_BY_USER_STYLE;

		if (m_saveType == EnumSaveStyleType::SAVE_STYLE_FOR_SHOP)
		{
			// 打开商城H5.
			msg_ptr mLocalShop(new MsgLocal(MessageTypeLocal,2003,nullptr));

			MsgCenter::Instance()->SendMsg(mLocalShop);

			m_saveType = EnumSaveStyleType::SAVE_STYLE_FOR_DEFAULT;
		}
		else if (m_saveType == EnumSaveStyleType::SAVE_STYLE_FOR_CANCEL_BTN)
		{
			// 点击取消按钮保存的时候直接跳转到main场景.
			FinishDesign();
		}
		else
		{
			mUIController->ChangePageTo(Change_PAGE::SHARED_PAGE);
		}
	}
	else
	{
		UUIManager::GetInstance()->RemoveAllAlertUI();

		// 保存失败的提示.
		FString mAlertText = GLanguageTool::GetInstance()->GetLanguage(TEXT("key135"));

		UUIManager::GetInstance()->OpenTrueAlert(mAlertText,this, &LogicDesignScene::OnSaveErrorCallback);
	}
}

void LogicDesignScene::OnStartRender(msg_ptr _msg)
{
	StartRenderStaticActor();
}

void LogicDesignScene::OnSaveErrorCallback()
{

}

void LogicDesignScene::CreateNewStyleJson(const FString & _iconUrl, TArray<FString> & _PromotionList,TSharedPtr<FJsonObject> & t_saveInfoObject)
{
	CreateStyleIcon(t_saveInfoObject, _iconUrl);

	CreateStyleInfo(t_saveInfoObject, _iconUrl);

	FString mStyleJsonStr;

	TSharedRef<TJsonWriter<TCHAR>> t_writer = TJsonWriterFactory<>::Create(&mStyleJsonStr);

	FJsonSerializer::Serialize(t_saveInfoObject.ToSharedRef(), t_writer);
}

void LogicDesignScene::SaveStyle(const FString & _iconUrl, TArray<FString> & _PromotionList)
{
	LoginModule * mLoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	TSharedPtr<FJsonObject> t_saveInfoObject = MakeShareable(new FJsonObject);

	// 1.创建style json.
	CreateNewStyleJson(_iconUrl, _PromotionList, t_saveInfoObject);

	FString mToken = mLoginModule->GetUserData()->GetToken();

	FString mOperator = mLoginModule->GetUserData()->GetUserId();

	// 2.提交服务器.
	msg_ptr mSaveMsg(new MsgHttp(MessageTypeHttpRequest, 4016, mToken, mOperator, false, t_saveInfoObject));

	// 3.通过消息系统发送消息.
	MsgCenter::Instance()->SendMsg(mSaveMsg);
}

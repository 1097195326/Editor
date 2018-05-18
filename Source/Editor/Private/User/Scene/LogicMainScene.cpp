#include "LogicMainScene.h"
#include "UIManager.h"
#include "GameDirector.h"
#include "GLanguageTool.h"
#include "GController.h"
#include "LoadDataModule.h"
#include "GModuleManager.h"

GREGISTER(LogicMainScene)

LogicMainScene::LogicMainScene() 
{

}
LogicMainScene::~LogicMainScene() 
{

}

void LogicMainScene::OnLoad()
{
	// 打开MainUIController.
	// 加载完成款详情的回调.
	
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, 4014, this, &LogicMainScene::OnLoadStyleDetailFinish);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, 4004, this, &LogicMainScene::OnLoadPartFinish);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, 4005, this, &LogicMainScene::OnLoadComponentFinish);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, 4011, this, &LogicMainScene::OnLoadSliceFinish);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, 4013, this, &LogicMainScene::OnLoadAccessoriesFinish);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, 4012, this, &LogicMainScene::OnLoadStitchFinish);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, 3100, this, &LogicMainScene::OnLoadSuitResourceFinish);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, LoadDataModule::m_downloadSuitDetailFinish, this, &LogicMainScene::OnLoadTDataFinish);


	OpenUIController(TEXT("MainUIController"));
}
void LogicMainScene::OnRelease()
{
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, LoadDataModule::m_downloadSuitDetailFinish, this, &LogicMainScene::OnLoadTDataFinish);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, 4014, this, &LogicMainScene::OnLoadStyleDetailFinish);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, 4004, this, &LogicMainScene::OnLoadPartFinish);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, 4005, this, &LogicMainScene::OnLoadComponentFinish);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, 4011, this, &LogicMainScene::OnLoadSliceFinish);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, 4013, this, &LogicMainScene::OnLoadAccessoriesFinish);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, 4012, this, &LogicMainScene::OnLoadStitchFinish);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, 3100, this, &LogicMainScene::OnLoadSuitResourceFinish);
}

void LogicMainScene::BeginJoinDesignLevel(int32 _designRID, int32 _desginTID, EnumDesignType _designType)
{
 	FString mLanguage = GLanguageTool::GetInstance()->GetLanguage(TEXT("key131"));

	UUIManager::GetInstance()->OpenMiddleTip(mLanguage,0.0f);

	m_currentRepeatCount = 1;

	m_checkDesignState = EnumCheckDesignState::WAITING;

	DesignModule *mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

	mDesignModule->SetDesignData(_designRID, _desginTID, _designType);

	//// 开始进入设计场景之前的准备.
	DesignState(EnumCheckDesignState::CHECK_R_DATA);
}

void LogicMainScene::DesignState(EnumCheckDesignState _checkDesignState)
{
	m_checkDesignState = _checkDesignState;
	DesignModule *mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");



	switch (m_checkDesignState)
	{
	case EnumCheckDesignState::CHECK_R_DATA:
		CheckRData();
		break;
	case EnumCheckDesignState::CHECK_PART_T_DATA:
	{
		LoadDataModule * mDataModule = GModuleManager::GetInstance()->GetModule<LoadDataModule>("LoadDataModule");

		DesignData * mDesignData = mDesignModule->GetDesignData();

		mDataModule->DownloadSuitDetailData(mDesignData->m_designTID);
		break;
	}
	case EnumCheckDesignState::CHECK_FINISH:
		JoinDesignLevel();
		break;
	default:
		break;
	}
}

void LogicMainScene::JoinDesignLevel()
{
	UUIManager::GetInstance()->CloseMiddleTip();

	UGameDirector::GetInstance()->LoadLevelAsync(TEXT("DesignLevel"));
}

void LogicMainScene::CheckRData()
{
	DesignModule *mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

	LoadDataModule * mLoadDataModule = GModuleManager::GetInstance()->GetModule<LoadDataModule>("LoadDataModule");

	DesignData * mDesignData = mDesignModule->GetDesignData();

	if (GController::m_isAdmin)
	{
		mDesignData->m_DesignType = EnumDesignType::DESIGN_BY_SUIT;

		msg_ptr mMsg(new MsgLocal(MessageTypeLocal, LoadDataModule::m_loadStyleRDetailFinish, EnumMsgResponseErrorCode::MessageTrue, TEXT("加载部片成功!")));

		MsgCenter::Instance()->SendMsg(mMsg);

		return;
	}
	else
	{
		// 加载款详情.
		mLoadDataModule->LoadStyleDetails(mDesignData->m_designRID);
	}
}
 
void LogicMainScene::CheckPartTData()
{
	DesignModule *mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

	LoadDataModule * mLoadDataModule = GModuleManager::GetInstance()->GetModule<LoadDataModule>("LoadDataModule");

	DesignData * mDesignData = mDesignModule->GetDesignData();

	mLoadDataModule->LoadPartT(mDesignData->m_designTID);
}

void LogicMainScene::CheckComponentTData()
{
	DesignModule *mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

	LoadDataModule * mLoadDataModule = GModuleManager::GetInstance()->GetModule<LoadDataModule>("LoadDataModule");

	DesignData * mDesignData = mDesignModule->GetDesignData();

	mLoadDataModule->LoadPartComponentT(mDesignData->m_designTID);
}

void LogicMainScene::CheckSliceTData()
{
	DesignModule *mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

	LoadDataModule * mLoadDataModule = GModuleManager::GetInstance()->GetModule<LoadDataModule>("LoadDataModule");

	DesignData * mDesignData = mDesignModule->GetDesignData();

	mLoadDataModule->LoadSliceT(mDesignData->m_designTID);
}

void LogicMainScene::CheckAccessoriesTData()
{
	DesignModule *mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");


	LoadDataModule * mLoadDataModule = GModuleManager::GetInstance()->GetModule<LoadDataModule>("LoadDataModule");

	DesignData * mDesignData = mDesignModule->GetDesignData();

	mLoadDataModule->LoadAccessoriesT(mDesignData->m_designTID);
}

void LogicMainScene::CheckStitchTData()
{
	DesignModule *mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

	LoadDataModule * mLoadDataModule = GModuleManager::GetInstance()->GetModule<LoadDataModule>("LoadDataModule");

	DesignData * mDesignData = mDesignModule->GetDesignData();

	mLoadDataModule->LoadStitchT(mDesignData->m_designTID);
}

void LogicMainScene::CheckSuitResource()
{
	// 检查pak包是否存在. 
	// gx! 临时判断以后存在.
	msg_ptr mMsg(new MsgLocal(MessageTypeLocal, 3100, EnumMsgResponseErrorCode::MessageTrue, TEXT("加载面料库成功!")));
	MsgCenter::Instance()->SendMsg(mMsg);
}

void LogicMainScene::OnLoadStyleDetailFinish(msg_ptr _msg)
{
	MsgLocal * mLocalMsg = _msg->ConvertMsg<MsgLocal>();

	EnumMsgResponseErrorCode mErrorCode = mLocalMsg->GetMsgErrorCode();

	if (mErrorCode == EnumMsgResponseErrorCode::MessageTrue)
	{
		m_currentRepeatCount = 1;

		// 开始执行下一步，验证T数据.
		DesignState(EnumCheckDesignState::CHECK_PART_T_DATA);
	}
	else
	{
		// 加载失败.判断是否重试.
		CheckRepeat();
	}
}

void LogicMainScene::OnLoadPartFinish(msg_ptr _msg)
{
	MsgLocal * mLocalMsg = _msg->ConvertMsg<MsgLocal>();

	EnumMsgResponseErrorCode mErrorCode = mLocalMsg->GetMsgErrorCode();

	if (mErrorCode == EnumMsgResponseErrorCode::MessageTrue)
	{
		m_currentRepeatCount = 1;
		// 开始执行下一步，验证T数据.
		DesignState(EnumCheckDesignState::CHECK_COMPONENT_T_DATA);
	}
	else
	{
		// 加载失败.判断是否重试.
		CheckRepeat();
	}
}

void LogicMainScene::OnLoadComponentFinish(msg_ptr _msg)
{
	MsgLocal * mLocalMsg = _msg->ConvertMsg<MsgLocal>();

	EnumMsgResponseErrorCode mErrorCode = mLocalMsg->GetMsgErrorCode();

	if (mErrorCode == EnumMsgResponseErrorCode::MessageTrue)
	{
		m_currentRepeatCount = 1;

		// 开始执行下一步，验证T数据.
		DesignState(EnumCheckDesignState::CHECK_SLICE_T_DATA);
	}
	else
	{
		// 加载失败.判断是否重试.
		CheckRepeat();
	}
}

void LogicMainScene::OnLoadSliceFinish(msg_ptr _msg)
{
	MsgLocal * mLocalMsg = _msg->ConvertMsg<MsgLocal>();

	EnumMsgResponseErrorCode mErrorCode = mLocalMsg->GetMsgErrorCode();

	if (mErrorCode == EnumMsgResponseErrorCode::MessageTrue)
	{
		m_currentRepeatCount = 1;

		// 开始执行下一步，验证T数据.
		DesignState(EnumCheckDesignState::CHECK_ACCESSORIES_T_DATA);
	}
	else
	{
		// 加载失败.判断是否重试.
		CheckRepeat();
	}
}

void LogicMainScene::OnLoadAccessoriesFinish(msg_ptr _msg)
{
	// 开始执行下一步，验证T数据.
	MsgLocal * mLocalMsg = _msg->ConvertMsg<MsgLocal>();

	EnumMsgResponseErrorCode mErrorCode = mLocalMsg->GetMsgErrorCode();

	if (mErrorCode == EnumMsgResponseErrorCode::MessageTrue)
	{
		m_currentRepeatCount = 1;

		// 开始执行下一步，验证T数据.
		DesignState(EnumCheckDesignState::CHECK_STITCH_T_DATA);
	}
	else
	{
		// 加载失败.判断是否重试.
		CheckRepeat();
	}
}

void LogicMainScene::OnLoadStitchFinish(msg_ptr _msg)
{
	MsgLocal * mLocalMsg = _msg->ConvertMsg<MsgLocal>();

	EnumMsgResponseErrorCode mErrorCode = mLocalMsg->GetMsgErrorCode();

	if (mErrorCode == EnumMsgResponseErrorCode::MessageTrue)
	{
		m_currentRepeatCount = 1;

		// 开始执行下一步，验证T数据.
		DesignState(EnumCheckDesignState::CHECK_SUIT_RESOURCE);
	}
	else
	{
		// 加载失败.判断是否重试.
		CheckRepeat();
	}
}

void LogicMainScene::OnLoadSuitResourceFinish(msg_ptr _msg)
{
	MsgLocal * mLocalMsg = _msg->ConvertMsg<MsgLocal>();

	EnumMsgResponseErrorCode mErrorCode = mLocalMsg->GetMsgErrorCode();

	if (mErrorCode == EnumMsgResponseErrorCode::MessageTrue)
	{
		m_currentRepeatCount = 1;
		// 开始执行下一步，验证T数据.
		DesignState(EnumCheckDesignState::CHECK_FINISH);
	}
	else
	{
		// 加载失败.判断是否重试.
		CheckRepeat();
	}
}

void LogicMainScene::OnLoadTDataFinish(msg_ptr _msg)
{
	MsgLocal * mLocalMsg = _msg->ConvertMsg<MsgLocal>();

	EnumMsgResponseErrorCode mErrorCode = mLocalMsg->GetMsgErrorCode();

	if (mErrorCode == EnumMsgResponseErrorCode::MessageTrue)
	{
		DesignState(EnumCheckDesignState::CHECK_FINISH);
	}
	else
	{
		UUIManager::GetInstance()->RemoveAllAlertUI();
	}
}

void LogicMainScene::OnErrorLoadDataClickBtnTrue()
{

}

void LogicMainScene::CheckRepeat()
{
	if (m_currentRepeatCount <= m_AllrepeatCount)
	{
		m_currentRepeatCount++;
		DesignState(m_checkDesignState);
	}
	else
	{
		OnCheckError();
	}
}

void LogicMainScene::OnCheckError()
{
	// 整个验证失败.
	UUIManager::GetInstance()->CloseMiddleTip();

	FString mLanguage = GLanguageTool::GetInstance()->GetLanguage(TEXT("key132"));

	// 打开tip.
	UUIManager::GetInstance()->OpenTrueAlert(mLanguage,this,&LogicMainScene::OnErrorLoadDataClickBtnTrue);

	// 清空数据.
	LoadDataModule * mLoadDataModule = GModuleManager::GetInstance()->GetModule<LoadDataModule>("LoadDataModule");

	mLoadDataModule->ClearStyleDetailData();

	mLoadDataModule->ClearSuitDetailData();
}


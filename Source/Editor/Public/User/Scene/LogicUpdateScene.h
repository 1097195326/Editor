#pragma once
#include "CoreMinimal.h"
#include "MsgChannel.h"
#include "BaseLogicScene.h"

class EDITOR_API LogicUpdateScene :public BaseLogicScene
{
public:
	LogicUpdateScene();
	virtual ~LogicUpdateScene();

protected:

	virtual void OnLoad() override;
	
	virtual void OnRelease() override;

public:
	void StartForcedUpdate();
	void OnFinishedForcedUpdate();
	void DownloadPak();
	void DownloadDefaultData();
	void DownloadSuitDetail(int32 _SuitTid);

	void OnDownloadPakSuccess(msg_ptr _msg);
	void OnDownloadDefaultDataSuccess(msg_ptr _msg);
	void OnInitXmlSuccess(msg_ptr _msg);
	void OnLoadTypeFinish(msg_ptr _msg);
	void OnDownloadSuitDetailSuccess(msg_ptr _msg);
public:

	//����Pak����Ϣ��.
	static const int32 m_DownloadPak_Progress = 3012;
	//����Ĭ��������Ϣ��.
	static const int32 m_DownloadDefaultData_Progress = 3013;
	//����suit������Ϣ��.
	static const int32 m_DownloadSuitDetail_Progress = 3014;
	//����Pak�����ʱ��Ϣ��.
	static const  int32  m_DownloadPak_Finished = 3015;



};

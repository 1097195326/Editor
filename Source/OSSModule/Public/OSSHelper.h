#pragma once
#include "IOSSDownload.h"

class OSSMODULE_API OSSHelper
{

public:
	OSSHelper();
	~OSSHelper();

	static OSSHelper *GetInstance();
	IOSSDownload *GetOSSTool();

	void Init(FString _AosEndPoint, FString _AosAccessKeyID, FString _AosAccessKeySecret, FString _AosBucket);

	void Release();

	void ExitGame();
	
private:
	static OSSHelper *Instance;
	bool isInit = false;
public:
	IOSSDownload *OSSDownload=nullptr;

};


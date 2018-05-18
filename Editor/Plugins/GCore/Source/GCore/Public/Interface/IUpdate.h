// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * update有优先级，比如先收到网络的消息，在处理游戏逻辑。最后在发送给网络.
 */
class GCORE_API IUpdate
{
public:
	IUpdate();
	virtual ~IUpdate();
    
    // update handler.
	virtual void OnUpdateReceived(float DeltaTime);

    virtual void OnUpdateService(float DeltaTime);
    
    virtual void OnUpdate(float DeltaTime);
    
    virtual void OnUpdateSend(float DeltaTime);
};

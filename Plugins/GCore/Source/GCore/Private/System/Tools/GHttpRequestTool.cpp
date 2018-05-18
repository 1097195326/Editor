// Fill out your copyright notice in the Description page of Project Settings.
#include "GCore.h"
#include "GHttpRequestTool.h"
//GHttpRequestTool::GHttpRequestTool()
//{
//	m_isCancel = false;
//
//	m_isFinish = false;
//}
//
//GHttpRequestTool::~GHttpRequestTool()
//{
//}
//
//void GHttpRequestTool::OnMsgResponse(FHttpRequestPtr inReq, FHttpResponsePtr inResp, bool inSuccess)
//{
//	m_isFinish = true;
//
//	if (!m_isCancel)
//	{
//		m_callbackFunc(inReq, inResp, inSuccess);
//
//		delete this;
//	}
//}
//
//void GHttpRequestTool::Request()
//{
//	FHttpModule::Get().SetHttpTimeout(5);
//
//	TSharedRef<IHttpRequest> m_HttpRequest = FHttpModule::Get().CreateRequest();
//
//	m_HttpRequest->SetURL(m_url);
//
//	m_HttpRequest->SetVerb(TEXT("GET"));
//	
//	m_HttpRequest->OnProcessRequestComplete() = FHttpRequestCompleteDelegate::CreateRaw(this, &GHttpRequestTool::OnMsgResponse);
//
//	m_HttpRequest->ProcessRequest();
//}
//
//void GHttpRequestTool::Cancel()
//{
//	m_isCancel = true;
//
//	if (!m_isFinish)
//	{
//		m_HttpRequest->CancelRequest();
//
//		delete this;
//	}
//}

// Fill out your copyright notice in the Description page of Project Settings.
#include "GCore.h"
#include "BaseC.h"


BaseC::BaseC()
{}

BaseC::~BaseC()
{}

int32 BaseC::GetCid()
{
	return m_cid;
}

void BaseC::SetCid(int32 _cid)
{
	m_cid = _cid;
}

void BaseC::SetTid(int32 _tid)
{
	m_tid = _tid;
}

int32 BaseC::GetTid()
{
	return m_tid;
}

BaseT * BaseC::GetBaseT()
{
	return m_dataT;
}

void BaseC::GLoad()
{
	OnLoad();
}

void BaseC::GRelease()
{
	OnRelease();

	delete this;
}

void BaseC::InitData(int32 _cid, int32 _rid, int32 _tid)
{
	m_cid = _cid;

	m_rid = _rid;

	m_tid = _tid;

	OnInitData(_cid, _rid, _tid);
}

void BaseC::ChangeTid(int32 _tid)
{
	m_tid = _tid;

	OnInitData(m_cid, m_rid, m_tid);
}

void BaseC::AddJsonC(TSharedPtr<FJsonObject> & _jsonObj)
{

}


#include "pch.h"
#include "CTSInfo.h"




void CTSInfo::initSession()
{
	m_nID = m_nSessID = m_nSequence = m_nStatus = m_nRecvCnt = m_nSendCnt = 0;

	m_nSendIndex = m_nRecvIndex = 0;
}

void CTSInfo::initSession(int initState)
{
	m_nSessID = m_nSequence = 0;
	m_nStatus = initState;
	m_nRecvCnt = m_nSendCnt = 0;

	m_nSendIndex = m_nRecvIndex = 0;
}

int CTSInfo::GetID()
{
	return m_nID;
}

void CTSInfo::SetID(int id)
{
	m_nID = id;
}

int CTSInfo::GetTSID()
{
	return m_nTS_ID;
}

void CTSInfo::SetTSID(int tsID)
{
	m_nTS_ID = tsID;
}

int CTSInfo::GetSessID()
{
	return m_nSessID;
}

void CTSInfo::SetSessID(int id)
{
	m_nSessID = id;
}

int CTSInfo::GetStatus()
{
	return m_nStatus;
}

void CTSInfo::SetStatus(int state)
{
	m_nStatus = state;
}

int CTSInfo::GetSequence()
{
	return m_nSequence;
}

int CTSInfo::IncreaseSequence()
{
	m_nSequence++;
	if (m_nSequence > 255)
		m_nSequence = 0;

	return m_nSequence;
}

int CTSInfo::GetRecvCnt()
{
	return m_nRecvCnt;
}

void CTSInfo::increaseRecvCnt()
{
	m_nRecvCnt++;
}

int CTSInfo::GetSendCnt()
{
	return m_nSendCnt;
}


void CTSInfo::increaseSendCnt()
{
	m_nSendCnt++;
}

int CTSInfo::GetSendIndex()
{
	return 0;
}

int CTSInfo::increaseSendIndex()
{
	return m_nSendIndex;
}

int CTSInfo::GetRecvIndex()
{
	return m_nRecvIndex;
}

void CTSInfo::SetRecvIndex(int index)
{
	m_nRecvIndex = index;
}

CString CTSInfo::Get_CurrentTime()
{
	SYSTEMTIME cur_time;
	GetLocalTime(&cur_time);
	CString csPCTime;

	//strPCTime.Format("%04d%02d%02d%02d%02d%02d%03ld", cur_time.wYear,cur_time.wMonth, cur_time.wDay,
	//	cur_time.wHour,	cur_time.wMinute, cur_time.wSecond,	cur_time.wMilliseconds);

	csPCTime.Format(_T("%02d:%02d:%02d.%03ld"), cur_time.wHour, cur_time.wMinute, cur_time.wSecond, cur_time.wMilliseconds);
	return csPCTime;
}

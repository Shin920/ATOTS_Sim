#pragma once
#include "pch.h"
#include <afxcmn.h>


enum LogColumns
{
	LogColumn_Start = 0,
	LogColumn_Time = LogColumn_Start, LogColumn_Command, LogColumn_Type, LogColumn_SendReceive, LogColumn_Binary,
	LogColumn_MAX
};

class CLogListCtrl : public CListCtrl
{
public:
	CLogListCtrl();
	virtual ~CLogListCtrl();

	
	BOOL SaveLogFile();
	BOOL SaveLogFileAppend(int tabValue);


	inline void SetThreadRun(BOOL b) { threadRun.store(b); }
	inline const BOOL GetThreadRun() const { return threadRun.load(); }
	BOOL isPauseChecked(BOOL logPause);
	
	void ThreadStop();

private:
	void WriteStringOfPacket(CStdioFile& ipfile, const int idx) const;
	const int itemMaxCount;

	mutex logMtx;
	thread loadfileThread;
	std::atomic<BOOL> threadRun;
public:
	DECLARE_MESSAGE_MAP()
		afx_msg void OnNMCustomdraw(NMHDR* pNMHDR, LRESULT* pResult);
};
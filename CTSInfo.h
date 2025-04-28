#pragma once

#define _TS_ITEM_MAX_	3
typedef enum {
	TS_ETCS_INFO_ = 0,
	TS_NETWORK_,
	TS_STU_NETWORK_,
	TS_AUTO_RUN_,
	//STU_KEEP_ALIVE_,
	TS_MAX_
}TS_CONFIG_enum_type;


typedef struct {
	TS_CONFIG_enum_type id_;
	char* strHdr;
	char* strItem[_TS_ITEM_MAX_];
}TS_CONFIG_type;

static TS_CONFIG_type TS_Config_Table[] = {
	{ TS_ETCS_INFO_, _T("ETCS Info"), {_T("TS ID"), _T(""), _T("") } },
	{ TS_NETWORK_, _T("TS Network"), { _T("IP"), _T("PORT"), _T("")} },
	{ TS_STU_NETWORK_, _T("TS-STU Network"), { _T("IP"), _T("PORT"), _T("") } },
	{ TS_AUTO_RUN_, _T("AUTO RUN"), {_T("Interval(msec)"), _T(""), _T("") } },
	//{ STU_KEEP_ALIVE_, _T("STU KEEP ALIVE"), { _T("UINT"), _T("COUNT"), _T("ALIVE SESSION") } },
	{ TS_MAX_, _T(""), { _T(""), _T(""), _T("") } },
};


class CTSInfo
{
public:
	int m_nID;
	int m_nSessID;
	int m_nStatus;
	int m_nSequence;
	int m_nTS_ID;

	int m_nSendCnt;
	int m_nRecvCnt;

	int m_nSendIndex, m_nRecvIndex;

	void initSession();

	void initSession(int initState);

	int	 GetSessID();
	void SetSessID(int id);

	int  GetID();
	void SetID(int id);

	int  GetStatus();
	void SetStatus(int state);

	int  GetSequence();
	int IncreaseSequence();

	int  GetTSID();
	void  SetTSID(int tsID);

	int  GetRecvCnt();
	int  GetSendCnt();
	void increaseRecvCnt();
	void increaseSendCnt();

	int  GetSendIndex();
	int increaseSendIndex();

	int  GetRecvIndex();
	void SetRecvIndex(int index);

	CString Get_CurrentTime();

};


#pragma once
#include "Feature.h"
#include "afxdialogex.h"
#include "CTSInfo.h"
#include "GridPropertyOverride.h"
#include "F110_Radio_Packet.h"
#include "CMFCLogInfoPropertyGridCtrl.h"
#include <afxcmn.h>
#include "packet01Tab.h"
#include "packet02Tab.h"
#include "packet04Tab.h"
#include "packet07Tab.h"
#include "packet09Tab.h"
#include "packet10Tab.h"
#include "FontManager.h"

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN			// 거의 사용하지 않는 내용은 Windows 헤더에서 제외
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXSPLICIT_CONSTRUCTORS			// 일부 CString 생성자는 명시적으로 

#define AFX_ALL_WARNINGS		// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기 해제

#define OB_MAX_SESSION_NUMBER	10
#define MAX_SESSION_NUMBER	4



#include <afxwin.h>				// MFC 핵심 및 표준 구성 요소
#include <afxext.h>				// MFC 확장

#include <afxdisp.h>			// MFC 자동화 클래스


/*
메인 탭 컨트롤 0번에 추가되는 Send 패킷용 Dialog
클래스 번호는 실제 NID_PACKET_ATO 번호를 가리킴
*/
class packet01Tab;
class packet02Tab;
class packet04Tab;
class packet07Tab;
class packet09Tab;
class packet10Tab;

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>			// Internet Explorer 4 공용 컨트롤에 대한 MFC 자원
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>				// Windows 공용 컨트롤에 대한 MFC 자원
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>		// MFC의 리본 및 컨트롤 막대 자원	

// CTabDlg1 대화 상자

#include "kr37_atp_msg.h"
#include "CLogListCtrl.h"

class CTabDlg1 : public CDialogEx
{
private:
	
	//Status Report variable
	CString curSpeed;
	CString tpId;
	CString arrTime;
	CWnd* m_pParentWnd;

	DECLARE_DYNAMIC(CTabDlg1)

public:
	CTabDlg1(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTabDlg1();
	//CRecvTab* pRTab;


	/*
	Send 패킷 다이얼로그 포인터
	*/
	packet01Tab* p01Tab;
	packet02Tab* p02Tab;
	packet04Tab* p04Tab;
	packet07Tab* p07Tab;
	packet09Tab* p09Tab;
	packet10Tab* p10Tab;
	
	CCriticalSection g_criticalExe_1;

	/*
	ATO State Icon Image용 enum
	*/
	enum class ImgATOstatus
	{
		Blank = -1,
		UN, CO, NA, AV,
		RE, EG, DE, FA,
		MAX
	};

	enum class STRIndicator
	{
		Blank = -1,
		bit0, bit1, bit2, bit3,
		bit4, bit5, bit6, bit7,
		MAX
	};


// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_TAB1 };
#endif
	
private:	
	void InitVariables();
	int currentPacket;

protected:
	void Init();
	void InitPropCtrl();
	void Release();
	void Init_Font();
	

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	/*
	패킷 Receive시 Array를 Decode하여 담아둘 구조체 포인터 선언
	*/
	ATO_Header* pHeaderStructure;
	OB_TS_Packet00_HSReqStruct* pHSReqStructure;
	OB_TS_Packet03_JPReqStruct* pJPReqStructure;
	OB_TS_Packet05_JPAckStruct* pJPAckStructure;
	OB_TS_Packet06_SPReqStruct* pSPReqStructure;
	OB_TS_Packet08_STRStruct* pSTRStructure;
	OB_TS_Packet11_SESSTermStruct* pSessTermStructure;

	DECLARE_MESSAGE_MAP()
public:



	// UDP 소켓
	SOCKET		m_SendSocket;
	sockaddr_in m_sockAddr;

	/*
	세션 ID
	Hand Over용 2개 
	*/
	BYTE m_SessID[3]{};
		
	// 상태 이미지
	CImage imgDisConn;
	CImage imgConn;
	
	//BYTE m_TS_ID[3];
	CStatic m_pic_onoff;
	CImage imgConnectFail;
	CImage imgBlank;
	CFont m_font;
	
	

	//ATO Status
	CImage imgStatusAV, imgStatusCO, imgStatusDE, imgStatusEG,
		imgStatusFA, imgStatusNA, imgStatusNP, imgStatusRE;
	int atoStatusValue;

	//Ctrl 변수
	CLogListCtrl m_lc_msg;
	CIPAddressCtrl m_cTS_IP;
	CEdit m_cTS_ID;
	CComboBox m_plist_cmb;
	CEdit m_cTS_port;
	CListCtrl m_strList;
	CListCtrl m_cListDebug;
	CTabCtrl m_ras_tab;
	CStatic m_etcs_id;
	CStatic m_pic_atostate;
	CButton m_auto_sa;
	CStatic m_pic_length;
	CListBox m_lb_debug;
	CIPAddressCtrl m_cOB_IP;
	CEdit m_cOB_port;
	CComboBox m_cmb_recv;
	CMFCPropertyGridCtrl m_propgrid;
	CButton m_log_pause;
	CButton m_show_alive;
	CStatic m_sTS_ID;
	CStatic m_sTS_IP;
	CStatic m_sTS_PORT;
	CStatic m_sOB_IP;
	CStatic m_sOB_PORT;
	CStatic m_sOP;
	CStatic m_sEN;
	CStatic m_sMAV;
	CEdit m_cOP;
	CEdit m_cEN;
	CEdit m_cMAV;
	CStatic m_send_alive;
	CStatic m_receive_alive;
	CStatic m_sAC;

	//SA-CONNECT(UDP) 연결시 사용할 변수
	CTSInfo m_OB[10];
	int m_nTS_ID;
	string m_nTS_IP;
	int m_nTS_PORT;
	int m_nOB_PORT;
	int	m_nSequence;	
	int m_msgType;
	int cnt = 1;
	int rcnt = 1;
	int m_nCountry_ID;
	int m_nETCS_ID;
	CString m_csETCS_ID;

	//시험 시작 시간 변수
	UINT32 testStartSec = 0;

	CString myPath;
	CString SPPath;

	BOOL		m_bATORunning;
	BOOL		m_bAutoResponse;
	BOOL        m_saConComplete;

	BOOL		m_bRunTimer_Server, m_bAutoRun;		// Auto Run 시작 여부
	BOOL		m_bAutoRunTimerStarted[10];	// 각 Timer 시작 여부
	UINT_PTR	m_nAutoRunTimer[10];		// 각 Timer ID

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	
	int m_nOB_ID[10];
	int m_nSess_ID[10];	

	int m_nAUTO_INTERVAL;
	
	/*
	CONFIG 설정 INI Load, Save 함수
	getExePath 함수로 현재 실행된 경로 구하여 저장함
	*/
	afx_msg void OnBnClickedBtnInit();
	afx_msg void OnBnClickedBtnApply();
	CString getExePath();
		
	/*
	TabCtrl 관련 함수
	OnDestroy시 TabCtrl용 Dialog 포인터 삭제
	*/
	afx_msg void OnTcnSelchangeTabTs(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();

	
	// TS Start, Finish
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonFinish();
	

	void CopySelectedItemToClipboard(CListCtrl& listCtrl);
	afx_msg void OnNMClickListMsg(NMHDR* pNMHDR, LRESULT* pResult);
	
	afx_msg void OnBnClickedButtonCon();
	
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnBnClickedButtonDiscon();

	afx_msg void OnLvnBeginlabeleditListMsg(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnRclickListMsg(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuExpand();
	afx_msg void OnMenuAllDelete();
		
	afx_msg void OnCbnSelchangeCmbRecv();
	void Release_Ctrl();
	void updtPacket(_In_ const byte* arr, _In_ const size_t length);
	void SetCtrl_ATOstatus(ImgATOstatus v);
	CString SetText_STRIndcators(UINT16 value);
	void SetCtrl_TPinformation();
	

	afx_msg void OnLvnItemchangedListMsg(NMHDR* pNMHDR, LRESULT* pResult);

	int GetSequence();
	
	void Cmd_Send(int cmd);
	
	void Cmd_Send126(int type);
	int  SendTo(BYTE* buf, int len);
	void Debug(CString msg);
	void Debug(const char* format, ...);
	void KR37_MakeLength(BYTE* bID, int length);
	void KR37_IntToByte(int nID, BYTE* bID, int size);
	int KR37_ByteToInt(BYTE* bID);
	CString Get_CurrentTime();
	void Parse_Cmd_TS(BYTE* buf, int len);
	int KR37_GetLength(BYTE* bID);
	void MessageLog(CString inout, CString cmd, CString type, CString origin_msg);
	void MessageLogVector(CString inout, CString cmd, CString type, const std::vector<UINT8>& buffer);
	void MessageLog(CString inout, CString cmd, CString type, BYTE* buffer, int len);
	void MessageLog_SC(CString inout, CString cmd, CString origin_msg);
	void MessageLog_SC(CString inout, CString cmd, BYTE* buffer, int len);
	void SessionSet(BYTE* sessID, BYTE* resp_etcsID);
	void SetSessionStatusResetAll();
	void SetSessionStatus(int index, BYTE state);
	void propgrid();
	void chgCmb(int index);
	int SetSessionStatus(int index, BYTE* obID, BYTE* sessID, int state);

	int CheckSessionID(BYTE* sessID);
	void SetConfigList(BOOL isInsert, int row, CString csCategory, CString csValue);
	void Init_IconImage();
	void StartTimer(BOOL bStart);	
	afx_msg LRESULT LogFileAdd(WPARAM wParam, LPARAM lParam);
	inline CString getPacketString(int index) { return m_NPmap[index].first; };
	/*
	
	CRC

	*/
	unsigned int KR37_set_crc32_init_value(unsigned int start);
	unsigned int KR37_compute_check_sum(byte* start, unsigned int data_len);	
	
	afx_msg void OnEnKillfocuscen();
	afx_msg void OnEnKillfocuscop();
	afx_msg void OnEnKillfocuscmav();
	
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);	// ESC, Enter 시 종료 방지
	afx_msg void OnBnClickedButtonAdd();
	
	using NIDPACKET_MAP = map<int, pair<CString, int>>;
	NIDPACKET_MAP m_NPmap{
		make_pair(0, make_pair(_T("HS Req"), 0)),
		make_pair(1, make_pair(_T("HS Ack"), 1)),
		make_pair(2, make_pair(_T("HS Rej"), 2)),
		make_pair(3, make_pair(_T("JP Req"), 3)),
		make_pair(4, make_pair(_T("JP"), 4)),
		make_pair(5, make_pair(_T("JP Ack"), 5)),
		make_pair(6, make_pair(_T("SP Req"), 6)),
		make_pair(7, make_pair(_T("SP"), 7)),
		make_pair(8, make_pair(_T("STR"), 8)),
		make_pair(9, make_pair(_T("STR Ack"), 9)),
		make_pair(10, make_pair(_T("SessTerm Req"), 10)),
		make_pair(11, make_pair(_T("Sess Term"), 11))
	};
	afx_msg void OnBnClickedButtonClear();
	CButton m_checkindi;
	afx_msg void OnBnClickedCheckIndi();
};


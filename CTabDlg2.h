#pragma once
#include "Feature.h"
#include "afxdialogex.h"
#include "CTSInfo.h"
#include "GridPropertyOverride.h"
#include "F110_Radio_Packet.h"
#include "CMFCLogInfoPropertyGridCtrl.h"
#include <afxcmn.h>
#include "packet01Tab_2.h"
#include "packet02Tab_2.h"
#include "packet04Tab_2.h"
#include "packet07Tab_2.h"
#include "packet09Tab_2.h"
#include "packet10Tab_2.h"
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

// CTabDlg2 대화 상자

#include "kr37_atp_msg.h"
#include "CLogListCtrl.h"

class CTabDlg2 : public CDialogEx
{
private:

	//Status Report variable
	CString curSpeed;
	CString tpId;
	CString arrTime;

	DECLARE_DYNAMIC(CTabDlg2)

public:
	CTabDlg2(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTabDlg2();
	
	packet01Tab_2* p01Tab_2;
	packet02Tab_2* p02Tab_2;
	packet04Tab_2* p04Tab_2;
	packet07Tab_2* p07Tab_2;
	packet09Tab_2* p09Tab_2;
	packet10Tab_2* p10Tab_2;

	CCriticalSection g_criticalExe_2;

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

	//패킷
	ATO_Header* pHeaderStructure;
	OB_TS_Packet00_HSReqStruct* pHSReqStructure;
	OB_TS_Packet03_JPReqStruct* pJPReqStructure;
	OB_TS_Packet05_JPAckStruct* pJPAckStructure;
	OB_TS_Packet06_SPReqStruct* pSPReqStructure;
	OB_TS_Packet08_STRStruct* pSTRStructure;
	OB_TS_Packet11_SESSTermStruct* pSessTermStructure;

	DECLARE_MESSAGE_MAP()
public:

	BYTE m_SessID_2[3]{};
	
	CImage imgDisConn;
	CImage imgConn;
	CImage imgWarn;
	CImage imgCheck;
	BYTE m_TS_ID[3];

	CImage imgBlank;
	CFont m_font;
	CFont m_font2;

	//ATO Status
	CImage imgStatusAV, imgStatusCO, imgStatusDE, imgStatusEG,
		imgStatusFA, imgStatusNA, imgStatusNP, imgStatusRE;
	int atoStatusValue;

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
	CStatic m_pic_onoff;

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
	CString getExePath();

	/*

	CRC

	*/
	unsigned int KR37_set_crc32_init_value(unsigned int start);
	unsigned int KR37_compute_check_sum(byte* start, unsigned int data_len);




	int m_nOB_ID[10];
	int m_nSess_ID[10];
	
	CImage imgConnectFail;

	int m_nAUTO_INTERVAL;



	CComboBox m_plist_cmb;

	//CListCtrl m_lc_msg;
	CLogListCtrl m_lc_msg;

	CIPAddressCtrl m_cTS_IP;
	CEdit m_cTS_ID;
	CEdit m_cTS_port;
	SOCKET		m_SendSocket;
	sockaddr_in m_sockAddr;
	afx_msg void OnBnClickedBtnInit();
	afx_msg void OnBnClickedBtnApply();


	//CButton m_chb_auto;


	CListCtrl m_strList;
	CListCtrl m_cListDebug;
	CTabCtrl m_ras_tab;
	afx_msg void OnTcnSelchangeTabTs(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();

	afx_msg void OnBnClickedButtonStart();
	CStatic m_etcs_id;
	afx_msg void OnBnClickedButtonFinish();
	CStatic m_pic_atostate;

	CButton m_auto_sa;

	void CopySelectedItemToClipboard(CListCtrl& listCtrl);
	afx_msg void OnNMClickListMsg(NMHDR* pNMHDR, LRESULT* pResult);
	CStatic m_pic_driver;
	CStatic m_dashboard;
	CStatic m_pic_length;
	CListBox m_lb_debug;
	//void CreateHorizontalScroll();
	afx_msg void OnBnClickedButtonCon();
	CIPAddressCtrl m_cOB_IP;
	CEdit m_cOB_port;
	CButton m_show_alive;
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnBnClickedButtonDiscon();

	afx_msg void OnLvnBeginlabeleditListMsg(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnRclickListMsg(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuExpand();
	afx_msg void OnMenuAllDelete();


	CComboBox m_cmb_recv;
	CMFCPropertyGridCtrl m_propgrid;
	afx_msg void OnCbnSelchangeCmbRecv();
	void Release_Ctrl();
	void updtPacket(_In_ const byte* arr, _In_ const size_t length);
	void SetCtrl_ATOstatus(ImgATOstatus v);
	CString SetText_STRIndcators(UINT16 value);
	void SetCtrl_TPinformation();
	CButton m_log_pause;
	
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
	CButton m_checkindi;

	afx_msg void OnEnKillfocuscop();
	afx_msg void OnEnKillfocuscen();
	afx_msg void OnEnKillfocuscmav();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedCheckIndi();
};


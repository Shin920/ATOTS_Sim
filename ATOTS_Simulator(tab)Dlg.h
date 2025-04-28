
// ATOTS_Simulator(tab)Dlg.h: 헤더 파일
//

#pragma once
//#include "MyServer.h"

#define TS_1 1
#define TS_2 2
#define BYTES_OF_SESSID 3
#define BYTES_OF_TSID 3
#define BITS_OF_ONEBYTE 8

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

class CTabDlg1;
class CTabDlg2;


// CATOTSSimulatortabDlg 대화 상자
class CATOTSSimulatortabDlg : public CDialogEx
{
private:
	//MyServer m_server;
	SOCKET		m_RecvSocket;
	BOOL		m_bRecvRunning;
// 생성입니다.
public:
	CATOTSSimulatortabDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	
	CTabDlg1* pDlg1;
	CTabDlg2* pDlg2;
	int m_nCurrentTab = -1;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ATOTS_SIMULATORTAB_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_Tab;
	afx_msg void OnTcnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();

	void TS_RecvFromByBlock();

	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnFinish();

	void KR37_IntToByte(int nID, BYTE* bID, int size);
	int KR37_ByteToInt(BYTE* bID);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

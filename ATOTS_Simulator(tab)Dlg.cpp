
// ATOTS_Simulator(tab)Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "ATOTS_Simulator(tab).h"
#include "ATOTS_Simulator(tab)Dlg.h"
#include "afxdialogex.h"
#include "CTabDlg1.h"
#include "CTabDlg2.h"

#define UPDATE_TO_VARI()		UpdateData(TRUE)	// Control 값을 변수에 적용
#define UPDATE_TO_CTRL()		UpdateData(FALSE)	// 변수를 Control 에 적용



// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.


// CATOTSSimulatortabDlg 대화 상자



CATOTSSimulatortabDlg::CATOTSSimulatortabDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ATOTS_SIMULATORTAB_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_crtBreakAlloc = 322580;
}

void CATOTSSimulatortabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, m_Tab);
}

BEGIN_MESSAGE_MAP(CATOTSSimulatortabDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CATOTSSimulatortabDlg::OnTcnSelchangeTab)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_START, &CATOTSSimulatortabDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_FINISH, &CATOTSSimulatortabDlg::OnBnClickedBtnFinish)
END_MESSAGE_MAP()


// CATOTSSimulatortabDlg 메시지 처리기

BOOL CATOTSSimulatortabDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	//AfxSetAllocStop(72);

	m_Tab.InsertItem(0, _T("TS1"));
	m_Tab.InsertItem(1, _T("TS2"));

	m_Tab.SetCurSel(0);

	CRect rect;
	m_Tab.GetWindowRect(&rect);

	pDlg1 = new CTabDlg1;
	pDlg1->Create(IDD_DLG_TAB1, &m_Tab);
	pDlg1->MoveWindow(0, 25, rect.Width(), rect.Height());
	pDlg1->ShowWindow(SW_SHOW);

	pDlg2 = new CTabDlg2;
	pDlg2->Create(IDD_DLG_TAB2, &m_Tab);
	pDlg2->MoveWindow(0, 25, rect.Width(), rect.Height());
	pDlg2->ShowWindow(SW_HIDE);
	
	//
	


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}


// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CATOTSSimulatortabDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CATOTSSimulatortabDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CATOTSSimulatortabDlg::OnTcnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult)	//Tab 처리
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int sel = m_Tab.GetCurSel();
	m_nCurrentTab = sel;
	
	switch (sel) {
	case 0:
		pDlg1->ShowWindow(SW_SHOW);
		pDlg2->ShowWindow(SW_HIDE);

		break;
	case 1:
		pDlg1->ShowWindow(SW_HIDE);
		pDlg2->ShowWindow(SW_SHOW);

		break;
	}

	*pResult = 0;
}


BOOL CATOTSSimulatortabDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(255, 255, 255));

	return TRUE;
}


HBRUSH CATOTSSimulatortabDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	pDC->SetBkColor(RGB(255, 255, 255)); // Static Control 전부 글자배경색 하얀색으로


	//특정 Static Control 글자색 변경
	//if (pWnd->GetDlgCtrlID() == IDC_STU_STATUS)    // 9행
	//{
	//	pDC->SetTextColor(RGB(0, 100, 0));     // 11행

	//}

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}





void CATOTSSimulatortabDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	m_bRecvRunning = FALSE;

	pDlg1->DestroyWindow();
	delete pDlg1;
	pDlg2->DestroyWindow();
	delete pDlg2;
	
	CMFCVisualManager::DestroyInstance(TRUE);
	//_CrtDumpMemoryLeaks();
	
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

static void TS_RECV_Thread(void* p)

{
	CATOTSSimulatortabDlg* pDlg = reinterpret_cast<CATOTSSimulatortabDlg*>(p);
	
	pDlg->TS_RecvFromByBlock();

}

// 여기서 TS1, TS2로 분기됨
void CATOTSSimulatortabDlg::TS_RecvFromByBlock()
{
	SA_MESSAGE_STRUCTURE msg_struct;
	int nSize;
	BYTE RecvBuf[1024] = {};
	int BufLen = 1024;
	sockaddr_in SenderAddr;
	int SenderAddrSize = sizeof(SenderAddr);
		

	// 두 다이얼로그의 Send 소켓 주소 및 포트
	sockaddr_in sender_addr_tab1;
	int sender_addr_tab1_len = sizeof(sender_addr_tab1);
	getsockname(pDlg1->m_SendSocket, (SOCKADDR*)&sender_addr_tab1, &sender_addr_tab1_len);

	sockaddr_in sender_addr_tab2;
	int sender_addr_tab2_len = sizeof(sender_addr_tab2);
	getsockname(pDlg2->m_SendSocket, (SOCKADDR*)&sender_addr_tab2, &sender_addr_tab2_len);

	while (m_bRecvRunning) {
		nSize = recvfrom(m_RecvSocket, (char*)&RecvBuf, BufLen, 0, (SOCKADDR*)&SenderAddr, &SenderAddrSize);
		if (nSize <= 0) break;
		// 수신한 데이터의 송신자 주소와 포트를 확인하여 두 다이얼로그의 Send 소켓과 동일한지 확인
		if ((SenderAddr.sin_addr.s_addr == sender_addr_tab1.sin_addr.s_addr &&
			SenderAddr.sin_port == sender_addr_tab1.sin_port) ||
			(SenderAddr.sin_addr.s_addr == sender_addr_tab2.sin_addr.s_addr &&
				SenderAddr.sin_port == sender_addr_tab2.sin_port))
		{
			// 두 다이얼로그의 송신자와 동일한 데이터는 무시
			continue;
		}



			memcpy((void*)&msg_struct, RecvBuf, nSize);


			// ETCS ID, 세션 ID로 분기
			if (nSize > 0)
			{
				//KR37 = ETCS ID 
				int KR37 = KR37_ByteToInt(msg_struct.body.connect_req.responder.etcs_id);

				if (pDlg1->m_nTS_ID == KR37
					|| Utility::areArraysEqual(pDlg1->m_SessID, msg_struct.body.data.sacepid, 3)
					|| Utility::areArraysEqual(pDlg1->m_SessID, msg_struct.body.disconnect.sacepid, 3))
				{
					pDlg1->Parse_Cmd_TS(RecvBuf, nSize);
				}
				else if (pDlg2->m_nTS_ID == KR37
					|| Utility::areArraysEqual(pDlg2->m_SessID_2, msg_struct.body.data.sacepid, 3)
					|| Utility::areArraysEqual(pDlg2->m_SessID_2, msg_struct.body.disconnect.sacepid, 3))
				{
					pDlg2->Parse_Cmd_TS(RecvBuf, nSize);
				}
				else
				{					
					if (msg_struct.header.opcode == OPCODE_SCM_ALIVE
						|| msg_struct.header.opcode == OPCODE_KVC_ALIVE)
					{
						pDlg1->Parse_Cmd_TS(RecvBuf, nSize);
						pDlg2->Parse_Cmd_TS(RecvBuf, nSize);
					}
				}

			}
		

		if (!m_bRecvRunning)
			break;
	}
}



void CATOTSSimulatortabDlg::OnBnClickedBtnStart()
{
	CString ts1port;
	CString ts2port;
	pDlg1->m_cTS_port.GetWindowTextA(ts1port);
	pDlg2->m_cTS_port.GetWindowTextA(ts2port);

	if (ts1port != ts2port) {
		AfxMessageBox(_T("TS1과 TS2의 포트가 같지 않습니다. (Receive Socket은 하나입니다)"));
	}
	else
	{
		GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_FINISH)->EnableWindow(TRUE);

		pDlg1->PostMessage(WM_COMMAND, MAKEWPARAM(IDC_BUTTON_START, BN_CLICKED), (LPARAM)GetDlgItem(IDC_BUTTON_START)->GetSafeHwnd());
		pDlg2->PostMessage(WM_COMMAND, MAKEWPARAM(IDC_BUTTON_START, BN_CLICKED), (LPARAM)GetDlgItem(IDC_BUTTON_START)->GetSafeHwnd());

		// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		sockaddr_in RecvAddr;
		int     nTmp;
		WSADATA wsaData;

		WSAStartup(MAKEWORD(2, 2), &wsaData); //이거

		m_RecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); //이거
		if (m_RecvSocket < 0) {
			pDlg1->Debug("Server Socket Creation Failed(%d)", m_RecvSocket);
			pDlg2->Debug("Server Socket Creation Failed(%d)", m_RecvSocket);
			return;
		}

		int nRecvBufferSize = 0;
		socklen_t	len = sizeof(nRecvBufferSize);
		getsockopt(m_RecvSocket, SOL_SOCKET, SO_RCVBUF, (char*)&nRecvBufferSize, &len);



		nRecvBufferSize = 10 * 1024 * 1024;
		setsockopt(m_RecvSocket, SOL_SOCKET, SO_RCVBUF, (char*)&nRecvBufferSize, len);



		// 포트 가져오게 수정 해야함
		// Bind the socket to any address and the specified port.

		int nowport = _ttoi(ts1port);

		RecvAddr.sin_family = AF_INET;
		RecvAddr.sin_port = htons(nowport);
		RecvAddr.sin_addr.s_addr = htonl(INADDR_ANY);

		nTmp = ::bind(m_RecvSocket, (SOCKADDR*)&RecvAddr, sizeof(RecvAddr)); //이거
		pDlg1->Debug("TS-OB Binding Result(%d), Port(%d)", nTmp, nowport);
		pDlg2->Debug("TS-OB Binding Result(%d), Port(%d)", nTmp, nowport);

		HANDLE h_rbc = (HANDLE) ::_beginthread(TS_RECV_Thread, 0, (void*)this);
		m_bRecvRunning = TRUE;
	}
}


void CATOTSSimulatortabDlg::OnBnClickedBtnFinish()
{
	GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_FINISH)->EnableWindow(FALSE);

	pDlg1->PostMessage(WM_COMMAND, MAKEWPARAM(IDC_BUTTON_FINISH, BN_CLICKED), (LPARAM)GetDlgItem(IDC_BUTTON_FINISH)->GetSafeHwnd());
	pDlg2->PostMessage(WM_COMMAND, MAKEWPARAM(IDC_BUTTON_FINISH, BN_CLICKED), (LPARAM)GetDlgItem(IDC_BUTTON_FINISH)->GetSafeHwnd());
	//pDlg1, 2 세션ID 초기화 필요
	
	memset(pDlg1->m_SessID, 0, sizeof(pDlg1->m_SessID)); // 모든 요소를 0으로 초기화
	memset(pDlg2->m_SessID_2, 0, sizeof(pDlg2->m_SessID_2)); // 모든 요소를 0으로 초기화

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bRecvRunning = FALSE;
	closesocket(m_RecvSocket);
	
}

void CATOTSSimulatortabDlg::KR37_IntToByte(int nID, BYTE* bID, int size)
{
	if (size == 3) {
		bID[0] = (BYTE)((nID >> 16) & 0xFF);
		bID[1] = (BYTE)((nID >> 8) & 0xFF);
		bID[2] = (BYTE)(nID & 0xFF);
	}
	else if (size == 4) {
		bID[0] = (BYTE)((nID >> 24) & 0xFF);
		bID[1] = (BYTE)((nID >> 16) & 0xFF);
		bID[2] = (BYTE)((nID >> 8) & 0xFF);
		bID[3] = (BYTE)(nID & 0xFF);
	}
}

int CATOTSSimulatortabDlg::KR37_ByteToInt(BYTE* bID)
{
	int retValue = 0;

	retValue = (((int)bID[0] & 0xFF) << 16) + (((int)bID[1] & 0xFF) << 8) + (((int)bID[2] & 0xFF));

	return retValue;
}


BOOL CATOTSSimulatortabDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}

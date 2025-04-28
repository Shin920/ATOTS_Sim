#include "pch.h"
#include "ATOTS_Simulator(tab).h"
#include "afxdialogex.h"
#include "CTabDlg2.h"
#include <iostream>
#include <memory>
#include "F110_Msg_Encoder_ATO.h"
#include "PropertyCtrlBuilder.h"
#include "F110_Msg_Decoder_ATO.h"
#include "CLogListCtrl.h"
#include "ATOTS_Simulator(tab)Dlg.h"

using namespace PacketHeaderEnum;
using namespace PacketHSAckEnum;
using namespace PacketHSRejEnum;
using namespace std;

#define PORT			7911
#define PACKET_LENGTH	1024
#define LOCAL_IP		"127.0.0.1"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define STU_ALIVE_Timer	101

#define NUMBER_TS_TS_SESSION 2
#define NUMBER_MAX_SESSION	10
#define NUMBER_OB_ACCEPT	(NUMBER_MAX_SESSION - NUMBER_TS_TS_SESSION)

#define UPDATE_TO_CTRL()		UpdateData(FALSE)	// 변수를 Control 에 적용
#define UPDATE_TO_VARI()		UpdateData(TRUE)	// Control 값을 변수에 적용

#define MY_Timer_Server	100

#pragma comment(lib, "ws2_32.lib")		//윈속 기능 라이브러리




// CTabDlg2 대화 상자

IMPLEMENT_DYNAMIC(CTabDlg2, CDialogEx)

CTabDlg2::CTabDlg2(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_TAB2, pParent)
{
	InitVariables();
	Init();
	
}

CTabDlg2::~CTabDlg2()
{
	
	Release();
	//Release_Ctrl();
}

void CTabDlg2::InitVariables()
{	
	m_bRunTimer_Server = FALSE;
	m_nAUTO_INTERVAL = 1000;	//msec
	m_nSequence = 0;
	m_nCountry_ID = 887;

	m_bATORunning = m_bAutoResponse = FALSE;
	m_saConComplete = FALSE;
}
void CTabDlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ONOFF_PIC, m_pic_onoff);



	DDX_Control(pDX, IDC_LIST_MSG, m_lc_msg);
	DDX_Control(pDX, IDC_IP_TS, m_cTS_IP);
	DDX_Control(pDX, IDC_TS_ID, m_cTS_ID);
	DDX_Control(pDX, IDC_TS_PORT, m_cTS_port);
	DDX_Control(pDX, IDC_LIST_DEBUG, m_cListDebug);
	DDX_Control(pDX, IDC_TAB_TS, m_ras_tab);
	DDX_Control(pDX, IDC_ETCS_ID, m_etcs_id);
	DDX_Control(pDX, IDC_ATO_STATE, m_pic_atostate);
	
	DDX_Control(pDX, IDC_IP_OB, m_cOB_IP);
	DDX_Control(pDX, IDC_OB_PORT, m_cOB_port);
	DDX_Control(pDX, IDC_CHK_AUTO_SA, m_auto_sa);
	DDX_Control(pDX, IDC_STR_LIST, m_strList);

	DDX_Control(pDX, IDC_CMB_RECV, m_cmb_recv);
	DDX_Control(pDX, IDC_MPG_RECV, m_propgrid);
	DDX_Control(pDX, IDC_LOG_PAUSE, m_log_pause);
	DDX_Control(pDX, IDC_CHK_SHOW_ALIVE, m_show_alive);

	DDX_Control(pDX, IDC_STATIC_1, m_sTS_ID);
	DDX_Control(pDX, IDC_STATIC_2, m_sTS_IP);
	DDX_Control(pDX, IDC_STATIC_3, m_sTS_PORT);
	DDX_Control(pDX, IDC_STATIC_4, m_sOB_IP);
	DDX_Control(pDX, IDC_STATIC_5, m_sOB_PORT);
	DDX_Control(pDX, IDC_OP, m_sOP);
	DDX_Control(pDX, IDC_EN, m_sEN);
	DDX_Control(pDX, IDC_MAV, m_sMAV);
	DDX_Control(pDX, IDC_cOP, m_cOP);
	DDX_Control(pDX, IDC_cEN, m_cEN);
	DDX_Control(pDX, IDC_cMAV, m_cMAV);
	DDX_Control(pDX, IDC_ALIVE_COUNTER, m_send_alive);
	DDX_Control(pDX, IDC_RECV_ALIVE_COUNTER, m_receive_alive);
	DDX_Control(pDX, IDC_SALIVE_COUNTER, m_sAC);
	DDX_Control(pDX, IDC_CHECK_INDI, m_checkindi);
}


BEGIN_MESSAGE_MAP(CTabDlg2, CDialogEx)

	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_TS, &CTabDlg2::OnTcnSelchangeTabTs)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_INIT, &CTabDlg2::OnBnClickedBtnInit)
	ON_BN_CLICKED(IDC_BTN_APPLY, &CTabDlg2::OnBnClickedBtnApply)
	ON_BN_CLICKED(IDC_BUTTON_START, &CTabDlg2::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_FINISH, &CTabDlg2::OnBnClickedButtonFinish)
	ON_NOTIFY(NM_CLICK, IDC_LIST_MSG, &CTabDlg2::OnNMClickListMsg)
	ON_BN_CLICKED(IDC_BUTTON_CON, &CTabDlg2::OnBnClickedButtonCon)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_DISCON, &CTabDlg2::OnBnClickedButtonDiscon)
	ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_LIST_MSG, &CTabDlg2::OnLvnBeginlabeleditListMsg)
	ON_WM_CONTEXTMENU()
	ON_MESSAGE(WM_INSERTPACKET, &CTabDlg2::LogFileAdd)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_MSG, &CTabDlg2::OnRclickListMsg)
	ON_COMMAND(IDR_MENU_EXPAND, &CTabDlg2::OnMenuExpand)
	ON_COMMAND(IDR_MENU_ALL_DELETE, &CTabDlg2::OnMenuAllDelete)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_CMB_RECV, &CTabDlg2::OnCbnSelchangeCmbRecv)
	ON_EN_KILLFOCUS(IDC_cOP, &CTabDlg2::OnEnKillfocuscop)
	ON_EN_KILLFOCUS(IDC_cEN, &CTabDlg2::OnEnKillfocuscen)
	ON_EN_KILLFOCUS(IDC_cMAV, &CTabDlg2::OnEnKillfocuscmav)
	ON_BN_CLICKED(IDC_CHECK_INDI, &CTabDlg2::OnBnClickedCheckIndi)
END_MESSAGE_MAP()


// CTabDlg2 메시지 처리기

void CTabDlg2::Init()
{
	pHeaderStructure = new ATO_Header;
	pHSReqStructure = new OB_TS_Packet00_HSReqStruct;
	pJPReqStructure = new OB_TS_Packet03_JPReqStruct;
	pJPAckStructure = new OB_TS_Packet05_JPAckStruct;
	pSPReqStructure = new OB_TS_Packet06_SPReqStruct;
	pSTRStructure = new OB_TS_Packet08_STRStruct;
	pSessTermStructure = new OB_TS_Packet11_SESSTermStruct;


	currentPacket = -1;


}

void CTabDlg2::InitPropCtrl()
{

	CRect rect;

	m_propgrid.EnableHeaderCtrl(TRUE, _T("Item"), _T("Value"));
	m_propgrid.GetWindowRect(&rect);
	m_propgrid.PostMessage(WM_SIZE, 0, MAKELONG(rect.Width() - 50, rect.Height()));


	CMFCPropertyGridProperty* pxpgp;
	CMFCPropertyEX<UINT32>* pxu;

	CMFCPropertyGridProperty* pxProperty;
	pxProperty = new CMFCPropertyGridProperty(_T("HEADER Data"));
	m_propgrid.AddProperty(pxProperty);

	pxpgp = new CMFCPropertyGridProperty(_T("NID_PACKET_ATO"), (variant_t)(int)0, _T("패킷 넘버"));
	pxpgp->Enable(FALSE);
	pxProperty->AddSubItem(pxpgp);

	pxu = new CMFCPropertyEX<UINT32>(_T("NID_OPERATIONAL"), (variant_t)(UINT32)0, _T("Train Running Number"));
	pxu->SetFormat("%u");
	pxu->SetMinMaxValue(0, UINT32_MAX);
	pxu->Enable(FALSE);
	pxProperty->AddSubItem(pxu);

	pxpgp = new CMFCPropertyGridProperty(_T("NID_ENGINE"), (variant_t)(int)0, _T("ETCS Identify"));
	pxpgp->Enable(FALSE);
	pxProperty->AddSubItem(pxpgp);

	pxpgp = new CMFCPropertyGridProperty(_T("T_TimeStamp_Date"), (variant_t)(int)0, _T("Timestamp in date since start-up"));
	pxpgp->Enable(FALSE);
	pxProperty->AddSubItem(pxpgp);

	pxpgp = new CMFCPropertyGridProperty(_T("T_TimeStamp_Seconds"), (variant_t)(int)0, _T("Timestamp in milliseconds since start-up"));
	pxpgp->Enable(FALSE);
	pxProperty->AddSubItem(pxpgp);

	pxpgp = new CMFCPropertyGridProperty(_T("N_Packet_Counter"), (variant_t)(int)0, _T("보내진 패킷 Counter"));
	pxpgp->Enable(FALSE);
	pxProperty->AddSubItem(pxpgp);

	pxProperty = new CMFCPropertyGridProperty(_T("BODY Data"));
	m_propgrid.AddProperty(pxProperty);
}

void CTabDlg2::Release()
{
	delete pHeaderStructure;
	delete pHSReqStructure;
	delete pJPReqStructure;
	delete pJPAckStructure;
	delete pSPReqStructure;
	delete pSTRStructure;
	delete pSessTermStructure;

}

void CTabDlg2::Init_Font()
{
	LOGFONT lf;
	GetFont()->GetLogFont(&lf);

	lf.lfHeight = 17;
	lf.lfWeight = FW_BOLD;

	//lf.lfFaceName;

	CFont* tempFont = new CFont;
	tempFont->CreateFontIndirect(&lf);
	m_sTS_ID.SetFont(tempFont);
	m_sTS_IP.SetFont(tempFont);
	m_sTS_PORT.SetFont(tempFont);
	m_sOB_IP.SetFont(tempFont);
	m_sOB_PORT.SetFont(tempFont);
	m_sOP.SetFont(tempFont);
	m_sEN.SetFont(tempFont);
	m_sMAV.SetFont(tempFont);
	m_sAC.SetFont(tempFont);
	m_checkindi.SetFont(tempFont);
	FontManager::GetInstance().InsertFont(_T("fontBold17"), tempFont);
		

	lf.lfHeight = 17;
	lf.lfWeight = FW_THIN;
	tempFont = new CFont;
	tempFont->CreateFontIndirect(&lf);
	m_cTS_ID.SetFont(tempFont);
	m_cTS_IP.SetFont(tempFont);
	m_cTS_port.SetFont(tempFont);
	m_cOB_IP.SetFont(tempFont);
	m_cOB_port.SetFont(tempFont);
	m_cOP.SetFont(tempFont);
	m_cEN.SetFont(tempFont);
	m_cMAV.SetFont(tempFont);
	m_send_alive.SetFont(tempFont);
	m_receive_alive.SetFont(tempFont);
	m_show_alive.SetFont(tempFont);
	m_log_pause.SetFont(tempFont);
	m_strList.SetFont(tempFont);
	FontManager::GetInstance().InsertFont(_T("fontThin17"), tempFont);

	
}



BOOL CTabDlg2::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	
	

	m_ras_tab.InsertItem(0, _T("HSAck"));
	m_ras_tab.InsertItem(1, _T("HSRej"));
	m_ras_tab.InsertItem(2, _T("JP"));
	m_ras_tab.InsertItem(3, _T("SP"));
	m_ras_tab.InsertItem(4, _T("STRAck"));
	m_ras_tab.InsertItem(5, _T("STReq"));

	m_ras_tab.SetCurSel(0);

	Init_IconImage();
	Init_Font();
	//CreateHorizontalScroll();


	CRect rect;


	m_ras_tab.GetWindowRect(&rect);

	p01Tab_2 = new packet01Tab_2;
	p01Tab_2->Create(IDD_DLG_SEND_7, &m_ras_tab);
	p01Tab_2->MoveWindow(0, 25, rect.Width(), rect.Height());
	p01Tab_2->ShowWindow(SW_SHOW);

	p02Tab_2 = new packet02Tab_2;
	p02Tab_2->Create(IDD_DLG_SEND_8, &m_ras_tab);
	p02Tab_2->MoveWindow(0, 25, rect.Width(), rect.Height());
	p02Tab_2->ShowWindow(SW_HIDE);

	p04Tab_2 = new packet04Tab_2;
	p04Tab_2->Create(IDD_DLG_SEND_9, &m_ras_tab);
	p04Tab_2->MoveWindow(0, 25, rect.Width(), rect.Height());
	p04Tab_2->ShowWindow(SW_HIDE);

	p07Tab_2 = new packet07Tab_2;
	p07Tab_2->Create(IDD_DLG_SEND_10, &m_ras_tab);
	p07Tab_2->MoveWindow(0, 25, rect.Width(), rect.Height());
	p07Tab_2->ShowWindow(SW_HIDE);

	p09Tab_2 = new packet09Tab_2;
	p09Tab_2->Create(IDD_DLG_SEND_11, &m_ras_tab);
	p09Tab_2->MoveWindow(0, 25, rect.Width(), rect.Height());
	p09Tab_2->ShowWindow(SW_HIDE);

	p10Tab_2 = new packet10Tab_2;
	p10Tab_2->Create(IDD_DLG_SEND_12, &m_ras_tab);
	p10Tab_2->MoveWindow(0, 25, rect.Width(), rect.Height());
	p10Tab_2->ShowWindow(SW_HIDE);


	m_auto_sa.SetCheck(BST_CHECKED);
	m_show_alive.SetCheck(BST_CHECKED);

	myPath.Format(_T("%s\\Config.ini"), Utility::GetExePath(g_criticalExe_2));
	SPPath.Format(_T("%s\\JP, SP ini\\SP\\"), Utility::GetExePath(g_criticalExe_2));

	//INIWriteString("CONFIG", "TS ID", "TS01", strFilePath);
	//INIWriteString("CONFIG", "IP", "192.168.0.1", strFilePath);
	//INIWriteString("CONFIG", "PORT", "7911", strFilePath);

	CString tsId = Utility::INIReadString("TS-02", "TS ID", myPath);
	CString tsIp = Utility::INIReadString("TS-02", "IP", myPath);
	CString tsPort = Utility::INIReadString("TS-02", "PORT", myPath);
	CString obIp = Utility::INIReadString("TS-02", "OB IP", myPath);
	CString obPort = Utility::INIReadString("TS-02", "OB PORT", myPath);



	int portInt;
	portInt = _ttoi(tsPort);				// INI 파일로 읽어온 Port번호(CString) int로 변환 

	


	//m_server.Create(portInt, 1, tsIp);		// INI 파일 정보대로 Socket Create
	//m_server.Listen();						// 클라이언트(ATO-OB)의 Connect Listen

	m_cTS_ID.SetWindowTextA(tsId);
	m_cTS_IP.SetWindowTextA(tsIp);
	m_cTS_port.SetWindowTextA(tsPort);
	m_cOB_IP.SetWindowTextA(obIp);
	m_cOB_port.SetWindowTextA(obPort);

	
	CString op = Utility::INIReadString("TS-02", "NID_OPERATIONAL", myPath);
	CString en = Utility::INIReadString("TS-02", "NID_ENGINE", myPath);
	CString ver = Utility::INIReadString("TS-02", "VERSION", myPath);

	m_cOP.SetWindowTextA(op);
	m_cEN.SetWindowTextA(en);
	SetDlgItemInt(IDC_cMAV, _ttoi(ver));

	int column = 0;
	int row = 0;



	m_cListDebug.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT);
	//m_cListDebug.ModifyStyle(0, LVS_NOCOLUMNHEADER);
	m_cListDebug.InsertColumn(column++, "", LVCFMT_CENTER, 1);
	m_cListDebug.InsertColumn(column++, _T("Time"), LVCFMT_CENTER, 100);
	m_cListDebug.InsertColumn(column++, _T("Logs"), LVCFMT_LEFT, 450);

	m_font.CreatePointFont(125, "맑음");
	m_lc_msg.SetFont(&m_font);

	m_lc_msg.ModifyStyle(LVS_OWNERDRAWFIXED, 0, 0);
	m_lc_msg.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_lc_msg.InsertColumn(1, _T("Time"), LVCFMT_CENTER, 100);
	m_lc_msg.InsertColumn(2, _T("Command"), LVCFMT_CENTER, 90);
	m_lc_msg.InsertColumn(3, _T("Type"), LVCFMT_CENTER, 90);
	m_lc_msg.InsertColumn(4, _T("<->"), LVCFMT_CENTER, 80);
	m_lc_msg.InsertColumn(5, _T("Original Binary Data"), LVCFMT_LEFT, 530);
	
#pragma region Decode 테스트

	/*제거됨*/
	
#pragma endregion

	m_strList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT);
	//m_cListDebug.ModifyStyle(0, LVS_NOCOLUMNHEADER);


	m_strList.ModifyStyle(LVS_OWNERDRAWFIXED, 0, 0);
	m_strList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_strList.InsertColumn(1, _T("Category"), LVCFMT_CENTER, 118);
	m_strList.InsertColumn(2, _T("Value"), LVCFMT_CENTER, 244);

	m_strList.InsertItem(1, _T("Indicators State"));
	m_strList.InsertItem(2, _T("Timing Point ID"));
	m_strList.InsertItem(3, _T("Arrival Time"));
	m_strList.InsertItem(4, _T("Current Speed"));



	//Recv Porpertygrid

	m_cmb_recv.AddString("Receive : HandShake Request Packet");
	m_cmb_recv.AddString("Receive : Journey Profile Request Packet");
	m_cmb_recv.AddString("Receive : Journey Profile Acknowledgement Packet");
	m_cmb_recv.AddString("Receive : Segment Profile Request Packet");
	m_cmb_recv.AddString("Receive : Status Report Packet");
	m_cmb_recv.AddString("Receive : Session Termination Packet");
	m_cmb_recv.SetCurSel(0);


	InitPropCtrl();

	return TRUE;
}

BOOL CTabDlg2::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(255, 255, 255));

	return TRUE;
}


HBRUSH CTabDlg2::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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







void CTabDlg2::OnBnClickedBtnInit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// INI 파일 경로 가져오기
	
	// INI 파일의 정보 읽어오기
	CString tsId = Utility::INIReadString("TS-02", "TS ID", myPath);
	CString tsIp = Utility::INIReadString("TS-02", "IP", myPath);
	CString tsPort = Utility::INIReadString("TS-02", "PORT", myPath);
	CString obIp = Utility::INIReadString("TS-02", "OB IP", myPath);
	CString obPort = Utility::INIReadString("TS-02", "OB PORT", myPath);

	// 컨트롤에 INT파일 정보 뿌려주기
	m_cTS_ID.SetWindowTextA(tsId);
	m_cTS_IP.SetWindowTextA(tsIp);
	m_cTS_port.SetWindowTextA(tsPort);
	m_cOB_IP.SetWindowTextA(obIp);
	m_cOB_port.SetWindowTextA(obPort);

}


void CTabDlg2::OnBnClickedBtnApply()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// INI 파일 경로 가져오기
	

	CString newIp;
	CString newPort;
	CString newID;
	CString newOBIp;
	CString newOBPort;

	// 컨트롤에 작성된 정보 가져오기
	m_cTS_ID.GetWindowTextA(newID);
	m_cTS_IP.GetWindowTextA(newIp);
	m_cTS_port.GetWindowTextA(newPort);
	m_cOB_IP.GetWindowTextA(newOBIp);
	m_cOB_port.GetWindowTextA(newOBPort);

	// INI 파일 덮어쓰기
	Utility::INIWriteString("TS-02", "TS ID", newID, myPath);
	Utility::INIWriteString("TS-02", "IP", newIp, myPath);
	Utility::INIWriteString("TS-02", "PORT", newPort, myPath);
	Utility::INIWriteString("TS-02", "OB IP", newOBIp, myPath);
	Utility::INIWriteString("TS-02", "OB PORT", newOBPort, myPath);

	// 새로운 IP, PORT 적용 시 자동으로 소켓 종료 후 다시 Create
	//m_server.Close();

	//int newportInt;
	//newportInt = _ttoi(newPort);

	//m_server.Create(newportInt, 1, newIp);
	//m_server.Listen();

}
void CTabDlg2::Parse_Cmd_TS(BYTE* buf, int len)
{
	BOOL	isRight = TRUE;
	int		nCurrSeq = 0;
	SA_MESSAGE_STRUCTURE msg_struct;
	int		nSize = 0;
	int		nIndex = -1;
	int		nLength = 0;
	int			msgType;
	UINT32		offset = 0;
	CString		csCommand = "Unknown";;
	CString		csType;


	//BYTE    chkID;
	//BYTE	buffer[1023];
	int		nPos = 0;
	unsigned int nCRC32 = 0;
#if(_THROUGHPUT_TEST_)
	int		nRecvIndex = 0;
#endif

	memcpy((void*)&msg_struct, buf, len);

	if (len < 10) {
		Debug("[recieved buffer Error] length is too short(%d)", len);
		MessageLog(_T("Error"), csCommand, _T("length"), buf, len);
		return;
	}

	// header copy
	nSize = sizeof(SA_MSG_COMMON_HEADER);
	memcpy((void*)&msg_struct.header, buf, nSize);

	// header check
	if (msg_struct.header.stx != 0x02) {
		Debug("[recieved buffer Error] stx value is wrong(0x%x)", msg_struct.header.stx);
		MessageLog(_T("Error"), csCommand, _T("stx"), buf, len);
		return;
	}

	nLength = KR37_GetLength(msg_struct.header.length);

	if (nLength != len) {
		Debug("[recieved buffer Error] length is not same(header.len=%d, received buf len=%d)", msg_struct.header.length, len);
		MessageLog(_T("Error"), csCommand, _T("length"), buf, len);
		return;
	}




	// 체크박스 체크상태로 Opcode 케이스 변경 필요

	// 단독 STU 통신
	if (m_checkindi.GetCheck() == BST_CHECKED) {

		switch (msg_struct.header.opcode) {
		case OPCODE_CONNECT_INDICATION:



			if (m_nTS_ID != KR37_ByteToInt(msg_struct.body.connect_indi.responder.etcs_id))
			{
				MessageLog_SC(_T("Error"), _T("ETCS ID"), buf, len);
				break;
			}
			//INDICATION에는 SessionID 포함되어있음
			memcpy(m_SessID_2, msg_struct.body.connect_indi.sacepid, sizeof(m_SessID_2));

			m_nETCS_ID = msg_struct.body.connect_indi.initiator.etcs_id[2];
			MessageLog_SC(_T("Receive"), _T("Conn.indi"), buf, len);
			//nIndex = SessionSet(msg_struct.body.connect_indi.sacepid, msg_struct.body.connect_indi.initiator.etcs_id);
			//검수 오류

			nIndex = SetSessionStatus(1, msg_struct.body.connect_indi.initiator.etcs_id, msg_struct.body.connect_indi.sacepid, STU_STATUS_CONNECTING);
			Debug("We received Connect.indi (index=%d)", nIndex);

			if (m_auto_sa.GetCheck() == BST_CHECKED)	// 자동 SA-CONNECT
			{
				Cmd_Send(1);
				m_pic_onoff.SetBitmap(imgConn);
				SetDlgItemInt(IDC_ETCS_ID, m_nETCS_ID);
				GetDlgItem(IDC_BUTTON_DISCON)->EnableWindow(TRUE);
				m_saConComplete = TRUE;
			}

			else
			{
				GetDlgItem(IDC_BUTTON_CON)->EnableWindow(TRUE);
			}



			break;
		case OPCODE_DATA_INDICATION:
			// BYTE Array => Header => NID_PACKET_ATO 
			csCommand = _T("Data.req");



			// 헤더만 Decode 후 NID_PACET_ATO 구하기
			DecodeMessage_ATOHeader(msg_struct.body.data.data, LEN_KTCS2_HEADER, offset, pHeaderStructure);
			msgType = pHeaderStructure->NID_PACKET_ATO;

			// Type 별 Log
			if (msgType == aHSReq)
			{
				csType = _T("HS Req");
				MessageLog(_T("Receive"), csCommand, csType, buf, len);
				DecodeMessage_ATO_Packet00_HsReq(pHSReqStructure, offset, msg_struct.body.data.data, sizeof(msg_struct.body.data.data));

				//자동 전송일 경우 
				if (p01Tab_2->m_chb_auto.GetCheck() == BST_CHECKED)
				{
					bool anyMatch = false;

					int myVersion = GetDlgItemInt(IDC_cMAV);

					// 지원 Version 목록에 있는지 확인 
					for (int i = 0; i < pHSReqStructure->N_ITER; ++i) {
						if (pHSReqStructure->M_ATO_Version[i] == myVersion) {
							anyMatch = true;
							// HS Ack
							p01Tab_2->PostMessage(WM_COMMAND, MAKEWPARAM(IDC_BTN_SEND, BN_CLICKED), (LPARAM)GetDlgItem(IDC_BTN_SEND)->GetSafeHwnd());
							break; // 찾으면 break;
						}
					}

					// 목록에 없음
					if (!anyMatch) {
						// HS Rej
						CMFCPropertyGridProperty* tempGrid;
						CMFCPropertyGridProperty* currentProperty;
						// 0 : 시스템 비호환 
						tempGrid = p02Tab_2->m_propgrid.GetProperty(BODY_DATA);
						currentProperty = tempGrid->GetSubItem(IDX_Q_Reject_Reason);
						Utility::SetValueWithTypeCasting(currentProperty, 0);

						p02Tab_2->PostMessage(WM_COMMAND, MAKEWPARAM(IDC_BTN_SEND, BN_CLICKED), (LPARAM)GetDlgItem(IDC_BTN_SEND)->GetSafeHwnd());
					}
				}

			}

			else if (msgType == aJPReq)
			{
				csType = _T("JP Req");
				MessageLog(_T("Receive"), csCommand, csType, buf, len);
				DecodeMessage_ATO_Packet03_JpReq(pJPReqStructure, offset, msg_struct.body.data.data, sizeof(msg_struct.body.data.data));

				//자동 전송일 경우
				if (p04Tab_2->m_chb_auto.GetCheck() == BST_CHECKED)
				{
					p04Tab_2->PostMessage(WM_COMMAND, MAKEWPARAM(IDC_BTN_SEND, BN_CLICKED), (LPARAM)GetDlgItem(IDC_BTN_SEND)->GetSafeHwnd());
				}
			}

			else if (msgType == aJPAck)
			{
				csType = _T("JP Ack");
				MessageLog(_T("Receive"), csCommand, csType, buf, len);
				DecodeMessage_ATO_Packet05_JpAck(pJPAckStructure, offset, msg_struct.body.data.data, sizeof(msg_struct.body.data.data));
			}

			else if (msgType == aSPReq)
			{
				csType = _T("SP Req");
				MessageLog(_T("Receive"), csCommand, csType, buf, len);
				DecodeMessage_ATO_Packet06_SpReq(pSPReqStructure, offset, msg_struct.body.data.data, sizeof(msg_struct.body.data.data));

				//자동 전송일 경우
				if (p07Tab_2->m_chb_auto.GetCheck() == BST_CHECKED && pSPReqStructure->N_ITER_ATO > 0)
				{
					for (int i = 0; i < static_cast<int>(pSPReqStructure->N_ITER_ATO); i++) {
						int index = 0;
						std::unordered_map<int, int> spMap = {
							{ 4097, 101 }, { 4098, 102 }, { 4099, 103 }, { 4100, 104 },
							{ 8193, 201 }, { 8194, 202 }, { 8195, 203 }, { 8196, 204 }
						};
						auto it = spMap.find(pSPReqStructure->SPReqDetails[i].NID_SP);
						if (it != spMap.end()) {
							index = it->second;
							p07Tab_2->RequestedLoadSPFromFile(index, SPPath); // Load
							p07Tab_2->PostMessage(WM_COMMAND, MAKEWPARAM(IDC_BTN_SEND, BN_CLICKED), (LPARAM)GetDlgItem(IDC_BTN_SEND)->GetSafeHwnd()); // send
						}
					}
				}
			}

			else if (msgType == aSTR)
			{
				csType = _T("STR");
				MessageLog(_T("Receive"), csCommand, csType, buf, len);
				DecodeMessage_ATO_Packet08_STR(pSTRStructure, offset, msg_struct.body.data.data, sizeof(msg_struct.body.data.data));
				SetCtrl_ATOstatus((ImgATOstatus)pSTRStructure->M_ATO_State);
				SetCtrl_TPinformation();

				//자동 전송일 경우
				if (p09Tab_2->m_chb_auto.GetCheck() == BST_CHECKED)
				{
					p09Tab_2->PostMessage(WM_COMMAND, MAKEWPARAM(IDC_BTN_SEND, BN_CLICKED), (LPARAM)GetDlgItem(IDC_BTN_SEND)->GetSafeHwnd());
				}
			}

			else if (msgType == aSeTerm)
			{
				csType = _T("Sess Term");
				MessageLog(_T("Receive"), csCommand, csType, buf, len);
				DecodeMessage_ATOHeader(msg_struct.body.data.data, sizeof(msg_struct.body.data.data), offset, pHeaderStructure);
			}

			else
			{

			}

			break;
			//KVC Alive 안받음
		//case OPCODE_KVC_ALIVE:			// 통신 테스트용 KVC ALIVE
		//	//Debug("KVC ALIVE Processing", nIndex);
		//	if (m_show_alive.GetCheck() == BST_CHECKED)
		//	{
		//		MessageLog_SC(_T("Receive"), _T("KVC.Alive"), buf, len);
		//	}
		//	this->SetDlgItemInt(IDC_RECV_ALIVE_COUNTER, rcnt);
		//	rcnt++;


		//	break;
			// 이 부분은 Cmd_Send(2) 말고 자체 세션 종료 처리
		case OPCODE_DISCONNECT_INDICATION: // Disconnect req 처리 (!= finish)

			MessageLog_SC(_T("Receive"), _T("Disc.indi"), buf, len);
			m_saConComplete = FALSE;
			m_pic_onoff.SetBitmap(imgDisConn);
			SetDlgItemText(IDC_ETCS_ID, _T("Unknown"));

			GetDlgItem(IDC_BUTTON_CON)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_DISCON)->EnableWindow(FALSE);

			m_nSequence = 0;


			memset(m_SessID_2, 0, sizeof(m_SessID_2)); // 모든 요소를 0으로 초기화

			break;
		case OPCODE_SCM_ALIVE:
			if (m_show_alive.GetCheck() == BST_CHECKED)
			{
				MessageLog_SC(_T("Receive"), _T("STU.Alive"), buf, len);
			}
			this->SetDlgItemInt(IDC_RECV_ALIVE_COUNTER, rcnt);
			rcnt++;
			break;
		default:
			//	MessageLog(_T("Recv"), _T("Default"), buf, len);

			Debug("Command(opCode=%d) Error", msg_struct.header.opcode);
			MessageLog(_T("Error"), csCommand, _T("opcode"), buf, len);
			return;

		}
	}
	//기존 Opcode
	else {
		switch (msg_struct.header.opcode) {
		case OPCODE_CONNECT_REQUEST:



			if (m_nTS_ID != KR37_ByteToInt(msg_struct.body.connect_req.responder.etcs_id))
			{
				/*TRACE1(_T("DEBUG >> 1: %d\n"), (int)msg_struct.body.connect_req.responder.etcs_id[0]);
				TRACE1(_T("DEBUG >> 2: %d\n"), (int)msg_struct.body.connect_req.responder.etcs_id[1]);
				TRACE1(_T("DEBUG >> 3: %d\n"), (int)msg_struct.body.connect_req.responder.etcs_id[2]);*/

				MessageLog_SC(_T("Error"), _T("ETCS ID"), buf, len);
				break;
			}

			m_nETCS_ID = msg_struct.body.connect_req.initiator.etcs_id[2];

			MessageLog_SC(_T("Receive"), _T("Conn.req"), buf, len);

			//nIndex = SessionSet(msg_struct.body.connect_indi.sacepid, msg_struct.body.connect_indi.initiator.etcs_id);

			//검수 오류




			nIndex = SetSessionStatus(1, msg_struct.body.connect_indi.initiator.etcs_id, msg_struct.body.connect_indi.sacepid, STU_STATUS_CONNECTING);





			Debug("We received Connect.req (index=%d)", nIndex);

			if (m_auto_sa.GetCheck() == BST_CHECKED)	// 자동 SA-CONNECT
			{
				Cmd_Send(1);
				m_pic_onoff.SetBitmap(imgConn);
				SetDlgItemInt(IDC_ETCS_ID, m_nETCS_ID);
				GetDlgItem(IDC_BUTTON_DISCON)->EnableWindow(TRUE);
				m_saConComplete = TRUE;
			}

			else
			{
				GetDlgItem(IDC_BUTTON_CON)->EnableWindow(TRUE);
			}



			break;
		case OPCODE_DATA_REQUEST:
			// BYTE Array => Header => NID_PACKET_ATO 
			csCommand = _T("Data.req");



			// 헤더만 Decode 후 NID_PACET_ATO 구하기
			DecodeMessage_ATOHeader(msg_struct.body.data.data, LEN_KTCS2_HEADER, offset, pHeaderStructure);
			msgType = pHeaderStructure->NID_PACKET_ATO;

			// Type 별 Log
			if (msgType == aHSReq)
			{
				csType = _T("HS Req");
				MessageLog(_T("Receive"), csCommand, csType, buf, len);
				DecodeMessage_ATO_Packet00_HsReq(pHSReqStructure, offset, msg_struct.body.data.data, sizeof(msg_struct.body.data.data));

				//자동 전송일 경우 
				if (p01Tab_2->m_chb_auto.GetCheck() == BST_CHECKED)
				{
					bool anyMatch = false;

					int myVersion = GetDlgItemInt(IDC_cMAV);

					// 지원 Version 목록에 있는지 확인 
					for (int i = 0; i < pHSReqStructure->N_ITER; ++i) {
						if (pHSReqStructure->M_ATO_Version[i] == myVersion) {
							anyMatch = true;
							// HS Ack
							p01Tab_2->PostMessage(WM_COMMAND, MAKEWPARAM(IDC_BTN_SEND, BN_CLICKED), (LPARAM)GetDlgItem(IDC_BTN_SEND)->GetSafeHwnd());
							break; // 찾으면 break;
						}
					}

					// 목록에 없음
					if (!anyMatch) {
						// HS Rej
						CMFCPropertyGridProperty* tempGrid;
						CMFCPropertyGridProperty* currentProperty;
						// 0 : 시스템 비호환 
						tempGrid = p02Tab_2->m_propgrid.GetProperty(BODY_DATA);
						currentProperty = tempGrid->GetSubItem(IDX_Q_Reject_Reason);
						Utility::SetValueWithTypeCasting(currentProperty, 0);

						p02Tab_2->PostMessage(WM_COMMAND, MAKEWPARAM(IDC_BTN_SEND, BN_CLICKED), (LPARAM)GetDlgItem(IDC_BTN_SEND)->GetSafeHwnd());
					}
				}

			}

			else if (msgType == aJPReq)
			{
				csType = _T("JP Req");
				MessageLog(_T("Receive"), csCommand, csType, buf, len);
				DecodeMessage_ATO_Packet03_JpReq(pJPReqStructure, offset, msg_struct.body.data.data, sizeof(msg_struct.body.data.data));

				//자동 전송일 경우
				if (p04Tab_2->m_chb_auto.GetCheck() == BST_CHECKED)
				{
					p04Tab_2->PostMessage(WM_COMMAND, MAKEWPARAM(IDC_BTN_SEND, BN_CLICKED), (LPARAM)GetDlgItem(IDC_BTN_SEND)->GetSafeHwnd());
				}
			}

			else if (msgType == aJPAck)
			{
				csType = _T("JP Ack");
				MessageLog(_T("Receive"), csCommand, csType, buf, len);
				DecodeMessage_ATO_Packet05_JpAck(pJPAckStructure, offset, msg_struct.body.data.data, sizeof(msg_struct.body.data.data));
			}

			else if (msgType == aSPReq)
			{
				csType = _T("SP Req");
				MessageLog(_T("Receive"), csCommand, csType, buf, len);
				DecodeMessage_ATO_Packet06_SpReq(pSPReqStructure, offset, msg_struct.body.data.data, sizeof(msg_struct.body.data.data));

				//자동 전송일 경우
				if (p07Tab_2->m_chb_auto.GetCheck() == BST_CHECKED && pSPReqStructure->N_ITER_ATO > 0)
				{
					for (int i = 0; i < static_cast<int>(pSPReqStructure->N_ITER_ATO); i++) {
						int index = 0;
						std::unordered_map<int, int> spMap = {
							{ 4097, 101 }, { 4098, 102 }, { 4099, 103 }, { 4100, 104 },
							{ 8193, 201 }, { 8194, 202 }, { 8195, 203 }, { 8196, 204 }
						};
						auto it = spMap.find(pSPReqStructure->SPReqDetails[i].NID_SP);
						if (it != spMap.end()) {
							index = it->second;
							p07Tab_2->RequestedLoadSPFromFile(index, SPPath); // Load
							p07Tab_2->PostMessage(WM_COMMAND, MAKEWPARAM(IDC_BTN_SEND, BN_CLICKED), (LPARAM)GetDlgItem(IDC_BTN_SEND)->GetSafeHwnd()); // send
						}
					}
				}
			}

			else if (msgType == aSTR)
			{
				csType = _T("STR");
				MessageLog(_T("Receive"), csCommand, csType, buf, len);
				DecodeMessage_ATO_Packet08_STR(pSTRStructure, offset, msg_struct.body.data.data, sizeof(msg_struct.body.data.data));
				SetCtrl_ATOstatus((ImgATOstatus)pSTRStructure->M_ATO_State);
				SetCtrl_TPinformation();

				//자동 전송일 경우
				if (p09Tab_2->m_chb_auto.GetCheck() == BST_CHECKED)
				{
					p09Tab_2->PostMessage(WM_COMMAND, MAKEWPARAM(IDC_BTN_SEND, BN_CLICKED), (LPARAM)GetDlgItem(IDC_BTN_SEND)->GetSafeHwnd());
				}
			}

			else if (msgType == aSeTerm)
			{
				csType = _T("Sess Term");
				MessageLog(_T("Receive"), csCommand, csType, buf, len);
				DecodeMessage_ATOHeader(msg_struct.body.data.data, sizeof(msg_struct.body.data.data), offset, pHeaderStructure);
			}

			else
			{

			}

			break;
		case OPCODE_KVC_ALIVE:			// 통신 테스트용 KVC ALIVE
			//Debug("KVC ALIVE Processing", nIndex);
			if (m_show_alive.GetCheck() == BST_CHECKED)
			{
				MessageLog_SC(_T("Receive"), _T("KVC.Alive"), buf, len);
			}
			this->SetDlgItemInt(IDC_RECV_ALIVE_COUNTER, rcnt);
			rcnt++;


			break;
		case OPCODE_DISCONNECT_REQUEST:
			// Disconnect req 처리 (!= finish)

			MessageLog_SC(_T("Receive"), _T("Disc.req"), buf, len);
			if (m_auto_sa.GetCheck() == BST_CHECKED)	// 자동 SA-CONNECT
			{
				Cmd_Send(2);
				m_pic_onoff.SetBitmap(imgDisConn);
				SetDlgItemText(IDC_ETCS_ID, _T("Unknown"));
			}

			break;
		case OPCODE_SCM_ALIVE:
			break;
		default:
			//	MessageLog(_T("Recv"), _T("Default"), buf, len);

			Debug("Command(opCode=%d) Error", msg_struct.header.opcode);
			MessageLog(_T("Error"), csCommand, _T("opcode"), buf, len);
			return;

		}

	}

}


int CTabDlg2::KR37_GetLength(BYTE* bID)
{
	int retValue = 0;

#if FEATURE_KR37_LITTLE_ENDIAN	// little endian
	retValue = (((int)bID[1] & 0xFF) << 8) + (((int)bID[0] & 0xFF));
#else	// big endian
	retValue = (((int)bID[0] & 0xFF) << 8) + (((int)bID[1] & 0xFF));
#endif

	return retValue;
}



void CTabDlg2::MessageLog(CString inout, CString cmd, CString type, CString origin_msg)
{
	int nColumn = 0;
	int	nIndex = m_lc_msg.GetItemCount();
	m_lc_msg.InsertItem(nIndex, "");
	m_lc_msg.SetItemText(nIndex, 0, "");
	m_lc_msg.SetItemText(nIndex, nColumn++, Get_CurrentTime());
	m_lc_msg.SetItemText(nIndex, nColumn++, cmd);
	m_lc_msg.SetItemText(nIndex, nColumn++, type);
	m_lc_msg.SetItemText(nIndex, nColumn++, inout);
	m_lc_msg.SetItemText(nIndex, nColumn++, origin_msg);

	if (m_log_pause.GetCheck() == BST_UNCHECKED)
	{
		m_lc_msg.SendMessage(WM_VSCROLL, SB_BOTTOM);
	}
	SendMessage(WM_INSERTPACKET, 100, 200);
}

void CTabDlg2::MessageLog(CString inout, CString cmd, CString type, BYTE* buffer, int len)
{
	CString csStx, csLen, csSeq, csOp, csBody;
	CString  hexPrint;

	if (len < 6) {
		Debug("The size of %s message is too short, Len(%d)", len);
		
	}
	/*
	csStx.Format(_T("%02x "), buffer[0]);
	csLen.Format(_T("%02x %02x"), buffer[1], buffer[2]);
	csSeq.Format(_T("%02x "), buffer[3]);
	csOp.Format(_T("%02x "), buffer[4]);
	*/
	for (int i = 0; i < len; i++)
	{
		CString csTmp;
		csTmp.Format(_T("%02x "), buffer[i]);
		csBody += csTmp;
	}

	MessageLog(inout, cmd, type, csBody);
}

void CTabDlg2::MessageLog_SC(CString inout, CString cmd, CString origin_msg)
{
	int nColumn = 0;
	int	nIndex = m_lc_msg.GetItemCount();
	m_lc_msg.InsertItem(nIndex, "");
	m_lc_msg.SetItemText(nIndex, 0, "");
	m_lc_msg.SetItemText(nIndex, nColumn++, Get_CurrentTime());
	m_lc_msg.SetItemText(nIndex, nColumn++, cmd);
	m_lc_msg.SetItemText(nIndex, nColumn++, _T("Safety"));
	m_lc_msg.SetItemText(nIndex, nColumn++, inout);
	m_lc_msg.SetItemText(nIndex, nColumn++, origin_msg);

	if (m_log_pause.GetCheck() == BST_UNCHECKED)
	{
		m_lc_msg.SendMessage(WM_VSCROLL, SB_BOTTOM);
	}

	SendMessage(WM_INSERTPACKET, 100, 200);
}

void CTabDlg2::MessageLog_SC(CString inout, CString cmd, BYTE* buffer, int len)
{
	CString csStx, csLen, csSeq, csOp, csBody;
	CString  hexPrint;

	if (len < 6) {
		Debug("The size of %s message is too short, Len(%d)", len);
		
	}
	/*
	csStx.Format(_T("%02x "), buffer[0]);
	csLen.Format(_T("%02x %02x"), buffer[1], buffer[2]);
	csSeq.Format(_T("%02x "), buffer[3]);
	csOp.Format(_T("%02x "), buffer[4]);
	*/
	for (int i = 0; i < len; i++)
	{
		CString csTmp;
		csTmp.Format(_T("%02x "), buffer[i]);
		csBody += csTmp;
	}

	MessageLog_SC(inout, cmd, csBody);
}

void CTabDlg2::SetSessionStatus(int index, BYTE state)
{
	SetSessionStatus(index, 0, 0, state);
}

void CTabDlg2::propgrid()
{
	CRect rect;

	m_propgrid.EnableHeaderCtrl(TRUE, _T("Item"), _T("Value"));
	m_propgrid.GetWindowRect(&rect);
	m_propgrid.PostMessage(WM_SIZE, 0, MAKELONG(rect.Width() - 50, rect.Height()));

	CMFCPropertyGridProperty* pxpgp;
	CMFCPropertyEX<UINT32>* pxu;

	CMFCPropertyGridProperty* pxProperty;
	pxProperty = new CMFCPropertyGridProperty(_T("HEADER Data"));
	m_propgrid.AddProperty(pxProperty);

	pxpgp = new CMFCPropertyGridProperty(_T("NID_PACKET_ATO"), (variant_t)(int)0, _T("패킷 넘버"));
	pxpgp->AllowEdit(FALSE);
	pxProperty->AddSubItem(pxpgp);

	pxu = new CMFCPropertyEX<UINT32>(_T("NID_OPERATIONAL"), (variant_t)(UINT32)0, _T("Train Running Number"));
	pxu->SetFormat("%u");
	pxu->SetMinMaxValue(0, UINT32_MAX);
	pxu->AllowEdit(FALSE);
	pxProperty->AddSubItem(pxu);

	pxpgp = new CMFCPropertyGridProperty(_T("NID_ENGINE"), (variant_t)(int)0, _T("ETCS Identify"));
	pxpgp->AllowEdit(FALSE);
	pxProperty->AddSubItem(pxpgp);

	pxpgp = new CMFCPropertyGridProperty(_T("T_TimeStamp_Date"), (variant_t)(int)0, _T("Timestamp in date since start-up"));
	pxpgp->AllowEdit(FALSE);
	pxProperty->AddSubItem(pxpgp);

	pxpgp = new CMFCPropertyGridProperty(_T("T_TimeStamp_Seconds"), (variant_t)(int)0, _T("Timestamp in milliseconds since start-up"));
	pxpgp->AllowEdit(FALSE);
	pxProperty->AddSubItem(pxpgp);

	pxpgp = new CMFCPropertyGridProperty(_T("N_Packet_Counter"), (variant_t)(int)0, _T("보내진 패킷 Counter"));
	pxpgp->AllowEdit(FALSE);
	pxProperty->AddSubItem(pxpgp);


	pxProperty = new CMFCPropertyGridProperty(_T("BODY Data"));
	m_propgrid.AddProperty(pxProperty);


	CMFCPropertyGridProperty* pxBodyProperty = m_propgrid.GetProperty((int)LogProperty::BODY);

	switch (m_msgType)
	{

	case aHSReq:
		PropertyCtrlBuilder::BuildBodyPkHSReq(pxBodyProperty);
		break;
	case aJPReq:
		PropertyCtrlBuilder::BuildBodyPkJPReq(pxBodyProperty);
		break;
	case aJPAck:
		PropertyCtrlBuilder::BuildBodyPkJPAck(pxBodyProperty);
		break;
	case aSPReq:
		PropertyCtrlBuilder::BuildBodyPkSPReq(pxBodyProperty);
		break;
	case aSTR:
		PropertyCtrlBuilder::BuildBodyPkSTR(pxBodyProperty);
		break;
	case aSeTerm:
		PropertyCtrlBuilder::BuildBodyPkSessTerm(pxBodyProperty);
		break;
	default: break;
	}
}

void CTabDlg2::chgCmb(int index)
{
	switch (index)
	{
	case aHSReq:					// NID_PACKET_ATO : 0 = HandShake Req
		m_cmb_recv.SetCurSel(0);
		
		break;
	case aJPReq:					// NID_PACKET_ATO : 3 = Journey Profile Req
		m_cmb_recv.SetCurSel(1);
		
		break;
	case aJPAck:					// NID_PACKET_ATO : 5 = Journey Profile Ack
		m_cmb_recv.SetCurSel(2);
	
		break;
	case aSPReq:					// NID_PACKET_ATO : 6 = Segment Profile Req
		m_cmb_recv.SetCurSel(3);
		
		break;
	case aSTR:					// NID_PACKET_ATO : 8 = Status Report
		m_cmb_recv.SetCurSel(4);
		
		break;
	case aSeTerm:					// NID_PACKET_ATO : 11 = Session Termination
		m_cmb_recv.SetCurSel(5);
		
		break;
	}
}

void CTabDlg2::OnCbnSelchangeCmbRecv()
{
	m_propgrid.RemoveAll();
	propgrid();

}

void CTabDlg2::Release_Ctrl()
{
	
}

void CTabDlg2::updtPacket(_In_ const byte* arr, _In_ const size_t length)
{

	//if (length < LEN_ALL_HEADER) return;
	ATO_Header header{};

	UINT32 offset = (sizeof(SA_MSG_COMMON_HEADER) + BYTES_OF_SESSID) * BITS_OF_ONEBYTE;

	DecodeMessage_ATOHeader(arr, (UINT32)length, offset, &header);
	PropertyCtrlBuilder::PacketHeaderStructureToCtrl(header, m_propgrid.GetProperty((int)LogProperty::HEADER));

	switch (m_msgType)
	{

	case aHSReq:
	{
		OB_TS_Packet00_HSReqStruct p{};
		DecodeMessage_ATO_Packet00_HsReq(&p, offset, arr, (UINT32)length);
		PropertyCtrlBuilder::PacketHSReqStructureToCtrl(p, m_propgrid.GetProperty((int)LogProperty::BODY));
	}
	break;
	case aJPReq:
	{
		OB_TS_Packet03_JPReqStruct p{};
		DecodeMessage_ATO_Packet03_JpReq(&p, offset, arr, (UINT32)length);
		PropertyCtrlBuilder::PacketJPReqStructureToCtrl(p, m_propgrid.GetProperty((int)LogProperty::BODY));
	}
	break;
	case aJPAck:
	{
		OB_TS_Packet05_JPAckStruct p{};
		DecodeMessage_ATO_Packet05_JpAck(&p, offset, arr, (UINT32)length);
		PropertyCtrlBuilder::PacketJPAckStructureToCtrl(p, m_propgrid.GetProperty((int)LogProperty::BODY));
	}
	break;
	case aSPReq:
	{
		OB_TS_Packet06_SPReqStruct p{};
		DecodeMessage_ATO_Packet06_SpReq(&p, offset, arr, (UINT32)length);
		PropertyCtrlBuilder::PacketSPReqStructureToCtrl(p, m_propgrid.GetProperty((int)LogProperty::BODY));
	}
	break;
	case aSTR:
	{
		OB_TS_Packet08_STRStruct p{};
		DecodeMessage_ATO_Packet08_STR(&p, offset, arr, (UINT32)length);
		PropertyCtrlBuilder::PacketSTRStructureToCtrl(p, m_propgrid.GetProperty((int)LogProperty::BODY));
	}
	break;
	case aSeTerm:
	{
		OB_TS_Packet11_SESSTermStruct p{};
		DecodeMessage_ATO_Packet11_SessTerm(&p, offset, arr, (UINT32)length);
		PropertyCtrlBuilder::PacketSessTermStructureToCtrl(p, m_propgrid.GetProperty((int)LogProperty::BODY));
	}
	break;
	default: break;
	}
}

void CTabDlg2::SetCtrl_ATOstatus(ImgATOstatus v)
{
	switch (v)
	{
	case ImgATOstatus::Blank:
		m_pic_atostate.SetBitmap(imgBlank);
		break;
	case ImgATOstatus::UN:
		m_pic_atostate.SetBitmap(imgBlank);
		break;
	case ImgATOstatus::CO:
		m_pic_atostate.SetBitmap(imgStatusCO);
		break;
	case ImgATOstatus::NA:
		m_pic_atostate.SetBitmap(imgStatusNA);
		break;
	case ImgATOstatus::AV:
		m_pic_atostate.SetBitmap(imgStatusAV);
		break;
	case ImgATOstatus::RE:
		m_pic_atostate.SetBitmap(imgStatusRE);
		break;
	case ImgATOstatus::EG:
		m_pic_atostate.SetBitmap(imgStatusEG);
		break;
	case ImgATOstatus::DE:
		m_pic_atostate.SetBitmap(imgStatusDE);
		break;
	case ImgATOstatus::FA:
		m_pic_atostate.SetBitmap(imgStatusFA);
		break;
	default:
		break;
	}
}

CString CTabDlg2::SetText_STRIndcators(UINT16 value)
{
	CString str;

	for (int i = 0; i < 8; ++i) {
		// i 비트를 가져오기 위해 (1 << i)와 & 연산을 사용
		if (value & (1 << i)) {
			switch (i)
			{
			case 0:
				str += _T("Consistency Error\n");
				break;
			case 1:
				str += _T("Routing Error\n");
				break;
			case 2:
				str += _T("Next STP Skip\n");
				break;
			case 3:
				str += _T("Low Adhension\n");
				break;
			case 4:
				str += _T("Conditions fullfil\n");
				break;
			case 5:
				str += _T("Train is moving\n");
				break;
			case 6:
				str += _T("Unable to stop\n");
				break;
			case 7:
				str += _T("Slip/slide reported\n");
				break;
			}
		}
	}

	if (str.IsEmpty()) {
		str = _T("None");
	}

	return str;
}

void CTabDlg2::SetCtrl_TPinformation()
{
	if (pSTRStructure != nullptr)
	{
		UINT16 date_bits = pSTRStructure->TPEstimation[0].T_Arrival_Date;
		UINT32 seconds_bits = pSTRStructure->TPEstimation[0].T_Arrival_Seconds;

		CString arrivalTP;
		CString strIndi;

		strIndi = SetText_STRIndcators(pSTRStructure->Q_STR_Indication);
		arrivalTP = Utility::utc_to_local_date_time(date_bits, seconds_bits);


		tpId.Format(_T("%05x"), (int)(pSTRStructure->TPEstimation[0].NID_TP));
		curSpeed.Format(_T("%d"), (int)(pSTRStructure->V_TRAIN_ATO));

		m_strList.SetItemText(0, 1, strIndi);
		m_strList.SetItemText(1, 1, tpId);
		m_strList.SetItemText(2, 1, arrivalTP);
		m_strList.SetItemText(3, 1, curSpeed);
	}
}
void CTabDlg2::SessionSet(BYTE* sessID, BYTE* resp_etcsID)
{
	int nIndex = -1;
	CString csTmp;
	int nSessID = KR37_ByteToInt(sessID);
	int nEtcsID = KR37_ByteToInt(resp_etcsID);

	int nTS_ID = nEtcsID & 0x3FFF;

	csTmp.Format(_T("%d"), nSessID);

	for (int i = NUMBER_OB_ACCEPT; i < NUMBER_MAX_SESSION; i++)
	{
		if (m_nOB_ID[i] == nTS_ID) {
			m_SessID_2[i] = nSessID;
			m_OB[i].SetSessID(nSessID);

			//StatusSetColor(i, STU_STATUS_CONNECTED);

			nIndex = i;
			break;
		}
	}
	return;
}




int CTabDlg2::GetSequence()
{
	m_nSequence++;

	if (m_nSequence > 255)
		m_nSequence = 1;

	return m_nSequence;
}

void CTabDlg2::Cmd_Send(int cmd)
{
	/*

	 cmd

	 0 = Alive

	 1 = Sa-Conn cfm

	 2 = Sa-DisConn indi

	 3 = HS Ack

	 4 = HS Rej

	 5 = JP

	 6 = SP

	 7 = STR ack

	 8 = Sess Term Req

	*/

	SA_MESSAGE_STRUCTURE msg_struct;
	CString		csCommand;

	BYTE	buffer[1024];

	int		buf_len = 0;

	int		nPos = 0;
	int		nSize = 0;
	int     sessNum = 0;

	unsigned int nCRC32 = 0;
	CString nowID;
	CString comm;

	int	nSequence = this->GetSequence();


	sessNum = 0x01;
	



	// 보내기
	msg_struct.header.stx = 0x02;

	switch (cmd)
	{
	case 0:
		msg_struct.header.opcode = OPCODE_SCM_ALIVE;

		//stu_alive.header.stx = 0x02;
		buf_len = sizeof(SA_MSG_COMMON_HEADER) + sizeof(SA_SCM_ALIVE_BODY_) + sizeof(SA_MSG_COMMON_TAIL);
		KR37_MakeLength(msg_struct.header.length, buf_len);
		msg_struct.header.sequence = nSequence;


		nSize = sizeof(SA_MSG_COMMON_HEADER);
		memcpy(buffer + nPos, (void*)&msg_struct.header, nSize);
		nPos += nSize;

		nSize = sizeof(SA_SCM_ALIVE_BODY_);
		msg_struct.body.scm_alive.count = nSequence;
		msg_struct.body.scm_alive.unit = 0x01;
		msg_struct.body.scm_alive.sess_num = sessNum;
		memcpy(buffer + nPos, (void*)&msg_struct.body, nSize);
		nPos += nSize;

		csCommand = _T("STU.Alive");


		break;

	case 1:
		if (m_checkindi.GetCheck() == BST_CHECKED) {
			msg_struct.header.opcode = OPCODE_CONNECT_RESPONSE; // 체크된 경우 사용할 opcode
			comm = "Conn.rsp";
		}
		else {
			msg_struct.header.opcode = OPCODE_CONNECT_CONFIRM; // 기본 opcode
			Utility::makeSessionID(m_SessID_2);				   // 단독 STU 통신 아닐 시 세션ID 직접 생성
			comm = "Conn.cfm";
		}

		buf_len = sizeof(SA_MSG_COMMON_HEADER) + sizeof(SA_CONNECT_RESP_CONF_BODY) + sizeof(SA_MSG_COMMON_TAIL);
		KR37_MakeLength(msg_struct.header.length, buf_len);

		msg_struct.header.sequence = nSequence;

		nSize = sizeof(SA_MSG_COMMON_HEADER);
		memcpy(buffer + nPos, (void*)&msg_struct.header, nSize);
		nPos += nSize;		

		msg_struct.body.connect_resp_conf.responder.etcs_id_type = 0x01;					// ER37_ETCS_ID_RBC;
		KR37_IntToByte(m_nTS_ID, msg_struct.body.connect_resp_conf.responder.etcs_id, 3);	// TS ID
		memcpy(msg_struct.body.connect_resp_conf.sacepid, m_SessID_2, 3);

		nSize = sizeof(SA_CONNECT_RESP_CONF_BODY);
		memcpy(buffer + nPos, (void*)&msg_struct.body, nSize);
		nPos += nSize;

		csCommand = comm;
		break;

	case 2:
		if (m_checkindi.GetCheck() == BST_CHECKED) {
			msg_struct.header.opcode = OPCODE_DISCONNECT_REQUEST; // 체크된 경우 사용할 opcode
			comm = "Disc.req";
		}
		else {
			msg_struct.header.opcode = OPCODE_DISCONNECT_INDICATION; // 기본 opcode
			comm = "Disc.indi";
		}

		buf_len = sizeof(SA_MSG_COMMON_HEADER) + sizeof(SA_DISCONNECT_BODY) + sizeof(SA_MSG_COMMON_TAIL);
		KR37_MakeLength(msg_struct.header.length, buf_len);

		msg_struct.header.sequence = nSequence;

		nSize = sizeof(SA_MSG_COMMON_HEADER);
		memcpy(buffer + nPos, (void*)&msg_struct.header, nSize);
		nPos += nSize;

		memcpy(msg_struct.body.connect_resp_conf.sacepid, m_SessID_2, 3);
		msg_struct.body.disconnect.disconnect_reason = 0;
		msg_struct.body.disconnect.disconnect_sub_reason = 0;

		nSize = sizeof(SA_DISCONNECT_BODY);
		memcpy(buffer + nPos, (void*)&msg_struct.body, nSize);
		nPos += nSize;

		csCommand = comm;
		m_saConComplete = FALSE;
		m_pic_onoff.SetBitmap(imgDisConn);
		SetDlgItemText(IDC_ETCS_ID, _T("Unknown"));

		GetDlgItem(IDC_BUTTON_CON)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DISCON)->EnableWindow(FALSE);

		m_nSequence = 0;


		memset(m_SessID_2, 0, sizeof(m_SessID_2)); // 모든 요소를 0으로 초기화

		break;
	}

	// CRC 추가 11.14

/* crc 계산할 데이터 사이즈 msgLen -crc(4) - stx(1) - etx(1) */
//nCRC32 = KR37_compute_check_sum(buffer + 1, buf_len - 4 - 1 - 1);

/* crc 계산할 데이터 사이즈 msgLen -crc(4) - etx(1) */

	nCRC32 = KR37_compute_check_sum(buffer, buf_len - 4 - 1);

	KR37_IntToByte(nCRC32, msg_struct.tail.crc32, 4);
	msg_struct.tail.etx = 0x03;

	//Debug("CRC32 = %u", nCRC32);

	nSize = sizeof(SA_MSG_COMMON_TAIL);
	memcpy(buffer + nPos, (void*)&msg_struct.tail, nSize);
	nPos += nSize;

	//SetSessionStatus(index, STU_STATUS_CONNECTED);

	SendTo(buffer, nPos);



	// ALIVE 메세지 아니거나
	if (csCommand != _T("STU.Alive"))
	{

		MessageLog_SC(_T("Send"), csCommand, buffer, nPos);
	}

	// ALIVE 메세지인데, 체크 되어있거나
	else if (csCommand == _T("STU.Alive") && m_show_alive.GetCheck() == BST_CHECKED)
	{
		MessageLog_SC(_T("Send"), csCommand, buffer, nPos);
	}



}


void CTabDlg2::Cmd_Send126(int type)
{
	SA_MESSAGE_STRUCTURE msg_struct;
	CString		csCommand;
	CString		csType;
	int			length;

	BYTE	buffer[1024]{};



	int		buf_len = 0;

	UINT32	    bodylength = 0;
	UINT32		nPos = 0;
	UINT32		nSize = 0;
	unsigned int nCRC32 = 0;



	int		nSequence = this->GetSequence();


	/**

	type에 따른 Safe-Message Type 속 Data Body 할당

	**/
	switch (type)
	{

		/**
		*	Handshake Ackknowledgement
		**/
	case eHsAck:

		csType = _T("HS Ack");

		length = GetLengthPacket01_HsAck(p01Tab_2->pHSAckStructure);
		bodylength = length / BITS_OF_ONEBYTE;

		if (length % BITS_OF_ONEBYTE != 0)
		{
			bodylength += sizeof(byte);
		}

		p01Tab_2->pHeaderStructure->L_PACKET_ATO = GetLengthPacket01_HsAck(p01Tab_2->pHSAckStructure);
		Encode_ATOHeader(p01Tab_2->pHeaderStructure, &nPos, msg_struct.body.data.data, sizeof(msg_struct.body.data.data));

		EncodeMessage_ATO_Packet01_HsAck(p01Tab_2->pHSAckStructure, &nPos, msg_struct.body.data.data, sizeof(msg_struct.body.data.data));



		break;


	case eHsRej:

		csType = _T("HS Rej");

		length = GetLengthPacket02_HsRej(p02Tab_2->pHSRejStructure);
		bodylength = length / BITS_OF_ONEBYTE;

		if (length % BITS_OF_ONEBYTE != 0)
		{
			bodylength += sizeof(byte);
		}

		p02Tab_2->pHeaderStructure->L_PACKET_ATO = GetLengthPacket02_HsRej(p02Tab_2->pHSRejStructure);
		Encode_ATOHeader(p02Tab_2->pHeaderStructure, &nPos, msg_struct.body.data.data, sizeof(msg_struct.body.data.data));
		//TRACE1(_T("size of pHeader Structure : %d\n"), (int)sizeof(pSTab->pHeaderStructure));	

		EncodeMessage_ATO_Packet02_HsRej(p02Tab_2->pHSRejStructure, &nPos, msg_struct.body.data.data, sizeof(msg_struct.body.data.data));


		break;
		/**
		*	Journey Profile
		**/
	case eJP:
		csType = _T("JP");

		length = GetLengthPacket04_Jp(p04Tab_2->pJPStructure);
		bodylength = length / BITS_OF_ONEBYTE;

		if (length % BITS_OF_ONEBYTE != 0)
		{
			bodylength += sizeof(byte);
		}

		p04Tab_2->pHeaderStructure->L_PACKET_ATO = GetLengthPacket04_Jp(p04Tab_2->pJPStructure);
		Encode_ATOHeader(p04Tab_2->pHeaderStructure, &nPos, msg_struct.body.data.data, sizeof(msg_struct.body.data.data));

		EncodeMessage_ATO_Packet04_Jp(p04Tab_2->pJPStructure, &nPos, msg_struct.body.data.data, sizeof(msg_struct.body.data.data));

		break;

	case eSP:

		csType = _T("SP");

		length = GetLengthPacket07_Sp(p07Tab_2->pSPStructure);
		bodylength = length / BITS_OF_ONEBYTE;

		if (length % BITS_OF_ONEBYTE != 0)
		{
			bodylength += sizeof(byte);
		}

		p07Tab_2->pHeaderStructure->L_PACKET_ATO = GetLengthPacket07_Sp(p07Tab_2->pSPStructure);
		Encode_ATOHeader(p07Tab_2->pHeaderStructure, &nPos, msg_struct.body.data.data, sizeof(msg_struct.body.data.data));

		EncodeMessage_ATO_Packet07_Sp(p07Tab_2->pSPStructure, &nPos, msg_struct.body.data.data, sizeof(msg_struct.body.data.data));


		break;
	case eSTRAck:

		csType = _T("STR Ack");

		length = GetLengthPacket09_STRAck(p09Tab_2->pSTRAckStructure);
		bodylength = length / BITS_OF_ONEBYTE;

		if (length % BITS_OF_ONEBYTE != 0)
		{
			bodylength += sizeof(byte);
		}

		p09Tab_2->pHeaderStructure->L_PACKET_ATO = GetLengthPacket09_STRAck(p09Tab_2->pSTRAckStructure);
		Encode_ATOHeader(p09Tab_2->pHeaderStructure, &nPos, msg_struct.body.data.data, sizeof(msg_struct.body.data.data));

		EncodeMessage_ATO_Packet09_STRAck(p09Tab_2->pSTRAckStructure, &nPos, msg_struct.body.data.data, sizeof(msg_struct.body.data.data));

		break;
	case eSeTrmReq:

		csType = _T("SessTerm Req");

		bodylength = GetATOHeaderSize() / BITS_OF_ONEBYTE;

		p10Tab_2->pHeaderStructure->L_PACKET_ATO = GetATOHeaderSize();
		Encode_ATOHeader(p10Tab_2->pHeaderStructure, &nPos, msg_struct.body.data.data, sizeof(msg_struct.body.data.data));


		break;
	}

	nPos = 0;

	// header construct
	msg_struct.header.stx = 0x02;
	msg_struct.header.opcode = OPCODE_DATA_INDICATION;
	msg_struct.header.sequence = nSequence;


	// STU Simulator err 
	// calculate full length 
	buf_len = sizeof(SA_MSG_COMMON_HEADER) + BYTES_OF_SESSID + bodylength + sizeof(SA_MSG_COMMON_TAIL);
	KR37_MakeLength(msg_struct.header.length, buf_len);

	// header copy
	nSize = sizeof(SA_MSG_COMMON_HEADER);
	memcpy(buffer + nPos, (void*)&msg_struct.header, nSize);
	nPos += nSize;

	//session id 구조체 저장
	memcpy(msg_struct.body.data.sacepid, m_SessID_2, BYTES_OF_SESSID);	// 3 Bytes

	//session id copy
	memcpy(buffer + nPos, (void*)&msg_struct.body.data.sacepid, BYTES_OF_SESSID);
	nPos += BYTES_OF_SESSID;

	//body copy to array
	nSize = bodylength;
	memcpy(buffer + nPos, (void*)&msg_struct.body.data.data, nSize);
	nPos += nSize;

	nCRC32 = KR37_compute_check_sum(buffer, buf_len - 4 - 1);

	KR37_IntToByte(nCRC32, msg_struct.tail.crc32, 4);
	msg_struct.tail.etx = 0x03;

	//Debug("CRC32 = %u", nCRC32);

	nSize = sizeof(SA_MSG_COMMON_TAIL);
	memcpy(buffer + nPos, (void*)&msg_struct.tail, nSize);
	nPos += nSize;

	//SetSessionStatus(index, STU_STATUS_CONNECTED);

	SendTo(buffer, nPos);

	csCommand = _T("Data.req");

	MessageLog(_T("Send"), csCommand, csType, buffer, nPos);

}
int CTabDlg2::SendTo(BYTE* buf, int len)
{
	int nTmp;

	if (m_SendSocket < 1) {
		m_SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); //이거
	}

	if (m_SendSocket < 1) {
		Debug("Client Socket Creation Failed(%d)", m_SendSocket);
		return -1;
	}


	/*
	sprintf_s(server_ip, 20, "%s", m_csSTU_IP.GetBuffer());

	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(m_nSTU_PORT);
	inet_pton(AF_INET, server_ip, &(ServerAddr.sin_addr));

	*/
	//ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	nTmp = sendto(m_SendSocket, (const char*)buf, len, 0, (SOCKADDR*)&m_sockAddr, sizeof(m_sockAddr));
	if (nTmp == -1) {
		Debug("sendto error reason=%d", GetLastError());
	}
	else {
		//Debug("sendto result(%d) socket(%d)", nTmp, m_SendSocket);
	}
	return nTmp;
}



void CTabDlg2::KR37_MakeLength(BYTE* bID, int length)
{
#ifdef FEATURE_KR37_LITTLE_ENDIAN  // little endian
	bID[1] = (BYTE)((length >> 8) & 0xFF);
	bID[0] = (BYTE)(length & 0xFF);
#else	// big endian
	bID[0] = (BYTE)((length >> 8) & 0xFF);
	bID[1] = (BYTE)(length & 0xFF);
#endif
}

void CTabDlg2::KR37_IntToByte(int nID, BYTE* bID, int size)
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

int CTabDlg2::KR37_ByteToInt(BYTE* bID)
{
	int retValue = 0;

	retValue = (((int)bID[0] & 0xFF) << 16) + (((int)bID[1] & 0xFF) << 8) + (((int)bID[2] & 0xFF));

	return retValue;
}


void CTabDlg2::Debug(CString msg)
{
	//msg.Format(_T("send result=%d"), nTmp);
	//m_cListDebug.AddString(msg);	// list box

	int	nIndex = m_cListDebug.GetItemCount();
	CString ctime;
	ctime = Get_CurrentTime();

#if(_WORKING_UI_)
	m_cListDebug.InsertItem(nIndex, "");
	m_cListDebug.SetItemText(nIndex, 0, "");
	m_cListDebug.SetItemText(nIndex, 1, Get_CurrentTime());
	m_cListDebug.SetItemText(nIndex, 2, msg);
	//m_lb_debug.AddString(ctime + " " + msg);
#else
	m_cListDebug.InsertItem(nIndex, Get_CurrentTime());
	m_cListDebug.SetItemText(nIndex, 1, msg);
#endif


	// Auto Scroll
	//m_cListDebug.SendMessage(WM_VSCROLL, SB_BOTTOM);
	//Invalidate(FALSE);
	//::MessageBox(NULL, msg, _T("Title"), MB_OK);
}

void CTabDlg2::Debug(const char* format, ...)
{
	//int	nIndex = m_cListDebug.GetIteGetItemCount();
	CString csTmp;
	char    Buf[1024];
	va_list l;

	va_start(l, format);
	//csTmp.Format(_T(format), l);
	vsprintf_s(Buf, 1024, (char*)format, l);
	va_end(l);

	csTmp.Format(_T("%s"), Buf);

	Debug(csTmp);
}



CString CTabDlg2::Get_CurrentTime()
{
	//DWORD dwTime = ::GetTickCount();
	//CTime cTime = CTime::GetCurrentTime();

	SYSTEMTIME cur_time;
	GetLocalTime(&cur_time);
	CString csPCTime;

	//strPCTime.Format("%04d%02d%02d%02d%02d%02d%03ld", cur_time.wYear,cur_time.wMonth, cur_time.wDay,
	//	cur_time.wHour,	cur_time.wMinute, cur_time.wSecond,	cur_time.wMilliseconds);

	csPCTime.Format(_T("%02d:%02d:%02d.%03ld"), cur_time.wHour, cur_time.wMinute, cur_time.wSecond, cur_time.wMilliseconds);
	return csPCTime;
}

int CTabDlg2::SetSessionStatus(int index, BYTE* obID, BYTE* sessID, int state)
{
	if (state == STU_STATUS_CONNECTING)			// 새로운 세션
	{
#if(_RBC_RBC_SIMULATOR)
		// RBC-RBC Connect Request
		if (index >= NUMBER_OB_ACCEPT && index < NUMBER_MAX_SESSION)
		{
			m_OB[index].SetStatus(state);
			//StatusSetColor(index, state);
			UPDATE_TO_CTRL();
			return -1;
		}
		// RBC-RBC Connect Indi
		else if (index == 0)
		{
			// 아래 KVC 의 Connect Indi 와 동일하게 행동
		}
#endif

		int nOB_ID = KR37_ByteToInt(obID);
		int nSESS_ID = KR37_ByteToInt(sessID);

		if (nSESS_ID < 1)
		{
			Debug("SetSessionStatus, Error, Session ID(%d)", nSESS_ID);
			return -1;
		}

		for (int i = 0; i < NUMBER_OB_ACCEPT; i++)
		{
			if (m_OB[i].GetID() == 0)
			{
				m_OB[i].SetID(nOB_ID);
				m_OB[i].SetSessID(nSESS_ID);
				m_OB[i].SetStatus(state);

				// UI Set
				m_nOB_ID[i] = nOB_ID;
				m_nSess_ID[i] = nSESS_ID;
				//StatusSetColor(i, state);

				//if(m_OB[i].GetID())

				Debug("SetSessionStatus, new index(%d)", i);

				return i;
			}
		}
		Debug("SetSessionStatus, Error, KVC_ID(%d), SessID(%d)", nOB_ID, nSESS_ID);
		return -1;

	}
	else if (state == STU_STATUS_DISCONNECTING)	// RBC 에서 연결 해제할 경우는 index 값이 넘어옴.
	{
		if (index < 0 || index >= NUMBER_MAX_SESSION)
		{
			Debug("ERROR, index(%d), line(#%d)", index, __LINE__);
			return -1;
		}
		else
		{
			Debug("Result : KVC(%d), Send(%d), Recv(%d)", m_OB[index].GetID(), m_OB[index].GetSendCnt(), m_OB[index].GetRecvCnt());
		}

		m_OB[index].initSession();

		//m_OB[i].SetStatus(nState);
		m_OB[index].SetStatus(STU_STATUS_READY);

		// UI Set
		m_nOB_ID[index] = 0;
		m_nSess_ID[index] = 0;
		//StatusSetColor(i, nState);
		//StatusSetColor(index, STU_STATUS_READY);
	}
	else if (state == STU_STATUS_DISCONNECTED)	// 연결 해제
	{
		int nSESS_ID = KR37_ByteToInt(sessID);
		if (nSESS_ID < 1)
		{
			Debug("SetSessionStatus, Error, Session ID(%d)", nSESS_ID);
			return -1;
		}

		for (int i = 0; i < NUMBER_MAX_SESSION; i++)
		{
			if (m_OB[i].GetSessID() == nSESS_ID)
			{
				Debug("Result : KVC(%d), Send(%d), Recv(%d)", m_OB[i].GetID(), m_OB[i].GetSendCnt(), m_OB[i].GetRecvCnt());

				m_OB[i].initSession();

				//m_OB[i].SetStatus(nState);
				m_OB[i].SetStatus(STU_STATUS_READY);

				// UI Set
				m_nOB_ID[i] = 0;
				m_nSess_ID[i] = 0;
				//StatusSetColor(i, nState);
				//StatusSetColor(i, STU_STATUS_READY);
			}
		}
	}

	else if ((state == STU_STATUS_INIT) || (state == STU_STATUS_READY))	// 초기화
	{
		if (index < 0 || index >= NUMBER_MAX_SESSION)
		{
			Debug("SetSessionStatus, Error, index(%d)", index);
			return -1;
		}
		m_OB[index].initSession();
		m_OB[index].SetStatus(STU_STATUS_READY);

		// UI
		m_nOB_ID[index] = 0;
		m_nSess_ID[index] = 0;
		//	StatusSetColor(index, STU_STATUS_READY);
	}
	else  // 기존 세션 상태 변경
	{
		if (index < 0 || index >= NUMBER_MAX_SESSION)
		{
			Debug("SetSessionStatus, Error, index(%d)", index);
			return -1;
		}
		m_OB[index].SetStatus(state);

		// UI
	//	StatusSetColor(index, state);
	}

	UPDATE_TO_CTRL();

	return -1;
}



int CTabDlg2::CheckSessionID(BYTE* sessID)
{
	return 0;
}

void CTabDlg2::SetConfigList(BOOL isInsert, int row, CString csCategory, CString csValue)
{

}

void CTabDlg2::Init_IconImage()
{

	imgDisConn.LoadFromResource(AfxGetInstanceHandle(), IDB_DISCONNECT1);
	imgConn.LoadFromResource(AfxGetInstanceHandle(), IDB_CONNECT1);
	imgBlank.LoadFromResource(AfxGetResourceHandle(), IDB_BLANK1);
	imgStatusNP.LoadFromResource(AfxGetResourceHandle(), IDB_ATO_NP1);
	imgStatusCO.LoadFromResource(AfxGetInstanceHandle(), IDB_ATO_CO1);
	imgStatusNA.LoadFromResource(AfxGetInstanceHandle(), IDB_ATO_NA1);
	imgStatusAV.LoadFromResource(AfxGetInstanceHandle(), IDB_ATO_AV1);
	imgStatusRE.LoadFromResource(AfxGetInstanceHandle(), IDB_ATO_RE1);
	imgStatusEG.LoadFromResource(AfxGetInstanceHandle(), IDB_ATO_EG1);
	imgStatusDE.LoadFromResource(AfxGetInstanceHandle(), IDB_ATO_DE1);
	imgStatusFA.LoadFromResource(AfxGetInstanceHandle(), IDB_ATO_FA1);
	imgWarn.LoadFromResource(AfxGetInstanceHandle(), IDB_WARN1);
	imgCheck.LoadFromResource(AfxGetInstanceHandle(), IDB_CHECK1);

	m_pic_onoff.SetBitmap(imgDisConn);
	m_pic_atostate.SetBitmap(imgBlank);

	
}



void CTabDlg2::SetSessionStatusResetAll()
{
	for (int i = 0; i < NUMBER_MAX_SESSION; i++)
	{
		SetSessionStatus(i, 0, 0, STU_STATUS_READY);
	}
}

void CTabDlg2::OnTcnSelchangeTabTs(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int sel = m_ras_tab.GetCurSel();

	switch (sel) {
	case 0:
		p01Tab_2->ShowWindow(SW_SHOW);
		p02Tab_2->ShowWindow(SW_HIDE);
		p04Tab_2->ShowWindow(SW_HIDE);
		p07Tab_2->ShowWindow(SW_HIDE);
		p09Tab_2->ShowWindow(SW_HIDE);
		p10Tab_2->ShowWindow(SW_HIDE);

		break;
	case 1:
		p01Tab_2->ShowWindow(SW_HIDE);
		p02Tab_2->ShowWindow(SW_SHOW);
		p04Tab_2->ShowWindow(SW_HIDE);
		p07Tab_2->ShowWindow(SW_HIDE);
		p09Tab_2->ShowWindow(SW_HIDE);
		p10Tab_2->ShowWindow(SW_HIDE);

		break;
	case 2:
		p01Tab_2->ShowWindow(SW_HIDE);
		p02Tab_2->ShowWindow(SW_HIDE);
		p04Tab_2->ShowWindow(SW_SHOW);
		p07Tab_2->ShowWindow(SW_HIDE);
		p09Tab_2->ShowWindow(SW_HIDE);
		p10Tab_2->ShowWindow(SW_HIDE);

		break;
	case 3:
		p01Tab_2->ShowWindow(SW_HIDE);
		p02Tab_2->ShowWindow(SW_HIDE);
		p04Tab_2->ShowWindow(SW_HIDE);
		p07Tab_2->ShowWindow(SW_SHOW);
		p09Tab_2->ShowWindow(SW_HIDE);
		p10Tab_2->ShowWindow(SW_HIDE);

		break;
	case 4:
		p01Tab_2->ShowWindow(SW_HIDE);
		p02Tab_2->ShowWindow(SW_HIDE);
		p04Tab_2->ShowWindow(SW_HIDE);
		p07Tab_2->ShowWindow(SW_HIDE);
		p09Tab_2->ShowWindow(SW_SHOW);
		p10Tab_2->ShowWindow(SW_HIDE);

		break;
	case 5:
		p01Tab_2->ShowWindow(SW_HIDE);
		p02Tab_2->ShowWindow(SW_HIDE);
		p04Tab_2->ShowWindow(SW_HIDE);
		p07Tab_2->ShowWindow(SW_HIDE);
		p09Tab_2->ShowWindow(SW_HIDE);
		p10Tab_2->ShowWindow(SW_SHOW);

		break;
	}


	*pResult = 0;
}


void CTabDlg2::OnDestroy()
{
	CDialogEx::OnDestroy();

	//pRTab->DestroyWindow();
	//delete pRTab;

	p01Tab_2->DestroyWindow();
	delete p01Tab_2;

	p02Tab_2->DestroyWindow();
	delete p02Tab_2;

	p04Tab_2->DestroyWindow();
	delete p04Tab_2;

	p07Tab_2->DestroyWindow();
	delete p07Tab_2;

	p09Tab_2->DestroyWindow();
	delete p09Tab_2;

	p10Tab_2->DestroyWindow();
	delete p10Tab_2;
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}



void CTabDlg2::OnBnClickedButtonStart()
{
	OnEnKillfocuscop();
	OnEnKillfocuscen();
	OnEnKillfocuscmav();


	CMFCPropertyGridProperty* tempGrid;
	CMFCPropertyGridProperty* currentProperty;
	tempGrid = p01Tab_2->m_propgrid.GetProperty(BODY_DATA);
	currentProperty = tempGrid->GetSubItem(IDX_HSA_NID_ATOTS);

	UPDATE_TO_VARI();	// Control 값을 변수에 적용

	GetDlgItem(IDC_TS_ID)->EnableWindow(FALSE);
	GetDlgItem(IDC_IP_TS)->EnableWindow(FALSE);
	GetDlgItem(IDC_TS_PORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_IP_OB)->EnableWindow(FALSE);
	GetDlgItem(IDC_OB_PORT)->EnableWindow(FALSE);
	
	cnt = 1;
	rcnt = 1;
	int BufLen = 1024;
	CString nowid;
	CString nowPort;
	CString nowIp;
	CString nowDesPort;
	CString nowDesIp;

	m_cTS_ID.GetWindowTextA(nowid);
	m_nTS_ID = int(m_nCountry_ID << 14);
	m_nTS_ID += _ttoi(nowid);
	Utility::SetValueWithTypeCasting(currentProperty, GetDlgItemInt(IDC_TS_ID));
	tempGrid = p02Tab_2->m_propgrid.GetProperty(BODY_DATA);
	currentProperty = tempGrid->GetSubItem(IDX_HSRJ_NID_ATOTS);
	Utility::SetValueWithTypeCasting(currentProperty, GetDlgItemInt(IDC_TS_ID));

	m_cTS_port.GetWindowTextA(nowPort);
	m_nTS_PORT = _ttoi(nowPort);


	m_cOB_port.GetWindowTextA(nowDesPort);
	m_nOB_PORT = _ttoi(nowDesPort);


	GetDlgItemText(IDC_IP_TS, nowIp);
	GetDlgItemText(IDC_IP_OB, nowDesIp);

	//-----------------------------------------------
	// Initialize Winsock
	WSADATA wsaData;

	WSAStartup(MAKEWORD(2, 2), &wsaData); //이거
	//-----------------------------------------------
	// Create a receiver socket to receive datagrams
	
	SetSessionStatusResetAll();
	

	/* 송신을 위한 Client Socket Create */
	char szSTU_IP[20];

	m_SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	sprintf_s(szSTU_IP, 20, "%s", nowDesIp.GetBuffer()); // IP 같게 함.

	m_sockAddr.sin_family = AF_INET;
	m_sockAddr.sin_port = htons(m_nOB_PORT);
	inet_pton(AF_INET, nowDesIp, &(m_sockAddr.sin_addr));

	Debug("TS-OB Client Socket(%d), TS : %s / %d", m_SendSocket, nowDesIp, m_nOB_PORT);

	// 모두 성공하면
	// 1. Control Change
	//m_picture_control.SetBitmap(imgConn);

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_FINISH)->EnableWindow(TRUE);
	m_pic_atostate.SetBitmap(imgBlank);
	m_strList.SetItemText(0, 1, "");
	m_strList.SetItemText(1, 1, "");
	m_strList.SetItemText(2, 1, "");
	m_strList.SetItemText(3, 1, "");

	// 2. 수신을 위한 Thread 시작
	m_bATORunning = TRUE;


	// 3. 송신을 위한  Timer 시작

	m_bAutoResponse = IsDlgButtonChecked(IDC_CHB_AUTO);


	if (m_bAutoResponse)
	{
		StartTimer(TRUE);
	}

	//SetKeepAliveUI(-1, 0, 0);
	SetSessionStatusResetAll();

	SetTimer(MY_Timer_Server, 1000, NULL);

	testStartSec = Utility::getTodaySeconds();

	OnBnClickedCheckIndi();
}


void CTabDlg2::StartTimer(BOOL bStart)
{	

	// 이미 시작한 Timer 가 있으면 일단 죽이고 보자
	if (m_bRunTimer_Server)
	{
		KillTimer(MY_Timer_Server);
	}

	// bStart 값이 FALSE 이면 Timer 중지라는 말
	if (bStart == FALSE)
	{
		m_bRunTimer_Server = FALSE;
		return;
	}

	// bStart 값이 TRUE 이면 Time 시작
	m_bRunTimer_Server = TRUE;

	/*if (IsDlgButtonChecked(IDC_CHECK_AUTO_RESPONSE_RANDOM))
	{
		SetTimer(MY_Timer_Server, m_nAUTO_INTERVAL / 10, NULL);
	}*/

	SetTimer(MY_Timer_Server, m_nAUTO_INTERVAL, NULL);

	Debug("Timer Start, interval(%d ms)", m_nAUTO_INTERVAL);

	return;

}


static tstring MakeFileNameWithTimeStamp(const TCHAR* frontfilename)
{
	time_t t = chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	tm tm_;
	localtime_s(&tm_, &t);
	TCHAR path[128];
	_stprintf_s(path, _T("_%02d%02d%02d.%02d.log"), tm_.tm_year % 100, tm_.tm_mon + 1, tm_.tm_mday, tm_.tm_hour);
	return frontfilename + tstring(path);
}

LRESULT CTabDlg2::LogFileAdd(WPARAM wParam, LPARAM lParam)
{
	m_lc_msg.SaveLogFileAppend(TS_2);
	return TRUE;
}

CString CTabDlg2::getExePath()
{
	CString strPath;
	GetModuleFileName(NULL, strPath.GetBufferSetLength(MAX_PATH), MAX_PATH);
	strPath.ReleaseBuffer();
	return strPath;
}


unsigned int CTabDlg2::KR37_set_crc32_init_value(unsigned int start)
{
	unsigned int init = 0;
	unsigned int byte;

	int val;
	if (start)
	{
		val = 32;
		while (val)
		{
			val -= 8;
			byte = (start >> val) & 0xFF;
			init = KR37_CRC32Table[((init >> 24) ^ byte) & 0xFF] ^
				(init << 8);
		}
	}
	return init;

}


unsigned int CTabDlg2::KR37_compute_check_sum(byte* start, unsigned int data_len)
{
	unsigned int  crc_init = KR37_set_crc32_init_value(0xFFFFFFFF);

	register int i, j;
	for (j = 0; j < (int)data_len; j++)
	{
		i = ((int)(crc_init >> 24) ^ *start++) & 0xff;
		crc_init = (crc_init << 8) ^ KR37_CRC32Table[i];
	}
	return crc_init;
}




void CTabDlg2::OnBnClickedButtonFinish()
{
	m_bATORunning = FALSE;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pic_onoff.SetBitmap(imgDisConn);
	m_etcs_id.SetWindowTextA("Unknown");
	
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_FINISH)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CON)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DISCON)->EnableWindow(FALSE);

	GetDlgItem(IDC_TS_ID)->EnableWindow(TRUE);
	GetDlgItem(IDC_IP_TS)->EnableWindow(TRUE);
	GetDlgItem(IDC_TS_PORT)->EnableWindow(TRUE);
	GetDlgItem(IDC_IP_OB)->EnableWindow(TRUE);
	GetDlgItem(IDC_OB_PORT)->EnableWindow(TRUE);

	KillTimer(MY_Timer_Server);
	m_nSequence = 0;
		
	closesocket(m_SendSocket);

	Debug(_T("TS Finished"));
	
}

void CTabDlg2::CopySelectedItemToClipboard(CListCtrl& listCtrl) {
	/// 리스트 컨트롤에서 선택된 항목의 인덱스를 가져옴
	int selectedIndex = listCtrl.GetNextItem(-1, LVNI_SELECTED);

	if (selectedIndex != -1) {
		// 선택된 항목의 텍스트를 가져옴
		CString sSelectedItemText = listCtrl.GetItemText(selectedIndex, 4);

		// 클립보드 열기
		if (OpenClipboard()) {
			EmptyClipboard();

			// 클립보드 데이터에 메모리 할당
			HGLOBAL hClipboardData = GlobalAlloc(GMEM_MOVEABLE, (sSelectedItemText.GetLength() + 1) * sizeof(TCHAR));

			if (hClipboardData) {
				// 메모리 잠금하고 텍스트 복사
				TCHAR* pClipboardData = (TCHAR*)GlobalLock(hClipboardData);
				_tcscpy_s(pClipboardData, sSelectedItemText.GetLength() + 1, sSelectedItemText);
				GlobalUnlock(hClipboardData);

				// 클립보드 데이터 설정
				SetClipboardData(CF_TEXT, hClipboardData); // 멀티바이트 텍스트에 대한 CF_TEXT
			}

			else

			{
				CloseClipboard();
				return;
			}

			// 클립보드 닫기
			CloseClipboard();
		}
	}
}


void CTabDlg2::OnNMClickListMsg(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 클릭하면 무조건 View탭 보임	2024.02.06 View탭 삭제
	//m_ras_tab.SetCurSel(0);
	//pRTab->ShowWindow(SW_SHOW);
	//pSTab->ShowWindow(SW_HIDE);

	// 클릭한 인덱스 값
	int clickindex = m_lc_msg.GetSelectionMark();


	CString binaryText = m_lc_msg.GetItemText(clickindex, 4);
	//m_buffer_text.SetWindowTextA(binaryText);		에디트 컨트롤 제거
	CopySelectedItemToClipboard(m_lc_msg);

	byte* arr;
	size_t len;
	Utility::TextToByteArray_MemoryAlloc(binaryText, arr, len);



	//type별로 NID_PACKET_ATO 정리1



	if (m_lc_msg.GetItemText(clickindex, 2) == "HS Req")
	{
		m_msgType = aHSReq;						// NID_PACKET_ATO : 0 = HandShake Req
		chgCmb(m_msgType);
		OnCbnSelchangeCmbRecv();
		updtPacket(arr, len);

	}
	else if (m_lc_msg.GetItemText(clickindex, 2) == "JP Req")
	{
		m_msgType = aJPReq;						// NID_PACKET_ATO : 3 = Journey Profile Req
		chgCmb(m_msgType);
		OnCbnSelchangeCmbRecv();
		updtPacket(arr, len);

	}
	else if (m_lc_msg.GetItemText(clickindex, 2) == "JP Ack")
	{
		m_msgType = aJPAck;						// NID_PACKET_ATO : 5 = Journey Profile Ack
		chgCmb(m_msgType);
		OnCbnSelchangeCmbRecv();
		updtPacket(arr, len);

	}
	else if (m_lc_msg.GetItemText(clickindex, 2) == "SP Req")
	{
		m_msgType = aSPReq;						// NID_PACKET_ATO : 6 = Segment Profile Req
		chgCmb(m_msgType);
		OnCbnSelchangeCmbRecv();
		updtPacket(arr, len);

	}
	else if (m_lc_msg.GetItemText(clickindex, 2) == "STR")
	{
		m_msgType = aSTR;						// NID_PACKET_ATO : 8 = Status Report
		chgCmb(m_msgType);
		OnCbnSelchangeCmbRecv();
		updtPacket(arr, len);
	}

	else if (m_lc_msg.GetItemText(clickindex, 2) == "Sess Term")
	{
		m_msgType = aSeTerm;						// NID_PACKET_ATO : 11 = Session Term
		chgCmb(m_msgType);
		OnCbnSelchangeCmbRecv();
		updtPacket(arr, len);
	}


	Utility::SafeDeleteArray(arr);
	*pResult = 0;

}

void CTabDlg2::OnBnClickedButtonCon()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItem(IDC_BUTTON_CON)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DISCON)->EnableWindow(TRUE);
	Cmd_Send(1);

	m_pic_onoff.SetBitmap(imgConn);
	SetDlgItemInt(IDC_ETCS_ID, m_nETCS_ID);
	m_saConComplete = TRUE;
}


void CTabDlg2::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	UINT_PTR idx = nIDEvent;


	if (idx == MY_Timer_Server)
	{
		Cmd_Send(0);
		// TimerCounter ++
		this->SetDlgItemInt(IDC_ALIVE_COUNTER, cnt);
		cnt++;


	}

	CDialogEx::OnTimer(nIDEvent);
}


void CTabDlg2::OnBnClickedButtonDiscon()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Cmd_Send(2);

	m_saConComplete = FALSE;
	m_pic_onoff.SetBitmap(imgDisConn);
	m_etcs_id.SetWindowTextA("Unknown");

	GetDlgItem(IDC_BUTTON_CON)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DISCON)->EnableWindow(FALSE);

	m_nSequence = 0;
#pragma endregion
}



//ListCtrl Limit 확장
void CTabDlg2::OnLvnBeginlabeleditListMsg(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVDISPINFO* pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CEdit* pEdit = m_lc_msg.GetEditControl();
	ASSERT(pEdit);
	pEdit->LimitText(2048); // Set the new limit to 1024 chars
	*pResult = 0;

}


void CTabDlg2::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CTabDlg2::OnRclickListMsg(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CPoint point;
	::GetCursorPos(&point);
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_MENU1));
	CMenu* popup = menu.GetSubMenu(0);
	popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);

	*pResult = 0;
}



void CTabDlg2::OnMenuExpand()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int nSelIndex = m_lc_msg.GetSelectionMark();

	if (nSelIndex != -1) {
		CString strMsg = m_lc_msg.GetItemText(nSelIndex, 4);
		AfxMessageBox(strMsg, MB_OK);
	}
}


void CTabDlg2::OnMenuAllDelete()
{
	m_lc_msg.DeleteAllItems();
}



void CTabDlg2::OnEnKillfocuscop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMFCPropertyGridProperty* tempGrid;
	CMFCPropertyGridProperty* currentProperty;
		
	CString op;
	m_cOP.GetWindowTextA(op);
	Utility::INIWriteString("TS-02", "NID_OPERATIONAL", op, myPath);

	tempGrid = p01Tab_2->m_propgrid.GetProperty(HEADER);
	currentProperty = tempGrid->GetSubItem(IDX_NID_OPERATIONAL);
	Utility::SetValueWithTypeCasting(currentProperty, GetDlgItemInt(IDC_cOP));
	
	tempGrid = p02Tab_2->m_propgrid.GetProperty(HEADER);
	currentProperty = tempGrid->GetSubItem(IDX_NID_OPERATIONAL);
	Utility::SetValueWithTypeCasting(currentProperty, GetDlgItemInt(IDC_cOP));

	tempGrid = p04Tab_2->m_propgrid.GetProperty(HEADER);
	currentProperty = tempGrid->GetSubItem(IDX_NID_OPERATIONAL);
	Utility::SetValueWithTypeCasting(currentProperty, GetDlgItemInt(IDC_cOP));

	tempGrid = p07Tab_2->m_propgrid.GetProperty(HEADER);
	currentProperty = tempGrid->GetSubItem(IDX_NID_OPERATIONAL);
	Utility::SetValueWithTypeCasting(currentProperty, GetDlgItemInt(IDC_cOP));

	tempGrid = p09Tab_2->m_propgrid.GetProperty(HEADER);
	currentProperty = tempGrid->GetSubItem(IDX_NID_OPERATIONAL);
	Utility::SetValueWithTypeCasting(currentProperty, GetDlgItemInt(IDC_cOP));

	tempGrid = p10Tab_2->m_propgrid.GetProperty(HEADER);
	currentProperty = tempGrid->GetSubItem(IDX_NID_OPERATIONAL);
	Utility::SetValueWithTypeCasting(currentProperty, GetDlgItemInt(IDC_cOP));
}


void CTabDlg2::OnEnKillfocuscen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMFCPropertyGridProperty* tempGrid;
	CMFCPropertyGridProperty* currentProperty;
	
	CString en;
	m_cEN.GetWindowTextA(en);
	Utility::INIWriteString("TS-02", "NID_ENGINE", en, myPath);

	tempGrid = p01Tab_2->m_propgrid.GetProperty(HEADER);
	currentProperty = tempGrid->GetSubItem(IDX_NID_ENGINE);
	Utility::SetValueWithTypeCasting(currentProperty, GetDlgItemInt(IDC_cEN));

	tempGrid = p02Tab_2->m_propgrid.GetProperty(HEADER);
	currentProperty = tempGrid->GetSubItem(IDX_NID_ENGINE);
	Utility::SetValueWithTypeCasting(currentProperty, GetDlgItemInt(IDC_cEN));

	tempGrid = p04Tab_2->m_propgrid.GetProperty(HEADER);
	currentProperty = tempGrid->GetSubItem(IDX_NID_ENGINE);
	Utility::SetValueWithTypeCasting(currentProperty, GetDlgItemInt(IDC_cEN));

	tempGrid = p07Tab_2->m_propgrid.GetProperty(HEADER);
	currentProperty = tempGrid->GetSubItem(IDX_NID_ENGINE);
	Utility::SetValueWithTypeCasting(currentProperty, GetDlgItemInt(IDC_cEN));

	tempGrid = p09Tab_2->m_propgrid.GetProperty(HEADER);
	currentProperty = tempGrid->GetSubItem(IDX_NID_ENGINE);
	Utility::SetValueWithTypeCasting(currentProperty, GetDlgItemInt(IDC_cEN));

	tempGrid = p10Tab_2->m_propgrid.GetProperty(HEADER);
	currentProperty = tempGrid->GetSubItem(IDX_NID_ENGINE);
	Utility::SetValueWithTypeCasting(currentProperty, GetDlgItemInt(IDC_cEN));
}


void CTabDlg2::OnEnKillfocuscmav()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMFCPropertyGridProperty* tempGrid;
	CMFCPropertyGridProperty* currentProperty;
		
	CString ver;
	m_cMAV.GetWindowTextA(ver);
	Utility::INIWriteString("TS-02", "VERSION", ver, myPath);

	tempGrid = p01Tab_2->m_propgrid.GetProperty(BODY_DATA);
	currentProperty = tempGrid->GetSubItem(IDX_HSA_M_ATO_Version);
	Utility::SetValueWithTypeCasting(currentProperty, GetDlgItemInt(IDC_cMAV));
}


BOOL CTabDlg2::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}


void CTabDlg2::OnBnClickedCheckIndi()
{
	m_checkindi.GetCheck() == BST_CHECKED ? KillTimer(MY_Timer_Server) : SetTimer(MY_Timer_Server, 1000, NULL);
	m_nSequence = 0;
}

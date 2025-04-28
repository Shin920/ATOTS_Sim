// CSendTab.cpp: 구현 파일
//

#include "pch.h"
#include "ATOTS_Simulator(tab).h"
#include "afxdialogex.h"
#include "GridPropertyOverride.h"
#include "ATOTS_Simulator(tab)Dlg.h"
#include "CTabDlg1.h"
#include <fstream>
#include "Utility.h"
#include "packet02Tab.h"
#include "PropertyCtrlBuilder.h"
#include "TextTable.h"

#define JSON false



#define _UINT16_GETVALUE_(Property, ID) (UINT16)Property.GetSubItem(ID)->GetValue().intVal;

#define _UINT32_GETVALUE_(Property, ID) (UINT32)Property->GetSubItem(ID)->GetValue().intVal;

using namespace PacketHeaderEnum;
using namespace PacketHSRejEnum;



// CSendTab 대화 상자

enum class SendProperty : int
{
	HEADER,
	BODY
};

// target에 문자열을 int로 변환 후 각 타입으로 캐스팅 후 저장(int32보다 작은범위의 타입에만 사용가능)
template<typename T>
static void LoadPacket(string& wholeTXT, const CHAR* idc, T& target)
{
	size_t strIdx, strEndlineIdx;
	string currentLine;

	// Q_TRAIN_DATA_VALID: 문자열 검색
	strIdx = wholeTXT.find(idc);
	if (strIdx != string::npos)
	{
		strIdx += strlen(idc);
		while (isdigit(wholeTXT[strIdx]) == 0 && wholeTXT[strIdx] != '-')++strIdx;//숫자 시작점으로 위치 옮김
		strEndlineIdx = wholeTXT.find(endLine, strIdx); // 문자열 이후의 \n의 위치
		currentLine = wholeTXT.substr(strIdx, strEndlineIdx - strIdx); // 문자열 한 줄
		target = (T)atoi(currentLine.c_str());
		wholeTXT = wholeTXT.substr(strEndlineIdx);
	}
}

IMPLEMENT_DYNAMIC(packet02Tab, CDialogEx)

packet02Tab::packet02Tab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_SEND_2, pParent), pHSRejStructure(nullptr)
{
	Init();
}

packet02Tab::~packet02Tab()
{
	Release();
}

void packet02Tab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MPG_SEND, m_propgrid);
	DDX_Control(pDX, IDC_CHB_AUTO, m_chb_auto);
	DDX_Control(pDX, IDC_CMB_PACKET, m_cmb_send);
}


BEGIN_MESSAGE_MAP(packet02Tab, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHB_AUTO, &packet02Tab::OnBnClickedChbAuto)
	ON_CBN_SELCHANGE(IDC_CMB_PACKET, &packet02Tab::OnCbnSelchangeCmbPacket)
	ON_BN_CLICKED(IDC_BTN_SEND, &packet02Tab::OnBnClickedBtnSend)
	ON_BN_CLICKED(IDC_BTN_SAVE, &packet02Tab::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_LOAD, &packet02Tab::OnBnClickedBtnLoad)
END_MESSAGE_MAP()


// CSendTab 메시지 처리기

void packet02Tab::Init()
{	
	
	pHeaderStructure = new ATO_Header;
	pHSRejStructure = new TS_OB_Packet02_HSRejStruct;
		
	pFileHSRejStructure = new TS_OB_Packet02_HSRejStruct;
	
	m_nPacketCounter = -1;
}

void packet02Tab::Release()
{
	delete pHeaderStructure;
	delete pHSRejStructure;
	
	delete pFileHSRejStructure;
	
	
}

BOOL packet02Tab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.


	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_cmb_send.AddString(_T("Send : Handshake Reject"));
	m_cmb_send.SetCurSel(0);	//UI



	m_chb_auto.SetCheck(BST_CHECKED);

//  테스트 편의성
//	GetDlgItem(IDC_BTN_SEND)->EnableWindow(FALSE);



	propgrid();
	initLoadPacketHSRejFromFile();


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}



void packet02Tab::PacketHeaderCtrlToStructure(const CMFCPropertyGridProperty* bodyProperty, ATO_Header& p)
{
	m_nPacketCounter = getPacketCounter();

	const CMFCPropertyGridProperty& cp = *(bodyProperty);
	UINT16 dateValue = (UINT16)cp.GetSubItem(IDX_T_Timestamp_Date)->GetValue().intVal;
	UINT32 secValue = (UINT32)cp.GetSubItem(IDX_T_Timestamp_Seconds)->GetValue().intVal;
	UINT8 cntValue = (UINT8)cp.GetSubItem(IDX_N_Packet_Counter)->GetValue().intVal;

	p.NID_PACKET_ATO = (UINT8)cp.GetSubItem(IDX_NID_PACKET_ATO)->GetValue().intVal;
	
	// NID_OPERATIONAL BCD 변환 및 패딩
	UINT32 nidOperationalValue = (UINT32)cp.GetSubItem(IDX_NID_OPERATIONAL)->GetValue().uintVal;
	std::vector<UINT8> bcdValue = Utility::intToBCD(nidOperationalValue);
	Utility::padBCD(bcdValue, 4);  // 4바이트로 패딩

	// BCD 값에서 UINT32로 변환하여 NID_OPERATIONAL에 저장
	p.NID_OPERATIONAL = 0;
	for (size_t i = 0; i < bcdValue.size(); ++i) {
		p.NID_OPERATIONAL |= (bcdValue[i] << ((bcdValue.size() - 1 - i) * 4));
	}

	p.NID_ENGINE = (UINT32)cp.GetSubItem(IDX_NID_ENGINE)->GetValue().intVal;


	if (dateValue == 0)
	{
		p.T_Timestamp_Date = Utility::getUtcDateSince2010();
	}
	else
	{
		p.T_Timestamp_Date = (UINT16)cp.GetSubItem(IDX_T_Timestamp_Date)->GetValue().intVal;
	}
	if (secValue == 0)
	{
		p.T_Timestamp_Seconds = Utility::getTodaySeconds();
	}
	else
	{
		p.T_Timestamp_Seconds = (UINT32)cp.GetSubItem(IDX_T_Timestamp_Seconds)->GetValue().intVal;
	}
	if (cntValue == 0)
	{
		p.N_Packet_Counter = m_nPacketCounter;
	}
	else
	{
		p.N_Packet_Counter = (UINT8)cp.GetSubItem(IDX_N_Packet_Counter)->GetValue().intVal;
	}
}


void packet02Tab::PacketHSRejCtrlToStructure(const CMFCPropertyGridProperty* bodyProperty, TS_OB_Packet02_HSRejStruct& p)
{
	const CMFCPropertyGridProperty& cp = *(bodyProperty);

	p.Q_Reject_Reason = (UINT8)cp.GetSubItem(IDX_Q_Reject_Reason)->GetValue().intVal;
	p.NID_C = (UINT16)cp.GetSubItem(IDX_HSRJ_NID_C)->GetValue().intVal;
	p.NID_ATOTS = (UINT16)cp.GetSubItem(IDX_HSRJ_NID_ATOTS)->GetValue().intVal;
}


BOOL packet02Tab::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(255, 255, 255));

	return TRUE;

	return CDialogEx::OnEraseBkgnd(pDC);
}


HBRUSH packet02Tab::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	pDC->SetBkColor(RGB(255, 255, 255)); // Static Control 전부 글자배경색 하얀색으로

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

void packet02Tab::propgrid()
{
	int pnum = 2;

	


	CRect rect;

	m_propgrid.EnableHeaderCtrl(TRUE, _T("Item"), _T("Value"));
	m_propgrid.GetWindowRect(&rect);
	m_propgrid.PostMessage(WM_SIZE, 0, MAKELONG(rect.Width() - 50, rect.Height()));

	CMFCPropertyGridProperty* pxpgp;
	CMFCPropertyEX<UINT32>* pxu;

	CMFCPropertyGridProperty* pxProperty;
	pxProperty = new CMFCPropertyGridProperty(_T("HEADER Data"));
	m_propgrid.AddProperty(pxProperty);

	pxpgp = new CMFCPropertyGridProperty(_T("NID_PACKET_ATO"), (variant_t)pnum, _T("패킷 넘버"));
	pxpgp->Enable(FALSE);
	pxProperty->AddSubItem(pxpgp);

	pxu = new CMFCPropertyEX<UINT32>(_T("NID_OPERATIONAL"), (variant_t)(UINT32)100210, _T("Train Running Number"));
	pxu->SetFormat("%u");
	pxu->SetMinMaxValue(0, UINT32_MAX);
	pxu->Enable(TRUE);
	pxProperty->AddSubItem(pxu);

	pxpgp = new CMFCPropertyGridProperty(_T("NID_ENGINE"), (variant_t)(int)351018, _T("ETCS Identify"));
	pxpgp->Enable(TRUE);
	pxProperty->AddSubItem(pxpgp);

	pxpgp = new CMFCPropertyGridProperty(_T("T_TimeStamp_Date"), (variant_t)(int)0, _T("Timestamp in date since start-up"));
	pxpgp->Enable(TRUE);
	pxProperty->AddSubItem(pxpgp);

	pxpgp = new CMFCPropertyGridProperty(_T("T_TimeStamp_Seconds"), (variant_t)(int)0, _T("Timestamp in milliseconds since start-up"));
	pxpgp->Enable(TRUE);
	pxProperty->AddSubItem(pxpgp);

	pxpgp = new CMFCPropertyGridProperty(_T("N_Packet_Counter"), (variant_t)(int)0, _T("보내진 패킷 Counter"));
	pxpgp->Enable(TRUE);
	pxProperty->AddSubItem(pxpgp);

	pxProperty->Expand(TRUE);

	pxProperty = new CMFCPropertyGridProperty(_T("BODY Data"));
	m_propgrid.AddProperty(pxProperty);


	CMFCPropertyGridProperty* pxBodyProperty = m_propgrid.GetProperty((int)SendProperty::BODY);


	
	PropertyCtrlBuilder::BuildBodyPkHSRej(pxBodyProperty);
	
}

void packet02Tab::OnBnClickedChbAuto()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		if (m_chb_auto.GetCheck() == BST_UNCHECKED)
	{
		//GetDlgItem(IDC_BTN_REJECT)->EnableWindow(TRUE);
		//GetDlgItem(IDC_BTN_SEND)->EnableWindow(TRUE);
	}
	else
	{
		//GetDlgItem(IDC_BTN_REJECT)->EnableWindow(FALSE);
		//GetDlgItem(IDC_BTN_SEND)->EnableWindow(FALSE);
	}

}


void packet02Tab::OnCbnSelchangeCmbPacket()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_propgrid.RemoveAll();
	propgrid();
}


void packet02Tab::OnBnClickedBtnSend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// Sa-Conn 연결여부 확인 필요
		
	CTabDlg1* targetDlg = ((CATOTSSimulatortabDlg*)AfxGetMainWnd())->pDlg1;

		
			PacketHeaderCtrlToStructure(m_propgrid.GetProperty(HEADER), *pHeaderStructure);
			PacketHSRejCtrlToStructure(m_propgrid.GetProperty(BODY_DATA), *pHSRejStructure);

			targetDlg->Cmd_Send126(eHsRej);
			
		
	
}

void packet02Tab::PacketHsRejStructureToINI(TS_OB_Packet02_HSRejStruct& p)
{
	if (&p == nullptr)return;

	CStdioFile ipfile;
	CStdioFile* pipfile = &ipfile;

	CString dir = Utility::GetDirectoryFromFileDialog(_T("*.ini"), HSRejDefaultConfigFileName, OFN_HIDEREADONLY, szConfigFilter);
	if (ipfile.Open(dir.GetString(), CFile::modeCreate | CFile::modeWrite) == FALSE) return;
	Utility::CFileWriteCHAR(pipfile, packetHSRej);
	Utility::CFileWriteCHAR(pipfile, endLine);

	CHAR tempChar[128]{};
	TextTablePacketHSRej& idcList = TextTablePacketHSRej::GetInstance();

	sprintf_s(tempChar, ": %d\n", p.Q_Reject_Reason);
	Utility::CFileWriteCHAR(pipfile, idcList.GetText(IDX_Q_Reject_Reason));
	Utility::CFileWriteCHAR(pipfile, tempChar);

	sprintf_s(tempChar, ": %d\n", p.NID_C);
	Utility::CFileWriteCHAR(pipfile, idcList.GetText(IDX_HSRJ_NID_C));
	Utility::CFileWriteCHAR(pipfile, tempChar);

	sprintf_s(tempChar, ": %d\n", p.NID_ATOTS);
	Utility::CFileWriteCHAR(pipfile, idcList.GetText(IDX_HSRJ_NID_ATOTS));
	Utility::CFileWriteCHAR(pipfile, tempChar);

}


BOOL packet02Tab::LoadPacketHSRejFromFile()
{
	TS_OB_Packet02_HSRejStruct& p = *pFileHSRejStructure;
	if (&p == nullptr)return FALSE;
	static CString pathName = HSRejDefaultConfigFileName;

	static bool initFlag = true;
	if (initFlag)
	{
		pathName = Utility::GetDirectoryFromFileDialog(_T("*.ini"), HSRejDefaultConfigFileName, OFN_HIDEREADONLY, szConfigFilter);
		if (pathName.Compare(_T("")) == 0) return FALSE;
		initFlag = true;
	}	

	CStdioFile ipfile;
	if (!ipfile.Open(pathName, CFile::modeRead))	//Open
	{
		// 파일 열기 실패 시 기본 파일 열기 시도
		if (!ipfile.Open(HSRejDefaultConfigFileName, CFile::modeRead)) return FALSE;
	}

	if (Utility::CheckFileUnicode(ipfile)) return FALSE;

	// 파일 내용 복사
	ULONGLONG fileSize = ipfile.GetLength();
	CHAR* tempStr = new CHAR[fileSize + 1]; // +1 for null terminator
	ipfile.Read(tempStr, (UINT)fileSize);
	tempStr[fileSize] = '\0'; // Null terminate the string
	string configContainer(tempStr); // 파일 전체 문자열
	delete[] tempStr;

	// [HandShake Reject] 문자열 검색
	size_t strIdx = configContainer.find(packetHSRej);
	if (strIdx != string::npos) //발견
	{
		configContainer = configContainer.substr(strIdx); // [HandShake Reject]을 포함한 이후의 문자열이 남음
		LoadPacketHSRejFromFile_StaticData(configContainer, p);
		ipfile.Close();
		return TRUE;
	}
	else
	{
		ipfile.Close();
		return FALSE; // HS Rej의 정보가 담긴 파일이 아님
	}
}

BOOL packet02Tab::initLoadPacketHSRejFromFile()
{
	TS_OB_Packet02_HSRejStruct& p = *pFileHSRejStructure;
	if (&p == nullptr)return FALSE;
	static CString pathName = HSRejDefaultConfigFileName;

	CStdioFile ipfile;
	if (ipfile.Open(pathName.GetString(), CFile::modeRead) == FALSE)
	{
		if (ipfile.Open(HSRejDefaultConfigFileName, CFile::modeRead) == FALSE) return FALSE;
	}
	if (Utility::CheckFileUnicode(ipfile) == TRUE) return FALSE;

	//파일 내용 복사
	ULONGLONG fileSize = ipfile.GetLength();
	CHAR* tempStr = new CHAR[fileSize];
	ipfile.Read(tempStr, (UINT)fileSize);
	string configContainer; //파일 전체 문자열
	configContainer = tempStr;
	delete[] tempStr;

	// [HS Rej] 문자열 검색
	size_t strIdx = configContainer.find(packetHSRej);
	if (strIdx != string::npos) //발견
		configContainer = configContainer.substr(strIdx); // [HS Rej]를 포함한 이후의 문자열이 남음
	else return FALSE; //HS Rej 의 정보가 담긴 파일이 아님

	LoadPacketHSRejFromFile_StaticData(configContainer, p);

	ipfile.Close();

	ReLoadPacketHSRej();

	return TRUE;
}

void packet02Tab::LoadPacketHSRejFromFile_StaticData(string& configTXT, TS_OB_Packet02_HSRejStruct& p)
{
	size_t strIdx, strEndlineIdx;
	string currentLine;


	TextTablePacketHSRej& idcList = TextTablePacketHSRej::GetInstance();

	// Q_Reject_Reason
	strIdx = configTXT.find(idcList.GetText(IDX_Q_Reject_Reason));
	if (strIdx != string::npos)
	{
		strIdx += strlen(idcList.GetText(IDX_Q_Reject_Reason));
		while (isdigit(configTXT[strIdx]) == 0)++strIdx;
		strEndlineIdx = configTXT.find(endLine, strIdx);
		currentLine = configTXT.substr(strIdx, strEndlineIdx - strIdx);
		p.Q_Reject_Reason = atoi(currentLine.c_str());
		configTXT = configTXT.substr(strEndlineIdx);
	}


	// NID_C: 문자열 검색
	strIdx = configTXT.find(idcList.GetText(IDX_HSRJ_NID_C));
	if (strIdx != string::npos)
	{
		strIdx += strlen(idcList.GetText(IDX_HSRJ_NID_C));
		while (isdigit(configTXT[strIdx]) == 0)++strIdx;
		strEndlineIdx = configTXT.find(endLine, strIdx);
		currentLine = configTXT.substr(strIdx, strEndlineIdx - strIdx);
		p.NID_C = atoi(currentLine.c_str());
		configTXT = configTXT.substr(strEndlineIdx);
	}

	// NID_ATOTS
	strIdx = configTXT.find(idcList.GetText(IDX_HSRJ_NID_ATOTS));
	if (strIdx != string::npos)
	{
		strIdx += strlen(idcList.GetText(IDX_HSRJ_NID_ATOTS));
		while (isdigit(configTXT[strIdx]) == 0)++strIdx;
		strEndlineIdx = configTXT.find(endLine, strIdx);
		currentLine = configTXT.substr(strIdx, strEndlineIdx - strIdx);
		p.NID_ATOTS = atoi(currentLine.c_str());
		configTXT = configTXT.substr(strEndlineIdx);
	}


	LoadPacket(configTXT, idcList.GetText(IDX_Q_Reject_Reason), p.Q_Reject_Reason);
	LoadPacket(configTXT, idcList.GetText(IDX_HSRJ_NID_C), p.NID_C);
	LoadPacket(configTXT, idcList.GetText(IDX_HSRJ_NID_ATOTS), p.NID_ATOTS);

}

void packet02Tab::ReLoadPacketHSRej()
{
	PropertyCtrlBuilder::PacketHSRejStructureToCtrl(*pFileHSRejStructure, m_propgrid.GetProperty(BODY_DATA));
}

void packet02Tab::OnBnClickedBtnSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		
		
			PacketHSRejCtrlToStructure(m_propgrid.GetProperty(BODY_DATA), *pFileHSRejStructure);
			PacketHsRejStructureToINI(*pFileHSRejStructure);
					

}

	



void packet02Tab::OnBnClickedBtnLoad()
{
	
		if (LoadPacketHSRejFromFile() == FALSE) return; //파일 읽기 실패 시 그대로 종료
		ReLoadPacketHSRej();

}

int packet02Tab::getPacketCounter()
{
	m_nPacketCounter++;

	if (m_nPacketCounter > 255)
		m_nPacketCounter = -1;

	return m_nPacketCounter;
}


BOOL packet02Tab::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}

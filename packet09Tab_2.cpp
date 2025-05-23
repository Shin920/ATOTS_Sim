﻿// CSendTab.cpp: 구현 파일
//

#include "pch.h"
#include "ATOTS_Simulator(tab).h"
#include "afxdialogex.h"
#include "GridPropertyOverride.h"
#include "ATOTS_Simulator(tab)Dlg.h"
#include "CTabDlg2.h"
#include <fstream>
#include "Utility.h"
#include "packet09Tab_2.h"
#include "PropertyCtrlBuilder.h"
#include "TextTable.h"

#define JSON false



#define _UINT16_GETVALUE_(Property, ID) (UINT16)Property.GetSubItem(ID)->GetValue().intVal;

#define _UINT32_GETVALUE_(Property, ID) (UINT32)Property->GetSubItem(ID)->GetValue().intVal;

using namespace PacketHeaderEnum;
using namespace PacketSTRAckEnum;


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

IMPLEMENT_DYNAMIC(packet09Tab_2, CDialogEx)

packet09Tab_2::packet09Tab_2(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_SEND_11, pParent), pSTRAckStructure(nullptr)
{
	Init();
}

packet09Tab_2::~packet09Tab_2()
{
	Release();
}

void packet09Tab_2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MPG_SEND, m_propgrid);
	DDX_Control(pDX, IDC_CHB_AUTO, m_chb_auto);
	DDX_Control(pDX, IDC_CMB_PACKET, m_cmb_send);
}


BEGIN_MESSAGE_MAP(packet09Tab_2, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHB_AUTO, &packet09Tab_2::OnBnClickedChbAuto)
	ON_CBN_SELCHANGE(IDC_CMB_PACKET, &packet09Tab_2::OnCbnSelchangeCmbPacket)
	ON_BN_CLICKED(IDC_BTN_SEND, &packet09Tab_2::OnBnClickedBtnSend)
	ON_BN_CLICKED(IDC_BTN_SAVE, &packet09Tab_2::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_LOAD, &packet09Tab_2::OnBnClickedBtnLoad)
END_MESSAGE_MAP()


// CSendTab 메시지 처리기

void packet09Tab_2::Init()
{
	pHeaderStructure = new ATO_Header;
	pSTRAckStructure = new TS_OB_Packet09_STRAckStruct;

	pFileSTRAckStructure = new TS_OB_Packet09_STRAckStruct;

	m_nPacketCounter = -1;
}

void packet09Tab_2::Release()
{
	delete pHeaderStructure;
	delete pSTRAckStructure;
	delete pFileSTRAckStructure;

}

BOOL packet09Tab_2::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.


	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_cmb_send.AddString(_T("Send : Status Report Acknowledgement"));
	m_cmb_send.SetCurSel(0);	//UI



	m_chb_auto.SetCheck(BST_CHECKED);

	//  테스트 편의성
	//	GetDlgItem(IDC_BTN_SEND)->EnableWindow(FALSE);



	propgrid();
	initLoadPacketSTRAckFromFile();


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}



void packet09Tab_2::PacketHeaderCtrlToStructure(const CMFCPropertyGridProperty* bodyProperty, ATO_Header& p)
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


void packet09Tab_2::PacketSTRAckCtrlToStructure(const CMFCPropertyGridProperty* bodyProperty, TS_OB_Packet09_STRAckStruct& p)
{
	const CMFCPropertyGridProperty& cp = *(bodyProperty);

	p.T_STR_Ref_TimeStamp_Date = (UINT16)cp.GetSubItem(IDX_T_STR_Reference_Timestamp_Date)->GetValue().intVal;
	p.T_STR_Ref_TimeStamp_Seconds = (UINT32)cp.GetSubItem(IDX_T_STR_Reference_Timestamp_Seconds)->GetValue().intVal;
	p.N_STR_Ref_Packet_Counter = (UINT8)cp.GetSubItem(IDX_N_STR_Reference_Packet_Counter)->GetValue().intVal;

}





BOOL packet09Tab_2::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(255, 255, 255));

	return TRUE;

	return CDialogEx::OnEraseBkgnd(pDC);
}


HBRUSH packet09Tab_2::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	pDC->SetBkColor(RGB(255, 255, 255)); // Static Control 전부 글자배경색 하얀색으로

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

void packet09Tab_2::propgrid()
{
	int pnum = 9;



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




	PropertyCtrlBuilder::BuildBodyPkSTRAck(pxBodyProperty);




}

void packet09Tab_2::OnBnClickedChbAuto()
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


void packet09Tab_2::OnCbnSelchangeCmbPacket()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_propgrid.RemoveAll();
	propgrid();
}


void packet09Tab_2::OnBnClickedBtnSend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// Sa-Conn 연결여부 확인 필요

	CTabDlg2* targetDlg = ((CATOTSSimulatortabDlg*)AfxGetMainWnd())->pDlg2;


	PacketHeaderCtrlToStructure(m_propgrid.GetProperty(HEADER), *pHeaderStructure);
	PacketSTRAckCtrlToStructure(m_propgrid.GetProperty(BODY_DATA), *pSTRAckStructure);

	targetDlg->Cmd_Send126(eSTRAck);



}

void packet09Tab_2::PacketSTRAckStructrureToINI(TS_OB_Packet09_STRAckStruct& p)
{
	if (&p == nullptr)return;

	CStdioFile ipfile;
	CStdioFile* pipfile = &ipfile;

	CString dir = Utility::GetDirectoryFromFileDialog(_T("*.ini"), STRAckDefaultConfigFileName, OFN_HIDEREADONLY, szConfigFilter);
	if (ipfile.Open(dir.GetString(), CFile::modeCreate | CFile::modeWrite) == FALSE) return;
	Utility::CFileWriteCHAR(pipfile, packetSTRAck);
	Utility::CFileWriteCHAR(pipfile, endLine);

	CHAR tempChar[128]{};
	TextTablePacketSTRAck& idcList = TextTablePacketSTRAck::GetInstance();

	sprintf_s(tempChar, ": %d\n", p.T_STR_Ref_TimeStamp_Date);
	Utility::CFileWriteCHAR(pipfile, idcList.GetText(IDX_T_STR_Reference_Timestamp_Date));
	Utility::CFileWriteCHAR(pipfile, tempChar);

	sprintf_s(tempChar, ": %d\n", p.T_STR_Ref_TimeStamp_Seconds);
	Utility::CFileWriteCHAR(pipfile, idcList.GetText(IDX_T_STR_Reference_Timestamp_Seconds));
	Utility::CFileWriteCHAR(pipfile, tempChar);

	sprintf_s(tempChar, ": %d\n", p.N_STR_Ref_Packet_Counter);
	Utility::CFileWriteCHAR(pipfile, idcList.GetText(IDX_N_STR_Reference_Packet_Counter));
	Utility::CFileWriteCHAR(pipfile, tempChar);


}

BOOL packet09Tab_2::LoadPacketSTRAckFromFile()
{
	TS_OB_Packet09_STRAckStruct& p = *pFileSTRAckStructure;
	if (&p == nullptr)return FALSE;
	static CString pathName = STRAckDefaultConfigFileName;

	static bool initFlag = true;
	if (initFlag)
	{
		pathName = Utility::GetDirectoryFromFileDialog(_T("*.ini"), STRAckDefaultConfigFileName, OFN_HIDEREADONLY, szConfigFilter);
		if (pathName.Compare(_T("")) == 0) return FALSE;
		initFlag = true;
	}
	

	CStdioFile ipfile;
	if (!ipfile.Open(pathName, CFile::modeRead))	//Open
	{
		// 파일 열기 실패 시 기본 파일 열기 시도
		if (!ipfile.Open(STRAckDefaultConfigFileName, CFile::modeRead)) return FALSE;
	}

	if (Utility::CheckFileUnicode(ipfile)) return FALSE;

	// 파일 내용 복사
	ULONGLONG fileSize = ipfile.GetLength();
	CHAR* tempStr = new CHAR[fileSize + 1]; // +1 for null terminator
	ipfile.Read(tempStr, (UINT)fileSize);
	tempStr[fileSize] = '\0'; // Null terminate the string
	string configContainer(tempStr); // 파일 전체 문자열
	delete[] tempStr;

	// [Status Report Acknowledgement] 문자열 검색
	size_t strIdx = configContainer.find(packetSTRAck);
	if (strIdx != string::npos) //발견
	{
		configContainer = configContainer.substr(strIdx); // [Status Report Acknowledgement]을 포함한 이후의 문자열이 남음
		LoadPacketSTRAckFromFile_StaticData(configContainer, p);
		ipfile.Close();
		return TRUE;
	}
	else
	{
		ipfile.Close();
		return FALSE; // Status Report Acknowledgement의 정보가 담긴 파일이 아님
	}
}

BOOL packet09Tab_2::initLoadPacketSTRAckFromFile()
{
	TS_OB_Packet09_STRAckStruct& p = *pFileSTRAckStructure;
	if (&p == nullptr)return FALSE;
	static CString pathName = STRAckDefaultConfigFileName;

	CStdioFile ipfile;
	if (ipfile.Open(pathName.GetString(), CFile::modeRead) == FALSE)
	{
		if (ipfile.Open(STRAckDefaultConfigFileName, CFile::modeRead) == FALSE) return FALSE;
	}
	if (Utility::CheckFileUnicode(ipfile) == TRUE) return FALSE;

	//파일 내용 복사
	ULONGLONG fileSize = ipfile.GetLength();
	CHAR* tempStr = new CHAR[fileSize];
	ipfile.Read(tempStr, (UINT)fileSize);
	string configContainer; //파일 전체 문자열
	configContainer = tempStr;
	delete[] tempStr;

	// [Status Report Acknowledgement] 문자열 검색
	size_t strIdx = configContainer.find(packetSTRAck);
	if (strIdx != string::npos) // 발견
		configContainer = configContainer.substr(strIdx); // [Status Report Acknowledgement]을 포함한 이후의 문자열이 남음
	else return FALSE; //STR Ack 의 정보가 담긴 파일이 아님

	LoadPacketSTRAckFromFile_StaticData(configContainer, p);

	ipfile.Close();

	ReLoadPacketSTRAck();

	return TRUE;
}

void packet09Tab_2::LoadPacketSTRAckFromFile_StaticData(string& configTXT, TS_OB_Packet09_STRAckStruct& p)
{
	size_t strIdx, strEndlineIdx;
	string currentLine;


	TextTablePacketSTRAck& idcList = TextTablePacketSTRAck::GetInstance();

	// T_STR_Ref_TimeStamp_Date 문자열 검색
	strIdx = configTXT.find(idcList.GetText(IDX_T_STR_Reference_Timestamp_Date));
	if (strIdx != string::npos)
	{
		strIdx += strlen(idcList.GetText(IDX_T_STR_Reference_Timestamp_Date));
		while (isdigit(configTXT[strIdx]) == 0)++strIdx;
		strEndlineIdx = configTXT.find(endLine, strIdx);
		currentLine = configTXT.substr(strIdx, strEndlineIdx - strIdx);
		p.T_STR_Ref_TimeStamp_Date = atoi(currentLine.c_str());
		configTXT = configTXT.substr(strEndlineIdx);
	}

	// T_STR_Ref_TimeStamp_Seconds
	strIdx = configTXT.find(idcList.GetText(IDX_T_STR_Reference_Timestamp_Seconds));
	if (strIdx != string::npos)
	{
		strIdx += strlen(idcList.GetText(IDX_T_STR_Reference_Timestamp_Seconds));
		while (isdigit(configTXT[strIdx]) == 0)++strIdx;
		strEndlineIdx = configTXT.find(endLine, strIdx);
		currentLine = configTXT.substr(strIdx, strEndlineIdx - strIdx);
		p.T_STR_Ref_TimeStamp_Seconds = atoi(currentLine.c_str());
		configTXT = configTXT.substr(strEndlineIdx);
	}

	// NID_M_ATO_Version
	strIdx = configTXT.find(idcList.GetText(IDX_N_STR_Reference_Packet_Counter));
	if (strIdx != string::npos)
	{
		strIdx += strlen(idcList.GetText(IDX_N_STR_Reference_Packet_Counter));
		while (isdigit(configTXT[strIdx]) == 0)++strIdx;
		strEndlineIdx = configTXT.find(endLine, strIdx);
		currentLine = configTXT.substr(strIdx, strEndlineIdx - strIdx);
		p.N_STR_Ref_Packet_Counter = atoi(currentLine.c_str());
		configTXT = configTXT.substr(strEndlineIdx);
	}

	LoadPacket(configTXT, idcList.GetText(IDX_T_STR_Reference_Timestamp_Date), p.T_STR_Ref_TimeStamp_Date);
	LoadPacket(configTXT, idcList.GetText(IDX_T_STR_Reference_Timestamp_Seconds), p.T_STR_Ref_TimeStamp_Seconds);
	LoadPacket(configTXT, idcList.GetText(IDX_N_STR_Reference_Packet_Counter), p.N_STR_Ref_Packet_Counter);
}



void packet09Tab_2::ReLoadPacketSTRAck()
{
	PropertyCtrlBuilder::PacketSTRAckStructureToCtrl(*pFileSTRAckStructure, m_propgrid.GetProperty(BODY_DATA));
}



void packet09Tab_2::OnBnClickedBtnSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.



	PacketSTRAckCtrlToStructure(m_propgrid.GetProperty(BODY_DATA), *pFileSTRAckStructure);

	PacketSTRAckStructrureToINI(*pFileSTRAckStructure);


}





void packet09Tab_2::OnBnClickedBtnLoad()
{

	if (LoadPacketSTRAckFromFile() == FALSE) return; //파일 읽기 실패 시 그대로 종료
	ReLoadPacketSTRAck();


}

int packet09Tab_2::getPacketCounter()
{
	m_nPacketCounter++;

	if (m_nPacketCounter > 255)
		m_nPacketCounter = -1;

	return m_nPacketCounter;
}


BOOL packet09Tab_2::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}

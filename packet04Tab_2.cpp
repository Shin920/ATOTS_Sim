// CSendTab.cpp: 구현 파일
//

#include "pch.h"
#include "ATOTS_Simulator(tab).h"
#include "afxdialogex.h"
#include "GridPropertyOverride.h"
#include "ATOTS_Simulator(tab)Dlg.h"
#include "CTabDlg2.h"
#include <fstream>
#include "Utility.h"
#include "packet04Tab_2.h"
#include "PropertyCtrlBuilder.h"
#include "TextTable.h"
#include "F110_Msg_Encoder_ATO.h"


#define _UINT16_GETVALUE_(Property, ID) (UINT16)Property.GetSubItem(ID)->GetValue().intVal;

#define _UINT32_GETVALUE_(Property, ID) (UINT32)Property->GetSubItem(ID)->GetValue().intVal;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


using namespace PacketHeaderEnum;
using namespace PacketJPEnum;


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

IMPLEMENT_DYNAMIC(packet04Tab_2, CDialogEx)

packet04Tab_2::packet04Tab_2(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_SEND_9, pParent), pJPStructure(nullptr)
{
	Init();
}

packet04Tab_2::~packet04Tab_2()
{
	Release();
}

void packet04Tab_2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MPG_SEND, m_propgrid);
	DDX_Control(pDX, IDC_CHB_AUTO, m_chb_auto);
	DDX_Control(pDX, IDC_CMB_PACKET, m_cmb_send);
}


BEGIN_MESSAGE_MAP(packet04Tab_2, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHB_AUTO, &packet04Tab_2::OnBnClickedChbAuto)
	ON_CBN_SELCHANGE(IDC_CMB_PACKET, &packet04Tab_2::OnCbnSelchangeCmbPacket)
	ON_BN_CLICKED(IDC_BTN_SEND, &packet04Tab_2::OnBnClickedBtnSend)
	ON_BN_CLICKED(IDC_BTN_SAVE, &packet04Tab_2::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_LOAD, &packet04Tab_2::OnBnClickedBtnLoad)
	ON_BN_CLICKED(IDC_BUTTON1, &packet04Tab_2::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BTN_SEND_DYNAMIC, &packet04Tab_2::OnBnClickedBtnSendDynamic)
END_MESSAGE_MAP()


// CSendTab 메시지 처리기

void packet04Tab_2::Init()
{
	pHeaderStructure = new ATO_Header;
	pJPStructure = new TS_OB_Packet04_JPStruct;

	pFileJPStructure = new TS_OB_Packet04_JPStruct;
	pRDJPStructure = new TS_OB_Packet04_JPStruct;

	m_nPacketCounter = -1;

}
void packet04Tab_2::Release()
{
	delete pHeaderStructure;
	delete pJPStructure;
	delete pFileJPStructure;
	delete pRDJPStructure;

}

BOOL packet04Tab_2::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.


	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_cmb_send.AddString(_T("Send : Journey Profile"));
	m_cmb_send.SetCurSel(0);	//UI



	m_chb_auto.SetCheck(BST_UNCHECKED);

	//  테스트 편의성
	//	GetDlgItem(IDC_BTN_SEND)->EnableWindow(FALSE);



	propgrid();
	initLoadPacketJPFromFile();


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}



void packet04Tab_2::PacketHeaderCtrlToStructure(const CMFCPropertyGridProperty* bodyProperty, ATO_Header& p)
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


void packet04Tab_2::PacketJPCtrlToStructure(const CMFCPropertyGridProperty* bodyProperty, TS_OB_Packet04_JPStruct& p)
{
	
	const CMFCPropertyGridProperty& cp = *(bodyProperty);

	p.Q_JP_Status = (UINT8)cp.GetSubItem(IDX_Q_JP_Status)->GetValue().intVal;
	p.N_ITER_SP = (UINT8)cp.GetSubItem(IDX_N_ITER_SP)->GetValue().intVal;
	for (int iSP = 0; iSP < (int)p.N_ITER_SP; ++iSP)
	{
		CMFCPropertyGridProperty* iterSPNJPProperty = cp.GetSubItem(IDX_SP_ITER_Property)->GetSubItem(iSP);
		p.segmentProfileReference[iSP].NID_C = (UINT16)iterSPNJPProperty->GetSubItem(IDX_SPNJP_NID_C)->GetValue().intVal;
		p.segmentProfileReference[iSP].NID_SP = (UINT32)iterSPNJPProperty->GetSubItem(IDX_SPNJP_NID_SP)->GetValue().intVal;		
		p.segmentProfileReference[iSP].M_SP_Version = (UINT16)iterSPNJPProperty->GetSubItem(IDX_SPNJP_M_SP_Version)->GetValue().intVal;
		p.segmentProfileReference[iSP].Q_SP_DIR = (UINT8)iterSPNJPProperty->GetSubItem(IDX_SPNJP_Q_SP_DIR)->GetValue().intVal;
		p.segmentProfileReference[iSP].N_ITER = (UINT8)iterSPNJPProperty->GetSubItem(IDX_SPNJP_N_ITER_TP)->GetValue().intVal;

		// Timing point Const
		for (int iTP = 0; iTP < (int)p.segmentProfileReference[iSP].N_ITER; ++iTP)
		{
			CMFCPropertyGridProperty* iterTPNSPNJPProperty = iterSPNJPProperty->GetSubItem(IDX_TIMING_POINT_CONSTRAINTS)->GetSubItem(iTP); // fix
			//TimingPointConstraints currentTPConstraint = p.segmentProfileReference[iSP].TPConstraints[iTP];
			p.segmentProfileReference[iSP].TPConstraints[iTP].NID_TP = (UINT32)iterTPNSPNJPProperty->GetSubItem(IDX_TPNSPNJP_NID_TP)->GetValue().intVal;

			
			p.segmentProfileReference[iSP].TPConstraints[iTP].T_Latest_Arrival_Date = (UINT16)iterTPNSPNJPProperty->GetSubItem(IDX_TPNSPNJP_T_Latest_Arrival_Date)->GetValue().intVal;
			p.segmentProfileReference[iSP].TPConstraints[iTP].T_Latest_Arrival_Seconds = (UINT32)iterTPNSPNJPProperty->GetSubItem(IDX_TPNSPNJP_T_Latest_Arrival_Seconds)->GetValue().intVal;
			p.segmentProfileReference[iSP].TPConstraints[iTP].T_Arrival_Window = (UINT16)iterTPNSPNJPProperty->GetSubItem(IDX_TPNSPNJP_T_Arrival_Window)->GetValue().intVal;
			p.segmentProfileReference[iSP].TPConstraints[iTP].Q_TP_Alignment = (UINT8)iterTPNSPNJPProperty->GetSubItem(IDX_TPNSPNJP_Q_TP_Alignment)->GetValue().intVal;
			p.segmentProfileReference[iSP].TPConstraints[iTP].Q_Stop_Skip_Pass = (UINT8)iterTPNSPNJPProperty->GetSubItem(IDX_TPNSPNJP_Q_Stop_Skip_Pass)->GetValue().intVal;
			p.segmentProfileReference[iSP].TPConstraints[iTP].Q_TP_information = (UINT8)iterTPNSPNJPProperty->GetSubItem(IDX_TPNSPNJP_Q_TP_Information)->GetValue().intVal;
			p.segmentProfileReference[iSP].TPConstraints[iTP].Q_Day_Light_Saving = (UINT8)iterTPNSPNJPProperty->GetSubItem(IDX_TPNSPNJP_Q_Day_Light_Saving)->GetValue().intVal;
			p.segmentProfileReference[iSP].TPConstraints[iTP].stpInformation.Q_Opening_Door_Side = (UINT8)iterTPNSPNJPProperty->GetSubItem(IDX_TPNSPNJP_Q_Opening_Door_Side)->GetValue().intVal;
			p.segmentProfileReference[iSP].TPConstraints[iTP].stpInformation.Q_Centralised_Opening = (UINT8)iterTPNSPNJPProperty->GetSubItem(IDX_TPNSPNJP_Q_Centralised_Opening)->GetValue().intVal;
			p.segmentProfileReference[iSP].TPConstraints[iTP].stpInformation.Q_Relaxed_Coupler = (UINT8)iterTPNSPNJPProperty->GetSubItem(IDX_TPNSPNJP_Q_Relaxed_Coupler)->GetValue().intVal;
			p.segmentProfileReference[iSP].TPConstraints[iTP].stpDepartureDetails.Q_Train_Hold = (UINT8)iterTPNSPNJPProperty->GetSubItem(IDX_TPNSPNJP_Q_Train_Hold)->GetValue().intVal;
			p.segmentProfileReference[iSP].TPConstraints[iTP].stpDepartureDetails.T_Departure_Date = (UINT16)iterTPNSPNJPProperty->GetSubItem(IDX_TPNSPNJP_T_Departure_Date)->GetValue().intVal;
			p.segmentProfileReference[iSP].TPConstraints[iTP].stpDepartureDetails.T_Departure_Seconds = (UINT32)iterTPNSPNJPProperty->GetSubItem(IDX_TPNSPNJP_T_Departure_Seconds)->GetValue().intVal;
			p.segmentProfileReference[iSP].TPConstraints[iTP].stpDepartureDetails.T_Minimum_Dwell_Time = (UINT16)iterTPNSPNJPProperty->GetSubItem(IDX_TPNSPNJP_T_Minimum_Dwell_Time)->GetValue().intVal;
			p.segmentProfileReference[iSP].TPConstraints[iTP].stpDepartureDetails.Q_Automatic_Closing = (UINT8)iterTPNSPNJPProperty->GetSubItem(IDX_TPNSPNJP_Q_Automatic_Closing)->GetValue().intVal;

		}

		p.segmentProfileReference[iSP].N_ITER_tempConstraints = (UINT8)iterSPNJPProperty->GetSubItem(IDX_SPNJP_N_ITER_TC)->GetValue().intVal;

		// Temporary Const
		for (int iTC = 0; iTC < (int)p.segmentProfileReference[iSP].N_ITER_tempConstraints; ++iTC)
		{
			CMFCPropertyGridProperty* TCNSPNJPProperty = iterSPNJPProperty->GetSubItem(IDX_TEMPORARY_CONSTRAINTS)->GetSubItem(iTC);
			//TemporaryConstrains currentTemConstraint = p.segmentProfileReference[iSP].temporaryConstrains[iTC];
			p.segmentProfileReference[iSP].temporaryConstrains[iTC].D_TC_Start_Location = (UINT32)TCNSPNJPProperty->GetSubItem(IDX_TCNSPNJP_D_TC_Start_Location)->GetValue().intVal;
			p.segmentProfileReference[iSP].temporaryConstrains[iTC].Q_Range = (UINT8)TCNSPNJPProperty->GetSubItem(IDX_TCNSPNJP_Q_Range)->GetValue().intVal;
			p.segmentProfileReference[iSP].temporaryConstrains[iTC].Q_TC_Type = (UINT8)TCNSPNJPProperty->GetSubItem(IDX_TCNSPNJP_Q_TC_Type)->GetValue().intVal;
			p.segmentProfileReference[iSP].temporaryConstrains[iTC].D_TC_End_Location = (UINT32)TCNSPNJPProperty->GetSubItem(IDX_TCNSPNJP_D_TC_End_Location)->GetValue().intVal;
			p.segmentProfileReference[iSP].temporaryConstrains[iTC].asr.Q_FRONT = (UINT8)TCNSPNJPProperty->GetSubItem(IDX_TCNSPNJP_Q_FRONT)->GetValue().intVal;
			p.segmentProfileReference[iSP].temporaryConstrains[iTC].asr.V_Speed_Level = (UINT8)TCNSPNJPProperty->GetSubItem(IDX_TCNSPNJP_V_Speed_Level)->GetValue().intVal;
			p.segmentProfileReference[iSP].temporaryConstrains[iTC].lowAdhesion.Q_Adhesion_Category = (UINT8)TCNSPNJPProperty->GetSubItem(IDX_TCNSPNJP_Q_Adhesion_Category)->GetValue().intVal;
			p.segmentProfileReference[iSP].temporaryConstrains[iTC].currentLimitation.M_CURRENT = (UINT16)TCNSPNJPProperty->GetSubItem(IDX_TCNSPNJP_M_CURRENT)->GetValue().intVal;
		}

	}
}

void packet04Tab_2::PacketJPCtrlToStructureDynamic(const CMFCPropertyGridProperty* bodyProperty, TS_OB_Packet04_JPStruct& p)
{
	CTabDlg2* targetDlg = ((CATOTSSimulatortabDlg*)AfxGetMainWnd())->pDlg2;



	const CMFCPropertyGridProperty& cp = *(bodyProperty);

	p.Q_JP_Status = (UINT8)cp.GetSubItem(IDX_Q_JP_Status)->GetValue().intVal;
	p.N_ITER_SP = (UINT8)cp.GetSubItem(IDX_N_ITER_SP)->GetValue().intVal;
	for (int iSP = 0; iSP < (int)p.N_ITER_SP; ++iSP)
	{
		CMFCPropertyGridProperty* iterSPNJPProperty = cp.GetSubItem(IDX_SP_ITER_Property)->GetSubItem(iSP);
		p.segmentProfileReference[iSP].NID_C = (UINT16)iterSPNJPProperty->GetSubItem(IDX_SPNJP_NID_C)->GetValue().intVal;
		p.segmentProfileReference[iSP].NID_SP = (UINT32)iterSPNJPProperty->GetSubItem(IDX_SPNJP_NID_SP)->GetValue().intVal;		
		p.segmentProfileReference[iSP].M_SP_Version = (UINT16)iterSPNJPProperty->GetSubItem(IDX_SPNJP_M_SP_Version)->GetValue().intVal;
		p.segmentProfileReference[iSP].Q_SP_DIR = (UINT8)iterSPNJPProperty->GetSubItem(IDX_SPNJP_Q_SP_DIR)->GetValue().intVal;
		p.segmentProfileReference[iSP].N_ITER = (UINT8)iterSPNJPProperty->GetSubItem(IDX_SPNJP_N_ITER_TP)->GetValue().intVal;

		// Timing point Const
		for (int iTP = 0; iTP < (int)p.segmentProfileReference[iSP].N_ITER; ++iTP)
		{
			CMFCPropertyGridProperty* iterTPNSPNJPProperty = iterSPNJPProperty->GetSubItem(IDX_TIMING_POINT_CONSTRAINTS)->GetSubItem(iTP); // fix
			//TimingPointConstraints currentTPConstraint = p.segmentProfileReference[iSP].TPConstraints[iTP];
			p.segmentProfileReference[iSP].TPConstraints[iTP].NID_TP = (UINT32)iterTPNSPNJPProperty->GetSubItem(IDX_TPNSPNJP_NID_TP)->GetValue().intVal;

			switch (p.segmentProfileReference[iSP].TPConstraints[iTP].NID_TP)
			{
			case 66049:
				p.segmentProfileReference[iSP].TPConstraints[iTP].T_Latest_Arrival_Seconds = targetDlg->testStartSec + 4800;
				p.segmentProfileReference[iSP].TPConstraints[iTP].stpDepartureDetails.T_Departure_Seconds = p.segmentProfileReference[iSP].TPConstraints[iTP].T_Latest_Arrival_Seconds + 60;
				break;
			case 66562:
				p.segmentProfileReference[iSP].TPConstraints[iTP].T_Latest_Arrival_Seconds = targetDlg->testStartSec + 13800;
				p.segmentProfileReference[iSP].TPConstraints[iTP].stpDepartureDetails.T_Departure_Seconds = p.segmentProfileReference[iSP].TPConstraints[iTP].T_Latest_Arrival_Seconds + 60;
				break;
			case 131585:
				p.segmentProfileReference[iSP].TPConstraints[iTP].T_Latest_Arrival_Seconds = targetDlg->testStartSec + 21900;
				p.segmentProfileReference[iSP].TPConstraints[iTP].stpDepartureDetails.T_Departure_Seconds = p.segmentProfileReference[iSP].TPConstraints[iTP].T_Latest_Arrival_Seconds + 60;
				break;
			case 132098:
				p.segmentProfileReference[iSP].TPConstraints[iTP].T_Latest_Arrival_Seconds = targetDlg->testStartSec + 29100;
				p.segmentProfileReference[iSP].TPConstraints[iTP].stpDepartureDetails.T_Departure_Seconds = p.segmentProfileReference[iSP].TPConstraints[iTP].T_Latest_Arrival_Seconds + 60;
				break;
			default:
				p.segmentProfileReference[iSP].TPConstraints[iTP].T_Latest_Arrival_Seconds = (UINT32)iterTPNSPNJPProperty->GetSubItem(IDX_TPNSPNJP_T_Latest_Arrival_Seconds)->GetValue().intVal;
				p.segmentProfileReference[iSP].TPConstraints[iTP].stpDepartureDetails.T_Departure_Seconds = (UINT32)iterTPNSPNJPProperty->GetSubItem(IDX_TPNSPNJP_T_Departure_Seconds)->GetValue().intVal;
				break;
			}
			p.segmentProfileReference[iSP].TPConstraints[iTP].T_Latest_Arrival_Date = (UINT16)iterTPNSPNJPProperty->GetSubItem(IDX_TPNSPNJP_T_Latest_Arrival_Date)->GetValue().intVal;
			//p.segmentProfileReference[iSP].TPConstraints[iTP].T_Latest_Arrival_Seconds = (UINT32)iterTPNSPNJPProperty->GetSubItem(IDX_TPNSPNJP_T_Latest_Arrival_Seconds)->GetValue().intVal;
			p.segmentProfileReference[iSP].TPConstraints[iTP].T_Arrival_Window = (UINT16)iterTPNSPNJPProperty->GetSubItem(IDX_TPNSPNJP_T_Arrival_Window)->GetValue().intVal;
			p.segmentProfileReference[iSP].TPConstraints[iTP].Q_TP_Alignment = (UINT8)iterTPNSPNJPProperty->GetSubItem(IDX_TPNSPNJP_Q_TP_Alignment)->GetValue().intVal;
			p.segmentProfileReference[iSP].TPConstraints[iTP].Q_Stop_Skip_Pass = (UINT8)iterTPNSPNJPProperty->GetSubItem(IDX_TPNSPNJP_Q_Stop_Skip_Pass)->GetValue().intVal;
			p.segmentProfileReference[iSP].TPConstraints[iTP].Q_TP_information = (UINT8)iterTPNSPNJPProperty->GetSubItem(IDX_TPNSPNJP_Q_TP_Information)->GetValue().intVal;
			p.segmentProfileReference[iSP].TPConstraints[iTP].Q_Day_Light_Saving = (UINT8)iterTPNSPNJPProperty->GetSubItem(IDX_TPNSPNJP_Q_Day_Light_Saving)->GetValue().intVal;
			p.segmentProfileReference[iSP].TPConstraints[iTP].stpInformation.Q_Opening_Door_Side = (UINT8)iterTPNSPNJPProperty->GetSubItem(IDX_TPNSPNJP_Q_Opening_Door_Side)->GetValue().intVal;
			p.segmentProfileReference[iSP].TPConstraints[iTP].stpInformation.Q_Centralised_Opening = (UINT8)iterTPNSPNJPProperty->GetSubItem(IDX_TPNSPNJP_Q_Centralised_Opening)->GetValue().intVal;
			p.segmentProfileReference[iSP].TPConstraints[iTP].stpInformation.Q_Relaxed_Coupler = (UINT8)iterTPNSPNJPProperty->GetSubItem(IDX_TPNSPNJP_Q_Relaxed_Coupler)->GetValue().intVal;
			p.segmentProfileReference[iSP].TPConstraints[iTP].stpDepartureDetails.Q_Train_Hold = (UINT8)iterTPNSPNJPProperty->GetSubItem(IDX_TPNSPNJP_Q_Train_Hold)->GetValue().intVal;
			p.segmentProfileReference[iSP].TPConstraints[iTP].stpDepartureDetails.T_Departure_Date = (UINT16)iterTPNSPNJPProperty->GetSubItem(IDX_TPNSPNJP_T_Departure_Date)->GetValue().intVal;
			//p.segmentProfileReference[iSP].TPConstraints[iTP].stpDepartureDetails.T_Departure_Seconds = (UINT32)iterTPNSPNJPProperty->GetSubItem(IDX_TPNSPNJP_T_Departure_Seconds)->GetValue().intVal;
			p.segmentProfileReference[iSP].TPConstraints[iTP].stpDepartureDetails.T_Minimum_Dwell_Time = (UINT16)iterTPNSPNJPProperty->GetSubItem(IDX_TPNSPNJP_T_Minimum_Dwell_Time)->GetValue().intVal;
			p.segmentProfileReference[iSP].TPConstraints[iTP].stpDepartureDetails.Q_Automatic_Closing = (UINT8)iterTPNSPNJPProperty->GetSubItem(IDX_TPNSPNJP_Q_Automatic_Closing)->GetValue().intVal;

		}

		p.segmentProfileReference[iSP].N_ITER_tempConstraints = (UINT8)iterSPNJPProperty->GetSubItem(IDX_SPNJP_N_ITER_TC)->GetValue().intVal;

		// Temporary Const
		for (int iTC = 0; iTC < (int)p.segmentProfileReference[iSP].N_ITER_tempConstraints; ++iTC)
		{
			CMFCPropertyGridProperty* TCNSPNJPProperty = iterSPNJPProperty->GetSubItem(IDX_TEMPORARY_CONSTRAINTS)->GetSubItem(iTC);
			//TemporaryConstrains currentTemConstraint = p.segmentProfileReference[iSP].temporaryConstrains[iTC];
			p.segmentProfileReference[iSP].temporaryConstrains[iTC].D_TC_Start_Location = (UINT32)TCNSPNJPProperty->GetSubItem(IDX_TCNSPNJP_D_TC_Start_Location)->GetValue().intVal;
			p.segmentProfileReference[iSP].temporaryConstrains[iTC].Q_Range = (UINT8)TCNSPNJPProperty->GetSubItem(IDX_TCNSPNJP_Q_Range)->GetValue().intVal;
			p.segmentProfileReference[iSP].temporaryConstrains[iTC].Q_TC_Type = (UINT8)TCNSPNJPProperty->GetSubItem(IDX_TCNSPNJP_Q_TC_Type)->GetValue().intVal;
			p.segmentProfileReference[iSP].temporaryConstrains[iTC].D_TC_End_Location = (UINT32)TCNSPNJPProperty->GetSubItem(IDX_TCNSPNJP_D_TC_End_Location)->GetValue().intVal;
			p.segmentProfileReference[iSP].temporaryConstrains[iTC].asr.Q_FRONT = (UINT8)TCNSPNJPProperty->GetSubItem(IDX_TCNSPNJP_Q_FRONT)->GetValue().intVal;
			p.segmentProfileReference[iSP].temporaryConstrains[iTC].asr.V_Speed_Level = (UINT8)TCNSPNJPProperty->GetSubItem(IDX_TCNSPNJP_V_Speed_Level)->GetValue().intVal;
			p.segmentProfileReference[iSP].temporaryConstrains[iTC].lowAdhesion.Q_Adhesion_Category = (UINT8)TCNSPNJPProperty->GetSubItem(IDX_TCNSPNJP_Q_Adhesion_Category)->GetValue().intVal;
			p.segmentProfileReference[iSP].temporaryConstrains[iTC].currentLimitation.M_CURRENT = (UINT16)TCNSPNJPProperty->GetSubItem(IDX_TCNSPNJP_M_CURRENT)->GetValue().intVal;
		}

	}
}


BOOL packet04Tab_2::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(255, 255, 255));

	return TRUE;

	return CDialogEx::OnEraseBkgnd(pDC);
}


HBRUSH packet04Tab_2::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	pDC->SetBkColor(RGB(255, 255, 255)); // Static Control 전부 글자배경색 하얀색으로

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

void packet04Tab_2::propgrid()
{
	int pnum = 4;



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

	pxProperty->Expand(FALSE);

	pxProperty = new CMFCPropertyGridProperty(_T("BODY Data"));
	m_propgrid.AddProperty(pxProperty);


	CMFCPropertyGridProperty* pxBodyProperty = m_propgrid.GetProperty((int)SendProperty::BODY);


	PropertyCtrlBuilder::BuildBodyPkJP(pxBodyProperty);

}

void packet04Tab_2::OnBnClickedChbAuto()
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


void packet04Tab_2::OnCbnSelchangeCmbPacket()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_propgrid.RemoveAll();
	propgrid();
}


void packet04Tab_2::OnBnClickedBtnSend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// Sa-Conn 연결여부 확인 필요

	CTabDlg2* targetDlg = ((CATOTSSimulatortabDlg*)AfxGetMainWnd())->pDlg2;


	PacketHeaderCtrlToStructure(m_propgrid.GetProperty(HEADER), *pHeaderStructure);
	PacketJPCtrlToStructure(m_propgrid.GetProperty(BODY_DATA), *pJPStructure);

	targetDlg->Cmd_Send126(eJP);

}


void packet04Tab_2::PacketJPStructrureToINI(TS_OB_Packet04_JPStruct& p)
{
	if (&p == nullptr)return;

	CStdioFile ipfile;
	CStdioFile* pipfile = &ipfile;

	CString dir = Utility::GetDirectoryFromFileDialog(_T("*.ini"), JPDefaultConfigFileName, OFN_HIDEREADONLY, szConfigFilter);
	if (ipfile.Open(dir.GetString(), CFile::modeCreate | CFile::modeWrite) == FALSE) return;
	Utility::CFileWriteCHAR(pipfile, packetJP);
	Utility::CFileWriteCHAR(pipfile, endLine);

	CHAR tempChar[128]{};
	TextTablePacketJP& idcList = TextTablePacketJP::GetInstance();

	sprintf_s(tempChar, ": %d\n", p.Q_JP_Status);
	Utility::CFileWriteCHAR(pipfile, idcList.GetText(IDX_Q_JP_Status));
	Utility::CFileWriteCHAR(pipfile, tempChar);

	sprintf_s(tempChar, ": %d\n", p.N_ITER_SP);
	Utility::CFileWriteCHAR(pipfile, idcList.GetText(IDX_N_ITER_SP));
	Utility::CFileWriteCHAR(pipfile, tempChar);

	// SP ITER
	for (int i = 0; i < (int)p.N_ITER_SP; ++i)
	{
		sprintf_s(tempChar, "%s(%d): %u\n", "NID_C", i, p.segmentProfileReference[i].NID_C);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		sprintf_s(tempChar, "%s(%d): %u\n", "NID_SP", i, p.segmentProfileReference[i].NID_SP);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		sprintf_s(tempChar, "%s(%d): %u\n", "M_SP_Version", i, p.segmentProfileReference[i].M_SP_Version);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		sprintf_s(tempChar, "%s(%d): %u\n", "Q_SP_DIR", i, p.segmentProfileReference[i].Q_SP_DIR);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		sprintf_s(tempChar, "%s(%d): %u\n", "N_ITER", i, p.segmentProfileReference[i].N_ITER);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		// Timing Point ITER
		for (int k = 0; k < (int)p.segmentProfileReference[i].N_ITER; ++k)
		{
			sprintf_s(tempChar, "%s(%d): %u\n", "NID_TP", k, p.segmentProfileReference[i].TPConstraints[k].NID_TP);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "T_Latest_Arrival_Date", k, p.segmentProfileReference[i].TPConstraints[k].T_Latest_Arrival_Date);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "T_Latest_Arrival_Seconds", k, p.segmentProfileReference[i].TPConstraints[k].T_Latest_Arrival_Seconds);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "T_Arrival_Window", k, p.segmentProfileReference[i].TPConstraints[k].T_Arrival_Window);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "Q_TP_Alignment", k, p.segmentProfileReference[i].TPConstraints[k].Q_TP_Alignment);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "Q_Stop_Skip_Pass", k, p.segmentProfileReference[i].TPConstraints[k].Q_Stop_Skip_Pass);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "Q_TP_information", k, p.segmentProfileReference[i].TPConstraints[k].Q_TP_information);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "Q_Day_Light_Saving", k, p.segmentProfileReference[i].TPConstraints[k].Q_Day_Light_Saving);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "Q_Opening_Door_Side", k, p.segmentProfileReference[i].TPConstraints[k].stpInformation.Q_Opening_Door_Side);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "Q_Centralised_Opening", k, p.segmentProfileReference[i].TPConstraints[k].stpInformation.Q_Centralised_Opening);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "Q_Relaxed_Coupler", k, p.segmentProfileReference[i].TPConstraints[k].stpInformation.Q_Relaxed_Coupler);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "Q_Train_Hold", k, p.segmentProfileReference[i].TPConstraints[k].stpDepartureDetails.Q_Train_Hold);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "T_Departure_Date", k, p.segmentProfileReference[i].TPConstraints[k].stpDepartureDetails.T_Departure_Date);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "T_Departure_Seconds", k, p.segmentProfileReference[i].TPConstraints[k].stpDepartureDetails.T_Departure_Seconds);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "T_Minimum_Dwell_Time", k, p.segmentProfileReference[i].TPConstraints[k].stpDepartureDetails.T_Minimum_Dwell_Time);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "Q_Automatic_Closing", k, p.segmentProfileReference[i].TPConstraints[k].stpDepartureDetails.Q_Automatic_Closing);
			Utility::CFileWriteCHAR(pipfile, tempChar);
		}

		sprintf_s(tempChar, "%s(%d): %u\n", "N_ITER", i, p.segmentProfileReference[i].N_ITER_tempConstraints);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		// Temp Const ITER
		for (int Temcon = 0; Temcon < (int)p.segmentProfileReference[i].N_ITER_tempConstraints; ++Temcon)
		{
			sprintf_s(tempChar, "%s(%d): %u\n", "Q_TC_Type", Temcon, p.segmentProfileReference[i].temporaryConstrains[Temcon].Q_TC_Type);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "Q_Range", Temcon, p.segmentProfileReference[i].temporaryConstrains[Temcon].Q_Range);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "D_TC_Start_Location", Temcon, p.segmentProfileReference[i].temporaryConstrains[Temcon].D_TC_Start_Location);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "D_TC_End_Location", Temcon, p.segmentProfileReference[i].temporaryConstrains[Temcon].D_TC_End_Location);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "Q_FRONT", Temcon, p.segmentProfileReference[i].temporaryConstrains[Temcon].asr.Q_FRONT);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "V_Speed_Level", Temcon, p.segmentProfileReference[i].temporaryConstrains[Temcon].asr.V_Speed_Level);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "Q_Adhesion_Category", Temcon, p.segmentProfileReference[i].temporaryConstrains[Temcon].lowAdhesion.Q_Adhesion_Category);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "M_CURRENT", Temcon, p.segmentProfileReference[i].temporaryConstrains[Temcon].currentLimitation.M_CURRENT);
			Utility::CFileWriteCHAR(pipfile, tempChar);
		}

	}


}


BOOL packet04Tab_2::LoadPacketJPFromFile()
{
	TS_OB_Packet04_JPStruct& p = *pFileJPStructure;
	if (&p == nullptr)return FALSE;
	static CString pathName = JPDefaultConfigFileName;

	static bool initFlag = true;
	if (initFlag)
	{
		pathName = Utility::GetDirectoryFromFileDialog(_T("*.ini"), JPDefaultConfigFileName, OFN_HIDEREADONLY, szConfigFilter);
		if (pathName.Compare(_T("")) == 0) return FALSE;
		initFlag = true;
	}

	CStdioFile ipfile;
	if (!ipfile.Open(pathName, CFile::modeRead))	//Open
	{
		// 파일 열기 실패 시 기본 파일 열기 시도
		if (!ipfile.Open(JPDefaultConfigFileName, CFile::modeRead)) return FALSE;
	}

	if (Utility::CheckFileUnicode(ipfile)) return FALSE;

	// 파일 내용 복사
	ULONGLONG fileSize = ipfile.GetLength();
	CHAR* tempStr = new CHAR[fileSize + 1]; // +1 for null terminator
	ipfile.Read(tempStr, (UINT)fileSize);
	tempStr[fileSize] = '\0'; // Null terminate the string
	string configContainer(tempStr); // 파일 전체 문자열
	delete[] tempStr;

	// [Journey Profile] 문자열 검색
	size_t strIdx = configContainer.find(packetJP);
	if (strIdx != string::npos) //발견
	{
		configContainer = configContainer.substr(strIdx); // [Journey Profile]을 포함한 이후의 문자열이 남음
		LoadPacketJPFromFile_StaticData(configContainer, p);
		ipfile.Close();
		return TRUE;
	}
	else
	{
		ipfile.Close();
		return FALSE; // JP의 정보가 담긴 파일이 아님
	}
}


BOOL packet04Tab_2::initLoadPacketJPFromFile()
{
	TS_OB_Packet04_JPStruct& p = *pFileJPStructure;
	if (&p == nullptr)return FALSE;
	static CString pathName = JPDefaultConfigFileName;

	CStdioFile ipfile;
	if (ipfile.Open(pathName.GetString(), CFile::modeRead) == FALSE)	//Open
	{
		if (ipfile.Open(JPDefaultConfigFileName, CFile::modeRead) == FALSE) return FALSE;
	}
	if (Utility::CheckFileUnicode(ipfile) == TRUE) return FALSE;

	//파일 내용 복사
	ULONGLONG fileSize = ipfile.GetLength();
	CHAR* tempStr = new CHAR[fileSize];
	ipfile.Read(tempStr, (UINT)fileSize);
	string configContainer; //파일 전체 문자열
	configContainer = tempStr;
	delete[] tempStr;

	// [Journey Profile] 문자열 검색
	size_t strIdx = configContainer.find(packetJP);
	if (strIdx != string::npos) //발견
		configContainer = configContainer.substr(strIdx); // [Journey Profile]을 포함한 이후의 문자열이 남음
	else return FALSE; //JP 의 정보가 담긴 파일이 아님

	LoadPacketJPFromFile_StaticData(configContainer, p);

	ipfile.Close();

	ReLoadPacketJP();

	return TRUE;
}


void packet04Tab_2::LoadPacketJPFromFile_StaticData(string& configTXT, TS_OB_Packet04_JPStruct& p)
{
	//size_t strIdx, strEndlineIdx;
	//string currentLine;


	TextTablePacketJP& idcList = TextTablePacketJP::GetInstance();

	LoadPacket(configTXT, idcList.GetText(IDX_Q_JP_Status), p.Q_JP_Status);
	LoadPacket(configTXT, idcList.GetText(IDX_N_ITER_SP), p.N_ITER_SP);

	CHAR iterIdc[64];

	for (int i = 0; i < (int)p.N_ITER_SP; ++i)
	{
		sprintf_s(iterIdc, "%s(%d)", "NID_C", i);
		LoadPacket(configTXT, iterIdc, p.segmentProfileReference[i].NID_C);

		sprintf_s(iterIdc, "%s(%d)", "NID_SP", i);
		LoadPacket(configTXT, iterIdc, p.segmentProfileReference[i].NID_SP);

		sprintf_s(iterIdc, "%s(%d)", "M_SP_Version", i);
		LoadPacket(configTXT, iterIdc, p.segmentProfileReference[i].M_SP_Version);

		sprintf_s(iterIdc, "%s(%d)", "Q_SP_DIR", i);
		LoadPacket(configTXT, iterIdc, p.segmentProfileReference[i].Q_SP_DIR);

		sprintf_s(iterIdc, "%s(%d)", "N_ITER", i);
		LoadPacket(configTXT, iterIdc, p.segmentProfileReference[i].N_ITER);

		for (int k = 0; k < (int)p.segmentProfileReference[i].N_ITER; ++k)
		{
			sprintf_s(iterIdc, "%s(%d)", "NID_TP", k);
			LoadPacket(configTXT, iterIdc, p.segmentProfileReference[i].TPConstraints[k].NID_TP);

			sprintf_s(iterIdc, "%s(%d)", "T_Latest_Arrival_Date", k);
			LoadPacket(configTXT, iterIdc, p.segmentProfileReference[i].TPConstraints[k].T_Latest_Arrival_Date);

			sprintf_s(iterIdc, "%s(%d)", "T_Latest_Arrival_Seconds", k);
			LoadPacket(configTXT, iterIdc, p.segmentProfileReference[i].TPConstraints[k].T_Latest_Arrival_Seconds);

			sprintf_s(iterIdc, "%s(%d)", "T_Arrival_Window", k);
			LoadPacket(configTXT, iterIdc, p.segmentProfileReference[i].TPConstraints[k].T_Arrival_Window);

			sprintf_s(iterIdc, "%s(%d)", "Q_TP_Alignment", k);
			LoadPacket(configTXT, iterIdc, p.segmentProfileReference[i].TPConstraints[k].Q_TP_Alignment);

			sprintf_s(iterIdc, "%s(%d)", "Q_Stop_Skip_Pass", k);
			LoadPacket(configTXT, iterIdc, p.segmentProfileReference[i].TPConstraints[k].Q_Stop_Skip_Pass);

			sprintf_s(iterIdc, "%s(%d)", "Q_TP_Information", k);
			LoadPacket(configTXT, iterIdc, p.segmentProfileReference[i].TPConstraints[k].Q_TP_information);

			sprintf_s(iterIdc, "%s(%d)", "Q_Day_Light_Saving", k);
			LoadPacket(configTXT, iterIdc, p.segmentProfileReference[i].TPConstraints[k].Q_Day_Light_Saving);

			sprintf_s(iterIdc, "%s(%d)", "Q_Opening_Door_Side", k);
			LoadPacket(configTXT, iterIdc, p.segmentProfileReference[i].TPConstraints[k].stpInformation.Q_Opening_Door_Side);

			sprintf_s(iterIdc, "%s(%d)", "Q_Centralised_Opening", k);
			LoadPacket(configTXT, iterIdc, p.segmentProfileReference[i].TPConstraints[k].stpInformation.Q_Centralised_Opening);

			sprintf_s(iterIdc, "%s(%d)", "Q_Relaxed_Coupler", k);
			LoadPacket(configTXT, iterIdc, p.segmentProfileReference[i].TPConstraints[k].stpInformation.Q_Relaxed_Coupler);

			sprintf_s(iterIdc, "%s(%d)", "Q_Train_Hold", k);
			LoadPacket(configTXT, iterIdc, p.segmentProfileReference[i].TPConstraints[k].stpDepartureDetails.Q_Train_Hold);

			sprintf_s(iterIdc, "%s(%d)", "T_Departure_Date", k);
			LoadPacket(configTXT, iterIdc, p.segmentProfileReference[i].TPConstraints[k].stpDepartureDetails.T_Departure_Date);

			sprintf_s(iterIdc, "%s(%d)", "T_Departure_Seconds", k);
			LoadPacket(configTXT, iterIdc, p.segmentProfileReference[i].TPConstraints[k].stpDepartureDetails.T_Departure_Seconds);

			sprintf_s(iterIdc, "%s(%d)", "T_Minimum_Dwell_Time", k);
			LoadPacket(configTXT, iterIdc, p.segmentProfileReference[i].TPConstraints[k].stpDepartureDetails.T_Minimum_Dwell_Time);

			sprintf_s(iterIdc, "%s(%d)", "Q_Automatic_Closing", k);
			LoadPacket(configTXT, iterIdc, p.segmentProfileReference[i].TPConstraints[k].stpDepartureDetails.Q_Automatic_Closing);
		}

		sprintf_s(iterIdc, "%s(%d)", "N_ITER", i);
		LoadPacket(configTXT, iterIdc, p.segmentProfileReference[i].N_ITER_tempConstraints);

		for (int k = 0; k < (int)p.segmentProfileReference[i].N_ITER_tempConstraints; ++k)
		{
			sprintf_s(iterIdc, "%s(%d)", "Q_TC_Type", k);
			LoadPacket(configTXT, iterIdc, p.segmentProfileReference[i].temporaryConstrains[k].Q_TC_Type);

			sprintf_s(iterIdc, "%s(%d)", "Q_Range", k);
			LoadPacket(configTXT, iterIdc, p.segmentProfileReference[i].temporaryConstrains[k].Q_Range);

			sprintf_s(iterIdc, "%s(%d)", "D_TC_Start_Location", k);
			LoadPacket(configTXT, iterIdc, p.segmentProfileReference[i].temporaryConstrains[k].D_TC_Start_Location);

			sprintf_s(iterIdc, "%s(%d)", "D_TC_End_Location", k);
			LoadPacket(configTXT, iterIdc, p.segmentProfileReference[i].temporaryConstrains[k].D_TC_End_Location);

			sprintf_s(iterIdc, "%s(%d)", "Q_FRONT", k);
			LoadPacket(configTXT, iterIdc, p.segmentProfileReference[i].temporaryConstrains[k].asr.Q_FRONT);

			sprintf_s(iterIdc, "%s(%d)", "V_Speed_Level", k);
			LoadPacket(configTXT, iterIdc, p.segmentProfileReference[i].temporaryConstrains[k].asr.V_Speed_Level);

			sprintf_s(iterIdc, "%s(%d)", "Q_Adhesion_Category", k);
			LoadPacket(configTXT, iterIdc, p.segmentProfileReference[i].temporaryConstrains[k].lowAdhesion.Q_Adhesion_Category);

			sprintf_s(iterIdc, "%s(%d)", "M_CURRENT", k);
			LoadPacket(configTXT, iterIdc, p.segmentProfileReference[i].temporaryConstrains[k].currentLimitation.M_CURRENT);
		}
	}

}


void packet04Tab_2::ReLoadPacketJP()
{
	PropertyCtrlBuilder::PacketJPStructureToCtrl(*pFileJPStructure, m_propgrid.GetProperty(BODY_DATA));
}




void packet04Tab_2::OnBnClickedBtnSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


		//Ctrl -> Structure -> File

	PacketJPCtrlToStructure(m_propgrid.GetProperty(BODY_DATA), *pFileJPStructure);

	PacketJPStructrureToINI(*pFileJPStructure);


}





void packet04Tab_2::OnBnClickedBtnLoad()
{


	if (LoadPacketJPFromFile() == FALSE) return; //파일 읽기 실패 시 그대로 종료
	ReLoadPacketJP();


}

int packet04Tab_2::getPacketCounter()
{
	m_nPacketCounter++;

	if (m_nPacketCounter > 255)
		m_nPacketCounter = -1;

	return m_nPacketCounter;
}


void packet04Tab_2::OnBnClickedButton1()
{
	BYTE	buffer[1024]{};
	UINT32  nPos = 0;
	int bodylength = 0;
	CString csBody;
	CString csTmp;
	CStdioFile ipfile;
	CStdioFile* pipfile = &ipfile;

	PacketJPCtrlToStructure(m_propgrid.GetProperty(BODY_DATA), *pRDJPStructure);

	EncodeMessage_ATO_Packet04_Jp(pRDJPStructure, &nPos, buffer, sizeof(buffer));

	bodylength = (GetLengthPacket04_Jp(pRDJPStructure) - GetATOHeaderSize()) % 8;

	if (bodylength == 0)
	{
		bodylength = (GetLengthPacket04_Jp(pRDJPStructure) - GetATOHeaderSize()) / 8;
	}

	else
	{
		bodylength = ((GetLengthPacket04_Jp(pRDJPStructure) - GetATOHeaderSize()) / 8) + 1;
	}

	memcpy(buffer, buffer, bodylength);

	CString dir = Utility::GetDirectoryFromFileDialog(_T("*.ini"), _T("JP_Raw_Data"), OFN_HIDEREADONLY, szConfigFilter);
	if (ipfile.Open(dir.GetString(), CFile::modeCreate | CFile::modeWrite) == FALSE) return;
	Utility::CFileWriteCHAR(pipfile, "[Jounrey Profile Raw Data]");
	Utility::CFileWriteCHAR(pipfile, endLine);

	for (int i = 0; i < bodylength; i++)
	{
		csTmp.Format(_T("%02x "), buffer[i]);
		csBody += csTmp;
	}

	csTmp.Format(_T("%04d"), bodylength);

	ipfile.WriteString(_T("["));
	ipfile.WriteString(csTmp);
	ipfile.WriteString(_T("]"));
	ipfile.WriteString(_T(" "));

	Utility::CFileWriteCHAR(pipfile, csBody);
}


BOOL packet04Tab_2::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}


void packet04Tab_2::OnBnClickedBtnSendDynamic()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CTabDlg2* targetDlg = ((CATOTSSimulatortabDlg*)AfxGetMainWnd())->pDlg2;


	PacketHeaderCtrlToStructure(m_propgrid.GetProperty(HEADER), *pHeaderStructure);
	PacketJPCtrlToStructureDynamic(m_propgrid.GetProperty(BODY_DATA), *pJPStructure);

	targetDlg->Cmd_Send126(eJP);
}

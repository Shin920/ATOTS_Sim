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
#include "packet07Tab.h"
#include "PropertyCtrlBuilder.h"
#include "TextTable.h"
#include "F110_Msg_Encoder_ATO.h"


#define _UINT16_GETVALUE_(Property, ID) (UINT16)Property.GetSubItem(ID)->GetValue().intVal;

#define _UINT32_GETVALUE_(Property, ID) (UINT32)Property->GetSubItem(ID)->GetValue().intVal;

using namespace PacketHeaderEnum;
using namespace PacketSPEnum;



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

	// 문자열 검색
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

IMPLEMENT_DYNAMIC(packet07Tab, CDialogEx)

packet07Tab::packet07Tab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_SEND_4, pParent), pSPStructure(nullptr)
{
	Init();
}

packet07Tab::~packet07Tab()
{
	Release();
}

void packet07Tab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MPG_SEND, m_propgrid);
	DDX_Control(pDX, IDC_CHB_AUTO, m_chb_auto);
	DDX_Control(pDX, IDC_CMB_PACKET, m_cmb_send);
}


BEGIN_MESSAGE_MAP(packet07Tab, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHB_AUTO, &packet07Tab::OnBnClickedChbAuto)
	ON_CBN_SELCHANGE(IDC_CMB_PACKET, &packet07Tab::OnCbnSelchangeCmbPacket)
	ON_BN_CLICKED(IDC_BTN_SEND, &packet07Tab::OnBnClickedBtnSend)
	ON_BN_CLICKED(IDC_BTN_SAVE, &packet07Tab::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_LOAD, &packet07Tab::OnBnClickedBtnLoad)
	ON_BN_CLICKED(IDC_BUTTON1, &packet07Tab::OnBnClickedButton1)
END_MESSAGE_MAP()


// CSendTab 메시지 처리기

void packet07Tab::Init()
{
	pHeaderStructure = new ATO_Header;
	pSPStructure = new TS_OB_Packet07_SPStruct;
		
	pFileSPStructure = new TS_OB_Packet07_SPStruct;
	pRDSPStructure = new TS_OB_Packet07_SPStruct;

	m_nPacketCounter = -1;
}

void packet07Tab::Release()
{
	delete pHeaderStructure;
	delete pSPStructure;
	delete pFileSPStructure;
	delete pRDSPStructure;
	
}

BOOL packet07Tab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.


	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_cmb_send.AddString(_T("Send : Segment Profile"));
	m_cmb_send.SetCurSel(0);	//UI



	m_chb_auto.SetCheck(BST_CHECKED);

	//  테스트 편의성
	//	GetDlgItem(IDC_BTN_SEND)->EnableWindow(FALSE);



	propgrid();
	initLoadPacketSPFromFile();


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}



void packet07Tab::PacketHeaderCtrlToStructure(const CMFCPropertyGridProperty* bodyProperty, ATO_Header& p)
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


void packet07Tab::PacketSPCtrlToStructure(const CMFCPropertyGridProperty* bodyProperty, TS_OB_Packet07_SPStruct& p)
{

	const CMFCPropertyGridProperty& cp = *(bodyProperty);
	p.N_ITER_ATO = (UINT8)cp.GetSubItem(IDX_SP_N_ITER)->GetValue().intVal;

	// SP iter
	for (int iSP = 0; iSP < (int)p.N_ITER_ATO; ++iSP)
	{
		CMFCPropertyGridProperty* iterSPNSPProperty = cp.GetSubItem(IDX_SPNSP_ITER_Property)->GetSubItem(iSP);

		p.SPStatus[iSP].NID_C = (UINT16)iterSPNSPProperty->GetSubItem(IDX_SPNSP_NID_C)->GetValue().intVal;
		p.SPStatus[iSP].NID_SP_1 = (UINT32)iterSPNSPProperty->GetSubItem(IDX_SPNSP_NID_SP)->GetValue().intVal;
		p.SPStatus[iSP].Q_SP_Status = (UINT8)iterSPNSPProperty->GetSubItem(IDX_SPNSP_Q_SP_Status)->GetValue().intVal;
		p.SPStatus[iSP].spStatusValid.M_SP_Version = (UINT16)iterSPNSPProperty->GetSubItem(IDX_SPNSP_M_SP_Version)->GetValue().intVal;
		p.SPStatus[iSP].spStatusValid.L_SP = (UINT32)iterSPNSPProperty->GetSubItem(IDX_SPNSP_L_SP)->GetValue().intVal;
		p.SPStatus[iSP].spStatusValid.D_EOA_Offset = (UINT32)iterSPNSPProperty->GetSubItem(IDX_SPNSP_D_EoA_Offset)->GetValue().intVal;
		p.SPStatus[iSP].spStatusValid.Q_UTC_Offset = (UINT8)iterSPNSPProperty->GetSubItem(IDX_SPNSP_Q_UTC_Offset)->GetValue().intVal;
		p.SPStatus[iSP].spStatusValid.M_SP_Altitude = (UINT8)iterSPNSPProperty->GetSubItem(IDX_SPNSP_M_SP_Altitude)->GetValue().intVal;
		p.SPStatus[iSP].spStatusValid.Q_ATOTS_Contact_Info_Dir = (UINT8)iterSPNSPProperty->GetSubItem(IDX_SPNSP_Q_ATOTS_Contact_Info_Dir)->GetValue().intVal;
		p.SPStatus[iSP].atoTSContactInfo.NID_C_1 = (UINT16)iterSPNSPProperty->GetSubItem(IDX_SPNSP_NID_C_TS)->GetValue().intVal;
		p.SPStatus[iSP].atoTSContactInfo.NID_ATOTS = (UINT16)iterSPNSPProperty->GetSubItem(IDX_SPNSP_NID_ATOTS)->GetValue().intVal;
		p.SPStatus[iSP].atoTSContactInfo.NID_C_2 = (UINT16)iterSPNSPProperty->GetSubItem(IDX_SPNSP_NID_C_SPC)->GetValue().intVal;
		p.SPStatus[iSP].atoTSContactInfo.NID_SP_2 = (UINT32)iterSPNSPProperty->GetSubItem(IDX_SPNSP_NID_SP_F)->GetValue().intVal;
		p.SPStatus[iSP].V_STATIC = (UINT8)iterSPNSPProperty->GetSubItem(IDX_SPNSP_V_STATIC)->GetValue().intVal;
		p.SPStatus[iSP].Q_FRONT = (UINT8)iterSPNSPProperty->GetSubItem(IDX_SPNSP_Q_FRONT)->GetValue().intVal;
		p.SPStatus[iSP].N_ITER_ATO_1 = (UINT8)iterSPNSPProperty->GetSubItem(IDX_SPNSP_N_ITER_SSP)->GetValue().intVal;

		// SSP iter (2)
		for (int iSSP = 0; iSSP < (int)p.SPStatus[iSP].N_ITER_ATO_1; ++iSSP)
		{
			CMFCPropertyGridProperty* iterSSPNSPNSPProperty = iterSPNSPProperty->GetSubItem(IDX_SPNSP_SSP_ITER_Property)->GetSubItem(iSSP);
			p.SPStatus[iSP].specificSSP[iSSP].Q_DIFF = (UINT8)iterSSPNSPNSPProperty->GetSubItem(IDX_Q_DIFF_SSP)->GetValue().intVal;
			p.SPStatus[iSP].specificSSP[iSSP].NC_CDDIFF = (UINT8)iterSSPNSPNSPProperty->GetSubItem(IDX_NC_CDDIFF_SSP)->GetValue().intVal;
			p.SPStatus[iSP].specificSSP[iSSP].NC_DIFF = (UINT8)iterSSPNSPNSPProperty->GetSubItem(IDX_NC_DIFF_SSP)->GetValue().intVal;
			p.SPStatus[iSP].specificSSP[iSSP].V_DIFF = (UINT8)iterSSPNSPNSPProperty->GetSubItem(IDX_V_DIFF_SSP)->GetValue().intVal;

		}

		p.SPStatus[iSP].N_ITER_ATO_2 = (UINT8)iterSPNSPProperty->GetSubItem(IDX_SPNSP_N_ITER_SSPC)->GetValue().intVal;

		//SSPC iter (2)
		for (int iSSPC = 0; iSSPC < (int)p.SPStatus[iSP].N_ITER_ATO_2; ++iSSPC)
		{
			CMFCPropertyGridProperty* iterSSPCNSPNSPProperty = iterSPNSPProperty->GetSubItem(IDX_SPNSP_SSPC_ITER_Property)->GetSubItem(iSSPC);
			//StaticSPChange currentSSPC = p.SPStatus[iSP].staticSPChange[iSSPC];

			p.SPStatus[iSP].staticSPChange[iSSPC].D_Location = (UINT32)iterSSPCNSPNSPProperty->GetSubItem(IDX_D_Location_SSPC)->GetValue().intVal;
			p.SPStatus[iSP].staticSPChange[iSSPC].V_STATIC = (UINT8)iterSSPCNSPNSPProperty->GetSubItem(IDX_V_STATIC_SSPC)->GetValue().intVal;
			p.SPStatus[iSP].staticSPChange[iSSPC].Q_FRONT = (UINT8)iterSSPCNSPNSPProperty->GetSubItem(IDX_Q_FRONT_SSPC)->GetValue().intVal;
			p.SPStatus[iSP].staticSPChange[iSSPC].N_ITER = (UINT8)iterSSPCNSPNSPProperty->GetSubItem(IDX_N_ITER_SSPC_N)->GetValue().intVal;

			//SSPC profile iter (3)
			for (int iSSPCF = 0; iSSPCF < (int)p.SPStatus[iSP].staticSPChange[iSSPC].N_ITER; ++iSSPCF)
			{
				CMFCPropertyGridProperty* iterSSPCPProperty = iterSSPCNSPNSPProperty->GetSubItem(IDX_SPNSP_SSPC_P_ITER_Property)->GetSubItem(iSSPCF);

				p.SPStatus[iSP].staticSPChange[iSSPC].specificSspChange[iSSPCF].Q_DIFF = (UINT8)iterSSPCPProperty->GetSubItem(IDX_Q_DIFF_N)->GetValue().intVal;
				p.SPStatus[iSP].staticSPChange[iSSPC].specificSspChange[iSSPCF].NC_CDDIFF = (UINT8)iterSSPCPProperty->GetSubItem(IDX_NC_CDDIFF_N)->GetValue().intVal;
				p.SPStatus[iSP].staticSPChange[iSSPC].specificSspChange[iSSPCF].NC_DIFF = (UINT8)iterSSPCPProperty->GetSubItem(IDX_NC_DIFF_N)->GetValue().intVal;
				p.SPStatus[iSP].staticSPChange[iSSPC].specificSspChange[iSSPCF].V_DIFF = (UINT8)iterSSPCPProperty->GetSubItem(IDX_V_DIFF_N)->GetValue().intVal;
			}
		}

		p.SPStatus[iSP].G_New_Gradient = (UINT16)iterSPNSPProperty->GetSubItem(IDX_SPNSP_G_New_Gradient)->GetValue().intVal;
		p.SPStatus[iSP].Q_GDIR = (UINT8)iterSPNSPProperty->GetSubItem(IDX_SPNSP_Q_GDIR)->GetValue().intVal;
		p.SPStatus[iSP].N_ITER_ATO_3 = (UINT8)iterSPNSPProperty->GetSubItem(IDX_SPNSP_N_ITER_GC)->GetValue().intVal;

		//GC iter (2)
		for (int iGC = 0; iGC < (int)p.SPStatus[iSP].N_ITER_ATO_3; ++iGC)
		{
			CMFCPropertyGridProperty* iterGCProperty = iterSPNSPProperty->GetSubItem(IDX_SPNSP_GC_ITER_Property)->GetSubItem(iGC);

			p.SPStatus[iSP].gradientChange[iGC].D_Location = (UINT32)iterGCProperty->GetSubItem(IDX_D_Location_GC)->GetValue().intVal;
			p.SPStatus[iSP].gradientChange[iGC].G_New_Gradient = (UINT16)iterGCProperty->GetSubItem(IDX_G_New_Gradient_GC)->GetValue().intVal;
			p.SPStatus[iSP].gradientChange[iGC].Q_GDIR = (UINT8)iterGCProperty->GetSubItem(IDX_Q_GDIR_GC)->GetValue().intVal;
		}

		p.SPStatus[iSP].Q_Radius_Category = (UINT8)iterSPNSPProperty->GetSubItem(IDX_SPNSP_Q_Radius_Category)->GetValue().intVal;
		p.SPStatus[iSP].N_ITER_ATO_4 = (UINT8)iterSPNSPProperty->GetSubItem(IDX_SPNSP_N_ITER_CC)->GetValue().intVal;

		//CC iter (2)
		for (int iCC = 0; iCC < (int)p.SPStatus[iSP].N_ITER_ATO_4; ++iCC)
		{
			CMFCPropertyGridProperty* iterCCProperty = iterSPNSPProperty->GetSubItem(IDX_SPNSP_CC_ITER_Property)->GetSubItem(iCC);

			p.SPStatus[iSP].curvesChange[iCC].D_Location = (UINT32)iterCCProperty->GetSubItem(IDX_D_Location_CC)->GetValue().intVal;
			p.SPStatus[iSP].curvesChange[iCC].Q_Radius_Category = (UINT8)iterCCProperty->GetSubItem(IDX_Q_Radius_Category_CC)->GetValue().intVal;
		}

		p.SPStatus[iSP].M_VOLTAGE = (UINT8)iterSPNSPProperty->GetSubItem(IDX_SPNSP_M_VOLTAGE)->GetValue().intVal;
		p.SPStatus[iSP].NID_CTRACTION = (UINT16)iterSPNSPProperty->GetSubItem(IDX_SPNSP_NID_CTRACTION)->GetValue().intVal;
		p.SPStatus[iSP].N_ITER_ATO_5 = (UINT8)iterSPNSPProperty->GetSubItem(IDX_SPNSP_N_ITER_PVC)->GetValue().intVal;

		//PVC iter (2)
		for (int iPVC = 0; iPVC < (int)p.SPStatus[iSP].N_ITER_ATO_5; ++iPVC)
		{
			CMFCPropertyGridProperty* iterPVCProperty = iterSPNSPProperty->GetSubItem(IDX_SPNSP_PVC_ITER_Property)->GetSubItem(iPVC);

			p.SPStatus[iSP].powerVoltageChange[iPVC].D_Location = (UINT32)iterPVCProperty->GetSubItem(IDX_D_Location_PVC)->GetValue().intVal;
			p.SPStatus[iSP].powerVoltageChange[iPVC].M_VOLTAGE = (UINT8)iterPVCProperty->GetSubItem(IDX_M_VOLTAGE_PVC)->GetValue().intVal;
			p.SPStatus[iSP].powerVoltageChange[iPVC].NID_CTRACTION = (UINT8)iterPVCProperty->GetSubItem(IDX_NID_CTRACTION_PVC)->GetValue().intVal;
		}

		p.SPStatus[iSP].M_CURRENT = (UINT16)iterSPNSPProperty->GetSubItem(IDX_SPNSP_M_CURRENT)->GetValue().intVal;
		p.SPStatus[iSP].N_ITER_ATO_6 = (UINT8)iterSPNSPProperty->GetSubItem(IDX_SPNSP_N_ITER_CLC)->GetValue().intVal;

		//CLC iter (2)
		for (int iCLC = 0; iCLC < (int)p.SPStatus[iSP].N_ITER_ATO_6; ++iCLC)
		{
			CMFCPropertyGridProperty* iterCLCProperty = iterSPNSPProperty->GetSubItem(IDX_SPNSP_CLC_ITER_Property)->GetSubItem(iCLC);

			p.SPStatus[iSP].currentLimitChange[iCLC].D_Location = (UINT32)iterCLCProperty->GetSubItem(IDX_D_Location_CLC)->GetValue().intVal;
			p.SPStatus[iSP].currentLimitChange[iCLC].M_CURRENT = (UINT16)iterCLCProperty->GetSubItem(IDX_M_CURRENT_CLC)->GetValue().intVal;
		}

		p.SPStatus[iSP].N_ITER_ATO_7 = (UINT8)iterSPNSPProperty->GetSubItem(IDX_SPNSP_N_ITER_BG)->GetValue().intVal;

		//BG iter (2)
		for (int iBG = 0; iBG < (int)p.SPStatus[iSP].N_ITER_ATO_7; ++iBG)
		{
			CMFCPropertyGridProperty* iterBGProperty = iterSPNSPProperty->GetSubItem(IDX_SPNSP_BG_ITER_Property)->GetSubItem(iBG);

			p.SPStatus[iSP].baliseGroup[iBG].Q_NEWNID_C = (UINT8)iterBGProperty->GetSubItem(IDX_Q_NEWNID_C)->GetValue().intVal;
			p.SPStatus[iSP].baliseGroup[iBG].NID_C = (UINT16)iterBGProperty->GetSubItem(IDX_NID_C_BG)->GetValue().intVal;
			p.SPStatus[iSP].baliseGroup[iBG].NID_BG = (UINT16)iterBGProperty->GetSubItem(IDX_NID_BG)->GetValue().intVal;
			p.SPStatus[iSP].baliseGroup[iBG].N_ITER_BG = (UINT8)iterBGProperty->GetSubItem(IDX_N_ITER_BG)->GetValue().intVal;

			//Balises iter (3)
			for (int iB = 0; iB < (int)p.SPStatus[iSP].baliseGroup[iBG].N_ITER_BG; ++iB)
			{
				CMFCPropertyGridProperty* iterBProperty = iterBGProperty->GetSubItem(IDX_SPNSP_B_ITER_Property)->GetSubItem(iB);

				p.SPStatus[iSP].baliseGroup[iBG].balises[iB].N_PIG = (UINT8)iterBProperty->GetSubItem(IDX_N_PIG)->GetValue().intVal;
				p.SPStatus[iSP].baliseGroup[iBG].balises[iB].D_Location = (UINT32)iterBProperty->GetSubItem(IDX_D_Location_N)->GetValue().intVal;
			}

		}

		p.SPStatus[iSP].N_ITER_ATO_8 = (UINT8)iterSPNSPProperty->GetSubItem(IDX_SPNSP_N_ITER_TP)->GetValue().intVal;

		//TP iter (2)
		for (int iTP = 0; iTP < (int)p.SPStatus[iSP].N_ITER_ATO_8; ++iTP)
		{
			CMFCPropertyGridProperty* iterTPProperty = iterSPNSPProperty->GetSubItem(IDX_SPNSP_TP_ITER_Property)->GetSubItem(iTP);

			p.SPStatus[iSP].timingPoints[iTP].NID_TP = (UINT32)iterTPProperty->GetSubItem(IDX_NID_TP)->GetValue().intVal;
			p.SPStatus[iSP].timingPoints[iTP].D_Location = (UINT32)iterTPProperty->GetSubItem(IDX_D_Location_TP)->GetValue().intVal;
			p.SPStatus[iSP].timingPoints[iTP].Q_Stop_Location_Tolerance = (UINT8)iterTPProperty->GetSubItem(IDX_Q_Stop_Location_Tolerance)->GetValue().intVal;

			p.SPStatus[iSP].timingPoints[iTP].Q_STP_Reached = (UINT8)iterTPProperty->GetSubItem(IDX_Q_STP_Reached)->GetValue().intVal;			
			p.SPStatus[iSP].timingPoints[iTP].L_TEXT = (UINT8)iterTPProperty->GetSubItem(IDX_L_TEXT)->GetValue().intVal;

			//TPN iter (3)
			for (int iTPN = 0; iTPN < (int)p.SPStatus[iSP].timingPoints[iTP].L_TEXT; ++iTPN)
			{
				CMFCPropertyGridProperty* iterTPNProperty = iterTPProperty->GetSubItem(IDX_SPNSP_LTEXT_ITER_Property)->GetSubItem(iTPN);

				p.SPStatus[iSP].timingPoints[iTP].X_TEXT[iTPN] = (UINT8)iterTPNProperty->GetSubItem(IDX_X_TEXT)->GetValue().intVal;
			}
		}

		p.SPStatus[iSP].N_ITER_ATO_9 = (UINT8)iterSPNSPProperty->GetSubItem(IDX_SPNSP_N_ITER_PA)->GetValue().intVal;

		//PA iter (2)
		for (int iPA = 0; iPA < (int)p.SPStatus[iSP].N_ITER_ATO_9; ++iPA)
		{
			CMFCPropertyGridProperty* iterPAProperty = iterSPNSPProperty->GetSubItem(IDX_SPNSP_PA_ITER_Property)->GetSubItem(iPA);

			p.SPStatus[iSP].platformArea[iPA].Q_Range = (UINT8)iterPAProperty->GetSubItem(IDX_Q_Range_PA)->GetValue().intVal;
			p.SPStatus[iSP].platformArea[iPA].D_Start_Location = (UINT32)iterPAProperty->GetSubItem(IDX_D_Start_Location_PA)->GetValue().intVal;
			p.SPStatus[iSP].platformArea[iPA].D_End_Location = (UINT32)iterPAProperty->GetSubItem(IDX_D_End_Location_PA)->GetValue().intVal;
		}

		p.SPStatus[iSP].N_ITER_ATO_10 = (UINT8)iterSPNSPProperty->GetSubItem(IDX_SPNSP_N_ITER_T)->GetValue().intVal;

		//Tunnel iter (2)
		for (int iTN = 0; iTN < (int)p.SPStatus[iSP].N_ITER_ATO_10; ++iTN)
		{
			CMFCPropertyGridProperty* iterTNProperty = iterSPNSPProperty->GetSubItem(IDX_SPNSP_T_ITER_Property)->GetSubItem(iTN);

			p.SPStatus[iSP].tunnel[iTN].Q_Range = (UINT8)iterTNProperty->GetSubItem(IDX_Q_Range_T)->GetValue().intVal;
			p.SPStatus[iSP].tunnel[iTN].Q_Tunnel_Category = (UINT8)iterTNProperty->GetSubItem(IDX_Q_Tunnel_Category)->GetValue().intVal;
			p.SPStatus[iSP].tunnel[iTN].D_Start_Location = (UINT32)iterTNProperty->GetSubItem(IDX_D_Start_Location_T)->GetValue().intVal;
			p.SPStatus[iSP].tunnel[iTN].D_End_Location = (UINT32)iterTNProperty->GetSubItem(IDX_D_End_Location_T)->GetValue().intVal;
		}

		p.SPStatus[iSP].N_ITER_ATO_11 = (UINT8)iterSPNSPProperty->GetSubItem(IDX_SPNSP_N_ITER_ALSP)->GetValue().intVal;

		//ALSP iter (2)
		for (int iAL = 0; iAL < (int)p.SPStatus[iSP].N_ITER_ATO_11; ++iAL)
		{
			CMFCPropertyGridProperty* iterALSPProperty = iterSPNSPProperty->GetSubItem(IDX_SPNSP_ALSP_ITER_Property)->GetSubItem(iAL);

			p.SPStatus[iSP].axleLoadSpeedProfile[iAL].Q_Range = (UINT8)iterALSPProperty->GetSubItem(IDX_Q_Range_ALSP)->GetValue().intVal;
			p.SPStatus[iSP].axleLoadSpeedProfile[iAL].M_AXLELOADCAT = (UINT8)iterALSPProperty->GetSubItem(IDX_M_AXLELOADCAT)->GetValue().intVal;
			p.SPStatus[iSP].axleLoadSpeedProfile[iAL].V_New_Speed_Level = (UINT8)iterALSPProperty->GetSubItem(IDX_V_New_Speed_Level)->GetValue().intVal;
			p.SPStatus[iSP].axleLoadSpeedProfile[iAL].Q_FRONT = (UINT8)iterALSPProperty->GetSubItem(IDX_Q_FRONT_ALSP)->GetValue().intVal;
			p.SPStatus[iSP].axleLoadSpeedProfile[iAL].D_Start_Location = (UINT32)iterALSPProperty->GetSubItem(IDX_D_Start_Location_ALSP)->GetValue().intVal;
			p.SPStatus[iSP].axleLoadSpeedProfile[iAL].D_End_Location = (UINT32)iterALSPProperty->GetSubItem(IDX_D_End_Location_ALSP)->GetValue().intVal;
		}

		p.SPStatus[iSP].N_ITER_ATO_12 = (UINT8)iterSPNSPProperty->GetSubItem(IDX_SPNSP_N_ITER_ULCS)->GetValue().intVal;

		//ULCS iter (2)
		for (int iUL = 0; iUL < (int)p.SPStatus[iSP].N_ITER_ATO_12; ++iUL)
		{
			CMFCPropertyGridProperty* iterULCSProperty = iterSPNSPProperty->GetSubItem(IDX_SPNSP_ULCS_ITER_Property)->GetSubItem(iUL);

			p.SPStatus[iSP].unprotectedLevelCrossingStop[iUL].D_UnprotectedLx_Stop_Nominal = (UINT32)iterULCSProperty->GetSubItem(IDX_D_UnprotectedLx_Stop_Nominal)->GetValue().intVal;
			p.SPStatus[iSP].unprotectedLevelCrossingStop[iUL].D_UnprotectedLx_Stop_Reverse = (UINT32)iterULCSProperty->GetSubItem(IDX_D_UnprotectedLx_Stop_Reverse)->GetValue().intVal;
		}

		p.SPStatus[iSP].N_ITER_ATO_13 = (UINT8)iterSPNSPProperty->GetSubItem(IDX_SPNSP_N_ITER_PBD)->GetValue().intVal;

		//PBD iter (2)
		for (int iPBD = 0; iPBD < (int)p.SPStatus[iSP].N_ITER_ATO_13; ++iPBD)
		{
			CMFCPropertyGridProperty* iterPBDProperty = iterSPNSPProperty->GetSubItem(IDX_SPNSP_PBD_ITER_Property)->GetSubItem(iPBD);

			p.SPStatus[iSP].permittedBrakingDistance[iPBD].Q_Range = (UINT8)iterPBDProperty->GetSubItem(IDX_Q_Range_PBD)->GetValue().intVal;
			p.SPStatus[iSP].permittedBrakingDistance[iPBD].D_Permitted_Braking_Distance = (UINT32)iterPBDProperty->GetSubItem(IDX_D_Permitted_Braking_Distance)->GetValue().intVal;
			p.SPStatus[iSP].permittedBrakingDistance[iPBD].Q_PBD_SBEB = (UINT8)iterPBDProperty->GetSubItem(IDX_Q_PBD_SBEB)->GetValue().intVal;
			p.SPStatus[iSP].permittedBrakingDistance[iPBD].G_PBD = (UINT8)iterPBDProperty->GetSubItem(IDX_G_PBD)->GetValue().intVal;
			p.SPStatus[iSP].permittedBrakingDistance[iPBD].Q_GDIR_PBD = (UINT8)iterPBDProperty->GetSubItem(IDX_Q_GDIR_PBD)->GetValue().intVal;
			p.SPStatus[iSP].permittedBrakingDistance[iPBD].D_Start_Location = (UINT32)iterPBDProperty->GetSubItem(IDX_D_Start_Location_PBD)->GetValue().intVal;
			p.SPStatus[iSP].permittedBrakingDistance[iPBD].D_End_Location = (UINT32)iterPBDProperty->GetSubItem(IDX_D_End_Location_PBD)->GetValue().intVal;
		}

		p.SPStatus[iSP].N_ITER_ATO_14 = (UINT8)iterSPNSPProperty->GetSubItem(IDX_SPNSP_N_ITER_SORB)->GetValue().intVal;

		//SORB iter (2)
		for (int iS1 = 0; iS1 < (int)p.SPStatus[iSP].N_ITER_ATO_14; ++iS1)
		{
			CMFCPropertyGridProperty* iterSORBProperty = iterSPNSPProperty->GetSubItem(IDX_SPNSP_SORB_ITER_Property)->GetSubItem(iS1);

			p.SPStatus[iSP].switchOffRegenerativeBrake[iS1].Q_Range = (UINT8)iterSORBProperty->GetSubItem(IDX_Q_Range_SORB)->GetValue().intVal;
			p.SPStatus[iSP].switchOffRegenerativeBrake[iS1].D_Start_Location = (UINT32)iterSORBProperty->GetSubItem(IDX_D_Start_Location_SORB)->GetValue().intVal;
			p.SPStatus[iSP].switchOffRegenerativeBrake[iS1].D_End_Location = (UINT32)iterSORBProperty->GetSubItem(IDX_D_End_Location_SORB)->GetValue().intVal;
		}

		p.SPStatus[iSP].N_ITER_ATO_15 = (UINT8)iterSPNSPProperty->GetSubItem(IDX_SPNSP_N_ITER_SOECB)->GetValue().intVal;

		//SOECB iter (2)
		for (int iS2 = 0; iS2 < (int)p.SPStatus[iSP].N_ITER_ATO_15; ++iS2)
		{
			CMFCPropertyGridProperty* iterSOECBProperty = iterSPNSPProperty->GetSubItem(IDX_SPNSP_SOECB_ITER_Property)->GetSubItem(iS2);

			p.SPStatus[iSP].switchOffEddyCurrentBrake[iS2].Q_Range = (UINT8)iterSOECBProperty->GetSubItem(IDX_Q_Range_SOECB)->GetValue().intVal;
			p.SPStatus[iSP].switchOffEddyCurrentBrake[iS2].D_Start_Location = (UINT32)iterSOECBProperty->GetSubItem(IDX_D_Start_Location_SOECB)->GetValue().intVal;
			p.SPStatus[iSP].switchOffEddyCurrentBrake[iS2].D_End_Location = (UINT32)iterSOECBProperty->GetSubItem(IDX_D_End_Location_SOECB)->GetValue().intVal;
		}

		p.SPStatus[iSP].N_ITER_ATO_16 = (UINT8)iterSPNSPProperty->GetSubItem(IDX_SPNSP_N_ITER_SOECEB)->GetValue().intVal;

		//SOECEB iter (2)
		for (int iS3 = 0; iS3 < (int)p.SPStatus[iSP].N_ITER_ATO_16; ++iS3)
		{
			CMFCPropertyGridProperty* iterSOECEB = iterSPNSPProperty->GetSubItem(IDX_SPNSP_SOECEB_ITER_Property)->GetSubItem(iS3);

			p.SPStatus[iSP].switchOffEddyEmergencyBrake[iS3].Q_Range = (UINT8)iterSOECEB->GetSubItem(IDX_Q_Range_SOECB)->GetValue().intVal;
			p.SPStatus[iSP].switchOffEddyEmergencyBrake[iS3].D_Start_Location = (UINT32)iterSOECEB->GetSubItem(IDX_D_Start_Location_SOECEB)->GetValue().intVal;
			p.SPStatus[iSP].switchOffEddyEmergencyBrake[iS3].D_End_Location = (UINT32)iterSOECEB->GetSubItem(IDX_D_End_Location_SOECEB)->GetValue().intVal;
		}

		p.SPStatus[iSP].N_ITER_ATO_17 = (UINT8)iterSPNSPProperty->GetSubItem(IDX_SPNSP_N_ITER_SOMSB)->GetValue().intVal;

		//SOMSB iter (2)
		for (int iS4 = 0; iS4 < (int)p.SPStatus[iSP].N_ITER_ATO_17; ++iS4)
		{
			CMFCPropertyGridProperty* iterSOMSB = iterSPNSPProperty->GetSubItem(IDX_SPNSP_SOMSB_ITER_Property)->GetSubItem(iS4);

			p.SPStatus[iSP].switchOffMagmaticShoeBrake[iS4].Q_Range = (UINT8)iterSOMSB->GetSubItem(IDX_Q_Range_SOMSB)->GetValue().intVal;
			p.SPStatus[iSP].switchOffMagmaticShoeBrake[iS4].D_Start_Location = (UINT32)iterSOMSB->GetSubItem(IDX_D_Start_Location_SOMSB)->GetValue().intVal;
			p.SPStatus[iSP].switchOffMagmaticShoeBrake[iS4].D_End_Location = (UINT32)iterSOMSB->GetSubItem(IDX_D_End_Location_SOMSB)->GetValue().intVal;
		}

		p.SPStatus[iSP].N_ITER_ATO_18 = (UINT8)iterSPNSPProperty->GetSubItem(IDX_SPNSP_N_ITER_ADBFI)->GetValue().intVal;

		//ADBI iter (2)
		for (int iAD = 0; iAD < (int)p.SPStatus[iSP].N_ITER_ATO_18; ++iAD)
		{
			CMFCPropertyGridProperty* iterADBI = iterSPNSPProperty->GetSubItem(IDX_SPNSP_ADBFI_ITER_Property)->GetSubItem(iAD);

			p.SPStatus[iSP].areaWithDynamicBrakeForceInhibition[iAD].Q_Range = (UINT8)iterADBI->GetSubItem(IDX_Q_Range_ADBFI)->GetValue().intVal;
			p.SPStatus[iSP].areaWithDynamicBrakeForceInhibition[iAD].D_Start_Location = (UINT32)iterADBI->GetSubItem(IDX_D_Start_Location_ADBFI)->GetValue().intVal;
			p.SPStatus[iSP].areaWithDynamicBrakeForceInhibition[iAD].D_End_Location = (UINT32)iterADBI->GetSubItem(IDX_D_End_Location_ADBFI)->GetValue().intVal;
		}

		p.SPStatus[iSP].N_ITER_ATO_19 = (UINT8)iterSPNSPProperty->GetSubItem(IDX_SPNSP_N_ITER_ALDBF)->GetValue().intVal;

		//ALDBF iter (2)
		for (int iAL = 0; iAL < (int)p.SPStatus[iSP].N_ITER_ATO_19; ++iAL)
		{
			CMFCPropertyGridProperty* iterALDBF = iterSPNSPProperty->GetSubItem(IDX_SPNSP_ALDBF_ITER_Property)->GetSubItem(iAL);

			p.SPStatus[iSP].areaWithLimitedDynamicBrakeForce[iAL].Q_Range = (UINT8)iterALDBF->GetSubItem(IDX_Q_Range_ALDBF)->GetValue().intVal;
			p.SPStatus[iSP].areaWithLimitedDynamicBrakeForce[iAL].M_Dynamic_Braking_Force_Limit = (UINT16)iterALDBF->GetSubItem(IDX_M_Dynamic_Brake_Force_Limit)->GetValue().intVal;
			p.SPStatus[iSP].areaWithLimitedDynamicBrakeForce[iAL].D_Start_Location = (UINT32)iterALDBF->GetSubItem(IDX_D_Start_Location_ALDBF)->GetValue().intVal;
			p.SPStatus[iSP].areaWithLimitedDynamicBrakeForce[iAL].D_End_Location = (UINT32)iterALDBF->GetSubItem(IDX_D_End_Location_ALDBF)->GetValue().intVal;
		}

	}

}

BOOL packet07Tab::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(255, 255, 255));

	return TRUE;

	return CDialogEx::OnEraseBkgnd(pDC);
}


HBRUSH packet07Tab::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	pDC->SetBkColor(RGB(255, 255, 255)); // Static Control 전부 글자배경색 하얀색으로

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

void packet07Tab::propgrid()
{
	int pnum = 7;
	


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


	

	PropertyCtrlBuilder::BuildBodyPkSP(pxBodyProperty);


	
}

void packet07Tab::OnBnClickedChbAuto()
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


void packet07Tab::OnCbnSelchangeCmbPacket()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_propgrid.RemoveAll();
	propgrid();
}


void packet07Tab::OnBnClickedBtnSend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// Sa-Conn 연결여부 확인 필요

	CTabDlg1* targetDlg = ((CATOTSSimulatortabDlg*)AfxGetMainWnd())->pDlg1;

		PacketHeaderCtrlToStructure(m_propgrid.GetProperty(HEADER), *pHeaderStructure);
		PacketSPCtrlToStructure(m_propgrid.GetProperty(BODY_DATA), *pSPStructure);

		targetDlg->Cmd_Send126(eSP);

	

}


void packet07Tab::PacketSPStructrureToINI(TS_OB_Packet07_SPStruct& p)
{
	if (&p == nullptr)return;

	CStdioFile ipfile;
	CStdioFile* pipfile = &ipfile;

	CString dir = Utility::GetDirectoryFromFileDialog(_T("*.ini"), SPDefaultConfigFileName, OFN_HIDEREADONLY, szConfigFilter);
	if (ipfile.Open(dir.GetString(), CFile::modeCreate | CFile::modeWrite) == FALSE) return;
	Utility::CFileWriteCHAR(pipfile, packetSP);
	Utility::CFileWriteCHAR(pipfile, endLine);

	CHAR tempChar[128]{};
	TextTablePacketSP& idcList = TextTablePacketSP::GetInstance();

	sprintf_s(tempChar, ": %d\n", p.N_ITER_ATO);
	Utility::CFileWriteCHAR(pipfile, idcList.GetText(IDX_SP_N_ITER));
	Utility::CFileWriteCHAR(pipfile, tempChar);

	//1st iter
	for (int i = 0; i < (int)p.N_ITER_ATO; ++i)
	{
		sprintf_s(tempChar, "%s(%d): %u\n", "NID_C", i, p.SPStatus[i].NID_C);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		sprintf_s(tempChar, "%s(%d): %u\n", "NID_SP", i, p.SPStatus[i].NID_SP_1);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		sprintf_s(tempChar, "%s(%d): %u\n", "Q_SP_Status", i, p.SPStatus[i].Q_SP_Status);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		sprintf_s(tempChar, "%s(%d): %u\n", "M_SP_Version", i, p.SPStatus[i].spStatusValid.M_SP_Version);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		sprintf_s(tempChar, "%s(%d): %u\n", "L_SP", i, p.SPStatus[i].spStatusValid.L_SP);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		sprintf_s(tempChar, "%s(%d): %u\n", "D_EoA_Offset", i, p.SPStatus[i].spStatusValid.D_EOA_Offset);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		sprintf_s(tempChar, "%s(%d): %u\n", "Q_UTC_Offset", i, p.SPStatus[i].spStatusValid.Q_UTC_Offset);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		sprintf_s(tempChar, "%s(%d): %u\n", "M_SP_Altitude", i, p.SPStatus[i].spStatusValid.M_SP_Altitude);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		sprintf_s(tempChar, "%s(%d): %u\n", "Q_ATOTS_Contact_Info_Dir", i, p.SPStatus[i].spStatusValid.Q_ATOTS_Contact_Info_Dir);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		sprintf_s(tempChar, "%s(%d): %u\n", "NID_C", i, p.SPStatus[i].atoTSContactInfo.NID_C_1);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		sprintf_s(tempChar, "%s(%d): %u\n", "NID_ATOTS", i, p.SPStatus[i].atoTSContactInfo.NID_ATOTS);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		sprintf_s(tempChar, "%s(%d): %u\n", "NID_C", i, p.SPStatus[i].atoTSContactInfo.NID_C_2);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		sprintf_s(tempChar, "%s(%d): %u\n", "NID_SP", i, p.SPStatus[i].atoTSContactInfo.NID_SP_2);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		sprintf_s(tempChar, "%s(%d): %u\n", "V_STATIC", i, p.SPStatus[i].V_STATIC);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		sprintf_s(tempChar, "%s(%d): %u\n", "Q_FRONT", i, p.SPStatus[i].Q_FRONT);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		//Specific SSP
		sprintf_s(tempChar, "%s(%d): %u\n", "N_ITER", i, p.SPStatus[i].N_ITER_ATO_1);
		Utility::CFileWriteCHAR(pipfile, tempChar);
				
		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_1; ++k)
		{
			sprintf_s(tempChar, "%s(%d): %u\n", "Q_DIFF", k, p.SPStatus[i].specificSSP[k].Q_DIFF);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "NC_CDDIFF", k, p.SPStatus[i].specificSSP[k].NC_CDDIFF);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "NC_DIFF", k, p.SPStatus[i].specificSSP[k].NC_DIFF);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "V_DIFF", k, p.SPStatus[i].specificSSP[k].V_DIFF);
			Utility::CFileWriteCHAR(pipfile, tempChar);
		}

		//Static Speed Profile Change
		sprintf_s(tempChar, "%s(%d): %u\n", "N_ITER", i, p.SPStatus[i].N_ITER_ATO_2);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_2; ++k)
		{
			sprintf_s(tempChar, "%s(%d): %u\n", "D_Location", k, p.SPStatus[i].staticSPChange[k].D_Location);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "V_STATIC", k, p.SPStatus[i].staticSPChange[k].V_STATIC);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "Q_FRONT", k, p.SPStatus[i].staticSPChange[k].Q_FRONT);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			//Specific SSP Change
			sprintf_s(tempChar, "%s(%d): %u\n", "N_ITER", k, p.SPStatus[i].staticSPChange[k].N_ITER);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			for (int l = 0; l < (int)p.SPStatus[i].staticSPChange[k].N_ITER; ++l)
			{
				sprintf_s(tempChar, "%s(%d): %u\n", "Q_DIFF", l, p.SPStatus[i].staticSPChange[k].specificSspChange[l].Q_DIFF);
				Utility::CFileWriteCHAR(pipfile, tempChar);

				sprintf_s(tempChar, "%s(%d): %u\n", "NC_CDDIFF", l, p.SPStatus[i].staticSPChange[k].specificSspChange[l].NC_CDDIFF);
				Utility::CFileWriteCHAR(pipfile, tempChar);

				sprintf_s(tempChar, "%s(%d): %u\n", "NC_DIFF", l, p.SPStatus[i].staticSPChange[k].specificSspChange[l].NC_DIFF);
				Utility::CFileWriteCHAR(pipfile, tempChar);

				sprintf_s(tempChar, "%s(%d): %u\n", "V_DIFF", l, p.SPStatus[i].staticSPChange[k].specificSspChange[l].V_DIFF);
				Utility::CFileWriteCHAR(pipfile, tempChar);
			}
		}

		sprintf_s(tempChar, "%s(%d): %u\n", "G_New_Gradient", i, p.SPStatus[i].G_New_Gradient);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		sprintf_s(tempChar, "%s(%d): %u\n", "Q_GDIR", i, p.SPStatus[i].Q_GDIR);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		//Gradients Change
		sprintf_s(tempChar, "%s(%d): %u\n", "N_ITER", i, p.SPStatus[i].N_ITER_ATO_3);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_3; ++k)
		{
			sprintf_s(tempChar, "%s(%d): %u\n", "D_Location", k, p.SPStatus[i].gradientChange[k].D_Location);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "G_New_Gradient", k, p.SPStatus[i].gradientChange[k].G_New_Gradient);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "Q_GDIR", k, p.SPStatus[i].gradientChange[k].Q_GDIR);
			Utility::CFileWriteCHAR(pipfile, tempChar);
		}

		sprintf_s(tempChar, "%s(%d): %u\n", "Q_Radius_Category", i, p.SPStatus[i].Q_Radius_Category);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		sprintf_s(tempChar, "%s(%d): %u\n", "N_ITER", i, p.SPStatus[i].N_ITER_ATO_4);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_4; ++k)
		{
			sprintf_s(tempChar, "%s(%d): %u\n", "D_Location", k, p.SPStatus[i].curvesChange[k].D_Location);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "Q_Radius_Category", k, p.SPStatus[i].curvesChange[k].Q_Radius_Category);
			Utility::CFileWriteCHAR(pipfile, tempChar);

		}

		sprintf_s(tempChar, "%s(%d): %u\n", "M_VOLTAGE", i, p.SPStatus[i].M_VOLTAGE);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		sprintf_s(tempChar, "%s(%d): %u\n", "NID_CTRACTION", i, p.SPStatus[i].NID_CTRACTION);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		sprintf_s(tempChar, "%s(%d): %u\n", "N_ITER", i, p.SPStatus[i].N_ITER_ATO_5);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_5; ++k)
		{
			sprintf_s(tempChar, "%s(%d): %u\n", "D_Location", k, p.SPStatus[i].powerVoltageChange[k].D_Location);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "M_VOLTAGE", k, p.SPStatus[i].powerVoltageChange[k].M_VOLTAGE);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "NID_CTRACTION", k, p.SPStatus[i].powerVoltageChange[k].NID_CTRACTION);
			Utility::CFileWriteCHAR(pipfile, tempChar);
		}

		sprintf_s(tempChar, "%s(%d): %u\n", "M_CURRENT", i, p.SPStatus[i].M_CURRENT);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		sprintf_s(tempChar, "%s(%d): %u\n", "N_ITER", i, p.SPStatus[i].N_ITER_ATO_6);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_6; ++k)
		{
			sprintf_s(tempChar, "%s(%d): %u\n", "D_Location", k, p.SPStatus[i].currentLimitChange[k].D_Location);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "M_CURRENT", k, p.SPStatus[i].currentLimitChange[k].M_CURRENT);
			Utility::CFileWriteCHAR(pipfile, tempChar);
		}

		sprintf_s(tempChar, "%s(%d): %u\n", "N_ITER", i, p.SPStatus[i].N_ITER_ATO_7);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_7; ++k)
		{
			sprintf_s(tempChar, "%s(%d): %u\n", "Q_NEWNID_C", k, p.SPStatus[i].baliseGroup[k].Q_NEWNID_C);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "NID_C", k, p.SPStatus[i].baliseGroup[k].NID_C);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "NID_BG", k, p.SPStatus[i].baliseGroup[k].NID_BG);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "N_ITER_BG", k, p.SPStatus[i].baliseGroup[k].N_ITER_BG);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			for (int l = 0; l < (int)p.SPStatus[i].baliseGroup[k].N_ITER_BG; ++l)
			{
				sprintf_s(tempChar, "%s(%d): %u\n", "N_PIG", l, p.SPStatus[i].baliseGroup[k].balises[l].N_PIG);
				Utility::CFileWriteCHAR(pipfile, tempChar);

				sprintf_s(tempChar, "%s(%d): %u\n", "D_Location", l, p.SPStatus[i].baliseGroup[k].balises[l].D_Location);
				Utility::CFileWriteCHAR(pipfile, tempChar);
			}
		}

		sprintf_s(tempChar, "%s(%d): %u\n", "N_ITER", i, p.SPStatus[i].N_ITER_ATO_8);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_8; ++k)
		{
			sprintf_s(tempChar, "%s(%d): %u\n", "NID_TP", k, p.SPStatus[i].timingPoints[k].NID_TP);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "D_Location", k, p.SPStatus[i].timingPoints[k].D_Location);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "Q_Stop_Location_Tolerance", k, p.SPStatus[i].timingPoints[k].Q_Stop_Location_Tolerance);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "Q_STP_Reached", k, p.SPStatus[i].timingPoints[k].Q_STP_Reached);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "L_TEXT", k, p.SPStatus[i].timingPoints[k].L_TEXT);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			for (int l = 0; l < (int)p.SPStatus[i].timingPoints[k].L_TEXT; ++l)
			{
				sprintf_s(tempChar, "%s(%d): %u\n", "X_TEXT", l, p.SPStatus[i].timingPoints[k].X_TEXT[l]);
				Utility::CFileWriteCHAR(pipfile, tempChar);
			}
		}

		sprintf_s(tempChar, "%s(%d): %u\n", "N_ITER", i, p.SPStatus[i].N_ITER_ATO_9);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_9; ++k)
		{
			sprintf_s(tempChar, "%s(%d): %u\n", "Q_Range", k, p.SPStatus[i].platformArea[k].Q_Range);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "D_Start_Location", k, p.SPStatus[i].platformArea[k].D_Start_Location);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "D_End_Location", k, p.SPStatus[i].platformArea[k].D_End_Location);
			Utility::CFileWriteCHAR(pipfile, tempChar);
		}

		sprintf_s(tempChar, "%s(%d): %u\n", "N_ITER", i, p.SPStatus[i].N_ITER_ATO_10);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_10; ++k)
		{
			sprintf_s(tempChar, "%s(%d): %u\n", "Q_Range", k, p.SPStatus[i].tunnel[k].Q_Range);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "Q_Tunnel_Category", k, p.SPStatus[i].tunnel[k].Q_Tunnel_Category);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "D_Start_Location", k, p.SPStatus[i].tunnel[k].D_Start_Location);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "D_End_Location", k, p.SPStatus[i].tunnel[k].D_End_Location);
			Utility::CFileWriteCHAR(pipfile, tempChar);
		}

		sprintf_s(tempChar, "%s(%d): %u\n", "N_ITER", i, p.SPStatus[i].N_ITER_ATO_11);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_11; ++k)
		{
			sprintf_s(tempChar, "%s(%d): %u\n", "Q_Range", k, p.SPStatus[i].axleLoadSpeedProfile[k].Q_Range);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "M_AXLELOADCAT", k, p.SPStatus[i].axleLoadSpeedProfile[k].M_AXLELOADCAT);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "V_New_Speed_Level", k, p.SPStatus[i].axleLoadSpeedProfile[k].V_New_Speed_Level);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "Q_FRONT", k, p.SPStatus[i].axleLoadSpeedProfile[k].Q_FRONT);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "D_Start_Location", k, p.SPStatus[i].axleLoadSpeedProfile[k].D_Start_Location);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "D_End_Location", k, p.SPStatus[i].axleLoadSpeedProfile[k].D_End_Location);
			Utility::CFileWriteCHAR(pipfile, tempChar);
		}

		sprintf_s(tempChar, "%s(%d): %u\n", "N_ITER", i, p.SPStatus[i].N_ITER_ATO_12);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_12; ++k)
		{
			sprintf_s(tempChar, "%s(%d): %u\n", "D_UnprotectedLx_Stop_Nominal", k, p.SPStatus[i].unprotectedLevelCrossingStop[k].D_UnprotectedLx_Stop_Nominal);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "D_UnprotectedLx_Stop_Reverse", k, p.SPStatus[i].unprotectedLevelCrossingStop[k].D_UnprotectedLx_Stop_Reverse);
			Utility::CFileWriteCHAR(pipfile, tempChar);
		}

		sprintf_s(tempChar, "%s(%d): %u\n", "N_ITER", i, p.SPStatus[i].N_ITER_ATO_13);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_13; ++k)
		{
			sprintf_s(tempChar, "%s(%d): %u\n", "Q_Range", k, p.SPStatus[i].permittedBrakingDistance[k].Q_Range);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "D_Permitted_Braking_Distance", k, p.SPStatus[i].permittedBrakingDistance[k].D_Permitted_Braking_Distance);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "Q_PBD_SBEB", k, p.SPStatus[i].permittedBrakingDistance[k].Q_PBD_SBEB);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "G_PBD", k, p.SPStatus[i].permittedBrakingDistance[k].G_PBD);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "Q_GDIR_PBD", k, p.SPStatus[i].permittedBrakingDistance[k].Q_GDIR_PBD);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "D_Start_Location", k, p.SPStatus[i].permittedBrakingDistance[k].D_Start_Location);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "D_End_Location", k, p.SPStatus[i].permittedBrakingDistance[k].D_End_Location);
			Utility::CFileWriteCHAR(pipfile, tempChar);
		}

		sprintf_s(tempChar, "%s(%d): %u\n", "N_ITER", i, p.SPStatus[i].N_ITER_ATO_14);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_14; ++k)
		{
			sprintf_s(tempChar, "%s(%d): %u\n", "Q_Range", k, p.SPStatus[i].switchOffRegenerativeBrake[k].Q_Range);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "D_Start_Location", k, p.SPStatus[i].switchOffRegenerativeBrake[k].D_Start_Location);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "D_End_Location", k, p.SPStatus[i].switchOffRegenerativeBrake[k].D_End_Location);
			Utility::CFileWriteCHAR(pipfile, tempChar);
		}

		sprintf_s(tempChar, "%s(%d): %u\n", "N_ITER", i, p.SPStatus[i].N_ITER_ATO_15);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_15; ++k)
		{
			sprintf_s(tempChar, "%s(%d): %u\n", "Q_Range", k, p.SPStatus[i].switchOffEddyCurrentBrake[k].Q_Range);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "D_Start_Location", k, p.SPStatus[i].switchOffEddyCurrentBrake[k].D_Start_Location);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "D_End_Location", k, p.SPStatus[i].switchOffEddyCurrentBrake[k].D_End_Location);
			Utility::CFileWriteCHAR(pipfile, tempChar);
		}

		sprintf_s(tempChar, "%s(%d): %u\n", "N_ITER", i, p.SPStatus[i].N_ITER_ATO_16);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_16; ++k)
		{
			sprintf_s(tempChar, "%s(%d): %u\n", "Q_Range", k, p.SPStatus[i].switchOffEddyEmergencyBrake[k].Q_Range);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "D_Start_Location", k, p.SPStatus[i].switchOffEddyEmergencyBrake[k].D_Start_Location);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "D_End_Location", k, p.SPStatus[i].switchOffEddyEmergencyBrake[k].D_End_Location);
			Utility::CFileWriteCHAR(pipfile, tempChar);
		}

		sprintf_s(tempChar, "%s(%d): %u\n", "N_ITER", i, p.SPStatus[i].N_ITER_ATO_17);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_17; ++k)
		{
			sprintf_s(tempChar, "%s(%d): %u\n", "Q_Range", k, p.SPStatus[i].switchOffMagmaticShoeBrake[k].Q_Range);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "D_Start_Location", k, p.SPStatus[i].switchOffMagmaticShoeBrake[k].D_Start_Location);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "D_End_Location", k, p.SPStatus[i].switchOffMagmaticShoeBrake[k].D_End_Location);
			Utility::CFileWriteCHAR(pipfile, tempChar);
		}

		sprintf_s(tempChar, "%s(%d): %u\n", "N_ITER", i, p.SPStatus[i].N_ITER_ATO_18);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_18; ++k)
		{
			sprintf_s(tempChar, "%s(%d): %u\n", "Q_Range", k, p.SPStatus[i].areaWithDynamicBrakeForceInhibition[k].Q_Range);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "D_Start_Location", k, p.SPStatus[i].areaWithDynamicBrakeForceInhibition[k].D_Start_Location);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "D_End_Location", k, p.SPStatus[i].areaWithDynamicBrakeForceInhibition[k].D_End_Location);
			Utility::CFileWriteCHAR(pipfile, tempChar);
		}

		sprintf_s(tempChar, "%s(%d): %u\n", "N_ITER", i, p.SPStatus[i].N_ITER_ATO_19);
		Utility::CFileWriteCHAR(pipfile, tempChar);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_19; ++k)
		{
			sprintf_s(tempChar, "%s(%d): %u\n", "Q_Range", k, p.SPStatus[i].areaWithLimitedDynamicBrakeForce[k].Q_Range);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "M_Dynamic_Braking_Force_Limit", k, p.SPStatus[i].areaWithLimitedDynamicBrakeForce[k].M_Dynamic_Braking_Force_Limit);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "D_Start_Location", k, p.SPStatus[i].areaWithLimitedDynamicBrakeForce[k].D_Start_Location);
			Utility::CFileWriteCHAR(pipfile, tempChar);

			sprintf_s(tempChar, "%s(%d): %u\n", "D_End_Location", k, p.SPStatus[i].areaWithLimitedDynamicBrakeForce[k].D_End_Location);
			Utility::CFileWriteCHAR(pipfile, tempChar);
		}
	}
}


BOOL packet07Tab::RequestedLoadSPFromFile(int num, CString path)
{
	TS_OB_Packet07_SPStruct& p = *pFileSPStructure;
	if (&p == nullptr)return FALSE;
	CString fileName;
	fileName.Format(_T("config_SP%d.ini"), num);

	path += fileName;

	CStdioFile ipfile;
	if (ipfile.Open(path.GetString(), CFile::modeRead) == FALSE)
	{
		if (ipfile.Open(fileName, CFile::modeRead) == FALSE) return FALSE;
	}
	if (Utility::CheckFileUnicode(ipfile) == TRUE) return FALSE;

	//파일 내용 복사
	ULONGLONG fileSize = ipfile.GetLength();
	CHAR* tempStr = new CHAR[fileSize];
	ipfile.Read(tempStr, (UINT)fileSize);
	string configContainer; //파일 전체 문자열
	configContainer = tempStr;
	delete[] tempStr;

	// [Segment Profile] 문자열 검색
	size_t strIdx = configContainer.find(packetSP);
	if (strIdx != string::npos) // 발견
		configContainer = configContainer.substr(strIdx); // [Segment Profile]을 포함한 이후의 문자열이 남음
	else return FALSE; //SP 의 정보가 담긴 파일이 아님

	LoadPacketSPFromFile_StaticData(configContainer, p);

	ipfile.Close();

	ReLoadPacketSP();

	return TRUE;
	
}

BOOL packet07Tab::LoadPacketSPFromFile()
{
	TS_OB_Packet07_SPStruct& p = *pFileSPStructure;
	if (&p == nullptr)return FALSE;
	static CString pathName = SPDefaultConfigFileName;

	static bool initFlag = true;
	if (initFlag)
	{
		pathName = Utility::GetDirectoryFromFileDialog(_T("*.ini"), SPDefaultConfigFileName, OFN_HIDEREADONLY, szConfigFilter);
		if (pathName.Compare(_T("")) == 0) return FALSE;
		initFlag = true; //
	}

	CStdioFile ipfile;
	if (!ipfile.Open(pathName, CFile::modeRead))	//Open
	{
		// 파일 열기 실패 시 기본 파일 열기 시도
		if (!ipfile.Open(SPDefaultConfigFileName, CFile::modeRead)) return FALSE;
	}

	if (Utility::CheckFileUnicode(ipfile)) return FALSE;

	// 파일 내용 복사
	ULONGLONG fileSize = ipfile.GetLength();
	CHAR* tempStr = new CHAR[fileSize + 1]; // +1 for null terminator
	ipfile.Read(tempStr, (UINT)fileSize);
	tempStr[fileSize] = '\0'; // Null terminate the string
	string configContainer(tempStr); // 파일 전체 문자열
	delete[] tempStr;

	// [Segment Profile] 문자열 검색
	size_t strIdx = configContainer.find(packetSP);
	if (strIdx != string::npos) //발견
	{
		configContainer = configContainer.substr(strIdx); // [Segment Profile]을 포함한 이후의 문자열이 남음
		LoadPacketSPFromFile_StaticData(configContainer, p);
		ipfile.Close();
		return TRUE;
	}
	else
	{
		ipfile.Close();
		return FALSE; // SP의 정보가 담긴 파일이 아님
	}
}

BOOL packet07Tab::initLoadPacketSPFromFile()
{
	TS_OB_Packet07_SPStruct& p = *pFileSPStructure;
	if (&p == nullptr)return FALSE;
	static CString pathName = SPDefaultConfigFileName;
	
	CStdioFile ipfile;
	if (ipfile.Open(pathName.GetString(), CFile::modeRead) == FALSE)
	{
		if (ipfile.Open(SPDefaultConfigFileName, CFile::modeRead) == FALSE) return FALSE;
	}
	if (Utility::CheckFileUnicode(ipfile) == TRUE) return FALSE;

	//파일 내용 복사
	ULONGLONG fileSize = ipfile.GetLength();
	CHAR* tempStr = new CHAR[fileSize];
	ipfile.Read(tempStr, (UINT)fileSize);
	string configContainer; //파일 전체 문자열
	configContainer = tempStr;
	delete[] tempStr;

	// [Segment Profile] 문자열 검색
	size_t strIdx = configContainer.find(packetSP);
	if (strIdx != string::npos) // 발견
		configContainer = configContainer.substr(strIdx); // [Segment Profile]을 포함한 이후의 문자열이 남음
	else return FALSE; //SP 의 정보가 담긴 파일이 아님

	LoadPacketSPFromFile_StaticData(configContainer, p);

	ipfile.Close();

	ReLoadPacketSP();

	return TRUE;
}


void packet07Tab::LoadPacketSPFromFile_StaticData(string& configTXT, TS_OB_Packet07_SPStruct& p)
{
	TextTablePacketSP& idcList = TextTablePacketSP::GetInstance();

	LoadPacket(configTXT, idcList.GetText(IDX_SP_N_ITER), p.N_ITER_ATO);


	CHAR iterIdc[64];

	for (int i = 0; i < (int)p.N_ITER_ATO; ++i)
	{
		sprintf_s(iterIdc, "%s(%d)", "NID_C", i);
		LoadPacket(configTXT, iterIdc, p.SPStatus[i].NID_C);

		sprintf_s(iterIdc, "%s(%d)", "NID_SP", i);
		LoadPacket(configTXT, iterIdc, p.SPStatus[i].NID_SP_1);

		sprintf_s(iterIdc, "%s(%d)", "Q_SP_Status", i);
		LoadPacket(configTXT, iterIdc, p.SPStatus[i].Q_SP_Status);

		sprintf_s(iterIdc, "%s(%d)", "M_SP_Version", i);
		LoadPacket(configTXT, iterIdc, p.SPStatus[i].spStatusValid.M_SP_Version);

		sprintf_s(iterIdc, "%s(%d)", "L_SP", i);
		LoadPacket(configTXT, iterIdc, p.SPStatus[i].spStatusValid.L_SP);

		sprintf_s(iterIdc, "%s(%d)", "D_EoA_Offset", i);
		LoadPacket(configTXT, iterIdc, p.SPStatus[i].spStatusValid.D_EOA_Offset);

		sprintf_s(iterIdc, "%s(%d)", "Q_UTC_Offset", i);
		LoadPacket(configTXT, iterIdc, p.SPStatus[i].spStatusValid.Q_UTC_Offset);

		sprintf_s(iterIdc, "%s(%d)", "M_SP_Altitude", i);
		LoadPacket(configTXT, iterIdc, p.SPStatus[i].spStatusValid.M_SP_Altitude);

		sprintf_s(iterIdc, "%s(%d)", "Q_ATOTS_Contact_Info_Dir", i);
		LoadPacket(configTXT, iterIdc, p.SPStatus[i].spStatusValid.Q_ATOTS_Contact_Info_Dir);

		sprintf_s(iterIdc, "%s(%d)", "NID_C", i);
		LoadPacket(configTXT, iterIdc, p.SPStatus[i].atoTSContactInfo.NID_C_1);

		sprintf_s(iterIdc, "%s(%d)", "NID_ATOTS", i);
		LoadPacket(configTXT, iterIdc, p.SPStatus[i].atoTSContactInfo.NID_ATOTS);

		sprintf_s(iterIdc, "%s(%d)", "NID_C", i);
		LoadPacket(configTXT, iterIdc, p.SPStatus[i].atoTSContactInfo.NID_C_2);

		sprintf_s(iterIdc, "%s(%d)", "NID_SP", i);
		LoadPacket(configTXT, iterIdc, p.SPStatus[i].atoTSContactInfo.NID_SP_2);

		sprintf_s(iterIdc, "%s(%d)", "V_STATIC", i);
		LoadPacket(configTXT, iterIdc, p.SPStatus[i].V_STATIC);

		sprintf_s(iterIdc, "%s(%d)", "Q_FRONT", i);
		LoadPacket(configTXT, iterIdc, p.SPStatus[i].Q_FRONT);

		sprintf_s(iterIdc, "%s(%d)", "N_ITER", i);
		LoadPacket(configTXT, iterIdc, p.SPStatus[i].N_ITER_ATO_1);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_1; ++k)
		{
			sprintf_s(iterIdc, "%s(%d)", "Q_DIFF", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].specificSSP[k].Q_DIFF);

			sprintf_s(iterIdc, "%s(%d)", "NC_CDDIFF", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].specificSSP[k].NC_CDDIFF);

			sprintf_s(iterIdc, "%s(%d)", "NC_DIFF", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].specificSSP[k].NC_DIFF);

			sprintf_s(iterIdc, "%s(%d)", "V_DIFF", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].specificSSP[k].V_DIFF);
		}

		sprintf_s(iterIdc, "%s(%d)", "N_ITER", i);
		LoadPacket(configTXT, iterIdc, p.SPStatus[i].N_ITER_ATO_2);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_2; ++k)
		{
			sprintf_s(iterIdc, "%s(%d)", "D_Location", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].staticSPChange[k].D_Location);

			sprintf_s(iterIdc, "%s(%d)", "V_STATIC", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].staticSPChange[k].V_STATIC);

			sprintf_s(iterIdc, "%s(%d)", "Q_FRONT", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].staticSPChange[k].Q_FRONT);

			sprintf_s(iterIdc, "%s(%d)", "N_ITER", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].staticSPChange[k].N_ITER);

			for (int l = 0; l < (int)p.SPStatus[i].staticSPChange[k].N_ITER; ++l)
			{
				sprintf_s(iterIdc, "%s(%d)", "Q_DIFF", k);
				LoadPacket(configTXT, iterIdc, p.SPStatus[i].staticSPChange[k].specificSspChange[l].Q_DIFF);

				sprintf_s(iterIdc, "%s(%d)", "NC_CDDIFF", k);
				LoadPacket(configTXT, iterIdc, p.SPStatus[i].staticSPChange[k].specificSspChange[l].NC_CDDIFF);

				sprintf_s(iterIdc, "%s(%d)", "NC_DIFF", k);
				LoadPacket(configTXT, iterIdc, p.SPStatus[i].staticSPChange[k].specificSspChange[l].NC_DIFF);

				sprintf_s(iterIdc, "%s(%d)", "V_DIFF", k);
				LoadPacket(configTXT, iterIdc, p.SPStatus[i].staticSPChange[k].specificSspChange[l].V_DIFF);
			}

		}

		sprintf_s(iterIdc, "%s(%d)", "G_New_Gradient", i);
		LoadPacket(configTXT, iterIdc, p.SPStatus[i].G_New_Gradient);

		sprintf_s(iterIdc, "%s(%d)", "Q_GDIR", i);
		LoadPacket(configTXT, iterIdc, p.SPStatus[i].Q_GDIR);

		sprintf_s(iterIdc, "%s(%d)", "N_ITER", i);
		LoadPacket(configTXT, iterIdc, p.SPStatus[i].N_ITER_ATO_3);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_3; ++k)
		{
			sprintf_s(iterIdc, "%s(%d)", "D_Location", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].gradientChange[k].D_Location);

			sprintf_s(iterIdc, "%s(%d)", "G_New_Gradient", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].gradientChange[k].G_New_Gradient);

			sprintf_s(iterIdc, "%s(%d)", "Q_GDIR", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].gradientChange[k].Q_GDIR);

		}

		sprintf_s(iterIdc, "%s(%d)", "Q_Radius_Category", i);
		LoadPacket(configTXT, iterIdc, p.SPStatus[i].Q_Radius_Category);

		sprintf_s(iterIdc, "%s(%d)", "N_ITER", i);
		LoadPacket(configTXT, iterIdc, p.SPStatus[i].N_ITER_ATO_4);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_4; ++k)
		{
			sprintf_s(iterIdc, "%s(%d)", "D_Location", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].curvesChange[k].D_Location);

			sprintf_s(iterIdc, "%s(%d)", "Q_Radius_Category", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].curvesChange[k].Q_Radius_Category);
		}

		sprintf_s(iterIdc, "%s(%d)", "M_VOLTAGE", i);
		LoadPacket(configTXT, iterIdc, p.SPStatus[i].M_VOLTAGE);

		sprintf_s(iterIdc, "%s(%d)", "NID_CTRACTION", i);
		LoadPacket(configTXT, iterIdc, p.SPStatus[i].NID_CTRACTION);

		sprintf_s(iterIdc, "%s(%d)", "N_ITER", i);
		LoadPacket(configTXT, iterIdc, p.SPStatus[i].N_ITER_ATO_5);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_5; ++k)
		{
			sprintf_s(iterIdc, "%s(%d)", "D_Location", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].powerVoltageChange[k].D_Location);

			sprintf_s(iterIdc, "%s(%d)", "M_VOLTAGE", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].powerVoltageChange[k].M_VOLTAGE);

			sprintf_s(iterIdc, "%s(%d)", "NID_CTRACTION", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].powerVoltageChange[k].NID_CTRACTION);
		}

		sprintf_s(iterIdc, "%s(%d)", "M_CURRENT", i);
		LoadPacket(configTXT, iterIdc, p.SPStatus[i].M_CURRENT);

		sprintf_s(iterIdc, "%s(%d)", "N_ITER", i);
		LoadPacket(configTXT, iterIdc, p.SPStatus[i].N_ITER_ATO_6);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_6; ++k)
		{
			sprintf_s(iterIdc, "%s(%d)", "D_Location", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].currentLimitChange[k].D_Location);

			sprintf_s(iterIdc, "%s(%d)", "M_CURRENT", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].currentLimitChange[k].M_CURRENT);
		}

		sprintf_s(iterIdc, "%s(%d)", "N_ITER", i);
		LoadPacket(configTXT, iterIdc, p.SPStatus[i].N_ITER_ATO_7);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_7; ++k)
		{
			sprintf_s(iterIdc, "%s(%d)", "Q_NEWNID_C", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].baliseGroup[k].Q_NEWNID_C);

			sprintf_s(iterIdc, "%s(%d)", "NID_C", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].baliseGroup[k].NID_C);

			sprintf_s(iterIdc, "%s(%d)", "NID_BG", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].baliseGroup[k].NID_BG);

			sprintf_s(iterIdc, "%s(%d)", "N_ITER_BG", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].baliseGroup[k].N_ITER_BG);

			for (int l = 0; l < (int)p.SPStatus[i].baliseGroup[k].N_ITER_BG; ++l)
			{
				sprintf_s(iterIdc, "%s(%d)", "N_PIG", l);
				LoadPacket(configTXT, iterIdc, p.SPStatus[i].baliseGroup[k].balises[l].N_PIG);

				sprintf_s(iterIdc, "%s(%d)", "D_Location", l);
				LoadPacket(configTXT, iterIdc, p.SPStatus[i].baliseGroup[k].balises[l].D_Location);
			}
		}

		sprintf_s(iterIdc, "%s(%d)", "N_ITER", i);
		LoadPacket(configTXT, iterIdc, p.SPStatus[i].N_ITER_ATO_8);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_8; ++k)
		{
			sprintf_s(iterIdc, "%s(%d)", "NID_TP", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].timingPoints[k].NID_TP);

			sprintf_s(iterIdc, "%s(%d)", "D_Location", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].timingPoints[k].D_Location);

			sprintf_s(iterIdc, "%s(%d)", "Q_Stop_Location_Tolerance", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].timingPoints[k].Q_Stop_Location_Tolerance);

			sprintf_s(iterIdc, "%s(%d)", "Q_STP_Reached", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].timingPoints[k].Q_STP_Reached);
			
			sprintf_s(iterIdc, "%s(%d)", "L_TEXT", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].timingPoints[k].L_TEXT);

			for (int l = 0; l < (int)p.SPStatus[i].timingPoints[k].L_TEXT; ++l)
			{
				sprintf_s(iterIdc, "%s(%d)", "X_TEXT", l);
				LoadPacket(configTXT, iterIdc, p.SPStatus[i].timingPoints[k].X_TEXT[l]);
			}

		}

		sprintf_s(iterIdc, "%s(%d)", "N_ITER", i);
		LoadPacket(configTXT, iterIdc, p.SPStatus[i].N_ITER_ATO_9);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_9; ++k)
		{
			sprintf_s(iterIdc, "%s(%d)", "Q_Range", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].platformArea[k].Q_Range);

			sprintf_s(iterIdc, "%s(%d)", "D_Start_Location", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].platformArea[k].D_Start_Location);

			sprintf_s(iterIdc, "%s(%d)", "D_End_Location", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].platformArea[k].D_End_Location);
		}

		sprintf_s(iterIdc, "%s(%d)", "N_ITER", i);
		LoadPacket(configTXT, iterIdc, p.SPStatus[i].N_ITER_ATO_10);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_10; ++k)
		{
			sprintf_s(iterIdc, "%s(%d)", "Q_Range", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].tunnel[k].Q_Range);

			sprintf_s(iterIdc, "%s(%d)", "Q_Tunnel_Category", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].tunnel[k].Q_Tunnel_Category);

			sprintf_s(iterIdc, "%s(%d)", "D_Start_Location", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].tunnel[k].D_Start_Location);

			sprintf_s(iterIdc, "%s(%d)", "D_End_Location", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].tunnel[k].D_End_Location);
		}

		sprintf_s(iterIdc, "%s(%d)", "N_ITER", i);
		LoadPacket(configTXT, iterIdc, p.SPStatus[i].N_ITER_ATO_11);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_11; ++k)
		{
			sprintf_s(iterIdc, "%s(%d)", "Q_Range", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].axleLoadSpeedProfile[k].Q_Range);

			sprintf_s(iterIdc, "%s(%d)", "M_AXLELOADCAT", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].axleLoadSpeedProfile[k].M_AXLELOADCAT);

			sprintf_s(iterIdc, "%s(%d)", "V_New_Speed_Level", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].axleLoadSpeedProfile[k].V_New_Speed_Level);

			sprintf_s(iterIdc, "%s(%d)", "Q_FRONT", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].axleLoadSpeedProfile[k].Q_FRONT);

			sprintf_s(iterIdc, "%s(%d)", "D_Start_Location", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].axleLoadSpeedProfile[k].D_Start_Location);

			sprintf_s(iterIdc, "%s(%d)", "D_End_Location", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].axleLoadSpeedProfile[k].D_End_Location);
		}

		sprintf_s(iterIdc, "%s(%d)", "N_ITER", i);
		LoadPacket(configTXT, iterIdc, p.SPStatus[i].N_ITER_ATO_12);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_12; ++k)
		{
			sprintf_s(iterIdc, "%s(%d)", "D_UnprotectedLx_Stop_Nominal", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].unprotectedLevelCrossingStop[k].D_UnprotectedLx_Stop_Nominal);

			sprintf_s(iterIdc, "%s(%d)", "D_UnprotectedLx_Stop_Reverse", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].unprotectedLevelCrossingStop[k].D_UnprotectedLx_Stop_Reverse);
		}

		sprintf_s(iterIdc, "%s(%d)", "N_ITER", i);
		LoadPacket(configTXT, iterIdc, p.SPStatus[i].N_ITER_ATO_13);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_13; ++k)
		{
			sprintf_s(iterIdc, "%s(%d)", "Q_Range", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].permittedBrakingDistance[k].Q_Range);

			sprintf_s(iterIdc, "%s(%d)", "D_Permitted_Braking_Distance", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].permittedBrakingDistance[k].D_Permitted_Braking_Distance);

			sprintf_s(iterIdc, "%s(%d)", "Q_PBD_SBEB", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].permittedBrakingDistance[k].Q_PBD_SBEB);

			sprintf_s(iterIdc, "%s(%d)", "G_PBD", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].permittedBrakingDistance[k].G_PBD);

			sprintf_s(iterIdc, "%s(%d)", "Q_GDIR_PBD", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].permittedBrakingDistance[k].Q_GDIR_PBD);

			sprintf_s(iterIdc, "%s(%d)", "D_Start_Location", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].permittedBrakingDistance[k].D_Start_Location);

			sprintf_s(iterIdc, "%s(%d)", "D_End_Location", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].permittedBrakingDistance[k].D_End_Location);
		}

		sprintf_s(iterIdc, "%s(%d)", "N_ITER", i);
		LoadPacket(configTXT, iterIdc, p.SPStatus[i].N_ITER_ATO_14);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_14; ++k)
		{
			sprintf_s(iterIdc, "%s(%d)", "Q_Range", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].switchOffRegenerativeBrake[k].Q_Range);

			sprintf_s(iterIdc, "%s(%d)", "D_Start_Location", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].switchOffRegenerativeBrake[k].D_Start_Location);

			sprintf_s(iterIdc, "%s(%d)", "D_End_Location", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].switchOffRegenerativeBrake[k].D_End_Location);
		}

		sprintf_s(iterIdc, "%s(%d)", "N_ITER", i);
		LoadPacket(configTXT, iterIdc, p.SPStatus[i].N_ITER_ATO_15);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_15; ++k)
		{
			sprintf_s(iterIdc, "%s(%d)", "Q_Range", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].switchOffEddyCurrentBrake[k].Q_Range);

			sprintf_s(iterIdc, "%s(%d)", "D_Start_Location", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].switchOffEddyCurrentBrake[k].D_Start_Location);

			sprintf_s(iterIdc, "%s(%d)", "D_End_Location", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].switchOffEddyCurrentBrake[k].D_End_Location);
		}

		sprintf_s(iterIdc, "%s(%d)", "N_ITER", i);
		LoadPacket(configTXT, iterIdc, p.SPStatus[i].N_ITER_ATO_16);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_16; ++k)
		{
			sprintf_s(iterIdc, "%s(%d)", "Q_Range", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].switchOffEddyEmergencyBrake[k].Q_Range);

			sprintf_s(iterIdc, "%s(%d)", "D_Start_Location", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].switchOffEddyEmergencyBrake[k].D_Start_Location);

			sprintf_s(iterIdc, "%s(%d)", "D_End_Location", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].switchOffEddyEmergencyBrake[k].D_End_Location);
		}

		sprintf_s(iterIdc, "%s(%d)", "N_ITER", i);
		LoadPacket(configTXT, iterIdc, p.SPStatus[i].N_ITER_ATO_17);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_17; ++k)
		{
			sprintf_s(iterIdc, "%s(%d)", "Q_Range", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].switchOffMagmaticShoeBrake[k].Q_Range);

			sprintf_s(iterIdc, "%s(%d)", "D_Start_Location", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].switchOffMagmaticShoeBrake[k].D_Start_Location);

			sprintf_s(iterIdc, "%s(%d)", "D_End_Location", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].switchOffMagmaticShoeBrake[k].D_End_Location);
		}

		sprintf_s(iterIdc, "%s(%d)", "N_ITER", i);
		LoadPacket(configTXT, iterIdc, p.SPStatus[i].N_ITER_ATO_18);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_18; ++k)
		{
			sprintf_s(iterIdc, "%s(%d)", "Q_Range", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].areaWithDynamicBrakeForceInhibition[k].Q_Range);

			sprintf_s(iterIdc, "%s(%d)", "D_Start_Location", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].areaWithDynamicBrakeForceInhibition[k].D_Start_Location);

			sprintf_s(iterIdc, "%s(%d)", "D_End_Location", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].areaWithDynamicBrakeForceInhibition[k].D_End_Location);
		}

		sprintf_s(iterIdc, "%s(%d)", "N_ITER", i);
		LoadPacket(configTXT, iterIdc, p.SPStatus[i].N_ITER_ATO_19);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_19; ++k)
		{
			sprintf_s(iterIdc, "%s(%d)", "Q_Range", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].areaWithLimitedDynamicBrakeForce[k].Q_Range);

			sprintf_s(iterIdc, "%s(%d)", "M_Dynamic_Braking_Force_Limit", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].areaWithLimitedDynamicBrakeForce[k].M_Dynamic_Braking_Force_Limit);

			sprintf_s(iterIdc, "%s(%d)", "D_Start_Location", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].areaWithLimitedDynamicBrakeForce[k].D_Start_Location);

			sprintf_s(iterIdc, "%s(%d)", "D_End_Location", k);
			LoadPacket(configTXT, iterIdc, p.SPStatus[i].areaWithLimitedDynamicBrakeForce[k].D_End_Location);
		}

	}
}


void packet07Tab::ReLoadPacketSP()
{
	PropertyCtrlBuilder::PacketSPStructureToCtrl(*pFileSPStructure, m_propgrid.GetProperty(BODY_DATA));
}




void packet07Tab::OnBnClickedBtnSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


		//Ctrl -> Structure -> File
	
		PacketSPCtrlToStructure(m_propgrid.GetProperty(BODY_DATA), *pFileSPStructure);

		PacketSPStructrureToINI(*pFileSPStructure);
	
}





void packet07Tab::OnBnClickedBtnLoad()
{
	
		if (LoadPacketSPFromFile() == FALSE) return; //파일 읽기 실패 시 그대로 종료
		ReLoadPacketSP();
		
}

int packet07Tab::getPacketCounter()
{
	m_nPacketCounter++;

	if (m_nPacketCounter > 255)
		m_nPacketCounter = -1;

	return m_nPacketCounter;
}


void packet07Tab::OnBnClickedButton1()
{
	
	BYTE	buffer[1024]{};
	UINT32  nPos = 0;
	int bodylength = 0;
	CString csBody;
	CString csTmp;
	CStdioFile ipfile;
	CStdioFile* pipfile = &ipfile;

	PacketSPCtrlToStructure(m_propgrid.GetProperty(BODY_DATA), *pRDSPStructure);

	EncodeMessage_ATO_Packet07_Sp(pRDSPStructure, &nPos, buffer, sizeof(buffer));

	bodylength = (GetLengthPacket07_Sp(pRDSPStructure) - GetATOHeaderSize()) % 8;

	if (bodylength == 0)
	{
		bodylength = (GetLengthPacket07_Sp(pRDSPStructure) - GetATOHeaderSize()) / 8;
	}

	else
	{
		bodylength = ((GetLengthPacket07_Sp(pRDSPStructure) - GetATOHeaderSize()) / 8) + 1;
	}

	memcpy(buffer, buffer, bodylength);

	CString dir = Utility::GetDirectoryFromFileDialog(_T("*.ini"), _T("SP_Raw_Data"), OFN_HIDEREADONLY, szConfigFilter);
	if (ipfile.Open(dir.GetString(), CFile::modeCreate | CFile::modeWrite) == FALSE) return;
	Utility::CFileWriteCHAR(pipfile, "[Segment Profile Raw Data]");
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


BOOL packet07Tab::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}

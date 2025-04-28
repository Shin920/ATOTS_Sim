#include "pch.h"
#include "PropertyCtrlBuilder.h"
#include "GridPropertyOverride.h"

using namespace PacketHSAckEnum;
using namespace PacketHSRejEnum;
using namespace PacketJPEnum;
using namespace PacketSPEnum;
using namespace PacketSTRAckEnum;

using namespace PacketHeaderEnum;
using namespace PacketHSReqEnum;
using namespace PacketJPReqEnum;
using namespace PacketJPAckEnum;
using namespace PacketSPReqEnum;
using namespace PacketSTREnum;
using namespace PacketSESSTermEnum;
using namespace std;

void PropertyCtrlBuilder::BuildBodyPkHSReq(CMFCPropertyGridProperty* pxBodyProperty)
{
	//CMFCPropertyGridProperty* pxProperty;
	CMFCPropertyIterator* pxiterContainer;
	CMFCPropertyGridProperty* pxiter;

	//pxProperty = new CMFCPropertyGridProperty(_T("Dynamic Data"));
	//pxBodyProperty->AddSubItem(pxProperty);

	pxiterContainer = new CMFCPropertyIteratorATO_VER(_T("N_ITER"), (variant_t)(int)0,
		_T("지원되는 ATO 버전의 개수"));
	pxiterContainer->SetFormat(_T("%d"));
	pxiterContainer->SetMinMaxValue(0, UINT8_MAX);
	pxiterContainer->AllowEdit(FALSE);
	pxBodyProperty->AddSubItem(pxiterContainer);

	pxiter = new CMFCPropertyGridProperty(_T("ATO_Version_ITER Property"));
	pxBodyProperty->AddSubItem(pxiter);

	pxiterContainer->SetTargetProperty(pxiter);
	pxiterContainer->AdjustIteratorProperty();
}

void PropertyCtrlBuilder::BuildBodyPkJPReq(CMFCPropertyGridProperty* pxBodyProperty)
{

	CMFCPropertyEX<int>* pxi;
	CMFCPropertyEX<UINT32>* pxu;

	pxi = new CMFCPropertyEX<int>(_T("NID_C"), (variant_t)(int)0,
		_T("참조 SP의 나라 혹은 지역의 ID"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b1111111111);
	pxi->AllowEdit(FALSE);
	pxBodyProperty->AddSubItem(pxi);

	pxu = new CMFCPropertyEX<UINT32>(_T("NID_SP"), (variant_t)(UINT32)0,
		_T("JP가 요청되는 SP의 ID"));
	pxu->SetFormat(_T("%u"));
	pxu->SetMinMaxValue(0, 0xffffffff);
	pxu->AllowEdit(FALSE);
	pxBodyProperty->AddSubItem(pxu);
}

void PropertyCtrlBuilder::BuildBodyPkJPAck(CMFCPropertyGridProperty* pxBodyProperty)
{

	CMFCPropertyEX<int>* pxi;
	

	pxi = new CMFCPropertyEX<int>(_T("T_JP_Reference_Timestamp_Date"), (variant_t)(int)0,
		_T("JP Ack의 Timestamp 날짜"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b111111111111111);
	pxi->AllowEdit(FALSE);
	pxBodyProperty->AddSubItem(pxi);

	pxi = new CMFCPropertyEX<int>(_T("T_JP_Reference_Timestamp_Seconds"), (variant_t)(int)0,
		_T("JP Ack의 Timestamp 시간초"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b11111111111111111);
	pxi->AllowEdit(FALSE);
	pxBodyProperty->AddSubItem(pxi);

	pxi = new CMFCPropertyEX<int>(_T("N_JP_Reference_Packet_Counter"), (variant_t)(int)0,
		_T("JP Ack의 Packet Counter"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, UINT8_MAX);
	pxi->AllowEdit(FALSE);
	pxBodyProperty->AddSubItem(pxi);

}

void PropertyCtrlBuilder::BuildBodyPkSPReq(CMFCPropertyGridProperty* pxBodyProperty)
{
	CMFCPropertyIterator* pxiterContainer;
	CMFCPropertyGridProperty* pxiter;

	pxiterContainer = new CMFCPropertyIteratorSPReq_SP(_T("N_ITER"), (variant_t)(int)0,
		_T("요청된 SP의 개수"));
	pxiterContainer->SetFormat(_T("%d"));
	pxiterContainer->SetMinMaxValue(0, UINT8_MAX);
	pxiterContainer->AllowEdit(FALSE);
	pxBodyProperty->AddSubItem(pxiterContainer);

	pxiter = new CMFCPropertyGridProperty(_T("SP_ITER Property"));
	pxBodyProperty->AddSubItem(pxiter);

	pxiterContainer->SetTargetProperty(pxiter);
	pxiterContainer->AdjustIteratorProperty();
}

void PropertyCtrlBuilder::BuildBodyPkSTR(CMFCPropertyGridProperty* pxBodyProperty)
{

	CMFCPropertyEX<int>* pxi;
	CMFCPropertyEX<UINT32>* pxu;
	CMFCPropertyStr* pxstr;
	CMFCPropertyIterator* pxiterContainer;
	CMFCPropertyGridProperty* pxiter;

	pxi = new CMFCPropertyEX<int>(_T("M_ATO_State"), (variant_t)(int)0, _T("현재 ATO 상태"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b1111);
	pxi->AllowEdit(FALSE);
	pxi->SetValue((variant_t)(int)0);

	pxBodyProperty->AddSubItem(pxi);

	pxi = new CMFCPropertyEX<int>(_T("Q_STR_Indicators"), (variant_t)(int)0, _T("상태를 표시하는 Bitset"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xffff);
	pxi->AllowEdit(FALSE);
	pxi->SetValue((variant_t)(int)0);

	pxBodyProperty->AddSubItem(pxi);

	pxi = new CMFCPropertyEX<int>(_T("V_TRAIN_ATO"), (variant_t)(int)0, _T("STR이 보내질 때의 현재 속도"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b1111111111);
	pxi->AllowEdit(FALSE);
	pxi->SetValue((variant_t)(int)0);

	pxBodyProperty->AddSubItem(pxi);

	pxi = new CMFCPropertyEX<int>(_T("L_TRAIN"), (variant_t)(int)0, _T("열차 길이"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xfff);
	pxi->AllowEdit(FALSE);
	pxi->SetValue((variant_t)(int)0);

	pxBodyProperty->AddSubItem(pxi);

	pxstr = new CMFCPropertyStr(_T("DRIVER_ID"), _T("init value"), _T("운전자 ID"));
	pxstr->maxLength = 16;
	pxstr->Enable(FALSE);

	pxBodyProperty->AddSubItem(pxstr);

	pxi = new CMFCPropertyEX<int>(_T("NID_C"), (variant_t)(int)0, _T("SP의 나라 혹은 지역의 ID"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b1111111111);
	pxi->AllowEdit(FALSE);
	pxi->SetValue((variant_t)(int)0);

	pxBodyProperty->AddSubItem(pxi);

	pxu = new CMFCPropertyEX<UINT32>(_T("NID_SP"), (variant_t)(UINT32)0, _T("SP ID"));		// pxu min max value err chk
	pxu->SetFormat(_T("%d"));
	pxu->SetMinMaxValue(0, 0xffffffff);
	pxu->AllowEdit(FALSE);
	pxu->SetValue((variant_t)(UINT32)0);

	pxBodyProperty->AddSubItem(pxu);

	pxi = new CMFCPropertyEX<int>(_T("D_Sending_Position"), (variant_t)(int)0, _T("STR이 보내진 순간의 열차 추정 전단부의 위치"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xffffff);
	pxi->AllowEdit(FALSE);
	pxi->SetValue((variant_t)(int)0);

	pxBodyProperty->AddSubItem(pxi);

	pxi = new CMFCPropertyEX<int>(_T("NID_C"), (variant_t)(int)0, _T("이전 TP의 나라 혹은 지역의 ID"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b1111111111);
	pxi->AllowEdit(FALSE);
	pxi->SetValue((variant_t)(int)0);

	pxBodyProperty->AddSubItem(pxi);

	pxu = new CMFCPropertyEX<UINT32>(_T("NID_TP"), (variant_t)(UINT32)0, _T("이전 TP ID"));
	pxu->SetFormat(_T("%u"));
	pxu->SetMinMaxValue(0, 0xffffffff);
	pxu->AllowEdit(FALSE);
	pxu->SetValue((variant_t)(UINT32)0);

	pxBodyProperty->AddSubItem(pxu);

	pxi = new CMFCPropertyEX<int>(_T("Q_Pass_Stop_Depart"), (variant_t)(int)0, _T("열차가 TP에서 멈췄는지 TP에서 출발했는지 TP를 통과했는지 표시하는 Qualifier"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b11);
	pxi->AllowEdit(FALSE);
	pxi->SetValue((variant_t)(int)0);

	pxBodyProperty->AddSubItem(pxi);

	pxi = new CMFCPropertyEX<int>(_T("Q_Accurate_Stopping"), (variant_t)(int)0, _T("이 qualifier는 열차가 Operational Stopping Point에 정확하게 멈췄는지 혹은 아닌지 명시합니다"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b11);
	pxi->AllowEdit(FALSE);
	pxi->SetValue((variant_t)(int)0);

	pxBodyProperty->AddSubItem(pxi);

	pxiterContainer = new CMFCPropertyIteratorSTR_TP(_T("N_ITER"), (variant_t)(int)1, _T("TP 정보의 개수"));
	pxiterContainer->SetFormat(_T("%d"));
	pxiterContainer->SetMinMaxValue(1, 0b11111);
	pxiterContainer->AllowEdit(FALSE);
	pxBodyProperty->AddSubItem(pxiterContainer);

	pxiter = new CMFCPropertyGridProperty(_T("N_ITER (TP INFO)"));
	pxBodyProperty->AddSubItem(pxiter);

	pxiterContainer->SetTargetProperty(pxiter);
	pxiterContainer->AdjustIteratorProperty();

}

void PropertyCtrlBuilder::BuildBodyPkSessTerm(CMFCPropertyGridProperty* pxBodyProperty)
{
	CMFCPropertyEX<int>* pxi;


	pxi = new CMFCPropertyEX<int>(_T("Q_Termination_Reason"), (variant_t)(int)0, _T("통신세션 종료 원인을 표시하는 Qualifier"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b1111);
	pxi->AllowEdit(FALSE);
	pxi->SetValue((variant_t)(int)0);

	pxBodyProperty->AddSubItem(pxi);
}

void PropertyCtrlBuilder::PacketHSReqStructureToCtrl(const OB_TS_Packet00_HSReqStruct& p, CMFCPropertyGridProperty* bodyProperty)
{
	CMFCPropertyGridProperty* cp = bodyProperty;

	Utility::SetValueWithTypeCasting(cp->GetSubItem(IDX_HSR_N_ITER), p.N_ITER);

	for (int i = 0; i < (int)p.N_ITER; ++i)
	{
		CMFCPropertyGridProperty* subProperty = cp->GetSubItem(IDX_HSR_N_ITER_Property)->GetSubItem(i);
		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_HSR_M_ATO_Version), p.M_ATO_Version[i]);
	}
}

void PropertyCtrlBuilder::PacketJPReqStructureToCtrl(const OB_TS_Packet03_JPReqStruct& p, CMFCPropertyGridProperty* bodyProperty)
{
	Utility::SetValueWithTypeCasting(bodyProperty->GetSubItem(IDX_JPR_NID_C), p.NID_C);
	Utility::SetValueWithTypeCasting(bodyProperty->GetSubItem(IDX_JPR_NID_SP), p.NID_SP);
}

void PropertyCtrlBuilder::PacketJPAckStructureToCtrl(const OB_TS_Packet05_JPAckStruct& p, CMFCPropertyGridProperty* bodyProperty)
{
	Utility::SetValueWithTypeCasting(bodyProperty->GetSubItem(IDX_T_JP_Reference_Timestamp_Date), p.T_JP_Ref_TimeStamp_Date);
	Utility::SetValueWithTypeCasting(bodyProperty->GetSubItem(IDX_T_JP_Reference_Timestamp_Seconds), p.T_JP_Ref_TimeStamp_Seconds);
	Utility::SetValueWithTypeCasting(bodyProperty->GetSubItem(IDX_N_JP_Reference_Packet_Counter), p.N_JP_Ref_Packet_Counter);
}

void PropertyCtrlBuilder::PacketSPReqStructureToCtrl(const OB_TS_Packet06_SPReqStruct& p, CMFCPropertyGridProperty* bodyProperty)
{
	CMFCPropertyGridProperty* cp = bodyProperty;

	Utility::SetValueWithTypeCasting(cp->GetSubItem(IDX_SPR_N_ITER), p.N_ITER_ATO);

	for (int i = 0; i < (int)p.N_ITER_ATO; ++i)
	{
		CMFCPropertyGridProperty* subProperty = cp->GetSubItem(IDX_SPR_ITER_Property)->GetSubItem(i);
		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPR_NID_C), p.SPReqDetails[i].NID_C);
		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPR_NID_SP), p.SPReqDetails[i].NID_SP);
	}
}

void PropertyCtrlBuilder::PacketSTRStructureToCtrl(const OB_TS_Packet08_STRStruct& p, CMFCPropertyGridProperty* bodyProperty)
{
	CMFCPropertyGridProperty* cp = bodyProperty;
	Utility::SetValueWithTypeCasting(bodyProperty->GetSubItem(IDX_STR_M_ATO_State), p.M_ATO_State);
	Utility::SetValueWithTypeCasting(bodyProperty->GetSubItem(IDX_STR_Q_STR_Indicators), p.Q_STR_Indication);
	Utility::SetValueWithTypeCasting(bodyProperty->GetSubItem(IDX_STR_V_TRAIN_ATO), p.V_TRAIN_ATO);
	Utility::SetValueWithTypeCasting(bodyProperty->GetSubItem(IDX_STR_L_TRAIN), p.L_TRAIN);	

	CString tempStr;
	CHAR tempChar[16]{};
	int i = 0;
	for (; i < 15 && isalnum((unsigned char)p.DRIVER_ID[i]) != 0; ++i) {
		tempChar[i] = (CHAR)p.DRIVER_ID[i];
	}
	tempChar[i] = '\0';  // 널 종료 문자 추가
	tempStr = tempChar;
	Utility::SetValueWithTypeCasting(bodyProperty->GetSubItem(IDX_STR_DRIVER_ID), tempStr.GetString());


	Utility::SetValueWithTypeCasting(bodyProperty->GetSubItem(IDX_STR_NID_C_SP), p.sendingLocation.NID_C);
	Utility::SetValueWithTypeCasting(bodyProperty->GetSubItem(IDX_STR_NID_SP), p.sendingLocation.NID_SP);
	Utility::SetValueWithTypeCasting(bodyProperty->GetSubItem(IDX_STR_D_Sending_Position), p.sendingLocation.D_Sending_Position);
	Utility::SetValueWithTypeCasting(bodyProperty->GetSubItem(IDX_STR_NID_C_TP), p.previousTpInfo.NID_C);
	Utility::SetValueWithTypeCasting(bodyProperty->GetSubItem(IDX_STR_NID_TP), p.previousTpInfo.NID_TP);
	Utility::SetValueWithTypeCasting(bodyProperty->GetSubItem(IDX_STR_Q_Pass_Stop_Depart), p.previousTpInfo.Q_Pass_Stop_Depart);
	Utility::SetValueWithTypeCasting(bodyProperty->GetSubItem(IDX_STR_Q_Accurate_Stopping), p.previousTpInfo.Q_Accurate_Stopping);
	Utility::SetValueWithTypeCasting(bodyProperty->GetSubItem(IDX_STR_N_ITER), p.N_ITER);

	for (int i = 0; i < (int)p.N_ITER; ++i)
	{
		CMFCPropertyGridProperty* subProperty = cp->GetSubItem(IDX_STR_TPE_ITER_Property)->GetSubItem(i);
		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_NID_C_TPE), p.TPEstimation[i].NID_C);
		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_NID_TP_TPE), p.TPEstimation[i].NID_TP);
		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_T_Arrival_Date_TPE), p.TPEstimation[i].T_Arrival_Date);
		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_T_Arrival_Seconds_TPE), p.TPEstimation[i].T_Arrival_Seconds);
	}
}

void PropertyCtrlBuilder::PacketSessTermStructureToCtrl(const OB_TS_Packet11_SESSTermStruct& p, CMFCPropertyGridProperty* bodyProperty)
{
	CMFCPropertyGridProperty* cp = bodyProperty;
	Utility::SetValueWithTypeCasting(bodyProperty->GetSubItem(IDX_Q_Termination_Reason), p.Q_Termination_Reason);
}

//--------------------------------------------------Send Tab---------------------------------------------------------------//

void PropertyCtrlBuilder::BuildBodyPkHSAck(CMFCPropertyGridProperty* pxBodyProperty)
{
	CMFCPropertyEX<int>* pxi;	

	pxi = new CMFCPropertyEX<int>(_T("NID_C"), (variant_t)(int)0, _T("SP의 나라 혹은 지역의 ID"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b1111111111);
	pxi->AllowEdit(TRUE);
	pxi->SetValue((variant_t)(int)887);

	pxBodyProperty->AddSubItem(pxi);

	pxi = new CMFCPropertyEX<int>(_T("NID_ATOTS"), (variant_t)(int)0, _T("ATO-TS의 ID"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b11111111111111);
	pxi->AllowEdit(TRUE);
	pxi->SetValue((variant_t)(int)2);

	pxBodyProperty->AddSubItem(pxi);

	pxi = new CMFCPropertyEX<int>(_T("M_ATO_Version"), (variant_t)(int)0, _T("사용될 ATO 시스템의 버전"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, UINT16_MAX);
	pxi->AllowEdit(TRUE);
	pxi->SetValue((variant_t)(int)2);

	pxBodyProperty->AddSubItem(pxi);

	pxi = new CMFCPropertyEX<int>(_T("T_Timeout_ATOTS_Response"), (variant_t)(int)0, _T("응답이 없는 경우에 요청을 반복하기 전에 ATO-OB가 기다려야하는 최소시간"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, UINT8_MAX);
	pxi->AllowEdit(TRUE);
	pxi->SetValue((variant_t)(int)2);

	pxBodyProperty->AddSubItem(pxi);

	pxi = new CMFCPropertyEX<int>(_T("T_Reporting_Time"), (variant_t)(int)0, _T("데이터 보고 목적으로 Status Report를 트리거하기 위한 보고 시간 사이클"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, UINT8_MAX);
	pxi->AllowEdit(TRUE);
	pxi->SetValue((variant_t)(int)2);

	pxBodyProperty->AddSubItem(pxi);
}

void PropertyCtrlBuilder::BuildBodyPkHSRej(CMFCPropertyGridProperty* pxBodyProperty)
{
	CMFCPropertyEX<int>* pxi;

	pxi = new CMFCPropertyEX<int>(_T("Q_Reject_Reason"), (variant_t)(int)0, _T("거절 이유"));
	pxi->SetFormat(_T(""));
	pxi->SetMinMaxValue(0, 0b11);
	pxi->AddSpecialOption(_T("시스템 비호환"), _T("%d: 시스템 비호환"));
	pxi->AddSpecialOption(_T("다른 ATO-TS가 담당"), _T("%d: 다른 ATO-TS가 담당"));
	pxi->AddSpecialOption(_T("담당 ATO-TS 알수없음"), _T("%d: 담당 ATO-TS 알수없음"));
	pxi->AllowEdit(FALSE);

	pxBodyProperty->AddSubItem(pxi);

	pxi = new CMFCPropertyEX<int>(_T("NID_C"), (variant_t)(int)0, _T("해당 ATO-TS의 나라 혹은 지역 ID"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b1111111111);
	pxi->AllowEdit(TRUE);
	pxi->SetValue((variant_t)(int)887);

	pxBodyProperty->AddSubItem(pxi);

	pxi = new CMFCPropertyEX<int>(_T("NID_ATOTS"), (variant_t)(int)0, _T("해당 ATO-TS의 ID"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b11111111111111);
	pxi->AllowEdit(TRUE);
	pxi->SetValue((variant_t)(int)0);

	pxBodyProperty->AddSubItem(pxi);
}

void PropertyCtrlBuilder::BuildBodyPkJP(CMFCPropertyGridProperty* pxBodyProperty)
{
	CMFCPropertyIterator* pxiterContainer;
	CMFCPropertyGridProperty* pxiter;
	CMFCPropertyEX<int>* pxi;

	pxi = new CMFCPropertyEX<int>(_T("Q_JP_Status"), (variant_t)(int)0, _T("Journey Profile의 상태"));
	pxi->SetFormat(_T(""));
	pxi->SetMinMaxValue(0, 0b111);
	pxi->AddSpecialOption(_T("Invalid"), _T("%d: Invalid"));
	pxi->AddSpecialOption(_T("Valid"), _T("%d: Valid"));
	pxi->AddSpecialOption(_T("Unavailable"), _T("%d: Unavailable"));
	pxi->AddSpecialOption(_T("Update"), _T("%d: Update"));
	pxi->AddSpecialOption(_T("Overwrite"), _T("%d: Overwrite"));
	pxi->AllowEdit(FALSE);
	//pxi->SetValue((variant_t)(int)2);

	pxBodyProperty->AddSubItem(pxi);

	pxiterContainer = new CMFCPropertyIteratorJP(_T("N_ITER_SP"), (variant_t)(int)3, _T("Segment Profile의 갯수"));
	pxiterContainer->SetFormat(_T("%d"));
	pxiterContainer->SetMinMaxValue(0, UINT8_MAX);
	pxiterContainer->AllowEdit(TRUE);
	pxBodyProperty->AddSubItem(pxiterContainer);

	pxiter = new CMFCPropertyGridProperty(_T("SP_ITER_Property"));
	pxBodyProperty->AddSubItem(pxiter);

	pxiterContainer->SetTargetProperty(pxiter);
	pxiterContainer->AdjustIteratorProperty();

}

void PropertyCtrlBuilder::BuildBodyPkSP(CMFCPropertyGridProperty* pxBodyProperty)
{
	CMFCPropertyIterator* pxiterContainer;
	CMFCPropertyGridProperty* pxiter;
	

	pxiterContainer = new CMFCPropertyIteratorSP(_T("N_ITER"), (variant_t)(int)1, _T("Segment Profile의 개수"));
	pxiterContainer->SetFormat(_T("%d"));
	pxiterContainer->SetMinMaxValue(0, 0b11111);
	pxiterContainer->AllowEdit(TRUE);
	pxBodyProperty->AddSubItem(pxiterContainer);

	pxiter = new CMFCPropertyGridProperty(_T("SP_ITER Property"));
	pxBodyProperty->AddSubItem(pxiter);

	pxiterContainer->SetTargetProperty(pxiter);
	pxiterContainer->AdjustIteratorProperty();
}

void PropertyCtrlBuilder::BuildBodyPkSTRAck(CMFCPropertyGridProperty* pxBodyProperty)
{
	
	CMFCPropertyEX<int>* pxi;

	pxi = new CMFCPropertyEX<int>(_T("T_STR_Reference_Timestamp_Date"), (variant_t)(int)0, _T("각각 제공된 Ack에 대한 STR패킷의 타임스탬프 날짜"));
	pxi->SetMinMaxValue(0, 0b111111111111111);
	pxi->SetFormat(_T("%d"));
	pxi->AllowEdit(TRUE);
	pxBodyProperty->AddSubItem(pxi);

	pxi = new CMFCPropertyEX<int>(_T("T_STR_Reference_Timestamp_Seconds"), (variant_t)(int)0, _T("각각 제공된 Ack에 대한 STR패킷의 타임스탬프 시간초"));
	pxi->SetMinMaxValue(0, 0b11111111111111111);
	pxi->SetFormat(_T("%d"));
	pxi->AllowEdit(TRUE);
	pxBodyProperty->AddSubItem(pxi);

	pxi = new CMFCPropertyEX<int>(_T("N_STR_Reference_Packet_Counter"), (variant_t)(int)0, _T("각각 제공된 Ack에 대한 STR의 N_Packet_Counter"));
	pxi->SetMinMaxValue(0, UINT8_MAX);
	pxi->SetFormat(_T("%d"));
	pxi->AllowEdit(TRUE);
	pxBodyProperty->AddSubItem(pxi);
}

void PropertyCtrlBuilder::BuildBodyPkSessTermReq(CMFCPropertyGridProperty* pxBodyProperty)
{
}

void PropertyCtrlBuilder::PacketHeaderStructureToCtrl(const ATO_Header& p, CMFCPropertyGridProperty* headerProperty)
{
	CMFCPropertyGridProperty* cp = headerProperty;
	Utility::SetValueWithTypeCasting(headerProperty->GetSubItem(IDX_NID_PACKET_ATO), p.NID_PACKET_ATO);
	Utility::SetValueWithTypeCasting(headerProperty->GetSubItem(IDX_NID_OPERATIONAL), p.NID_OPERATIONAL);
	Utility::SetValueWithTypeCasting(headerProperty->GetSubItem(IDX_NID_ENGINE), p.NID_ENGINE);
	Utility::SetValueWithTypeCasting(headerProperty->GetSubItem(IDX_T_Timestamp_Date), p.T_Timestamp_Date);
	Utility::SetValueWithTypeCasting(headerProperty->GetSubItem(IDX_T_Timestamp_Seconds), p.T_Timestamp_Seconds);
	Utility::SetValueWithTypeCasting(headerProperty->GetSubItem(IDX_N_Packet_Counter), p.N_Packet_Counter);

}

void PropertyCtrlBuilder::PacketHSAckStructureToCtrl(const TS_OB_Packet01_HSAckStruct& p, CMFCPropertyGridProperty* bodyProperty)
{
	CMFCPropertyGridProperty* cp = bodyProperty;

	Utility::SetValueWithTypeCasting(bodyProperty->GetSubItem(IDX_HSA_NID_C), p.NID_C);
	Utility::SetValueWithTypeCasting(bodyProperty->GetSubItem(IDX_HSA_NID_ATOTS), p.NID_ATOTS);
	Utility::SetValueWithTypeCasting(bodyProperty->GetSubItem(IDX_HSA_M_ATO_Version), p.M_ATO_Version);
	Utility::SetValueWithTypeCasting(bodyProperty->GetSubItem(IDX_HSA_T_Timeout_ATOTS_Response), p.T_Timeout_ATOTOS_Response);
	Utility::SetValueWithTypeCasting(bodyProperty->GetSubItem(IDX_HSA_T_Reporting_Time), p.T_Reporting_Time);

}

void PropertyCtrlBuilder::PacketHSRejStructureToCtrl(const TS_OB_Packet02_HSRejStruct& p, CMFCPropertyGridProperty* bodyProperty)
{
	CMFCPropertyGridProperty* cp = bodyProperty;

	Utility::SetValueWithTypeCasting(bodyProperty->GetSubItem(IDX_Q_Reject_Reason), p.Q_Reject_Reason);
	Utility::SetValueWithTypeCasting(bodyProperty->GetSubItem(IDX_HSRJ_NID_C), p.NID_C);
	Utility::SetValueWithTypeCasting(bodyProperty->GetSubItem(IDX_HSRJ_NID_ATOTS), p.NID_ATOTS);
	
}

void PropertyCtrlBuilder::PacketJPStructureToCtrl(const TS_OB_Packet04_JPStruct& p, CMFCPropertyGridProperty* bodyProperty)
{
	CMFCPropertyGridProperty* cp = bodyProperty;

	Utility::SetValueWithTypeCasting(bodyProperty->GetSubItem(IDX_Q_JP_Status), p.Q_JP_Status);
	Utility::SetValueWithTypeCasting(bodyProperty->GetSubItem(IDX_N_ITER_SP), p.N_ITER_SP);

	for (int i = 0; i < (int)p.N_ITER_SP; ++i)
	{
		CMFCPropertyGridProperty* subProperty = cp->GetSubItem(IDX_SP_ITER_Property)->GetSubItem(i);
		
		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNJP_NID_C), p.segmentProfileReference[i].NID_C);
		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNJP_NID_SP), p.segmentProfileReference[i].NID_SP);
		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNJP_M_SP_Version), p.segmentProfileReference[i].M_SP_Version);
		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNJP_Q_SP_DIR), p.segmentProfileReference[i].Q_SP_DIR);
		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNJP_N_ITER_TP), p.segmentProfileReference[i].N_ITER);

		for (int k = 0; k < (int)p.segmentProfileReference[i].N_ITER; ++k)
		{
			CMFCPropertyGridProperty* tpProperty = subProperty->GetSubItem(IDX_TIMING_POINT_CONSTRAINTS)->GetSubItem(k);

			Utility::SetValueWithTypeCasting(tpProperty->GetSubItem(IDX_TPNSPNJP_NID_TP), p.segmentProfileReference[i].TPConstraints[k].NID_TP);
			Utility::SetValueWithTypeCasting(tpProperty->GetSubItem(IDX_TPNSPNJP_T_Latest_Arrival_Date), p.segmentProfileReference[i].TPConstraints[k].T_Latest_Arrival_Date);
			Utility::SetValueWithTypeCasting(tpProperty->GetSubItem(IDX_TPNSPNJP_T_Latest_Arrival_Seconds), p.segmentProfileReference[i].TPConstraints[k].T_Latest_Arrival_Seconds);
			Utility::SetValueWithTypeCasting(tpProperty->GetSubItem(IDX_TPNSPNJP_T_Arrival_Window), p.segmentProfileReference[i].TPConstraints[k].T_Arrival_Window);
			Utility::SetValueWithTypeCasting(tpProperty->GetSubItem(IDX_TPNSPNJP_Q_TP_Alignment), p.segmentProfileReference[i].TPConstraints[k].Q_TP_Alignment);
			Utility::SetValueWithTypeCasting(tpProperty->GetSubItem(IDX_TPNSPNJP_Q_Stop_Skip_Pass), p.segmentProfileReference[i].TPConstraints[k].Q_Stop_Skip_Pass);
			Utility::SetValueWithTypeCasting(tpProperty->GetSubItem(IDX_TPNSPNJP_Q_TP_Information), p.segmentProfileReference[i].TPConstraints[k].Q_TP_information);
			Utility::SetValueWithTypeCasting(tpProperty->GetSubItem(IDX_TPNSPNJP_Q_Day_Light_Saving), p.segmentProfileReference[i].TPConstraints[k].Q_Day_Light_Saving);
			Utility::SetValueWithTypeCasting(tpProperty->GetSubItem(IDX_TPNSPNJP_Q_Opening_Door_Side), p.segmentProfileReference[i].TPConstraints[k].stpInformation.Q_Opening_Door_Side);
			Utility::SetValueWithTypeCasting(tpProperty->GetSubItem(IDX_TPNSPNJP_Q_Centralised_Opening), p.segmentProfileReference[i].TPConstraints[k].stpInformation.Q_Centralised_Opening);
			Utility::SetValueWithTypeCasting(tpProperty->GetSubItem(IDX_TPNSPNJP_Q_Relaxed_Coupler), p.segmentProfileReference[i].TPConstraints[k].stpInformation.Q_Relaxed_Coupler);
			Utility::SetValueWithTypeCasting(tpProperty->GetSubItem(IDX_TPNSPNJP_Q_Train_Hold), p.segmentProfileReference[i].TPConstraints[k].stpDepartureDetails.Q_Train_Hold);
			Utility::SetValueWithTypeCasting(tpProperty->GetSubItem(IDX_TPNSPNJP_T_Departure_Date), p.segmentProfileReference[i].TPConstraints[k].stpDepartureDetails.T_Departure_Date);
			Utility::SetValueWithTypeCasting(tpProperty->GetSubItem(IDX_TPNSPNJP_T_Departure_Seconds), p.segmentProfileReference[i].TPConstraints[k].stpDepartureDetails.T_Departure_Seconds);
			Utility::SetValueWithTypeCasting(tpProperty->GetSubItem(IDX_TPNSPNJP_T_Minimum_Dwell_Time), p.segmentProfileReference[i].TPConstraints[k].stpDepartureDetails.T_Minimum_Dwell_Time);
			Utility::SetValueWithTypeCasting(tpProperty->GetSubItem(IDX_TPNSPNJP_Q_Automatic_Closing), p.segmentProfileReference[i].TPConstraints[k].stpDepartureDetails.Q_Automatic_Closing);
		}

		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNJP_N_ITER_TC), p.segmentProfileReference[i].N_ITER_tempConstraints);

		for (int k = 0; k < (int)p.segmentProfileReference[i].N_ITER_tempConstraints; ++k)
		{
			CMFCPropertyGridProperty* tcProperty = subProperty->GetSubItem(IDX_TEMPORARY_CONSTRAINTS)->GetSubItem(k);

			Utility::SetValueWithTypeCasting(tcProperty->GetSubItem(IDX_TCNSPNJP_Q_TC_Type), p.segmentProfileReference[i].temporaryConstrains[k].Q_TC_Type);
			Utility::SetValueWithTypeCasting(tcProperty->GetSubItem(IDX_TCNSPNJP_Q_Range), p.segmentProfileReference[i].temporaryConstrains[k].Q_Range);
			Utility::SetValueWithTypeCasting(tcProperty->GetSubItem(IDX_TCNSPNJP_D_TC_Start_Location), p.segmentProfileReference[i].temporaryConstrains[k].D_TC_Start_Location);
			Utility::SetValueWithTypeCasting(tcProperty->GetSubItem(IDX_TCNSPNJP_D_TC_End_Location), p.segmentProfileReference[i].temporaryConstrains[k].D_TC_End_Location);
			Utility::SetValueWithTypeCasting(tcProperty->GetSubItem(IDX_TCNSPNJP_Q_FRONT), p.segmentProfileReference[i].temporaryConstrains[k].asr.Q_FRONT);
			Utility::SetValueWithTypeCasting(tcProperty->GetSubItem(IDX_TCNSPNJP_V_Speed_Level), p.segmentProfileReference[i].temporaryConstrains[k].asr.V_Speed_Level);
			Utility::SetValueWithTypeCasting(tcProperty->GetSubItem(IDX_TCNSPNJP_Q_Adhesion_Category), p.segmentProfileReference[i].temporaryConstrains[k].lowAdhesion.Q_Adhesion_Category);
			Utility::SetValueWithTypeCasting(tcProperty->GetSubItem(IDX_TCNSPNJP_M_CURRENT), p.segmentProfileReference[i].temporaryConstrains[k].currentLimitation.M_CURRENT);
		}

	}

}

void PropertyCtrlBuilder::PacketSPStructureToCtrl(const TS_OB_Packet07_SPStruct& p, CMFCPropertyGridProperty* bodyProperty)
{
	CMFCPropertyGridProperty* cp = bodyProperty;

	Utility::SetValueWithTypeCasting(bodyProperty->GetSubItem(IDX_SP_N_ITER), p.N_ITER_ATO);

	for (int i = 0; i < (int)p.N_ITER_ATO; ++i)
	{
		CMFCPropertyGridProperty* subProperty = cp->GetSubItem(IDX_SPNSP_ITER_Property)->GetSubItem(i);

		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNSP_NID_C), p.SPStatus[i].NID_C);
		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNSP_NID_SP), p.SPStatus[i].NID_SP_1);
		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNSP_Q_SP_Status), p.SPStatus[i].Q_SP_Status);
		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNSP_M_SP_Version), p.SPStatus[i].spStatusValid.M_SP_Version);
		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNSP_L_SP), p.SPStatus[i].spStatusValid.L_SP);
		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNSP_D_EoA_Offset), p.SPStatus[i].spStatusValid.D_EOA_Offset);
		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNSP_Q_UTC_Offset), p.SPStatus[i].spStatusValid.Q_UTC_Offset);
		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNSP_M_SP_Altitude), p.SPStatus[i].spStatusValid.M_SP_Altitude);
		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNSP_Q_ATOTS_Contact_Info_Dir), p.SPStatus[i].spStatusValid.Q_ATOTS_Contact_Info_Dir);
		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNSP_NID_C_TS), p.SPStatus[i].atoTSContactInfo.NID_C_1);
		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNSP_NID_ATOTS), p.SPStatus[i].atoTSContactInfo.NID_ATOTS);
		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNSP_NID_C_SPC), p.SPStatus[i].atoTSContactInfo.NID_C_2);
		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNSP_NID_SP_F), p.SPStatus[i].atoTSContactInfo.NID_SP_2);
		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNSP_V_STATIC), p.SPStatus[i].V_STATIC);
		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNSP_Q_FRONT), p.SPStatus[i].Q_FRONT);
		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNSP_N_ITER_SSP), p.SPStatus[i].N_ITER_ATO_1);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_1; ++k)
		{
			CMFCPropertyGridProperty* sspProperty = subProperty->GetSubItem(IDX_SPNSP_SSP_ITER_Property)->GetSubItem(k);

			Utility::SetValueWithTypeCasting(sspProperty->GetSubItem(IDX_Q_DIFF_SSP), p.SPStatus[i].specificSSP[k].Q_DIFF);
			Utility::SetValueWithTypeCasting(sspProperty->GetSubItem(IDX_NC_CDDIFF_SSP), p.SPStatus[i].specificSSP[k].NC_CDDIFF);
			Utility::SetValueWithTypeCasting(sspProperty->GetSubItem(IDX_NC_DIFF_SSP), p.SPStatus[i].specificSSP[k].NC_DIFF);
			Utility::SetValueWithTypeCasting(sspProperty->GetSubItem(IDX_V_DIFF_SSP), p.SPStatus[i].specificSSP[k].V_DIFF);
		}

		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNSP_N_ITER_SSPC), p.SPStatus[i].N_ITER_ATO_2);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_2; ++k)
		{
			CMFCPropertyGridProperty* sspcProperty = subProperty->GetSubItem(IDX_SPNSP_SSPC_ITER_Property)->GetSubItem(k);

			Utility::SetValueWithTypeCasting(sspcProperty->GetSubItem(IDX_D_Location_SSPC), p.SPStatus[i].staticSPChange[k].D_Location);
			Utility::SetValueWithTypeCasting(sspcProperty->GetSubItem(IDX_V_STATIC_SSPC), p.SPStatus[i].staticSPChange[k].V_STATIC);
			Utility::SetValueWithTypeCasting(sspcProperty->GetSubItem(IDX_Q_FRONT_SSPC), p.SPStatus[i].staticSPChange[k].Q_FRONT);
			Utility::SetValueWithTypeCasting(sspcProperty->GetSubItem(IDX_N_ITER_SSPC_N), p.SPStatus[i].staticSPChange[k].N_ITER);

			for (int l = 0; l < (int)p.SPStatus[i].staticSPChange[k].N_ITER; ++l)
			{
				CMFCPropertyGridProperty* sspcnProperty = sspcProperty->GetSubItem(IDX_SPNSP_SSPC_P_ITER_Property)->GetSubItem(l);

				Utility::SetValueWithTypeCasting(sspcnProperty->GetSubItem(IDX_Q_DIFF_N), p.SPStatus[i].staticSPChange[k].specificSspChange[l].Q_DIFF);
				Utility::SetValueWithTypeCasting(sspcnProperty->GetSubItem(IDX_NC_CDDIFF_N), p.SPStatus[i].staticSPChange[k].specificSspChange[l].NC_CDDIFF);
				Utility::SetValueWithTypeCasting(sspcnProperty->GetSubItem(IDX_NC_DIFF_N), p.SPStatus[i].staticSPChange[k].specificSspChange[l].NC_DIFF);
				Utility::SetValueWithTypeCasting(sspcnProperty->GetSubItem(IDX_V_DIFF_N), p.SPStatus[i].staticSPChange[k].specificSspChange[l].V_DIFF);
			}
		}

		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNSP_G_New_Gradient), p.SPStatus[i].G_New_Gradient);
		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNSP_Q_GDIR), p.SPStatus[i].Q_GDIR);
		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNSP_N_ITER_GC), p.SPStatus[i].N_ITER_ATO_3);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_3; ++k)
		{
			CMFCPropertyGridProperty* gcProperty = subProperty->GetSubItem(IDX_SPNSP_GC_ITER_Property)->GetSubItem(k);

			Utility::SetValueWithTypeCasting(gcProperty->GetSubItem(IDX_D_Location_GC), p.SPStatus[i].gradientChange[k].D_Location);
			Utility::SetValueWithTypeCasting(gcProperty->GetSubItem(IDX_G_New_Gradient_GC), p.SPStatus[i].gradientChange[k].G_New_Gradient);
			Utility::SetValueWithTypeCasting(gcProperty->GetSubItem(IDX_Q_GDIR_GC), p.SPStatus[i].gradientChange[k].Q_GDIR);			
		}

		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNSP_Q_Radius_Category), p.SPStatus[i].Q_Radius_Category);
		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNSP_N_ITER_CC), p.SPStatus[i].N_ITER_ATO_4);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_4; ++k)
		{
			CMFCPropertyGridProperty* ccProperty = subProperty->GetSubItem(IDX_SPNSP_CC_ITER_Property)->GetSubItem(k);

			Utility::SetValueWithTypeCasting(ccProperty->GetSubItem(IDX_D_Location_CC), p.SPStatus[i].curvesChange[k].D_Location);
			Utility::SetValueWithTypeCasting(ccProperty->GetSubItem(IDX_Q_Radius_Category_CC), p.SPStatus[i].curvesChange[k].Q_Radius_Category);
		}

		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNSP_M_VOLTAGE), p.SPStatus[i].M_VOLTAGE);
		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNSP_NID_CTRACTION), p.SPStatus[i].NID_CTRACTION);
		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNSP_N_ITER_PVC), p.SPStatus[i].N_ITER_ATO_5);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_5; ++k)
		{
			CMFCPropertyGridProperty* pvcProperty = subProperty->GetSubItem(IDX_SPNSP_PVC_ITER_Property)->GetSubItem(k);

			Utility::SetValueWithTypeCasting(pvcProperty->GetSubItem(IDX_D_Location_PVC), p.SPStatus[i].powerVoltageChange[k].D_Location);
			Utility::SetValueWithTypeCasting(pvcProperty->GetSubItem(IDX_M_VOLTAGE_PVC), p.SPStatus[i].powerVoltageChange[k].M_VOLTAGE);
			Utility::SetValueWithTypeCasting(pvcProperty->GetSubItem(IDX_NID_CTRACTION_PVC), p.SPStatus[i].powerVoltageChange[k].NID_CTRACTION);
		}

		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNSP_M_CURRENT), p.SPStatus[i].M_CURRENT);
		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNSP_N_ITER_CLC), p.SPStatus[i].N_ITER_ATO_6);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_6; ++k)
		{
			CMFCPropertyGridProperty* clcProperty = subProperty->GetSubItem(IDX_SPNSP_CLC_ITER_Property)->GetSubItem(k);

			Utility::SetValueWithTypeCasting(clcProperty->GetSubItem(IDX_D_Location_CLC), p.SPStatus[i].currentLimitChange[k].D_Location);
			Utility::SetValueWithTypeCasting(clcProperty->GetSubItem(IDX_M_CURRENT_CLC), p.SPStatus[i].currentLimitChange[k].M_CURRENT);
		}

		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNSP_N_ITER_BG), p.SPStatus[i].N_ITER_ATO_7);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_7; ++k)
		{
			CMFCPropertyGridProperty* bgProperty = subProperty->GetSubItem(IDX_SPNSP_BG_ITER_Property)->GetSubItem(k);

			Utility::SetValueWithTypeCasting(bgProperty->GetSubItem(IDX_Q_NEWNID_C), p.SPStatus[i].baliseGroup[k].Q_NEWNID_C);
			Utility::SetValueWithTypeCasting(bgProperty->GetSubItem(IDX_NID_C_BG), p.SPStatus[i].baliseGroup[k].NID_C);
			Utility::SetValueWithTypeCasting(bgProperty->GetSubItem(IDX_NID_BG), p.SPStatus[i].baliseGroup[k].NID_BG);
			Utility::SetValueWithTypeCasting(bgProperty->GetSubItem(IDX_N_ITER_BG), p.SPStatus[i].baliseGroup[k].N_ITER_BG);

			for (int l = 0; l < (int)p.SPStatus[i].baliseGroup[k].N_ITER_BG; ++l)
			{
				CMFCPropertyGridProperty* nbgProperty = bgProperty->GetSubItem(IDX_SPNSP_B_ITER_Property)->GetSubItem(l);

				Utility::SetValueWithTypeCasting(nbgProperty->GetSubItem(IDX_N_PIG), p.SPStatus[i].baliseGroup[k].balises[l].N_PIG);
				Utility::SetValueWithTypeCasting(nbgProperty->GetSubItem(IDX_D_Location_N), p.SPStatus[i].baliseGroup[k].balises[l].D_Location);
			}

		}

		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNSP_N_ITER_TP), p.SPStatus[i].N_ITER_ATO_8);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_8; ++k)
		{
			CMFCPropertyGridProperty* tpProperty = subProperty->GetSubItem(IDX_SPNSP_TP_ITER_Property)->GetSubItem(k);

			Utility::SetValueWithTypeCasting(tpProperty->GetSubItem(IDX_NID_TP), p.SPStatus[i].timingPoints[k].NID_TP);
			Utility::SetValueWithTypeCasting(tpProperty->GetSubItem(IDX_D_Location_TP), p.SPStatus[i].timingPoints[k].D_Location);
			Utility::SetValueWithTypeCasting(tpProperty->GetSubItem(IDX_Q_Stop_Location_Tolerance), p.SPStatus[i].timingPoints[k].Q_Stop_Location_Tolerance);
			Utility::SetValueWithTypeCasting(tpProperty->GetSubItem(IDX_Q_STP_Reached), p.SPStatus[i].timingPoints[k].Q_STP_Reached);
			Utility::SetValueWithTypeCasting(tpProperty->GetSubItem(IDX_L_TEXT), p.SPStatus[i].timingPoints[k].L_TEXT);

			for (int l = 0; l < (int)p.SPStatus[i].timingPoints[k].L_TEXT; ++l)
			{

				CMFCPropertyGridProperty* tpnProperty = tpProperty->GetSubItem(IDX_SPNSP_LTEXT_ITER_Property)->GetSubItem(l);

				Utility::SetValueWithTypeCasting(tpnProperty->GetSubItem(IDX_X_TEXT), p.SPStatus[i].timingPoints[k].X_TEXT[l]);
			}
		}

		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNSP_N_ITER_PA), p.SPStatus[i].N_ITER_ATO_9);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_9; ++k)
		{
			CMFCPropertyGridProperty* paProperty = subProperty->GetSubItem(IDX_SPNSP_PA_ITER_Property)->GetSubItem(k);

			Utility::SetValueWithTypeCasting(paProperty->GetSubItem(IDX_Q_Range_PA), p.SPStatus[i].platformArea[k].Q_Range);
			Utility::SetValueWithTypeCasting(paProperty->GetSubItem(IDX_D_Start_Location_PA), p.SPStatus[i].platformArea[k].D_Start_Location);
			Utility::SetValueWithTypeCasting(paProperty->GetSubItem(IDX_D_End_Location_PA), p.SPStatus[i].platformArea[k].D_End_Location);
		}

		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNSP_N_ITER_T), p.SPStatus[i].N_ITER_ATO_10);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_10; ++k)
		{
			CMFCPropertyGridProperty* tnProperty = subProperty->GetSubItem(IDX_SPNSP_T_ITER_Property)->GetSubItem(k);

			Utility::SetValueWithTypeCasting(tnProperty->GetSubItem(IDX_Q_Range_T), p.SPStatus[i].tunnel[k].Q_Range);
			Utility::SetValueWithTypeCasting(tnProperty->GetSubItem(IDX_Q_Tunnel_Category), p.SPStatus[i].tunnel[k].Q_Tunnel_Category);
			Utility::SetValueWithTypeCasting(tnProperty->GetSubItem(IDX_D_Start_Location_T), p.SPStatus[i].tunnel[k].D_Start_Location);
			Utility::SetValueWithTypeCasting(tnProperty->GetSubItem(IDX_D_End_Location_T), p.SPStatus[i].tunnel[k].D_End_Location);
		}

		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNSP_N_ITER_ALSP), p.SPStatus[i].N_ITER_ATO_11);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_11; ++k)
		{
			CMFCPropertyGridProperty* alspProperty = subProperty->GetSubItem(IDX_SPNSP_ALSP_ITER_Property)->GetSubItem(k);

			Utility::SetValueWithTypeCasting(alspProperty->GetSubItem(IDX_Q_Range_ALSP), p.SPStatus[i].axleLoadSpeedProfile[k].Q_Range);
			Utility::SetValueWithTypeCasting(alspProperty->GetSubItem(IDX_M_AXLELOADCAT), p.SPStatus[i].axleLoadSpeedProfile[k].M_AXLELOADCAT);
			Utility::SetValueWithTypeCasting(alspProperty->GetSubItem(IDX_V_New_Speed_Level), p.SPStatus[i].axleLoadSpeedProfile[k].V_New_Speed_Level);
			Utility::SetValueWithTypeCasting(alspProperty->GetSubItem(IDX_Q_FRONT_ALSP), p.SPStatus[i].axleLoadSpeedProfile[k].Q_FRONT);
		}

		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNSP_N_ITER_ULCS), p.SPStatus[i].N_ITER_ATO_12);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_12; ++k)
		{
			CMFCPropertyGridProperty* alspProperty = subProperty->GetSubItem(IDX_SPNSP_ULCS_ITER_Property)->GetSubItem(k);

			Utility::SetValueWithTypeCasting(alspProperty->GetSubItem(IDX_D_UnprotectedLx_Stop_Nominal), p.SPStatus[i].unprotectedLevelCrossingStop[k].D_UnprotectedLx_Stop_Nominal);
			Utility::SetValueWithTypeCasting(alspProperty->GetSubItem(IDX_D_UnprotectedLx_Stop_Reverse), p.SPStatus[i].unprotectedLevelCrossingStop[k].D_UnprotectedLx_Stop_Reverse);			
		}

		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNSP_N_ITER_PBD), p.SPStatus[i].N_ITER_ATO_13);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_13; ++k)
		{
			CMFCPropertyGridProperty* pbdProperty = subProperty->GetSubItem(IDX_SPNSP_PBD_ITER_Property)->GetSubItem(k);

			Utility::SetValueWithTypeCasting(pbdProperty->GetSubItem(IDX_Q_Range_PBD), p.SPStatus[i].permittedBrakingDistance[k].Q_Range);
			Utility::SetValueWithTypeCasting(pbdProperty->GetSubItem(IDX_D_Permitted_Braking_Distance), p.SPStatus[i].permittedBrakingDistance[k].D_Permitted_Braking_Distance);
			Utility::SetValueWithTypeCasting(pbdProperty->GetSubItem(IDX_Q_PBD_SBEB), p.SPStatus[i].permittedBrakingDistance[k].Q_PBD_SBEB);
			Utility::SetValueWithTypeCasting(pbdProperty->GetSubItem(IDX_G_PBD), p.SPStatus[i].permittedBrakingDistance[k].G_PBD);
			Utility::SetValueWithTypeCasting(pbdProperty->GetSubItem(IDX_Q_GDIR_PBD), p.SPStatus[i].permittedBrakingDistance[k].Q_GDIR_PBD);
			Utility::SetValueWithTypeCasting(pbdProperty->GetSubItem(IDX_D_Start_Location_PBD), p.SPStatus[i].permittedBrakingDistance[k].D_Start_Location);
			Utility::SetValueWithTypeCasting(pbdProperty->GetSubItem(IDX_D_End_Location_PBD), p.SPStatus[i].permittedBrakingDistance[k].D_End_Location);
		}

		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNSP_N_ITER_SORB), p.SPStatus[i].N_ITER_ATO_14);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_14; ++k)
		{
			CMFCPropertyGridProperty* sorbProperty = subProperty->GetSubItem(IDX_SPNSP_SORB_ITER_Property)->GetSubItem(k);

			Utility::SetValueWithTypeCasting(sorbProperty->GetSubItem(IDX_Q_Range_SORB), p.SPStatus[i].switchOffRegenerativeBrake[k].Q_Range);
			Utility::SetValueWithTypeCasting(sorbProperty->GetSubItem(IDX_D_Start_Location_SORB), p.SPStatus[i].switchOffRegenerativeBrake[k].D_Start_Location);
			Utility::SetValueWithTypeCasting(sorbProperty->GetSubItem(IDX_D_End_Location_SORB), p.SPStatus[i].switchOffRegenerativeBrake[k].D_End_Location);
		}

		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNSP_N_ITER_SOECB), p.SPStatus[i].N_ITER_ATO_15);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_15; ++k)
		{
			CMFCPropertyGridProperty* soecbProperty = subProperty->GetSubItem(IDX_SPNSP_SOECB_ITER_Property)->GetSubItem(k);

			Utility::SetValueWithTypeCasting(soecbProperty->GetSubItem(IDX_Q_Range_SOECB), p.SPStatus[i].switchOffEddyCurrentBrake[k].Q_Range);
			Utility::SetValueWithTypeCasting(soecbProperty->GetSubItem(IDX_D_Start_Location_SOECB), p.SPStatus[i].switchOffEddyCurrentBrake[k].D_Start_Location);
			Utility::SetValueWithTypeCasting(soecbProperty->GetSubItem(IDX_D_End_Location_SOECB), p.SPStatus[i].switchOffEddyCurrentBrake[k].D_End_Location);
		}

		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNSP_N_ITER_SOECEB), p.SPStatus[i].N_ITER_ATO_16);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_16; ++k)
		{
			CMFCPropertyGridProperty* soecebProperty = subProperty->GetSubItem(IDX_SPNSP_SOECEB_ITER_Property)->GetSubItem(k);

			Utility::SetValueWithTypeCasting(soecebProperty->GetSubItem(IDX_Q_Range_SOECEB), p.SPStatus[i].switchOffEddyEmergencyBrake[k].Q_Range);
			Utility::SetValueWithTypeCasting(soecebProperty->GetSubItem(IDX_D_Start_Location_SOECEB), p.SPStatus[i].switchOffEddyEmergencyBrake[k].D_Start_Location);
			Utility::SetValueWithTypeCasting(soecebProperty->GetSubItem(IDX_D_End_Location_SOECEB), p.SPStatus[i].switchOffEddyEmergencyBrake[k].D_End_Location);
		}

		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNSP_N_ITER_SOMSB), p.SPStatus[i].N_ITER_ATO_17);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_17; ++k)
		{
			CMFCPropertyGridProperty* somsbProperty = subProperty->GetSubItem(IDX_SPNSP_SOMSB_ITER_Property)->GetSubItem(k);

			Utility::SetValueWithTypeCasting(somsbProperty->GetSubItem(IDX_Q_Range_SOMSB), p.SPStatus[i].switchOffMagmaticShoeBrake[k].Q_Range);
			Utility::SetValueWithTypeCasting(somsbProperty->GetSubItem(IDX_D_Start_Location_SOMSB), p.SPStatus[i].switchOffMagmaticShoeBrake[k].D_Start_Location);
			Utility::SetValueWithTypeCasting(somsbProperty->GetSubItem(IDX_D_End_Location_SOMSB), p.SPStatus[i].switchOffMagmaticShoeBrake[k].D_End_Location);
		}

		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNSP_N_ITER_ADBFI), p.SPStatus[i].N_ITER_ATO_18);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_18; ++k)
		{
			CMFCPropertyGridProperty* adbfiProperty = subProperty->GetSubItem(IDX_SPNSP_ADBFI_ITER_Property)->GetSubItem(k);

			Utility::SetValueWithTypeCasting(adbfiProperty->GetSubItem(IDX_Q_Range_ADBFI), p.SPStatus[i].areaWithDynamicBrakeForceInhibition[k].Q_Range);
			Utility::SetValueWithTypeCasting(adbfiProperty->GetSubItem(IDX_D_Start_Location_ADBFI), p.SPStatus[i].areaWithDynamicBrakeForceInhibition[k].D_Start_Location);
			Utility::SetValueWithTypeCasting(adbfiProperty->GetSubItem(IDX_D_End_Location_ADBFI), p.SPStatus[i].areaWithDynamicBrakeForceInhibition[k].D_End_Location);
		}

		Utility::SetValueWithTypeCasting(subProperty->GetSubItem(IDX_SPNSP_N_ITER_ALDBF), p.SPStatus[i].N_ITER_ATO_19);

		for (int k = 0; k < (int)p.SPStatus[i].N_ITER_ATO_19; ++k)
		{
			CMFCPropertyGridProperty* aldbfProperty = subProperty->GetSubItem(IDX_SPNSP_ALDBF_ITER_Property)->GetSubItem(k);

			Utility::SetValueWithTypeCasting(aldbfProperty->GetSubItem(IDX_Q_Range_ALDBF), p.SPStatus[i].areaWithLimitedDynamicBrakeForce[k].Q_Range);
			Utility::SetValueWithTypeCasting(aldbfProperty->GetSubItem(IDX_M_Dynamic_Brake_Force_Limit), p.SPStatus[i].areaWithLimitedDynamicBrakeForce[k].M_Dynamic_Braking_Force_Limit);
			Utility::SetValueWithTypeCasting(aldbfProperty->GetSubItem(IDX_D_Start_Location_ALDBF), p.SPStatus[i].areaWithLimitedDynamicBrakeForce[k].D_Start_Location);
			Utility::SetValueWithTypeCasting(aldbfProperty->GetSubItem(IDX_D_End_Location_ALDBF), p.SPStatus[i].areaWithLimitedDynamicBrakeForce[k].D_End_Location);

		}
	}
}

void PropertyCtrlBuilder::PacketSTRAckStructureToCtrl(const TS_OB_Packet09_STRAckStruct& p, CMFCPropertyGridProperty* bodyProperty)
{
	CMFCPropertyGridProperty* cp = bodyProperty;

	Utility::SetValueWithTypeCasting(bodyProperty->GetSubItem(IDX_T_STR_Reference_Timestamp_Date), p.T_STR_Ref_TimeStamp_Date);
	Utility::SetValueWithTypeCasting(bodyProperty->GetSubItem(IDX_T_STR_Reference_Timestamp_Seconds), p.T_STR_Ref_TimeStamp_Seconds);
	Utility::SetValueWithTypeCasting(bodyProperty->GetSubItem(IDX_N_STR_Reference_Packet_Counter), p.N_STR_Ref_Packet_Counter);
	
}

void PropertyCtrlBuilder::PacketSessTermReqStructureToCtrl(const TS_OB_Packet10_SESSTermReqStruct& p, CMFCPropertyGridProperty* bodyProperty)
{
}


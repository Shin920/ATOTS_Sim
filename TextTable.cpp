#include "pch.h"
#include "TextTable.h"

#include "PropertyCtrlBuilder.h"

using namespace PacketHSAckEnum;
using namespace PacketHSRejEnum;
using namespace PacketJPEnum;
using namespace PacketSPEnum;
using namespace PacketSTRAckEnum;

template <typename T>
void TextTable<T>::AddText(int val, const CHAR* txt)
{
	txtList.insert(make_pair(val, txt));
}

TextTablePacketHSAck::TextTablePacketHSAck()
{
	Init();
}

void TextTablePacketHSAck::Init()
{
	AddText(IDX_HSA_NID_C , "NID_C");
	AddText(IDX_HSA_NID_ATOTS, "NID_ATOTS");
	AddText(IDX_HSA_M_ATO_Version, "M_ATO_Version");
	AddText(IDX_HSA_T_Timeout_ATOTS_Response, "T_Timeout_ATOTS_Response");
	AddText(IDX_HSA_T_Reporting_Time, "T_Reporting_Time");
}

TextTablePacketHSRej::TextTablePacketHSRej()
{
	Init();
}

void TextTablePacketHSRej::Init()
{
	AddText(IDX_Q_Reject_Reason, "Q_Reject_Reason");
	AddText(IDX_HSRJ_NID_C, "NID_C");
	AddText(IDX_HSRJ_NID_ATOTS, "NID_ATOTS");
	
}

TextTablePacketJP::TextTablePacketJP()
{
	Init();
}

void TextTablePacketJP::Init()
{
	AddText(IDX_Q_JP_Status, "Q_JP_Status");
	AddText(IDX_N_ITER_SP, "N_ITER_SP");
	AddText(IDX_SPNJP_NID_C, "NID_C");
}

TextTablePacketSP::TextTablePacketSP()
{
	Init();
}

void TextTablePacketSP::Init()
{
	AddText(IDX_SP_N_ITER, "N_ITER");
}

TextTablePacketSTRAck::TextTablePacketSTRAck()
{
	Init();
}

void TextTablePacketSTRAck::Init()
{
	AddText(IDX_T_STR_Reference_Timestamp_Date, "T_STR_Reference_Timestamp_Date");
	AddText(IDX_T_STR_Reference_Timestamp_Seconds, "T_STR_Reference_Timestamp_Seconds");
	AddText(IDX_N_STR_Reference_Packet_Counter, "N_STR_Reference_Packet_Counter");
}


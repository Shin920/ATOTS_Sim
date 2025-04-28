#pragma once
#include "F110_Radio_Packet.h"
#include "126 Item Enums.h"

class PropertyCtrlBuilder
{
public:

	static void BuildBodyPkHSReq(CMFCPropertyGridProperty* pxBodyProperty);
	static void BuildBodyPkJPReq(CMFCPropertyGridProperty* pxBodyProperty);
	static void BuildBodyPkJPAck(CMFCPropertyGridProperty* pxBodyProperty);
	static void BuildBodyPkSPReq(CMFCPropertyGridProperty* pxBodyProperty);
	static void BuildBodyPkSTR(CMFCPropertyGridProperty* pxBodyProperty);
	static void BuildBodyPkSessTerm(CMFCPropertyGridProperty* pxBodyProperty);

	static void PacketHSReqStructureToCtrl(const OB_TS_Packet00_HSReqStruct& p, CMFCPropertyGridProperty* bodyProperty);
	static void PacketJPReqStructureToCtrl(const OB_TS_Packet03_JPReqStruct& p, CMFCPropertyGridProperty* bodyProperty);
	static void PacketJPAckStructureToCtrl(const OB_TS_Packet05_JPAckStruct& p, CMFCPropertyGridProperty* bodyProperty);
	static void PacketSPReqStructureToCtrl(const OB_TS_Packet06_SPReqStruct& p, CMFCPropertyGridProperty* bodyProperty);
	static void PacketSTRStructureToCtrl(const OB_TS_Packet08_STRStruct& p, CMFCPropertyGridProperty* bodyProperty);
	static void PacketSessTermStructureToCtrl(const OB_TS_Packet11_SESSTermStruct& p, CMFCPropertyGridProperty* bodyProperty);


	static void BuildBodyPkHSAck(CMFCPropertyGridProperty* pxBodyProperty);
	static void BuildBodyPkHSRej(CMFCPropertyGridProperty* pxBodyProperty);
	static void BuildBodyPkJP(CMFCPropertyGridProperty* pxBodyProperty);
	static void BuildBodyPkSP(CMFCPropertyGridProperty* pxBodyProperty);
	static void BuildBodyPkSTRAck(CMFCPropertyGridProperty* pxBodyProperty);
	static void BuildBodyPkSessTermReq(CMFCPropertyGridProperty* pxBodyProperty);


	static void PacketHeaderStructureToCtrl(const ATO_Header& p, CMFCPropertyGridProperty* headerProperty);
	static void PacketHSAckStructureToCtrl(const TS_OB_Packet01_HSAckStruct& p, CMFCPropertyGridProperty* bodyProperty);
	static void PacketHSRejStructureToCtrl(const TS_OB_Packet02_HSRejStruct& p, CMFCPropertyGridProperty* bodyProperty);
	static void PacketJPStructureToCtrl(const TS_OB_Packet04_JPStruct& p, CMFCPropertyGridProperty* bodyProperty);
	static void PacketSPStructureToCtrl(const TS_OB_Packet07_SPStruct& p, CMFCPropertyGridProperty* bodyProperty);
	static void PacketSTRAckStructureToCtrl(const TS_OB_Packet09_STRAckStruct& p, CMFCPropertyGridProperty* bodyProperty);
	static void PacketSessTermReqStructureToCtrl(const TS_OB_Packet10_SESSTermReqStruct& p, CMFCPropertyGridProperty* bodyProperty);

	
	
};
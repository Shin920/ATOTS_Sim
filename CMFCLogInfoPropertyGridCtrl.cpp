//#include "CMFCLogInfoPropertyGridCtrl.h"
//#include "GridPropertyOverride.h"
//#include "126 Item Enums.h"
//#include "PropertyCtrlBuilder.h"
//
//using namespace PacketHeaderEnum;
//
//CMFCLogInfoPropertyGridCtrl::CMFCLogInfoPropertyGridCtrl(CMFCPropertyGridCtrl* ctrl)
//{
//	logInfoProperty = ctrl;
//	currentPacket = -1;
//	InitCtrl();
//}
//
//CMFCLogInfoPropertyGridCtrl::~CMFCLogInfoPropertyGridCtrl()
//{
//}
//
//void CMFCLogInfoPropertyGridCtrl::InitProperty()
//{
//	currentPacket = -1;
//	InitHeaderProperty(logInfoProperty->GetProperty((int)LogProperty::HEADER));
//	RemovePropertyContents(LogProperty::BODY);	
//	logInfoProperty->AdjustLayout();
//}
//
//void CMFCLogInfoPropertyGridCtrl::ShowPacket(const byte* arr, const size_t length)
//{
//	if (length < LEN_ALL_HEADER) return;
//	static ATO_Header header{};
//	
//	UINT32 offset = 0; //bit 단위
//
//	//배열에서 헤더정보 확인
//	
//}
//
//BOOL CMFCLogInfoPropertyGridCtrl::InitCtrl()
//{
//	if (logInfoProperty == nullptr) return FALSE;
//
//	CRect rect;
//
//	logInfoProperty->EnableHeaderCtrl(TRUE, _T("Item"), _T("Value"));
//	logInfoProperty->GetWindowRect(&rect);
//	logInfoProperty->PostMessage(WM_SIZE, 0, MAKELONG(rect.Width() - 50, rect.Height()));
//
//	CMFCPropertyGridProperty* pxpgp;
//	CMFCPropertyEX<UINT32>* pxu;
//
//	CMFCPropertyGridProperty* pxProperty;
//	pxProperty = new CMFCPropertyGridProperty(_T("HEADER Data"));
//	logInfoProperty->AddProperty(pxProperty);
//
//	pxpgp = new CMFCPropertyGridProperty(_T("NID_PACKET_ATO"), (variant_t)(int)0, _T("패킷 넘버"));
//	pxpgp->Enable(FALSE);
//	pxProperty->AddSubItem(pxpgp);
//
//	pxu = new CMFCPropertyEX<UINT32>(_T("NID_OPERATIONAL"), (variant_t)(UINT32)0, _T("Train Running Number"));
//	pxu->Enable(FALSE);
//	pxProperty->AddSubItem(pxu);
//
//	pxpgp = new CMFCPropertyGridProperty(_T("NID_ENGINE"), (variant_t)(int)0, _T("ETCS Identify"));
//	pxpgp->Enable(FALSE);
//	pxProperty->AddSubItem(pxpgp);
//
//	pxpgp = new CMFCPropertyGridProperty(_T("T_TimeStamp_Date"), (variant_t)(int)0, _T("Timestamp in date since start-up"));
//	pxpgp->Enable(FALSE);
//	pxProperty->AddSubItem(pxpgp);
//
//	pxpgp = new CMFCPropertyGridProperty(_T("T_TimeStamp_Seconds"), (variant_t)(int)0, _T("Timestamp in milliseconds since start-up"));
//	pxpgp->Enable(FALSE);
//	pxProperty->AddSubItem(pxpgp);
//
//	pxpgp = new CMFCPropertyGridProperty(_T("N_Packet_Counter"), (variant_t)(int)0, _T("보내진 패킷 Counter"));
//	pxpgp->Enable(FALSE);
//	pxProperty->AddSubItem(pxpgp);
//
//
//	pxProperty = new CMFCPropertyGridProperty(_T("BODY Data"));
//	logInfoProperty->AddProperty(pxProperty);
//}
//
//void CMFCLogInfoPropertyGridCtrl::SetHeaderProperty(const ATO_Header& header, CMFCPropertyGridProperty* headerProperty)
//{
//	if (headerProperty == nullptr) return;
//	headerProperty->GetSubItem((int)PacketHeaderIDX::IDX_NID_PACKET_ATO)->SetValue((variant_t)(UINT8)header.NID_PACKET_ATO);
//	headerProperty->GetSubItem((int)PacketHeaderIDX::IDX_NID_OPERATIONAL)->SetValue((variant_t)(UINT32)header.NID_OPERATIONAL);
//	headerProperty->GetSubItem((int)PacketHeaderIDX::IDX_NID_ENGINE)->SetValue((variant_t)(UINT32)header.NID_ENGINE);
//	headerProperty->GetSubItem((int)PacketHeaderIDX::IDX_T_Timestamp_Date)->SetValue((variant_t)(UINT16)header.T_Timestamp_Date);
//	headerProperty->GetSubItem((int)PacketHeaderIDX::IDX_T_Timestamp_Seconds)->SetValue((variant_t)(UINT16)header.T_Timestamp_Seconds);
//	headerProperty->GetSubItem((int)PacketHeaderIDX::IDX_N_Packet_Counter)->SetValue((variant_t)(UINT16)header.N_Packet_Counter);
//
//
//}
//
//void CMFCLogInfoPropertyGridCtrl::InitHeaderProperty(CMFCPropertyGridProperty* headerProperty)
//{
//	if (headerProperty == nullptr) return;
//	headerProperty->GetSubItem((int)PacketHeaderIDX::IDX_NID_PACKET_ATO)->SetValue((variant_t)(int)0);
//	headerProperty->GetSubItem((int)PacketHeaderIDX::IDX_NID_OPERATIONAL)->SetValue((variant_t)(UINT32)0);
//	headerProperty->GetSubItem((int)PacketHeaderIDX::IDX_NID_ENGINE)->SetValue((variant_t)(UINT32)0);
//	headerProperty->GetSubItem((int)PacketHeaderIDX::IDX_T_Timestamp_Date)->SetValue((variant_t)(UINT16)0);
//	headerProperty->GetSubItem((int)PacketHeaderIDX::IDX_T_Timestamp_Seconds)->SetValue((variant_t)(UINT16)0);
//	headerProperty->GetSubItem((int)PacketHeaderIDX::IDX_N_Packet_Counter)->SetValue((variant_t)(UINT16)0);
//}
//
//void CMFCLogInfoPropertyGridCtrl::RemovePropertyContents(const LogProperty target)
//{
//	CMFCPropertyGridProperty* pxTarget = logInfoProperty->GetProperty((int)target);
//	int subItemCount = pxTarget->GetSubItemsCount();
//	while (subItemCount)
//	{
//		CMFCPropertyGridProperty* targetToBeRemoved = pxTarget->GetSubItem(--subItemCount); //삭제할 목표 설정
//		pxTarget->RemoveSubItem(targetToBeRemoved);
//	}
//}
//
//void CMFCLogInfoPropertyGridCtrl::AddBody(const RecvPacketTypes pkNum)
//{
//	CMFCPropertyGridProperty* pxBodyProperty = logInfoProperty->GetProperty((int)LogProperty::BODY);
//	switch (pkNum)
//	{
//	case RecvPacketTypes::eHsReq: PropertyCtrlBuilder::BuildBodyPkHSReq(pxBodyProperty); break;
//	case RecvPacketTypes::eJPReq: PropertyCtrlBuilder::BuildBodyPkJPReq(pxBodyProperty); break;
//	case RecvPacketTypes::eJPAck: PropertyCtrlBuilder::BuildBodyPkJPAck(pxBodyProperty); break;
//	case RecvPacketTypes::eSPReq: PropertyCtrlBuilder::BuildBodyPkSPReq(pxBodyProperty); break;
//	case RecvPacketTypes::eSTR: PropertyCtrlBuilder::BuildBodyPkSTR(pxBodyProperty); break;
//	default: break;
//	}
//}
//
//void CMFCLogInfoPropertyGridCtrl::ChangeBody(const RecvPacketTypes pkNum)
//{
//	if (currentPacket != pkNum)
//	{
//		if (currentPacket != RecvPacketTypes::PacketNotSelected)RemovePropertyContents(LogProperty::BODY);
//		AddBody(pkNum);
//		logInfoProperty->AdjustLayout();
//		currentPacket = pkNum;
//	}
//
//}
//
//void CMFCLogInfoPropertyGridCtrl::EnableBodyRecursive(BOOL b)
//{
//	CMFCPropertyGridProperty* pxBodyProperty = logInfoProperty->GetProperty((int)LogProperty::BODY);
//	queue<CMFCPropertyGridProperty*>targetProperties;
//	targetProperties.push(pxBodyProperty);
//	while (!targetProperties.empty()) //BFS로 전체 활성화/비활성화
//	{
//		CMFCPropertyGridProperty*& currentProperty = targetProperties.front(); //맨 앞의 원소 참조
//		currentProperty->Enable(b);
//		for (int i = 0; i < currentProperty->GetSubItemsCount(); i++)
//		{
//			targetProperties.push(currentProperty->GetSubItem(i)); //subitem을 큐에 삽입
//		}
//		targetProperties.pop(); //맨 앞의 원소 제거
//	}
//}

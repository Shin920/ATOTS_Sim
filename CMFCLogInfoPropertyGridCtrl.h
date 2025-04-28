#pragma once
#include "pch.h"


//enum class LogProperty : int
//{
//	HEADER,
//	BODY	
//};
//
//class CMFCLogInfoPropertyGridCtrl
//{
//public:
//	CMFCLogInfoPropertyGridCtrl(CMFCPropertyGridCtrl* ctrl = nullptr);
//	~CMFCLogInfoPropertyGridCtrl();
//
//
//	void InitProperty();
//	void ShowPacket(const byte* arr, const size_t length);
//
//private:
//	BOOL InitCtrl();
//
//	CMFCPropertyGridCtrl* logInfoProperty;
//	int currentPacket;
//
//	void SetHeaderProperty(const ATO_Header& header, CMFCPropertyGridProperty* headerProperty);
//	
//	void InitHeaderProperty(CMFCPropertyGridProperty* headerProperty);
//	
//	void RemovePropertyContents(const LogProperty target);
//	void AddBody(const RecvPacketTypes pkNum);
//	void ChangeBody(const RecvPacketTypes pkNum);
//	void EnableBodyRecursive(BOOL b);
//};
//

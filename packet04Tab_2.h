﻿#pragma once
#include "afxdialogex.h"
#include "GridPropertyOverride.h"
#include "F110_Radio_Packet.h"
#include "126 Item Enums.h"



// CSendTab 대화 상자

class packet04Tab_2 : public CDialogEx
{
	DECLARE_DYNAMIC(packet04Tab_2)

public:
	packet04Tab_2(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~packet04Tab_2();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SEND };
#endif

protected:
	void Init();
	void Release();


protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.



	//inline CMFCPropertyGridCtrl* GetPropertyGrid() {return }


	DECLARE_MESSAGE_MAP()
public:
	int m_nPacketCounter;

	//Send용 Packet 구조체 포인터	
	ATO_Header* pHeaderStructure;
	TS_OB_Packet04_JPStruct* pJPStructure;


	//File용 Packet 구조체 포인터
	TS_OB_Packet04_JPStruct* pFileJPStructure;
	TS_OB_Packet04_JPStruct* pRDJPStructure;

	void PacketHeaderCtrlToStructure(const CMFCPropertyGridProperty* bodyProperty, ATO_Header& p);
	void PacketJPCtrlToStructure(const CMFCPropertyGridProperty* bodyProperty, TS_OB_Packet04_JPStruct& p);
	void PacketJPCtrlToStructureDynamic(const CMFCPropertyGridProperty* bodyProperty, TS_OB_Packet04_JPStruct& p);


	//INI파일 관련 함수

	static void PacketJPStructrureToINI(TS_OB_Packet04_JPStruct& p);



	BOOL LoadPacketJPFromFile();

	BOOL initLoadPacketJPFromFile();

	void LoadPacketJPFromFile_StaticData(string& configTXT, TS_OB_Packet04_JPStruct& p);



	void ReLoadPacketJP();



	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void propgrid();
	CMFCPropertyGridCtrl m_propgrid;
	CButton m_chb_auto;
	afx_msg void OnBnClickedChbAuto();
	CComboBox m_cmb_send;
	afx_msg void OnCbnSelchangeCmbPacket();


	afx_msg void OnBnClickedBtnSend();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnLoad();

	int getPacketCounter();
	afx_msg void OnBnClickedButton1();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtnSendDynamic();
};

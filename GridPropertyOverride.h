#pragma once
#include <afxpropertygridctrl.h>
#include "126 Item Enums.h"

class CMFCPropertyStr : public CMFCPropertyGridProperty
{
public:
	CMFCPropertyStr(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);
	CMFCPropertyStr(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	virtual ~CMFCPropertyStr();

	virtual BOOL OnEndEdit() override;
	virtual BOOL OnUpdateValue() override;

	int maxLength;
protected:
	void Init();
};

class CMFCPropertySwitch : public CMFCPropertyGridProperty
{
public:
	CMFCPropertySwitch(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);
	CMFCPropertySwitch(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	virtual ~CMFCPropertySwitch();

	virtual void OnSelectCombo() override;
	virtual BOOL OnDblClk(CPoint point) override;

	void Init();
	void Release();
	void AddTarget(CMFCPropertyGridProperty* t);

protected:
	list<CMFCPropertyGridProperty*>* targets;
	void SetTargetEnableState(BOOL b);
};

//=============================================================================

class CMFCPropertyHeader : public CMFCPropertyGridProperty
{
public:
	enum IDXHeader
	{
		IDX_NID_PACKET_ATO,
		IDX_NID_OPERATIONAL,
		IDX_T_Timestamp_Date,
		IDX_T_Timestamp_Seconds,
		IDX_N_Packet_Counter
	};

public:
	CMFCPropertyHeader(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);
	virtual ~CMFCPropertyHeader();

	void SetPacketNumber(int n);

protected:
	void Init();
};


template<typename T>
class CMFCPropertyEX : public CMFCPropertyGridProperty
{
public:
	enum SetMinMaxFlag
	{
		SMMF_SetMax = 1,
		SMMF_SetMin = 2
	};
public:
	CMFCPropertyEX(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);
	CMFCPropertyEX(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	virtual ~CMFCPropertyEX();

	virtual BOOL OnEndEdit() override;
	virtual CString FormatProperty() override;
	virtual void OnSelectCombo() override;
	virtual BOOL OnUpdateValue() override;

	void SetFormat(const TCHAR* format);

	//특정 입력값에 대한 표시 텍스트 설정
	void AddSpecialValue(pair<T, const TCHAR*> p);

	//콤보박스 선택 인덱스를 특정 값으로 변환
	void AddComboBoxlValue(pair<int, T> p);

	//최대 최소값 설정
	void SetMinMaxValue(T minv, T maxv, int flag = SMMF_SetMax | SMMF_SetMin);


	//옵션 이름, 표시할 텍스트 (없는 경우 옵션이름과 동일), 실제 들어갈 값(없는 경우 옵션 인덱스 값과 동일)
	void AddSpecialOption(const TCHAR* optionTXT, const TCHAR* formatTXT, T actualValue);
	void AddSpecialOption(const TCHAR* optionTXT, const TCHAR* formatTXT);
	void AddSpecialOption(const TCHAR* optionTXT, T actualValue);
	void AddSpecialOption(const TCHAR* optionTXT);

protected:
	const TCHAR* m_format;
	void Init();
	void GetValueFromVariantT(T& t);
	T maxVal;
	T minVal;
	unordered_map<T, const TCHAR*> specialValueList; //현재 값이 이 리스트에 있는 값인 경우 지정한 포맷으로 변경
	unordered_map<int, T> comboBoxValueList; //콤보리스트를 선택 시 특정 값으로 설정
	int comboBoxOptionAmount;
};

//이 프로퍼티의 값이 바뀔 때마다 Iterator 정보를 담고있는 프로퍼티를 추가/삭제하기 위한 클래스
class CMFCPropertyIterator : public CMFCPropertyEX<int>
{
public:
	CMFCPropertyIterator(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);
	CMFCPropertyIterator(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	virtual ~CMFCPropertyIterator();

	virtual BOOL OnEndEdit();

	void AdjustIteratorProperty();
	inline void SetTargetProperty(CMFCPropertyGridProperty* t) { targetProperty = t; }

protected:
	virtual void AddIteratorProperty(int nextIdx = 0) = 0;
	virtual void RemoveIteratorProperty(int targetIdx);

	CMFCPropertyGridProperty* targetProperty; //Iterator 프로퍼티를 담고있는 부모 프로퍼티
};

class CMFCPropertyIteratorJP : public CMFCPropertyIterator		//JP ITER 
{
public:
	CMFCPropertyIteratorJP(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);
	CMFCPropertyIteratorJP(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	virtual ~CMFCPropertyIteratorJP();

protected:
	virtual void AddIteratorProperty(int nextIdx = 0);
};

class CMFCPropertyIteratorJP_TP : public CMFCPropertyIterator		//JP_TP ITER
{
public:
	CMFCPropertyIteratorJP_TP(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);
	CMFCPropertyIteratorJP_TP(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	virtual ~CMFCPropertyIteratorJP_TP();

protected:
	virtual void AddIteratorProperty(int nextIdx = 0);
};

class CMFCPropertyIteratorJP_TC : public CMFCPropertyIterator		//JP_TC ITER
{
public:
	CMFCPropertyIteratorJP_TC(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);
	CMFCPropertyIteratorJP_TC(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	virtual ~CMFCPropertyIteratorJP_TC();

protected:
	virtual void AddIteratorProperty(int nextIdx = 0);
};

class CMFCPropertyIteratorSP : public CMFCPropertyIterator			//SP ITER
{
public:
	CMFCPropertyIteratorSP(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);
	CMFCPropertyIteratorSP(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	virtual ~CMFCPropertyIteratorSP();

protected:
	virtual void AddIteratorProperty(int nextIdx = 0);
};

class CMFCPropertyIteratorSP_SSP : public CMFCPropertyIterator		//SP_SSP ITER
{
public:
	CMFCPropertyIteratorSP_SSP(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);
	CMFCPropertyIteratorSP_SSP(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	virtual ~CMFCPropertyIteratorSP_SSP();

protected:
	virtual void AddIteratorProperty(int nextIdx = 0);
};

class CMFCPropertyIteratorSP_SSPCHG : public CMFCPropertyIterator		//SP_SSPCHG ITER
{
public:
	CMFCPropertyIteratorSP_SSPCHG(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);
	CMFCPropertyIteratorSP_SSPCHG(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	virtual ~CMFCPropertyIteratorSP_SSPCHG();

protected:
	virtual void AddIteratorProperty(int nextIdx = 0);
};

class CMFCPropertyIteratorSP_SSPCHG_SSSP : public CMFCPropertyIterator		//SP_SSPCHG_SSSP ITER
{
public:
	CMFCPropertyIteratorSP_SSPCHG_SSSP(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);
	CMFCPropertyIteratorSP_SSPCHG_SSSP(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	virtual ~CMFCPropertyIteratorSP_SSPCHG_SSSP();

protected:
	virtual void AddIteratorProperty(int nextIdx = 0);
};

class CMFCPropertyIteratorSP_gradChg : public CMFCPropertyIterator		//SP_gradChg ITER
{
public:
	CMFCPropertyIteratorSP_gradChg(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);
	CMFCPropertyIteratorSP_gradChg(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	virtual ~CMFCPropertyIteratorSP_gradChg();

protected:
	virtual void AddIteratorProperty(int nextIdx = 0);
};


class CMFCPropertyIteratorSP_curvChg : public CMFCPropertyIterator		//SP_curvChg ITER
{
public:
	CMFCPropertyIteratorSP_curvChg(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);
	CMFCPropertyIteratorSP_curvChg(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	virtual ~CMFCPropertyIteratorSP_curvChg();

protected:
	virtual void AddIteratorProperty(int nextIdx = 0);
};

class CMFCPropertyIteratorSP_pvChg : public CMFCPropertyIterator		//SP_pvChg ITER
{
public:
	CMFCPropertyIteratorSP_pvChg(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);
	CMFCPropertyIteratorSP_pvChg(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	virtual ~CMFCPropertyIteratorSP_pvChg();

protected:
	virtual void AddIteratorProperty(int nextIdx = 0);
};

class CMFCPropertyIteratorSP_clChg : public CMFCPropertyIterator		//SP_clChg ITER
{
public:
	CMFCPropertyIteratorSP_clChg(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);
	CMFCPropertyIteratorSP_clChg(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	virtual ~CMFCPropertyIteratorSP_clChg();

protected:
	virtual void AddIteratorProperty(int nextIdx = 0);
};

class CMFCPropertyIteratorSP_BG : public CMFCPropertyIterator		//SP_BG ITER
{
public:
	CMFCPropertyIteratorSP_BG(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);
	CMFCPropertyIteratorSP_BG(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	virtual ~CMFCPropertyIteratorSP_BG();

protected:
	virtual void AddIteratorProperty(int nextIdx = 0);
};

class CMFCPropertyIteratorSP_BG_BAL : public CMFCPropertyIterator		//SP_BG_BAL ITER
{
public:
	CMFCPropertyIteratorSP_BG_BAL(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);
	CMFCPropertyIteratorSP_BG_BAL(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	virtual ~CMFCPropertyIteratorSP_BG_BAL();

protected:
	virtual void AddIteratorProperty(int nextIdx = 0);
};

class CMFCPropertyIteratorSP_TP : public CMFCPropertyIterator		//SP_TP ITER
{
public:
	CMFCPropertyIteratorSP_TP(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);
	CMFCPropertyIteratorSP_TP(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	virtual ~CMFCPropertyIteratorSP_TP();

protected:
	virtual void AddIteratorProperty(int nextIdx = 0);
};

class CMFCPropertyIteratorSP_TP_NM : public CMFCPropertyIterator		//SP_TP_NM ITER
{
public:
	CMFCPropertyIteratorSP_TP_NM(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);
	CMFCPropertyIteratorSP_TP_NM(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	virtual ~CMFCPropertyIteratorSP_TP_NM();

protected:
	virtual void AddIteratorProperty(int nextIdx = 0);
};

class CMFCPropertyIteratorSP_PA : public CMFCPropertyIterator		//SP_PA ITER
{
public:
	CMFCPropertyIteratorSP_PA(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);
	CMFCPropertyIteratorSP_PA(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	virtual ~CMFCPropertyIteratorSP_PA();

protected:
	virtual void AddIteratorProperty(int nextIdx = 0);
};

class CMFCPropertyIteratorSP_TN : public CMFCPropertyIterator		//SP_TN ITER
{
public:
	CMFCPropertyIteratorSP_TN(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);
	CMFCPropertyIteratorSP_TN(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	virtual ~CMFCPropertyIteratorSP_TN();

protected:
	virtual void AddIteratorProperty(int nextIdx = 0);
};

class CMFCPropertyIteratorSP_ALSP : public CMFCPropertyIterator		//SP_ALSP ITER
{
public:
	CMFCPropertyIteratorSP_ALSP(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);
	CMFCPropertyIteratorSP_ALSP(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	virtual ~CMFCPropertyIteratorSP_ALSP();

protected:
	virtual void AddIteratorProperty(int nextIdx = 0);
};

class CMFCPropertyIteratorSP_ULCS : public CMFCPropertyIterator		//SP_ULCS ITER
{
public:
	CMFCPropertyIteratorSP_ULCS(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);
	CMFCPropertyIteratorSP_ULCS(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	virtual ~CMFCPropertyIteratorSP_ULCS();

protected:
	virtual void AddIteratorProperty(int nextIdx = 0);
};

class CMFCPropertyIteratorSP_PBD : public CMFCPropertyIterator		//SP_PBD ITER
{
public:
	CMFCPropertyIteratorSP_PBD(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);
	CMFCPropertyIteratorSP_PBD(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	virtual ~CMFCPropertyIteratorSP_PBD();

protected:
	virtual void AddIteratorProperty(int nextIdx = 0);
};

class CMFCPropertyIteratorSP_SORB : public CMFCPropertyIterator		//SP_SORB ITER
{
public:
	CMFCPropertyIteratorSP_SORB(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);
	CMFCPropertyIteratorSP_SORB(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	virtual ~CMFCPropertyIteratorSP_SORB();

protected:
	virtual void AddIteratorProperty(int nextIdx = 0);
};

class CMFCPropertyIteratorSP_SOECB : public CMFCPropertyIterator		//SP_SOECB ITER
{
public:
	CMFCPropertyIteratorSP_SOECB(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);
	CMFCPropertyIteratorSP_SOECB(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	virtual ~CMFCPropertyIteratorSP_SOECB();

protected:
	virtual void AddIteratorProperty(int nextIdx = 0);
};

class CMFCPropertyIteratorSP_SOECEB : public CMFCPropertyIterator		//SP_SOECEB ITER
{
public:
	CMFCPropertyIteratorSP_SOECEB(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);
	CMFCPropertyIteratorSP_SOECEB(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	virtual ~CMFCPropertyIteratorSP_SOECEB();

protected:
	virtual void AddIteratorProperty(int nextIdx = 0);
};

class CMFCPropertyIteratorSP_SOMSB : public CMFCPropertyIterator		//SP_SOMSB ITER
{
public:
	CMFCPropertyIteratorSP_SOMSB(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);
	CMFCPropertyIteratorSP_SOMSB(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	virtual ~CMFCPropertyIteratorSP_SOMSB();

protected:
	virtual void AddIteratorProperty(int nextIdx = 0);
};

class CMFCPropertyIteratorSP_ADBFI : public CMFCPropertyIterator		//SP_ADBFI ITER
{
public:
	CMFCPropertyIteratorSP_ADBFI(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);
	CMFCPropertyIteratorSP_ADBFI(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	virtual ~CMFCPropertyIteratorSP_ADBFI();

protected:
	virtual void AddIteratorProperty(int nextIdx = 0);
};

class CMFCPropertyIteratorSP_ALDBF : public CMFCPropertyIterator		//SP_ALDBF ITER
{
public:
	CMFCPropertyIteratorSP_ALDBF(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);
	CMFCPropertyIteratorSP_ALDBF(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	virtual ~CMFCPropertyIteratorSP_ALDBF();

protected:
	virtual void AddIteratorProperty(int nextIdx = 0);
};

//----------------------------------------------------------------- Send End -----------------------------------------------------------------//

class CMFCPropertyIteratorATO_VER : public CMFCPropertyIterator		//HS Req ITER
{
public:
	CMFCPropertyIteratorATO_VER(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);
	CMFCPropertyIteratorATO_VER(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	virtual ~CMFCPropertyIteratorATO_VER();

protected:
	virtual void AddIteratorProperty(int nextIdx = 0);
};

class CMFCPropertyIteratorSPReq_SP : public CMFCPropertyIterator		//SP Req ITER
{
public:
	CMFCPropertyIteratorSPReq_SP(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);
	CMFCPropertyIteratorSPReq_SP(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	virtual ~CMFCPropertyIteratorSPReq_SP();

protected:
	virtual void AddIteratorProperty(int nextIdx = 0);
};

class CMFCPropertyIteratorSTR_TP : public CMFCPropertyIterator		//SP Req ITER
{
public:
	CMFCPropertyIteratorSTR_TP(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);
	CMFCPropertyIteratorSTR_TP(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	virtual ~CMFCPropertyIteratorSTR_TP();

protected:
	virtual void AddIteratorProperty(int nextIdx = 0);
};
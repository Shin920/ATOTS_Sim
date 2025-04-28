#include "pch.h"
#include "GridPropertyOverride.h"
#include "CTabDlg1.h"
#include "packet01Tab.h"
#include "packet02Tab.h"
#include "packet04Tab.h"
#include "packet07Tab.h"
#include "packet09Tab.h"
#include "packet10Tab.h"



CMFCPropertyStr::CMFCPropertyStr(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList)
	: CMFCPropertyGridProperty(strGroupName, dwData, bIsValueList)
{
	Init();
	
}


CMFCPropertyStr::CMFCPropertyStr(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars)
	: CMFCPropertyGridProperty(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{
	Init();
}

CMFCPropertyStr::~CMFCPropertyStr()
{
	
}

BOOL CMFCPropertyStr::OnEndEdit()
{
	//최대문자수 조절
	CString tempStr;
	tempStr = m_varValue.bstrVal;
	int length = tempStr.GetLength();
	if (tempStr.GetLength() > maxLength)
	{
		m_varValue.SetString(tempStr.Left(maxLength), VT_BSTR);
	}
	__super::OnEndEdit();
	return TRUE;
}

BOOL CMFCPropertyStr::OnUpdateValue()
{
	__super::OnUpdateValue();
	return TRUE;
}

void CMFCPropertyStr::Init()
{
	
	maxLength = 256;
}

CMFCPropertyHeader::CMFCPropertyHeader(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList)
	: CMFCPropertyGridProperty(strGroupName, dwData, bIsValueList)
{
	Init();
}

CMFCPropertyHeader::~CMFCPropertyHeader()
{
}

void CMFCPropertyHeader::SetPacketNumber(int n)
{
	this->GetSubItem(IDX_NID_PACKET_ATO)->SetValue((variant_t)n);
}

void CMFCPropertyHeader::Init()
{

	CMFCPropertyEX<int>* pxi;
	CMFCPropertyEX<UINT32>* pxu;
	
	
	

	pxi = new CMFCPropertyEX<int>(_T("NID_PACKET_ATO"), (variant_t)(int)0,
		_T("ATO Packet number."));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 255);
	pxi->Enable(FALSE);
	this->AddSubItem(pxi);

	pxu = new CMFCPropertyEX<UINT32>(_T("NID_OPERATIONAL"), (variant_t)(UINT32)0,
		_T("Length of Header + User Data length."));
	pxu->SetFormat(_T("%u"));
	pxu->SetMinMaxValue(0, UINT32_MAX);
	pxu->Enable(FALSE);	
	this->AddSubItem(pxu);

	pxi = new CMFCPropertyEX<int>(_T("NID_ENGINE"), (variant_t)(int)0,
		_T("ATO Packet number."));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 255);
	pxi->Enable(FALSE);
	this->AddSubItem(pxi);

	pxi = new CMFCPropertyEX<int>(_T("T_TimeStamp_Date"), (variant_t)(int)0,
		_T("ATO Packet number."));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 255);
	pxi->Enable(FALSE);
	this->AddSubItem(pxi);

	pxi = new CMFCPropertyEX<int>(_T("T_TimeStamp_Seconds"), (variant_t)(int)0,
		_T("ATO Packet number."));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 255);
	pxi->Enable(FALSE);
	this->AddSubItem(pxi);

	pxi = new CMFCPropertyEX<int>(_T("N_Packet_Counter"), (variant_t)(int)0,
		_T("ATO Packet number."));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 255);
	pxi->Enable(FALSE);
	this->AddSubItem(pxi);



	
	this->Expand(FALSE);
}

//=============================================================================================================

template <typename T>
CMFCPropertyEX<T>::CMFCPropertyEX(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList)
	: CMFCPropertyGridProperty(strGroupName, dwData, bIsValueList)
{
	Init();
}

template <typename T>
CMFCPropertyEX<T>::CMFCPropertyEX(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars)
	: CMFCPropertyGridProperty(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{
	Init();
}


template <typename T>
CMFCPropertyEX<T>::~CMFCPropertyEX()
{
}


template <typename T>
void CMFCPropertyEX<T>::OnSelectCombo()
{
	int comboIdx = this->m_pWndCombo->GetCurSel(); //선택하지 않고 닫는 경우 -1
	T comboValue = (T)comboIdx;
	//콤보박스 선택 인덱스를 특정 값으로 매핑해주는 리스트가 존재하는 경우 그 값을 사용
	if (!comboBoxValueList.empty())
	{
		unordered_map<int, T>::iterator it = comboBoxValueList.find(comboIdx);
		//콤보박스 선택 인덱스에 해당하는 값이 존재하는 경우
		if (it != comboBoxValueList.end())
		{
			comboValue = it->second;
		}
	}


	m_varValue = ((variant_t)comboValue);

	__super::OnEndEdit();
	return;
}


template <typename T>
BOOL CMFCPropertyEX<T>::OnUpdateValue()
{
	int asdfads = 111;
	CString* asdf = (CString*)&m_varValue.bstrVal;
	__super::OnUpdateValue();
	return TRUE;
}


template <typename T>
BOOL CMFCPropertyEX<T>::OnEndEdit()
{
	//최대 최소값 조절
	T varValue = NULL;
	GetValueFromVariantT(varValue); //m_varValue의 vt값을 읽어서 형 변환 후 varValue에 저장

	//특수 값이 존재하는 경우 최대최소 값 검사 제외
	unordered_map<T, const TCHAR*>::iterator it = specialValueList.find(varValue);
	if (it == specialValueList.end())
	{
		if (varValue > maxVal)m_varValue = ((variant_t)maxVal);
		else if (varValue < minVal)m_varValue = ((variant_t)minVal);
	}
	__super::OnEndEdit();
	return TRUE;
}


template <typename T>
void CMFCPropertyEX<T>::GetValueFromVariantT(T& t)
{
	switch (GetValue().vt)
	{
	case VT_I1:
	case VT_I2:
	case VT_I4:
	case VT_I8:
	case VT_INT:
		t = (T)(m_varValue.intVal);
		break;

	case VT_R4:
	case VT_R8:
		t = (T)(m_varValue.dblVal);
		break;

	case VT_UI1:
	case VT_UI2:
	case VT_UI4:
	case VT_UI8:
	case VT_UINT:
		t = (T)(m_varValue.uintVal);
		break;
	default:
		break;
	}
}


template <typename T>
CString CMFCPropertyEX<T>::FormatProperty()
{
	CString str;
	variant_t val = GetValue();

	//특정 값을 입력 받으면 해당하는 format으로 변경
	unordered_map<T, const TCHAR*>::iterator it = specialValueList.find(val);
	if (it != specialValueList.end())
	{
		T currentVal;
		GetValueFromVariantT(currentVal);
		str.Format(it->second, currentVal);
		return str;
	}

	T varValue = NULL;
	GetValueFromVariantT(varValue);

	str.Format(m_format, varValue);

	return str;
}


template <typename T>
void CMFCPropertyEX<T>::SetFormat(const TCHAR* format)
{
	m_format = format;
}


template <typename T>
void CMFCPropertyEX<T>::AddSpecialValue(pair<T, const TCHAR*> p)
{
	specialValueList.insert(p);
}


template <typename T>
void CMFCPropertyEX<T>::AddComboBoxlValue(pair<int, T> p)
{
	comboBoxValueList.insert(p);
}

template <typename T>
void CMFCPropertyEX<T>::SetMinMaxValue(T minv, T maxv, int flag)
{
	if ((flag & SMMF_SetMin) != 0)minVal = minv;
	if ((flag & SMMF_SetMax) != 0)maxVal = maxv;
}

template<typename T>
void CMFCPropertyEX<T>::AddSpecialOption(const TCHAR* optionTXT)
{
	AddOption(optionTXT);
	AddSpecialValue(make_pair(comboBoxOptionAmount, optionTXT));
	comboBoxOptionAmount++;
}

template<typename T>
void CMFCPropertyEX<T>::AddSpecialOption(const TCHAR* optionTXT, const TCHAR* formatTXT)
{
	AddOption(optionTXT);
	AddSpecialValue(make_pair(comboBoxOptionAmount, formatTXT));
	comboBoxOptionAmount++;
}

template<typename T>
void CMFCPropertyEX<T>::AddSpecialOption(const TCHAR* optionTXT, T actualValue)
{
	AddOption(optionTXT);
	AddComboBoxlValue(make_pair(comboBoxOptionAmount, actualValue));
	AddSpecialValue(make_pair(actualValue, optionTXT));
	comboBoxOptionAmount++;
}

template<typename T>
void CMFCPropertyEX<T>::AddSpecialOption(const TCHAR* optionTXT, const TCHAR* formatTXT, T actualValue)
{
	//콤보박스 아이템 추가
	AddOption(optionTXT);

	//콤보박스 선택 시 컨트롤에 실제 입력될 값을 지정
	AddComboBoxlValue(make_pair(comboBoxOptionAmount, actualValue));

	//해당 값이 입력 될 때 표시할 format 지정
	AddSpecialValue(make_pair(actualValue, formatTXT));

	comboBoxOptionAmount++;
}


template <typename T>
void CMFCPropertyEX<T>::Init()
{
	SetFormat(_T("Default Format. %d"));
	SetMinMaxValue((T)0, (T)46);
	comboBoxOptionAmount = 0;
}


template class CMFCPropertyEX<int>;
template class CMFCPropertyEX<UINT32>;

//======================================================================================================





CMFCPropertySwitch::CMFCPropertySwitch(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList)
	: CMFCPropertyGridProperty(strGroupName, dwData, bIsValueList)
{
	Init();
}


CMFCPropertySwitch::CMFCPropertySwitch(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars)
	: CMFCPropertyGridProperty(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{
	Init();
}

CMFCPropertySwitch::~CMFCPropertySwitch()
{
	Release();
}

void CMFCPropertySwitch::OnSelectCombo()
{
	int comboIdx = this->m_pWndCombo->GetCurSel();
	BOOL enable = FALSE;
	switch (comboIdx)
	{
	case 0: enable = TRUE; break; // Yes
	default: break;
	}

	SetTargetEnableState(enable);

	__super::OnSelectCombo();
	return;
}

BOOL CMFCPropertySwitch::OnDblClk(CPoint point)
{
	return FALSE;
}

void CMFCPropertySwitch::Init()
{
	targets = new list<CMFCPropertyGridProperty*>;
	SetValue(_T("No"));

	AddOption(_T("Yes"));
	AddOption(_T("No"));

	AllowEdit(FALSE);
}

void CMFCPropertySwitch::Release()
{
	delete targets;
}

void CMFCPropertySwitch::AddTarget(CMFCPropertyGridProperty* t)
{
	targets->push_back(t);
}

void CMFCPropertySwitch::SetTargetEnableState(BOOL b)
{
	for (CMFCPropertyGridProperty*& it : *targets)
	{
		it->Enable(b);
	}
}

CMFCPropertyIterator::CMFCPropertyIterator(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList)
	: CMFCPropertyEX<int>(strGroupName, dwData, bIsValueList)
{
	targetProperty = nullptr;
}

CMFCPropertyIterator::CMFCPropertyIterator(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars)
	: CMFCPropertyEX<int>(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{
	targetProperty = nullptr;
}

CMFCPropertyIterator::~CMFCPropertyIterator()
{
}


BOOL CMFCPropertyIterator::OnEndEdit()
{
	//최대 최소값 조절 (PropertyEX::OnEndEdit())
	__super::OnEndEdit();

	AdjustIteratorProperty();
	//값 변경 후 개수 조절 후 컨트롤 다시 그리기

	return TRUE;
}

void CMFCPropertyIterator::AdjustIteratorProperty()
{
	if (targetProperty == nullptr) return;

	int itercount = targetProperty->GetSubItemsCount(); // 현재 iterator 개수
	const int currentIterCount = m_varValue.intVal;

	while (itercount != currentIterCount)
	{
		if (itercount < currentIterCount) // 프로퍼티 추가
		{
			AddIteratorProperty(itercount);
			itercount++;
		}
		else // 프로퍼티 제거
		{
			itercount--;
			RemoveIteratorProperty(itercount);
		}
	}

	//변경 적용
	this->m_pWndList->AdjustLayout();
}

void CMFCPropertyIterator::RemoveIteratorProperty(int targetIdx)
{
	assert(targetProperty != nullptr);

	CMFCPropertyGridProperty*&& targetPropertyIter = targetProperty->GetSubItem(targetIdx); //[x]라고 써있는 프로퍼티

	int targetIterIdx = targetPropertyIter->GetSubItemsCount(); //[x]의 아이템 개수
	while (targetIterIdx) //뒤에서부터 제거
	{
		CMFCPropertyGridProperty*&& targetToBeRemoved = targetPropertyIter->GetSubItem(--targetIterIdx); //삭제할 목표 설정
		targetPropertyIter->RemoveSubItem(targetToBeRemoved);
	}
	targetProperty->RemoveSubItem(targetPropertyIter); //[x] 제거
}


CMFCPropertyIteratorJP::CMFCPropertyIteratorJP(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList) : CMFCPropertyIterator(strGroupName, dwData, bIsValueList)
{
}

CMFCPropertyIteratorJP::CMFCPropertyIteratorJP(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars) : CMFCPropertyIterator(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{
}

CMFCPropertyIteratorJP::~CMFCPropertyIteratorJP()
{
}

void CMFCPropertyIteratorJP::AddIteratorProperty(int nextIdx)	// next Idx = ITER 값
																// 
{
	assert(targetProperty != nullptr);

	TCHAR idxStr[10];
	_stprintf_s(idxStr, _T("[%d]"), nextIdx);
	CMFCPropertyGridProperty* pxHeader = new CMFCPropertyGridProperty(idxStr); // 개별 Iterator를 담는 프로퍼티
	targetProperty->AddSubItem(pxHeader);

	int headerIdxValue = nextIdx;
	CMFCPropertyEX<int>* pxi;
	CMFCPropertyEX<UINT32>* pxu;
	//CMFCPropertyGridProperty* pxProperty;
	CMFCPropertyIterator* pxiterContainer;
	CMFCPropertyGridProperty* pxiter;
	
	TCHAR JP_IterHeader[30];

	_stprintf_s(JP_IterHeader, _T("[%d] NID_C"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(JP_IterHeader, (variant_t)(int)887,	_T("SP의 나라 혹은 지역의 ID"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b1111111111);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(JP_IterHeader, _T("[%d] NID_SP"), headerIdxValue);
	pxu = new CMFCPropertyEX<UINT32>(JP_IterHeader, (variant_t)(UINT32)0,
		_T("SP의 ID"));
	pxu->SetFormat(_T("%u"));
	pxu->SetMinMaxValue(0, UINT32_MAX);
	pxu->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxu);

	_stprintf_s(JP_IterHeader, _T("[%d] M_SP_Version"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(JP_IterHeader, (variant_t)(int)0,
		_T("SP 버전의 Identifier"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, UINT16_MAX);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(JP_IterHeader, _T("[%d] Q_SP_DIR"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(JP_IterHeader, (variant_t)(int)0,
		_T("SP 버전의 Identifier"));
	pxi->SetFormat(_T(""));
	pxi->SetMinMaxValue(0, 0b1);
	pxi->AddSpecialOption(_T("Reverse"), _T("%d: Reverse"));
	pxi->AddSpecialOption(_T("Nominal"), _T("%d: Nominal"));
	pxi->AllowEdit(FALSE);
	pxHeader->AddSubItem(pxi);
	
	//참고할 TP 시작

	_stprintf_s(JP_IterHeader, _T("[%d] N_ITER"), headerIdxValue);
	pxiterContainer = new CMFCPropertyIteratorJP_TP(JP_IterHeader, (variant_t)(int)3, _T("TP 정보의 개수"));
	pxiterContainer->SetFormat(_T("%d"));
	pxiterContainer->SetMinMaxValue(0, UINT8_MAX);
	pxiterContainer->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxiterContainer);

	pxiter = new CMFCPropertyGridProperty(_T("Timing Points"));
	pxHeader->AddSubItem(pxiter);

	pxiterContainer->SetTargetProperty(pxiter);			// 함수 수정하고 값 수정
	pxiterContainer->AdjustIteratorProperty();

	// TP 끝

	_stprintf_s(JP_IterHeader, _T("[%d] N_ITER"), headerIdxValue);
	pxiterContainer = new CMFCPropertyIteratorJP_TC(JP_IterHeader, (variant_t)(int)2, _T("Temporary Constraints의 개수"));
	pxiterContainer->SetFormat(_T("%d"));
	pxiterContainer->SetMinMaxValue(0, 0b11111);
	pxiterContainer->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxiterContainer);

	pxiter = new CMFCPropertyGridProperty(_T("Temporary Constraints"));
	pxHeader->AddSubItem(pxiter);

	pxiterContainer->SetTargetProperty(pxiter);			
	pxiterContainer->AdjustIteratorProperty();


}

CMFCPropertyIteratorJP_TP::CMFCPropertyIteratorJP_TP(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList) : CMFCPropertyIterator(strGroupName, dwData, bIsValueList)
{
}

CMFCPropertyIteratorJP_TP::CMFCPropertyIteratorJP_TP(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars) : CMFCPropertyIterator(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{
}

CMFCPropertyIteratorJP_TP::~CMFCPropertyIteratorJP_TP()
{
}

void CMFCPropertyIteratorJP_TP::AddIteratorProperty(int nextIdx)
{
	assert(targetProperty != nullptr);

	TCHAR idxStr[10];
	_stprintf_s(idxStr, _T("[%d]"), nextIdx);
	CMFCPropertyGridProperty* pxHeader = new CMFCPropertyGridProperty(idxStr); // 개별 Iterator를 담는 프로퍼티
	targetProperty->AddSubItem(pxHeader);

	int headerIdxValue = nextIdx;
	CMFCPropertyEX<int>* pxi;
	CMFCPropertyEX<UINT32>* pxu;

	TCHAR JP_TP_IterHeader[30];

	_stprintf_s(JP_TP_IterHeader, _T("[%d] NID_TP"), headerIdxValue);
	pxu = new CMFCPropertyEX<UINT32>(JP_TP_IterHeader, (variant_t)(UINT32)0,
		_T("TP ID"));
	pxu->SetFormat(_T("%u"));
	pxu->SetMinMaxValue(0, UINT32_MAX);
	pxu->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxu);

	_stprintf_s(JP_TP_IterHeader, _T("[%d] T_Latest_Arrival_Date"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(JP_TP_IterHeader, (variant_t)(int)0,
		_T("요청된 TP에서의 도착시간 데이터"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b111111111111111);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(JP_TP_IterHeader, _T("[%d] T_Latest_Arrival_Seconds"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(JP_TP_IterHeader, (variant_t)(int)0,
		_T("요청된 TP에서의 도착시간 초"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b11111111111111111);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(JP_TP_IterHeader, _T("[%d] T_Arrival_Window"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(JP_TP_IterHeader, (variant_t)(int)0,
		_T("해당 시점(TP)보다 일찍 도착할 수 있는 허용 시간범위"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b1111111111);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(JP_TP_IterHeader, _T("[%d] Q_TP_Alignment"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(JP_TP_IterHeader, (variant_t)(int)0,
		_T("이 qualifier는 TP location이 열차의 전방,중간,끝에서 적용가능한지 정의합니다."));
	pxi->SetFormat(_T(""));
	pxi->SetMinMaxValue(0, 0b11);
	pxi->AddSpecialOption(_T("Front"), _T("%d: Front"));
	pxi->AddSpecialOption(_T("Middle"), _T("%d: Middle"));
	pxi->AddSpecialOption(_T("Rear"), _T("%d: Rear"));
	pxi->AllowEdit(FALSE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(JP_TP_IterHeader, _T("[%d] Q_Stop_Skip_Pass"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(JP_TP_IterHeader, (variant_t)(int)0,
		_T("Timing Point가 Passing Point, operational Stopping Point, Skipped Stopping Point인지 명시합니다."));
	pxi->SetFormat(_T(""));
	pxi->SetMinMaxValue(0, 0b11);
	pxi->AddSpecialOption(_T("Stopping Point"), _T("%d: Stopping Point"));
	pxi->AddSpecialOption(_T("Stopping Point to be skipped"), _T("%d: Stopping Point to be skipped"));
	pxi->AddSpecialOption(_T("Passing Point"), _T("%d: Passing Point"));
	pxi->AllowEdit(FALSE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(JP_TP_IterHeader, _T("[%d] Q_TP_Information"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(JP_TP_IterHeader, (variant_t)(int)0,
		_T("TP에 대해 명시하는 specific information을 명시합니다."));
	pxi->SetFormat(_T(""));
	pxi->SetMinMaxValue(0, 1);
	pxi->AddSpecialOption(_T("No specific information"), _T("%d: No specific information"));
	pxi->AddSpecialOption(_T("End of Journey"), _T("%d: End of Journey"));
	pxi->AllowEdit(FALSE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(JP_TP_IterHeader, _T("[%d] Q_Day_Light_Saving"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(JP_TP_IterHeader, (variant_t)(int)0,
		_T("이 변수는 일광절약시간을 현지 시간에 적용 가능한지 정의합니다."));
	pxi->SetFormat(_T(""));
	pxi->SetMinMaxValue(0, 0b1);
	pxi->AddSpecialOption(_T("No saving hour"), _T("%d: No saving hour"));
	pxi->AddSpecialOption(_T("Saving hour"), _T("%d: Saving hour"));
	pxi->AllowEdit(FALSE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(JP_TP_IterHeader, _T("[%d] Q_Opening_Door_Side"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(JP_TP_IterHeader, (variant_t)(int)0,
		_T("ATO-OB가 열차 문 개방을 관리해야하는지 및 어느 쪽 문이 열려야하는지에 대한 여부를 명시합니다. 문 방향은 JP의 진행방향 기준입니다."));
	pxi->SetFormat(_T(""));
	pxi->SetMinMaxValue(0, 0b11);
	pxi->AddSpecialOption(_T("none"), _T("00: None"));
	pxi->AddSpecialOption(_T("right"), _T("01: Right"));
	pxi->AddSpecialOption(_T("left"), _T("10: Left"));
	pxi->AddSpecialOption(_T("both"), _T("11: Both"));
	pxi->AllowEdit(FALSE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(JP_TP_IterHeader, _T("[%d] Q_Centralised_Opening"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(JP_TP_IterHeader, (variant_t)(int)0,
		_T("이 변수는 문이 중앙집중방식으로 혹은 승객에 의해 열려야 하는지에 대한 여부를 정의합니다. "));
	pxi->SetFormat(_T(""));
	pxi->SetMinMaxValue(0, 0b1);
	pxi->AddSpecialOption(_T("Opening by passengers"), _T("%d: Opening by passengers"));
	pxi->AddSpecialOption(_T("Centralised automatic opening of the relevant doors"), _T("%d: Centralised automatic opening of the relevant doors"));
	pxi->AllowEdit(FALSE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(JP_TP_IterHeader, _T("[%d] Q_Relaxed_Coupler"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(JP_TP_IterHeader, (variant_t)(int)0,
		_T("이 변수는 열차에게 연결기 개방 상태로 정지하는것을 요청합니다."));
	pxi->SetFormat(_T(""));
	pxi->SetMinMaxValue(0, 0b1);
	pxi->AddSpecialOption(_T("no request for coupler relaxation"), _T("%d: No request for coupler relaxation"));
	pxi->AddSpecialOption(_T("request for coupler relaxation"), _T("%d: Request for coupler relaxation"));
	pxi->AllowEdit(FALSE);
	pxHeader->AddSubItem(pxi);
		
	_stprintf_s(JP_TP_IterHeader, _T("[%d] Q_Train_Hold"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(JP_TP_IterHeader, (variant_t)(int)0,
		_T("이 변수는 열차가 Stopping Point에 대기하도록 요청되었는지에 대한 여부를 정의합니다."));
	pxi->SetFormat(_T(""));
	pxi->SetMinMaxValue(0, 0b1);
	pxi->AddSpecialOption(_T("Do not hold train"), _T("%d: Do not hold train"));
	pxi->AddSpecialOption(_T("Hold train"), _T("%d: Hold train"));
	pxi->AllowEdit(FALSE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(JP_TP_IterHeader, _T("[%d] T_Departure_Date"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(JP_TP_IterHeader, (variant_t)(int)0,
		_T("Stopping Point로부터 예상되는 출발시간 날짜."));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b111111111111111);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(JP_TP_IterHeader, _T("[%d] T_Departure_Seconds"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(JP_TP_IterHeader, (variant_t)(int)0,
		_T("Stopping Point로부터 예상되는 출발시간 초"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b11111111111111111);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(JP_TP_IterHeader, _T("[%d] T_Minimum_Dwell_Time"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(JP_TP_IterHeader, (variant_t)(int)0,
		_T("주어진 Stopping Point의 최소 체류시간"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b1111111111);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(JP_TP_IterHeader, _T("[%d] Q_Automatic_Closing"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(JP_TP_IterHeader, (variant_t)(int)0,
		_T("주어진 Stopping Point의 최소 체류시간"));
	pxi->SetFormat(_T("d"));
	pxi->SetMinMaxValue(0, 0b1);
	pxi->AddSpecialOption(_T("ATO-OB does not manage train door closing"), _T("%d: ATO-OB does not manage train door closing"));
	pxi->AddSpecialOption(_T("ATO-OB manages train door closing"), _T("%d: ATO-OB manages train door closing"));
	pxi->AllowEdit(FALSE);
	pxHeader->AddSubItem(pxi);

}

CMFCPropertyIteratorJP_TC::CMFCPropertyIteratorJP_TC(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList) : CMFCPropertyIterator(strGroupName, dwData, bIsValueList)
{
}

CMFCPropertyIteratorJP_TC::CMFCPropertyIteratorJP_TC(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars) : CMFCPropertyIterator(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{
}

CMFCPropertyIteratorJP_TC::~CMFCPropertyIteratorJP_TC()
{
}

void CMFCPropertyIteratorJP_TC::AddIteratorProperty(int nextIdx)
{
	assert(targetProperty != nullptr);

	TCHAR idxStr[10];
	_stprintf_s(idxStr, _T("[%d]"), nextIdx);
	CMFCPropertyGridProperty* pxHeader = new CMFCPropertyGridProperty(idxStr); // 개별 Iterator를 담는 프로퍼티
	targetProperty->AddSubItem(pxHeader);

	int headerIdxValue = nextIdx;
	CMFCPropertyEX<int>* pxi;

	TCHAR JP_TC_IterHeader[30];

	_stprintf_s(JP_TC_IterHeader, _T("[%d] Q_TC_Type"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(JP_TC_IterHeader, (variant_t)(int)0,
		_T("temporary constraint의 종류"));
	pxi->SetFormat(_T(""));
	pxi->SetMinMaxValue(0, 0b111);
	pxi->AddSpecialOption(_T("ASR"), _T("%d: ASR"));
	pxi->AddSpecialOption(_T("Low Adhesion"), _T("%d: Low Adhesion"));
	pxi->AddSpecialOption(_T("ATO Inhibition Zone"), _T("%d: ATO Inhibition Zone"));
	pxi->AddSpecialOption(_T("DAS Inhibition Zone"), _T("%d: DAS Inhibition Zone"));
	pxi->AddSpecialOption(_T("Current Limitation"), _T("%d: Current Limitation"));
	pxi->AllowEdit(FALSE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(JP_TC_IterHeader, _T("[%d] Q_Range"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(JP_TC_IterHeader, (variant_t)(int)0,
		_T("제약이 시작, 종료, 시작 및 종료, SP전체를 포함하는지 명시합니다."));
	pxi->SetFormat(_T(""));
	pxi->SetMinMaxValue(0, 0b11);
	pxi->AddSpecialOption(_T("Starts"), _T("%d: Starts"));
	pxi->AddSpecialOption(_T("Ends"), _T("%d: Ends"));
	pxi->AddSpecialOption(_T("StartsEnds"), _T("%d: StartsEnds"));
	pxi->AddSpecialOption(_T("WholeSP"), _T("%d: WholeSP"));
	pxi->AllowEdit(FALSE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(JP_TC_IterHeader, _T("[%d] D_TC_Start_Location"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(JP_TC_IterHeader, (variant_t)(int)0,
		_T("SP의 시작부분에 상대적인 temporary constraint의 시작위치."));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xffffff);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(JP_TC_IterHeader, _T("[%d] D_TC_End_Location"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(JP_TC_IterHeader, (variant_t)(int)0,
		_T("SP의 시작부분에 상대적인 temporary constraint의 끝위치."));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xffffff);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(JP_TC_IterHeader, _T("[%d] Q_FRONT"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(JP_TC_IterHeader, (variant_t)(int)0,
		_T("프로필 요소에 대해 주어진 속도제한이 열차의 앞부분 혹은 뒷부분이 해당 요소를 떠날때까지 적용되어야 하는지에 대한 Qualifier"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b1);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(JP_TC_IterHeader, _T("[%d] V_Speed_Level"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(JP_TC_IterHeader, (variant_t)(int)0,
		_T("사용된 값은 V_STATIC와 유사하게 7비트에 의해 제공된 처음 120개의 값만 speed Level을 결정하는데 사용됩니다."));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 120);	// Spare 값 반영 7bit -> 120
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(JP_TC_IterHeader, _T("[%d] Q_Adhesion_Cateogory"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(JP_TC_IterHeader, (variant_t)(int)0,
		_T("lower boundaries에 대한 wheel-rail 인터페이스의 EN15595에 정의에 따라 정렬된 Adhesion category."));
	pxi->SetFormat(_T(""));
	pxi->SetMinMaxValue(0, 0b111);
	pxi->AddSpecialOption(_T("Dry Rail"), _T("%d: Dry Rail"));
	pxi->AddSpecialOption(_T("Dry Rail (Medium)"), _T("%d: Dry Rail (Medium)"));
	pxi->AddSpecialOption(_T("Dry Rail (Low)"), _T("%d: Dry Rail (Low)"));
	pxi->AddSpecialOption(_T("Low Adhesion"), _T("%d: Low Adhesion"));
	pxi->AddSpecialOption(_T("Very Low Adhesion"), _T("%d: Very Low Adhesion"));
	pxi->AllowEdit(FALSE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(JP_TC_IterHeader, _T("[%d] M_CURRENT"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(JP_TC_IterHeader, (variant_t)(int)0,
		_T("허가된 전류소비량"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b1111111111);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);
		


}

CMFCPropertyIteratorSP::CMFCPropertyIteratorSP(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList) : CMFCPropertyIterator(strGroupName, dwData, bIsValueList)
{
}

CMFCPropertyIteratorSP::CMFCPropertyIteratorSP(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars) : CMFCPropertyIterator(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{
}

CMFCPropertyIteratorSP::~CMFCPropertyIteratorSP()
{
}

void CMFCPropertyIteratorSP::AddIteratorProperty(int nextIdx)
{
	assert(targetProperty != nullptr);

	TCHAR idxStr[10];
	_stprintf_s(idxStr, _T("[%d]"), nextIdx);
	CMFCPropertyGridProperty* pxHeader = new CMFCPropertyGridProperty(idxStr); // 개별 Iterator를 담는 프로퍼티
	targetProperty->AddSubItem(pxHeader);


	int headerIdxValue = nextIdx;
	CMFCPropertyEX<int>* pxi;
	CMFCPropertyEX<UINT32>* pxu;
	CMFCPropertyIterator* pxiterContainer;
	CMFCPropertyGridProperty* pxiter;

	TCHAR SP_IterHeader[100];

	_stprintf_s(SP_IterHeader, _T("[%d] NID_C"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_IterHeader, (variant_t)(int)887,
		_T("SP의 나라 혹은 지역의 ID"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b1111111111);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_IterHeader, _T("[%d] NID_SP"), headerIdxValue);
	pxu = new CMFCPropertyEX<UINT32>(SP_IterHeader, (variant_t)(UINT32)0,
		_T("SP의 ID"));
	pxu->SetFormat(_T("%u"));
	pxu->SetMinMaxValue(0, UINT32_MAX);
	pxu->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxu);

	_stprintf_s(SP_IterHeader, _T("[%d] Q_SP_Status"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_IterHeader, (variant_t)(int)0,
		_T("SP의 상태"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b1);
	pxi->AddSpecialOption(_T("Invalid"), _T("%d: Invalid"));
	pxi->AddSpecialOption(_T("Valid"), _T("%d: Valid"));
	pxi->AllowEdit(FALSE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_IterHeader, _T("[%d] M_SP_Version"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_IterHeader, (variant_t)(int)0,
		_T("SP 버전의 Identifier"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, UINT16_MAX);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_IterHeader, _T("[%d] L_SP"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_IterHeader, (variant_t)(int)0,
		_T("SP가 적용되는 railway Segment의 길이"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xffffff);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_IterHeader, _T("[%d] D_EoA_Offset"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_IterHeader, (variant_t)(int)0,
		_T("EoA 후방에 열차가 멈추기 위한 거리"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xffffff);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_IterHeader, _T("[%d] Q_UTC_Offset"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_IterHeader, (variant_t)(int)0,
		_T("local time을 계산하기 위해 UTC시간에 더하기 위한 오프셋"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b1111111);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_IterHeader, _T("[%d] M_SP_Altitude"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_IterHeader, (variant_t)(int)0,
		_T("SP의 시작점에서의 고도"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xfffff);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_IterHeader, _T("[%d] Q_ATOTS_Contact_Info_Dir"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_IterHeader, (variant_t)(int)0,
		_T("또다른 ATO-TS의 접촉 정보가 SP의 정/역 운행 방향에 대해 유효한지 혹은 접촉 정보가 없는지 여부를 표시하는 Qualifier"));
	pxi->SetFormat(_T(""));
	pxi->SetMinMaxValue(0, 0b11);
	pxi->AddSpecialOption(_T("No Contact info follows"), _T("%d : No Contact info follows"));
	pxi->AddSpecialOption(_T("ATO-TS contact info for nominal direction follows"), _T("%d: ATO-TS contact info for nominal direction follows"));
	pxi->AddSpecialOption(_T("ATO-TS contact info for reverse direction follows"), _T("%d: ATO-TS contact info for reverse direction follows"));
	pxi->AllowEdit(FALSE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_IterHeader, _T("[%d] NID_C"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_IterHeader, (variant_t)(int)887,
		_T("ATO-TS의 나라 혹은 지역의 ID"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b1111111111);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_IterHeader, _T("[%d] NID_ATOTS"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_IterHeader, (variant_t)(int)0,
		_T("인접한 ATO-TS의 ID"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b11111111111111);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_IterHeader, _T("[%d] NID_C"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_IterHeader, (variant_t)(int)887,
		_T("SP의 나라 혹은 지역의 ID"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b1111111111);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_IterHeader, _T("[%d] NID_SP"), headerIdxValue);
	pxu = new CMFCPropertyEX<UINT32>(SP_IterHeader, (variant_t)(UINT32)0,
		_T("인접 ATO-TS 영역의 첫번째 segment의 SP ID"));
	pxu->SetFormat(_T("%u"));
	pxu->SetMinMaxValue(0, UINT32_MAX);
	pxu->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxu);

	_stprintf_s(SP_IterHeader, _T("[%d] V_STATIC"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_IterHeader, (variant_t)(int)0,
		_T("SP의 시작부분에서의 기본 Static Speed Profile 속도"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b1111111);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);
	

	_stprintf_s(SP_IterHeader, _T("[%d] Q_FRONT"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_IterHeader, (variant_t)(int)0,
		_T("프로필 요소에 대해 주어진 속도제한이 열차의 앞부분 혹은 뒷부분이 해당 요소를 떠날때까지 적용되어야 하는지에 대한 표시하기 위한 Qualifier"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b1);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);


	//SSP 시작

	_stprintf_s(SP_IterHeader, _T("[%d] N_ITER"), headerIdxValue);
	pxiterContainer = new CMFCPropertyIteratorSP_SSP(SP_IterHeader, (variant_t)(int)1, _T("Specific SSP의 개수"));
	pxiterContainer->SetFormat(_T("%d"));
	pxiterContainer->SetMinMaxValue(0, 0b11111);
	pxiterContainer->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxiterContainer);

	pxiter = new CMFCPropertyGridProperty(_T("SSP"));
	pxHeader->AddSubItem(pxiter);

	pxiterContainer->SetTargetProperty(pxiter);			// 함수 수정하고 값 수정
	pxiterContainer->AdjustIteratorProperty();

	//SSP Change 시작

	_stprintf_s(SP_IterHeader, _T("[%d] N_ITER"), headerIdxValue);
	pxiterContainer = new CMFCPropertyIteratorSP_SSPCHG(SP_IterHeader, (variant_t)(int)0, _T("Static Speed Profile Change의 개수"));
	pxiterContainer->SetFormat(_T("%d"));
	pxiterContainer->SetMinMaxValue(0, 0b11111);
	pxiterContainer->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxiterContainer);

	pxiter = new CMFCPropertyGridProperty(_T("SSP Change"));
	pxHeader->AddSubItem(pxiter);

	pxiterContainer->SetTargetProperty(pxiter);			// 함수 수정하고 값 수정
	pxiterContainer->AdjustIteratorProperty();

	_stprintf_s(SP_IterHeader, _T("[%d] G_New_Gradient"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_IterHeader, (variant_t)(int)0,
		_T("SP의 시작부분의 새 gradient의 값"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b1111111111);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_IterHeader, _T("[%d] Q_GDIR"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_IterHeader, (variant_t)(int)0,
		_T("경사 기울기(gradient slope)에 대한 Qualifier"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b1);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	// Gradients Change 시작

	_stprintf_s(SP_IterHeader, _T("[%d] N_ITER"), headerIdxValue);
	pxiterContainer = new CMFCPropertyIteratorSP_gradChg(SP_IterHeader, (variant_t)(int)1, _T("gradient 변경의 개수"));
	pxiterContainer->SetFormat(_T("%d"));
	pxiterContainer->SetMinMaxValue(0, 0b11111);
	pxiterContainer->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxiterContainer);

	pxiter = new CMFCPropertyGridProperty(_T("N_ITER (GRADCHG)"));
	pxHeader->AddSubItem(pxiter);

	pxiterContainer->SetTargetProperty(pxiter);			// 함수 수정하고 값 수정
	pxiterContainer->AdjustIteratorProperty();

	_stprintf_s(SP_IterHeader, _T("[%d] Q_Radius_Category"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_IterHeader, (variant_t)(int)0,
		_T("SP의 시작부분의 Curve category"));
	pxi->SetFormat(_T(""));
	pxi->SetMinMaxValue(0, 0b11111);
	pxi->AddSpecialOption(_T("R>7000m"), _T("%d: R>7000m"));
	pxi->AddSpecialOption(_T("7000m≥R>4500m"), _T("%d: 7000m≥R>4500m"));
	pxi->AddSpecialOption(_T("4500m≥R>2800m"), _T("%d: 4500m≥R>2800m"));
	pxi->AddSpecialOption(_T("2800m≥R>2000m"), _T("%d: 2800m≥R>2000m"));
	pxi->AddSpecialOption(_T("2000m≥R>1500m"), _T("%d: 2000m≥R>1500m"));
	pxi->AddSpecialOption(_T("1500m≥R>1250m"), _T("%d: 1500m≥R>1250m"));
	pxi->AddSpecialOption(_T("1250m≥R>1075m"), _T("%d: 1250m≥R>1075m"));
	pxi->AddSpecialOption(_T("1075m≥R>925m"), _T("%d: 1075m≥R>925m"));
	pxi->AddSpecialOption(_T("925m≥R>825m"), _T("%d: 925m≥R>825m"));
	pxi->AddSpecialOption(_T("825m≥R>725m"), _T("%d: 825m≥R>725m"));
	pxi->AddSpecialOption(_T("725m≥R>625m"), _T("%d: 725m≥R>625m"));
	pxi->AddSpecialOption(_T("625m≥R>525m"), _T("%d: 625m≥R>525m"));
	pxi->AddSpecialOption(_T("525m≥R>475m"), _T("%d: 525m≥R>475m"));
	pxi->AddSpecialOption(_T("475m≥R>425m"), _T("%d: 475m≥R>425m"));
	pxi->AddSpecialOption(_T("425m≥R>375m"), _T("%d: 425m≥R>375m"));
	pxi->AddSpecialOption(_T("375m≥R>325m"), _T("%d: 375m≥R>325m"));
	pxi->AddSpecialOption(_T("325m≥R>300m"), _T("%d: 325m≥R>300m"));
	pxi->AddSpecialOption(_T("300m≥R>275m"), _T("%d: 300m≥R>275m"));
	pxi->AddSpecialOption(_T("275m≥R>250m"), _T("%d: 275m≥R>250m"));
	pxi->AddSpecialOption(_T("250m≥R>225m"), _T("%d: 250m≥R>225m"));
	pxi->AddSpecialOption(_T("225m≥R>200m"), _T("%d: 225m≥R>200m"));
	pxi->AddSpecialOption(_T("200m≥R>175m"), _T("%d: 200m≥R>175m"));
	pxi->AddSpecialOption(_T("175m≥R>150m"), _T("%d: 175m≥R>150m"));
	pxi->AddSpecialOption(_T("R≤150m"), _T("%d: R≤150m"));
	pxi->AllowEdit(FALSE);
	pxHeader->AddSubItem(pxi);

	// Curv change

	_stprintf_s(SP_IterHeader, _T("[%d] N_ITER"), headerIdxValue);
	pxiterContainer = new CMFCPropertyIteratorSP_curvChg(SP_IterHeader, (variant_t)(int)1, _T("curve change의 개수"));
	pxiterContainer->SetFormat(_T("%d"));
	pxiterContainer->SetMinMaxValue(0, 0b11111);
	pxiterContainer->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxiterContainer);

	pxiter = new CMFCPropertyGridProperty(_T("N_ITER (CURVCHG)"));
	pxHeader->AddSubItem(pxiter);

	pxiterContainer->SetTargetProperty(pxiter);			// 함수 수정하고 값 수정
	pxiterContainer->AdjustIteratorProperty();

	_stprintf_s(SP_IterHeader, _T("[%d] M_VOLTAGE"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_IterHeader, (variant_t)(int)0,
		_T("특정 line 혹은 각각 엔진에서 사용할 수 있는 곳에 설치된 traction system 의 전압을 표시합니다"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xf);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_IterHeader, _T("[%d] NID_CTRACTION"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_IterHeader, (variant_t)(int)0,
		_T("M_VOLTAGE 외에 추가적으로 traction system을 완전히 정의하는데 필요한 정보를 식별합니다."));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b1111111111);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	// power voltage change

	_stprintf_s(SP_IterHeader, _T("[%d] N_ITER"), headerIdxValue);
	pxiterContainer = new CMFCPropertyIteratorSP_pvChg(SP_IterHeader, (variant_t)(int)1, _T("Power Voltage change의 개수"));
	pxiterContainer->SetFormat(_T("%d"));
	pxiterContainer->SetMinMaxValue(0, 0b11111);
	pxiterContainer->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxiterContainer);

	pxiter = new CMFCPropertyGridProperty(_T("N_ITER (PVCHG)"));
	pxHeader->AddSubItem(pxiter);

	pxiterContainer->SetTargetProperty(pxiter);			// 함수 수정하고 값 수정
	pxiterContainer->AdjustIteratorProperty();

	_stprintf_s(SP_IterHeader, _T("[%d] M_CURRENT"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_IterHeader, (variant_t)(int)0,
		_T("허가된 전류소비량"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b1111111111);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	// current limitation change

	_stprintf_s(SP_IterHeader, _T("[%d] N_ITER"), headerIdxValue);
	pxiterContainer = new CMFCPropertyIteratorSP_clChg(SP_IterHeader, (variant_t)(int)1, _T("허가전류소비량 변경 개수"));
	pxiterContainer->SetFormat(_T("%d"));
	pxiterContainer->SetMinMaxValue(0, 0b11111);
	pxiterContainer->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxiterContainer);

	pxiter = new CMFCPropertyGridProperty(_T("N_ITER (CLCHG)"));
	pxHeader->AddSubItem(pxiter);

	pxiterContainer->SetTargetProperty(pxiter);			// 함수 수정하고 값 수정
	pxiterContainer->AdjustIteratorProperty();

	// Balise Group

	_stprintf_s(SP_IterHeader, _T("[%d] N_ITER"), headerIdxValue);
	pxiterContainer = new CMFCPropertyIteratorSP_BG(SP_IterHeader, (variant_t)(int)1, _T("balise group의 개수"));
	pxiterContainer->SetFormat(_T("%d"));
	pxiterContainer->SetMinMaxValue(0, 0b11111);
	pxiterContainer->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxiterContainer);

	pxiter = new CMFCPropertyGridProperty(_T("Balise Group"));
	pxHeader->AddSubItem(pxiter);

	pxiterContainer->SetTargetProperty(pxiter);			// 함수 수정하고 값 수정
	pxiterContainer->AdjustIteratorProperty();

	// Timing Point

	_stprintf_s(SP_IterHeader, _T("[%d] N_ITER"), headerIdxValue);
	pxiterContainer = new CMFCPropertyIteratorSP_TP(SP_IterHeader, (variant_t)(int)1, _T("TP의 개수"));
	pxiterContainer->SetFormat(_T("%d"));
	pxiterContainer->SetMinMaxValue(0, 0b11111);
	pxiterContainer->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxiterContainer);

	pxiter = new CMFCPropertyGridProperty(_T("Timing Point"));
	pxHeader->AddSubItem(pxiter);

	pxiterContainer->SetTargetProperty(pxiter);			// 함수 수정하고 값 수정
	pxiterContainer->AdjustIteratorProperty();

	// Platform Area

	_stprintf_s(SP_IterHeader, _T("[%d] N_ITER"), headerIdxValue);
	pxiterContainer = new CMFCPropertyIteratorSP_PA(SP_IterHeader, (variant_t)(int)1, _T("Platform Area의 개수"));
	pxiterContainer->SetFormat(_T("%d"));
	pxiterContainer->SetMinMaxValue(0, 0b11111);
	pxiterContainer->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxiterContainer);

	pxiter = new CMFCPropertyGridProperty(_T("Platform Area"));
	pxHeader->AddSubItem(pxiter);

	pxiterContainer->SetTargetProperty(pxiter);			// 함수 수정하고 값 수정
	pxiterContainer->AdjustIteratorProperty();

	// Tunnel

	_stprintf_s(SP_IterHeader, _T("[%d] N_ITER"), headerIdxValue);
	pxiterContainer = new CMFCPropertyIteratorSP_TN(SP_IterHeader, (variant_t)(int)1, _T("Tunnel의 개수"));
	pxiterContainer->SetFormat(_T("%d"));
	pxiterContainer->SetMinMaxValue(0, 0b11111);
	pxiterContainer->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxiterContainer);

	pxiter = new CMFCPropertyGridProperty(_T("Tunnel"));
	pxHeader->AddSubItem(pxiter);

	pxiterContainer->SetTargetProperty(pxiter);			// 함수 수정하고 값 수정
	pxiterContainer->AdjustIteratorProperty();

	// Axle Load Speed Profile

	_stprintf_s(SP_IterHeader, _T("[%d] N_ITER"), headerIdxValue);
	pxiterContainer = new CMFCPropertyIteratorSP_ALSP(SP_IterHeader, (variant_t)(int)1, _T("Axle Load Speed Profile의 개수"));
	pxiterContainer->SetFormat(_T("%d"));
	pxiterContainer->SetMinMaxValue(0, 0b11111);
	pxiterContainer->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxiterContainer);

	pxiter = new CMFCPropertyGridProperty(_T("N_ITER (ALSP)"));
	pxHeader->AddSubItem(pxiter);

	pxiterContainer->SetTargetProperty(pxiter);			// 함수 수정하고 값 수정
	pxiterContainer->AdjustIteratorProperty();

	// Unprotected Level Crossing Stop

	_stprintf_s(SP_IterHeader, _T("[%d] N_ITER"), headerIdxValue);
	pxiterContainer = new CMFCPropertyIteratorSP_ULCS(SP_IterHeader, (variant_t)(int)1, _T("Unprotected Level Crossing Stop의 개수"));
	pxiterContainer->SetFormat(_T("%d"));
	pxiterContainer->SetMinMaxValue(0, 0b11111);
	pxiterContainer->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxiterContainer);

	pxiter = new CMFCPropertyGridProperty(_T("N_ITER (ULCS)"));
	pxHeader->AddSubItem(pxiter);

	pxiterContainer->SetTargetProperty(pxiter);			// 함수 수정하고 값 수정
	pxiterContainer->AdjustIteratorProperty();

	// Permitted Braking Distance

	_stprintf_s(SP_IterHeader, _T("[%d] N_ITER"), headerIdxValue);
	pxiterContainer = new CMFCPropertyIteratorSP_PBD(SP_IterHeader, (variant_t)(int)1, _T("Permitted Braking Distance Area의 개수"));
	pxiterContainer->SetFormat(_T("%d"));
	pxiterContainer->SetMinMaxValue(0, 0b11111);
	pxiterContainer->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxiterContainer);

	pxiter = new CMFCPropertyGridProperty(_T("N_ITER (PBD)"));
	pxHeader->AddSubItem(pxiter);

	pxiterContainer->SetTargetProperty(pxiter);			// 함수 수정하고 값 수정
	pxiterContainer->AdjustIteratorProperty();

	//// Switch Off Regenerative Brake

	_stprintf_s(SP_IterHeader, _T("[%d] N_ITER"), headerIdxValue);
	pxiterContainer = new CMFCPropertyIteratorSP_SORB(SP_IterHeader, (variant_t)(int)1, _T("Switch off Regenerative Brake areas의 개수"));
	pxiterContainer->SetFormat(_T("%d"));
	pxiterContainer->SetMinMaxValue(0, 0b11111);
	pxiterContainer->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxiterContainer);

	pxiter = new CMFCPropertyGridProperty(_T("N_ITER (SORB)"));
	pxHeader->AddSubItem(pxiter);

	pxiterContainer->SetTargetProperty(pxiter);			// 함수 수정하고 값 수정
	pxiterContainer->AdjustIteratorProperty();


	//// Switch Off Eddy Current Brake

	_stprintf_s(SP_IterHeader, _T("[%d] N_ITER"), headerIdxValue);
	pxiterContainer = new CMFCPropertyIteratorSP_SOECB(SP_IterHeader, (variant_t)(int)1, _T("Switch Off Eddy Current Brake의 개수"));
	pxiterContainer->SetFormat(_T("%d"));
	pxiterContainer->SetMinMaxValue(0, 0b11111);
	pxiterContainer->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxiterContainer);

	pxiter = new CMFCPropertyGridProperty(_T("N_ITER (SOECB)"));
	pxHeader->AddSubItem(pxiter);

	pxiterContainer->SetTargetProperty(pxiter);			// 함수 수정하고 값 수정
	pxiterContainer->AdjustIteratorProperty();



	//// Switch Off Eddy Current Emergency Brake

	_stprintf_s(SP_IterHeader, _T("[%d] N_ITER"), headerIdxValue);
	pxiterContainer = new CMFCPropertyIteratorSP_SOECEB(SP_IterHeader, (variant_t)(int)1, _T("Switch Off Eddy Current Emergency Brake areas의 개수"));
	pxiterContainer->SetFormat(_T("%d"));
	pxiterContainer->SetMinMaxValue(0, 0b11111);
	pxiterContainer->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxiterContainer);

	pxiter = new CMFCPropertyGridProperty(_T("N_ITER (SOECEB)"));
	pxHeader->AddSubItem(pxiter);

	pxiterContainer->SetTargetProperty(pxiter);			// 함수 수정하고 값 수정
	pxiterContainer->AdjustIteratorProperty();

	//// Switch Off Magnetic Shoe Brake

	_stprintf_s(SP_IterHeader, _T("[%d] N_ITER"), headerIdxValue);
	pxiterContainer = new CMFCPropertyIteratorSP_SOMSB(SP_IterHeader, (variant_t)(int)1, _T("Switch Off Magnetic Shoe Brake areas의 개수"));
	pxiterContainer->SetFormat(_T("%d"));
	pxiterContainer->SetMinMaxValue(0, 0b11111);
	pxiterContainer->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxiterContainer);

	pxiter = new CMFCPropertyGridProperty(_T("N_ITER (SOMSB)"));
	pxHeader->AddSubItem(pxiter);

	pxiterContainer->SetTargetProperty(pxiter);			// 함수 수정하고 값 수정
	pxiterContainer->AdjustIteratorProperty();


	//// Area with Dynamic Brake force Inhibition

	_stprintf_s(SP_IterHeader, _T("[%d] N_ITER"), headerIdxValue);
	pxiterContainer = new CMFCPropertyIteratorSP_ADBFI(SP_IterHeader, (variant_t)(int)1, _T("Area with Dynamic Brake force Inhibition의 개수"));
	pxiterContainer->SetFormat(_T("%d"));
	pxiterContainer->SetMinMaxValue(0, 0b11111);
	pxiterContainer->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxiterContainer);

	pxiter = new CMFCPropertyGridProperty(_T("N_ITER (ADBFI)"));
	pxHeader->AddSubItem(pxiter);

	pxiterContainer->SetTargetProperty(pxiter);			// 함수 수정하고 값 수정
	pxiterContainer->AdjustIteratorProperty();

	//// Area with limited Dynamic Brake force

	_stprintf_s(SP_IterHeader, _T("[%d] N_ITER"), headerIdxValue);
	pxiterContainer = new CMFCPropertyIteratorSP_ALDBF(SP_IterHeader, (variant_t)(int)1, _T("Area with limited Dynamic Brake force의 개수"));
	pxiterContainer->SetFormat(_T("%d"));
	pxiterContainer->SetMinMaxValue(0, 0b11111);
	pxiterContainer->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxiterContainer);

	pxiter = new CMFCPropertyGridProperty(_T("N_ITER (ALDBF)"));
	pxHeader->AddSubItem(pxiter);

	pxiterContainer->SetTargetProperty(pxiter);			// 함수 수정하고 값 수정
	pxiterContainer->AdjustIteratorProperty();
}

CMFCPropertyIteratorSP_SSP::CMFCPropertyIteratorSP_SSP(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList) : CMFCPropertyIterator(strGroupName, dwData, bIsValueList)
{
}

CMFCPropertyIteratorSP_SSP::CMFCPropertyIteratorSP_SSP(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars) : CMFCPropertyIterator(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)

{
}

CMFCPropertyIteratorSP_SSP::~CMFCPropertyIteratorSP_SSP()
{
}

void CMFCPropertyIteratorSP_SSP::AddIteratorProperty(int nextIdx)
{
	assert(targetProperty != nullptr);

	TCHAR idxStr[10];
	_stprintf_s(idxStr, _T("[%d]"), nextIdx);
	CMFCPropertyGridProperty* pxHeader = new CMFCPropertyGridProperty(idxStr); // 개별 Iterator를 담는 프로퍼티
	targetProperty->AddSubItem(pxHeader);

	int headerIdxValue = nextIdx;
	CMFCPropertyEX<int>* pxi;
	//CMFCPropertyGridProperty* pxProperty;
	

	TCHAR SP_SSPIterHeader[30];

	_stprintf_s(SP_SSPIterHeader, _T("[%d] Q_DIFF"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_SSPIterHeader, (variant_t)(int)0,
		_T("특별 SSP 범주의 종류"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b11);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_SSPIterHeader, _T("[%d] NC_CDDIFF"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_SSPIterHeader, (variant_t)(int)0,
		_T("정적 line speed에 대한 다른 값이 존재하는 “Cant 불충분” SSP category"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xf);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);
		
	_stprintf_s(SP_SSPIterHeader, _T("[%d] NC_DIFF"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_SSPIterHeader, (variant_t)(int)0,
		_T("static line speed에 대한 다른 값이 존재하는 “other specific” SSP category"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xf);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_SSPIterHeader, _T("[%d] V_DIFF"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_SSPIterHeader, (variant_t)(int)0,
		_T("train category와 관련된 Absolute Positive Speed"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b1111111);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);



}

CMFCPropertyIteratorSP_SSPCHG::CMFCPropertyIteratorSP_SSPCHG(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList) : CMFCPropertyIterator(strGroupName, dwData, bIsValueList)
{
}

CMFCPropertyIteratorSP_SSPCHG::CMFCPropertyIteratorSP_SSPCHG(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars) : CMFCPropertyIterator(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{
}

CMFCPropertyIteratorSP_SSPCHG::~CMFCPropertyIteratorSP_SSPCHG()
{
}

void CMFCPropertyIteratorSP_SSPCHG::AddIteratorProperty(int nextIdx)
{
	assert(targetProperty != nullptr);

	TCHAR idxStr[10];
	_stprintf_s(idxStr, _T("[%d]"), nextIdx);
	CMFCPropertyGridProperty* pxHeader = new CMFCPropertyGridProperty(idxStr); // 개별 Iterator를 담는 프로퍼티
	targetProperty->AddSubItem(pxHeader);

	int headerIdxValue = nextIdx;
	CMFCPropertyEX<int>* pxi;
	//CMFCPropertyGridProperty* pxProperty;
	CMFCPropertyIterator* pxiterContainer;
	CMFCPropertyGridProperty* pxiter;

	TCHAR SP_SSPCHGIterHeader[30];

	_stprintf_s(SP_SSPCHGIterHeader, _T("[%d] D_Location"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_SSPCHGIterHeader, (variant_t)(int)0,
		_T("SP의 시작점에 상대적인 Static Speed Profile 변경의 위치"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xffffff);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_SSPCHGIterHeader, _T("[%d] V_STATIC"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_SSPCHGIterHeader, (variant_t)(int)0,
		_T("Basic Static Speed Profile speed"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b1111111);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_SSPCHGIterHeader, _T("[%d] Q_FRONT"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_SSPCHGIterHeader, (variant_t)(int)0,
		_T("프로필 요소에 대해 주어진 속도제한이 열차의 앞부분 혹은 뒷부분이 해당 요소를 떠날때까지 적용되어야 하는지에 대한 표시하기 위한 Qualifier"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b1);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_SSPCHGIterHeader, _T("[%d] N_ITER"), headerIdxValue);
	pxiterContainer = new CMFCPropertyIteratorSP_SSPCHG_SSSP(SP_SSPCHGIterHeader, (variant_t)(int)1, _T("Specific SSP Change의 개수"));
	pxiterContainer->SetFormat(_T("%d"));
	pxiterContainer->SetMinMaxValue(0, 0b11111);
	pxiterContainer->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxiterContainer);

	pxiter = new CMFCPropertyGridProperty(_T("SSSP"));
	pxHeader->AddSubItem(pxiter);

	pxiterContainer->SetTargetProperty(pxiter);			// 함수 수정하고 값 수정
	pxiterContainer->AdjustIteratorProperty();
	
}

CMFCPropertyIteratorSP_SSPCHG_SSSP::CMFCPropertyIteratorSP_SSPCHG_SSSP(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList) : CMFCPropertyIterator(strGroupName, dwData, bIsValueList)
{
}

CMFCPropertyIteratorSP_SSPCHG_SSSP::CMFCPropertyIteratorSP_SSPCHG_SSSP(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars) : CMFCPropertyIterator(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{
}

CMFCPropertyIteratorSP_SSPCHG_SSSP::~CMFCPropertyIteratorSP_SSPCHG_SSSP()
{
}

void CMFCPropertyIteratorSP_SSPCHG_SSSP::AddIteratorProperty(int nextIdx)
{
	assert(targetProperty != nullptr);

	TCHAR idxStr[10];
	_stprintf_s(idxStr, _T("[%d]"), nextIdx);
	CMFCPropertyGridProperty* pxHeader = new CMFCPropertyGridProperty(idxStr); // 개별 Iterator를 담는 프로퍼티
	targetProperty->AddSubItem(pxHeader);

	int headerIdxValue = nextIdx;
	CMFCPropertyEX<int>* pxi;

	TCHAR SP_SSPCHG_SSSPIterHeader[30];

	_stprintf_s(SP_SSPCHG_SSSPIterHeader, _T("[%d] Q_DIFF"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_SSPCHG_SSSPIterHeader, (variant_t)(int)0,
		_T("특별 SSP 범주의 종류"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b11);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_SSPCHG_SSSPIterHeader, _T("[%d] NC_CDDIFF"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_SSPCHG_SSSPIterHeader, (variant_t)(int)0,
		_T("정적 line speed에 대한 다른 값이 존재하는 “Cant 불충분” SSP category"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xf);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_SSPCHG_SSSPIterHeader, _T("[%d] NC_DIFF"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_SSPCHG_SSSPIterHeader, (variant_t)(int)0,
		_T("static line speed에 대한 다른 값이 존재하는 “other specific” SSP category"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xf);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_SSPCHG_SSSPIterHeader, _T("[%d] V_DIFF"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_SSPCHG_SSSPIterHeader, (variant_t)(int)0,
		_T("train category와 관련된 Absolute Positive Speed"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xf);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);
}

CMFCPropertyIteratorSP_gradChg::CMFCPropertyIteratorSP_gradChg(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList) : CMFCPropertyIterator(strGroupName, dwData, bIsValueList)
{
}

CMFCPropertyIteratorSP_gradChg::CMFCPropertyIteratorSP_gradChg(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars) : CMFCPropertyIterator(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{
}

CMFCPropertyIteratorSP_gradChg::~CMFCPropertyIteratorSP_gradChg()
{
}

void CMFCPropertyIteratorSP_gradChg::AddIteratorProperty(int nextIdx)
{
	assert(targetProperty != nullptr);

	TCHAR idxStr[10];
	_stprintf_s(idxStr, _T("[%d]"), nextIdx);
	CMFCPropertyGridProperty* pxHeader = new CMFCPropertyGridProperty(idxStr); // 개별 Iterator를 담는 프로퍼티
	targetProperty->AddSubItem(pxHeader);

	int headerIdxValue = nextIdx;
	CMFCPropertyEX<int>* pxi;

	TCHAR SP_GRADCHG[30];

	_stprintf_s(SP_GRADCHG, _T("[%d] D_Location"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_GRADCHG, (variant_t)(int)0,
		_T("SP의 시작부분에 상대적인 gradient변경의 위치"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xffffff);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_GRADCHG, _T("[%d] G_New_Gradient"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_GRADCHG, (variant_t)(int)0,
		_T("새 gradient의 값"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b111111111);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_GRADCHG, _T("[%d] Q_GDIR"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_GRADCHG, (variant_t)(int)0,
		_T("gradient 방향 Qualifier"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b1);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);
}

CMFCPropertyIteratorSP_curvChg::CMFCPropertyIteratorSP_curvChg(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList) : CMFCPropertyIterator(strGroupName, dwData, bIsValueList)
{
}

CMFCPropertyIteratorSP_curvChg::CMFCPropertyIteratorSP_curvChg(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars) : CMFCPropertyIterator(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{
}

CMFCPropertyIteratorSP_curvChg::~CMFCPropertyIteratorSP_curvChg()
{
}

void CMFCPropertyIteratorSP_curvChg::AddIteratorProperty(int nextIdx)
{
	assert(targetProperty != nullptr);

	TCHAR idxStr[10];
	_stprintf_s(idxStr, _T("[%d]"), nextIdx);
	CMFCPropertyGridProperty* pxHeader = new CMFCPropertyGridProperty(idxStr); // 개별 Iterator를 담는 프로퍼티
	targetProperty->AddSubItem(pxHeader);


	int headerIdxValue = nextIdx;
	CMFCPropertyEX<int>* pxi;

	TCHAR SP_CURVCHG[30];

	_stprintf_s(SP_CURVCHG, _T("[%d] D_Location"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_CURVCHG, (variant_t)(int)0,
		_T("SP의 시작부분에 상대적인 curve change의 위치"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xffffff);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_CURVCHG, _T("[%d] Q_Radius_Category"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_CURVCHG, (variant_t)(int)0,
		_T("Curve Category"));
	pxi->SetFormat(_T(""));
	pxi->SetMinMaxValue(0, 0b11111);
	pxi->AddSpecialOption(_T("R>7000m"), _T("%d: R>7000m"));
	pxi->AddSpecialOption(_T("7000m≥R>4500m"), _T("%d: 7000m≥R>4500m"));
	pxi->AddSpecialOption(_T("4500m≥R>2800m"), _T("%d: 4500m≥R>2800m"));
	pxi->AddSpecialOption(_T("2800m≥R>2000m"), _T("%d: 2800m≥R>2000m"));
	pxi->AddSpecialOption(_T("2000m≥R>1500m"), _T("%d: 2000m≥R>1500m"));
	pxi->AddSpecialOption(_T("1500m≥R>1250m"), _T("%d: 1500m≥R>1250m"));
	pxi->AddSpecialOption(_T("1250m≥R>1075m"), _T("%d: 1250m≥R>1075m"));
	pxi->AddSpecialOption(_T("1075m≥R>925m"), _T("%d: 1075m≥R>925m"));
	pxi->AddSpecialOption(_T("925m≥R>825m"), _T("%d: 925m≥R>825m"));
	pxi->AddSpecialOption(_T("825m≥R>725m"), _T("%d: 825m≥R>725m"));
	pxi->AddSpecialOption(_T("725m≥R>625m"), _T("%d: 725m≥R>625m"));
	pxi->AddSpecialOption(_T("625m≥R>525m"), _T("%d: 625m≥R>525m"));
	pxi->AddSpecialOption(_T("525m≥R>475m"), _T("%d: 525m≥R>475m"));
	pxi->AddSpecialOption(_T("475m≥R>425m"), _T("%d: 475m≥R>425m"));
	pxi->AddSpecialOption(_T("425m≥R>375m"), _T("%d: 425m≥R>375m"));
	pxi->AddSpecialOption(_T("375m≥R>325m"), _T("%d: 375m≥R>325m"));
	pxi->AddSpecialOption(_T("325m≥R>300m"), _T("%d: 325m≥R>300m"));
	pxi->AddSpecialOption(_T("300m≥R>275m"), _T("%d: 300m≥R>275m"));
	pxi->AddSpecialOption(_T("275m≥R>250m"), _T("%d: 275m≥R>250m"));
	pxi->AddSpecialOption(_T("250m≥R>225m"), _T("%d: 250m≥R>225m"));
	pxi->AddSpecialOption(_T("225m≥R>200m"), _T("%d: 225m≥R>200m"));
	pxi->AddSpecialOption(_T("200m≥R>175m"), _T("%d: 200m≥R>175m"));
	pxi->AddSpecialOption(_T("175m≥R>150m"), _T("%d: 175m≥R>150m"));
	pxi->AddSpecialOption(_T("R≤150m"), _T("%d: R≤150m"));
	pxi->AllowEdit(FALSE);
	pxHeader->AddSubItem(pxi);
}

CMFCPropertyIteratorSP_pvChg::CMFCPropertyIteratorSP_pvChg(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList) : CMFCPropertyIterator(strGroupName, dwData, bIsValueList)
{
}

CMFCPropertyIteratorSP_pvChg::CMFCPropertyIteratorSP_pvChg(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars) : CMFCPropertyIterator(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{
}

CMFCPropertyIteratorSP_pvChg::~CMFCPropertyIteratorSP_pvChg()
{
}

void CMFCPropertyIteratorSP_pvChg::AddIteratorProperty(int nextIdx)
{
	assert(targetProperty != nullptr);

	TCHAR idxStr[10];
	_stprintf_s(idxStr, _T("[%d]"), nextIdx);
	CMFCPropertyGridProperty* pxHeader = new CMFCPropertyGridProperty(idxStr); // 개별 Iterator를 담는 프로퍼티
	targetProperty->AddSubItem(pxHeader);

	int headerIdxValue = nextIdx;
	CMFCPropertyEX<int>* pxi;

	TCHAR SP_PVCHG[30];

	_stprintf_s(SP_PVCHG, _T("[%d] D_Location"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_PVCHG, (variant_t)(int)0,
		_T("SP의 시작부분에 상대적인 전압 변경의 위치"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xffffff);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_PVCHG, _T("[%d] M_VOLTAGE"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_PVCHG, (variant_t)(int)0,
		_T("SP의 시작부분에 상대적인 전압 변경의 위치"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xf);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_PVCHG, _T("[%d] NID_CTRACTION"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_PVCHG, (variant_t)(int)0,
		_T("M_VOLTAGE 외에 추가적으로 traction system을 완전히 정의하는데 필요한 정보를 식별합니다"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b1111111111);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);
}

CMFCPropertyIteratorSP_clChg::CMFCPropertyIteratorSP_clChg(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList) : CMFCPropertyIterator(strGroupName, dwData, bIsValueList)
{
}

CMFCPropertyIteratorSP_clChg::CMFCPropertyIteratorSP_clChg(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars) : CMFCPropertyIterator(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{
}

CMFCPropertyIteratorSP_clChg::~CMFCPropertyIteratorSP_clChg()
{
}

void CMFCPropertyIteratorSP_clChg::AddIteratorProperty(int nextIdx)
{
	assert(targetProperty != nullptr);

	TCHAR idxStr[10];
	_stprintf_s(idxStr, _T("[%d]"), nextIdx);
	CMFCPropertyGridProperty* pxHeader = new CMFCPropertyGridProperty(idxStr); // 개별 Iterator를 담는 프로퍼티
	targetProperty->AddSubItem(pxHeader);

	int headerIdxValue = nextIdx;
	CMFCPropertyEX<int>* pxi;

	TCHAR CLCHGIterHeader[30];

	_stprintf_s(CLCHGIterHeader, _T("[%d] D_Location"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(CLCHGIterHeader, (variant_t)(int)0,
		_T("SP의 시작부분에 상대적인 허가전류소비량 변경의 위치"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xffffff);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(CLCHGIterHeader, _T("[%d] M_CURRENT"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(CLCHGIterHeader, (variant_t)(int)0,
		_T("허가된 전류소비량"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b1111111111);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);
}

CMFCPropertyIteratorSP_BG::CMFCPropertyIteratorSP_BG(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList) : CMFCPropertyIterator(strGroupName, dwData, bIsValueList)
{
}

CMFCPropertyIteratorSP_BG::CMFCPropertyIteratorSP_BG(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars) : CMFCPropertyIterator(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{
}

CMFCPropertyIteratorSP_BG::~CMFCPropertyIteratorSP_BG()
{
}

void CMFCPropertyIteratorSP_BG::AddIteratorProperty(int nextIdx)
{
	assert(targetProperty != nullptr);

	TCHAR idxStr[10];
	_stprintf_s(idxStr, _T("[%d]"), nextIdx);
	CMFCPropertyGridProperty* pxHeader = new CMFCPropertyGridProperty(idxStr); // 개별 Iterator를 담는 프로퍼티
	targetProperty->AddSubItem(pxHeader);

	int headerIdxValue = nextIdx;
	CMFCPropertyEX<int>* pxi;
	//CMFCPropertyGridProperty* pxProperty;
	CMFCPropertyIterator* pxiterContainer;
	CMFCPropertyGridProperty* pxiter;

	TCHAR SPBGIterHeader[30];

	_stprintf_s(SPBGIterHeader, _T("[%d] Q_NEWNID_C"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SPBGIterHeader, (variant_t)(int)0,
		_T("이 발리스그룹의 NID_C가 뒤따르는 항목의 값인지 혹은 SP의 NID_C가 이 발리스그룹에서 유효한지를 가리키는 식별자"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b1);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SPBGIterHeader, _T("[%d] NID_C"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SPBGIterHeader, (variant_t)(int)887,
		_T("BG의 나라 혹은 지역의 ID"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b1111111111);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SPBGIterHeader, _T("[%d] NID_BG"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SPBGIterHeader, (variant_t)(int)0,
		_T("발리스 그룹 ID"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b11111111111111);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SPBGIterHeader, _T("[%d] N_ITER_BG"), headerIdxValue);
	pxiterContainer = new CMFCPropertyIteratorSP_BG_BAL(SPBGIterHeader, (variant_t)(int)1, _T("k번째 SP에 위치해있는 발리스그룹 내부의 발리스의 개수"));
	pxiterContainer->SetFormat(_T("%d"));
	pxiterContainer->SetMinMaxValue(0, 0b111);
	pxiterContainer->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxiterContainer);

	pxiter = new CMFCPropertyGridProperty(_T("Balises"));
	pxHeader->AddSubItem(pxiter);

	pxiterContainer->SetTargetProperty(pxiter);			// 함수 수정하고 값 수정
	pxiterContainer->AdjustIteratorProperty();
}

CMFCPropertyIteratorSP_BG_BAL::CMFCPropertyIteratorSP_BG_BAL(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList) : CMFCPropertyIterator(strGroupName, dwData, bIsValueList)
{
}

CMFCPropertyIteratorSP_BG_BAL::CMFCPropertyIteratorSP_BG_BAL(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars) : CMFCPropertyIterator(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{
}

CMFCPropertyIteratorSP_BG_BAL::~CMFCPropertyIteratorSP_BG_BAL()
{
}

void CMFCPropertyIteratorSP_BG_BAL::AddIteratorProperty(int nextIdx)
{
	assert(targetProperty != nullptr);

	TCHAR idxStr[10];
	_stprintf_s(idxStr, _T("[%d]"), nextIdx);
	CMFCPropertyGridProperty* pxHeader = new CMFCPropertyGridProperty(idxStr); // 개별 Iterator를 담는 프로퍼티
	targetProperty->AddSubItem(pxHeader);

	int headerIdxValue = nextIdx;
	CMFCPropertyEX<int>* pxi;

	TCHAR SPBGBALIterHeader[30];

	_stprintf_s(SPBGBALIterHeader, _T("[%d] N_PIG"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SPBGBALIterHeader, (variant_t)(int)0,
		_T("발리스그룹내의 상대적인 위치"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b111);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SPBGBALIterHeader, _T("[%d] D_Location"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SPBGBALIterHeader, (variant_t)(int)0,
		_T("SP의 시작점에 상대적인 발리스의 위치"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xffffff);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);
}

CMFCPropertyIteratorSP_TP::CMFCPropertyIteratorSP_TP(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList) : CMFCPropertyIterator(strGroupName, dwData, bIsValueList)
{
}

CMFCPropertyIteratorSP_TP::CMFCPropertyIteratorSP_TP(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars) : CMFCPropertyIterator(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{
}

CMFCPropertyIteratorSP_TP::~CMFCPropertyIteratorSP_TP()
{
}

void CMFCPropertyIteratorSP_TP::AddIteratorProperty(int nextIdx)
{
	assert(targetProperty != nullptr);

	TCHAR idxStr[10];
	_stprintf_s(idxStr, _T("[%d]"), nextIdx);
	CMFCPropertyGridProperty* pxHeader = new CMFCPropertyGridProperty(idxStr); // 개별 Iterator를 담는 프로퍼티
	targetProperty->AddSubItem(pxHeader);

	int headerIdxValue = nextIdx;
	CMFCPropertyEX<int>* pxi;
	CMFCPropertyEX<UINT32>* pxu;
	//CMFCPropertyGridProperty* pxProperty;
	CMFCPropertyIterator* pxiterContainer;
	CMFCPropertyGridProperty* pxiter;

	TCHAR SPTPIterHeader[30];

	_stprintf_s(SPTPIterHeader, _T("[%d] NID_TP"), headerIdxValue);
	pxu = new CMFCPropertyEX<UINT32>(SPTPIterHeader, (variant_t)(UINT32)0,
		_T("TP ID. NID_TP는 NID_C에서 고유합니다"));
	pxu->SetFormat(_T("%u"));
	pxu->SetMinMaxValue(0, UINT32_MAX);
	pxu->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxu);

	_stprintf_s(SPTPIterHeader, _T("[%d] D_Location"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SPTPIterHeader, (variant_t)(int)0,
		_T("SP의 시작지점에 상대적인 TP 위치"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xffffff);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SPTPIterHeader, _T("[%d] Q_Stop_Location_Tolerance"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SPTPIterHeader, (variant_t)(int)0,
		_T("TP가 Stopping Point일때 사용하기 위한 요구된 정지 허용범위"));
	pxi->SetFormat(_T(""));
	pxi->SetMinMaxValue(0, 0b11111);
	pxi->AddSpecialOption(_T("10cm"), _T("%d: 10cm"));
	pxi->AddSpecialOption(_T("20cm"), _T("%d: 20cm"));
	pxi->AddSpecialOption(_T("30cm"), _T("%d: 30cm"));
	pxi->AddSpecialOption(_T("40cm"), _T("%d: 40cm"));
	pxi->AddSpecialOption(_T("50cm"), _T("%d: 50cm"));
	pxi->AddSpecialOption(_T("1m"), _T("%d: 1m"));
	pxi->AddSpecialOption(_T("1,5m"), _T("%d: 1,5m"));
	pxi->AddSpecialOption(_T("2m"), _T("%d: 2m"));
	pxi->AddSpecialOption(_T("2.5m"), _T("%d: 2.5m"));
	pxi->AddSpecialOption(_T("3m"), _T("%d: 3m"));
	pxi->AddSpecialOption(_T("5m"), _T("%d: 5m"));
	pxi->AddSpecialOption(_T("7,5m"), _T("%d: 7,5m"));
	pxi->AddSpecialOption(_T("10m"), _T("%d: 10m"));
	pxi->AddSpecialOption(_T("15m"), _T("%d: 15m"));
	pxi->AddSpecialOption(_T("20m"), _T("%d: 20m"));
	pxi->AddSpecialOption(_T("25m"), _T("%d: 25m"));
	pxi->AddSpecialOption(_T("30m"), _T("%d: 30m"));
	pxi->AddSpecialOption(_T("50m"), _T("%d: 50m"));
	pxi->AddSpecialOption(_T("75m"), _T("%d: 75m"));
	pxi->AddSpecialOption(_T("100m"), _T("%d: 100m"));
	pxi->AddSpecialOption(_T("No requirement"), _T("31: No requirement"), 31);
	pxi->AllowEdit(FALSE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SPTPIterHeader, _T("[%d] Q_STP_Reached"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SPTPIterHeader, (variant_t)(int)0,
		_T("도달한 것으로 간주하는 Stopping Point로부터의 거리"));
	pxi->SetFormat(_T(""));
	pxi->SetMinMaxValue(0, 0b11111);
	pxi->AddSpecialOption(_T("10cm"), _T("%d: 10cm"));
	pxi->AddSpecialOption(_T("20cm"), _T("%d: 20cm"));
	pxi->AddSpecialOption(_T("30cm"), _T("%d: 30cm"));
	pxi->AddSpecialOption(_T("40cm"), _T("%d: 40cm"));
	pxi->AddSpecialOption(_T("50cm"), _T("%d: 50cm"));
	pxi->AddSpecialOption(_T("1m"), _T("%d: 1m"));
	pxi->AddSpecialOption(_T("1,5m"), _T("%d: 1,5m"));
	pxi->AddSpecialOption(_T("2m"), _T("%d: 2m"));
	pxi->AddSpecialOption(_T("2.5m"), _T("%d: 2.5m"));
	pxi->AddSpecialOption(_T("3m"), _T("%d: 3m"));
	pxi->AddSpecialOption(_T("5m"), _T("%d: 5m"));
	pxi->AddSpecialOption(_T("7,5m"), _T("%d: 7,5m"));
	pxi->AddSpecialOption(_T("10m"), _T("%d: 10m"));
	pxi->AddSpecialOption(_T("15m"), _T("%d: 15m"));
	pxi->AddSpecialOption(_T("20m"), _T("%d: 20m"));
	pxi->AddSpecialOption(_T("25m"), _T("%d: 25m"));
	pxi->AddSpecialOption(_T("30m"), _T("%d: 30m"));
	pxi->AddSpecialOption(_T("50m"), _T("%d: 50m"));
	pxi->AddSpecialOption(_T("75m"), _T("%d: 75m"));
	pxi->AddSpecialOption(_T("100m"), _T("%d: 100m"));
	pxi->AddSpecialOption(_T("No requirement"), _T("31: No requirement"), 31);
	pxi->AllowEdit(FALSE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SPTPIterHeader, _T("[%d] L_TEXT"), headerIdxValue);
	pxiterContainer = new CMFCPropertyIteratorSP_TP_NM(SPTPIterHeader, (variant_t)(int)1, _T("L_TEXT"));
	pxiterContainer->SetFormat(_T("%d"));
	pxiterContainer->SetMinMaxValue(0, UINT8_MAX);
	pxiterContainer->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxiterContainer);

	pxiter = new CMFCPropertyGridProperty(_T("TP NAME"));
	pxHeader->AddSubItem(pxiter);

	pxiterContainer->SetTargetProperty(pxiter);			// 함수 수정하고 값 수정
	pxiterContainer->AdjustIteratorProperty();
}

CMFCPropertyIteratorSP_TP_NM::CMFCPropertyIteratorSP_TP_NM(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList) : CMFCPropertyIterator(strGroupName, dwData, bIsValueList)
{
}

CMFCPropertyIteratorSP_TP_NM::CMFCPropertyIteratorSP_TP_NM(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars) : CMFCPropertyIterator(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{
}

CMFCPropertyIteratorSP_TP_NM::~CMFCPropertyIteratorSP_TP_NM()
{
}

void CMFCPropertyIteratorSP_TP_NM::AddIteratorProperty(int nextIdx)
{
	assert(targetProperty != nullptr);

	TCHAR idxStr[10];
	_stprintf_s(idxStr, _T("[%d]"), nextIdx);
	CMFCPropertyGridProperty* pxHeader = new CMFCPropertyGridProperty(idxStr); // 개별 Iterator를 담는 프로퍼티
	targetProperty->AddSubItem(pxHeader);

	int headerIdxValue = nextIdx;
	CMFCPropertyEX<int>* pxi;

	TCHAR SPTPNMIterHeader[30];

	_stprintf_s(SPTPNMIterHeader, _T("[%d] X_TEXT"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SPTPNMIterHeader, (variant_t)(int)0,
		_T("TP 이름"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, UINT8_MAX);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);
}

CMFCPropertyIteratorSP_PA::CMFCPropertyIteratorSP_PA(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList) : CMFCPropertyIterator(strGroupName, dwData, bIsValueList)
{
}

CMFCPropertyIteratorSP_PA::CMFCPropertyIteratorSP_PA(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars) : CMFCPropertyIterator(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{
}

CMFCPropertyIteratorSP_PA::~CMFCPropertyIteratorSP_PA()
{
}

void CMFCPropertyIteratorSP_PA::AddIteratorProperty(int nextIdx)
{
	assert(targetProperty != nullptr);

	TCHAR idxStr[10];
	_stprintf_s(idxStr, _T("[%d]"), nextIdx);
	CMFCPropertyGridProperty* pxHeader = new CMFCPropertyGridProperty(idxStr); // 개별 Iterator를 담는 프로퍼티
	targetProperty->AddSubItem(pxHeader);

	int headerIdxValue = nextIdx;
	CMFCPropertyEX<int>* pxi;

	TCHAR SPPAIterHeader[30];

	_stprintf_s(SPPAIterHeader, _T("[%d] Q_Range"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SPPAIterHeader, (variant_t)(int)0,
		_T("Platform Area가 시작하는지 끝나는지, 둘 다인지, 해당SP를 다 포함하는지 명시합니다"));
	pxi->SetFormat(_T(""));
	pxi->SetMinMaxValue(0, 0b11);
	pxi->AddSpecialOption(_T("Starts"), _T("%d: Starts"));
	pxi->AddSpecialOption(_T("Ends"), _T("%d: Ends"));
	pxi->AddSpecialOption(_T("StartsEnds"), _T("%d: StartsEnds"));
	pxi->AddSpecialOption(_T("WholeSP"), _T("%d: WholeSP"));
	pxi->AllowEdit(FALSE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SPPAIterHeader, _T("[%d] D_Start_Location"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SPPAIterHeader, (variant_t)(int)0,
		_T("SP의 시작부분에 상대적인 platform start의 위치"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xffffff);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SPPAIterHeader, _T("[%d] D_End_Location"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SPPAIterHeader, (variant_t)(int)0,
		_T("SP의 시작부분에 상대적인 platform end의 위치"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xffffff);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);
}

CMFCPropertyIteratorSP_TN::CMFCPropertyIteratorSP_TN(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList) : CMFCPropertyIterator(strGroupName, dwData, bIsValueList)
{
}

CMFCPropertyIteratorSP_TN::CMFCPropertyIteratorSP_TN(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars) : CMFCPropertyIterator(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{
}

CMFCPropertyIteratorSP_TN::~CMFCPropertyIteratorSP_TN()
{
}

void CMFCPropertyIteratorSP_TN::AddIteratorProperty(int nextIdx)
{
	assert(targetProperty != nullptr);

	TCHAR idxStr[10];
	_stprintf_s(idxStr, _T("[%d]"), nextIdx);
	CMFCPropertyGridProperty* pxHeader = new CMFCPropertyGridProperty(idxStr); // 개별 Iterator를 담는 프로퍼티
	targetProperty->AddSubItem(pxHeader);

	int headerIdxValue = nextIdx;
	CMFCPropertyEX<int>* pxi;

	TCHAR SPTNIterHeader[30];

	_stprintf_s(SPTNIterHeader, _T("[%d] Q_Range"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SPTNIterHeader, (variant_t)(int)0,
		_T("Tunnel이 시작하는지 끝나는지, 둘 다 인지, 해당 SP를 다 포함하는지 명시합니다"));
	pxi->SetFormat(_T(""));
	pxi->SetMinMaxValue(0, 0b11);
	pxi->AddSpecialOption(_T("Starts"), _T("%d: Starts"));
	pxi->AddSpecialOption(_T("Ends"), _T("%d: Ends"));
	pxi->AddSpecialOption(_T("StartsEnds"), _T("%d: StartsEnds"));
	pxi->AddSpecialOption(_T("WholeSP"), _T("%d: WholeSP"));
	pxi->AllowEdit(FALSE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SPTNIterHeader, _T("[%d] Q_Tunnel_Category"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SPTNIterHeader, (variant_t)(int)0,
		_T("Tunnel의 category"));
	pxi->SetFormat(_T(""));
	pxi->SetMinMaxValue(0, 0b11);
	pxi->AddSpecialOption(_T("Single track tunnel"), _T("%d: Single track tunnel"));
	pxi->AddSpecialOption(_T("Double track tunnel"), _T("%d: Double track tunnel"));
	pxi->AddSpecialOption(_T("Wide-cross section tunnel"), _T("%d: Wide-cross section tunnel"));
	pxi->AllowEdit(FALSE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SPTNIterHeader, _T("[%d] D_Start_Location"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SPTNIterHeader, (variant_t)(int)0,
		_T("SP의 시작부분에 상대적인 tunnel start의 위치"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xffffff);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SPTNIterHeader, _T("[%d] D_End_Location"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SPTNIterHeader, (variant_t)(int)0,
		_T("SP의 시작부분에 상대적인 tunnel end의 위치"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xffffff);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);
}

CMFCPropertyIteratorSP_ALSP::CMFCPropertyIteratorSP_ALSP(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList) : CMFCPropertyIterator(strGroupName, dwData, bIsValueList)
{
}

CMFCPropertyIteratorSP_ALSP::CMFCPropertyIteratorSP_ALSP(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars) : CMFCPropertyIterator(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{
}

CMFCPropertyIteratorSP_ALSP::~CMFCPropertyIteratorSP_ALSP()
{
}

void CMFCPropertyIteratorSP_ALSP::AddIteratorProperty(int nextIdx)
{
	assert(targetProperty != nullptr);

	TCHAR idxStr[10];
	_stprintf_s(idxStr, _T("[%d]"), nextIdx);
	CMFCPropertyGridProperty* pxHeader = new CMFCPropertyGridProperty(idxStr); // 개별 Iterator를 담는 프로퍼티
	targetProperty->AddSubItem(pxHeader);

	int headerIdxValue = nextIdx;
	CMFCPropertyEX<int>* pxi;

	TCHAR SP_ALSPIterHeader[30];

	_stprintf_s(SP_ALSPIterHeader, _T("[%d] Q_Range"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_ALSPIterHeader, (variant_t)(int)0,
		_T("Axle Load Speed Profile이 시작하는지 끝나는지 둘 다인지 SP를 다 포함하는지 명시합니다"));
	pxi->SetFormat(_T(""));
	pxi->SetMinMaxValue(0, 0b11);
	pxi->AddSpecialOption(_T("Starts"), _T("%d: Starts"));
	pxi->AddSpecialOption(_T("Ends"), _T("%d: Ends"));
	pxi->AddSpecialOption(_T("StartsEnds"), _T("%d: StartsEnds"));
	pxi->AddSpecialOption(_T("WholeSP"), _T("%d: WholeSP"));
	pxi->AllowEdit(FALSE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_ALSPIterHeader, _T("[%d] M_AXLELOADCAT"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_ALSPIterHeader, (variant_t)(int)0,
		_T("축 부하 종류"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b1111111);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_ALSPIterHeader, _T("[%d] V_New_Speed_Level"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_ALSPIterHeader, (variant_t)(int)0,
		_T("열차 축 부하 ≥ M_AXLELOADCAT (k,v) 인 경우 적용되어야 하는 속도제한"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0x1111111);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_ALSPIterHeader, _T("[%d] Q_FRONT"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_ALSPIterHeader, (variant_t)(int)0,
		_T("프로필 요소에 대해 주어진 속도제한이 열차의 앞부분 혹은 뒷부분이 해당 요소를 떠날때까지 적용되어야 하는지에 대한 표시하기 위한 Qualifier"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b1);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_ALSPIterHeader, _T("[%d] D_Start_Location"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_ALSPIterHeader, (variant_t)(int)0,
		_T("SP의 시작부분에 상대적인 Axle Load Speed Profile start의 위치"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xffffff);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_ALSPIterHeader, _T("[%d] D_End_Location"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_ALSPIterHeader, (variant_t)(int)0,
		_T("SP의 시작부분에 상대적인 Axle Load Speed Profile end의 위치"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xffffff);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);
}

CMFCPropertyIteratorSP_ULCS::CMFCPropertyIteratorSP_ULCS(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList) : CMFCPropertyIterator(strGroupName, dwData, bIsValueList)
{
}

CMFCPropertyIteratorSP_ULCS::CMFCPropertyIteratorSP_ULCS(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars) : CMFCPropertyIterator(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{
}

CMFCPropertyIteratorSP_ULCS::~CMFCPropertyIteratorSP_ULCS()
{
}

void CMFCPropertyIteratorSP_ULCS::AddIteratorProperty(int nextIdx)
{
	assert(targetProperty != nullptr);

	TCHAR idxStr[10];
	_stprintf_s(idxStr, _T("[%d]"), nextIdx);
	CMFCPropertyGridProperty* pxHeader = new CMFCPropertyGridProperty(idxStr); // 개별 Iterator를 담는 프로퍼티
	targetProperty->AddSubItem(pxHeader);

	int headerIdxValue = nextIdx;
	CMFCPropertyEX<int>* pxi;

	TCHAR SP_ULCSIterHeader[100];

	_stprintf_s(SP_ULCSIterHeader, _T("[%d] D_UnprotectedLx_Stop_Nominal"), headerIdxValue);	//[%d] D_UnprotectedLx_Stop_Nominal
	pxi = new CMFCPropertyEX<int>(SP_ULCSIterHeader, (variant_t)(int)0,
		_T("SP의 유효한 이동방향이 정방향인 경우의 비보호 건널목의 뒷부분에서의 정지위치"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xffffff);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_ULCSIterHeader, _T("[%d] D_UnprotectedLx_Stop_Reverse"), headerIdxValue);	//[%d] D_UnprotectedLx_Stop_Reverse
	pxi = new CMFCPropertyEX<int>(SP_ULCSIterHeader, (variant_t)(int)0,
		_T("SP의 유효한 이동방향이 역방향인 경우의 비보호 건널목의 뒷부분에서의 정지위치"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xffffff);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);
}

CMFCPropertyIteratorSP_PBD::CMFCPropertyIteratorSP_PBD(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList) : CMFCPropertyIterator(strGroupName, dwData, bIsValueList)
{
}

CMFCPropertyIteratorSP_PBD::CMFCPropertyIteratorSP_PBD(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars) : CMFCPropertyIterator(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{
}

CMFCPropertyIteratorSP_PBD::~CMFCPropertyIteratorSP_PBD()
{
}

void CMFCPropertyIteratorSP_PBD::AddIteratorProperty(int nextIdx)
{
	assert(targetProperty != nullptr);

	TCHAR idxStr[10];
	_stprintf_s(idxStr, _T("[%d]"), nextIdx);
	CMFCPropertyGridProperty* pxHeader = new CMFCPropertyGridProperty(idxStr); // 개별 Iterator를 담는 프로퍼티
	targetProperty->AddSubItem(pxHeader);

	int headerIdxValue = nextIdx;
	CMFCPropertyEX<int>* pxi;

	TCHAR SP_PBDIterHeader[100];

	_stprintf_s(SP_PBDIterHeader, _T("[%d] Q_Range"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_PBDIterHeader, (variant_t)(int)0,
		_T("Permitted Braking Distance area가 시작하는지 끝나는지 둘 다인지 SP의 전체범위를 포함하는지 명시합니다"));
	pxi->SetFormat(_T(""));
	pxi->SetMinMaxValue(0, 0b11);
	pxi->AddSpecialOption(_T("Starts"), _T("%d: Starts"));
	pxi->AddSpecialOption(_T("Ends"), _T("%d: Ends"));
	pxi->AddSpecialOption(_T("StartsEnds"), _T("%d: StartsEnds"));
	pxi->AddSpecialOption(_T("WholeSP"), _T("%d: WholeSP"));
	pxi->AllowEdit(FALSE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_PBDIterHeader, _T("[%d] D_Permitted_Braking_Distance"), headerIdxValue);		// [%d] D_Permitted_Braking_Distance
	pxi = new CMFCPropertyEX<int>(SP_PBDIterHeader, (variant_t)(int)0,
		_T("허용 제동 거리 값"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xffffff);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_PBDIterHeader, _T("[%d] Q_PBD_SBEB"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_PBDIterHeader, (variant_t)(int)0,
		_T("허용제동거리가 Service Brake혹은 Emergency Brake로 도달되어야하는지에 대한 여부"));
	pxi->SetFormat(_T(""));
	pxi->SetMinMaxValue(0, 0b1);
	pxi->AddSpecialOption(_T("Service Brake"), _T("%d: Service Brake"));
	pxi->AddSpecialOption(_T("Emergency Brake"), _T("%d: Emergency Brake"));
	pxi->AllowEdit(FALSE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_PBDIterHeader, _T("[%d] G_PBD"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_PBDIterHeader, (variant_t)(int)0,
		_T("계산에 적용가능한 단일 gradient 값"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, UINT8_MAX);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_PBDIterHeader, _T("[%d] Q_GDIR_PBD"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_PBDIterHeader, (variant_t)(int)0,
		_T("gradient의 방향"));
	pxi->SetFormat(_T(""));
	pxi->SetMinMaxValue(0, 0b1);
	pxi->AddSpecialOption(_T("Downhill"), _T("%d: Downhill"));
	pxi->AddSpecialOption(_T("Uphill"), _T("%d: Uphill"));
	pxi->AllowEdit(FALSE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_PBDIterHeader, _T("[%d] D_Start_Location"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_PBDIterHeader, (variant_t)(int)0,
		_T("SP의 뒷부분에 상대적인 Permitted Braking Distance area start의 위치"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xffffff);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_PBDIterHeader, _T("[%d] D_End_Location"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_PBDIterHeader, (variant_t)(int)0,
		_T("SP의 뒷부분에 상대적인 Permitted Braking Distance area start의 위치"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xffffff);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);
}

CMFCPropertyIteratorSP_SORB::CMFCPropertyIteratorSP_SORB(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList) : CMFCPropertyIterator(strGroupName, dwData, bIsValueList)
{
}

CMFCPropertyIteratorSP_SORB::CMFCPropertyIteratorSP_SORB(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars) : CMFCPropertyIterator(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{
}

CMFCPropertyIteratorSP_SORB::~CMFCPropertyIteratorSP_SORB()
{
}

void CMFCPropertyIteratorSP_SORB::AddIteratorProperty(int nextIdx)
{
	assert(targetProperty != nullptr);

	TCHAR idxStr[10];
	_stprintf_s(idxStr, _T("[%d]"), nextIdx);
	CMFCPropertyGridProperty* pxHeader = new CMFCPropertyGridProperty(idxStr); // 개별 Iterator를 담는 프로퍼티
	targetProperty->AddSubItem(pxHeader);

	int headerIdxValue = nextIdx;
	CMFCPropertyEX<int>* pxi;

	TCHAR SP_SORBIterHeader[30];

	_stprintf_s(SP_SORBIterHeader, _T("[%d] Q_Range"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_SORBIterHeader, (variant_t)(int)0,
		_T("Switch off Regenerative Brake area가 시작하는지 끝나는지 둘 다인지 SP의 전체범위를 포함하는지 명시합니다"));
	pxi->SetFormat(_T(""));
	pxi->SetMinMaxValue(0, 0b11);
	pxi->AddSpecialOption(_T("Starts"), _T("%d: Starts"));
	pxi->AddSpecialOption(_T("Ends"), _T("%d: Ends"));
	pxi->AddSpecialOption(_T("StartsEnds"), _T("%d: StartsEnds"));
	pxi->AddSpecialOption(_T("WholeSP"), _T("%d: WholeSP"));
	pxi->AllowEdit(FALSE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_SORBIterHeader, _T("[%d] D_Start_Location"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_SORBIterHeader, (variant_t)(int)0,
		_T("SP의 시작부분에 상대적인 Switch off Regenerative Brake area start의 위치"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xffffff);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_SORBIterHeader, _T("[%d] D_End_Location"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_SORBIterHeader, (variant_t)(int)0,
		_T("SP의 시작부분에 상대적인 Switch off Regenerative Brake area end의 위치"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xffffff);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);
}

CMFCPropertyIteratorSP_SOECB::CMFCPropertyIteratorSP_SOECB(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList) : CMFCPropertyIterator(strGroupName, dwData, bIsValueList)
{
}

CMFCPropertyIteratorSP_SOECB::CMFCPropertyIteratorSP_SOECB(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars) : CMFCPropertyIterator(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{
}

CMFCPropertyIteratorSP_SOECB::~CMFCPropertyIteratorSP_SOECB()
{
}

void CMFCPropertyIteratorSP_SOECB::AddIteratorProperty(int nextIdx)
{
	assert(targetProperty != nullptr);

	TCHAR idxStr[10];
	_stprintf_s(idxStr, _T("[%d]"), nextIdx);
	CMFCPropertyGridProperty* pxHeader = new CMFCPropertyGridProperty(idxStr); // 개별 Iterator를 담는 프로퍼티
	targetProperty->AddSubItem(pxHeader);

	int headerIdxValue = nextIdx;
	CMFCPropertyEX<int>* pxi;

	TCHAR SP_SOECBIterHeader[30];

	_stprintf_s(SP_SOECBIterHeader, _T("[%d] Q_Range"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_SOECBIterHeader, (variant_t)(int)0,
		_T("SSwitch off eddy current brake for service brake area가 시작하는지 끝나는지 둘 다인지 SP의 전체범위를 포함하는지 명시합니다"));
	pxi->SetFormat(_T(""));
	pxi->SetMinMaxValue(0, 0b11);
	pxi->AddSpecialOption(_T("Starts"), _T("%d: Starts"));
	pxi->AddSpecialOption(_T("Ends"), _T("%d: Ends"));
	pxi->AddSpecialOption(_T("StartsEnds"), _T("%d: StartsEnds"));
	pxi->AddSpecialOption(_T("WholeSP"), _T("%d: WholeSP"));
	pxi->AllowEdit(FALSE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_SOECBIterHeader, _T("[%d] D_Start_Location"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_SOECBIterHeader, (variant_t)(int)0,
		_T("SP의 시작부분에 상대적인 Switch off eddy current brake for service brake area start의 위치"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xffffff);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_SOECBIterHeader, _T("[%d] D_End_Location"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_SOECBIterHeader, (variant_t)(int)0,
		_T("SP의 시작부분에 상대적인 Switch off eddy current brake for service brake area end의 위치"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xffffff);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);
}

CMFCPropertyIteratorSP_SOECEB::CMFCPropertyIteratorSP_SOECEB(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList) : CMFCPropertyIterator(strGroupName, dwData, bIsValueList)
{
}

CMFCPropertyIteratorSP_SOECEB::CMFCPropertyIteratorSP_SOECEB(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars) : CMFCPropertyIterator(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{
}

CMFCPropertyIteratorSP_SOECEB::~CMFCPropertyIteratorSP_SOECEB()
{
}

void CMFCPropertyIteratorSP_SOECEB::AddIteratorProperty(int nextIdx)
{
	assert(targetProperty != nullptr);

	TCHAR idxStr[10];
	_stprintf_s(idxStr, _T("[%d]"), nextIdx);
	CMFCPropertyGridProperty* pxHeader = new CMFCPropertyGridProperty(idxStr); // 개별 Iterator를 담는 프로퍼티
	targetProperty->AddSubItem(pxHeader);

	int headerIdxValue = nextIdx;
	CMFCPropertyEX<int>* pxi;

	TCHAR SP_SOECEBIterHeader[30];

	_stprintf_s(SP_SOECEBIterHeader, _T("[%d] Q_Range"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_SOECEBIterHeader, (variant_t)(int)0,
		_T("Switch off eddy current brake for emergency brake area가 시작하는지 끝나는지 둘 다인지 SP의 전체범위를 포함하는지 명시합니다"));
	pxi->SetFormat(_T(""));
	pxi->SetMinMaxValue(0, 0b11);
	pxi->AddSpecialOption(_T("Starts"), _T("%d: Starts"));
	pxi->AddSpecialOption(_T("Ends"), _T("%d: Ends"));
	pxi->AddSpecialOption(_T("StartsEnds"), _T("%d: StartsEnds"));
	pxi->AddSpecialOption(_T("WholeSP"), _T("%d: WholeSP"));
	pxi->AllowEdit(FALSE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_SOECEBIterHeader, _T("[%d] D_Start_Location"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_SOECEBIterHeader, (variant_t)(int)0,
		_T("SP의 시작부분에 상대적인 Switch off eddy current brake for emergency brake area start의 위치"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xffffff);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_SOECEBIterHeader, _T("[%d] D_End_Location"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_SOECEBIterHeader, (variant_t)(int)0,
		_T("SP의 시작부분에 상대적인 Switch off eddy current brake for emergency brake area end의 위치"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xffffff);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);
}

CMFCPropertyIteratorSP_SOMSB::CMFCPropertyIteratorSP_SOMSB(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList) : CMFCPropertyIterator(strGroupName, dwData, bIsValueList)
{
}

CMFCPropertyIteratorSP_SOMSB::CMFCPropertyIteratorSP_SOMSB(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars) : CMFCPropertyIterator(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{
}

CMFCPropertyIteratorSP_SOMSB::~CMFCPropertyIteratorSP_SOMSB()
{
}

void CMFCPropertyIteratorSP_SOMSB::AddIteratorProperty(int nextIdx)
{
	assert(targetProperty != nullptr);

	TCHAR idxStr[10];
	_stprintf_s(idxStr, _T("[%d]"), nextIdx);
	CMFCPropertyGridProperty* pxHeader = new CMFCPropertyGridProperty(idxStr); // 개별 Iterator를 담는 프로퍼티
	targetProperty->AddSubItem(pxHeader);

	int headerIdxValue = nextIdx;
	CMFCPropertyEX<int>* pxi;

	TCHAR SP_SOMSBIterHeader[30];

	_stprintf_s(SP_SOMSBIterHeader, _T("[%d] Q_Range"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_SOMSBIterHeader, (variant_t)(int)0,
		_T("Switch off Magnetic Shoe Brake area가 시작하는지 끝나는지 둘 다인지 SP의 전체범위를 포함하는지 명시합니다"));
	pxi->SetFormat(_T(""));
	pxi->SetMinMaxValue(0, 0b11);
	pxi->AddSpecialOption(_T("Starts"), _T("%d: Starts"));
	pxi->AddSpecialOption(_T("Ends"), _T("%d: Ends"));
	pxi->AddSpecialOption(_T("StartsEnds"), _T("%d: StartsEnds"));
	pxi->AddSpecialOption(_T("WholeSP"), _T("%d: WholeSP"));
	pxi->AllowEdit(FALSE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_SOMSBIterHeader, _T("[%d] D_Start_Location"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_SOMSBIterHeader, (variant_t)(int)0,
		_T("SP의 시작부분에 상대적인 Switch off Magnetic Shoe Brake area start의 위치"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xffffff);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_SOMSBIterHeader, _T("[%d] D_End_Location"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_SOMSBIterHeader, (variant_t)(int)0,
		_T("SP의 시작부분에 상대적인 Switch off Magnetic Shoe Brake area end의 위치 "));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xffffff);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);
}

CMFCPropertyIteratorSP_ADBFI::CMFCPropertyIteratorSP_ADBFI(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList) : CMFCPropertyIterator(strGroupName, dwData, bIsValueList)
{
}

CMFCPropertyIteratorSP_ADBFI::CMFCPropertyIteratorSP_ADBFI(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars) : CMFCPropertyIterator(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{
}

CMFCPropertyIteratorSP_ADBFI::~CMFCPropertyIteratorSP_ADBFI()
{
}

void CMFCPropertyIteratorSP_ADBFI::AddIteratorProperty(int nextIdx)
{
	assert(targetProperty != nullptr);

	TCHAR idxStr[10];
	_stprintf_s(idxStr, _T("[%d]"), nextIdx);
	CMFCPropertyGridProperty* pxHeader = new CMFCPropertyGridProperty(idxStr); // 개별 Iterator를 담는 프로퍼티
	targetProperty->AddSubItem(pxHeader);

	int headerIdxValue = nextIdx;
	CMFCPropertyEX<int>* pxi;

	TCHAR SP_ADBFIIterHeader[30];

	_stprintf_s(SP_ADBFIIterHeader, _T("[%d] Q_Range"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_ADBFIIterHeader, (variant_t)(int)0,
		_T("Dynamic Brake Inhibition Area가 시작하는지 끝나는지 둘 다인지 SP의 전체범위를 포함하는지 명시합니다"));
	pxi->SetFormat(_T(""));
	pxi->SetMinMaxValue(0, 0b11);
	pxi->AddSpecialOption(_T("Starts"), _T("%d: Starts"));
	pxi->AddSpecialOption(_T("Ends"), _T("%d: Ends"));
	pxi->AddSpecialOption(_T("forceStartsEnds"), _T("%d: forceStartsEnds"));
	pxi->AddSpecialOption(_T("WholeSP"), _T("%d: WholeSP"));
	pxi->AllowEdit(FALSE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_ADBFIIterHeader, _T("[%d] D_Start_Location"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_ADBFIIterHeader, (variant_t)(int)0,
		_T("SP의 시작부분에 상대적인 Dynamic Brake Force Inhibition Area start의 위치"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xffffff);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_ADBFIIterHeader, _T("[%d] D_End_Location"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_ADBFIIterHeader, (variant_t)(int)0,
		_T("SP의 시작부분에 상대적인 Dynamic Brake Force Inhibition Area end의 위치"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xffffff);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

}

CMFCPropertyIteratorSP_ALDBF::CMFCPropertyIteratorSP_ALDBF(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList) : CMFCPropertyIterator(strGroupName, dwData, bIsValueList)
{
}

CMFCPropertyIteratorSP_ALDBF::CMFCPropertyIteratorSP_ALDBF(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars) : CMFCPropertyIterator(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{
}

CMFCPropertyIteratorSP_ALDBF::~CMFCPropertyIteratorSP_ALDBF()
{
}

void CMFCPropertyIteratorSP_ALDBF::AddIteratorProperty(int nextIdx)
{
	assert(targetProperty != nullptr);

	TCHAR idxStr[10];
	_stprintf_s(idxStr, _T("[%d]"), nextIdx);
	CMFCPropertyGridProperty* pxHeader = new CMFCPropertyGridProperty(idxStr); // 개별 Iterator를 담는 프로퍼티
	targetProperty->AddSubItem(pxHeader);

	int headerIdxValue = nextIdx;
	CMFCPropertyEX<int>* pxi;

	TCHAR SP_ALDBFIterHeader[100];

	_stprintf_s(SP_ALDBFIterHeader, _T("[%d] Q_Range"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_ALDBFIterHeader, (variant_t)(int)0,
		_T("the Dynamic Brake Limitation Area가 시작하는지 끝나는지 둘 다인지 SP의 전체범위를 포함하는지 명시합니다"));
	pxi->SetFormat(_T(""));
	pxi->SetMinMaxValue(0, 0b11);
	pxi->AddSpecialOption(_T("Starts"), _T("%d: Starts"));
	pxi->AddSpecialOption(_T("Ends"), _T("%d: Ends"));
	pxi->AddSpecialOption(_T("forceStartsEnds"), _T("%d: forceStartsEnds"));
	pxi->AddSpecialOption(_T("WholeSP"), _T("%d: WholeSP"));
	pxi->AllowEdit(FALSE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_ALDBFIterHeader, _T("[%d] M_Dynamic_Brake_Force_Limit"), headerIdxValue);	//[%d] M_Dynamic_Brake_Force_Limit
	pxi = new CMFCPropertyEX<int>(SP_ALDBFIterHeader, (variant_t)(int)0,
		_T("Dynamic Brake에 의해 적용되는 최대 제동력"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, UINT16_MAX);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_ALDBFIterHeader, _T("[%d] D_Start_Location"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_ALDBFIterHeader, (variant_t)(int)0,
		_T("SP의 시작부분에 상대적인 Dynamic Brake Force Limit Area start의 위치"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xffffff);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SP_ALDBFIterHeader, _T("[%d] D_End_Location"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SP_ALDBFIterHeader, (variant_t)(int)0,
		_T("SP의 시작부분에 상대적인 Dynamic Brake Force Limit Area end의 위치"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0xffffff);
	pxi->AllowEdit(TRUE);
	pxHeader->AddSubItem(pxi);
}

//----------------------------------------------------------------- Send End -----------------------------------------------------------------//

CMFCPropertyIteratorATO_VER::CMFCPropertyIteratorATO_VER(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList) : CMFCPropertyIterator(strGroupName, dwData, bIsValueList)
{
}

CMFCPropertyIteratorATO_VER::CMFCPropertyIteratorATO_VER(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars) : CMFCPropertyIterator(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{
}

CMFCPropertyIteratorATO_VER::~CMFCPropertyIteratorATO_VER()
{
}

void CMFCPropertyIteratorATO_VER::AddIteratorProperty(int nextIdx)
{
	assert(targetProperty != nullptr);

	TCHAR idxStr[10];
	_stprintf_s(idxStr, _T("[%d]"), nextIdx);
	CMFCPropertyGridProperty* pxHeader = new CMFCPropertyGridProperty(idxStr); // 개별 Iterator를 담는 프로퍼티
	targetProperty->AddSubItem(pxHeader);

	int headerIdxValue = nextIdx;
	CMFCPropertyEX<int>* pxi;

	TCHAR HSReq_ATO_VERIterHeader[30];

	_stprintf_s(HSReq_ATO_VERIterHeader, _T("[%d] M_ATO_Version"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(HSReq_ATO_VERIterHeader, (variant_t)(int)0,
		_T("지원되는 ATO 시스템 버전"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, UINT16_MAX);
	pxi->AllowEdit(FALSE);
	pxHeader->AddSubItem(pxi);
}

CMFCPropertyIteratorSPReq_SP::CMFCPropertyIteratorSPReq_SP(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList) : CMFCPropertyIterator(strGroupName, dwData, bIsValueList)
{
}

CMFCPropertyIteratorSPReq_SP::CMFCPropertyIteratorSPReq_SP(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars) : CMFCPropertyIterator(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{
}

CMFCPropertyIteratorSPReq_SP::~CMFCPropertyIteratorSPReq_SP()
{
}

void CMFCPropertyIteratorSPReq_SP::AddIteratorProperty(int nextIdx)
{
	assert(targetProperty != nullptr);

	TCHAR idxStr[10];
	_stprintf_s(idxStr, _T("[%d]"), nextIdx);
	CMFCPropertyGridProperty* pxHeader = new CMFCPropertyGridProperty(idxStr); // 개별 Iterator를 담는 프로퍼티
	targetProperty->AddSubItem(pxHeader);

	int headerIdxValue = nextIdx;
	CMFCPropertyEX<int>* pxi;
	CMFCPropertyEX<UINT32>* pxu;

	TCHAR SPReq_SPIterHeader[30];

	_stprintf_s(SPReq_SPIterHeader, _T("[%d] NID_C"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(SPReq_SPIterHeader, (variant_t)(int)887,
		_T("SP의 나라 혹은 지역의 ID"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b1111111111);
	pxi->AllowEdit(FALSE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(SPReq_SPIterHeader, _T("[%d] NID_SP"), headerIdxValue);
	pxu = new CMFCPropertyEX<UINT32>(SPReq_SPIterHeader, (variant_t)(UINT32)0,
		_T("요청된 SP의 ID"));
	pxu->SetFormat(_T("%u"));
	pxu->SetMinMaxValue(0, UINT32_MAX);
	pxu->AllowEdit(FALSE);
	pxHeader->AddSubItem(pxu);
}

CMFCPropertyIteratorSTR_TP::CMFCPropertyIteratorSTR_TP(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList) : CMFCPropertyIterator(strGroupName, dwData, bIsValueList)
{
}

CMFCPropertyIteratorSTR_TP::CMFCPropertyIteratorSTR_TP(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars) : CMFCPropertyIterator(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{
}

CMFCPropertyIteratorSTR_TP::~CMFCPropertyIteratorSTR_TP()
{
}

void CMFCPropertyIteratorSTR_TP::AddIteratorProperty(int nextIdx)
{
	assert(targetProperty != nullptr);

	TCHAR idxStr[10];
	_stprintf_s(idxStr, _T("[%d]"), nextIdx);
	CMFCPropertyGridProperty* pxHeader = new CMFCPropertyGridProperty(idxStr); // 개별 Iterator를 담는 프로퍼티
	targetProperty->AddSubItem(pxHeader);

	int headerIdxValue = nextIdx;
	CMFCPropertyEX<int>* pxi;
	CMFCPropertyEX<UINT32>* pxu;

	TCHAR STR_TPIterHeader[30];

	_stprintf_s(STR_TPIterHeader, _T("[%d] NID_C"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(STR_TPIterHeader, (variant_t)(int)0,
		_T("TP 정보의 개수"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b1111111111);
	pxi->AllowEdit(FALSE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(STR_TPIterHeader, _T("[%d] NID_TP"), headerIdxValue);
	pxu = new CMFCPropertyEX<UINT32>(STR_TPIterHeader, (variant_t)(UINT32)0,
		_T("다음 TP의 ID"));
	pxu->SetFormat(_T("%d"));
	pxu->SetMinMaxValue(0, 0xffffffff);
	pxu->AllowEdit(FALSE);
	pxHeader->AddSubItem(pxu);

	_stprintf_s(STR_TPIterHeader, _T("[%d] T_Arrival_Date"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(STR_TPIterHeader, (variant_t)(int)0,
		_T("TP에 도착하는 날짜"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b111111111111111);
	pxi->AllowEdit(FALSE);
	pxHeader->AddSubItem(pxi);

	_stprintf_s(STR_TPIterHeader, _T("[%d] T_Arrival_Seconds"), headerIdxValue);
	pxi = new CMFCPropertyEX<int>(STR_TPIterHeader, (variant_t)(int)0,
		_T("TP에 도착하는 초단위 추정 시간"));
	pxi->SetFormat(_T("%d"));
	pxi->SetMinMaxValue(0, 0b11111111111111111);
	pxi->AllowEdit(FALSE);
	pxHeader->AddSubItem(pxi);

}

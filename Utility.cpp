#include "pch.h"
#include "Utility.h"
#include "TextTable.h"
#include "126 Item Enums.h"



using namespace PacketHSAckEnum;

using namespace std;
std::string str;

void Utility::CreateDialogForTapCtrl(CDialogEx* dlg, int ctrlID, CTabCtrl* tap)
{
	CRect rc;
	ZeroMemory(&rc, sizeof(rc));
	tap->GetWindowRect(rc);

	dlg->Create(ctrlID, tap);
	dlg->MoveWindow(0, 20, rc.Width(), rc.Height());
}

void Utility::SelectTabOfTabCtrl(CTabCtrl* tab, int id)
{
	if (tab == nullptr)return;

	if (id >= tab->GetItemCount())id = tab->GetItemCount() - 1;

	tab->SetCurSel(id);
	NMHDR nmhdr;

	nmhdr.code = TCN_SELCHANGE;
	nmhdr.idFrom = tab->GetDlgCtrlID();
	nmhdr.hwndFrom = tab->m_hWnd;
	SendMessage(tab->GetParent()->m_hWnd, WM_NOTIFY, nmhdr.idFrom, (LPARAM)&nmhdr);
}



DWORD Utility::IPtoDWORD(const CHAR* address)
{
	DWORD result = 0;

	byte ipNumber = 0;
	int ipStrIdx = 0;

	while (isdigit(address[ipStrIdx]) == 0) ++ipStrIdx;
	for (; isdigit(address[ipStrIdx]) != 0; ++ipStrIdx)
	{
		ipNumber *= 10;
		ipNumber += (address[ipStrIdx] - ('0'));
	}
	result += ipNumber;
	result <<= 8;
	ipNumber = 0;

	while (isdigit(address[++ipStrIdx]) == 0);
	for (; isdigit(address[ipStrIdx]) != 0; ++ipStrIdx)
	{
		ipNumber *= 10;
		ipNumber += (address[ipStrIdx] - ('0'));
	}
	result += ipNumber;
	result <<= 8;
	ipNumber = 0;

	while (isdigit(address[++ipStrIdx]) == 0);
	for (; isdigit(address[ipStrIdx]) != 0; ++ipStrIdx)
	{
		ipNumber *= 10;
		ipNumber += (address[ipStrIdx] - ('0'));
	}
	result += ipNumber;
	result <<= 8;
	ipNumber = 0;

	while (isdigit(address[++ipStrIdx]) == 0);
	for (; isdigit(address[ipStrIdx]) != 0; ++ipStrIdx)
	{
		ipNumber *= 10;
		ipNumber += (address[ipStrIdx] - ('0'));
	}
	result += ipNumber;

	return result;
}

DWORD Utility::IPtoDWORD(const WCHAR* address)
{
	DWORD result = 0;

	byte ipNumber = 0;
	int ipStrIdx = 0;

	while (iswdigit(address[ipStrIdx]) == 0) ++ipStrIdx;
	for (; iswdigit(address[ipStrIdx]) != 0; ++ipStrIdx)
	{
		ipNumber *= 10;
		ipNumber += (address[ipStrIdx] - L'0');
	}
	result += ipNumber;
	result <<= 8;
	ipNumber = 0;

	while (iswdigit(address[ipStrIdx]) == 0) ++ipStrIdx;
	for (; iswdigit(address[ipStrIdx]) != 0; ++ipStrIdx)
	{
		ipNumber *= 10;
		ipNumber += (address[ipStrIdx] - L'0');
	}
	result += ipNumber;
	result <<= 8;
	ipNumber = 0;

	while (iswdigit(address[ipStrIdx]) == 0) ++ipStrIdx;
	for (; iswdigit(address[ipStrIdx]) != 0; ++ipStrIdx)
	{
		ipNumber *= 10;
		ipNumber += (address[ipStrIdx] - L'0');
	}
	result += ipNumber;
	result <<= 8;
	ipNumber = 0;

	while (iswdigit(address[ipStrIdx]) == 0) ++ipStrIdx;
	for (; iswdigit(address[ipStrIdx]) != 0; ++ipStrIdx)
	{
		ipNumber *= 10;
		ipNumber += (address[ipStrIdx] - L'0');
	}
	result += ipNumber;

	return result;
}

string Utility::DWORDtoIP(DWORD address) //BE
{
	string result;
	char tempStr[20];

	int ipValue[4]{};

	for (int i = 3; i >= 0; i--)
	{
		ipValue[i] = address & 0xFF;
		address >>= 8;
	}

	sprintf_s(tempStr, "%d.%d.%d.%d", ipValue[0], ipValue[1], ipValue[2], ipValue[3]);
	result = tempStr;
	return result;
}


BOOL Utility::CFileWriteTCHAR(CFile* f, const TCHAR* txt)
{
	if (f == nullptr) return FALSE;
	for (size_t strIDX = 0; txt[strIDX] != _T('\0'); strIDX++)
	{
		f->Write((txt + strIDX), sizeof(TCHAR));
	}
	return TRUE;
}

BOOL Utility::CFileWriteCHAR(CFile* f, const CHAR* txt)
{
	if (f == nullptr) return FALSE;
	for (size_t strIDX = 0; txt[strIDX] != '\0'; strIDX++)
	{
		f->Write((txt + strIDX), sizeof(CHAR));
	}
	return TRUE;
}

variant_t Utility::CastVariantT(const variant_t& insertVal, const VARTYPE& castType)
{
	// TODO: 여기에 return 문을 삽입합니다.
	variant_t result;
	switch (castType)
	{
	case VT_I1:
	case VT_I2:
	case VT_I4:
	case VT_I8:
	case VT_INT:
		result = (int)insertVal;
		break;

	case VT_R4:
	case VT_R8:
		result = (double)insertVal;
		break;

	case VT_UI1:
	case VT_UI2:
	case VT_UI4:
	case VT_UI8:
	case VT_UINT:
		result = (UINT)insertVal;
		break;
	default:
		result = insertVal;
		break;
	}
	result.vt = castType;
	return result;
}

void Utility::SetValueWithTypeCasting(CMFCPropertyGridProperty* targetProperty, variant_t val)
{
	VARTYPE variableType = targetProperty->GetValue().vt;
	variant_t insertValue = Utility::CastVariantT(val, variableType);
	targetProperty->SetValue(insertValue);
	targetProperty->OnEndEdit();
}





BOOL Utility::CheckFileUnicode(CFile& f)
{
	if (f.GetLength() < sizeof(WORD)) return FALSE;
	WORD unicodeWord;
	f.Read(&unicodeWord, sizeof(WORD));
	f.Seek(0, CFile::begin);

	if (unicodeWord == 0xFEFF) return TRUE;

	return FALSE;
}

CString Utility::GetDirectoryFromFileDialog(LPCTSTR	lpszDefExt, LPCTSTR fileName, DWORD dwFlags, LPCTSTR lpszFilter)
{
	CString pathName = _T("");

	CFileDialog dlg(TRUE, lpszDefExt, fileName, dwFlags, lpszFilter);
	if (IDOK == dlg.DoModal()) pathName = dlg.GetPathName();

	return pathName;
}


enum class HexCheck : byte
{
	HEX_MAX = 0x10,
	TEXT_END = 0xFE,
	NOT_HEX = 0xFF
};

void Utility::TextToByteArray_MemoryAlloc(const TCHAR* txt, byte*& buffer, size_t& length)
{
	vector<byte> container;
	container.reserve(1600);

	size_t i = 0;
	byte tempHex = LetterToByte(txt[i]);
	while (tempHex != (byte)HexCheck::TEXT_END)
	{
		byte hexCode = 0;

		//두개씩 읽음
		while (LetterToByte(txt[i]) == (byte)HexCheck::NOT_HEX) ++i; // hex가 아닌 경우 인덱스 이동

		tempHex = LetterToByte(txt[i]);
		if (tempHex == (byte)HexCheck::TEXT_END) break;
		else hexCode |= (tempHex << 4);
		++i;

		while (LetterToByte(txt[i]) == (byte)HexCheck::NOT_HEX) ++i; // hex가 아닌 경우 인덱스 이동

		tempHex = LetterToByte(txt[i]);
		if (tempHex == (byte)HexCheck::TEXT_END) break;
		else hexCode |= tempHex;
		++i;

		container.push_back(hexCode);
	}

	length = container.size();
	buffer = new byte[length]; // length == 바이트 배열 길이 (1-base)
	std::copy(container.begin(), container.end(), buffer);
}

byte Utility::LetterToByte(const TCHAR txt)
{
	byte hexCode;
	if (txt == _T('\0'))
		hexCode = (byte)HexCheck::TEXT_END;

	else if (txt >= _T('0') && txt <= _T('9'))
		hexCode = (byte)(txt - _T('0'));
	else if (txt >= _T('A') && txt <= _T('F'))
		hexCode = (byte)(txt - _T('A') + 0xA);
	else if (txt >= _T('a') && txt <= _T('f'))
		hexCode = (byte)(txt - _T('a') + 0xA);

	else hexCode = (byte)HexCheck::NOT_HEX;

	return hexCode;
}

std::vector<UINT8> Utility::tokenizeString16(const CString& str)
{
	std::vector<UINT8> tokens; // 토큰들을 저장할 벡터

	// CString을 std::string으로 변환
	std::string strMB = CT2A(str);

	// 공백 제거
	strMB.erase(remove_if(strMB.begin(), strMB.end(), ::isspace), strMB.end());

	// 2자리씩 끊어서 16진수로 변환하여 tokens에 저장
	for (size_t i = 0; i < strMB.size(); i += 2) {
		std::string token = strMB.substr(i, 2);
		UINT8 num = (UINT8)std::stoul(token, nullptr, 16); // 토큰을 UINT8로 변환 (16진수로 해석)
		tokens.push_back(num); // 변환된 숫자를 벡터에 추가
	}

	return tokens;
}


//Little Endian, (int)12345678 -> (BCD)0x12345678
UINT32 Utility::ValueToBCD32(UINT32 val)
{
	UINT32 result = 0;
	for (int i = 0; val > 0; val /= 10)
	{
		UINT32 digit = val % 10; //1의 자리수
		result |= digit << (i * 4); //맨 오른쪽에서 순서대로 채움
		++i;
	}
	return result;
}

//Little Endian, (BCD)0x12345678 -> (int)12345678
UINT32 Utility::BCD32ToValue(UINT32 val)
{
	UINT32 result = 0;
	for (int amplifier = 1; val > 0; val >>= 4)
	{
		UINT32 hex4 = val & 0xF; //맨 오른쪽 비트 4개
		result += hex4 * amplifier;
		amplifier *= 10;
	}
	return result;
}

void Utility::ATO_Memcpy(UINT8* Dest, const UINT8* Src, const UINT32 DestSize, UINT32 SrcSize)
{
	UINT32 idx = 0;

	if (DestSize < SrcSize)
	{
		SrcSize = DestSize;
	}
	else
	{
		for (idx = 0U; idx < SrcSize; idx++)
		{
			*(Dest + idx) = *(Src + idx);
		}
	}
}

//bitsetOrder TRUE: big endian order(126),  FALSE: little endien order(130)
UINT32 Utility::DecodeByte(const UINT8* szData, const UINT32 offset, const UINT32 nBitLen, BOOL bitsetOrder)
{
	UINT32 result = 0;

	if ((nBitLen == 8) && ((offset % 8) == 0))
	{
		UINT8 dataIdx = (offset / 8);
		result = szData[dataIdx];
	}
	else
	{
		UINT32 nBitIdx = 0;
		UINT8 nBitMask = 0, bitValue = 0;

		//ss130용으로 추가 (비트셋 읽는 순서가 반대)
		if (bitsetOrder == FALSE)
		{
			for (nBitIdx = offset; nBitIdx < (offset + nBitLen); nBitIdx++)
			{
				UINT8 bitIdxOfCurrentByte = (nBitIdx % 8);
				nBitMask = (1 << bitIdxOfCurrentByte); //n번째 비트
				bitValue = (((szData[(nBitIdx / 8)] & nBitMask) == 0) ? FALSE : TRUE); //n번째 비트의 플래그 확인
				result |= ((bitValue == TRUE) ? 1 : 0) << (nBitIdx - offset);
			}
		}
		else
		{
			for (nBitIdx = offset; nBitIdx < (offset + nBitLen); nBitIdx++)
			{
				nBitMask = (1 << (7 - (nBitIdx % 8)));
				bitValue = (((szData[(nBitIdx / 8)] & nBitMask) == 0) ? FALSE : TRUE);
				result = ((result << 1) | ((bitValue == TRUE) ? 1 : 0));
			}
		}
	}

	return result;
}

void Utility::EncodeByte(UINT8* szOutData, const UINT32 offset, const UINT32 nBitLen, const UINT8 value)
{
	UINT8 dataIdx = 0;

	if ((nBitLen == 8) && ((offset % 8) == 0))
	{
		dataIdx = (offset / 8);
		szOutData[dataIdx] = value;
	}
	else
	{
		UINT32 nBitIdx = 0, nBitMask = 0;

		for (nBitIdx = offset; nBitIdx < (offset + nBitLen); nBitIdx++)
		{
			dataIdx = (nBitIdx / 8);
			nBitMask = (7 - ((nBitIdx) % 8));
			if ((value & (0x80 >> ((nBitIdx - offset) + (8 - nBitLen)))) != 0)
			{
				szOutData[dataIdx] = (szOutData[dataIdx] | (1 << nBitMask));
			}
			else
			{
				szOutData[dataIdx] = (szOutData[dataIdx] & (~(1 << nBitMask)));
			}
		}
	}

}

void Utility::EncodeByte_Reverse(UINT8* szOutData, const UINT32 offset, const UINT32 nBitLen, const UINT8 value)
{

}

BOOL Utility::Encode_uint8(UINT8* szOutData, const UINT32 nDataLen, const UINT32 offset, const UINT32 nBitLen, const UINT8 value)
{
	BOOL bRet = TRUE;

	if ((offset + nBitLen) > (nDataLen * 8))
	{
		bRet = FALSE;
	}
	else
	{
		EncodeByte(szOutData, offset, nBitLen, value);
	}
	return bRet;
}

BOOL Utility::Encode_uint16(UINT8* szOutData, const UINT32 nDataLen, const UINT32 offset, const UINT32 nBitLen, const UINT16 value)
{
	BOOL nRet = TRUE;

	if ((offset + nBitLen) > (nDataLen * 8))
	{
		nRet = FALSE;
	}
	else
	{
		EncodeByte(szOutData, offset, nBitLen - 8, ((value & 0xFF00) >> 8));
		EncodeByte(szOutData, (offset + nBitLen) - 8, 8, (value & 0x00FF));
	}
	return nRet;
}

BOOL Utility::Encode_uint32(UINT8* szOutData, const UINT32 nDataLen, const UINT32 offset, const UINT32 nBitLen, const UINT32 value)
{
	BOOL nRet = TRUE;

	if ((offset + nBitLen) > (nDataLen * 8))
	{
		nRet = FALSE;
	}
	else
	{
		if (nBitLen > 24)
		{
			/* 32bit ~ 16bit */
			EncodeByte(szOutData, offset, nBitLen - 24, (value & 0xFF000000U) >> 24);
			EncodeByte(szOutData, (offset + nBitLen) - 24, 8, ((value & 0x00FF0000U) >> 16));
		}
		else
		{
			/* 16bit ~ 24bit */
			EncodeByte(szOutData, offset, nBitLen - 16, ((value & 0x00FF0000U) >> 16));
		}
		/* 16bit ~ 1bit */
		EncodeByte(szOutData, (offset + nBitLen) - 16, 8, ((value & 0x0000FF00U) >> 8));
		EncodeByte(szOutData, (offset + nBitLen) - 8, 8, (value & 0x000000FFU));
	}
	return nRet;
}

BOOL Utility::Decode_uint8(const UINT8* szData, const UINT32 nDataLen, const UINT32 offset, const UINT32 nBitLen, UINT8* pResult)
{
	BOOL nRet = TRUE;

	if ((offset + nBitLen) > (nDataLen * 8))
	{
		nRet = FALSE;
	}
	else
	{
		*pResult = (UINT8)DecodeByte(szData, offset, nBitLen);
	}
	return nRet;
}

BOOL Utility::Decode_uint16(const UINT8* szData, const UINT32 nDataLen, const UINT32 offset, const UINT32 nBitLen, UINT16* pResult)
{
	BOOL nRet = TRUE;
	UINT16 nTemp1 = 0, nTemp2 = 0;

	if ((offset + nBitLen) > (nDataLen * 8))
	{
		nRet = FALSE;
	}
	else
	{
		nTemp1 = (UINT16)DecodeByte(szData, (offset + nBitLen) - 8, 8);
		nTemp2 = (UINT16)DecodeByte(szData, offset, nBitLen - 8);
		*pResult = nTemp1 | (nTemp2 << 8);
	}
	return nRet;
}

BOOL Utility::Decode_int16(const UINT8* szData, const UINT32 nDataLen, const UINT32 offset, const UINT32 nBitLen, INT16* pResult)
{
	BOOL nRet = TRUE;
	UINT8 HIGH_Mask;
	UINT16 bHIGHBytes;

	nRet = Decode_uint16(szData, nDataLen, offset, nBitLen, (UINT16*)pResult);
	if (nRet == TRUE)
	{
		bHIGHBytes = (((UINT16)(*pResult) & 0xFF00U) >> 8U);
		HIGH_Mask = (UINT8)(1U << ((nBitLen - 1) % 8U));

		if (((UINT8)(bHIGHBytes & HIGH_Mask) != 0U))  //음수
		{
			*pResult = -(INT16)((0xFFFFU - (UINT16)*pResult) + 1U);
		}
	}
	return nRet;
}

BOOL Utility::Decode_int32(const UINT8* szData, const UINT32 nDataLen, const UINT32 offset, const UINT32 nBitLen, INT32* pResult)
{
	BOOL nRet = TRUE;
	UINT8 HIGH_Mask;
	UINT32 bHIGHBytes;

	nRet = Decode_uint32(szData, nDataLen, offset, nBitLen, (UINT32*)pResult);
	if (nRet == TRUE)
	{
		bHIGHBytes = (((UINT32)(*pResult) & 0xFFFF0000U) >> 16U);
		HIGH_Mask = (UINT8)(1U << ((nBitLen - 1) % 8U));

		if (((UINT8)(bHIGHBytes & HIGH_Mask) != 0U))  //음수
		{
			*pResult = -(INT32)((0xFFFFFFFFU - (UINT32)*pResult) + 1U);
		}
	}
	return nRet;
}



BOOL Utility::Decode_uint32(const UINT8* szData, const UINT32 nDataLen, const UINT32 offset, const UINT32 nBitLen, UINT32* pResult)
{
	BOOL nRet = TRUE;
	UINT32 nTemp1 = 0, nTemp2 = 0, nTemp3 = 0, nTemp4 = 0;

	if ((offset + nBitLen) > (nDataLen * 8))
	{
		nRet = FALSE;
	}
	else
	{
		if (nBitLen > 24)
		{
			nTemp1 = (UINT32)DecodeByte(szData, (offset + nBitLen) - 8, 8);
			nTemp2 = (UINT32)DecodeByte(szData, (offset + nBitLen) - 16, 8);
			nTemp3 = (UINT32)DecodeByte(szData, (offset + nBitLen) - 24, 8);
			nTemp4 = (UINT32)DecodeByte(szData, offset, nBitLen - 24);

			*pResult = nTemp1 | (nTemp2 << 8) | (nTemp3 << 16) | (nTemp4 << 24);
		}
		else
		{
			nTemp1 = (UINT16)DecodeByte(szData, (offset + nBitLen) - 8, 8);
			nTemp2 = (UINT16)DecodeByte(szData, (offset + nBitLen) - 16, 8);
			nTemp3 = (UINT16)DecodeByte(szData, offset, nBitLen - 16);

			*pResult = nTemp1 | (nTemp2 << 8) | (nTemp3 << 16);
		}
	}
	return nRet;
}





int Utility::CMperSecondToKMperHour(int cms)
{
	double cmps = (double)cms;
	double result = cmps * (3600.0 / 100000.0);

	return (int)result;
}

void Utility::SecondsToHourMinuteSecond(UINT32 input, int& hour, int& minute, int& second)
{
	hour = input / 3600;
	input %= 3600;
	minute = input / 60;
	input %= 60;
	second = input;
	return;
}

void Utility::MiliSecondsToHourMinuteSecondMs(UINT32 input, int& h, int& m, int& s, int& ms)
{
	ms = input % 1000;
	SecondsToHourMinuteSecond((input / 1000), h, m, s);
	return;
}



bool Utility::GetButtonChectState(HWND hButton)
{
	int result = BST_UNCHECKED;
	static DWORD_PTR dwResult;
	result = (int)::SendMessageTimeout(hButton, BM_GETCHECK, 0, 0, SMTO_NORMAL, 100, &dwResult);
	if (result == BST_CHECKED)return true;
	else return false;
}



BOOL IsLeapYear(UINT16 year)
{
	BOOL bRet;
	// 윤년
	if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
	{
		bRet = TRUE;
	}
	else
	{
		bRet = FALSE;
	}

	return bRet;
}

/**
 * 년마다 각 달의 일수 반환 함수
 *
 * @param year
 * @param month
 * @return
 */
INT32 GetDaysInMonth(UINT32 year, INT32 month)
{
	INT32 daysInMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	if ((month == 2) && IsLeapYear(year) == TRUE)
	{
		return 29;
	}

	return daysInMonth[month - 1];
}


bool Utility::areArraysEqual(const BYTE arr1[], const BYTE arr2[], int size)
{
	for (int i = 0; i < size; ++i) {
		if (arr1[i] != arr2[i]) {
			return false; // 배열 요소 중 하나라도 다르면 false 반환
		}
	}
	return true; // 모든 배열 요소가 동일하면 true 반환
}

UINT16 Utility::GetUTCTimeStamp_Date(void)
{
	UINT16 nTotalDays = 0;
	UINT8 nCurYear = 0, nCurMonth = 0, nCurDay = 0;
	UINT16 nCurFulYear = 0;

	// year : 00 ~ 255, month : 1~11, day: 1~31
	//(void)rtcDateRead(&nCurYear, &nCurMonth, &nCurDay);
	nCurFulYear = (2000 + nCurYear);

	// 년도당 일수
	for (UINT16 nIdx = TIMESTAMP_REFERENCE_YEAR; nIdx < nCurFulYear; nIdx++)
	{
		nTotalDays += IsLeapYear(nIdx) ? 366 : 365;
	}
	// 달 당 일수
	for (UINT16 nIdx2 = 1; nIdx2 < nCurMonth; nIdx2++)
	{
		nTotalDays += GetDaysInMonth(nCurFulYear, nIdx2);
	}
	//1일 기준 증가 치
	nTotalDays += nCurDay - 1;

	return nTotalDays;

}

UINT32 Utility::GetUTCTimeStamp_Seconds(void)
{
	UINT8 nCurHour = 0, nCurMin = 0, nCurSecs = 0;
	UINT32 nTotalSecs = 0;

	// hours : 0 ~ 23, min : 0 ~ 59, secs : 0 ~ 59
	//(void)rtcTimeRead(&nCurHour, &nCurMin, &nCurSecs);

	// 시간당 초수
	for (UINT8 nIdx = 0; nIdx < nCurHour; nIdx++)
	{
		nTotalSecs += 3600;
	}
	// 분당 초수
	for (UINT8 nIdx2 = 0; nIdx2 < nCurMin; nIdx2++)
	{
		nTotalSecs += 60;
	}
	// 초수
	nTotalSecs += nCurSecs;

	return nTotalSecs;

}

UINT16 Utility::convertUtcDateToLocal(UINT16 utcdate)
{
	
    // 2010년 1월 1일의 시간
    auto start = std::chrono::system_clock::from_time_t(0);

    // utcDate를 시간으로 변환
    auto duration = std::chrono::hours(24 * static_cast<int>(utcdate));
    auto timePoint = start + duration;

    // 시간을 std::time_t로 변환
    std::time_t time = std::chrono::system_clock::to_time_t(timePoint);

    // UTC 시간을 로컬 시간으로 변환
	std::tm localTime;
	localtime_s(&localTime, &time);

    // 필요한 정보를 추출하여 로컬 날짜를 계산
    int localYear = localTime.tm_year + 1900;
    int localMonth = localTime.tm_mon + 1; // 월은 0부터 시작하므로 +1
    int localDay = localTime.tm_mday;

    // 2010년 1월 1일부터의 로컬 날짜를 일 수로 계산
    uint16_t localDate = static_cast<uint16_t>((localYear - 2010) * 365 + localTime.tm_yday + 1);

    return localDate;

}

void Utility::makeSessionID(BYTE sessionId[3]) {
	auto now = std::chrono::system_clock::now();
	auto duration = now.time_since_epoch();
	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

	// 밀리초 단위의 현재 시간을 3바이트 크기로 축소
	int sessionIdInt = milliseconds % 0xFFFFFF;

	// 0xFFFFFF까지의 값을 0x00FFFFFF까지 확장
	sessionIdInt |= 0xFF000000;

	// sessionId를 BYTE 배열로 변환
	sessionId[0] = (sessionIdInt >> 16) & 0xFF;
	sessionId[1] = (sessionIdInt >> 8) & 0xFF;
	sessionId[2] = sessionIdInt & 0xFF;
}

bool areArraysEqual(const int arr1[], const int arr2[], int size) {
	for (int i = 0; i < size; ++i) {
		if (arr1[i] != arr2[i]) {
			return false; // 배열 요소 중 하나라도 다르면 false 반환
		}
	}
	return true; // 모든 배열 요소가 동일하면 true 반환
}

CString Utility::utc_to_local_date_time(UINT16 date_bits, UINT32 seconds_bits)
{
	std::tm start_date = {};
	start_date.tm_year = 2010 - 1900;
	start_date.tm_mon = 0;
	start_date.tm_mday = 1;
	start_date.tm_hour = 0;
	start_date.tm_min = 0;
	start_date.tm_sec = 0;

	// 날짜와 초를 각각 15비트와 17비트로 나누기
	int date_decimal = date_bits;
	int seconds_decimal = seconds_bits;

	// UTC 시작 시간부터 더해야 할 초 계산
	int total_seconds = date_decimal * 86400 + seconds_decimal;

	// UTC 시작 시간에 더한 후 로컬 시간으로 변환
	std::time_t raw_time = std::mktime(&start_date) + total_seconds;
	std::tm local_time = {};
	localtime_s(&local_time, &raw_time);

	// 로컬 시간을 yy/mm/dd hh:mm:ss 형식으로 변환하여 반환
	char buffer[20];
	std::strftime(buffer, 20, "%Y/%m/%d %H:%M:%S", &local_time);
	return CString(buffer);
}


void Utility::INIWriteString(CString strAppName, CString strKeyName, CString strValue, CString strFilePath)
{
	WritePrivateProfileString(strAppName, strKeyName, strValue, strFilePath);
}

CString Utility::INIReadString(CString strAppName, CString strKeyName, CString strFilePath)
{
	char szReturnString[1024] = { 0, };

	memset(szReturnString, NULL, 1024);

	GetPrivateProfileString(strAppName, strKeyName, "", szReturnString, 1024, strFilePath);

	CString str;

	str.Format("%s", szReturnString);

	return str;
}

CString Utility::GetExePath(CCriticalSection& g_criticalExe)
{
	g_criticalExe.Lock();

	TCHAR pBuf[MAX_PATH] = { 0 }; // MAX_PATH로 버퍼 크기 조정
	DWORD dwSize = GetModuleFileName(NULL, pBuf, MAX_PATH);

	if (dwSize == 0) {
		// GetModuleFileName 실패
		g_criticalExe.Unlock();
		return _T("");
	}

	CString strFilePath(pBuf);
	strFilePath = strFilePath.Left(strFilePath.ReverseFind(_T('\\')));

	g_criticalExe.Unlock();

	return CStringT<char, StrTraitMFC<char, ChTraitsCRT<char>>>(strFilePath);
}

std::vector<UINT8> Utility::intToBCD(UINT32 num)
{
	std::vector<UINT8> bcd;
	while (num != 0) {
		bcd.insert(bcd.begin(), (num % 10));
		num /= 10;
	}
	return bcd;
}

void Utility::padBCD(std::vector<UINT8>& bcd, size_t totalBytes)
{
	while (bcd.size() < totalBytes * 2) {
		bcd.push_back(0xF);
	}
}



//void Utility::PacketHSAckStructureToJson(TS_OB_Packet01_HSAckStruct& p)
//{
//	Json::Value root;
//	Json::Value integer_value;
//	Json::StyledWriter writer;
//	ofstream output_file("HS Ack.json");
//	std::string str;
//
//	integer_value["NID_C"] = p.NID_C;
//	integer_value["NID_ATOTS"] = p.NID_ATOTS;
//	integer_value["M_ATO_Version"] = p.M_ATO_Version;
//	integer_value["T_Timeout_ATOTS_Response"] = p.T_Timeout_ATOTOS_Response;
//	integer_value["T_Reporting_Time"] = p.T_Reporting_Time;
//
//	root["HandShake Acknowledgement"] = integer_value;
//
//
//	str = writer.write(root);
//	output_file << str;
//	output_file.close();
//}
//
//void Utility::PacketJPStructureToJson(TS_OB_Packet04_JPStruct& p)
//{
//	Json::Value root;
//	Json::Value int_value;
//	Json::Value sp_iter;
//	Json::Value tp_iter;
//	Json::Value tc_iter;
//
//	Json::StyledWriter writer;
//	ofstream output_file("JP.json");
//	std::string str;
//
//	int_value["Q_JP_Status"] = p.Q_JP_Status;
//	int_value["N_ITER_SP"] = p.N_ITER_SP;
//	
//	root["Journey Profile"] = int_value;
//
//	for (int k = 0; k < p.N_ITER_SP ; ++k)
//	{
//		sp_iter["NID_C"] = p.segmentProfileReference[k].NID_C;
//	}
//
//	int_value["SP_ITER"] = sp_iter;
//
//	str = writer.write(root);
//	output_file << str;
//	output_file.close();
//
//}

UINT16 Utility::getUtcDateSince2010()
{
	// 현재 시간
	auto now = std::chrono::system_clock::now();

	// 2010년 1월 1일의 시간
	auto start = std::chrono::system_clock::from_time_t(0);
	auto duration = now - start;

	// 일 수로 변환
	auto days = std::chrono::duration_cast<std::chrono::hours>(duration).count() / 24;

	// 최대 15비트로 날짜 값을 제한
	const UINT16 maxDateValue = (1 << 15) - 1;
	return static_cast<UINT16>(days & maxDateValue);
	
}

UINT32 Utility::getTodaySeconds() {
	// 현재 UTC 시간을 얻기
	auto now = std::chrono::system_clock::now();
	std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

	// 현재 날짜의 시작(00:00:00) 시간을 얻기
	std::tm startOfDay;
	gmtime_s(&startOfDay, &currentTime);
	startOfDay.tm_hour = 0;
	startOfDay.tm_min = 0;
	startOfDay.tm_sec = 0;
	std::time_t startOfDayTime = std::mktime(&startOfDay);

	// 현재 시간과 시작 시간의 차이를 초로 계산
	std::chrono::duration<UINT32> secondsSinceStart = std::chrono::duration_cast<std::chrono::seconds>(now - std::chrono::system_clock::from_time_t(startOfDayTime));

	// 17비트로 자르기
	UINT32 result = secondsSinceStart.count() & 0x1FFFF;

	return result;
}
void ProcessWindowMessage()

{

	MSG msg;

	while (::PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))

	{

		::SendMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);

	}

}
#pragma once
#include "F110_Radio_Packet.h"


class Utility
{
public:
	
	static void CreateDialogForTapCtrl(CDialogEx* dlg, int ctrlID, CTabCtrl* tap);
	static void SelectTabOfTabCtrl(CTabCtrl* tab, int id);

	/*static void PacketHSAckStructureToJson(TS_OB_Packet01_HSAckStruct& p);
	static void PacketJPStructureToJson(TS_OB_Packet04_JPStruct& p);*/

	

	static DWORD IPtoDWORD(const CHAR* address);
	static DWORD IPtoDWORD(const WCHAR* address);
	static string DWORDtoIP(DWORD address);
	static BOOL CFileWriteTCHAR(CFile* f, const TCHAR* txt);
	static BOOL CFileWriteCHAR(CFile* f, const CHAR* txt);

	static variant_t CastVariantT(const variant_t& insertVal, const VARTYPE& castType);
	static void SetValueWithTypeCasting(CMFCPropertyGridProperty* targetProperty, variant_t val);

	static UINT32 CalculateCRCValue(const UINT8* bData, UINT16 nDataLen);
	static BOOL CheckFileUnicode(CFile& f);

	static CString GetDirectoryFromFileDialog(LPCTSTR lpszDefExt = (LPCTSTR)0, LPCTSTR fileName = (LPCTSTR)0, DWORD dwFlags = 6UL, LPCTSTR lpszFilter = (LPCTSTR)0);


	static void TextToByteArray_MemoryAlloc(const TCHAR* txt, byte*& buffer, size_t& length);

	static byte LetterToByte(const TCHAR txt);
	static std::vector<UINT8> tokenizeString16(const CString& str);



	static UINT32 ValueToBCD32(UINT32 val);
	static UINT32 BCD32ToValue(UINT32 val);
	void ATO_Memcpy(UINT8* Dest, const UINT8* Src, const UINT32 DestSize, UINT32 SrcSize);

	static UINT32 DecodeByte(const UINT8* szData, const UINT32 offset, const UINT32 nBitLen, BOOL bitsetOrder = TRUE);
	static BOOL Decode_uint8(const UINT8* szData, const UINT32 nDataLen, const UINT32 offset, const UINT32 nBitLen, UINT8* pResult);
	static BOOL Decode_uint16(const UINT8* szData, const UINT32 nDataLen, const UINT32 offset, const UINT32 nBitLen, UINT16* pResult);
	static BOOL Decode_uint32(const UINT8* szData, const UINT32 nDataLen, const UINT32 offset, const UINT32 nBitLen, UINT32* pResult);
	static BOOL Decode_int16(const UINT8* szData, const UINT32 nDataLen, const UINT32 offset, const UINT32 nBitLen, INT16* pResult);
	static BOOL Decode_int32(const UINT8* szData, const UINT32 nDataLen, const UINT32 offset, const UINT32 nBitLen, INT32* pResult);

	

	static void EncodeByte(UINT8* szOutData, const UINT32 offset, const UINT32 nBitLen, const UINT8 value);
	static void EncodeByte_Reverse(UINT8* szOutData, const UINT32 offset, const UINT32 bitLen, const UINT8 value);
	static BOOL Encode_uint8(UINT8* szOutData, UINT32 nDataLen, const UINT32 offset, const UINT32 nBitLen, const UINT8 value);
	static BOOL Encode_uint16(UINT8* szOutData, UINT32 nDataLen, const UINT32 offset, const UINT32 nBitLen, const UINT16 value);
	static BOOL Encode_uint32(UINT8* szOutData, UINT32 nDataLen, const UINT32 offset, const UINT32 nBitLen, const UINT32 value);

	
	static int CMperSecondToKMperHour(int cms);

	static void SecondsToHourMinuteSecond(UINT32 input, int& hour, int& minute, int& second);
	static void MiliSecondsToHourMinuteSecondMs(UINT32 input, int& h, int& m, int& s, int& ms);


	static bool GetButtonChectState(HWND hButton);

	static UINT16 getUtcDateSince2010();
	static UINT32 getTodaySeconds();
	static void ProcessWindowMessage();

	template <typename T>
	static void SafeDelete(T* p);

	template <typename T>
	static void SafeDeleteArray(T* p);

	template <typename T>
	static void ClearPtrStack(stack<T*>*& st);

	template <typename T>
	static void ClearPtrStack(stack<T*>& st);


	static bool areArraysEqual(const BYTE arr1[], const BYTE arr2[], int size);
	// Time
	UINT16 GetUTCTimeStamp_Date(void);
	UINT32 GetUTCTimeStamp_Seconds(void);
	UINT16 convertUtcDateToLocal(UINT16 utcdate);
	static void makeSessionID(BYTE sessionId[3]);

	static CString utc_to_local_date_time(UINT16 date_bits, UINT32 seconds_bits);

	// INI
	static void INIWriteString(CString strAppName, CString strKeyName, CString strValue, CString strFilePath);
	static CString INIReadString(CString strAppName, CString strKeyName, CString strFilePath);
	static CString GetExePath(CCriticalSection& g_criticalExe);

	// BCD
	static std::vector<UINT8> intToBCD(UINT32 num);
	static void padBCD(std::vector<UINT8>& bcd, size_t totalBytes);

};

template<typename T>
inline void Utility::SafeDelete(T* p)
{
	if (p != nullptr)delete p, p = nullptr;
}

template<typename T>
inline void Utility::SafeDeleteArray(T* p)
{
	if (p != nullptr)delete[] p, p = nullptr;
}

template<typename T>
inline void Utility::ClearPtrStack(stack<T*>*& st)
{
	if (st != nullptr)
	{
		ClearPtrStack(*st);
		delete st;
	}
	st = nullptr;
}

template<typename T>
inline void Utility::ClearPtrStack(stack<T*>& st)
{
	while (st.empty() == 0)
	{
		Utility::SafeDelete(st.top());
		st.pop();
	}
}

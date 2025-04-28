#include "CLogListCtrl.h"
#include "ATOTS_Simulator(tab)Dlg.h"



CLogListCtrl::CLogListCtrl() :itemMaxCount(55000), threadRun(TRUE)
{
	
}

CLogListCtrl::~CLogListCtrl()
{
}

//파일 이름을 생성해주는 함수
static tstring MakeFileNameWithTimeStamp(const TCHAR* frontfilename)
{
	time_t t = chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	tm tm_;
	localtime_s(&tm_, &t);
	TCHAR path[128];
	_stprintf_s(path, _T("_%02d%02d%02d.%02d.log"), tm_.tm_year % 100, tm_.tm_mon + 1, tm_.tm_mday, tm_.tm_hour);
	return frontfilename + tstring(path);
}


//해당하는 인덱스의 값을 파일에 저장
void CLogListCtrl::WriteStringOfPacket(CStdioFile& ipfile, const int idx) const
{
	ipfile.WriteString(GetItemText(idx, LogColumn_Time));
	ipfile.WriteString(_T(" ["));
	ipfile.WriteString(GetItemText(idx, LogColumn_Command));
	ipfile.WriteString(_T(":"));
	ipfile.WriteString(GetItemText(idx, LogColumn_Type));
	ipfile.WriteString(_T("] "));
	ipfile.WriteString(GetItemText(idx, LogColumn_Binary));
	ipfile.WriteString(_T("\n"));
}


BOOL CLogListCtrl::SaveLogFile()
{
	if (GetItemCount() < 1)return FALSE;
	BOOL result = TRUE;
	CStdioFile ipfile;

	tstring dir = MakeFileNameWithTimeStamp(_T("TS_Simulator_Packet_Log"));

	//이어쓰기
	result = ipfile.Open(dir.c_str(), CFile::modeWrite);
	if (result == FALSE)
	{
		result = ipfile.Open(dir.c_str(), CFile::modeCreate | CFile::modeWrite);
		if (result == FALSE) return FALSE;
	}
	ipfile.SeekToEnd(); //기존파일이 있는경우 포인터 맨뒤로

	const int lines = GetItemCount();
	for (int i = 0; i < lines; ++i)
	{
		WriteStringOfPacket(ipfile, i);
	}

	ipfile.Close();
	return result;
}

BOOL CLogListCtrl::SaveLogFileAppend(int tabValue)
{
	BOOL result = TRUE;
	CStdioFile ipfile;
	
	tstring dir;

	switch (tabValue)
	{
	case TS_1:
		dir = MakeFileNameWithTimeStamp(_T("TS_Simulator_Packet_Log"));
		break;
	case TS_2:
		dir = MakeFileNameWithTimeStamp(_T("TS2_Simulator_Packet_Log"));
		break;
	}
	//파일 열기
	result = ipfile.Open(dir.c_str(), CFile::modeWrite);
	if (result == FALSE)
	{
		result = ipfile.Open(dir.c_str(), CFile::modeCreate | CFile::modeWrite);
		if (result == FALSE) return FALSE;
	}
	ipfile.SeekToEnd(); //포인터 맨뒤로

	//이어쓰기
	const int topIdx = GetItemCount() - 1;
	WriteStringOfPacket(ipfile, topIdx);

	ipfile.Close();
	return result;
}

BOOL CLogListCtrl::isPauseChecked(BOOL logPause)
{
	int currentCount = GetItemCount();
	if (currentCount >= itemMaxCount)
	{
		//현재 개수가 최대개수보다 많은 경우 전부 지우고 개수 0으로 설정
		DeleteAllItems();
		currentCount = 0;
	}

	const int topIdx = currentCount;

	if (logPause == FALSE) EnsureVisible(topIdx, FALSE);
	return TRUE;
}

void CLogListCtrl::ThreadStop()
{
	threadRun = FALSE;
}

void CLogListCtrl::OnNMCustomdraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVCUSTOMDRAW lplvcd = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);
	CString value;
	CString value2;
	switch (lplvcd->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;

	case CDDS_ITEMPREPAINT:
		// 현재 행의 데이터 값 얻기
		value = GetItemText(static_cast<int>(lplvcd->nmcd.dwItemSpec), 3);  // 1은 두 번째 열
		value2 = GetItemText(static_cast<int>(lplvcd->nmcd.dwItemSpec), 1);

		// 특정 조건에 따라 처리
		if (value == _T("Receive") && value2 != _T("KVC.Alive")) {
			lplvcd->clrTextBk = RGB(144, 238, 144); //초록색 
			lplvcd->clrText = RGB(0, 0, 0);
		}

		value = GetItemText(static_cast<int>(lplvcd->nmcd.dwItemSpec), 3);
		if (value == _T("Error")) {
			lplvcd->clrTextBk = RGB(233, 0, 0); // 붉은색
			lplvcd->clrText = RGB(255, 233, 255); // 흰색
		}
	}
}
BEGIN_MESSAGE_MAP(CLogListCtrl, CListCtrl)
ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CLogListCtrl::OnNMCustomdraw)
END_MESSAGE_MAP()


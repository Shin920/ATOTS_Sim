// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

#pragma warning(disable  : 26454)
#define _CRTDBG_MAP_ALLOC
// 여기에 미리 컴파일하려는 헤더 추가
#include "framework.h"

#include <WS2tcpip.h>
#include <WinSock2.h>
#include <assert.h>
#include <chrono>
#include <functional>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <list>
#include <map>
#include <mutex>
#include <queue>
#include <stack>
#include <sstream>
#include <string>
#include <tchar.h>
#include <thread>
#include <unordered_map>
#include <vector>
#include <bitset>
#include <future>
#include <stdlib.h>
#include <crtdbg.h>

using namespace std;

#include "json.h"

#include "Utility.h"

extern const UINT32 KR37_CRC32Table[256];
extern const TCHAR* HSAckDefaultConfigFileName;
extern const TCHAR* HSRejDefaultConfigFileName;
extern const TCHAR* JPDefaultConfigFileName;
extern const TCHAR* SPDefaultConfigFileName;
extern const TCHAR* STRAckDefaultConfigFileName;

extern const TCHAR BASED_CODE szConfigFilter[];


extern const CHAR* packetHSAck;
extern const CHAR* packetHSRej;
extern const CHAR* packetJP;
extern const CHAR* packetSP;
extern const CHAR* packetSTRAck;


extern const CHAR* endLine;

enum class LogProperty : int
{
	HEADER,
	BODY
};


enum WND_MSG_type
{
	WM_USERMSG_START = WM_USER,
	WM_PARSEPACKET00,
	WM_PARSEPACKET01,
	WM_CONSTRUCTPACKET05,
	WM_CONSTRUCTPACKET06,
	WM_CONSTRUCTPACKET07,

	WM_SESSIONSTATUSCHANGE,
	WM_INSERTPACKET
};

enum SendPacketTypes
{
	PacketNotSel = -1,
	eHsAck = 0, eHsRej = 1, eJP = 2, eSP = 3, eSTRAck = 4, eSeTrmReq = 5,
	PacketMAXV
};

enum RecvPacketTypes
{
	PacketNotSelected = -1,
	eHsReq = 0, eJPReq = 1, eJPAck = 2, eSPReq = 3, eSTR = 4,
	PacketMAX
};

enum allPacketTypes
{
	NoPacket = -1,
	aHSReq = 0, aHSAck = 1, aHSRej = 2, aJPReq = 3, aJP = 4,
	aJPAck = 5, aSPReq = 6, aSP = 7, aSTR = 8, aSTRAck = 9,
	aSeTermReq = 10, aSeTerm = 11,
	MaxPacket
};

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef UNICODE
#define tstring wstring
#else
#define tstring string
#endif //UNICODE

#endif //PCH_H
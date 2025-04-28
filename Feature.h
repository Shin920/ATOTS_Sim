#pragma once
#define _LS          _fnLoadString

#define _WORKING_UI_		TRUE

#define _DISPLAY_KVC_		TRUE
#define _DISPLAY_RBC_		TRUE

#define COLOR_MAIN_DIALOG_BACKGROUND		RGB(240,240,240)
#define COLOR_SUB_DIALOG_BACKGROUND		 RGB(254, 254, 254)

/*
	Throughput Test 시 True
*/
#define _THROUGHPUT_TEST_	FALSE

/* 환경시험용 시뮬레이터 생성시 True
* 아래 2개 사항이 반영됨.
* 1. 환경 시험 시작 및 시험 성공, 실패 등의 로그가 추가되고
* 2. RBC ID = 39 로 고정
*/
#define _ONLY_FOR_ENV_TEST_ FALSE

#if(_DISPLAY_RBC_)
// RBC-RBC(Subset-098) Simulator 생성시에는 아래 Feautre 를 TRUE 로
#define	_RBC_RBC_SIMULATOR	TRUE
#endif


/*
아래 Feature 는 Sequence Number 가 존재하지 않는 Old Version 일 경우 define 하여 Compile 한다.
*/
//#define RBC_OLD_VERSION_EXCEPT_SEQUENCE#pragma once

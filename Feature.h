#pragma once
#define _LS          _fnLoadString

#define _WORKING_UI_		TRUE

#define _DISPLAY_KVC_		TRUE
#define _DISPLAY_RBC_		TRUE

#define COLOR_MAIN_DIALOG_BACKGROUND		RGB(240,240,240)
#define COLOR_SUB_DIALOG_BACKGROUND		 RGB(254, 254, 254)

/*
	Throughput Test �� True
*/
#define _THROUGHPUT_TEST_	FALSE

/* ȯ������ �ùķ����� ������ True
* �Ʒ� 2�� ������ �ݿ���.
* 1. ȯ�� ���� ���� �� ���� ����, ���� ���� �αװ� �߰��ǰ�
* 2. RBC ID = 39 �� ����
*/
#define _ONLY_FOR_ENV_TEST_ FALSE

#if(_DISPLAY_RBC_)
// RBC-RBC(Subset-098) Simulator �����ÿ��� �Ʒ� Feautre �� TRUE ��
#define	_RBC_RBC_SIMULATOR	TRUE
#endif


/*
�Ʒ� Feature �� Sequence Number �� �������� �ʴ� Old Version �� ��� define �Ͽ� Compile �Ѵ�.
*/
//#define RBC_OLD_VERSION_EXCEPT_SEQUENCE#pragma once

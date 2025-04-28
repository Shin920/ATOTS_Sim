#ifndef DEFINE_H_
#define DEFINE_H_

/**
 *  KTCS-3 ATO 협의체 결정 내용 반영 Define
 */
//#define KTCS_3
//#define CheckValueSpare
 /***/
 /********************************************************************************************
  *                                        UTC Time
  *******************************************************************************************/

#define LOCAL static
#define PROCESS void

#define STATUS int
#define FOREVER	for (;;)

#define MAX_DATE_RANGE 32767
#define TIMESTAMP_REFERENCE_YEAR 2010
#define TIMESTAMP_REFERENCE_MONTH 1
#define TIMESTAMP_REFERENCE_DAY 1

#define WAIT_TIME_MSG_QUEUE		(DEFAULT_SYS_CLOCK_RATE / 100) // 100ms

#define STX_VALUE               0x02
#define ETX_VALUE               0x03
#define DIR_UP                  0
#define DIR_DOWN                1
#define SEQ_RANGE               0x03
#define MAX_TASK_MSG_FAIL_COUNT      3

  //#define NULL ((void*)0)

typedef enum
{
    MSG_STATUS_NONE,
    MSG_STATUS_FIND_STX,
    MSG_STATUS_READING,
    MSG_STATUS_ERROR,
    MSG_STATUS_MAX
} MSG_STATUS_TYPE;

typedef enum
{
    UNIT_1,
    UNIT_2,
} UNIT_Type_Enum;

#endif  /* DEFINE_H */



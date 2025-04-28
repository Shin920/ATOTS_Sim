#ifndef ATO_TYPES_H
#define ATO_TYPES_H

#include "define.h"
#include <string.h>
/***************************************************************************************************************
 *
 *                                          ATO_types.h - type defined
 *
 * *************************************************************************************************************/
 /** Boolean Type */
//typedef enum
//{
//    FALSE,
//    TRUE
//} BOOL;

/**< Unsigned Integer 8 bits wide. */
typedef unsigned char  UINT8;
/**< Unsigned Integer 16 bits wide. */
typedef unsigned short int UINT16;
/**< Unsigned Integer 32 bits wide. */
typedef unsigned int UINT32;

typedef char        CHAR;
/**< A 64 bits unsigned integer */
typedef unsigned long long  UINT64;

/**< signed Integer 8 bits wide. */
typedef signed char INT8;
/**< signed Integer 16 bits wide. */
typedef signed short int  INT16;
/**< signed Integer 32 bits wide. */
typedef signed int INT32;
/**< A 64 bits signed integer */
typedef signed long long  INT64;

typedef float FLOAT32;

/** FUNCPTR */
//typedef int (*FUNCPTR)(...);
/** Watch dog ID */
//typedef uintptr_t WDOG_ID;

typedef struct		/* RING - ring buffer */
{
    int pToBuf;		/* offset from start of buffer where to write next */
    int pFromBuf;	/* offset from start of buffer where to read next */
    int bufSize;	/* size of ring in bytes */
    char* buf;		/* pointer to start of buffer */
} RING;

typedef RING* RING_ID;
typedef int _Vx_OBJ_HANDLE;
typedef _Vx_OBJ_HANDLE  OBJ_HANDLE;
typedef OBJ_HANDLE  TASK_ID;
typedef OBJ_HANDLE  MSG_Q_ID;
#define PACKED  __attribute__ ((packed))
#define MSG_Q_FIFO	0x00	/* tasks wait in FIFO order */



#endif

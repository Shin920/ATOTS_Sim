#include <minwindef.h>


#ifndef __KR37_ATP_MSG_H__
#define __KR37_ATP_MSG_H__

/*
 * ���� 4 bit : request(0), confirm(1), indication(2), response(3)
 * ���� 4 bit : connect(0), data(1), disconnect(2)
 * */
typedef enum
{
    // Request
    OPCODE_CONNECT_REQUEST = 0x00, // 1. KVC -> SFM
    OPCODE_DATA_REQUEST = 0x01,
    OPCODE_DISCONNECT_REQUEST = 0x02,
    // confirm
    OPCODE_CONNECT_CONFIRM = 0x10, //      4. SFM -> KVC
    // indication
    OPCODE_CONNECT_INDICATION = 0x20, //      2. SFM -> RBC
    OPCODE_DATA_INDICATION = 0x21,
    OPCODE_DISCONNECT_INDICATION = 0x22,
    // response
    OPCODE_CONNECT_RESPONSE = 0x30, // 3. RBC -> SFM

    // user create
    OPCODE_NONE = 0xA0,
    OPCODE_STU_INIT,
    OPCODE_STU_STATUS_REQ,                  // ctrl -> sfm ���� ��ȸ ��û
    OPCODE_STU_STATUS_INDI,                 // sfm -> ctrl ���� �� ����
    OPCODE_STU_ACCEPT_REQ,                  // RBC tcp accept ���� ctrl -> sfm ���� connect ��û
    OPCODE_STU_CONNECT_INDI_EXTRA,
    OPCODE_STU_CONNECT_CONF_EXTRA,
    OPCODE_STU_SCM_ALIVE_EXTRA,
    OPCODE_STU_TERMINATE = 0xAF,
    // user create

    // else
    OPCODE_SCM_ALIVE = 0xFF,
    OPCODE_KVC_ALIVE = 0xFE
} ATP_Message_OpCode_Enum;

typedef enum
{
    STU_STATUS_NONE = 0,        // Process Start �� �ؾ��ϴ� ����
    STU_STATUS_INIT,            // Initialize ��
    STU_STATUS_READY,           // �ʱ�ȭ �Ϸ�
    STU_STATUS_CONNECTING,      // ���� ���� �� : AU1~3
    STU_STATUS_WAITING_AR,      // ���� ���� �� : Connect_Response ��ٸ��� ���� , AR
    STU_STATUS_CONNECTED,       // ���� ���� �Ϸ�
    STU_STATUS_DISCONNECTING,   // ���� ���� ���� ��
    STU_STATUS_DISCONNECTED,    // ���� ����
    STU_STATUS_ERROR,           // ������ ���� ���μ��� ���ᰡ ����Ǵ� ����
    STU_STATUS_KILL_PROCESS,    // SFM, CFM ���μ��� �� �� �ϳ��� ���� ����
    // �� ���̿� �߰�, STU_STATUS_MAX �� 0x0F(15) �� �Ѿ�� �ȵȴ�.
    STU_STATUS_MAX              // MAX
} KR37_STU_STATUS_Enum;

typedef enum
{
    STU_ERROR_NONE = 0,
    STU_ERROR_INIT,                 // Lib init �� ����
    STU_ERROR_SOCK_CREATE_UDP,      // client socket creat ����
    STU_ERROR_SOCK_CREATE_IMC,      // IMC ����� client socket creat ����
    STU_ERROR_CONN_IMC_TIMEOUT,     // cfm �� imc ���� ���Ͽ� ���� ����
    STU_ERROR_SOCK_BIND_UDP,        // ctrl ���� data ���ſ� udp socket binding ����
    STU_ERROR_SOCK_BIND_IMC,        // imc tcp socket bind ����
    STU_ERROR_ETCS_INFO,            // peer ����(ETCS ID, ETCS Type, applicatin type...)
    STU_ERROR_CONN_TIMEOUT,         // connection timeout
    STU_ERROR_KEY,                  // KMAC ����
    STU_ERROR_FIFO_FULL,            // IMC Fifo Full
    STU_ERROR_SESSION_ID,           // SaCEPID ����
    STU_ERROR_DISCONNECT_REASON,    // disconnect request reason ���� ( 0 < reason < 127 �ϰ�� ó�� �ȵ�)
    STU_ERROR_ETC                   // ��Ÿ �� ���ǵ� ����

} KR37_STU_ERROR_Eum;

typedef enum
{
    ETCS_ID_TYPE_RADIO, // Radio In-fill unit
    ETCS_ID_TYPE_RBC,   // RBC
    ETCS_ID_TYPE_ENGINE,// Engine(���� ��ǻ��)
    ETCS_ID_TYPE_BALISE,// �߸���
    ETCS_ID_TYPE_KEY,   // Key management entity
    ETCS_ID_TYPE_INTER, // Interlocking related entity
    ETCS_ID_TYPE_RESERVED // 6~7 Reserved
} ATP_ETCS_ID_Type;

#pragma pack(push, 1)

/*
 * KVC, RBC -> SFM Control Message
 * SFM ���� parsing �ؾ� �ϴ� message
 * */
 // KVC -> SFM
typedef struct SA_MSG_COMMON_HEADER_
{
    BYTE      stx;    // 0x02, fixed value
    BYTE      length[2]; // 2 bytes
    BYTE      sequence;
    BYTE      opcode;
} SA_MSG_COMMON_HEADER;

typedef struct SA_MSG_COMMON_HEADER_RBC_
{
    BYTE      stx;    // 0x02, fixed value
    BYTE      length[2]; // 2 bytes
    BYTE      opcode;    
} SA_MSG_COMMON_HEADER_RBC;

typedef struct SA_MSG_COMMON_TAIL_
{
    BYTE  crc32[4];
    BYTE  etx;    // 0x03, fixed value
} SA_MSG_COMMON_TAIL;

typedef struct SA_MSG_ETCS_ID_
{
    BYTE  etcs_id_type;
    BYTE  etcs_id[3];
} SA_MSG_ETCS_ID;

typedef struct SA_CONNECT_REQUEST_BODY_
{
    SA_MSG_ETCS_ID responder;
    SA_MSG_ETCS_ID initiator;
    BYTE         app_type;
} SA_CONNECT_REQUEST_BODY;

typedef struct SA_CONNECT_INDICATION_BODY_
{
    BYTE         sacepid[3];
    SA_MSG_ETCS_ID responder;
    SA_MSG_ETCS_ID initiator;
    BYTE  app_type;
} SA_CONNECT_INDICATION_BODY;


typedef struct SA_CONNECT_RESP_CONF_BODY_
{
    BYTE         sacepid[3];
    SA_MSG_ETCS_ID responder;
} SA_CONNECT_RESP_CONF_BODY;

typedef struct SA_DATA_BODY_
{
    BYTE  sacepid[3];
    BYTE  data[1023];
} SA_DATA_BODY;

/* Data �ۼ��� Test �� ���� Code */
typedef struct SA_DATA_TH_BODY_
{
    BYTE  index[2];
    BYTE  data[300];
} SA_DATA_TH_BODY;


typedef struct SA_DISCONNECT_BODY_
{
    BYTE  sacepid[3];
    BYTE  disconnect_reason;
    BYTE  disconnect_sub_reason;
} SA_DISCONNECT_BODY;

typedef struct SA_SCM_ALIVE_BODY_
{
    BYTE  count;      // 0x01 ~ 0xFF, 1�� ����
    BYTE  unit;       // SCM 1 : 0x01, SCM 2 : 0x02
    BYTE  sess_num;   // count of Sessions
} SA_SCM_ALIVE_BODY;

typedef struct SA_SCM_ALIVE_EXTRA_BODY_
{
    BYTE  sacepid[3];
    BYTE  count;      // 0x01 ~ 0xFF, 1�� ����
} SA_SCM_ALIVE_EXTRA_BODY;

typedef struct SA_SCM_STATUS_BODY_
{
    SA_MSG_ETCS_ID initiator;
    BYTE  imc_port[3];
    BYTE  state;      // KR37_STU_STATUS_Enum
    BYTE  error_code; // KR37_STU_ERROR_Eum
} SA_SCM_STATUS_BODY;

/**
 * User Define Command
 */
typedef struct SA_SESSION_INDICATION_BODY_
{
    BYTE  sacepid[3];
    BYTE  imc_port[3];
} SA_SESSION_INDICATION_BODY;

typedef struct SA_STU_ACCEPT_REQUEST_BODY_
{
    BYTE  imc_port[3];
} SA_STU_ACCEPT_STATUS_REQ_BODY;


typedef struct SA_MESSAGE_STRUCTURE_
{
    SA_MSG_COMMON_HEADER            header;

    union {
        SA_CONNECT_REQUEST_BODY     connect_req;
        SA_CONNECT_INDICATION_BODY  connect_indi;
        SA_CONNECT_RESP_CONF_BODY   connect_resp_conf;     // response or confirm
        SA_DATA_BODY                data;
        SA_DISCONNECT_BODY          disconnect;
        SA_SCM_ALIVE_BODY           scm_alive;
        SA_SCM_STATUS_BODY          scm_status;
    } body;

    SA_MSG_COMMON_TAIL tail;
} SA_MESSAGE_STRUCTURE;

typedef struct SA_MESSAGE_STRUCTURE_RBC_
{
    SA_MSG_COMMON_HEADER_RBC    header;

    union {
        SA_CONNECT_INDICATION_BODY  connect_indi;
        SA_CONNECT_RESP_CONF_BODY   connect_resp;     // response or confirm
        SA_DATA_BODY                data;
        SA_DISCONNECT_BODY          disconnect;
        SA_SCM_ALIVE_BODY           scm_alive;

        SA_SCM_STATUS_BODY          scm_status;
    } body;

    SA_MSG_COMMON_TAIL tail;

} SA_MESSAGE_STRUCTURE_RBC;

typedef struct SA_MESSAGE_STRUCTURE_KVC_ALIVE
{
    SA_MSG_COMMON_HEADER            header;
    SA_MSG_COMMON_TAIL tail;
} SA_MESSAGE_STRUCTURE_KVC_ALIVE;

typedef struct SA_MESSAGE_STRUCTURE_STU_ALIVE
{
    SA_MSG_COMMON_HEADER            header;
    SA_MSG_COMMON_TAIL tail;
} SA_MESSAGE_STRUCTURE_STU_ALIVE;
#pragma pack(pop)

#endif
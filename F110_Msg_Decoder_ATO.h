#include "ATO_Types.h"
#include "F110_Radio_Packet.h"

/**
 *
 * @param pInBuf
 * @param nInBufLen
 * @param pHeader
 * @return
 */
BOOL DecodeMessage_ATOHeader(const byte* pInBuf, UINT32 nInBufLen, UINT32& offset, ATO_Header* pHeader);

/**
 * ATO OB�� ���� ������ HandShake Req Packet ���ڵ�
 *
 * @param pInBuf
 * @param nInBufLen
 * @param pMsg
 * @return
 */
BOOL DecodeMessage_ATO_Packet00_HsReq(OB_TS_Packet00_HSReqStruct* pMsg, UINT32& offset, const UINT8* pInBuf, UINT32 nInBufLen);

/**
 * ATO OB�� ���� ������ Journey Profile Request ���ڵ�
 *
 * @param pInBuf
 * @param nInBufLen
 * @param pMsg
 * @return
 */
BOOL DecodeMessage_ATO_Packet03_JpReq(OB_TS_Packet03_JPReqStruct* pMsg, UINT32& offset, const UINT8* pInBuf, UINT32 nInBufLen);
BOOL DecodeMessage_ATO_Packet04_JP(TS_OB_Packet04_JPStruct* pMsg, UINT32& offset, const UINT8* pInBuf, UINT32 nInBufLen);

/**
 * ATO OB�� ���� ������ JP Ack Packet ���ڵ� ó��
 * @param pInBuf
 * @param nInBufLen
 * @param pMsg
 * @return
 */
BOOL DecodeMessage_ATO_Packet05_JpAck(OB_TS_Packet05_JPAckStruct* pMsg, UINT32& offset, const UINT8* pInBuf, UINT32 nInBufLen);

/**
 * ATO OB�� ���� ������ SP Req Packet ���ڵ� ó��
 * @param pInBuf
 * @param nInBufLen
 * @param pMsg
 * @return
 */
BOOL DecodeMessage_ATO_Packet06_SpReq(OB_TS_Packet06_SPReqStruct* pMsg, UINT32& offset, const UINT8* pInBuf, UINT32 nInBufLen);
BOOL DecodeMessage_ATO_Packet07_SP(TS_OB_Packet07_SPStruct* pMsg, UINT32& offset, const UINT8* pInBuf, UINT32 nInBufLen);
/**
 * ATO OB�� ���� ������ Status Report Packet ���ڵ�
 * @param pInBuf
 * @param nInBufLen
 * @param pMsg
 * @return
 */
BOOL DecodeMessage_ATO_Packet08_STR(OB_TS_Packet08_STRStruct* pMsg, UINT32& offset, const UINT8* pInBuf, UINT32 nInBufLen);
BOOL DecodeMessage_ATO_Packet09_STRAck(TS_OB_Packet09_STRAckStruct* pMsg, UINT32& offset, const UINT8* pInBuf, UINT32 nInBufLen);

/**
 * ATO OB�� ���� ������ Session Termination Packet ���ڵ�
 * @param pInBuf
 * @param nInBufLen
 * @param messageStruct
 * @return
 */
BOOL DecodeMessage_ATO_Packet11_SessTerm(OB_TS_Packet11_SESSTermStruct* messageStruct, UINT32& offset, const UINT8* pInBuf, UINT32 nInBufLen);

BOOL DecodeMessage_ATO_Packet01_HsAck(TS_OB_Packet01_HSAckStruct* pOutMsg, UINT32& offset, const UINT8* pInBuf, UINT32 nInBufLen);

UINT32 GetLengthPacket00_HsReq(OB_TS_Packet00_HSReqStruct* pMsg);
UINT32 GetLengthPacket03_JpReq(OB_TS_Packet03_JPReqStruct* pMsg);
UINT32 GetLengthPacket05_JpAck(OB_TS_Packet05_JPAckStruct* pMsg);
UINT32 GetLengthPacket06_SpReq(OB_TS_Packet06_SPReqStruct* pMsg);
UINT32 GetLengthPacket08_STR(OB_TS_Packet08_STRStruct* pMsg);
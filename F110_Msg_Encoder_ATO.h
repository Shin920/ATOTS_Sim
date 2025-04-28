#include "ATO_Types.h"
#include "F110_Radio_Packet.h"

BOOL GetATOHeaderSize(void);

/**
* Encode ATO Header
* @param pOutBuf
* @param nOutBufLen
* @param offset
* @param pHeader
* @return
*/
BOOL Encode_ATOHeader(ATO_Header* pHeader, UINT32* offset, UINT8* pOutBuf, UINT32 nOutBufLen);

// �׽�Ʈ�� �߰�

/**
 * ATO-OB�� ������ Handshake Ack Packet ���ڵ�
 * @param pOutMsg
 * @param pOutBuf
 * @param nOutBufLen
 * @return
 */
BOOL EncodeMessage_ATO_Packet01_HsAck(TS_OB_Packet01_HSAckStruct* pOutMsg, UINT32* offset, UINT8* pOutBuf, UINT32 nOutBufLen);

/**
 * ATO-OB�� ������ Handshake Reject Packet ���ڵ�
 * @param pOutMsg
 * @param pOutBuf
 * @param nOutBufLen
 * @return
 */
BOOL EncodeMessage_ATO_Packet02_HsRej(TS_OB_Packet02_HSRejStruct* pOutMsg, UINT32* offset, UINT8* pOutBuf, UINT32 nOutBufLen);

/**
 * ATO-OB�� ������ JP Packet ���ڵ�
 * @param pOutMsg
 * @param pOutBuf
 * @param nOutBufLen
 * @return
 */
BOOL EncodeMessage_ATO_Packet04_Jp(TS_OB_Packet04_JPStruct* pOutMsg, UINT32* offset, UINT8* pOutBuf, UINT32 nOutBufLen);

/**
 * ATO-OB�� ������ SP Packet ���ڵ�
 * @param pOutMsg
 * @param pOutBuf
 * @param nOutBufLen
 * @return
 */
BOOL EncodeMessage_ATO_Packet07_Sp(TS_OB_Packet07_SPStruct* pOutMsg, UINT32* offset, UINT8* pOutBuf, UINT32 nOutBufLen);

/**
 * ATO-OB�� ������ STR Ack ���ڵ�
 * @param pOutMsg
 * @param pOutBuf
 * @param nOutBufLen
 * @return
 */
BOOL EncodeMessage_ATO_Packet09_STRAck(TS_OB_Packet09_STRAckStruct* pOutMsg, UINT32* offset, UINT8* pOutBuf, UINT32 nOutBufLen);

/**
 * ATO-OB�� ������ Session Termination Request ���ڵ�
 * @param pOutMsg
 * @param pOutBuf
 * @param nOutBufLen
 * @return
 */
BOOL EncodeMessage_ATO_Packet10_SESSTermReq(TS_OB_Packet10_SESSTermReqStruct* pOutMsg, UINT32* offset, UINT8* pOutBuf, UINT32 nOutBufLen);

UINT32 GetLengthPacket01_HsAck(TS_OB_Packet01_HSAckStruct* pMsg);
UINT32 GetLengthPacket02_HsRej(TS_OB_Packet02_HSRejStruct* pMsg);
UINT32 GetLengthPacket04_Jp(TS_OB_Packet04_JPStruct* pMsg);
UINT32 GetLengthPacket07_Sp(TS_OB_Packet07_SPStruct* pMsg);
UINT32 GetLengthPacket09_STRAck(TS_OB_Packet09_STRAckStruct* pMsg);
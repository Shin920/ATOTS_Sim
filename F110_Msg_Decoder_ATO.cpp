#include "pch.h"
#include "Utility.h"
#include "F110_Msg_Decoder_ATO.h"



BOOL Check_NID_PACKET_ATO(UINT8 NID_PACKET_ATO)
{
    BOOL nResult = TRUE;
    if (NID_PACKET_ATO >= ATO_Packet_Type_SPARE)
    {
        nResult = FALSE;
    }
    return nResult;
}


UINT32 GetATOHeaderSize(void)
{
    UINT32 nRet;
    nRet = LEN_NID_PACKET_ATO + LEN_L_PACKET_ATO + LEN_NID_OPERATIONAL + LEN_NID_ENGINE
        + LEN_T_TIMESTAMP_DATE + LEN_T_TIMESTAMP_SECONDS + LEN_N_PACKET_COUNTER;

    return nRet;
}

UINT32 GetLengthPacket00_HsReq(OB_TS_Packet00_HSReqStruct* pMsg)
{
    UINT32 nLength = 0;
    nLength += GetATOHeaderSize();

    if (pMsg->N_ITER > 0)
    {
        nLength += LEN_M_ATO_VERSION * (pMsg->N_ITER);
    }
    nLength = LEN_N_ITER;
    return nLength;
}

/**
 * Hs Request packet 03 길이 반환
 * @param pMsg
 * @return
 */
UINT32 GetLengthPacket03_JpReq(OB_TS_Packet03_JPReqStruct* pMsg)
{
    UINT32 nLength = 0;
    nLength += GetATOHeaderSize();
    nLength += LEN_NID_C + LEN_NID_SP;
    return nLength;
}


/**
 * Hs Request packet 05 길이 반환
 * @param pMsg
 * @return
 */
UINT32 GetLengthPacket05_JpAck(OB_TS_Packet05_JPAckStruct* pMsg)
{
    UINT32 nLength = 0;
    nLength += GetATOHeaderSize();
    nLength += LEN_T_JP_REFERENCE_TIMESTAMP_DATE + LEN_T_JP_REFERENCE_TIMESTAMP_SECONDS + LEN_N_JP_REFERENCE_PACKET_COUNTER;
    return nLength;
}


/**
 * Hs Request packet 05 길이 반환
 * @param pMsg
 * @return
 */
UINT32 GetLengthPacket06_SpReq(OB_TS_Packet06_SPReqStruct* pMsg)
{
    UINT32 nLength = 0;
    nLength += GetATOHeaderSize();
    if (pMsg->N_ITER_ATO > 0)
    {
        nLength += ((LEN_NID_C + LEN_NID_SP) * (pMsg->N_ITER_ATO));
    }
    nLength += LEN_N_ITER_ATO;
    return nLength;
}

/**
 * Hs Request packet 08 길이 반환
 * @param pMsg
 * @return
 */
UINT32 GetLengthPacket08_STR(OB_TS_Packet08_STRStruct* pMsg)
{
    UINT32 nLength = 0;
    nLength += GetATOHeaderSize();
    nLength += LEN_M_ATO_STATE + LEN_Q_STR_INDICATION + LEN_V_TRAIN_ATO + LEN_L_TRAIN + LEN_DRIVER_ID
        + LEN_NID_C + LEN_NID_SP + LEN_D_SENDING_POSITION + LEN_NID_C + LEN_NID_TP + LEN_Q_PASS_STOP_DEPART;
    if (pMsg->previousTpInfo.Q_Accurate_Stopping == Q_Pass_Stop_Depart_Stopped_TP)
    {
        nLength += LEN_Q_ACCURATE_STOPPING;
    }

    if (pMsg->N_ITER > 0)
    {
        nLength += ((LEN_NID_C + LEN_NID_TP + LEN_T_ARRIVAL_DATE + LEN_T_ARRIVAL_SECONDS) * pMsg->N_ITER);
    }
    nLength += LEN_N_ITER;
    return nLength;
}

/**
 * Hs Request packet 08 길이 반환
 * @param pMsg
 * @return
 */
LOCAL UINT32 GetLengthPacket11_SessTerm(OB_TS_Packet11_SESSTermStruct* pMsg)
{
    UINT32 nLength = 0;
    nLength += GetATOHeaderSize();
    nLength += LEN_Q_TERMINATION_REASON;
    return nLength;
}


BOOL Decode_Packet08_PrevTPInfo(const byte* pInBuf, UINT32 nInBufLen, UINT32& offset, PreviousTPInformation* pPrvTPInfo)
{
    INT32 nResult = TRUE;
    if (pPrvTPInfo == nullptr)return FALSE;

    /* NID_C */
    nResult = Utility::Decode_uint16(pInBuf, nInBufLen, offset, LEN_NID_C, &pPrvTPInfo->NID_C);
    /* NID_TP */
    if (nResult == TRUE)
    {
        offset += LEN_NID_C;
        nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_NID_TP, &pPrvTPInfo->NID_TP);
    }
    /* Q_Pass_Stop_Depart */
    if (nResult == TRUE)
    {
        offset += LEN_NID_TP;
        nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_PASS_STOP_DEPART, &pPrvTPInfo->Q_Pass_Stop_Depart);
    }
    if (nResult == TRUE)
    {
        offset += LEN_Q_PASS_STOP_DEPART;
    }
    return nResult;
}

BOOL Decode_Packet08_TPEstimation(const byte* pInBuf, UINT32 nInBufLen, UINT32& offset, TimingPointEstimation* pTPEst)
{
    INT32 nResult = TRUE;
    if (pTPEst == nullptr)return FALSE;

    /* NID_C */
    nResult = Utility::Decode_uint16(pInBuf, nInBufLen, offset, LEN_NID_C, &pTPEst->NID_C);
    /* NID_TP */
    if (nResult == TRUE)
    {
        offset += LEN_NID_C;
        nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_NID_TP, &pTPEst->NID_TP);
    }
    /* T_Arrival_Date */
    if (nResult == TRUE)
    {
        offset += LEN_NID_TP;
        nResult = Utility::Decode_uint16(pInBuf, nInBufLen, offset, LEN_T_ARRIVAL_DATE, &pTPEst->T_Arrival_Date);
    }
    /* T_Arrival_Seconds */
    if (nResult == TRUE)
    {
        offset += LEN_T_ARRIVAL_DATE;
        nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_T_ARRIVAL_SECONDS, &pTPEst->T_Arrival_Seconds);
    }

    if (nResult == TRUE)
    {
        offset += LEN_T_ARRIVAL_SECONDS;
    }
    return nResult;
}


BOOL Decode_Packet08_STRSendingLocation(const byte* pInBuf, UINT32 nInBufLen, UINT32& offset, STRSendingLocation* pSndLocation)
{
    BOOL nResult = TRUE;
    if (pSndLocation == nullptr)return FALSE;

    /* NID_C */
    nResult = Utility::Decode_uint16(pInBuf, nInBufLen, offset, LEN_NID_C, &pSndLocation->NID_C);
    /* NID_SP */
    if (nResult == TRUE)
    {
        offset += LEN_NID_C;
        nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_NID_SP, &pSndLocation->NID_SP);
    }
    /* D_Sending_Position */
    if (nResult == TRUE)
    {
        offset += LEN_NID_SP;
        nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_D_SENDING_POSITION, &pSndLocation->D_Sending_Position);
    }
    if (nResult == TRUE)
    {
        offset += LEN_D_SENDING_POSITION;
    }
    return nResult;
}




BOOL DecodeMessage_ATOHeader(const byte* pInBuf, UINT32 nInBufLen, UINT32& offset, ATO_Header* pHeader)
{
    BOOL nResult = TRUE;
    UINT32 nRcvTotalBitLen = (nInBufLen * sizeof(UINT8));

    /* NID_PACKET_ATO */
    nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_NID_PACKET_ATO, &pHeader->NID_PACKET_ATO);


    /* NID_L_PACKET_ATO */
    if (nResult == TRUE)
    {
        offset += LEN_NID_PACKET_ATO;
        nResult = Utility::Decode_uint16(pInBuf, nInBufLen, offset, LEN_L_PACKET_ATO, &pHeader->L_PACKET_ATO);
    }
    
    /**  NID_OPERATIONAL */
    if (nResult == TRUE)
    {
        offset += LEN_L_PACKET_ATO;
        nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_NID_OPERATIONAL, &pHeader->NID_OPERATIONAL);
    }
    /**  NID_ENGINE */
    if (nResult == TRUE)
    {
        offset += LEN_NID_OPERATIONAL;
        nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_NID_ENGINE, &pHeader->NID_ENGINE);
    }
    /**  T_Timestamp_Date */
    if (nResult == TRUE)
    {
        offset += LEN_NID_ENGINE;
        nResult = Utility::Decode_uint16(pInBuf, nInBufLen, offset, LEN_T_TIMESTAMP_DATE, &pHeader->T_Timestamp_Date);
    }
    /**  T_Timestamp_Seconds */
    if (nResult == TRUE)
    {
        offset += LEN_T_TIMESTAMP_DATE;
        nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_T_TIMESTAMP_SECONDS, &pHeader->T_Timestamp_Seconds);
    }
    /**  N_Packet_Counter */
    if (nResult == TRUE)
    {
        offset += LEN_T_TIMESTAMP_SECONDS;
        nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_N_PACKET_COUNTER, &pHeader->N_Packet_Counter);
    }
    if (nResult == TRUE)
    {
        offset += LEN_N_PACKET_COUNTER;
    }

    return nResult;
    


}




BOOL DecodeMessage_ATO_Packet00_HsReq(OB_TS_Packet00_HSReqStruct* pMsg, UINT32& offset, const UINT8* pInBuf, UINT32 nInBufLen)
{
    BOOL nResult = TRUE;
    if (pMsg == nullptr)return FALSE;
    /** N_ITER **/

    nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_N_ITER, &pMsg->N_ITER);

    /** M_ATO_Version **/

    if (nResult == TRUE)
    {
        offset += LEN_N_ITER;
        if (pMsg->N_ITER > 0)
        {
            UINT8 nIdx = 0;
            do
            {
                nResult = Utility::Decode_uint16(pInBuf, nInBufLen, offset, LEN_M_ATO_VERSION, &pMsg->M_ATO_Version[nIdx]);
                offset += LEN_M_ATO_VERSION;
                nIdx++;
            } while ((nResult == TRUE) && (nIdx < pMsg->N_ITER));
        }
    }

    return nResult;
}

BOOL DecodeMessage_ATO_Packet03_JpReq(OB_TS_Packet03_JPReqStruct* pMsg, UINT32& offset, const UINT8* pInBuf, UINT32 nInBufLen)
{
    BOOL nResult = TRUE;
    if (pMsg == nullptr)return FALSE;

    /* NID_C */
    nResult = Utility::Decode_uint16(pInBuf, nInBufLen, offset, LEN_NID_C, &pMsg->NID_C);
    /* NID_SP */
    if (nResult == TRUE)
    {
        offset += LEN_NID_C;
        nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_NID_SP, &pMsg->NID_SP);
    }
    if (nResult == TRUE)
    {
        offset += LEN_NID_SP;
    }

    return nResult;

}

BOOL Decode_Packet04_STPInformation(const UINT8* pInBuf, const UINT32 nInBufLen, UINT32& offset, STPInformation* pSTPInfo)
{
    BOOL nResult = TRUE;
    /* Q_Opening_Door_Side */
    nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_OPENING_DOOR_SIDE, &pSTPInfo->Q_Opening_Door_Side);
    /* Q_Centralised_Opening */
    if (nResult == TRUE)
    {
        offset += LEN_Q_OPENING_DOOR_SIDE;
        nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_CENTRALISED_OPENING, &pSTPInfo->Q_Centralised_Opening);
    }
    /* Q_Relaxed_Coupler */
    if (nResult == TRUE)
    {
        offset += LEN_Q_CENTRALISED_OPENING;
        nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_RELAXED_COUPLER, &pSTPInfo->Q_Relaxed_Coupler);
    }

    if (nResult == TRUE)
    {
        offset += LEN_Q_RELAXED_COUPLER;
    }
    return nResult;
}


BOOL Decode_Packet04_STPDepartureDetails(const UINT8* pInBuf, const UINT32 nInBufLen, UINT32& offset, STPDepartureDetails* pSTPDepDetails)
{
    INT32 nResult = TRUE;

    /* Q_Train_Hold */
    nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_TRAIN_HOLD, &pSTPDepDetails->Q_Train_Hold);

    /* T_Departure_Date */
    if (nResult == TRUE)
    {
        offset += LEN_Q_TRAIN_HOLD;
        nResult = Utility::Decode_uint16(pInBuf, nInBufLen, offset, LEN_T_DEPARTURE_DATE, &pSTPDepDetails->T_Departure_Date);
    }
    /* T_Departure_Seconds */
    if (nResult == TRUE)
    {
        offset += LEN_T_DEPARTURE_DATE;
        nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_T_DEPARTURE_SECONDS, &pSTPDepDetails->T_Departure_Seconds);
    }
    /* T_Minimum_Dwell_Time */
    if (nResult == TRUE)
    {
        offset += LEN_T_DEPARTURE_SECONDS;
        nResult = Utility::Decode_uint16(pInBuf, nInBufLen, offset, LEN_T_MINIMUM_DWELL_TIME, &pSTPDepDetails->T_Minimum_Dwell_Time);
    }
    /* Q_Automatic_Closing */
    if (nResult == TRUE)
    {
        offset += LEN_T_MINIMUM_DWELL_TIME;
        nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_AUTOMATIC_CLOSING, &pSTPDepDetails->Q_Automatic_Closing);
    }
    if (nResult == TRUE)
    {
        offset += LEN_Q_AUTOMATIC_CLOSING;
    }
    return nResult;
}



BOOL Decode_Packet04_TemporaryConstrains(const UINT8* pInBuf, const UINT32 nInBufLen, UINT32& offset, TemporaryConstrains* pTmpConstrains)
{
    BOOL nResult = TRUE;

    /* Q_TC_Type */
    nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_TC_TYPE, &pTmpConstrains->Q_TC_Type);
#ifdef CheckValueSpare
    if (nResult == OK)
    {
        nResult = Check_Q_TC_Type(pTmpConstrains->Q_TC_Type);
    }
#endif
    /* Q_Range */
    if (nResult == TRUE)
    {
        offset += LEN_Q_TC_TYPE;
        nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_RANGE, &pTmpConstrains->Q_Range);
    }
    /* D_TC_Start_Location */
    if (nResult == TRUE)
    {
        offset += LEN_Q_RANGE;
        nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_D_TC_START_LOCATION, &pTmpConstrains->D_TC_Start_Location);
    }
    /* D_TC_End_Location */
    if (nResult == TRUE)
    {
        offset += LEN_D_TC_START_LOCATION;
        nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_D_TC_END_LOCATION, &pTmpConstrains->D_TC_End_Location);
    }

    if (nResult == TRUE)
    {
        offset += LEN_D_TC_END_LOCATION;
        if (pTmpConstrains->Q_TC_Type == Q_TC_Type_ASR)
        {
            /** ASR - Q_FRONT */
            nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_FRONT, &pTmpConstrains->asr.Q_FRONT);
            /** ASR - V_Speed_Level */
            if (nResult == TRUE)
            {
                offset += LEN_Q_FRONT;
                nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_V_SPEED_LEVEL, &pTmpConstrains->asr.V_Speed_Level);
#ifdef CheckValueSpare
                if (nResult == OK)
                {
                    nResult = Check_V_Speed_Level(pTmpConstrains->asr.V_Speed_Level);
                }
#endif
            }
            if (nResult == TRUE)
            {
                offset += LEN_V_SPEED_LEVEL;
            }
        }
        else if (pTmpConstrains->Q_TC_Type == Q_TC_Type_Low_Adhesion)
        {
            /** Low Adhesion - Q_Adhesion_Category */
            nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_ADHESION_CATEGORY, &pTmpConstrains->lowAdhesion.Q_Adhesion_Category);
            if (nResult == TRUE)
            {
                offset += LEN_Q_ADHESION_CATEGORY;
#ifdef CheckValueSpare
                nResult = Check_Q_Adhesion_Category(pTmpConstrains->lowAdhesion.Q_Adhesion_Category);
#endif
            }
        }
        else if (pTmpConstrains->Q_TC_Type == Q_TC_Type_Current_Limitation)
        {
            nResult = Utility::Decode_uint16(pInBuf, nInBufLen, offset, LEN_M_CURRENT, &pTmpConstrains->currentLimitation.M_CURRENT);
            if (nResult == TRUE)
            {
                offset += LEN_M_CURRENT;
            }
        }
        else
        {
            nResult = FALSE;
        }
    }

    return nResult;
}


BOOL Decode_Packet04_TimingPointConstraints(const UINT8* pInBuf, const UINT32 nInBufLen, UINT32& offset, TimingPointConstraints* pTPConstraints)
{
    BOOL nResult = TRUE;

    /* NID_TP */
    nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_NID_TP, &pTPConstraints->NID_TP);

    /* L_Latest_Arrival_Date */
    if (nResult == TRUE)
    {
        offset += LEN_NID_TP;
        nResult = Utility::Decode_uint16(pInBuf, nInBufLen, offset, LEN_T_LATEST_ARRIVAL_DATE, &pTPConstraints->T_Latest_Arrival_Date);
    }
    /* T_Latest_Arrival_Seconds */
    if (nResult == TRUE)
    {
        offset += LEN_T_LATEST_ARRIVAL_DATE;
        nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_T_LATEST_ARRIVAL_SECONDS, &pTPConstraints->T_Latest_Arrival_Seconds);
    }
    /* T_Arrival_Window */
    if (nResult == TRUE)
    {
        offset += LEN_T_LATEST_ARRIVAL_SECONDS;
        nResult = Utility::Decode_uint16(pInBuf, nInBufLen, offset, LEN_T_ARRIVAL_WINDOW, &pTPConstraints->T_Arrival_Window);
    }
    /* Q_TP_Alignment */
    if (nResult == TRUE)
    {
        offset += LEN_T_ARRIVAL_WINDOW;
        nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_TP_ALIGNMENT, &pTPConstraints->Q_TP_Alignment);
#ifdef CheckValueSpare
        if (nResult == OK)
        {
            nResult = Check_Q_TP_Alignment(pTPConstraints->Q_TP_Alignment);
        }
#endif
    }
    /* Q_Stop_Skip_Pass */
    if (nResult == TRUE)
    {
        offset += LEN_Q_TP_ALIGNMENT;
        nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_STOP_SKIP_PASS, &pTPConstraints->Q_Stop_Skip_Pass);
#ifdef CheckValueSpare
        if (nResult == OK)
        {
            nResult = Check_Q_Stop_Skip_Pass(pTPConstraints->Q_Stop_Skip_Pass);
        }
#endif
    }
    /* Q_TP_information */
    if (nResult == TRUE)
    {
        offset += LEN_Q_STOP_SKIP_PASS;
        nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_TP_INFORMATION, &pTPConstraints->Q_TP_information);
#ifdef CheckValueSpare
        if (nResult == OK)
        {
            nResult = Check_Q_TP_information(pTPConstraints->Q_TP_information);
        }
#endif
    }
    /* Q_Day_Light_Saving */
    if (nResult == TRUE)
    {
        offset += LEN_Q_TP_INFORMATION;
        nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_DAY_LIGHT_SAVING, &pTPConstraints->Q_Day_Light_Saving);
    }

    if (nResult == TRUE)
    {
        offset += LEN_Q_DAY_LIGHT_SAVING;
        if (pTPConstraints->Q_Stop_Skip_Pass == Q_Stop_SKIP_Pass_STOPPING_POINT)
        {
            nResult = Decode_Packet04_STPInformation(pInBuf, nInBufLen, offset, &pTPConstraints->stpInformation);
            if ((nResult == TRUE)
                && (pTPConstraints->Q_TP_information != Q_TP_information_End_of_Journey))
            {
                nResult = Decode_Packet04_STPDepartureDetails(pInBuf, nInBufLen, offset, &pTPConstraints->stpDepartureDetails);
            }
        }
    }

    return nResult;
}


BOOL Decode_Packet04_SegmentProfileReference(const UINT8* pInBuf, const UINT32 nInBufLen, UINT32& offset, SegmentProfileReference* pSPRef)
{
    BOOL nResult = TRUE;
    UINT8 nIdx = 0;

    /* NID_C */
    nResult = Utility::Decode_uint16(pInBuf, nInBufLen, offset, LEN_NID_C, &pSPRef->NID_C);

    /* NID_SP */
    if (nResult == TRUE)
    {
        offset += LEN_NID_C;
        nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_NID_SP, &pSPRef->NID_SP);
    }
    /* M_SP_Version */
    if (nResult == TRUE)
    {
        offset += LEN_NID_SP;
        nResult = Utility::Decode_uint16(pInBuf, nInBufLen, offset, LEN_M_SP_VERSION, &pSPRef->M_SP_Version);

    }
    /* Q_SP_DIR */
    if (nResult == TRUE)
    {
        offset += LEN_M_SP_VERSION;
        nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_SP_DIR, &pSPRef->Q_SP_DIR);
    }
    /* N_ITER */
    if (nResult == TRUE)
    {
        offset += LEN_Q_SP_DIR;
        nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_N_ITER, &pSPRef->N_ITER);
    }
    /* TimingPointConstraints */
    if (nResult == TRUE)
    {
        offset += LEN_N_ITER;
        if (pSPRef->N_ITER > 0)
        {
            nIdx = (UINT8)0;
            do
            {
                nResult = Decode_Packet04_TimingPointConstraints(pInBuf, nInBufLen, offset, &pSPRef->TPConstraints[nIdx]);
                nIdx++;
            } while ((nResult == TRUE) && (nIdx < pSPRef->N_ITER));
        }
    }
    /** N_ITER2 */
    if (nResult == TRUE)
    {
        nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_N_ITER, &pSPRef->N_ITER_tempConstraints);
    }
    /** Temporary Constrains */
    if (nResult == TRUE)
    {
        offset += LEN_N_ITER;
        if (pSPRef->N_ITER_tempConstraints > 0)
        {
            nIdx = (UINT8)0;
            do
            {
                nResult = Decode_Packet04_TemporaryConstrains(pInBuf, nInBufLen, offset, &pSPRef->temporaryConstrains[nIdx]);
                nIdx++;
            } while ((nResult == TRUE) && (nIdx < pSPRef->N_ITER_tempConstraints));
        }
    }
    return nResult;
}


BOOL DecodeMessage_ATO_Packet04_JP(TS_OB_Packet04_JPStruct* pMsg, UINT32& offset, const UINT8* pInBuf, UINT32 nInBufLen)
{
    BOOL nResult = TRUE;
    UINT8 nIdx = 0;

    /**  Q_JP_Status */
    /** Journey Profile Details */
    nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_JP_STATUS, &pMsg->Q_JP_Status);
#ifdef CheckValueSpare
    if (nResult == OK)
    {
        nResult = Check_Q_JP_Status(pMsg->Q_JP_Status);
    }
#endif

    if ((pMsg->Q_JP_Status == Q_JP_Status_Valid)
        || (pMsg->Q_JP_Status == Q_JP_Status_Update)
        || (pMsg->Q_JP_Status == Q_JP_Status_Overwrite))
    {
        if (nResult == TRUE)
        {
            offset += LEN_Q_JP_STATUS;
            nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_N_ITER_SP, &pMsg->N_ITER_SP);
        }
        if ((nResult == TRUE) && (pMsg->N_ITER_SP > 0))
        {
            offset += LEN_N_ITER_SP;
            nIdx = (UINT8)0;
            do
            {
                /** Segment Profile Reference */
                nResult = Decode_Packet04_SegmentProfileReference(pInBuf, nInBufLen, offset, &pMsg->segmentProfileReference[nIdx]);
                nIdx++;
            } while ((nResult == TRUE) && (nIdx < pMsg->N_ITER_SP));
        }
    }

    return nResult;

}




BOOL DecodeMessage_ATO_Packet05_JpAck(OB_TS_Packet05_JPAckStruct* pMsg, UINT32& offset, const UINT8* pInBuf, UINT32 nInBufLen)
{
    BOOL nResult = TRUE;
    if (pMsg == nullptr)return FALSE;

    /* T_JP_Ref_TimeStamp_Date */
    nResult = Utility::Decode_uint16(pInBuf, nInBufLen, offset, LEN_T_JP_REFERENCE_TIMESTAMP_DATE, &pMsg->T_JP_Ref_TimeStamp_Date);
    /* T_JP_Ref_TimeStamp_Seconds */
    if (nResult == TRUE)
    {
        offset += LEN_T_JP_REFERENCE_TIMESTAMP_DATE;
        nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_T_JP_REFERENCE_TIMESTAMP_SECONDS, &pMsg->T_JP_Ref_TimeStamp_Seconds);
    }
    /* N_JP_Ref_Packet_Counter */
    if (nResult == TRUE)
    {
        offset += LEN_T_JP_REFERENCE_TIMESTAMP_SECONDS;
        nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_N_JP_REFERENCE_PACKET_COUNTER, &pMsg->N_JP_Ref_Packet_Counter);
    }
    if (nResult == TRUE)
    {
        offset += LEN_N_JP_REFERENCE_PACKET_COUNTER;
    }

    return nResult;

}

BOOL DecodeMessage_ATO_Packet06_SPReqDetails(SegmentProfileReqDetails* pMsg, UINT32& offset, const UINT8* pInBuf, UINT32 nInBufLen)
{
    BOOL nResult = TRUE;
    if (pMsg == nullptr)return FALSE;

    /* NID_C */
    nResult = Utility::Decode_uint16(pInBuf, nInBufLen, offset, LEN_NID_C, &pMsg->NID_C);
    /* NID_SP */
    if (nResult == TRUE)
    {
        offset += LEN_NID_C;
        nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_NID_SP, &pMsg->NID_SP);
    }
    if (nResult == TRUE)
    {
        offset += LEN_NID_SP;
    }
    return nResult;

}

BOOL DecodeMessage_ATO_Packet06_SpReq(OB_TS_Packet06_SPReqStruct* pMsg, UINT32& offset, const UINT8* pInBuf, UINT32 nInBufLen)
{
    BOOL nResult = TRUE;

    /* N_ITER_ATO */
    nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_N_ITER_ATO, &pMsg->N_ITER_ATO);
    /* SPReqDetails */
    if (nResult == TRUE)
    {
        offset += LEN_N_ITER_ATO;
        if (pMsg->N_ITER_ATO > 0)
        {
            UINT8 nIdx = 0;
            do
            {
                nResult = DecodeMessage_ATO_Packet06_SPReqDetails(&pMsg->SPReqDetails[nIdx], offset, pInBuf, nInBufLen);
                nIdx++;
            } while ((nResult == TRUE) && (nIdx < pMsg->N_ITER_ATO));
        }
    }

    return nResult;
}

BOOL Decode_Packet07_SegmentStatusValid(const UINT8* pInBuf, const UINT32 nInBufLen, UINT32& offset, SPStatusValid* pSpValid)
{
    BOOL nResult = TRUE;

    /* M_SP_Version */
    nResult = Utility::Decode_uint16(pInBuf, nInBufLen, offset, LEN_M_SP_VERSION, &pSpValid->M_SP_Version);
    /* L_SP */
    if (nResult == TRUE)
    {
        offset += LEN_M_SP_VERSION;
        nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_L_SP, &pSpValid->L_SP);
    }
    /* D_EOA_Offset */
    if (nResult == TRUE)
    {
        offset += LEN_L_SP;
        nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_D_EOA_OFFSET, &pSpValid->D_EOA_Offset);
    }
    /* Q_UTC_Offset */
    if (nResult == TRUE)
    {
        offset += LEN_D_EOA_OFFSET;
        nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_UTC_OFFSET, &pSpValid->Q_UTC_Offset);
    }
    /* M_SP_Altitude */
    if (nResult == TRUE)
    {
        offset += LEN_Q_UTC_OFFSET;
        nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_M_SP_ATTITUDE, &pSpValid->M_SP_Altitude);
    }
    /* Q_ATOTS_Contact_Info_Dir */
    if (nResult == TRUE)
    {
        offset += LEN_M_SP_ATTITUDE;
        nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_ATOTS_CONTACT_INFO_DIR, &pSpValid->Q_ATOTS_Contact_Info_Dir);
    }
#ifdef CheckValueSpare
    if (nResult == OK)
    {
        nResult = Check_Q_ATOTS_Contact_Info_Dir(pSpValid->Q_ATOTS_Contact_Info_Dir);
    }
#endif
    if (nResult == TRUE)
    {
        offset += LEN_Q_ATOTS_CONTACT_INFO_DIR;
    }
    return nResult;
}

/**
 *
 * @param pInput
 * @param nInBufLen
 * @param offset
 * @param pATOTSContactInfo
 * @return
 */
BOOL Decode_Packet07_ATOTSContactInfo(const UINT8* pInBuf, const UINT32 nInBufLen, UINT32& offset, ATOTSContactInfo* pATOTSContactInfo)
{
    BOOL nResult = TRUE;
    /* NID_C */
    nResult = Utility::Decode_uint16(pInBuf, nInBufLen, offset, LEN_NID_C, &pATOTSContactInfo->NID_C_1);
    /* NID_ATOTS */
    if (nResult == TRUE)
    {
        offset += LEN_NID_C;
        nResult = Utility::Decode_uint16(pInBuf, nInBufLen, offset, LEN_NID_ATOTS, &pATOTSContactInfo->NID_ATOTS);
    }
    /* NID_C */
    if (nResult == TRUE)
    {
        offset += LEN_NID_ATOTS;
        nResult = Utility::Decode_uint16(pInBuf, nInBufLen, offset, LEN_NID_C, &pATOTSContactInfo->NID_C_2);
    }
    /* NID_SP */
    if (nResult == TRUE)
    {
        offset += LEN_NID_C;
        nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_NID_SP, &pATOTSContactInfo->NID_SP_2);
    }

    if (nResult == TRUE)
    {
        offset += LEN_NID_SP;
    }
    return nResult;
}

/**
 * Specific Static speed Profile
 * @param pInput
 * @param nInBufLen
 * @param offset
 * @param pSSP
 * @return
 */
BOOL Decode_Packet07_SpecificSSP(const UINT8* pInBuf, const UINT32 nInBufLen, UINT32& offset, SpecificSSP* pSSP)
{
    BOOL nResult = TRUE;

    /* Q_DIFF */
    nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_DIFF, &pSSP->Q_DIFF);
    if (nResult == TRUE)
    {
        offset += LEN_Q_DIFF;
        if (pSSP->Q_DIFF == Q_DIFF_Cant_Deficiency_specific_category)
        {
            /* NC_CDDIFF */
            nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_NC_CDDIFF, &pSSP->NC_CDDIFF);
            if (nResult == TRUE)
            {
                offset += LEN_NC_CDDIFF;
            }
        }
        else if ((pSSP->Q_DIFF == Q_DIFF_Other_specific_category_replaces_the_Cant_Deficiency_SSP)
            || (pSSP->Q_DIFF == Q_DIFF_Other_specific_category_does_not_replace_the_Cant_Deficiency_SSP))
        {
            /* NC_DIFF */
            nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_NC_DIFF, &pSSP->NC_DIFF);
            /* V_DIFF */
            if (nResult == TRUE)
            {
                offset += LEN_NC_DIFF;
                nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_V_DIFF, &pSSP->V_DIFF);
            }
            if (nResult == TRUE)
            {
                offset += LEN_V_DIFF;
            }
        }
        else
        {
            nResult = TRUE;
        }
    }
    return nResult;
}

/**
 *
 * @param pInput
 * @param nInBufLen
 * @param offset
 * @param pSSSPChange
 * @return
 */
BOOL Decode_Packet07_SpecificSSPChange(const UINT8* pInBuf, const UINT32 nInBufLen, UINT32& offset, SpecificSspChange* pSSSPChange)
{
    BOOL nResult = TRUE;

    /* Q_DIFF */
    nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_DIFF, &pSSSPChange->Q_DIFF);
    if (nResult == TRUE)
    {
        offset += LEN_Q_DIFF;
        if (pSSSPChange->Q_DIFF == Q_DIFF_Cant_Deficiency_specific_category)
        {
            /* NC_CDDIFF */
            nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_NC_CDDIFF, &pSSSPChange->NC_CDDIFF);
            if (nResult == TRUE)
            {
                offset += LEN_NC_CDDIFF;
            }
        }
        else if ((pSSSPChange->Q_DIFF == Q_DIFF_Other_specific_category_replaces_the_Cant_Deficiency_SSP)
            || (pSSSPChange->Q_DIFF == Q_DIFF_Other_specific_category_does_not_replace_the_Cant_Deficiency_SSP))
        {
            /* NC_DIFF */
            if (nResult == TRUE)
            {
                nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_NC_DIFF, &pSSSPChange->NC_DIFF);
            }
            /* V_DIFF */
            if (nResult == TRUE)
            {
                offset += LEN_NC_DIFF;
                nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_V_DIFF, &pSSSPChange->V_DIFF);
            }
            if (nResult == TRUE)
            {
                offset += LEN_V_DIFF;
            }
        }
        else
        {
            nResult = ERROR;
        }
    }
    return nResult;
}

/**
 * Static Speed Profile
 * @param pInput
 * @param nInBufLen
 * @param offset
 * @param pStaticSP
 * @return
 */
LOCAL INT32 Decode_Packet07_StaticSPChange(const UINT8* pInBuf, const UINT32 nInBufLen, UINT32& offset, StaticSPChange* pStaticSP)
{
    INT32 nResult = TRUE;
    UINT8 nIdx = 0;

    /* D_Location */
    nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_D_LOCATION, &pStaticSP->D_Location);
    /* V_STATIC */
    if (nResult == TRUE)
    {
        offset += LEN_D_LOCATION;
        nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_V_STATIC, &pStaticSP->V_STATIC);
#ifdef CheckValueSpare
        if (nResult == OK)
        {
            nResult = Check_V_STATIC(pStaticSP->V_STATIC);
        }
#endif
    }
    /* Q_FRONT */
    if (nResult == TRUE)
    {
        offset += LEN_V_STATIC;
        nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_FRONT, &pStaticSP->Q_FRONT);
    }
    /* N_ITER */
    if (nResult == TRUE)
    {
        offset += LEN_Q_FRONT;
        nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_N_ITER, &pStaticSP->N_ITER);
    }

    if (nResult == TRUE)
    {
        offset += LEN_N_ITER;
        if (pStaticSP->N_ITER > 0)
        {
            nIdx = (UINT8)0;
            do
            {
                nResult = Decode_Packet07_SpecificSSPChange(pInBuf, nInBufLen, offset, &pStaticSP->specificSspChange[nIdx]);
                nIdx++;
            } while ((nResult == TRUE) && (nIdx < pStaticSP->N_ITER));
        }
    }
    return nResult;
}

/**
 * Gradient Info
 * @param pInput
 * @param nInBufLen
 * @param offset
 * @param pGradient
 * @return
 */
BOOL Decode_Packet07_GradientChange(const UINT8* pInBuf, const UINT32 nInBufLen, UINT32& offset, GradientChange* pGradient)
{
    BOOL nResult = TRUE;

    /* D_Location */
    nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_D_LOCATION, &pGradient->D_Location);
    /* G_New_Gradient */
    if (nResult == TRUE)
    {
        offset += LEN_D_LOCATION;
        nResult = Utility::Decode_uint16(pInBuf, nInBufLen, offset, LEN_G_NEW_GRADIENT, &pGradient->G_New_Gradient);
    }
    /* Q_GDIR */
    if (nResult == TRUE)
    {
        offset += LEN_G_NEW_GRADIENT;
        nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_GDIR, &pGradient->Q_GDIR);
    }
    if (nResult == TRUE)
    {
        offset += LEN_Q_GDIR;
    }
    return nResult;
}

/**
 * Curve
 * @param pInput
 * @param nInBufLen
 * @param offset
 * @param pCurve
 * @return
 */
BOOL Decode_Packet07_CurveChange(const UINT8* pInBuf, const UINT32 nInBufLen, UINT32& offset, CurvesChange* pCurve)
{
    BOOL nResult = TRUE;

    /* D_Location */
    nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_D_LOCATION, &pCurve->D_Location);
    /* Q_Radius_Category */
    if (nResult == TRUE)
    {
        offset += LEN_D_LOCATION;
        nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_RADIUS_CATEGORY, &pCurve->Q_Radius_Category);
#ifdef CheckValueSpare
        if (nResult == OK)
        {
            nResult = Check_Q_Radius_Category(pCurve->Q_Radius_Category);
        }
#endif
    }
    if (nResult == TRUE)
    {
        offset += LEN_Q_RADIUS_CATEGORY;
    }
    return nResult;
}

#ifndef KTCS_3

/**
 * Power Voltage Change
 * @param pInput
 * @param nInBufLen
 * @param offset
 * @param pPWVoltage
 * @return
 */
BOOL Decode_Packet07_PWVoltageChange(const UINT8* pInBuf, const UINT32 nInBufLen, UINT32& offset, PowerVoltageChange* pPWVoltage)
{
    BOOL nResult = TRUE;

    /* D_Location */
    nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_D_LOCATION, &pPWVoltage->D_Location);
    /* M_VOLTAGE */
    if (nResult == TRUE)
    {
        offset += LEN_D_LOCATION;
        nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_M_VOLTAGE, &pPWVoltage->M_VOLTAGE);
    }
    /* NIC_CTRACTION */
    if (nResult == TRUE)
    {
        offset += LEN_M_VOLTAGE;
        if (pPWVoltage->M_VOLTAGE != M_VOLTAGE_Line_not_fitted_with_any_traction_system)
        {
            nResult = Utility::Decode_uint16(pInBuf, nInBufLen, offset, LEN_NID_CTRACTION, &pPWVoltage->NID_CTRACTION);
            if (nResult == TRUE)
            {
                offset += LEN_NID_CTRACTION;
            }
        }
    }

    return nResult;
}
#endif

/**
 * Current Limitation Change
 *
 * @param pInput
 * @param nInBufLen
 * @param offset
 * @param pCurrentChange
 * @return
 */
BOOL Decode_Packet07_CurrentLimitChange(const UINT8* pInBuf, const UINT32 nInBufLen, UINT32& offset, CurrentLimitationChange* pCurrentChange)
{
    BOOL nResult = TRUE;

    /* D_Location */
    nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_D_LOCATION, &pCurrentChange->D_Location);
    /* LEN_M_CURRENT */
    if (nResult == TRUE)
    {
        offset += LEN_D_LOCATION;
        nResult = Utility::Decode_uint16(pInBuf, nInBufLen, offset, LEN_M_CURRENT, &pCurrentChange->M_CURRENT);
    }
    /* NIC_CTRACTION */
    if (nResult == TRUE)
    {
        offset += LEN_M_CURRENT;

    }
    return nResult;
}

/**
 * Balise in BaliseGroup
 *
 * @param pInput
 * @param nInBufLen
 * @param offset
 * @param pBalise
 * @return
 */
BOOL Decode_Packet07_Balise(const UINT8* pInBuf, const UINT32 nInBufLen, UINT32& offset, Balise* pBalise)
{
    BOOL nResult = TRUE;
    /* N_PIG */
    nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_N_PIG, &pBalise->N_PIG);
    /* LEN_D_LOCATION */
    if (nResult == TRUE)
    {
        offset += LEN_N_PIG;
        nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_D_LOCATION, &pBalise->D_Location);
    }

    if (nResult == TRUE)
    {
        offset += LEN_D_LOCATION;
    }
    return nResult;
}

/**
 * Balise Group
 * @param pInput
 * @param nInBufLen
 * @param offset
 * @param pBaliseGroup
 * @return
 */
BOOL Decode_Packet07_BaliseGroup(const UINT8* pInBuf, const UINT32 nInBufLen, UINT32& offset, BaliseGroup* pBaliseGroup)
{
    BOOL nResult = TRUE;
    UINT8 nIdx = 0;

    /* Q_NEWNID_C */
    nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_NEWNID_C, &pBaliseGroup->Q_NEWNID_C);

    /* NID_C */
    if (nResult == TRUE)
    {
        offset += LEN_Q_NEWNID_C;
        if (pBaliseGroup->Q_NEWNID_C == Q_NEWNID_C_NID_C_to_be_used_follows)
        {
            nResult = Utility::Decode_uint16(pInBuf, nInBufLen, offset, LEN_NID_C, &pBaliseGroup->NID_C);
            if (nResult == TRUE)
            {
                offset += LEN_NID_C;
            }
        }
        else
        {
            pBaliseGroup->NID_C = NID_C_DEFAULT;
        }
    }
    /* NID_BG */
    if (nResult == TRUE)
    {
        nResult = Utility::Decode_uint16(pInBuf, nInBufLen, offset, LEN_NID_BG, &pBaliseGroup->NID_BG);
    }
    /* N_ITER_BG */
    if (nResult == TRUE)
    {
        offset += LEN_NID_BG;
        nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_N_ITER_BG, &pBaliseGroup->N_ITER_BG);
    }
    if (nResult == TRUE)
    {
        offset += LEN_N_ITER_BG;
        if (pBaliseGroup->N_ITER_BG > 0)
        {
            nIdx = (UINT8)0;
            do
            {
                nResult = Decode_Packet07_Balise(pInBuf, nInBufLen, offset, &pBaliseGroup->balises[nIdx]);
                nIdx++;
            } while ((nResult == TRUE) && (nIdx < pBaliseGroup->N_ITER_BG));
        }
    }
    return nResult;
}

/**
 * Timing Points
 * @param pInput
 * @param nInBufLen
 * @param offset
 * @param pTP
 * @return
 */
BOOL Decode_Packet07_TimingPoints(const UINT8* pInBuf, const UINT32 nInBufLen, UINT32& offset, TimingPoints* pTP)
{
    BOOL nResult = TRUE;
    UINT8 nIdx = 0;

    /* NID_TP */
    nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_NID_TP, &pTP->NID_TP);
    /* D_Location */
    if (nResult == TRUE)
    {
        offset += LEN_NID_TP;
        nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_D_LOCATION, &pTP->D_Location);
    }
    /* Q_Stop_Location_Tolerance */
    if (nResult == TRUE)
    {
        offset += LEN_D_LOCATION;
        nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_STOP_LOCATION_TOLERANCE, &pTP->Q_Stop_Location_Tolerance);
#ifdef CheckValueSpare
        if (nResult == OK)
        {
            nResult = Check_Q_Stop_Location_Tolerance(pTP->Q_Stop_Location_Tolerance);
        }
#endif
    }
    /* Q_STP_Reached */
    if (nResult == TRUE)
    {
        offset += LEN_Q_STOP_LOCATION_TOLERANCE;
        nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_STP_REACHED, &pTP->Q_STP_Reached);
#ifdef CheckValueSpare
        if (nResult == OK)
        {
            nResult = Check_Q_STP_Reached(pTP->Q_STP_Reached);
        }
#endif
    }
    /* L_TEXT */
    if (nResult == TRUE)
    {
        offset += LEN_Q_STP_REACHED;
        nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_L_TEXT, &pTP->L_TEXT);
    }
    if (nResult == TRUE)
    {
        offset += LEN_L_TEXT;
        if (pTP->L_TEXT > 0)
        {
            nIdx = (UINT8)0;
            do
            {
                nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_X_TEXT, &pTP->X_TEXT[nIdx]);
                offset += LEN_X_TEXT;
                nIdx++;
            } while ((nResult == TRUE) && (nIdx < pTP->L_TEXT));
        }
    }
    return nResult;
}

/**
 * Platform Area
 *
 * @param pInput
 * @param nInBufLen
 * @param offset
 * @param pPlatform
 * @return
 */
BOOL Decode_Packet07_PlatformArea(const UINT8* pInBuf, const UINT32 nInBufLen, UINT32& offset, PlatformArea* pPlatform)
{
    BOOL nResult = TRUE;

    /* Q_Range*/
    nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_RANGE, &pPlatform->Q_Range);
    /* D_Start_Location */
    if (nResult == TRUE)
    {
        offset += LEN_Q_RANGE;
        if ((pPlatform->Q_Range == Q_Range_Starts) || (pPlatform->Q_Range == Q_Range_StartsEnds))
        {
            nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_D_START_LOCATION, &pPlatform->D_Start_Location);
            if (nResult == TRUE)
            {
                offset += LEN_D_START_LOCATION;
            }
        }
    }
    /* D_End_Location */
    if (nResult == TRUE)
    {
        if ((pPlatform->Q_Range == Q_Range_Ends) || (pPlatform->Q_Range == Q_Range_StartsEnds))
        {
            nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_D_END_LOCATION, &pPlatform->D_End_Location);
            if (nResult == TRUE)
            {
                offset += LEN_D_END_LOCATION;
            }
        }
    }

    return nResult;
}

/**
 * Tunnel
 * @param pInput
 * @param nInBufLen
 * @param offset
 * @param pTunnel
 * @return
 */
BOOL Decode_Packet07_Tunnel(const UINT8* pInBuf, const UINT32 nInBufLen, UINT32& offset, Tunnel* pTunnel)
{
    BOOL nResult = TRUE;

    /* Q_Range*/
    nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_RANGE, &pTunnel->Q_Range);
    /* Q_Tunnel_Category */
    if (nResult == TRUE)
    {
        offset += LEN_Q_RANGE;
        nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_TUNNEL_CATEGORY, &pTunnel->Q_Tunnel_Category);
#ifdef CheckValueSpare
        if (nResult == OK)
        {
            nResult = Check_Q_Tunnel_Category(pTunnel->Q_Tunnel_Category);
        }
#endif
    }
    /* D_Start_Location */
    if (nResult == TRUE)
    {
        offset += LEN_Q_TUNNEL_CATEGORY;
        if ((pTunnel->Q_Range == Q_Range_Starts) || (pTunnel->Q_Range == Q_Range_StartsEnds))
        {
            nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_D_START_LOCATION, &pTunnel->D_Start_Location);
            if (nResult == TRUE)
            {
                offset += LEN_D_START_LOCATION;
            }
        }
    }
    /* D_End_Location */
    if (nResult == TRUE)
    {
        if ((pTunnel->Q_Range == Q_Range_Ends) || (pTunnel->Q_Range == Q_Range_StartsEnds))
        {
            nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_D_END_LOCATION, &pTunnel->D_End_Location);
            if (nResult == TRUE)
            {
                offset += LEN_D_END_LOCATION;
            }
        }
    }

    return nResult;
}

/**
 * AxleLoad Speed Profile
 * @param pInput
 * @param nInBufLen
 * @param offset
 * @param pAxleLoadSP
 * @return
 */
BOOL Decode_Packet07_AxleLoadSP(const UINT8* pInBuf, const UINT32 nInBufLen, UINT32& offset, AxleLoadSpeedProfile* pAxleLoadSP)
{
    BOOL nResult = TRUE;

    /* Q_Range*/
    nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_RANGE, &pAxleLoadSP->Q_Range);
    /* M_AXLELOADCAT */
    if (nResult == TRUE)
    {
        offset += LEN_Q_RANGE;
        nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_M_AXLELOADCAT, &pAxleLoadSP->M_AXLELOADCAT);
    }
    /* V_New_Speed_Level */
    if (nResult == TRUE)
    {
        offset += LEN_M_AXLELOADCAT;
        nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_V_SPEED_LEVEL, &pAxleLoadSP->V_New_Speed_Level);
#ifdef CheckValueSpare
        if (nResult == OK)
        {
            nResult = Check_V_New_Speed_Level(pAxleLoadSP->V_New_Speed_Level);
        }
#endif
    }
    /* Q_FRONT */
    if (nResult == TRUE)
    {
        offset += LEN_V_SPEED_LEVEL;
        nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_FRONT, &pAxleLoadSP->Q_FRONT);
    }
    /* D_Start_Location */
    if (nResult == TRUE)
    {
        offset += LEN_Q_FRONT;
        if ((pAxleLoadSP->Q_Range == Q_Range_Starts) || (pAxleLoadSP->Q_Range == Q_Range_StartsEnds))
        {
            nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_D_START_LOCATION, &pAxleLoadSP->D_Start_Location);
            if (nResult == TRUE)
            {
                offset += LEN_D_START_LOCATION;
            }
        }
    }
    /* D_End_Location */
    if (nResult == TRUE)
    {
        if ((pAxleLoadSP->Q_Range == Q_Range_Ends) || (pAxleLoadSP->Q_Range == Q_Range_StartsEnds))
        {
            nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_D_END_LOCATION, &pAxleLoadSP->D_End_Location);
            if (nResult == TRUE)
            {
                offset += LEN_D_END_LOCATION;
            }
        }
    }
    return nResult;
}


/**
 * Unprotected Level Crossing Stop
 * @param pInput
 * @param nInBufLen
 * @param offset
 * @param pUnprotectedCRST
 * @return
 */
BOOL Decode_Packet07_UnprotectedLevelCrossingStop(const UINT8* pInBuf, const UINT32 nInBufLen, UINT32& offset, UnprotectedLevelCrossingStop* pUnprotectedCRST)
{
    BOOL nResult = TRUE;
    /* D_UnprotectedLx_Stop_Nominal */
    nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_D_UNPROTECTEDLX_STOP_NOMINAL, &pUnprotectedCRST->D_UnprotectedLx_Stop_Nominal);

    /* D_UnprotectedLx_Stop_Reverse */
    if (nResult == TRUE)
    {
        offset += LEN_D_UNPROTECTEDLX_STOP_NOMINAL;
        nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_D_UNPROTECTEDLX_STOP_REVERSE, &pUnprotectedCRST->D_UnprotectedLx_Stop_Reverse);
    }
    if (nResult == TRUE)
    {
        offset += LEN_D_UNPROTECTEDLX_STOP_REVERSE;
    }

    return nResult;
}

/**
 * Permitted Braking Distance
 * @param pInput
 * @param nInBufLen
 * @param offset
 * @param pPBDistance
 * @return
 */
BOOL Decode_Packet07_PermittedBrakingDistance(const UINT8* pInBuf, const UINT32 nInBufLen, UINT32& offset, PermittedBrakingDistance* pPBDistance)
{
    BOOL nResult = TRUE;

    /* Q_Range*/
    nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_RANGE, &pPBDistance->Q_Range);

    /* D_Permitted_Braking_Distance */
    if (nResult == TRUE)
    {
        offset += LEN_Q_RANGE;
        nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_D_PERMITTED_BRAKING_DISTANCE, &pPBDistance->D_Permitted_Braking_Distance);
    }
    /* Q_PBD_SBEB */
    if (nResult == TRUE)
    {
        offset += LEN_D_PERMITTED_BRAKING_DISTANCE;
        nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_PBD_SBEB, &pPBDistance->Q_PBD_SBEB);
    }
    /* G_PBD */
    if (nResult == TRUE)
    {
        offset += LEN_Q_PBD_SBEB;
        nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_G_PBD, &pPBDistance->G_PBD);
    }
    /* Q_GDIR_PBD */
    if (nResult == TRUE)
    {
        offset += LEN_G_PBD;
        nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_GDIR_PBD, &pPBDistance->Q_GDIR_PBD);
    }
    /* D_Start_Location */
    if (nResult == TRUE)
    {
        offset += LEN_Q_GDIR_PBD;
        if ((pPBDistance->Q_Range == Q_Range_Starts) || (pPBDistance->Q_Range == Q_Range_StartsEnds))
        {
            nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_D_START_LOCATION, &pPBDistance->D_Start_Location);
            if (nResult == TRUE)
            {
                offset += LEN_D_START_LOCATION;
            }
        }
    }
    /* D_End_Location */
    if (nResult == TRUE)
    {
        if ((pPBDistance->Q_Range == Q_Range_Ends) || (pPBDistance->Q_Range == Q_Range_StartsEnds))
        {
            nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_D_END_LOCATION, &pPBDistance->D_End_Location);
            if (nResult == TRUE)
            {
                offset += LEN_D_END_LOCATION;
            }
        }
    }
    return nResult;
}

/**
 * Switch Off Regenerative Brake
 *
 * @param pInput
 * @param nInBufLen
 * @param offset
 * @param pSWoffRBrake
 * @return
 */
BOOL Decode_Packet07_SwitchOffRegenerativeBrake(const UINT8* pInBuf, const UINT32 nInBufLen, UINT32& offset, SwitchOffRegenerativeBrake* pSWoffRBrake)
{
    BOOL nResult = TRUE;

    /* Q_Range*/
    nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_RANGE, &pSWoffRBrake->Q_Range);
    /* D_Start_Location */
    if (nResult == TRUE)
    {
        offset += LEN_Q_RANGE;
        if ((pSWoffRBrake->Q_Range == Q_Range_Starts) || (pSWoffRBrake->Q_Range == Q_Range_StartsEnds))
        {
            nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_D_START_LOCATION, &pSWoffRBrake->D_Start_Location);
            if (nResult == TRUE)
            {
                offset += LEN_D_START_LOCATION;
            }
        }
    }
    /* D_End_Location */
    if (nResult == TRUE)
    {
        if ((pSWoffRBrake->Q_Range == Q_Range_Ends) || (pSWoffRBrake->Q_Range == Q_Range_StartsEnds))
        {
            nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_D_END_LOCATION, &pSWoffRBrake->D_End_Location);
            if (nResult == TRUE)
            {
                offset += LEN_D_END_LOCATION;
            }
        }
    }

    return nResult;
}

/**
 *
 * @param pInB
 * @param nInBufLen
 * @param offset
 * @param pSWoffEDBrake
 * @return
 */
BOOL Decode_Packet07_SwitchOffEddyCurrentBrake(const UINT8* pInBuf, const UINT32 nInBufLen, UINT32& offset, SwitchOffEddyCurrentBrake* pSWoffEDBrake)
{
    BOOL nResult = TRUE;
    /* Q_Range*/
    nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_RANGE, &pSWoffEDBrake->Q_Range);
    /* D_Start_Location */
    if (nResult == TRUE)
    {
        offset += LEN_Q_RANGE;
        if ((pSWoffEDBrake->Q_Range == Q_Range_Starts) || (pSWoffEDBrake->Q_Range == Q_Range_StartsEnds))
        {
            nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_D_START_LOCATION, &pSWoffEDBrake->D_Start_Location);
            if (nResult == TRUE)
            {
                offset += LEN_D_START_LOCATION;
            }
        }
    }
    /* D_End_Location */
    if (nResult == TRUE)
    {
        if ((pSWoffEDBrake->Q_Range == Q_Range_Ends) || (pSWoffEDBrake->Q_Range == Q_Range_StartsEnds))
        {
            nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_D_END_LOCATION, &pSWoffEDBrake->D_End_Location);
            if (nResult == TRUE)
            {
                offset += LEN_D_END_LOCATION;
            }
        }
    }
    return nResult;
}

/**
 *
 * @param pInBuif
 * @param nInBufLen
 * @param offset
 * @param pSWoffEDEBrake
 * @return
 */
BOOL Decode_Packet07_SwitchOffEddyCurrentEmergencyBrake(const UINT8* pInBuf, const UINT32 nInBufLen, UINT32& offset, SwitchOffEddyEmergencyBrake* pSWoffEDEBrake)
{
    BOOL nResult = TRUE;

    /* Q_Range*/
    nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_RANGE, &pSWoffEDEBrake->Q_Range);
    /* D_Start_Location */
    if (nResult == TRUE)
    {
        offset += LEN_Q_RANGE;
        if ((pSWoffEDEBrake->Q_Range == Q_Range_Starts) || (pSWoffEDEBrake->Q_Range == Q_Range_StartsEnds))
        {
            nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_D_START_LOCATION, &pSWoffEDEBrake->D_Start_Location);
            if (nResult == TRUE)
            {
                offset += LEN_D_START_LOCATION;
            }
        }
    }
    /* D_End_Location */
    if (nResult == TRUE)
    {
        if ((pSWoffEDEBrake->Q_Range == Q_Range_Ends) || (pSWoffEDEBrake->Q_Range == Q_Range_StartsEnds))
        {
            nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_D_END_LOCATION, &pSWoffEDEBrake->D_End_Location);
            if (nResult == TRUE)
            {
                offset += LEN_D_END_LOCATION;
            }
        }
    }
    return nResult;
}

/**
 *
 * @param pInBuf
 * @param nInBufLen
 * @param offset
 * @param pSWoffMgShoeBrake
 * @return
 */
BOOL Decode_Packet07_SwitchOfMagmaticShoeBrake(const UINT8* pInBuf, const UINT32 nInBufLen, UINT32& offset, SwitchOffMagmaticShoeBrake* pSWoffMgShoeBrake)
{
    BOOL nResult = TRUE;

    /* Q_Range*/
    nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_RANGE, &pSWoffMgShoeBrake->Q_Range);
    /* D_Start_Location */
    if (nResult == TRUE)
    {
        offset += LEN_Q_RANGE;
        if ((pSWoffMgShoeBrake->Q_Range == Q_Range_Starts) || (pSWoffMgShoeBrake->Q_Range == Q_Range_StartsEnds))
        {
            nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_D_START_LOCATION, &pSWoffMgShoeBrake->D_Start_Location);
            if (nResult == TRUE)
            {
                offset += LEN_D_START_LOCATION;
            }
        }
    }
    /* D_End_Location */
    if (nResult == TRUE)
    {
        if ((pSWoffMgShoeBrake->Q_Range == Q_Range_Ends) || (pSWoffMgShoeBrake->Q_Range == Q_Range_StartsEnds))
        {
            nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_D_END_LOCATION, &pSWoffMgShoeBrake->D_End_Location);
            if (nResult == TRUE)
            {
                offset += LEN_D_END_LOCATION;
            }
        }
    }
    return nResult;
}

/**
 *
 * @param pInBuf
 * @param nInBufLen
 * @param offset
 * @param pBrakeForceInhibition
 * @return
 */
BOOL Decode_Packet07_AreaWithDynamicBrakeForceInhibition(const UINT8* pInBuf, const UINT32 nInBufLen, UINT32& offset, AreaWithDynamicBrakeForceInhibition* pBrakeForceInhibition)
{
    BOOL nResult = TRUE;

    /* Q_Range*/
    nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_RANGE, &pBrakeForceInhibition->Q_Range);
    /* D_Start_Location */
    if (nResult == TRUE)
    {
        offset += LEN_Q_RANGE;
        if ((pBrakeForceInhibition->Q_Range == Q_Range_Starts) || (pBrakeForceInhibition->Q_Range == Q_Range_StartsEnds))
        {
            nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_D_START_LOCATION, &pBrakeForceInhibition->D_Start_Location);
            if (nResult == TRUE)
            {
                offset += LEN_D_START_LOCATION;
            }
        }
    }
    /* D_End_Location */
    if (nResult == TRUE)
    {
        if ((pBrakeForceInhibition->Q_Range == Q_Range_Ends) || (pBrakeForceInhibition->Q_Range == Q_Range_StartsEnds))
        {
            nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_D_END_LOCATION, &pBrakeForceInhibition->D_End_Location);
            if (nResult == TRUE)
            {
                offset += LEN_D_END_LOCATION;
            }
        }
    }
    return nResult;
}

/**
 *
 * @param pInBuf
 * @param nInBufLen
 * @param offset
 * @param pBrakeForce
 * @return
 */
BOOL Decode_Packet07_AreaWithLimitedDynamicBrakeForce(const UINT8* pInBuf, const UINT32 nInBufLen, UINT32& offset, AreaWithLimitedDynamicBrakeForce* pBrakeForce)
{
    BOOL nResult = TRUE;

    /* Q_Range*/
    nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_RANGE, &pBrakeForce->Q_Range);

    /* M_Dynamic_Braking_Force_Limit */
    if (nResult == TRUE)
    {
        offset += LEN_Q_RANGE;
        nResult = Utility::Decode_uint16(pInBuf, nInBufLen, offset, LEN_M_DYNAMIC_BRAKING_FORCE_LIMIT, &pBrakeForce->M_Dynamic_Braking_Force_Limit);
#ifdef CheckValueSpare
        if (nResult == OK)
        {
            nResult = Check_M_Dynamic_Braking_Force_Limit(pBrakeForce->M_Dynamic_Braking_Force_Limit);
        }
#endif
    }
    /* D_Start_Location */
    if (nResult == TRUE)
    {
        offset += LEN_M_DYNAMIC_BRAKING_FORCE_LIMIT;
        if ((pBrakeForce->Q_Range == Q_Range_Starts) || (pBrakeForce->Q_Range == Q_Range_StartsEnds))
        {
            nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_D_START_LOCATION, &pBrakeForce->D_Start_Location);
            if (nResult == TRUE)
            {
                offset += LEN_D_START_LOCATION;
            }
        }
    }
    /* D_End_Location */
    if (nResult == TRUE)
    {
        if ((pBrakeForce->Q_Range == Q_Range_Ends) || (pBrakeForce->Q_Range == Q_Range_StartsEnds))
        {
            nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_D_END_LOCATION, &pBrakeForce->D_End_Location);
            if (nResult == TRUE)
            {
                offset += LEN_D_END_LOCATION;
            }
        }
    }
    return nResult;
}


BOOL Decode_Packet07_SegmentProfileStatus(const UINT8* pInBuf, const UINT32 nInBufLen, UINT32& offset, SegmentProfileStatus* pSPStatus)
{
    BOOL nResult = TRUE;
    UINT8 nIdx = 0;

    /* NID_C */
    nResult = Utility::Decode_uint16(pInBuf, nInBufLen, offset, LEN_NID_C, &pSPStatus->NID_C);

    /* NID_SP */
    if (nResult == TRUE)
    {
        offset += LEN_NID_C;
        nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_NID_SP, &pSPStatus->NID_SP_1);
    }
    /* Q_SP_Status */
    if (nResult == TRUE)
    {
        offset += LEN_NID_SP;
        nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_SP_STATUS, &pSPStatus->Q_SP_Status);
    }
    /* SegmentStatusValid */
    if (nResult == TRUE)
    {
        offset += LEN_Q_SP_STATUS;

        if (pSPStatus->Q_SP_Status == Q_SP_Status_Valid)
        {
            nResult = Decode_Packet07_SegmentStatusValid(pInBuf, nInBufLen, offset, &pSPStatus->spStatusValid);

            /* Q_ATOTS_Contact_Info */
            if (nResult == TRUE)
            {
                /** Q_ATOTS_Contact_Info_Dir_Normal or Q_ATOTS_Contact_Info_Dir_Reverse */
                if ((pSPStatus->spStatusValid.Q_ATOTS_Contact_Info_Dir == Q_ATOTS_Contact_Info_Dir_ATOTS_contract_info_for_normal_direction_follows)
                    || (pSPStatus->spStatusValid.Q_ATOTS_Contact_Info_Dir == Q_ATOTS_Contact_Info_Dir_ATOTS_contract_info_for_reverse_direction_follows))
                {
                    nResult = Decode_Packet07_ATOTSContactInfo(pInBuf, nInBufLen, offset, &pSPStatus->atoTSContactInfo);
                }
            }

            /****************** Static Speed Profile Start *******************************/
            /* V_STATIC */
            if (nResult == TRUE)
            {
                nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_V_STATIC, &pSPStatus->V_STATIC);
#ifdef CheckValueSpare
                if (nResult == OK)
                {
                    nResult = Check_V_STATIC(pSPStatus->V_STATIC);
                }
#endif
            }
            /* Q_FRONT */
            if (nResult == TRUE)
            {
                offset += LEN_V_STATIC;
                nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_FRONT, &pSPStatus->Q_FRONT);
            }
            /* N_ITER_ATO */
            if (nResult == TRUE)
            {
                offset += LEN_Q_FRONT;
                nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_N_ITER_ATO, &pSPStatus->N_ITER_ATO_1);
            }
            /* Specific SSP */
            if (nResult == TRUE)
            {
                offset += LEN_N_ITER_ATO;
                if (pSPStatus->N_ITER_ATO_1 > 0)
                {
                    nIdx = (UINT8)0;
                    do
                    {
                        nResult = Decode_Packet07_SpecificSSP(pInBuf, nInBufLen, offset, &pSPStatus->specificSSP[nIdx]);
                        nIdx++;
                    } while ((nResult == TRUE) && (nIdx < pSPStatus->N_ITER_ATO_1));
                }
            }
            /* N_ITER_ATO */
            if (nResult == TRUE)
            {
                nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_N_ITER_ATO, &pSPStatus->N_ITER_ATO_2);
            }
            /* Static Speed Profile Change */
            if (nResult == TRUE)
            {
                offset += LEN_N_ITER_ATO;
                if (pSPStatus->N_ITER_ATO_2 > 0)
                {
                    nIdx = (UINT8)0;
                    do
                    {
                        nResult = Decode_Packet07_StaticSPChange(pInBuf, nInBufLen, offset, &pSPStatus->staticSPChange[nIdx]);
                        nIdx++;
                    } while ((nResult == TRUE) && (nIdx < pSPStatus->N_ITER_ATO_2));
                }
            }
            /************************** Gradient Start *******************************/
            /* G_New_Gradient */
            if (nResult == TRUE)
            {
                nResult = Utility::Decode_uint16(pInBuf, nInBufLen, offset, LEN_G_NEW_GRADIENT, &pSPStatus->G_New_Gradient);
            }
            /* Q_GDIR */
            if (nResult == TRUE)
            {
                offset += LEN_G_NEW_GRADIENT;
                nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_GDIR, &pSPStatus->Q_GDIR);
            }
            /* N_ITER_ATO */
            if (nResult == TRUE)
            {
                offset += LEN_Q_GDIR;
                nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_N_ITER_ATO, &pSPStatus->N_ITER_ATO_3);
            }
            /* Static Speed Profile Change */
            if (nResult == TRUE)
            {
                offset += LEN_N_ITER_ATO;
                if (pSPStatus->N_ITER_ATO_3 > 0)
                {
                    nIdx = (UINT8)0;
                    do
                    {
                        nResult = Decode_Packet07_GradientChange(pInBuf, nInBufLen, offset, &pSPStatus->gradientChange[nIdx]);
                        nIdx++;
                    } while ((nResult == TRUE) && (nIdx < pSPStatus->N_ITER_ATO_3));
                }
            }
            /**************************  Curve Start   *******************************/
            /* Q_Radius_Category */
            if (nResult == TRUE)
            {
                nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_RADIUS_CATEGORY, &pSPStatus->Q_Radius_Category);
#ifdef CheckValueSpare
                if (nResult == OK)
                {
                    nResult = Check_Q_Radius_Category(pSPStatus->Q_Radius_Category);
                }
#endif
            }
            /* N_ITER_ATO */
            if (nResult == TRUE)
            {
                offset += LEN_Q_RADIUS_CATEGORY;
                nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_N_ITER_ATO, &pSPStatus->N_ITER_ATO_4);
            }
            /* Static Speed Profile Change */
            if (nResult == TRUE)
            {
                offset += LEN_N_ITER_ATO;
                if (pSPStatus->N_ITER_ATO_4 > 0)
                {
                    nIdx = (UINT8)0;
                    do
                    {
                        nResult = Decode_Packet07_CurveChange(pInBuf, nInBufLen, offset, &pSPStatus->curvesChange[nIdx]);
                        nIdx++;
                    } while ((nResult == TRUE) && (nIdx < pSPStatus->N_ITER_ATO_4));
                }
            }
            /************************** Power voltage Start **************************/
            /* M_VOLTAGE */
            if (nResult == TRUE)
            {
                nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_M_VOLTAGE, &pSPStatus->M_VOLTAGE);
            }
            /* NID_CTRACTION */
            if (nResult == TRUE)
            {
                offset += LEN_M_VOLTAGE;
                if (pSPStatus->M_VOLTAGE != M_VOLTAGE_Line_not_fitted_with_any_traction_system)
                {
                    nResult = Utility::Decode_uint16(pInBuf, nInBufLen, offset, LEN_NID_CTRACTION, &pSPStatus->NID_CTRACTION);
                    if (nResult == TRUE)
                    {
                        offset += LEN_NID_CTRACTION;
                    }
                }
            }
            /* N_ITER_ATO */
            if (nResult == TRUE)
            {

                nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_N_ITER_ATO, &pSPStatus->N_ITER_ATO_5);
            }
            /* Static Speed Profile Change */
            if (nResult == TRUE)
            {
                offset += LEN_N_ITER_ATO;
#ifndef KTCS_3
                if (pSPStatus->N_ITER_ATO_5 > 0)
                {
                    nIdx = (UINT8)0;
                    do
                    {
                        nResult = Decode_Packet07_PWVoltageChange(pInBuf, nInBufLen, offset, &pSPStatus->powerVoltageChange[nIdx]);
                        nIdx++;
                    } while ((nResult == TRUE) && (nIdx < pSPStatus->N_ITER_ATO_5));
                }
#endif
            }
            /********************* current Limitation start *************************/
            /* M_CURRENT */
            if (nResult == TRUE)
            {
                nResult = Utility::Decode_uint16(pInBuf, nInBufLen, offset, LEN_M_CURRENT, &pSPStatus->M_CURRENT);
            }
            /* N_ITER_ATO */
            if (nResult == TRUE)
            {
                offset += LEN_M_CURRENT;
                nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_N_ITER_ATO, &pSPStatus->N_ITER_ATO_6);
            }
            /* Current Limitation Change */
            if (nResult == TRUE)
            {
                offset += LEN_N_ITER_ATO;
                if (pSPStatus->N_ITER_ATO_6 > 0)
                {
                    nIdx = (UINT8)0;
                    do
                    {
                        nResult = Decode_Packet07_CurrentLimitChange(pInBuf, nInBufLen, offset, &pSPStatus->currentLimitChange[nIdx]);
                        nIdx++;
                    } while ((nResult == TRUE) && (nIdx < pSPStatus->N_ITER_ATO_6));
                }
            }
            /*********************   Balise Group  *************************/
            /* N_ITER_ATO */
            if (nResult == TRUE)
            {
                nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_N_ITER_ATO, &pSPStatus->N_ITER_ATO_7);
            }
            /*  Balise Group */
            if (nResult == TRUE)
            {
                offset += LEN_N_ITER_ATO;
                if (pSPStatus->N_ITER_ATO_7 > 0)
                {
                    nIdx = (UINT8)0;
                    do
                    {
                        nResult = Decode_Packet07_BaliseGroup(pInBuf, nInBufLen, offset, &pSPStatus->baliseGroup[nIdx]);
                        nIdx++;
                    } while ((nResult == TRUE) && (nIdx < pSPStatus->N_ITER_ATO_7));
                }
            }
            /*********************   TimingPoints  *************************/
            /* N_ITER_ATO */
            if (nResult == TRUE)
            {
                nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_N_ITER_ATO, &pSPStatus->N_ITER_ATO_8);
            }
            /* TimingPoints */
            if (nResult == TRUE)
            {
                offset += LEN_N_ITER_ATO;
                if (pSPStatus->N_ITER_ATO_8 > 0)
                {
                    nIdx = (UINT8)0;
                    do
                    {
                        nResult = Decode_Packet07_TimingPoints(pInBuf, nInBufLen, offset, &pSPStatus->timingPoints[nIdx]);
                        nIdx++;
                    } while ((nResult == TRUE) && (nIdx < pSPStatus->N_ITER_ATO_8));
                }
            }
            /*********************   Platform Area  *************************/
            /* N_ITER_ATO */
            if (nResult == TRUE)
            {
                nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_N_ITER_ATO, &pSPStatus->N_ITER_ATO_9);
            }
            /*  Platform Area  */
            if (nResult == TRUE)
            {
                offset += LEN_N_ITER_ATO;
                if (pSPStatus->N_ITER_ATO_9 > 0)
                {
                    nIdx = (UINT8)0;
                    do
                    {
                        nResult = Decode_Packet07_PlatformArea(pInBuf, nInBufLen, offset, &pSPStatus->platformArea[nIdx]);
                        nIdx++;
                    } while ((nResult == TRUE) && (nIdx < pSPStatus->N_ITER_ATO_9));
                }
            }
            /*********************  Tunnel  *************************/
            /* N_ITER_ATO */
            if (nResult == TRUE)
            {
                nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_N_ITER_ATO, &pSPStatus->N_ITER_ATO_10);
            }
            /*   Tunnel  */
            if (nResult == TRUE)
            {
                offset += LEN_N_ITER_ATO;
                if (pSPStatus->N_ITER_ATO_10 > 0)
                {
                    nIdx = (UINT8)0;
                    do
                    {
                        nResult = Decode_Packet07_Tunnel(pInBuf, nInBufLen, offset, &pSPStatus->tunnel[nIdx]);
                        nIdx++;
                    } while ((nResult == TRUE) && (nIdx < pSPStatus->N_ITER_ATO_10));
                }
            }
            /*********************  Axle Load Speed Profile  *************************/
            /* N_ITER_ATO */
            if (nResult == TRUE)
            {
                nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_N_ITER_ATO, &pSPStatus->N_ITER_ATO_11);
            }
            /*   Axle Load Speed Profile  */
            if (nResult == TRUE)
            {
                offset += LEN_N_ITER_ATO;
                if (pSPStatus->N_ITER_ATO_11 > 0)
                {
                    nIdx = (UINT8)0;
                    do
                    {
                        nResult = Decode_Packet07_AxleLoadSP(pInBuf, nInBufLen, offset, &pSPStatus->axleLoadSpeedProfile[nIdx]);
                        nIdx++;
                    } while ((nResult == TRUE) && (nIdx < pSPStatus->N_ITER_ATO_11));
                }
            }
            /*********************  Unprotected Level Crossing Stop  *************************/
            /* N_ITER_ATO */
            if (nResult == TRUE)
            {
                nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_N_ITER_ATO, &pSPStatus->N_ITER_ATO_12);
            }
            /*  Unprotected Level Crossing Stop  */
            if (nResult == TRUE)
            {
                offset += LEN_N_ITER_ATO;
#ifndef KTCS_3
                if (pSPStatus->N_ITER_ATO_12 > 0)
                {
                    nIdx = (UINT8)0;
                    do
                    {
                        nResult = Decode_Packet07_UnprotectedLevelCrossingStop(pInBuf, nInBufLen, offset, &pSPStatus->unprotectedLevelCrossingStop[nIdx]);
                        nIdx++;
                    } while ((nResult == TRUE) && (nIdx < pSPStatus->N_ITER_ATO_12));
                }
#endif
            }
            /********************* Permitted Braking Distance  *************************/
            /* N_ITER_ATO */
            if (nResult == TRUE)
            {
                nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_N_ITER_ATO, &pSPStatus->N_ITER_ATO_13);
            }
            /*  Permitted Braking Distance  */
            if (nResult == TRUE)
            {
                offset += LEN_N_ITER_ATO;
#ifndef KTCS_3
                if (pSPStatus->N_ITER_ATO_13 > 0)
                {
                    nIdx = (UINT8)0;
                    do
                    {
                        nResult = Decode_Packet07_PermittedBrakingDistance(pInBuf, nInBufLen, offset, &pSPStatus->permittedBrakingDistance[nIdx]);
                        nIdx++;
                    } while ((nResult == TRUE) && (nIdx < pSPStatus->N_ITER_ATO_13));
                }
#endif
            }
            /********************* Switch Off Regenerative Brake  *************************/
            /* N_ITER_ATO */
            if (nResult == TRUE)
            {
                nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_N_ITER_ATO, &pSPStatus->N_ITER_ATO_14);
            }
            /*  Switch Off Regenerative Brake  */
            if (nResult == TRUE)
            {
                offset += LEN_N_ITER_ATO;
#ifndef KTCS_3
                if (pSPStatus->N_ITER_ATO_14 > 0)
                {
                    nIdx = (UINT8)0;
                    do
                    {
                        nResult = Decode_Packet07_SwitchOffRegenerativeBrake(pInBuf, nInBufLen, offset, &pSPStatus->switchOffRegenerativeBrake[nIdx]);
                        nIdx++;
                    } while ((nResult == TRUE) && (nIdx < pSPStatus->N_ITER_ATO_14));
                }
#endif
            }
            /********************* Switch Off Eddy Current Brake  *************************/
            /* N_ITER_ATO */
            if (nResult == TRUE)
            {
                nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_N_ITER_ATO, &pSPStatus->N_ITER_ATO_15);
            }
            /*  Switch Off Eddy Current Brake  */
            if (nResult == TRUE)
            {
                offset += LEN_N_ITER_ATO;
#ifndef KTCS_3
                if (pSPStatus->N_ITER_ATO_15 > 0)
                {
                    nIdx = (UINT8)0;
                    do
                    {
                        nResult = Decode_Packet07_SwitchOffEddyCurrentBrake(pInBuf, nInBufLen, offset, &pSPStatus->switchOffEddyCurrentBrake[nIdx]);
                        nIdx++;
                    } while ((nResult == TRUE) && (nIdx < pSPStatus->N_ITER_ATO_15));
                }
#endif
            }
            /*********************Switch Off Eddy Current Emergency Brake *************************/
            /* N_ITER_ATO */
            if (nResult == TRUE)
            {
                nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_N_ITER_ATO, &pSPStatus->N_ITER_ATO_16);
            }
            /*  Switch Off Eddy Current Emergency Brake   */
            if (nResult == TRUE)
            {
                offset += LEN_N_ITER_ATO;
#ifndef KTCS_3
                if (pSPStatus->N_ITER_ATO_16 > 0)
                {
                    nIdx = (UINT8)0;
                    do
                    {
                        nResult = Decode_Packet07_SwitchOffEddyCurrentEmergencyBrake(pInBuf, nInBufLen, offset, &pSPStatus->switchOffEddyEmergencyBrake[nIdx]);
                        nIdx++;
                    } while ((nResult == TRUE) && (nIdx < pSPStatus->N_ITER_ATO_16));
                }
#endif
            }
            /*********************  Switch Off Magmatic Shoe Brake  *************************/
            /* N_ITER_ATO */
            if (nResult == TRUE)
            {
                nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_N_ITER_ATO, &pSPStatus->N_ITER_ATO_17);
            }
            /*  Switch Off Magmatic Shoe Brake  */
            if (nResult == TRUE)
            {
                offset += LEN_N_ITER_ATO;
#ifndef KTCS_3
                if (pSPStatus->N_ITER_ATO_17 > 0)
                {
                    nIdx = (UINT8)0;
                    do
                    {
                        nResult = Decode_Packet07_SwitchOfMagmaticShoeBrake(pInBuf, nInBufLen, offset, &pSPStatus->switchOffMagmaticShoeBrake[nIdx]);
                        nIdx++;
                    } while ((nResult == TRUE) && (nIdx < pSPStatus->N_ITER_ATO_17));
                }
#endif
            }
            /********************* Area With Dynamic Brake Force Inhibition  *************************/
            /* N_ITER_ATO */
            if (nResult == TRUE)
            {
                nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_N_ITER_ATO, &pSPStatus->N_ITER_ATO_18);
            }
            /*  Area With Dynamic Brake Force Inhibition */
            if (nResult == TRUE)
            {
                offset += LEN_N_ITER_ATO;
#ifndef KTCS_3
                if (pSPStatus->N_ITER_ATO_18 > 0)
                {
                    nIdx = (UINT8)0;
                    do
                    {
                        nResult = Decode_Packet07_AreaWithDynamicBrakeForceInhibition(pInBuf, nInBufLen, offset, &pSPStatus->areaWithDynamicBrakeForceInhibition[nIdx]);
                        nIdx++;
                    } while ((nResult == TRUE) && (nIdx < pSPStatus->N_ITER_ATO_18));
                }
#endif
            }
            /********************* Switch Off Regenerative Brake  *************************/
            /* N_ITER_ATO */
            if (nResult == TRUE)
            {
                nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_N_ITER_ATO, &pSPStatus->N_ITER_ATO_19);
            }
            /*  Switch Off Regenerative Brake  */
            if (nResult == TRUE)
            {
                offset += LEN_N_ITER_ATO;
#ifndef KTCS_3
                if (pSPStatus->N_ITER_ATO_19 > 0)
                {
                    nIdx = (UINT8)0;
                    do
                    {
                        nResult = Decode_Packet07_AreaWithLimitedDynamicBrakeForce(pInBuf, nInBufLen, offset, &pSPStatus->areaWithLimitedDynamicBrakeForce[nIdx]);
                        nIdx++;
                    } while ((nResult == TRUE) && (nIdx < pSPStatus->N_ITER_ATO_19));
                }
#endif
            }
        }
    }
    return nResult;
}


BOOL DecodeMessage_ATO_Packet07_SP(TS_OB_Packet07_SPStruct* pMsg, UINT32& offset, const UINT8* pInBuf, UINT32 nInBufLen)
{
    BOOL nResult = TRUE;
    UINT8 nIdx = 0;

    /* N_ITER_ATO */
    nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_N_ITER_ATO, &pMsg->N_ITER_ATO);
    if ((nResult == TRUE) && (pMsg->N_ITER_ATO > 0))
    {
        offset += LEN_N_ITER_ATO;
        nIdx = (UINT8)0;
        do
        {
            /** Segment Profile Status */
            nResult = Decode_Packet07_SegmentProfileStatus(pInBuf, nInBufLen, offset, &pMsg->SPStatus[nIdx]);
            nIdx++;
        } while ((nResult == TRUE) && (nIdx < pMsg->N_ITER_ATO));
    }

    return nResult;
}



BOOL DecodeMessage_ATO_Packet08_STR(OB_TS_Packet08_STRStruct* pMsg, UINT32& offset, const UINT8* pInBuf, UINT32 nInBufLen)
{
    BOOL nResult = TRUE;
    if (pMsg == nullptr)return FALSE;


    /* M_ATO_State */
    nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_M_ATO_STATE, &pMsg->M_ATO_State);
    /* Q_STR_Indication */
    if (nResult == TRUE)
    {
        offset += LEN_M_ATO_STATE;
        nResult = Utility::Decode_uint16(pInBuf, nInBufLen, offset, LEN_Q_STR_INDICATION, &pMsg->Q_STR_Indication);
    }
    /* V_TRAIN_ATO */
    if (nResult == TRUE)
    {
        offset += LEN_Q_STR_INDICATION;
        nResult = Utility::Decode_uint16(pInBuf, nInBufLen, offset, LEN_V_TRAIN_ATO, &pMsg->V_TRAIN_ATO);
    }
    /* L_TRAIN */
    if (nResult == TRUE)
    {
        offset += LEN_V_TRAIN_ATO;
        nResult = Utility::Decode_uint16(pInBuf, nInBufLen, offset, LEN_L_TRAIN, &pMsg->L_TRAIN);
    }
    /* DRIVER_ID */
    if (nResult == TRUE)
    {
        offset += LEN_L_TRAIN;
        UINT8 nIdx = 0;
        do
        {
            nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, 8, &pMsg->DRIVER_ID[nIdx]);
            offset += 8;
            nIdx++;
        } while ((nResult == TRUE) && (nIdx < LEN_DRIVER_ID_ARRAY));
    }
    /* STR Sending Location */
    if (nResult == TRUE)
    {        
        nResult = Decode_Packet08_STRSendingLocation(pInBuf, nInBufLen, offset, &pMsg->sendingLocation);
    }
    /* Previous TP Information */
    if (nResult == TRUE)
    {
       
        nResult = Decode_Packet08_PrevTPInfo(pInBuf, nInBufLen, offset, &pMsg->previousTpInfo);
    }
    if (pMsg->previousTpInfo.Q_Pass_Stop_Depart == Q_Pass_Stop_Depart_Stopped_TP)
    {
        /* Q_Accurate_Stopping */
        if (nResult == TRUE)
        {
            nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_ACCURATE_STOPPING, &pMsg->previousTpInfo.Q_Accurate_Stopping);
            
            if (nResult == TRUE)
            {
                offset += LEN_Q_ACCURATE_STOPPING;
            }
        }        
    }

    /* N_ITER */
    if (nResult == TRUE)
    {        
        nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_N_ITER, &pMsg->N_ITER);
    }
    if (nResult == TRUE)
    {
        offset += LEN_N_ITER;
        if (pMsg->N_ITER > 0)
        {
            UINT8 nIdx = 0;
            do
            {
                nResult = Decode_Packet08_TPEstimation(pInBuf, nInBufLen, offset, &pMsg->TPEstimation[nIdx]);
                nIdx++;
            } while ((nResult == TRUE) && (nIdx < pMsg->N_ITER));
        }

    }

    return nResult;

}

BOOL DecodeMessage_ATO_Packet09_STRAck(TS_OB_Packet09_STRAckStruct* pMsg, UINT32& offset, const UINT8* pInBuf, UINT32 nInBufLen)
{
    BOOL nResult = TRUE;

    /**  T_STR_Ref_TimeStamp_Date */
    nResult = Utility::Decode_uint16(pInBuf, nInBufLen, offset, LEN_T_STR_REFERENCE_TIMESTAMP_DATE, &pMsg->T_STR_Ref_TimeStamp_Date);

    /**  T_STR_Ref_TimeStamp_Seconds */
    if (nResult == TRUE)
    {
        offset += LEN_T_STR_REFERENCE_TIMESTAMP_DATE;
        nResult = Utility::Decode_uint32(pInBuf, nInBufLen, offset, LEN_T_STR_REFERENCE_TIMESTAMP_SECONDS, &pMsg->T_STR_Ref_TimeStamp_Seconds);
    }
    /**  N_STR_Ref_Packet_Counter */
    if (nResult == TRUE)
    {
        offset += LEN_T_STR_REFERENCE_TIMESTAMP_SECONDS;
        nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_N_STR_REFERENCE_PACKET_COUNTER, &pMsg->N_STR_Ref_Packet_Counter);
    }
    if (nResult == TRUE)
    {
        offset += LEN_N_STR_REFERENCE_PACKET_COUNTER;
    }

    return nResult;
}

BOOL DecodeMessage_ATO_Packet11_SessTerm(OB_TS_Packet11_SESSTermStruct* pMsg, UINT32& offset, const UINT8* pInBuf, UINT32 nInBufLen)
{
    BOOL nResult = TRUE;
    if (pMsg == nullptr)return FALSE;

    if (nResult == TRUE)
    {
        nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_Q_TERMINATION_REASON, &pMsg->Q_Termination_Reason);
    }

    if (nResult == TRUE)
    {
        offset += LEN_Q_TERMINATION_REASON;
    }

    return nResult;

}

BOOL DecodeMessage_ATO_Packet01_HsAck(TS_OB_Packet01_HSAckStruct* pMsg, UINT32& offset, const UINT8* pInBuf, UINT32 nInBufLen)
{
    BOOL nResult = TRUE;



    /** NID_C **/
    nResult = Utility::Decode_uint16(pInBuf, nInBufLen, offset, LEN_NID_C, &pMsg->NID_C);

    /** NID_ATOTS **/
    if (nResult == TRUE)
    {
        offset += LEN_NID_C;
        nResult = Utility::Decode_uint16(pInBuf, nInBufLen, offset, LEN_NID_ATOTS, &pMsg->NID_ATOTS);
    }

    /** M_ATO_Version **/
    if (nResult == TRUE)
    {
        offset += LEN_NID_ATOTS;
        nResult = Utility::Decode_uint16(pInBuf, nInBufLen, offset, LEN_M_ATO_VERSION, &pMsg->M_ATO_Version);
    }

    /** T_Timeout_ATO_TS_Response **/
    if (nResult == TRUE)
    {
        offset += LEN_M_ATO_VERSION;
        nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_T_TIMEOUT_ATOTS_RESPONSE, &pMsg->T_Timeout_ATOTOS_Response);
    }

    /** T_Reporting_Time **/
    if (nResult == TRUE)
    {
        offset += LEN_T_TIMEOUT_ATOTS_RESPONSE;
        nResult = Utility::Decode_uint8(pInBuf, nInBufLen, offset, LEN_T_REPORTING_TIME, &pMsg->T_Reporting_Time);
    }
    if (nResult == TRUE)
    {
        offset += LEN_T_REPORTING_TIME;
    }


    return nResult;
}


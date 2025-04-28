#include "pch.h"
#include "F110_Msg_Encoder_ATO.h"


/**
 * ATO-OB로 전송할 Message의 Header사이즈 반환
 * @return
 */
INT32 GetATOHeaderSize(void)
{
    UINT32 nRet = 0;
    nRet = LEN_NID_PACKET_ATO + LEN_L_PACKET_ATO + LEN_NID_OPERATIONAL + LEN_NID_ENGINE
        + LEN_T_TIMESTAMP_DATE + LEN_T_TIMESTAMP_SECONDS + LEN_N_PACKET_COUNTER;

    return nRet;
}

/**
 * Hs Ackknowledgement packet 01 길이 반환
 * @param pMsg
 * @return
 */
UINT32 GetLengthPacket01_HsAck(TS_OB_Packet01_HSAckStruct* pMsg)
{
    UINT32 nLength = 0;
    nLength += GetATOHeaderSize();

    
    nLength += LEN_NID_C + LEN_NID_ATOTS + LEN_M_ATO_VERSION
         + LEN_T_TIMEOUT_ATOTS_RESPONSE + LEN_T_REPORTING_TIME;    
   
    return nLength;
}

UINT32 GetLengthPacket02_HsRej(TS_OB_Packet02_HSRejStruct* pMsg)
{
    UINT32 nLength = 0;
    nLength += GetATOHeaderSize();


    nLength += LEN_Q_REJECT_REASON + LEN_NID_C + LEN_NID_ATOTS;

    return nLength;
}

UINT32 GetLengthPacket04_Jp(TS_OB_Packet04_JPStruct* pMsg)
{
    UINT32 nLength = 0;
    nLength += GetATOHeaderSize();

    nLength += LEN_Q_JP_STATUS;

    if ((pMsg->Q_JP_Status == Q_JP_Status_Valid)
        || (pMsg->Q_JP_Status == Q_JP_Status_Update)
        || (pMsg->Q_JP_Status == Q_JP_Status_Overwrite))
    {
        if (pMsg->N_ITER_SP > 0)
        {
            nLength += LEN_N_ITER_SP;

            for (int k = 0; k < pMsg->N_ITER_SP; k++)
            {

                nLength += LEN_NID_C + LEN_NID_SP + LEN_M_SP_VERSION + LEN_Q_SP_DIR + LEN_N_ITER + LEN_N_ITER;   // iter = 0

                if (pMsg->segmentProfileReference[k].N_ITER > 0)  //TP Iter
                {
                    for (int l = 0; l < pMsg->segmentProfileReference[k].N_ITER; l++)
                    {
                        nLength += LEN_NID_TP + LEN_T_LATEST_ARRIVAL_DATE + LEN_T_LATEST_ARRIVAL_SECONDS + LEN_T_ARRIVAL_WINDOW + LEN_Q_TP_ALIGNMENT + LEN_Q_STOP_SKIP_PASS +
                            LEN_Q_TP_INFORMATION + LEN_Q_DAY_LIGHT_SAVING;

                        if (pMsg->segmentProfileReference[k].TPConstraints[l].Q_Stop_Skip_Pass == Q_Stop_SKIP_Pass_STOPPING_POINT)
                        {
                            nLength += LEN_Q_OPENING_DOOR_SIDE + LEN_Q_CENTRALISED_OPENING + LEN_Q_RELAXED_COUPLER;

                            if (pMsg->segmentProfileReference[k].TPConstraints[l].Q_TP_information != Q_TP_information_End_of_Journey)
                            {
                                nLength += LEN_Q_TRAIN_HOLD + LEN_T_DEPARTURE_DATE + LEN_T_DEPARTURE_SECONDS + LEN_T_MINIMUM_DWELL_TIME + LEN_Q_AUTOMATIC_CLOSING;

                               /* if (pMsg->segmentProfileReference[k].TPConstraints[l].stpDepartureDetails.Q_Train_Hold != Q_Train_Hold_Is_Hold_train)
                                {
                                    nLength += LEN_T_DEPARTURE_DATE + LEN_T_DEPARTURE_SECONDS + LEN_T_MINIMUM_DWELL_TIME + LEN_Q_AUTOMATIC_CLOSING;
                                }*/
                            }
                            
                        }
                    }                    
                }

                if (pMsg->segmentProfileReference[k].N_ITER_tempConstraints > 0) // TC Iter
                {                    
                    for (int l = 0; l < pMsg->segmentProfileReference[k].N_ITER_tempConstraints; l++)
                    {
                        nLength += LEN_Q_TC_TYPE + LEN_Q_RANGE + LEN_D_TC_START_LOCATION + LEN_D_TC_END_LOCATION;
                       
                        if (pMsg->segmentProfileReference[k].temporaryConstrains[l].Q_TC_Type == Q_TC_Type_ASR)
                        {
                            nLength += LEN_Q_FRONT + LEN_V_SPEED_LEVEL;
                        }
                        else if (pMsg->segmentProfileReference[k].temporaryConstrains[l].Q_TC_Type == Q_TC_Type_Low_Adhesion)
                        {
                            nLength += LEN_Q_ADHESION_CATEGORY;
                        }
                        else if (pMsg->segmentProfileReference[k].temporaryConstrains[l].Q_TC_Type == Q_TC_Type_Current_Limitation)
                        {
                            nLength += LEN_M_CURRENT;
                        }
                    }
                }
            }
        }
    }
    return nLength;
}
    


UINT32 GetLengthPacket07_Sp(TS_OB_Packet07_SPStruct* pMsg)
{
    UINT32 nLength = 0;

    nLength += GetATOHeaderSize();

    nLength += LEN_N_ITER;

    if (pMsg->N_ITER_ATO > 0)
    {
        // 1st ITER
        for (int k = 0; k < pMsg->N_ITER_ATO; k++)      
        {

            nLength += LEN_NID_C + LEN_NID_SP + LEN_Q_SP_STATUS;

            // Q_SP_Status Check : 
            if (pMsg->SPStatus[k].Q_SP_Status == Q_SP_Status_Valid)
            {
                nLength += LEN_M_SP_VERSION + LEN_L_SP + LEN_D_EOA_OFFSET + LEN_Q_UTC_OFFSET + LEN_M_SP_ATTITUDE +
                    LEN_Q_ATOTS_CONTACT_INFO_DIR;

                // Contact info Check : 
                if ((pMsg->SPStatus[k].spStatusValid.Q_ATOTS_Contact_Info_Dir == Q_ATOTS_Contact_Info_Dir_ATOTS_contract_info_for_normal_direction_follows)
                    || (pMsg->SPStatus[k].spStatusValid.Q_ATOTS_Contact_Info_Dir == Q_ATOTS_Contact_Info_Dir_ATOTS_contract_info_for_reverse_direction_follows))
                {
                    nLength += LEN_NID_C + LEN_NID_ATOTS + LEN_NID_C + LEN_NID_SP;
                }

                // N_ITERS = 0 
                nLength += LEN_V_STATIC + LEN_Q_FRONT + LEN_N_ITER + LEN_N_ITER + LEN_G_NEW_GRADIENT + LEN_Q_GDIR + LEN_N_ITER + LEN_Q_RADIUS_CATEGORY + LEN_N_ITER +
                    LEN_M_VOLTAGE + LEN_N_ITER + LEN_M_CURRENT + LEN_N_ITER + LEN_N_ITER + LEN_N_ITER + LEN_N_ITER + LEN_N_ITER + LEN_N_ITER +
                    LEN_N_ITER + LEN_N_ITER + LEN_N_ITER + LEN_N_ITER + LEN_N_ITER + LEN_N_ITER + LEN_N_ITER + LEN_N_ITER;

                if (pMsg->SPStatus[k].M_VOLTAGE != M_VOLTAGE_Line_not_fitted_with_any_traction_system)
                {
                    nLength += LEN_NID_CTRACTION;
                }

                if (pMsg->SPStatus[k].N_ITER_ATO_1 > 0)   // Specific SSP
                {
                    for (int l = 0; l < pMsg->SPStatus[k].N_ITER_ATO_1; l++)
                    {
                        nLength += LEN_Q_DIFF;

                        if (pMsg->SPStatus[k].specificSSP[l].Q_DIFF == Q_DIFF_Cant_Deficiency_specific_category) 
                        {
                            nLength += LEN_NC_CDDIFF;
                        }

                        else if ((pMsg->SPStatus[k].specificSSP[l].Q_DIFF == Q_DIFF_Other_specific_category_replaces_the_Cant_Deficiency_SSP)
                            || (pMsg->SPStatus[k].specificSSP[l].Q_DIFF == Q_DIFF_Other_specific_category_does_not_replace_the_Cant_Deficiency_SSP))
                        {
                            nLength += LEN_NC_DIFF + LEN_V_DIFF;
                        }

                    }
                }

                if (pMsg->SPStatus[k].N_ITER_ATO_2 > 0)   // Static Speed Profile Change 
                {
                    for (int l = 0; l < pMsg->SPStatus[k].N_ITER_ATO_2; l++)
                    {
                        nLength += LEN_D_LOCATION + LEN_V_STATIC + LEN_Q_FRONT + LEN_N_ITER;    

                        if (pMsg->SPStatus[k].staticSPChange[l].N_ITER > 0)     // Specific SSP Change
                        {
                            for (int m = 0; m < pMsg->SPStatus[k].staticSPChange[l].N_ITER; m++) 
                            {

                                nLength += LEN_Q_DIFF;

                                if (pMsg->SPStatus[k].staticSPChange[l].specificSspChange[m].Q_DIFF == Q_DIFF_Cant_Deficiency_specific_category)
                                {
                                    nLength += LEN_NC_CDDIFF;
                                }

                                else if ((pMsg->SPStatus[k].staticSPChange[l].specificSspChange[m].Q_DIFF == Q_DIFF_Other_specific_category_replaces_the_Cant_Deficiency_SSP)
                                    || pMsg->SPStatus[k].staticSPChange[l].specificSspChange[m].Q_DIFF == Q_DIFF_Other_specific_category_does_not_replace_the_Cant_Deficiency_SSP)
                                {
                                    nLength += LEN_NC_DIFF + LEN_V_DIFF;
                                }
                            }
                        }
                    }
                }

                if (pMsg->SPStatus[k].N_ITER_ATO_3 > 0)   // Gradients Change
                {
                    for (int l = 0; l < pMsg->SPStatus[k].N_ITER_ATO_3; l++)
                    {
                        nLength += LEN_D_LOCATION + LEN_G_NEW_GRADIENT + LEN_Q_GDIR;
                    }
                }

                if (pMsg->SPStatus[k].N_ITER_ATO_4 > 0)   // Curves Change
                {
                    for (int l = 0; l < pMsg->SPStatus[k].N_ITER_ATO_4; l++)
                    {
                        nLength += LEN_D_LOCATION + LEN_Q_RADIUS_CATEGORY;
                    }
                }

                if (pMsg->SPStatus[k].N_ITER_ATO_5 > 0)   // Power Voltage Change
                {
                    for (int l = 0; l < pMsg->SPStatus[k].N_ITER_ATO_5; l++)
                    {
                        nLength += LEN_D_LOCATION + LEN_M_VOLTAGE;
                            
                        if (pMsg->SPStatus[k].powerVoltageChange[l].M_VOLTAGE != M_VOLTAGE_Line_not_fitted_with_any_traction_system)
                        {
                            nLength += LEN_NID_CTRACTION;
                        }
                    }
                }

                if (pMsg->SPStatus[k].N_ITER_ATO_6 > 0)   // Current Limitation Change
                {
                    for (int l = 0; l < pMsg->SPStatus[k].N_ITER_ATO_6; l++)
                    {
                        nLength += LEN_D_LOCATION + LEN_M_CURRENT;
                    }
                }

                if (pMsg->SPStatus[k].N_ITER_ATO_7 > 0)   // Balise Group
                {
                    for (int l = 0; l < pMsg->SPStatus[k].N_ITER_ATO_7; l++)
                    {

                        nLength += LEN_Q_NEWNID_C + LEN_NID_BG + LEN_N_ITER_BG;

                        if (pMsg->SPStatus[k].baliseGroup[l].Q_NEWNID_C == Q_NEWNID_C_NID_C_to_be_used_follows)
                        {
                            nLength += LEN_NID_C;
                        }

                        if (pMsg->SPStatus[k].baliseGroup[l].N_ITER_BG > 0)  // Balises
                        {
                            for (int m = 0; m < pMsg->SPStatus[k].baliseGroup[l].N_ITER_BG; m++)
                            {
                                nLength += LEN_N_PIG + LEN_D_LOCATION;
                            }
                        }
                    }
                }

                if (pMsg->SPStatus[k].N_ITER_ATO_8 > 0)   // Timing Points
                {
                    for (int l = 0; l < pMsg->SPStatus[k].N_ITER_ATO_8; l++)
                    {
                        nLength += LEN_NID_TP + LEN_D_LOCATION + LEN_Q_STOP_LOCATION_TOLERANCE + LEN_Q_STP_REACHED + LEN_L_TEXT;

                        if (pMsg->SPStatus[k].timingPoints[l].L_TEXT > 0)   // Timing Point Name
                        {
                            for (int m = 0; m < pMsg->SPStatus[k].timingPoints[l].L_TEXT; m++)
                            {
                                nLength += LEN_X_TEXT;
                            }
                        }
                    }
                }

                if (pMsg->SPStatus[k].N_ITER_ATO_9 > 0)   // Platform Area
                {
                    for (int l = 0; l < pMsg->SPStatus[k].N_ITER_ATO_9; l++)
                    {
                        nLength += LEN_Q_RANGE;

                        if ((pMsg->SPStatus[k].platformArea[l].Q_Range == Q_Range_Starts) || (pMsg->SPStatus[k].platformArea[l].Q_Range == Q_Range_StartsEnds))
                        {
                            nLength += LEN_D_START_LOCATION;
                        }

                        if ((pMsg->SPStatus[k].platformArea[l].Q_Range == Q_Range_Ends) || (pMsg->SPStatus[k].platformArea[l].Q_Range == Q_Range_StartsEnds))
                        {
                            nLength += LEN_D_END_LOCATION;
                        }

                    }
                }

                if (pMsg->SPStatus[k].N_ITER_ATO_10 > 0)  // Tunnel
                {
                    for (int l = 0; l < pMsg->SPStatus[k].N_ITER_ATO_10; l++)
                    {
                        nLength += LEN_Q_RANGE + LEN_Q_TUNNEL_CATEGORY;

                        if ((pMsg->SPStatus[k].tunnel[l].Q_Range == Q_Range_Starts) || (pMsg->SPStatus[k].tunnel[l].Q_Range == Q_Range_StartsEnds))
                        {
                            nLength += LEN_D_START_LOCATION;
                        }

                        if ((pMsg->SPStatus[k].tunnel[l].Q_Range == Q_Range_Ends) || (pMsg->SPStatus[k].tunnel[l].Q_Range == Q_Range_StartsEnds))
                        {
                            nLength += LEN_D_END_LOCATION;
                        }
                    }
                }

                if (pMsg->SPStatus[k].N_ITER_ATO_11 > 0)  // Axle Load Speed Profile
                {
                    for (int l = 0; l < pMsg->SPStatus[k].N_ITER_ATO_11; l++)
                    {
                        nLength += LEN_Q_RANGE + LEN_M_AXLELOADCAT + LEN_V_NEW_SPEED_LEVEL + LEN_Q_FRONT;

                        if ((pMsg->SPStatus[k].axleLoadSpeedProfile[l].Q_Range == Q_Range_Starts) || (pMsg->SPStatus[k].axleLoadSpeedProfile[l].Q_Range == Q_Range_StartsEnds))
                        {
                            nLength += LEN_D_START_LOCATION;
                        }

                        if ((pMsg->SPStatus[k].axleLoadSpeedProfile[l].Q_Range == Q_Range_Ends) || (pMsg->SPStatus[k].axleLoadSpeedProfile[l].Q_Range == Q_Range_StartsEnds))
                        {
                            nLength += LEN_D_END_LOCATION;
                        }

                    }
                }

                if (pMsg->SPStatus[k].N_ITER_ATO_12 > 0)  // Unprotected Level Crossing Stop
                {
                    for (int l = 0; l < pMsg->SPStatus[k].N_ITER_ATO_12; l++)
                    {
                        nLength += LEN_D_UNPROTECTEDLX_STOP_NOMINAL + LEN_D_UNPROTECTEDLX_STOP_REVERSE;
                    }
                }

                if (pMsg->SPStatus[k].N_ITER_ATO_13 > 0)  // Permitted Braking Distance
                {
                    for (int l = 0; l < pMsg->SPStatus[k].N_ITER_ATO_13; l++)
                    {
                        nLength += LEN_Q_RANGE + LEN_D_PERMITTED_BRAKING_DISTANCE + LEN_Q_PBD_SBEB + LEN_G_PBD + LEN_Q_GDIR_PBD;

                        if ((pMsg->SPStatus[k].permittedBrakingDistance[l].Q_Range == Q_Range_Starts) || (pMsg->SPStatus[k].permittedBrakingDistance[l].Q_Range == Q_Range_StartsEnds))
                        {
                            nLength += LEN_D_START_LOCATION;
                        }

                        if ((pMsg->SPStatus[k].permittedBrakingDistance[l].Q_Range == Q_Range_Ends) || (pMsg->SPStatus[k].permittedBrakingDistance[l].Q_Range == Q_Range_StartsEnds))
                        {
                            nLength += LEN_D_END_LOCATION;
                        }

                    }
                }

                if (pMsg->SPStatus[k].N_ITER_ATO_14 > 0)  // Switch Off Regenerative Brake
                {
                    for (int l = 0; l < pMsg->SPStatus[k].N_ITER_ATO_14; l++)
                    {
                        nLength += LEN_Q_RANGE;

                        if ((pMsg->SPStatus[k].switchOffRegenerativeBrake[l].Q_Range == Q_Range_Starts) || (pMsg->SPStatus[k].switchOffRegenerativeBrake[l].Q_Range == Q_Range_StartsEnds))
                        {
                            nLength += LEN_D_START_LOCATION;
                        }

                        if ((pMsg->SPStatus[k].switchOffRegenerativeBrake[l].Q_Range == Q_Range_Ends) || (pMsg->SPStatus[k].switchOffRegenerativeBrake[l].Q_Range == Q_Range_StartsEnds))
                        {
                            nLength += LEN_D_END_LOCATION;
                        }
                    }
                }

                if (pMsg->SPStatus[k].N_ITER_ATO_15 > 0)  // Switch Off Eddy Current Brake
                {
                    for (int l = 0; l < pMsg->SPStatus[k].N_ITER_ATO_15; l++)
                    {
                        nLength += LEN_Q_RANGE;

                        if ((pMsg->SPStatus[k].switchOffEddyCurrentBrake[l].Q_Range == Q_Range_Starts) || (pMsg->SPStatus[k].switchOffEddyCurrentBrake[l].Q_Range == Q_Range_StartsEnds))
                        {
                            nLength += LEN_D_START_LOCATION;
                        }

                        if ((pMsg->SPStatus[k].switchOffEddyCurrentBrake[l].Q_Range == Q_Range_Ends) || (pMsg->SPStatus[k].switchOffEddyCurrentBrake[l].Q_Range == Q_Range_StartsEnds))
                        {
                            nLength += LEN_D_END_LOCATION;
                        }
                    }
                }

                if (pMsg->SPStatus[k].N_ITER_ATO_16 > 0)  // Switch Off Eddy Current Emergency Brake
                {
                    for (int l = 0; l < pMsg->SPStatus[k].N_ITER_ATO_16; l++)
                    {
                        nLength += LEN_Q_RANGE;

                        if ((pMsg->SPStatus[k].switchOffEddyEmergencyBrake[l].Q_Range == Q_Range_Starts) || (pMsg->SPStatus[k].switchOffEddyEmergencyBrake[l].Q_Range == Q_Range_StartsEnds))
                        {
                            nLength += LEN_D_START_LOCATION;
                        }

                        if ((pMsg->SPStatus[k].switchOffEddyEmergencyBrake[l].Q_Range == Q_Range_Ends) || (pMsg->SPStatus[k].switchOffEddyEmergencyBrake[l].Q_Range == Q_Range_StartsEnds))
                        {
                            nLength += LEN_D_END_LOCATION;
                        }
                    }
                }

                if (pMsg->SPStatus[k].N_ITER_ATO_17 > 0)  // Switch Off Magnetic Shoe Brake
                {
                    for (int l = 0; l < pMsg->SPStatus[k].N_ITER_ATO_17; l++)
                    {
                        nLength += LEN_Q_RANGE;

                        if ((pMsg->SPStatus[k].switchOffMagmaticShoeBrake[l].Q_Range == Q_Range_Starts) || (pMsg->SPStatus[k].switchOffMagmaticShoeBrake[l].Q_Range == Q_Range_StartsEnds))
                        {
                            nLength += LEN_D_START_LOCATION;
                        }

                        if ((pMsg->SPStatus[k].switchOffMagmaticShoeBrake[l].Q_Range == Q_Range_Ends) || (pMsg->SPStatus[k].switchOffMagmaticShoeBrake[l].Q_Range == Q_Range_StartsEnds))
                        {
                            nLength += LEN_D_END_LOCATION;
                        }
                    }
                }

                if (pMsg->SPStatus[k].N_ITER_ATO_18 > 0)  // Area with Dynamic Brake force Inhibition
                {
                    for (int l = 0; l < pMsg->SPStatus[k].N_ITER_ATO_18; l++)
                    {
                        nLength += LEN_Q_RANGE;

                        if ((pMsg->SPStatus[k].areaWithDynamicBrakeForceInhibition[l].Q_Range == Q_Range_Starts) || (pMsg->SPStatus[k].areaWithDynamicBrakeForceInhibition[l].Q_Range == Q_Range_StartsEnds))
                        {
                            nLength += LEN_D_START_LOCATION;
                        }

                        if ((pMsg->SPStatus[k].areaWithDynamicBrakeForceInhibition[l].Q_Range == Q_Range_Ends) || (pMsg->SPStatus[k].areaWithDynamicBrakeForceInhibition[l].Q_Range == Q_Range_StartsEnds))
                        {
                            nLength += LEN_D_END_LOCATION;
                        }
                    }
                }

                if (pMsg->SPStatus[k].N_ITER_ATO_19 > 0)  // Area with limited Dynamic Brake force
                {
                    for (int l = 0; l < pMsg->SPStatus[k].N_ITER_ATO_19; l++)
                    {
                        nLength += LEN_Q_RANGE + LEN_M_DYNAMIC_BRAKING_FORCE_LIMIT;

                        if ((pMsg->SPStatus[k].areaWithLimitedDynamicBrakeForce[l].Q_Range == Q_Range_Starts) || (pMsg->SPStatus[k].areaWithLimitedDynamicBrakeForce[l].Q_Range == Q_Range_StartsEnds))
                        {
                            nLength += LEN_D_START_LOCATION;
                        }

                        if ((pMsg->SPStatus[k].areaWithLimitedDynamicBrakeForce[l].Q_Range == Q_Range_Ends) || (pMsg->SPStatus[k].areaWithLimitedDynamicBrakeForce[l].Q_Range == Q_Range_StartsEnds))
                        {
                            nLength += LEN_D_END_LOCATION;
                        }
                    }
                }
            }

        }

    }

    return nLength;
}

  

UINT32 GetLengthPacket09_STRAck(TS_OB_Packet09_STRAckStruct* pMsg)
{
    UINT32 nLength = 0;
    nLength += GetATOHeaderSize();

    nLength += LEN_T_STR_REFERENCE_TIMESTAMP_DATE + LEN_T_STR_REFERENCE_TIMESTAMP_SECONDS + LEN_N_STR_REFERENCE_PACKET_COUNTER;

    return nLength;
}

BOOL Encode_Packet04_STPDepartureDetails(UINT8* pOutBuf, const UINT32 nOutBufLen, UINT32* offset, STPDepartureDetails* pSTPDepDetails)
{
    BOOL nResult = TRUE;

    /* Q_Train_Hold */
    nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_TRAIN_HOLD, pSTPDepDetails->Q_Train_Hold);

    /* T_Departure_Date */
    if (nResult == TRUE)
    {
        *offset += LEN_Q_TRAIN_HOLD;
        nResult = Utility::Encode_uint16(pOutBuf, nOutBufLen, *offset, LEN_T_DEPARTURE_DATE, pSTPDepDetails->T_Departure_Date);
    }
    /* T_Departure_Seconds */
    if (nResult == TRUE)
    {
        *offset += LEN_T_DEPARTURE_DATE;
        nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_T_DEPARTURE_SECONDS, pSTPDepDetails->T_Departure_Seconds);
    }
    /* T_Minimum_Dwell_Time */
    if (nResult == TRUE)
    {
        *offset += LEN_T_DEPARTURE_SECONDS;
        nResult = Utility::Encode_uint16(pOutBuf, nOutBufLen, *offset, LEN_T_MINIMUM_DWELL_TIME, pSTPDepDetails->T_Minimum_Dwell_Time);
    }
    /* Q_Automatic_Closing */
    if (nResult == TRUE)
    {
        *offset += LEN_T_MINIMUM_DWELL_TIME;
        nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_AUTOMATIC_CLOSING, pSTPDepDetails->Q_Automatic_Closing);
    }
    if (nResult == TRUE)
    {
        *offset += LEN_Q_AUTOMATIC_CLOSING;
    }
    return nResult;
}




BOOL Encode_Packet04_STPInformation(UINT8* pOutBuf, const UINT32 nOutBufLen, UINT32* offset, STPInformation* pSTPInfo)
{
    BOOL nResult = TRUE;
    /* Q_Opening_Door_Side */
    nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_OPENING_DOOR_SIDE, pSTPInfo->Q_Opening_Door_Side);
    /* Q_Centralised_Opening */
    if (nResult == TRUE)
    {
        *offset += LEN_Q_OPENING_DOOR_SIDE;
        nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_CENTRALISED_OPENING, pSTPInfo->Q_Centralised_Opening);
    }
    /* Q_Relaxed_Coupler */
    if (nResult == TRUE)
    {
        *offset += LEN_Q_CENTRALISED_OPENING;
        nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_RELAXED_COUPLER, pSTPInfo->Q_Relaxed_Coupler);
    }

    if (nResult == TRUE)
    {
        *offset += LEN_Q_RELAXED_COUPLER;
    }
    return nResult;
}

BOOL Encode_Packet04_TimingPointConstraints(UINT8* pOutBuf, const UINT32 nOutBufLen, UINT32* offset, TimingPointConstraints* pTPConstraints)
{
    BOOL nResult = TRUE;

    /* NID_TP */
    nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_NID_TP, pTPConstraints->NID_TP);

    /* L_Latest_Arrival_Date */
    if (nResult == TRUE)
    {
        *offset += LEN_NID_TP;
        nResult = Utility::Encode_uint16(pOutBuf, nOutBufLen, *offset, LEN_T_LATEST_ARRIVAL_DATE, pTPConstraints->T_Latest_Arrival_Date);
    }
    /* T_Latest_Arrival_Seconds */
    if (nResult == TRUE)
    {
        *offset += LEN_T_LATEST_ARRIVAL_DATE;
        nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_T_LATEST_ARRIVAL_SECONDS, pTPConstraints->T_Latest_Arrival_Seconds);
    }
    /* T_Arrival_Window */
    if (nResult == TRUE)
    {
        *offset += LEN_T_LATEST_ARRIVAL_SECONDS;
        nResult = Utility::Encode_uint16(pOutBuf, nOutBufLen, *offset, LEN_T_ARRIVAL_WINDOW, pTPConstraints->T_Arrival_Window);
    }
    /* Q_TP_Alignment */
    if (nResult == TRUE)
    {
        *offset += LEN_T_ARRIVAL_WINDOW;
        nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_TP_ALIGNMENT, pTPConstraints->Q_TP_Alignment);
    }

    /* Q_Stop_Skip_Pass */
    if (nResult == TRUE)
    {
        *offset += LEN_Q_TP_ALIGNMENT;
        nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_STOP_SKIP_PASS, pTPConstraints->Q_Stop_Skip_Pass);
    }

    /* Q_TP_information */
    if (nResult == TRUE)
    {
        *offset += LEN_Q_STOP_SKIP_PASS;
        nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_TP_INFORMATION, pTPConstraints->Q_TP_information);

        /* Q_Day_Light_Saving */
        if (nResult == TRUE)
        {
            *offset += LEN_Q_TP_INFORMATION;
            nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_DAY_LIGHT_SAVING, pTPConstraints->Q_Day_Light_Saving);
        }
        if (nResult == TRUE)
        {
            *offset += LEN_Q_DAY_LIGHT_SAVING;

            if (pTPConstraints->Q_Stop_Skip_Pass == Q_Stop_SKIP_Pass_STOPPING_POINT)
            {
                nResult = Encode_Packet04_STPInformation(pOutBuf, nOutBufLen, offset, &pTPConstraints->stpInformation);
                if ((nResult == TRUE)
                    && (pTPConstraints->Q_TP_information != Q_TP_information_End_of_Journey))
                {
                    Encode_Packet04_STPDepartureDetails(pOutBuf, nOutBufLen, offset, &pTPConstraints->stpDepartureDetails);
                }
            }            
        }

        return nResult;
    }
}

BOOL Encode_Packet04_TemporaryConstrains(UINT8* pOutBuf, const UINT32 nOutBufLen, UINT32* offset, TemporaryConstrains* pTmpConstrains)
{
    BOOL nResult = TRUE;

    /* Q_TC_Type */
    nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_TC_TYPE, pTmpConstrains->Q_TC_Type);

    /* Q_Range */
    if (nResult == TRUE)
    {
        *offset += LEN_Q_TC_TYPE;
        nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_RANGE, pTmpConstrains->Q_Range);
    }
    /* D_TC_Start_Location */
    if (nResult == TRUE)
    {
        *offset += LEN_Q_RANGE;
        nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_D_TC_START_LOCATION, pTmpConstrains->D_TC_Start_Location);
    }
    /* D_TC_End_Location */
    if (nResult == TRUE)
    {
        *offset += LEN_D_TC_START_LOCATION;
        nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_D_TC_END_LOCATION, pTmpConstrains->D_TC_End_Location);
    }

    if (nResult == TRUE)
    {
        *offset += LEN_D_TC_END_LOCATION;
        if (pTmpConstrains->Q_TC_Type == Q_TC_Type_ASR)
        {
            /** ASR - Q_FRONT */
            nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_FRONT, pTmpConstrains->asr.Q_FRONT);
            /** ASR - V_Speed_Level */
            if (nResult == TRUE)
            {
                *offset += LEN_Q_FRONT;
                nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_V_SPEED_LEVEL, pTmpConstrains->asr.V_Speed_Level);
            }
            if (nResult == TRUE)
            {
                *offset += LEN_V_SPEED_LEVEL;
            }
        }
        else if (pTmpConstrains->Q_TC_Type == Q_TC_Type_Low_Adhesion)
        {
            /** Low Adhesion - Q_Adhesion_Category */
            nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_ADHESION_CATEGORY, pTmpConstrains->lowAdhesion.Q_Adhesion_Category);

            if (nResult == TRUE)
            {
                *offset += LEN_Q_ADHESION_CATEGORY;
            }
        }
        else if (pTmpConstrains->Q_TC_Type == Q_TC_Type_Current_Limitation)
        {
            nResult = Utility::Encode_uint16(pOutBuf, nOutBufLen, *offset, LEN_M_CURRENT, pTmpConstrains->currentLimitation.M_CURRENT);
            if (nResult == TRUE)
            {
                *offset += LEN_M_CURRENT;
            }
        }
    }

    return nResult;
}

BOOL Encode_Packet04_SegmentProfileReference(UINT8* pOutBuf, const UINT32 nOutBufLen, UINT32* offset, SegmentProfileReference* pSPRef)
{
    BOOL nResult = TRUE;

    /* NID_C */
    nResult = Utility::Encode_uint16(pOutBuf, nOutBufLen, *offset, LEN_NID_C, pSPRef->NID_C);

    /* NID_SP */
    if (nResult == TRUE)
    {
        *offset += LEN_NID_C;
        nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_NID_SP, pSPRef->NID_SP);
    }
    /* M_SP_Version */
    if (nResult == TRUE)
    {
        *offset += LEN_NID_SP;
        nResult = Utility::Encode_uint16(pOutBuf, nOutBufLen, *offset, LEN_M_SP_VERSION, pSPRef->M_SP_Version);

    }
    /* Q_SP_DIR */
    if (nResult == TRUE)
    {
        *offset += LEN_M_SP_VERSION;
        nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_SP_DIR, pSPRef->Q_SP_DIR);
    }
    /* N_ITER */
    if (nResult == TRUE)
    {
        *offset += LEN_Q_SP_DIR;
        nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_N_ITER, pSPRef->N_ITER);
    }
    /* TimingPointConstraints */
    if (nResult == TRUE)
    {
        *offset += LEN_N_ITER;
        if (pSPRef->N_ITER > 0)
        {
            UINT8 nIdx = (UINT8)0;
            do
            {
                nResult = Encode_Packet04_TimingPointConstraints(pOutBuf, nOutBufLen, offset, &pSPRef->TPConstraints[nIdx]);
                nIdx++;
            } while ((nResult == TRUE) && (nIdx < pSPRef->N_ITER));
        }
    }

    /** N_ITER2 */
    if (nResult == TRUE)
    {
        nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_N_ITER, pSPRef->N_ITER_tempConstraints);
    }
    /** Temporary Constrains */
    if (nResult == TRUE)
    {
        *offset += LEN_N_ITER;
        if (pSPRef->N_ITER_tempConstraints > 0)
        {
            UINT8 nIdx = (UINT8)0;
            do
            {
                nResult = Encode_Packet04_TemporaryConstrains(pOutBuf, nOutBufLen, offset, &pSPRef->temporaryConstrains[nIdx]);
                nIdx++;
            } while ((nResult == TRUE) && (nIdx < pSPRef->N_ITER_tempConstraints));
        }
    }
    return nResult;
}



/**
* Encode ATO Header
* @param pOutBuf
* @param nOutBufLen
* @param offset
* @param pHeader
* @return
*/

BOOL Encode_ATOHeader(ATO_Header* pHeader, UINT32* offset, UINT8* pOutBuf, UINT32 nOutBufLen)
{
    BOOL nResult = TRUE;
   

    /* NID_PACKET_ATO */
    nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_NID_PACKET_ATO, pHeader->NID_PACKET_ATO);
#   
    /* L_PACKET_ATO */
    if (nResult == TRUE)
    {
        * offset += LEN_NID_PACKET_ATO;
        nResult = Utility::Encode_uint16(pOutBuf, nOutBufLen, *offset, LEN_L_PACKET_ATO, pHeader->L_PACKET_ATO);
    }

        /**  NID_OPERATIONAL */
        if (nResult == TRUE)
        {
            * offset += LEN_L_PACKET_ATO;

            nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_NID_OPERATIONAL, pHeader->NID_OPERATIONAL);
        }
        /**  NID_ENGINE */
        if (nResult == TRUE)
        {
            *offset += LEN_NID_OPERATIONAL;
            nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_NID_ENGINE, pHeader->NID_ENGINE);
        }
        /**  T_Timestamp_Date */
        if (nResult == TRUE)
        {
            *offset += LEN_NID_ENGINE;
            nResult = Utility::Encode_uint16(pOutBuf, nOutBufLen, *offset, LEN_T_TIMESTAMP_DATE, pHeader->T_Timestamp_Date);
        }
        /**  T_Timestamp_Seconds */
        if (nResult == TRUE)
        {
            *offset += LEN_T_TIMESTAMP_DATE;
            nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_T_TIMESTAMP_SECONDS, pHeader->T_Timestamp_Seconds);
        }
        /**  N_Packet_Counter */
        if (nResult == TRUE)
        {
            *offset += LEN_T_TIMESTAMP_SECONDS;
            nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_N_PACKET_COUNTER, pHeader->N_Packet_Counter);
        }
        if (nResult == TRUE)
        {
            *offset += LEN_N_PACKET_COUNTER;
        }
    
    return nResult;
}

/**
 * ATO-OB로 전송할 Handshake Ack Packet 인코딩
 * @param pOutMsg
 * @param pOutBuf
 * @param nOutBufLen
 * @return
 */

BOOL EncodeMessage_ATO_Packet01_HsAck(TS_OB_Packet01_HSAckStruct* pOutMsg, UINT32* offset, UINT8* pOutBuf, UINT32 nOutBufLen)
{

    BOOL nResult = TRUE;
   
   
    
        /** NID_C **/
        nResult = Utility::Encode_uint16(pOutBuf, nOutBufLen, *offset, LEN_NID_C, pOutMsg->NID_C);

        /** NID_ATOTS **/
        if (nResult == TRUE)
        {
            *offset += LEN_NID_C;
            nResult = Utility::Encode_uint16(pOutBuf, nOutBufLen, *offset, LEN_NID_ATOTS, pOutMsg->NID_ATOTS);
        }

        /** M_ATO_Version **/
        if (nResult == TRUE)
        {
            *offset += LEN_NID_ATOTS;
            nResult = Utility::Encode_uint16(pOutBuf, nOutBufLen, *offset, LEN_M_ATO_VERSION, pOutMsg->M_ATO_Version);
        }

        /** T_Timeout_ATO_TS_Response **/
        if (nResult == TRUE)
        {
            *offset += LEN_M_ATO_VERSION;
            nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_T_TIMEOUT_ATOTS_RESPONSE, pOutMsg->T_Timeout_ATOTOS_Response);
        }

        /** T_Reporting_Time **/
        if (nResult == TRUE)
        {
            *offset += LEN_T_TIMEOUT_ATOTS_RESPONSE;
            nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_T_REPORTING_TIME, pOutMsg->T_Reporting_Time);
        }
        if (nResult == TRUE)
        {
            *offset += LEN_T_REPORTING_TIME;
        }


        return nResult;

  

}

BOOL EncodeMessage_ATO_Packet02_HsRej(TS_OB_Packet02_HSRejStruct* pOutMsg, UINT32* offset, UINT8* pOutBuf, UINT32 nOutBufLen)
{
    BOOL nResult = TRUE;
    

    
    if (pOutMsg == nullptr)return FALSE;

    /** Q_Reject_Reason **/
    nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_REJECT_REASON, pOutMsg->Q_Reject_Reason);

    /** NID_C **/
    if (nResult == TRUE)
    {
        *offset += LEN_Q_REJECT_REASON;
        nResult = Utility::Encode_uint16(pOutBuf, nOutBufLen, *offset, LEN_NID_C, pOutMsg->NID_C);
    }

    /** NID_ATOTS **/
    if (nResult == TRUE)
    {
        *offset += LEN_NID_C;
        nResult = Utility::Encode_uint16(pOutBuf, nOutBufLen, *offset, LEN_NID_ATOTS, pOutMsg->NID_ATOTS);
    }
    if (nResult == TRUE)
    {
        *offset += LEN_NID_ATOTS;
    }

    return nResult;
}

BOOL EncodeMessage_ATO_Packet04_Jp(TS_OB_Packet04_JPStruct* pOutMsg, UINT32* offset, UINT8* pOutBuf, UINT32 nOutBufLen)
{
    BOOL nResult = TRUE;
    
    /**  Q_JP_Status */
    /** Journey Profile Details */
    nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_JP_STATUS, pOutMsg->Q_JP_Status);

    if ((pOutMsg->Q_JP_Status == Q_JP_Status_Valid)
        || (pOutMsg->Q_JP_Status == Q_JP_Status_Update)
        || (pOutMsg->Q_JP_Status == Q_JP_Status_Overwrite))
    {
        if (nResult == TRUE)
        {
            *offset += LEN_Q_JP_STATUS;
            nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_N_ITER_SP, pOutMsg->N_ITER_SP);
        }
        if ((nResult == TRUE) && (pOutMsg->N_ITER_SP > 0))
        {
            *offset += LEN_N_ITER_SP;
            UINT8 nIdx = (UINT8)0;
            do
            {
                /** Segment Profile Reference */
                nResult = Encode_Packet04_SegmentProfileReference(pOutBuf, nOutBufLen, offset, &pOutMsg->segmentProfileReference[nIdx]);
                nIdx++;
            } while ((nResult == TRUE) && (nIdx < pOutMsg->N_ITER_SP));
        }
    }

    return nResult;

}

BOOL Encode_Packet07_SegmentStatusValid(UINT8* pOutBuf, const UINT32 nOutBufLen, UINT32* offset, SPStatusValid* pSpValid)
{
    BOOL nResult = TRUE;

    /* M_SP_Version */
    nResult = Utility::Encode_uint16(pOutBuf, nOutBufLen, *offset, LEN_M_SP_VERSION, pSpValid->M_SP_Version);
    /* L_SP */
    if (nResult == TRUE)
    {
        *offset += LEN_M_SP_VERSION;
        nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_L_SP, pSpValid->L_SP);
    }
    /* D_EOA_Offset */
    if (nResult == TRUE)
    {
        *offset += LEN_L_SP;
        nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_D_EOA_OFFSET, pSpValid->D_EOA_Offset);
    }
    /* Q_UTC_Offset */
    if (nResult == TRUE)
    {
        *offset += LEN_D_EOA_OFFSET;
        nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_UTC_OFFSET, pSpValid->Q_UTC_Offset);
    }
    /* M_SP_Altitude */
    if (nResult == TRUE)
    {
        *offset += LEN_Q_UTC_OFFSET;
        nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_M_SP_ATTITUDE, pSpValid->M_SP_Altitude);
    }
    /* Q_ATOTS_Contact_Info_Dir */
    if (nResult == TRUE)
    {
        *offset += LEN_M_SP_ATTITUDE;
        nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_ATOTS_CONTACT_INFO_DIR, pSpValid->Q_ATOTS_Contact_Info_Dir);
    }

    if (nResult == TRUE)
    {
        *offset += LEN_Q_ATOTS_CONTACT_INFO_DIR;
    }
    return nResult;
}

BOOL Encode_Packet07_ATOTSContactInfo(UINT8* pOutBuf, const UINT32 nOutBufLen, UINT32* offset, ATOTSContactInfo* pATOTSContactInfo)
{
    BOOL nResult = TRUE;
    /* NID_C */
    nResult = Utility::Encode_uint16(pOutBuf, nOutBufLen, *offset, LEN_NID_C, pATOTSContactInfo->NID_C_1);
    /* NID_ATOTS */
    if (nResult == TRUE)
    {
        *offset += LEN_NID_C;
        nResult = Utility::Encode_uint16(pOutBuf, nOutBufLen, *offset, LEN_NID_ATOTS, pATOTSContactInfo->NID_ATOTS);
    }
    /* NID_C */
    if (nResult == TRUE)
    {
        *offset += LEN_NID_ATOTS;
        nResult = Utility::Encode_uint16(pOutBuf, nOutBufLen, *offset, LEN_NID_C, pATOTSContactInfo->NID_C_2);
    }
    /* NID_SP */
    if (nResult == TRUE)
    {
        *offset += LEN_NID_C;
        nResult = Utility::Encode_uint16(pOutBuf, nOutBufLen, *offset, LEN_NID_SP, pATOTSContactInfo->NID_SP_2);
    }

    if (nResult == TRUE)
    {
        *offset += LEN_NID_SP;
    }
    return nResult;
}

BOOL Encode_Packet07_SpecificSSP(UINT8* pOutBuf, const UINT32 nOutBufLen, UINT32* offset, SpecificSSP* pSSP)
{
    BOOL nResult = TRUE;

    /* Q_DIFF */
    nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_DIFF, pSSP->Q_DIFF);
    if (nResult == TRUE)
    {
        *offset += LEN_Q_DIFF;
        if (pSSP->Q_DIFF == Q_DIFF_Cant_Deficiency_specific_category)
        {
            /* NC_CDDIFF */
            nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_NC_CDDIFF, pSSP->NC_CDDIFF);
            if (nResult == TRUE)
            {
                *offset += LEN_NC_CDDIFF;
            }
        }
        else if ((pSSP->Q_DIFF == Q_DIFF_Other_specific_category_replaces_the_Cant_Deficiency_SSP)
            || (pSSP->Q_DIFF == Q_DIFF_Other_specific_category_does_not_replace_the_Cant_Deficiency_SSP))
        {
            /* NC_DIFF */
            if (nResult == TRUE)
            {
                nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_NC_DIFF, pSSP->NC_DIFF);
            }
            /* V_DIFF */
            if (nResult == TRUE)
            {
                *offset += LEN_NC_DIFF;
                nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_V_DIFF, pSSP->V_DIFF);
            }
            if (nResult == TRUE)
            {
                *offset += LEN_V_DIFF;
            }
        }
    }
    return nResult;
}

BOOL Encode_Packet07_SpecificSSPChange(UINT8* pOutBuf, const UINT32 nOutBufLen, UINT32* offset, SpecificSspChange* pSSSPChange)
{
    BOOL nResult = TRUE;
   

    /* Q_DIFF */
    nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_DIFF, pSSSPChange->Q_DIFF);
    if (nResult == TRUE)
    {
        *offset += LEN_Q_DIFF;
        if (pSSSPChange->Q_DIFF == Q_DIFF_Cant_Deficiency_specific_category)
        {
            /* NC_CDDIFF */
            nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_NC_CDDIFF, pSSSPChange->NC_CDDIFF);
            if (nResult == TRUE)
            {
                *offset += LEN_NC_CDDIFF;
            }
        }
        else if ((pSSSPChange->Q_DIFF == Q_DIFF_Other_specific_category_replaces_the_Cant_Deficiency_SSP)
            || (pSSSPChange->Q_DIFF == Q_DIFF_Other_specific_category_does_not_replace_the_Cant_Deficiency_SSP))
        {
            /* NC_DIFF */
            if (nResult == TRUE)
            {
                nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_NC_DIFF, pSSSPChange->NC_DIFF);
            }
            /* V_DIFF */
            if (nResult == TRUE)
            {
                *offset += LEN_NC_DIFF;
                nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_V_DIFF, pSSSPChange->V_DIFF);
            }
            if (nResult == TRUE)
            {
                *offset += LEN_V_DIFF;
            }
        }
    }
    return nResult;
}

BOOL Encode_Packet07_StaticSPChange(UINT8* pOutBuf, const UINT32 nOutBufLen, UINT32* offset, StaticSPChange* pStaticSP)
{
    BOOL nResult = TRUE;

    /* D_Location */
    nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_D_LOCATION, pStaticSP->D_Location);
    /* V_STATIC */
    if (nResult == TRUE)
    {
        *offset += LEN_D_LOCATION;
        nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_V_STATIC, pStaticSP->V_STATIC);
    }
    /* Q_FRONT */
    if (nResult == TRUE)
    {
        *offset += LEN_V_STATIC;
        nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_FRONT, pStaticSP->Q_FRONT);
    }
    /* N_ITER */
    if (nResult == TRUE)
    {
        *offset += LEN_Q_FRONT;
        nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_N_ITER, pStaticSP->N_ITER);
    }

    if (nResult == TRUE)
    {
        *offset += LEN_N_ITER;
        if (pStaticSP->N_ITER > 0)
        {
            UINT8 nIdx = (UINT8)0;
            do
            {
                nResult = Encode_Packet07_SpecificSSPChange(pOutBuf, nOutBufLen, offset, &pStaticSP->specificSspChange[nIdx]);
                nIdx++;
            } while ((nResult == TRUE) && (nIdx < pStaticSP->N_ITER));
        }
    }
    return nResult;
}

BOOL Encode_Packet07_GradientChange(UINT8* pOutBuf, const UINT32 nOutBufLen, UINT32* offset, GradientChange* pGradient)
{
    BOOL nResult = TRUE;

    /* D_Location */   
    nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_D_LOCATION, pGradient->D_Location);
    /* G_New_Gradient */
    if (nResult == TRUE)
    {
        *offset += LEN_D_LOCATION;
        nResult = Utility::Encode_uint16(pOutBuf, nOutBufLen, *offset, LEN_G_NEW_GRADIENT, pGradient->G_New_Gradient);
    }
    /* Q_GDIR */
    if (nResult == TRUE)
    {
        *offset += LEN_G_NEW_GRADIENT;
        nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_GDIR, pGradient->Q_GDIR);
    }
    if (nResult == TRUE)
    {
        *offset += LEN_Q_GDIR;
    }
    return nResult;
}

BOOL Encode_Packet07_CurveChange(UINT8* pOutBuf, const UINT32 nOutBufLen, UINT32* offset, CurvesChange* pCurve)
{
    BOOL nResult = TRUE;

    /* D_Location */
    nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_D_LOCATION, pCurve->D_Location);
    /* Q_Radius_Category */
    if (nResult == TRUE)
    {
        *offset += LEN_D_LOCATION;
        nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_RADIUS_CATEGORY, pCurve->Q_Radius_Category);
    }
    if (nResult == TRUE)
    {
        *offset += LEN_Q_RADIUS_CATEGORY;
    }
    return nResult;
}

BOOL Encode_Packet07_PWVoltageChange(UINT8* pOutBuf, const UINT32 nOutBufLen, UINT32* offset, PowerVoltageChange* pPWVoltage)
{
    BOOL nResult = TRUE;

    /* D_Location */
    nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_D_LOCATION, pPWVoltage->D_Location);
    /* M_VOLTAGE */
    if (nResult == TRUE)
    {
        *offset += LEN_D_LOCATION;
        nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_M_VOLTAGE, pPWVoltage->M_VOLTAGE);
    }
    /* NIC_CTRACTION */
    if (nResult == TRUE)
    {
        *offset += LEN_M_VOLTAGE;
        if (pPWVoltage->M_VOLTAGE != M_VOLTAGE_Line_not_fitted_with_any_traction_system)
        {
            nResult = Utility::Encode_uint16(pOutBuf, nOutBufLen, *offset, LEN_NID_CTRACTION, pPWVoltage->NID_CTRACTION);
            if (nResult == TRUE)
            {
                *offset += LEN_NID_CTRACTION;
            }
        }
    }

    return nResult;
}

BOOL Encode_Packet07_CurrentLimitChange(UINT8* pOutBuf, const UINT32 nOutBufLen, UINT32* offset, CurrentLimitationChange* pCurrentChange)
{
    BOOL nResult = TRUE;

    /* D_Location */
    nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_D_LOCATION, pCurrentChange->D_Location);
    /* LEN_M_CURRENT */
    if (nResult == TRUE)
    {
        *offset += LEN_D_LOCATION;
        nResult = Utility::Encode_uint16(pOutBuf, nOutBufLen, *offset, LEN_M_CURRENT, pCurrentChange->M_CURRENT);
    }
    /* NIC_CTRACTION */
    if (nResult == TRUE)
    {
        *offset += LEN_M_CURRENT;

    }
    return nResult;
}

BOOL Encode_Packet07_Balise(UINT8* pOutBuf, const UINT32 nOutBufLen, UINT32* offset, Balise* pBalise)
{
    BOOL nResult = TRUE;
    /* N_PIG */
    nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_N_PIG, pBalise->N_PIG);
    /* LEN_D_LOCATION */
    if (nResult == TRUE)
    {
        *offset += LEN_N_PIG;
        nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_D_LOCATION, pBalise->D_Location);
        if (nResult == TRUE)
        {
            *offset += LEN_D_LOCATION;
        }
    }
    return nResult;
}

BOOL Encode_Packet07_BaliseGroup(UINT8* pOutBuf, const UINT32 nOutBufLen, UINT32* offset, BaliseGroup* pBaliseGroup)
{
    BOOL nResult = TRUE;

    /* Q_NEWNID_C */
    nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_NEWNID_C, pBaliseGroup->Q_NEWNID_C);

    /* NID_C */
    if (nResult == TRUE)
    {
        *offset += LEN_Q_NEWNID_C;
        if (pBaliseGroup->Q_NEWNID_C == Q_NEWNID_C_NID_C_to_be_used_follows)
        {
            nResult = Utility::Encode_uint16(pOutBuf, nOutBufLen, *offset, LEN_NID_C, pBaliseGroup->NID_C);
            if (nResult == TRUE)
            {
                *offset += LEN_NID_C;

            }
        }
    }
    /* NID_BG */
    if (nResult == TRUE)
    {
        nResult = Utility::Encode_uint16(pOutBuf, nOutBufLen, *offset, LEN_NID_BG, pBaliseGroup->NID_BG);
    }
    /* N_ITER_BG */
    if (nResult == TRUE)
    {
        *offset += LEN_NID_BG;
        nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_N_ITER_BG, pBaliseGroup->N_ITER_BG);
    }
    if (nResult == TRUE)
    {
        *offset += LEN_N_ITER_BG;
        if (pBaliseGroup->N_ITER_BG > 0)
        {
            UINT8 nIdx = (UINT8)0;
            do
            {
                nResult = Encode_Packet07_Balise(pOutBuf, nOutBufLen, offset, &pBaliseGroup->balises[nIdx]);
                nIdx++;
            } while ((nResult == TRUE) && (nIdx < pBaliseGroup->N_ITER_BG));
        }
    }
    return nResult;
}

BOOL Encode_Packet07_TimingPoints(UINT8* pOutBuf, const UINT32 nOutBufLen, UINT32* offset, TimingPoints* pTP)
{
    BOOL nResult = TRUE;

    /* NID_TP */
    nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_NID_TP, pTP->NID_TP);
    /* D_Location */
    if (nResult == TRUE)
    {
        *offset += LEN_NID_TP;
        nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_D_LOCATION, pTP->D_Location);
    }
    /* Q_Stop_Location_Tolerance */
    if (nResult == TRUE)
    {
        *offset += LEN_D_LOCATION;
        nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_STOP_LOCATION_TOLERANCE, pTP->Q_Stop_Location_Tolerance);
    }
    /* Q_STP_Reached */
    if (nResult == TRUE)
    {
        *offset += LEN_Q_STOP_LOCATION_TOLERANCE;
        nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_STP_REACHED, pTP->Q_STP_Reached);
    }
    /* L_TEXT */
    if (nResult == TRUE)
    {
        *offset += LEN_Q_STP_REACHED;
        nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_L_TEXT, pTP->L_TEXT);
    }
    if (nResult == TRUE)
    {
        *offset += LEN_L_TEXT;
        if (pTP->L_TEXT > 0)
        {
            UINT8 nIdx = (UINT8)0;
            do
            {
                nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_X_TEXT, pTP->X_TEXT[nIdx]);
                *offset += LEN_X_TEXT;
                nIdx++;               
            } while ((nResult == TRUE) && (nIdx < pTP->L_TEXT));
        }
    }
    return nResult;
}

BOOL Encode_Packet07_PlatformArea(UINT8* pOutBuf, const UINT32 nOutBufLen, UINT32* offset, PlatformArea* pPlatform)
{
    BOOL nResult = TRUE;

    /* Q_Range*/
    nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_RANGE, pPlatform->Q_Range);
    /* D_Start_Location */
    if (nResult == TRUE)
    {
        *offset += LEN_Q_RANGE;
        if ((pPlatform->Q_Range == Q_Range_Starts) || (pPlatform->Q_Range == Q_Range_StartsEnds))
        {
            nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_D_START_LOCATION, pPlatform->D_Start_Location);
            if (nResult == TRUE)
            {
                *offset += LEN_D_START_LOCATION;
            }
        }
    }
    /* D_End_Location */
    if (nResult == TRUE)
    {
        if ((pPlatform->Q_Range == Q_Range_Ends) || (pPlatform->Q_Range == Q_Range_StartsEnds))
        {
            nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_D_END_LOCATION, pPlatform->D_End_Location);
            if (nResult == TRUE)
            {
                *offset += LEN_D_END_LOCATION;
            }
        }
    }

    return nResult;
}

BOOL Encode_Packet07_Tunnel(UINT8* pOutBuf, const UINT32 nOutBufLen, UINT32* offset, Tunnel* pTunnel)
{
    INT32 nResult = TRUE;

    /* Q_Range*/
    nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_RANGE, pTunnel->Q_Range);
    /* Q_Tunnel_Category */
    if (nResult == TRUE)
    {
        *offset += LEN_Q_RANGE;
        nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_TUNNEL_CATEGORY, pTunnel->Q_Tunnel_Category);
    }
    /* D_Start_Location */
    if (nResult == TRUE)
    {
        *offset += LEN_Q_TUNNEL_CATEGORY;
        if ((pTunnel->Q_Range == Q_Range_Starts) || (pTunnel->Q_Range == Q_Range_StartsEnds))
        {
            nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_D_START_LOCATION, pTunnel->D_Start_Location);
            if (nResult == TRUE)
            {
                *offset += LEN_D_START_LOCATION;
            }
        }
    }
    /* D_End_Location */
    if (nResult == TRUE)
    {
        if ((pTunnel->Q_Range == Q_Range_Ends) || (pTunnel->Q_Range == Q_Range_StartsEnds))
        {
            nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_D_END_LOCATION, pTunnel->D_End_Location);
            if (nResult == TRUE)
            {
                *offset += LEN_D_END_LOCATION;
            }
        }
    }

    return nResult;
}

BOOL Encode_Packet07_AxleLoadSP(UINT8* pOutBuf, const UINT32 nOutBufLen, UINT32* offset, AxleLoadSpeedProfile* pAxleLoadSP)
{
    BOOL nResult = TRUE;

    /* Q_Range*/
    nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_RANGE, pAxleLoadSP->Q_Range);
    /* M_AXLELOADCAT */
    if (nResult == TRUE)
    {
        *offset += LEN_Q_RANGE;
        nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_M_AXLELOADCAT, pAxleLoadSP->M_AXLELOADCAT);
    }
    /* V_New_Speed_Level */
    if (nResult == TRUE)
    {
        *offset += LEN_M_AXLELOADCAT;
        nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_V_SPEED_LEVEL, pAxleLoadSP->V_New_Speed_Level);
    }
    /* Q_FRONT */
    if (nResult == TRUE)
    {
        *offset += LEN_V_SPEED_LEVEL;
        nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_FRONT, pAxleLoadSP->Q_FRONT);
    }
    /* D_Start_Location */
    if (nResult == TRUE)
    {
        *offset += LEN_Q_FRONT;
        if ((pAxleLoadSP->Q_Range == Q_Range_Starts) || (pAxleLoadSP->Q_Range == Q_Range_StartsEnds))
        {
            nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_D_START_LOCATION, pAxleLoadSP->D_Start_Location);
            if (nResult == TRUE)
            {
                *offset += LEN_D_START_LOCATION;
            }
        }
    }
    /* D_End_Location */
    if (nResult == TRUE)
    {
        if ((pAxleLoadSP->Q_Range == Q_Range_Ends) || (pAxleLoadSP->Q_Range == Q_Range_StartsEnds))
        {
            nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_D_END_LOCATION, pAxleLoadSP->D_End_Location);
            if (nResult == TRUE)
            {
                *offset += LEN_D_END_LOCATION;
            }
        }
    }
    return nResult;
}

BOOL Encode_Packet07_UnprotectedLevelCrossingStop(UINT8* pOutBuf, const UINT32 nOutBufLen, UINT32* offset, UnprotectedLevelCrossingStop* pUnprotectedCRST)
{
    BOOL nResult = TRUE;
    /* D_UnprotectedLx_Stop_Nominal */
    nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_D_UNPROTECTEDLX_STOP_NOMINAL, pUnprotectedCRST->D_UnprotectedLx_Stop_Nominal);

    /* D_UnprotectedLx_Stop_Reverse */
    if (nResult == TRUE)
    {
        *offset += LEN_D_UNPROTECTEDLX_STOP_NOMINAL;
        nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_D_UNPROTECTEDLX_STOP_REVERSE, pUnprotectedCRST->D_UnprotectedLx_Stop_Reverse);
    }
    if (nResult == TRUE)
    {
        *offset += LEN_D_UNPROTECTEDLX_STOP_REVERSE;
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
BOOL Encode_Packet07_PermittedBrakingDistance(UINT8* pOutBuf, const UINT32 nOutBufLen, UINT32* offset, PermittedBrakingDistance* pPBDistance)
{
    BOOL nResult = TRUE;

    /* Q_Range*/
    nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_RANGE, pPBDistance->Q_Range);

    /* D_Permitted_Braking_Distance */
    if (nResult == TRUE)
    {
        *offset += LEN_Q_RANGE;
        nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_D_PERMITTED_BRAKING_DISTANCE, pPBDistance->D_Permitted_Braking_Distance);
    }
    /* Q_PBD_SBEB */
    if (nResult == TRUE)
    {
        *offset += LEN_D_PERMITTED_BRAKING_DISTANCE;
        nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_PBD_SBEB, pPBDistance->Q_PBD_SBEB);
    }
    /* G_PBD */
    if (nResult == TRUE)
    {
        *offset += LEN_Q_PBD_SBEB;
        nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_G_PBD, pPBDistance->G_PBD);
    }
    /* Q_GDIR_PBD */
    if (nResult == TRUE)
    {
        *offset += LEN_G_PBD;
        nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_GDIR_PBD, pPBDistance->Q_GDIR_PBD);
    }
    /* D_Start_Location */
    if (nResult == TRUE)
    {
        *offset += LEN_Q_GDIR_PBD;
        if ((pPBDistance->Q_Range == Q_Range_Starts) || (pPBDistance->Q_Range == Q_Range_StartsEnds))
        {
            nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_D_START_LOCATION, pPBDistance->D_Start_Location);
            if (nResult == TRUE)
            {
                *offset += LEN_D_START_LOCATION;
            }
        }
    }
    /* D_End_Location */
    if (nResult == TRUE)
    {
        if ((pPBDistance->Q_Range == Q_Range_Ends) || (pPBDistance->Q_Range == Q_Range_StartsEnds))
        {
            nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_D_END_LOCATION, pPBDistance->D_End_Location);
            if (nResult == TRUE)
            {
                *offset += LEN_D_END_LOCATION;
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
BOOL Encode_Packet07_SwitchOffRegenerativeBrake(UINT8* pOutBuf, const UINT32 nOutBufLen, UINT32* offset, SwitchOffRegenerativeBrake* pSWoffRBrake)
{
    BOOL nResult = TRUE;

    /* Q_Range*/
    nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_RANGE, pSWoffRBrake->Q_Range);
    /* D_Start_Location */
    if (nResult == TRUE)
    {
        *offset += LEN_Q_RANGE;
        if ((pSWoffRBrake->Q_Range == Q_Range_Starts) || (pSWoffRBrake->Q_Range == Q_Range_StartsEnds))
        {
            nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_D_START_LOCATION, pSWoffRBrake->D_Start_Location);
            if (nResult == TRUE)
            {
                *offset += LEN_D_START_LOCATION;
            }
        }
    }
    /* D_End_Location */
    if (nResult == TRUE)
    {
        if ((pSWoffRBrake->Q_Range == Q_Range_Ends) || (pSWoffRBrake->Q_Range == Q_Range_StartsEnds))
        {
            nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_D_END_LOCATION, pSWoffRBrake->D_End_Location);
            if (nResult == TRUE)
            {
                *offset += LEN_D_END_LOCATION;
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
BOOL Encode_Packet07_SwitchOffEddyCurrentBrake(UINT8* pOutBuf, const UINT32 nOutBufLen, UINT32* offset, SwitchOffEddyCurrentBrake* pSWoffEDBrake)
{
    BOOL nResult = TRUE;
    /* Q_Range*/
    nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_RANGE, pSWoffEDBrake->Q_Range);
    /* D_Start_Location */
    if (nResult == TRUE)
    {
        *offset += LEN_Q_RANGE;
        if ((pSWoffEDBrake->Q_Range == Q_Range_Starts) || (pSWoffEDBrake->Q_Range == Q_Range_StartsEnds))
        {
            nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_D_START_LOCATION, pSWoffEDBrake->D_Start_Location);
            if (nResult == TRUE)
            {
                *offset += LEN_D_START_LOCATION;
            }
        }
    }
    /* D_End_Location */
    if (nResult == TRUE)
    {
        if ((pSWoffEDBrake->Q_Range == Q_Range_Ends) || (pSWoffEDBrake->Q_Range == Q_Range_StartsEnds))
        {
            nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_D_END_LOCATION, pSWoffEDBrake->D_End_Location);
            if (nResult == TRUE)
            {
                *offset += LEN_D_END_LOCATION;
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
BOOL Encode_Packet07_SwitchOffEddyCurrentEmergencyBrake(UINT8* pOutBuf, const UINT32 nOutBufLen, UINT32* offset, SwitchOffEddyEmergencyBrake* pSWoffEDEBrake)
{
    BOOL nResult = TRUE;

    /* Q_Range*/
    nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_RANGE, pSWoffEDEBrake->Q_Range);
    /* D_Start_Location */
    if (nResult == TRUE)
    {
        *offset += LEN_Q_RANGE;
        if ((pSWoffEDEBrake->Q_Range == Q_Range_Starts) || (pSWoffEDEBrake->Q_Range == Q_Range_StartsEnds))
        {
            nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_D_START_LOCATION, pSWoffEDEBrake->D_Start_Location);
            if (nResult == TRUE)
            {
                *offset += LEN_D_START_LOCATION;
            }
        }
    }
    /* D_End_Location */
    if (nResult == TRUE)
    {
        if ((pSWoffEDEBrake->Q_Range == Q_Range_Ends) || (pSWoffEDEBrake->Q_Range == Q_Range_StartsEnds))
        {
            nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_D_END_LOCATION, pSWoffEDEBrake->D_End_Location);
            if (nResult == TRUE)
            {
                *offset += LEN_D_END_LOCATION;
            }
        }
    }
    return nResult;
}

BOOL Encode_Packet07_SwitchOfMagmaticShoeBrake(UINT8* pOutBuf, const UINT32 nOutBufLen, UINT32* offset, SwitchOffMagmaticShoeBrake* pSWoffMgShoeBrake)
{
    BOOL nResult = TRUE;

    /* Q_Range*/
    nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_RANGE, pSWoffMgShoeBrake->Q_Range);
    /* D_Start_Location */
    if (nResult == TRUE)
    {
        *offset += LEN_Q_RANGE;
        if ((pSWoffMgShoeBrake->Q_Range == Q_Range_Starts) || (pSWoffMgShoeBrake->Q_Range == Q_Range_StartsEnds))
        {
            nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_D_START_LOCATION, pSWoffMgShoeBrake->D_Start_Location);
            if (nResult == TRUE)
            {
                *offset += LEN_D_START_LOCATION;                
            }
        }
    }
    /* D_End_Location */
    if (nResult == TRUE)
    {
        if ((pSWoffMgShoeBrake->Q_Range == Q_Range_Ends) || (pSWoffMgShoeBrake->Q_Range == Q_Range_StartsEnds))
        {
            nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_D_END_LOCATION, pSWoffMgShoeBrake->D_End_Location);
            if (nResult == TRUE)
            {
                *offset += LEN_D_END_LOCATION;
            }
        }
    }
    return nResult;
}

BOOL Encode_Packet07_AreaWithDynamicBrakeForceInhibition(UINT8* pOutBuf, const UINT32 nOutBufLen, UINT32* offset, AreaWithDynamicBrakeForceInhibition* pBrakeForceInhibition)
{
    BOOL nResult = TRUE;

    /* Q_Range*/
    nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_RANGE, pBrakeForceInhibition->Q_Range);
    /* D_Start_Location */
    if (nResult == TRUE)
    {
        *offset += LEN_Q_RANGE;
        if ((pBrakeForceInhibition->Q_Range == Q_Range_Starts) || (pBrakeForceInhibition->Q_Range == Q_Range_StartsEnds))
        {
            nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_D_START_LOCATION, pBrakeForceInhibition->D_Start_Location);
            if (nResult == TRUE)
            {
                *offset += LEN_D_START_LOCATION;
            }
        }
    }
    /* D_End_Location */
    if (nResult == TRUE)
    {
        if ((pBrakeForceInhibition->Q_Range == Q_Range_Ends) || (pBrakeForceInhibition->Q_Range == Q_Range_StartsEnds))
        {
            nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_D_END_LOCATION, pBrakeForceInhibition->D_End_Location);
            if (nResult == TRUE)
            {
                *offset += LEN_D_END_LOCATION;
            }
        }
    }
    return nResult;
}

BOOL Encode_Packet07_AreaWithLimitedDynamicBrakeForce(UINT8* pOutBuf, const UINT32 nOutBufLen, UINT32* offset, AreaWithLimitedDynamicBrakeForce* pBrakeForce)
{
    BOOL nResult = TRUE;

    /* Q_Range*/
    nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_RANGE, pBrakeForce->Q_Range);

    /* M_Dynamic_Braking_Force_Limit */
    if (nResult == TRUE)
    {
        *offset += LEN_Q_RANGE;
        nResult = Utility::Encode_uint16(pOutBuf, nOutBufLen, *offset, LEN_M_DYNAMIC_BRAKING_FORCE_LIMIT, pBrakeForce->M_Dynamic_Braking_Force_Limit);
    }
    /* D_Start_Location */
    if (nResult == TRUE)
    {
        *offset += LEN_M_DYNAMIC_BRAKING_FORCE_LIMIT;
        if ((pBrakeForce->Q_Range == Q_Range_Starts) || (pBrakeForce->Q_Range == Q_Range_StartsEnds))
        {
            nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_D_START_LOCATION, pBrakeForce->D_Start_Location);
            if (nResult == TRUE)
            {
                *offset += LEN_D_START_LOCATION;
            }
        }
    }
    /* D_End_Location */
    if (nResult == TRUE)
    {
        if ((pBrakeForce->Q_Range == Q_Range_Ends) || (pBrakeForce->Q_Range == Q_Range_StartsEnds))
        {
            nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_D_END_LOCATION, pBrakeForce->D_End_Location);
            if (nResult == TRUE)
            {
                *offset += LEN_D_END_LOCATION;
            }
        }
    }
    return nResult;

}



BOOL Encode_Packet07_SegmentProfileStatus(SegmentProfileStatus* pOutMsg, UINT32* offset, UINT8* pOutBuf, UINT32 nOutBufLen)
{
    BOOL nResult = TRUE;
    UINT8 nIdx = 0;

    /* NID_C */
    nResult = Utility::Encode_uint16(pOutBuf, nOutBufLen, *offset, LEN_NID_C, pOutMsg->NID_C);

    /* NID_SP */
    if (nResult == TRUE)
    {
        *offset += LEN_NID_C;
        nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_NID_SP, pOutMsg->NID_SP_1);
    }
    /* Q_SP_Status */
    if (nResult == TRUE)
    {
        *offset += LEN_NID_SP;
        nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_SP_STATUS, pOutMsg->Q_SP_Status);
    }
    /* SegmentStatusValid */
    if (nResult == TRUE)
    {
        *offset += LEN_Q_SP_STATUS;

        if (pOutMsg->Q_SP_Status == Q_SP_Status_Valid)
        {
            nResult = Encode_Packet07_SegmentStatusValid(pOutBuf, nOutBufLen, offset, &pOutMsg->spStatusValid);


            /* Q_ATOTS_Contact_Info */
            if (nResult == TRUE)
            {
                /** Q_ATOTS_Contact_Info_Dir_Normal or Q_ATOTS_Contact_Info_Dir_Reverse */
                if ((pOutMsg->spStatusValid.Q_ATOTS_Contact_Info_Dir == Q_ATOTS_Contact_Info_Dir_ATOTS_contract_info_for_normal_direction_follows)
                    || (pOutMsg->spStatusValid.Q_ATOTS_Contact_Info_Dir == Q_ATOTS_Contact_Info_Dir_ATOTS_contract_info_for_reverse_direction_follows))
                {
                    nResult = Encode_Packet07_ATOTSContactInfo(pOutBuf, nOutBufLen, offset, &pOutMsg->atoTSContactInfo);
                }
            }

            /****************** Static Speed Profile Start *******************************/
            /* V_STATIC */
            if (nResult == TRUE)
            {
                nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_V_STATIC, pOutMsg->V_STATIC);
            }
            /* Q_FRONT */
            if (nResult == TRUE)
            {
                *offset += LEN_V_STATIC;
                nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_FRONT, pOutMsg->Q_FRONT);
            }
            /* N_ITER_ATO */
            if (nResult == TRUE)
            {
                *offset += LEN_Q_FRONT;
                nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_N_ITER_ATO, pOutMsg->N_ITER_ATO_1);
            }
            /* Specific SSP */
            if (nResult == TRUE)
            {
                *offset += LEN_N_ITER_ATO;
                if (pOutMsg->N_ITER_ATO_1 > 0)
                {
                    nIdx = (UINT8)0;

                    do
                    {
                        nResult = Encode_Packet07_SpecificSSP(pOutBuf, nOutBufLen, offset, &pOutMsg->specificSSP[nIdx]);
                        nIdx++;
                    } while ((nResult == TRUE) && (nIdx < pOutMsg->N_ITER_ATO_1));
                }
            }
            /* N_ITER_ATO */
            if (nResult == TRUE)
            {
                nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_N_ITER_ATO, pOutMsg->N_ITER_ATO_2);
            }
            /* Static Speed Profile Change */
            if (nResult == TRUE)
            {
                *offset += LEN_N_ITER_ATO;
                if (pOutMsg->N_ITER_ATO_2 > 0)
                {
                    UINT8 nIdx2 = (UINT8)0;
                    do
                    {
                        nResult = Encode_Packet07_StaticSPChange(pOutBuf, nOutBufLen, offset, &pOutMsg->staticSPChange[nIdx2]);
                        nIdx2++;
                    } while ((nResult == TRUE) && (nIdx2 < pOutMsg->N_ITER_ATO_2));
                }
            }
            /************************** Gradient Start *******************************/
            /* G_New_Gradient */
            if (nResult == TRUE)
            {
                nResult = Utility::Encode_uint16(pOutBuf, nOutBufLen, *offset, LEN_G_NEW_GRADIENT, pOutMsg->G_New_Gradient);
            }
            /* Q_GDIR */
            if (nResult == TRUE)
            {
                *offset += LEN_G_NEW_GRADIENT;
                nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_GDIR, pOutMsg->Q_GDIR);
            }
            /* N_ITER_ATO */
            if (nResult == TRUE)
            {
                *offset += LEN_Q_GDIR;
                nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_N_ITER_ATO, pOutMsg->N_ITER_ATO_3);
            }
            /* Gradient Change */
            if (nResult == TRUE)
            {
                *offset += LEN_N_ITER_ATO;
                if (pOutMsg->N_ITER_ATO_3 > 0)
                {
                    UINT8 nIdx3 = (UINT8)0;
                    do
                    {
                        nResult = Encode_Packet07_GradientChange(pOutBuf, nOutBufLen, offset, &pOutMsg->gradientChange[nIdx3]);
                        nIdx3++;
                    } while ((nResult == TRUE) && (nIdx3 < pOutMsg->N_ITER_ATO_3));
                }
            }
            /**************************  Curve Start   *******************************/
            /* Q_Radius_Category */
            if (nResult == TRUE)
            {
                nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_Q_RADIUS_CATEGORY, pOutMsg->Q_Radius_Category);
            }
            /* N_ITER_ATO */
            if (nResult == TRUE)
            {
                *offset += LEN_Q_RADIUS_CATEGORY;
                nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_N_ITER_ATO, pOutMsg->N_ITER_ATO_4);
            }
            /* Curve Change */
            if (nResult == TRUE)
            {
                *offset += LEN_N_ITER_ATO;
                if (pOutMsg->N_ITER_ATO_4 > 0)
                {
                    UINT8 nIdx4 = (UINT8)0;
                    do
                    {
                        nResult = Encode_Packet07_CurveChange(pOutBuf, nOutBufLen, offset, &pOutMsg->curvesChange[nIdx4]);
                        nIdx4++;
                    } while ((nResult == TRUE) && (nIdx4 < pOutMsg->N_ITER_ATO_4));
                }
            }
            /************************** Power voltage Start **************************/
            /* M_VOLTAGE */
            if (nResult == TRUE)
            {
                nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_M_VOLTAGE, pOutMsg->M_VOLTAGE);
            }
            /* NID_CTRACTION */
            if (nResult == TRUE)
            {
                *offset += LEN_M_VOLTAGE;
                if (pOutMsg->M_VOLTAGE != M_VOLTAGE_Line_not_fitted_with_any_traction_system)
                {
                    nResult = Utility::Encode_uint16(pOutBuf, nOutBufLen, *offset, LEN_NID_CTRACTION, pOutMsg->NID_CTRACTION);
                    if (nResult == TRUE)
                    {
                        *offset += LEN_NID_CTRACTION;
                    }
                }
            }
            /* N_ITER_ATO */
            if (nResult == TRUE)
            {
                nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_N_ITER_ATO, pOutMsg->N_ITER_ATO_5);
            }

            *offset += LEN_N_ITER_ATO;
            /* Power Voltage Change */
            if (pOutMsg->N_ITER_ATO_5 > 0)
            {
                UINT8 nIdx5 = (UINT8)0;
                do
                {
                    nResult = Encode_Packet07_PWVoltageChange(pOutBuf, nOutBufLen, offset, &pOutMsg->powerVoltageChange[nIdx5]);
                    nIdx5++;
                } while ((nResult == TRUE) && (nIdx5 < pOutMsg->N_ITER_ATO_5));
            }

            /********************* current Limitation start *************************/
            /* M_CURRENT */
            if (nResult == TRUE)
            {
                nResult = Utility::Encode_uint16(pOutBuf, nOutBufLen, *offset, LEN_M_CURRENT, pOutMsg->M_CURRENT);
            }
            /* N_ITER_ATO */
            if (nResult == TRUE)
            {
                *offset += LEN_M_CURRENT;
                nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_N_ITER_ATO, pOutMsg->N_ITER_ATO_6);
            }
            /* Current Limitation Change */
            if (nResult == TRUE)
            {
                *offset += LEN_N_ITER_ATO;
                if (pOutMsg->N_ITER_ATO_6 > 0)
                {
                    UINT8 nIdx6 = (UINT8)0;
                    do
                    {
                        nResult = Encode_Packet07_CurrentLimitChange(pOutBuf, nOutBufLen, offset, &pOutMsg->currentLimitChange[nIdx6]);
                        nIdx6++;
                    } while ((nResult == TRUE) && (nIdx6 < pOutMsg->N_ITER_ATO_6));
                }
            }
            /*********************   Balise Group  *************************/
            /* N_ITER_ATO */
            if (nResult == TRUE)
            {
                nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_N_ITER_ATO, pOutMsg->N_ITER_ATO_7);
            }
            /*  Balise Group */
            if (nResult == TRUE)
            {
                *offset += LEN_N_ITER_ATO;
                if (pOutMsg->N_ITER_ATO_7 > 0)
                {
                    UINT8 nIdx7 = (UINT8)0;
                    do
                    {
                        nResult = Encode_Packet07_BaliseGroup(pOutBuf, nOutBufLen, offset, &pOutMsg->baliseGroup[nIdx7]);
                        nIdx7++;
                    } while ((nResult == TRUE) && (nIdx7 < pOutMsg->N_ITER_ATO_7));
                }
            }
            /*********************   TimingPoints  *************************/
            /* N_ITER_ATO */
            if (nResult == TRUE)
            {
                nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_N_ITER_ATO, pOutMsg->N_ITER_ATO_8);
            }
            /* TimingPoints */
            if (nResult == TRUE)
            {
                *offset += LEN_N_ITER_ATO;
                if (pOutMsg->N_ITER_ATO_8 > 0)
                {
                    UINT8 nIdx8 = (UINT8)0;
                    do
                    {
                        nResult = Encode_Packet07_TimingPoints(pOutBuf, nOutBufLen, offset, &pOutMsg->timingPoints[nIdx8]);
                        nIdx8++;
                    } while ((nResult == TRUE) && (nIdx8 < pOutMsg->N_ITER_ATO_8));
                }
            }
            /*********************   Platform Area  *************************/
            /* N_ITER_ATO */
            if (nResult == TRUE)
            {
                nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_N_ITER_ATO, pOutMsg->N_ITER_ATO_9);
            }
            /*  Platform Area  */
            if (nResult == TRUE)
            {
                *offset += LEN_N_ITER_ATO;
                if (pOutMsg->N_ITER_ATO_9 > 0)
                {
                    UINT8 nIdx9 = (UINT8)0;
                    do
                    {
                        nResult = Encode_Packet07_PlatformArea(pOutBuf, nOutBufLen, offset, &pOutMsg->platformArea[nIdx9]);
                        nIdx9++;
                    } while ((nResult == TRUE) && (nIdx9 < pOutMsg->N_ITER_ATO_9));
                }
            }
            /*********************  Tunnel  *************************/
            /* N_ITER_ATO */
            if (nResult == TRUE)
            {
                nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_N_ITER_ATO, pOutMsg->N_ITER_ATO_10);
            }
            /*   Tunnel  */
            if (nResult == TRUE)
            {
                *offset += LEN_N_ITER_ATO;
                if (pOutMsg->N_ITER_ATO_10 > 0)
                {
                    UINT8 nIdx10 = (UINT8)0;
                    do
                    {
                        nResult = Encode_Packet07_Tunnel(pOutBuf, nOutBufLen, offset, &pOutMsg->tunnel[nIdx10]);
                        nIdx10++;
                    } while ((nResult == TRUE) && (nIdx10 < pOutMsg->N_ITER_ATO_10));
                }
            }
            /*********************  Axle Load Speed Profile  *************************/
            /* N_ITER_ATO */
            if (nResult == TRUE)
            {
                nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_N_ITER_ATO, pOutMsg->N_ITER_ATO_11);
            }
            /*   Axle Load Speed Profile  */
            if (nResult == TRUE)
            {
                *offset += LEN_N_ITER_ATO;
                if (pOutMsg->N_ITER_ATO_11 > 0)
                {
                    UINT8 nIdx11 = (UINT8)0;
                    do
                    {
                        nResult = Encode_Packet07_AxleLoadSP(pOutBuf, nOutBufLen, offset, &pOutMsg->axleLoadSpeedProfile[nIdx11]);
                        nIdx11++;

                    } while ((nResult == TRUE) && (nIdx11 < pOutMsg->N_ITER_ATO_11));
                }
            }
            /*********************  Unprotected Level Crossing Stop  *************************/
            /* N_ITER_ATO */
            if (nResult == TRUE)
            {
                nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_N_ITER_ATO, pOutMsg->N_ITER_ATO_12);
            }
            /*  Unprotected Level Crossing Stop  */
            if (nResult == TRUE)
            {
                *offset += LEN_N_ITER_ATO;
                if (pOutMsg->N_ITER_ATO_12 > 0)
                {
                    UINT8 nIdx12 = (UINT8)0;
                    do
                    {
                        nResult = Encode_Packet07_UnprotectedLevelCrossingStop(pOutBuf, nOutBufLen, offset, &pOutMsg->unprotectedLevelCrossingStop[nIdx12]);
                        nIdx12++;

                    } while ((nResult == TRUE) && (nIdx12 < pOutMsg->N_ITER_ATO_12));
                }

            }
            /********************* Permitted Braking Distance  *************************/
            /* N_ITER_ATO */
            if (nResult == TRUE)
            {
                nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_N_ITER_ATO, pOutMsg->N_ITER_ATO_13);
            }
            /*  Permitted Braking Distance  */
            if (nResult == TRUE)
            {
                *offset += LEN_N_ITER_ATO;
                if (pOutMsg->N_ITER_ATO_13 > 0)
                {
                    UINT8 nIdx13 = (UINT8)0;
                    do
                    {
                        nResult = Encode_Packet07_PermittedBrakingDistance(pOutBuf, nOutBufLen, offset, &pOutMsg->permittedBrakingDistance[nIdx13]);
                        nIdx13++;
                    } while ((nResult == TRUE) && (nIdx13 < pOutMsg->N_ITER_ATO_13));
                }
            }
            /********************* Switch Off Regenerative Brake  *************************/
            /* N_ITER_ATO */
            if (nResult == TRUE)
            {
                nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_N_ITER_ATO, pOutMsg->N_ITER_ATO_14);
            }
            /*  Switch Off Regenerative Brake  */
            if (nResult == TRUE)
            {
                *offset += LEN_N_ITER_ATO;
                if (pOutMsg->N_ITER_ATO_14 > 0)
                {
                    UINT8 nIdx14 = (UINT8)0;
                    do
                    {
                        nResult = Encode_Packet07_SwitchOffRegenerativeBrake(pOutBuf, nOutBufLen, offset, &pOutMsg->switchOffRegenerativeBrake[nIdx14]);
                        nIdx14++;
                    } while ((nResult == TRUE) && (nIdx14 < pOutMsg->N_ITER_ATO_14));
                }

            }
            /********************* Switch Off Eddy Current Brake  *************************/
            /* N_ITER_ATO */
            if (nResult == TRUE)
            {
                nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_N_ITER_ATO, pOutMsg->N_ITER_ATO_15);
            }
            /*  Switch Off Eddy Current Brake  */
            if (nResult == TRUE)
            {
                *offset += LEN_N_ITER_ATO;
                if (pOutMsg->N_ITER_ATO_15 > 0)
                {
                    UINT8 nIdx15 = (UINT8)0;
                    do
                    {
                        nResult = Encode_Packet07_SwitchOffEddyCurrentBrake(pOutBuf, nOutBufLen, offset, &pOutMsg->switchOffEddyCurrentBrake[nIdx15]);
                        nIdx15++;
                    } while ((nResult == TRUE) && (nIdx15 < pOutMsg->N_ITER_ATO_15));
                }
            }
            /*********************Switch Off Eddy Current Emergency Brake *************************/
            /* N_ITER_ATO */
            if (nResult == TRUE)
            {
                nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_N_ITER_ATO, pOutMsg->N_ITER_ATO_16);
            }
            /*  Switch Off Eddy Current Emergency Brake   */
            if (nResult == TRUE)
            {
                *offset += LEN_N_ITER_ATO;
                if (pOutMsg->N_ITER_ATO_16 > 0)
                {
                    UINT8 nIdx16 = (UINT8)0;
                    do
                    {
                        nResult = Encode_Packet07_SwitchOffEddyCurrentEmergencyBrake(pOutBuf, nOutBufLen, offset, &pOutMsg->switchOffEddyEmergencyBrake[nIdx16]);
                        nIdx16++;
                    } while ((nResult == TRUE) && (nIdx16 < pOutMsg->N_ITER_ATO_16));
                }

            }
            /*********************  Switch Off Magmatic Shoe Brake  *************************/
            /* N_ITER_ATO */
            if (nResult == TRUE)
            {
                nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_N_ITER_ATO, pOutMsg->N_ITER_ATO_17);
            }
            /*  Switch Off Magmatic Shoe Brake  */
            if (nResult == TRUE)
            {
                *offset += LEN_N_ITER_ATO;
                if (pOutMsg->N_ITER_ATO_17 > 0)
                {
                    UINT8 nIdx17 = (UINT8)0;
                    do
                    {
                        nResult = Encode_Packet07_SwitchOfMagmaticShoeBrake(pOutBuf, nOutBufLen, offset, &pOutMsg->switchOffMagmaticShoeBrake[nIdx17]);
                        nIdx17++;
                    } while ((nResult == TRUE) && (nIdx17 < pOutMsg->N_ITER_ATO_17));
                }

            }
            /********************* Area With Dynamic Brake Force Inhibition  *************************/
            /* N_ITER_ATO */
            if (nResult == TRUE)
            {
                nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_N_ITER_ATO, pOutMsg->N_ITER_ATO_18);
            }
            /*  Area With Dynamic Brake Force Inhibition */
            if (nResult == TRUE)
            {
                *offset += LEN_N_ITER_ATO;
                if (pOutMsg->N_ITER_ATO_18 > 0)
                {
                    UINT8 nIdx18 = (UINT8)0;
                    do
                    {
                        nResult = Encode_Packet07_AreaWithDynamicBrakeForceInhibition(pOutBuf, nOutBufLen, offset, &pOutMsg->areaWithDynamicBrakeForceInhibition[nIdx18]);
                        nIdx18++;
                    } while ((nResult == TRUE) && (nIdx18 < pOutMsg->N_ITER_ATO_18));
                }
#
            }
            /********************* Area With Dynamic Brake Force Inhibition  *************************/
            /* N_ITER_ATO */
            if (nResult == TRUE)
            {
                nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_N_ITER_ATO, pOutMsg->N_ITER_ATO_19);
            }
            /*  Area With Dynamic Brake Force Inhibition  */
            if (nResult == TRUE)
            {
                *offset += LEN_N_ITER_ATO;
                if (pOutMsg->N_ITER_ATO_19 > 0)
                {
                    UINT8 nIdx19 = (UINT8)0;
                    do
                    {
                        nResult = Encode_Packet07_AreaWithLimitedDynamicBrakeForce(pOutBuf, nOutBufLen, offset, &pOutMsg->areaWithLimitedDynamicBrakeForce[nIdx19]);
                        nIdx19++;
                    } while ((nResult == TRUE) && (nIdx19 < pOutMsg->N_ITER_ATO_19));
                }

            }
        }
    }

    return nResult;

}


BOOL EncodeMessage_ATO_Packet07_Sp(TS_OB_Packet07_SPStruct* pOutMsg, UINT32* offset, UINT8* pOutBuf, UINT32 nOutBufLen)
{
    BOOL nResult = TRUE;

    /* N_ITER_ATO */
    nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_N_ITER_ATO, pOutMsg->N_ITER_ATO);

    *offset += LEN_N_ITER_ATO;
    if ((nResult == TRUE) && (pOutMsg->N_ITER_ATO > 0))
    {        
        UINT8 nIdx = (UINT8)0;
        do
        {
            /** Segment Profile Status */
            nResult = Encode_Packet07_SegmentProfileStatus(&pOutMsg->SPStatus[nIdx], offset, pOutBuf, nOutBufLen);
            nIdx++;
        } while ((nResult == TRUE) && (nIdx < pOutMsg->N_ITER_ATO));
    }

    return nResult;

}

BOOL EncodeMessage_ATO_Packet09_STRAck(TS_OB_Packet09_STRAckStruct* pOutMsg, UINT32* offset, UINT8* pOutBuf, UINT32 nOutBufLen)
{
    BOOL nResult = TRUE;

    /**  T_STR_Ref_TimeStamp_Date */
    nResult = Utility::Encode_uint16(pOutBuf, nOutBufLen, *offset, LEN_T_STR_REFERENCE_TIMESTAMP_DATE, pOutMsg->T_STR_Ref_TimeStamp_Date);

    /**  T_STR_Ref_TimeStamp_Seconds */
    if (nResult == TRUE)
    {
        *offset += LEN_T_STR_REFERENCE_TIMESTAMP_DATE;
        nResult = Utility::Encode_uint32(pOutBuf, nOutBufLen, *offset, LEN_T_STR_REFERENCE_TIMESTAMP_SECONDS, pOutMsg->T_STR_Ref_TimeStamp_Seconds);
    }
    /**  N_STR_Ref_Packet_Counter */
    if (nResult == TRUE)
    {
        *offset += LEN_T_STR_REFERENCE_TIMESTAMP_SECONDS;
        nResult = Utility::Encode_uint8(pOutBuf, nOutBufLen, *offset, LEN_N_STR_REFERENCE_PACKET_COUNTER, pOutMsg->N_STR_Ref_Packet_Counter);
    }
    if (nResult == TRUE)
    {
        *offset += LEN_N_STR_REFERENCE_PACKET_COUNTER;
    }

    return nResult;

}

BOOL EncodeMessage_ATO_Packet10_SESSTermReq(TS_OB_Packet10_SESSTermReqStruct* pOutMsg, UINT32* offset, UINT8* pOutBuf, UINT32 nOutBufLen)
{
    BOOL nResult = TRUE;
    return nResult;
}


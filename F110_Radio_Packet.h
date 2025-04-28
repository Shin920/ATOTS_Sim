#ifndef F110_RADIO_PACKET_H_
#define F110_RADIO_PACKET_H_
//
#include "define.h"


/*********************************************************************************************
 *   ATO OB - ATO-TS Message Default and defined value
 **********************************************************************************************/

#define UNDEFINED_NID_SP  0xFFFFFFFF        // Decimal - 4294967295


//#define MAX_ITER_ATO_COUNT 10
//#define MAX_ITER_COUNT 10
//#define MAX_ITER_SP_COUNT 10

#define MAX_ITER_ATO_COUNT 32
#define MAX_ITER_COUNT 32
#define MAX_ITER_SP_COUNT 255


#define MAX_L_TEXT 255
#define NID_SP_UNDEFINED 0xFFFFFFFF
#define MAX_V_Speed_Level 120
#define MAX_V_STATIC 120
#define MAX_DYNAMIC_BRAKE_FORCE_LIMITS 3000
#define UNKNOWN_DYNAMIC_BRAKE_FORCE_LIMITS 0xFFFF
 /********************************************************************************************
  *                    ATO OB - ATO-TS Message Field Length (Subset-126)
  ********************************************************************************************/

  /**
   *  ATO Header Field Variable Length
   */
#define LEN_NID_PACKET_ATO 8
#define LEN_L_PACKET_ATO 16
#define LEN_NID_OPERATIONAL 32
#define LEN_NID_ENGINE 24
#define LEN_T_TIMESTAMP_DATE 15
#define LEN_T_TIMESTAMP_SECONDS 17
#define LEN_N_PACKET_COUNTER 8
#define LEN_ALL_HEADER 120
#define LEN_KTCS2_HEADER 104

   /**
    *  ATO Packet Field Variable Length
    */
#define LEN_N_ITER_ATO 5
#define LEN_M_ATO_VERSION 16
#define LEN_NID_C 10
#define LEN_NID_ATOTS 14
#define LEN_M_ATO_VERSION 16
#define LEN_T_TIMEOUT_ATOTS_RESPONSE 8
#define LEN_T_REPORTING_TIME 8

#define LEN_Q_REJECT_REASON 2
#define LEN_NID_SP 32

#define LEN_Q_JP_STATUS 3
#define LEN_N_ITER_SP 8
#define LEN_NID_SP  32
#define LEN_M_SP_VERSION 16
#define LEN_Q_SP_DIR    1

#define LEN_N_ITER 5
#define LEN_NID_TP 32
#define LEN_T_LATEST_ARRIVAL_DATE 15
#define LEN_T_LATEST_ARRIVAL_SECONDS 17
#define LEN_T_ARRIVAL_WINDOW 10
#define LEN_Q_TP_ALIGNMENT 2
#define LEN_Q_STOP_SKIP_PASS 2
#define LEN_Q_TP_INFORMATION 2
#define LEN_Q_DAY_LIGHT_SAVING 1
#define LEN_Q_OPENING_DOOR_SIDE 2
#define LEN_Q_CENTRALISED_OPENING 1
#define LEN_Q_RELAXED_COUPLER 1
#define LEN_Q_TRAIN_HOLD 1
#define LEN_T_DEPARTURE_DATE 15
#define LEN_T_DEPARTURE_SECONDS 17
#define LEN_T_MINIMUM_DWELL_TIME 10
#define LEN_Q_AUTOMATIC_CLOSING 1
#define LEN_Q_TC_TYPE 3
#define LEN_Q_RANGE 2
#define LEN_D_TC_START_LOCATION 24
#define LEN_D_TC_END_LOCATION 24
#define LEN_Q_FRONT 1
#define LEN_V_SPEED_LEVEL 7
#define LEN_Q_ADHESION_CATEGORY 3
#define LEN_M_CURRENT 10

#define LEN_T_JP_REFERENCE_TIMESTAMP_DATE 15
#define LEN_T_JP_REFERENCE_TIMESTAMP_SECONDS 17
#define LEN_N_JP_REFERENCE_PACKET_COUNTER 8

#define LEN_Q_SP_STATUS 1
#define LNE_M_SP_VERSION 16
#define LEN_L_SP 24
#define LEN_D_EOA_OFFSET 24
#define LEN_Q_UTC_OFFSET 7
#define LEN_M_SP_ATTITUDE 20
#define LEN_Q_ATOTS_CONTACT_INFO_DIR 2

#define LEN_V_STATIC 7
#define LEN_Q_FRONT 1
#define LEN_Q_DIFF 2
#define LEN_NC_CDDIFF 4
#define LEN_NC_DIFF 4
#define LEN_V_DIFF 7
#define LEN_D_LOCATION 24
#define LEN_G_NEW_GRADIENT 10
#define LEN_Q_GDIR 1

#define LEN_Q_RADIUS_CATEGORY 5
#define LEN_M_VOLTAGE 4
#define LEN_NID_CTRACTION 10
#define LEN_M_CURRENT 10
#define LEN_Q_NEWNID_C 1
#define NID_C_DEFAULT 887
#define LEN_NID_BG 14
#define LEN_N_ITER_BG 3
#define LEN_N_PIG 3
#define LEN_NIP_TP 32
#define LEN_Q_STOP_LOCATION_TOLERANCE 5
#define LEN_Q_STP_REACHED 5
#define LEN_L_TEXT 8
#define LEN_X_TEXT 8
#define LEN_Q_RANGE 2
#define LEN_D_START_LOCATION 24
#define LEN_D_END_LOCATION 24
#define LEN_Q_TUNNEL_CATEGORY 2
#define LEN_M_AXLELOADCAT 7
#define LEN_V_NEW_SPEED_LEVEL 7

#define LEN_D_UNPROTECTEDLX_STOP_NOMINAL 24
#define LEN_D_UNPROTECTEDLX_STOP_REVERSE 24
#define LEN_D_PERMITTED_BRAKING_DISTANCE 24
#define LEN_Q_PBD_SBEB 1
#define LEN_G_PBD 8
#define LEN_Q_GDIR_PBD 1
#define LEN_M_DYNAMIC_BRAKING_FORCE_LIMIT 16

#define LEN_M_ATO_STATE 4
#define LEN_Q_STR_INDICATION 16
#define LEN_V_TRAIN_ATO 10
#define LEN_L_TRAIN 16
#define LEN_DRIVER_ID 128
#define LEN_DRIVER_ID_ARRAY 16
#define LEN_D_SENDING_POSITION 24
#define LEN_Q_PASS_STOP_DEPART 2
#define LEN_Q_ACCURATE_STOPPING 2
#define LEN_T_ARRIVAL_DATE 15
#define LEN_T_ARRIVAL_SECONDS 17

#define LEN_T_STR_REFERENCE_TIMESTAMP_DATE 15
#define LEN_T_STR_REFERENCE_TIMESTAMP_SECONDS 17
#define LEN_N_STR_REFERENCE_PACKET_COUNTER 8

#define LEN_Q_TERMINATION_REASON 3

    /********************************************************************************************
     *   enum, struct 정의
     ********************************************************************************************/

typedef enum
{
    ATO_Packet_Type_HANDSHAKE_REQUEST = 0,
    ATO_Packet_Type_HANDSHAKE_ACKNOWLEDGEMENT = 1,
    ATO_Packet_Type_HANDSHAKE_REJECT = 2,
    ATO_Packet_Type_JP_REQUEST = 3,
    ATO_Packet_Type_JP = 4,
    ATO_Packet_Type_JP_ACKNOWLEDGEMENT = 5,
    ATO_Packet_Type_SP_REQUEST = 6,
    ATO_Packet_Type_SP = 7,
    ATO_Packet_Type_STATUS_REPORT = 8,
    ATO_Packet_Type_STATUS_REPORT_ACKNOWLEDGEMENT = 9,
    ATO_Packet_Type_SESSION_TERMINATION_REQUEST = 10,
    ATO_Packet_Type_SESSION_TERMINATION = 11,
    ATO_Packet_Type_SPARE = 12,
    ATO_Packet_Type_SPARE_MAX = 255,
} ATO_Packet_Type_Enum;

typedef enum
{
    Q_Reject_Reason_ATO_system_version_incompatible,
    Q_Reject_Reason_Another_ATO_TS_inCharge,
    Q_Reject_Reason_ATO_TS_inCharge_unknown,
    Q_Reject_Reason_Spare,
} Q_Reject_Reason_Enum;

typedef enum
{
    Q_JP_Status_Invalid,            // 유효하지 않음
    Q_JP_Status_Valid,              // 요청한 SP 식별자를 포함하는 JP
    Q_JP_Status_Unavailable,        // ATO-OB에 이미 전송한 JP안에 SP 식별자가 속하지 않음을 지정
    Q_JP_Status_Update,             // ATO-OB의 가시성 범위내에서 JP를 업데이트했다고 지정
    Q_JP_Status_Overwrite,          // 이전에 수신한 jp에 대해 완전히 덮어 쓰도록 지정
    Q_JP_Status_Spare_Start = 5,
    Q_JP_Status_Spare_Max = 7,
} Q_JP_Status_Enum;

typedef enum
{
    Q_SP_Status_Invalid,
    Q_SP_Status_Valid,
} Q_SP_Status_Enum;

typedef enum
{
    Q_SP_DIR_Reverse,
    Q_SP_DIR_Normal,
} Q_SP_DIR_Enum;

typedef enum
{
    Q_Range_Starts,
    Q_Range_Ends,
    Q_Range_StartsEnds,
    Q_Range_WholeSP,
} Q_Range_Enum;

typedef enum
{
    Q_TP_Alignment_FRONT,
    Q_TP_Alignment_MIDDLE,
    Q_TP_Alignment_REAR,
    Q_TP_Alignment_Spare,
} Q_TP_Alignment_Enum;

/**
 *  Timing Point Type
 */
typedef enum
{
    Q_Stop_SKIP_Pass_STOPPING_POINT = 0,
    Q_Stop_SKIP_Pass_STOPPING_POINT_TO_BE_SKIPPED = 1,
    Q_Stop_SKIP_Pass_PASSING_POINT = 2,
    Q_Stop_SKIP_Pass_Spare,
} Q_Stop_SKIP_Pass_Enum;

typedef enum
{
    Q_TP_information_No_specific_information,
    Q_TP_information_End_of_Journey,
    Q_TP_information_Spare,
} Q_TP_information_Enum;

typedef enum
{
    Q_Tunnel_Category_Single_Track_Tunnel,
    Q_Tunnel_Category_Double_Track_Tunnel,
    Q_Tunnel_Category_Wide_Cross_Section_Tunnel,
    Q_Tunnel_Category_Spare
} Q_Tunnel_Category_Enum;
typedef enum
{
    Q_Day_Light_Saving_Is_No_Saving_hour,
    Q_Day_Light_Saving_Is_Saving_hour
} Q_Day_Light_Saving_Enum;

typedef enum
{
    Q_Relaxed_Coupler_No_request_coupler_relaxation,
    Q_Relaxed_Coupler_request_coupler_relaxation
} Q_Relaxed_Coupler_Enum;

typedef enum
{
    Q_Train_Hold_Is_Do_not_hold_train,
    Q_Train_Hold_Is_Hold_train,
} Q_Train_Hold_Enum;

typedef enum
{
    Q_TC_Type_ASR,
    Q_TC_Type_Low_Adhesion,
    Q_TC_Type_ATO_Inhibition_Zone,
    Q_TC_Type_DAS_Inhibition_Zone,
    Q_TC_Type_Current_Limitation,
    Q_TC_Type_Spare_Start = 5,
    Q_TC_Type_Spare_End = 7
} Q_TC_Type_Enum;

typedef enum
{
    Q_Adhesion_Category_Dry_Rail,
    Q_Adhesion_Category_Dry_Rail_Medium,
    Q_Adhesion_Category_Dry_Rail_Low,
    Q_Adhesion_Category_Low_Adhesion,
    Q_Adhesion_Category_Very_Low_Adhesion,
    Q_Adhesion_Category_Extremely_Low_Adhesion,
    Q_Adhesion_Category_Spare_start,
    Q_Adhesion_Category_Spare_End,
} Q_Adhesion_Category_Enum;

typedef enum
{
    Q_ATOTS_Contact_Info_Dir_No_Contract_info_follows,
    Q_ATOTS_Contact_Info_Dir_ATOTS_contract_info_for_normal_direction_follows,
    Q_ATOTS_Contact_Info_Dir_ATOTS_contract_info_for_reverse_direction_follows,
    Q_ATOTS_Contact_Info_Dir_Spare
} Q_ATOTS_Contact_Info_Dir_Enum;

/**
 * Qualifier for specific SSP categories
 */
typedef enum
{
    Q_DIFF_Cant_Deficiency_specific_category,       // not used
    Q_DIFF_Other_specific_category_replaces_the_Cant_Deficiency_SSP,
    Q_DIFF_Other_specific_category_does_not_replace_the_Cant_Deficiency_SSP,
    Q_DIFF_Spare,
} Q_DIFF_Enum;

/**
 * 정적 라인 속도에 대해 다른 값이 존재하는 "캔트 결핍" SSP Category로,
 * V_DIFF와 함께 사용하여 특정 열차가 V_STATIC 에서 제공하는 국제 기본 정적 속도 보다 더 빠르게 또는 더 낮게 이동할 수 있음.
 */
typedef enum
{
    NC_CDDIFF_SpecificSSP_applicable_to_Cant_Deficiency_80mm,
    NC_CDDIFF_SpecificSSP_applicable_to_Cant_Deficiency_100mm,
    NC_CDDIFF_SpecificSSP_applicable_to_Cant_Deficiency_130mm,
    NC_CDDIFF_SpecificSSP_applicable_to_Cant_Deficiency_150mm,
    NC_CDDIFF_SpecificSSP_applicable_to_Cant_Deficiency_165mm,
    NC_CDDIFF_SpecificSSP_applicable_to_Cant_Deficiency_180mm,
    NC_CDDIFF_SpecificSSP_applicable_to_Cant_Deficiency_210mm,
    NC_CDDIFF_SpecificSSP_applicable_to_Cant_Deficiency_225mm,
    NC_CDDIFF_SpecificSSP_applicable_to_Cant_Deficiency_245mm,
    NC_CDDIFF_SpecificSSP_applicable_to_Cant_Deficiency_275mm,
    NC_CDDIFF_SpecificSSP_applicable_to_Cant_Deficiency_300mm,
    NC_CDDIFF_SpecificSSP_applicable_to_Cant_Deficiency_Spare,
} NC_CDDIFF_Enum;

/**
 * Specific SSP categoryies
 */
typedef enum
{
    NC_DIFF_Specific_SSP_applicable_to_international_train_category1,
    NC_DIFF_Specific_SSP_applicable_to_international_train_category2,
    NC_DIFF_Specific_SSP_applicable_to_international_train_category3,
    NC_DIFF_Specific_SSP_applicable_to_international_train_category4,
    NC_DIFF_Specific_SSP_applicable_to_international_train_category5,
    NC_DIFF_Specific_SSP_applicable_to_international_train_category6,
    NC_DIFF_Specific_SSP_applicable_to_international_train_category7,
    NC_DIFF_Specific_SSP_applicable_to_international_train_category8,
    NC_DIFF_Specific_SSP_applicable_to_international_train_category9,
    NC_DIFF_Specific_SSP_applicable_to_international_train_category10,
    NC_DIFF_Specific_SSP_applicable_to_international_train_category11,
    NC_DIFF_Specific_SSP_applicable_to_international_train_category12,
    NC_DIFF_Specific_SSP_applicable_to_international_train_category13,
    NC_DIFF_Specific_SSP_applicable_to_international_train_category14,
    NC_DIFF_Specific_SSP_applicable_to_international_train_category15,
    NC_DIFF_Spare,
} NC_DIFF_Enum;

/**
 * Traction System Voltage
 */
typedef enum
{
    M_VOLTAGE_Line_not_fitted_with_any_traction_system,
    M_VOLTAGE_1AC_25kV_50Hz,
    M_VOLTAGE_2AC_15kV_16_7_Hz,  /* 2DC 15kv, 16.7Hz*/
    M_VOLTAGE_3DC_3kV,
    M_VOLTAGE_4DC_1_5kV,        /* 4DC 1.5kv*/
    M_VOLTAGE_5DC_600_750V,     /* 5DC 600/700v*/
    M_VOLTAGE_Spare_Start = 6,
    M_VOLTAGE_Spare_End = 15,
} M_VOLTAGE_Enum;

typedef enum
{
    Q_NEWNID_C_Use_NID_C_of_SP,
    Q_NEWNID_C_NID_C_to_be_used_follows,
} Q_NEWNID_C_Enum;

typedef enum
{
    Q_Radius_Category_0_greater_than_7000m,
    Q_Radius_Category_1_greater_than_4500m_and_less_than_or_equal_7000m,
    Q_Radius_Category_2_greater_than_2800m_and_less_than_or_equal_4500m,
    Q_Radius_Category_3_greater_than_2000m_and_less_than_or_equal_2800m,
    Q_Radius_Category_4_greater_than_1500m_and_less_than_or_equal_2000m,
    Q_Radius_Category_5_greater_than_1250m_and_less_than_or_equal_1500m,
    Q_Radius_Category_6_greater_than_1075m_and_less_than_or_equal_1250m,
    Q_Radius_Category_7_greater_than_925m_and_less_than_or_equal_10750m,
    Q_Radius_Category_8_greater_than_825m_and_less_than_or_equal_925m,
    Q_Radius_Category_9_greater_than_725m_and_less_than_or_equal_825m,
    Q_Radius_Category_10_greater_than_625m_and_less_than_or_equal_725m,
    Q_Radius_Category_11_greater_than_525m_and_less_than_or_equal_625m,
    Q_Radius_Category_12_greater_than_475m_and_less_than_or_equal_525m,
    Q_Radius_Category_13_greater_than_425m_and_less_than_or_equal_475m,
    Q_Radius_Category_14_greater_than_375m_and_less_than_or_equal_425m,
    Q_Radius_Category_15_greater_than_325m_and_less_than_or_equal_375m,
    Q_Radius_Category_16_greater_than_300m_and_less_than_or_equal_325m,
    Q_Radius_Category_17_greater_than_275m_and_less_than_or_equal_300m,
    Q_Radius_Category_18_greater_than_250m_and_less_than_or_equal_275m,
    Q_Radius_Category_19_greater_than_225m_and_less_than_or_equal_250m,
    Q_Radius_Category_20_greater_than_200m_and_less_than_or_equal_225m,
    Q_Radius_Category_21_greater_than_175m_and_less_than_or_equal_200m,
    Q_Radius_Category_22_greater_than_150m_and_less_than_or_equal_175m,
    Q_Radius_Category_23_less_than_or_equal_150m,
    Q_Radius_Category_Spare_start = 24,
    Q_Radius_Category_Spare_End = 31,
} Q_Radius_Category_Enum;

typedef enum
{
    Q_Stop_Location_Tolerance_0_10cm,
    Q_Stop_Location_Tolerance_1_20cm,
    Q_Stop_Location_Tolerance_2_30cm,
    Q_Stop_Location_Tolerance_3_40cm,
    Q_Stop_Location_Tolerance_4_50cm,
    Q_Stop_Location_Tolerance_5_100cm,
    Q_Stop_Location_Tolerance_6_150cm,
    Q_Stop_Location_Tolerance_7_200cm,
    Q_Stop_Location_Tolerance_8_250cm,
    Q_Stop_Location_Tolerance_9_300cm,
    Q_Stop_Location_Tolerance_10_500cm,
    Q_Stop_Location_Tolerance_11_750cm,
    Q_Stop_Location_Tolerance_12_1000cm,
    Q_Stop_Location_Tolerance_13_1500cm,
    Q_Stop_Location_Tolerance_14_2000cm,
    Q_Stop_Location_Tolerance_15_2500cm,
    Q_Stop_Location_Tolerance_16_3000cm,
    Q_Stop_Location_Tolerance_17_5000cm,
    Q_Stop_Location_Tolerance_18_7500cm,
    Q_Stop_Location_Tolerance_19_10000cm,
    Q_Stop_Location_Tolerance_Spare_Start = 20,
    Q_Stop_Location_Tolerance_Spare_End = 30,
    Q_Stop_Location_Tolerance_31_No_requirement = 31,
} Q_Stop_Location_Tolerance_Enum;

typedef enum
{
    Q_STP_Reached_0_10cm,
    Q_STP_Reached_1_20cm,
    Q_STP_Reached_2_30cm,
    Q_STP_Reached_3_40cm,
    Q_STP_Reached_4_50cm,
    Q_STP_Reached_5_100cm,
    Q_STP_Reached_6_150cm,
    Q_STP_Reached_7_200cm,
    Q_STP_Reached_8_250cm,
    Q_STP_Reached_9_300cm,
    Q_STP_Reached_10_500cm,
    Q_STP_Reached_11_750cm,
    Q_STP_Reached_12_1000cm,
    Q_STP_Reached_13_1500cm,
    Q_STP_Reached_14_2000cm,
    Q_STP_Reached_15_2500cm,
    Q_STP_Reached_16_3000cm,
    Q_STP_Reached_17_5000cm,
    Q_STP_Reached_18_7500cm,
    Q_STP_Reached_19_1000cm,
    Q_STP_Reached_Spare_start = 20,
    Q_STP_Reached_Spare_End = 30,
    Q_STP_Reached_31_No_requirement = 31,
} Q_STP_Reached_Enum;

typedef enum
{
    Q_PBD_SBEB_Service_Brake,
    Q_PBD_SBEB_Emergency_Brake,
} Q_PBD_SBEB_Enum;

typedef enum
{
    Q_GDIR_PBD_Downhill,
    Q_GDIR_PBD_Uphill,
} Q_GDIR_PBD_Enum;

typedef enum
{
    Q_STR_Indication_JP_SP_Consistency_Error,
    Q_STR_Indication_Routing_Error,
    Q_STR_Indication_Next_Stop_Point_Skip,
    Q_STR_Indication_Low_Adhesion_Report_By_Driver,
    Q_STR_Indication_Operational_Condition_fulfilment,
    Q_STR_Indication_Train_is_moving,
    Q_STR_Indication_Unable_to_stop_next_Stopping_Point,
    Q_STR_Indication_Slip_Slide_reported_by_Train,
    Q_STR_Indication_spare_start = 8,
    Q_STR_Indication_spare_end = 15,
} Q_STR_Indication_Enum;
typedef enum
{
    Q_Termination_Reason_0_End_of_Journey_Reached,
    Q_Termination_Reason_1_ATOTS_Term_Request,
    Q_Termination_Reason_2_Last_SP_Left,
    Q_Termination_Reason_3_Cab_Closed,
    Q_Termination_Reason_4_TRN_or_Train_length_not_valid,
    Q_Termination_Reason_5_ETCS_OB_In_NL,
    Q_Termination_Reason_6_ETCS_OB_In_SH,
    Q_Termination_Reason_7_ATOTS_Handover_cancelled,
    Q_Termination_Reason_Spare_start = 8,
    Q_Termination_Reason_Spare_end = 15,
}Q_Termination_Reason_Enum;

enum Q_Pass_Stop_Depart_Enum
{
    Q_Pass_Stop_Depart_Passed_TP,
    Q_Pass_Stop_Depart_Stopped_TP,
    Q_Pass_Stop_Depart_Departed_TP,
    Q_Pass_Stop_Depart_Undefined
};

#pragma pack(push, 1)
typedef struct header_
{
    UINT8 NID_PACKET_ATO;
    UINT16 L_PACKET_ATO;
    UINT32 NID_OPERATIONAL;
    UINT32 NID_ENGINE;
    UINT16 T_Timestamp_Date;
    UINT32 T_Timestamp_Seconds;
    UINT8 N_Packet_Counter;
} ATO_Header;


/**************************************************************
 *
 *      Handshake Request 관련 구조체 정의
 *      (Pkt NO : 0)
 *
 **************************************************************/


typedef struct _HandshakeRequest
{
    ATO_Header header;
    UINT8 N_ITER;
    UINT16 M_ATO_Version[MAX_ITER_ATO_COUNT];
}OB_TS_Packet00_HSReqStruct;



/**************************************************************
 *
 *      Handshake Acknowledgement  관련 구조체 정의
 *      (Pkt NO : 1)
 *
 **************************************************************/

typedef struct _HandshakeAcknowledgement
{
    ATO_Header header;
    UINT16 NID_C;
    UINT16 NID_ATOTS;
    UINT16 M_ATO_Version;
    UINT8 T_Timeout_ATOTOS_Response;        //Resolution Seconds
    UINT8 T_Reporting_Time;                 //Resolution Seconds
}TS_OB_Packet01_HSAckStruct;

/**************************************************************
 *
 *      Handshake Reject 관련 구조체 정의
 *      (Pkt NO : 2)
 *
 **************************************************************/

typedef struct _HandshakeReject
{
    ATO_Header header;
    UINT8 Q_Reject_Reason;
    UINT16 NID_C;
    UINT16 NID_ATOTS;
}TS_OB_Packet02_HSRejStruct;


/**************************************************************
 *
 *      Journey Profile Request관련 구조체 정의
 *      (Pkt NO : 3)
 *
 **************************************************************/

typedef struct _JourneyProfileRequest
{
    ATO_Header header;
    UINT16 NID_C;
    UINT32 NID_SP;
}OB_TS_Packet03_JPReqStruct;

/**************************************************************
 *
 *      Journey Profile 관련 구조체 정의
 *      (Pkt NO : 4)
 *
 **************************************************************/

typedef struct
{
    UINT8 Q_Opening_Door_Side;
    UINT8 Q_Centralised_Opening;
    UINT8 Q_Relaxed_Coupler;
}STPInformation;

typedef struct
{
    UINT8 Q_Train_Hold;
    UINT16 T_Departure_Date;
    UINT32 T_Departure_Seconds;
    UINT16 T_Minimum_Dwell_Time;
    UINT8 Q_Automatic_Closing;
}STPDepartureDetails;

typedef struct
{
    UINT8 Q_FRONT;
    UINT8 V_Speed_Level;
}AdditionalSpeedRestriction;

typedef struct
{
    UINT8 Q_Adhesion_Category;
}LowAdhesion;

typedef struct
{
    UINT16 M_CURRENT;
}CurrentLimitation;

typedef struct
{
    UINT8 Q_TC_Type;
    UINT8 Q_Range;
    UINT32 D_TC_Start_Location;
    UINT32 D_TC_End_Location;
    /** Q_TC_TYPE == ASR  */
    AdditionalSpeedRestriction asr;
    /** Q_TC_TYPE == Low Adhesion */
    LowAdhesion lowAdhesion;
    CurrentLimitation currentLimitation;
}TemporaryConstrains;

typedef struct
{
    UINT32 NID_TP;
    UINT16 T_Latest_Arrival_Date;
    UINT32 T_Latest_Arrival_Seconds;
    UINT16 T_Arrival_Window;
    UINT8 Q_TP_Alignment;
    UINT8 Q_Stop_Skip_Pass;
    UINT8 Q_TP_information;
    UINT8 Q_Day_Light_Saving;
    /** Q_Stop_Skip_Pass == Stopping Point */
    STPInformation stpInformation;
    /** Q_Stop_Skip_Pass == Stopping Point and Q_TP_Information != End_of_Journey */
    STPDepartureDetails stpDepartureDetails;
}TimingPointConstraints;

typedef struct
{
    UINT16 NID_C;
    UINT32 NID_SP;
    UINT16 M_SP_Version;
    UINT8 Q_SP_DIR;
    UINT8 N_ITER;
    TimingPointConstraints TPConstraints[MAX_ITER_ATO_COUNT];
    /** Temporary Constrains */
    UINT8 N_ITER_tempConstraints;
    TemporaryConstrains temporaryConstrains[MAX_ITER_COUNT];
}SegmentProfileReference;

typedef struct _JourneyProfile
{
    ATO_Header header;
    /** Journey Profile Details */
    UINT8 Q_JP_Status;
    /** Segment Profile Reference */
    UINT8 N_ITER_SP;
    SegmentProfileReference segmentProfileReference[MAX_ITER_SP_COUNT];
}TS_OB_Packet04_JPStruct;


/**************************************************************
 *
 *      Journey Profile Acknowledgement 관련 구조체 정의
 *      (Pkt NO : 5)
 *
 **************************************************************/

typedef struct _JourneyProfileAcknowledgement
{
    ATO_Header header;
    UINT16 T_JP_Ref_TimeStamp_Date;
    UINT32 T_JP_Ref_TimeStamp_Seconds;
    UINT8 N_JP_Ref_Packet_Counter;
}OB_TS_Packet05_JPAckStruct;


/**************************************************************
 *
 *      Segment Profile RequestPacket 관련 구조체 정의
 *      (Pkt NO :  6)
 *
 **************************************************************/


typedef struct
{
    UINT16 NID_C;
    UINT32 NID_SP;
}SegmentProfileReqDetails;

typedef struct segmentRequest_
{
    ATO_Header header;
    UINT8 N_ITER_ATO;
    SegmentProfileReqDetails SPReqDetails[MAX_ITER_ATO_COUNT];
}OB_TS_Packet06_SPReqStruct;

/**************************************************************
 *
 *      Segment Profile Packet 관련 구조체 정의
 *      (Pkt NO :  7)
 *
 **************************************************************/

typedef struct
{
    UINT8 Q_DIFF;
    UINT8 NC_CDDIFF;
    UINT8 NC_DIFF;
    UINT8 V_DIFF;
}SpecificSSP;

typedef struct
{
    UINT8 Q_DIFF;
    UINT8 NC_CDDIFF;
    UINT8 NC_DIFF;
    UINT8 V_DIFF;
}SpecificSspChange;

typedef struct
{
    UINT32 D_Location;
    UINT8 V_STATIC;
    UINT8 Q_FRONT;
    UINT8 N_ITER;
    SpecificSspChange specificSspChange[MAX_ITER_COUNT];
}StaticSPChange;

typedef struct
{
    UINT32 D_Location;
    UINT16 G_New_Gradient;
    UINT8 Q_GDIR;
}GradientChange;

typedef struct
{
    UINT32 D_Location;
    UINT8 Q_Radius_Category;
}CurvesChange;

typedef struct
{
    UINT32 D_Location;
    UINT8 M_VOLTAGE;
    UINT16 NID_CTRACTION;
}PowerVoltageChange;

typedef struct
{
    UINT32 D_Location;
    UINT16 M_CURRENT;
}CurrentLimitationChange;

typedef struct
{
    UINT8 N_PIG;
    UINT32 D_Location;
}Balise;

typedef struct
{
    UINT8 Q_NEWNID_C;
    UINT16 NID_C;
    UINT16 NID_BG;
    UINT8 N_ITER_BG;
    Balise balises[MAX_ITER_COUNT];
}BaliseGroup;

typedef struct
{
    UINT32 NID_TP;
    UINT32 D_Location;
    UINT8 Q_Stop_Location_Tolerance;
    UINT8 Q_STP_Reached;
    /** Timing Point Names */
    UINT8 L_TEXT;
    UINT8 X_TEXT[MAX_L_TEXT];
}TimingPoints;

typedef struct
{
    UINT8 Q_Range;
    UINT32 D_Start_Location;
    UINT32 D_End_Location;
}PlatformArea;

typedef struct
{
    UINT8 Q_Range;
    UINT8 Q_Tunnel_Category;
    UINT32 D_Start_Location;
    UINT32 D_End_Location;
}Tunnel;

typedef struct
{
    UINT8 Q_Range;
    UINT8 M_AXLELOADCAT;
    UINT8 V_New_Speed_Level;
    UINT8 Q_FRONT;
    UINT32 D_Start_Location;
    UINT32 D_End_Location;
}AxleLoadSpeedProfile;

typedef struct
{
    UINT32 D_UnprotectedLx_Stop_Nominal;
    UINT32 D_UnprotectedLx_Stop_Reverse;
}UnprotectedLevelCrossingStop;

typedef struct
{
    UINT8 Q_Range;
    UINT32 D_Permitted_Braking_Distance;
    UINT8 Q_PBD_SBEB;
    UINT8 G_PBD;
    UINT8 Q_GDIR_PBD;
    UINT32 D_Start_Location;
    UINT32 D_End_Location;
}PermittedBrakingDistance;

typedef struct
{
    UINT8 Q_Range;
    UINT32 D_Start_Location;
    UINT32 D_End_Location;
}SwitchOffRegenerativeBrake;

typedef struct
{
    UINT8 Q_Range;
    UINT32 D_Start_Location;
    UINT32 D_End_Location;
}SwitchOffEddyCurrentBrake;

typedef struct
{
    UINT8 Q_Range;
    UINT32 D_Start_Location;
    UINT32 D_End_Location;
}SwitchOffEddyEmergencyBrake;

typedef struct
{
    UINT8 Q_Range;
    UINT32 D_Start_Location;
    UINT32 D_End_Location;
}SwitchOffMagmaticShoeBrake;

typedef struct
{
    UINT8 Q_Range;
    UINT32 D_Start_Location;
    UINT32 D_End_Location;
}AreaWithDynamicBrakeForceInhibition;

typedef struct
{
    UINT8 Q_Range;
    UINT16 M_Dynamic_Braking_Force_Limit;
    UINT32 D_Start_Location;
    UINT32 D_End_Location;
}AreaWithLimitedDynamicBrakeForce;

typedef struct
{
    UINT16 M_SP_Version;
    UINT32 L_SP;
    UINT32 D_EOA_Offset;
    UINT8 Q_UTC_Offset;
    UINT32 M_SP_Altitude;
    UINT8 Q_ATOTS_Contact_Info_Dir;
}SPStatusValid;

typedef struct
{
    /** Q_ATOTS_Contact_Info_Dir_Normal or Q_ATOTS_Contact_Info_Dir_Reverse */
    UINT16 NID_C_1;
    UINT16 NID_ATOTS;
    UINT16 NID_C_2;
    UINT32 NID_SP_2;
}ATOTSContactInfo;

typedef struct
{
    UINT16 NID_C;
    UINT32 NID_SP_1;
    UINT8 Q_SP_Status;
    /** Q_SP_Status != Invalid */
    SPStatusValid spStatusValid;
    /** ATO-TS Contact Information
     (Q_ATOTS_Contact_Info_Dir == ATO-TS contact info for nominal direction follows) ||
     (Q_ATOTS_Contact_Info_Dir == ATO-TS contact info for reverse direction follows)*/
    ATOTSContactInfo atoTSContactInfo;
    /** Static Speed Profile Start */
    UINT8 V_STATIC;
    UINT8 Q_FRONT;
    UINT8 N_ITER_ATO_1;
    SpecificSSP specificSSP[MAX_ITER_COUNT];
    UINT8 N_ITER_ATO_2;
    StaticSPChange staticSPChange[MAX_ITER_COUNT];
    /** Gradient Start */
    UINT16 G_New_Gradient;
    UINT8 Q_GDIR;
    UINT8 N_ITER_ATO_3;
    GradientChange gradientChange[MAX_ITER_COUNT];
    /** Curve Start */
    UINT8 Q_Radius_Category;
    UINT8 N_ITER_ATO_4;
    CurvesChange curvesChange[MAX_ITER_COUNT];
    /** Power voltage Start */
    UINT8 M_VOLTAGE ;
    UINT16 NID_CTRACTION;
    UINT8 N_ITER_ATO_5;
    PowerVoltageChange powerVoltageChange[MAX_ITER_COUNT];
    /** current Limitation start */
    UINT16 M_CURRENT;
    UINT8 N_ITER_ATO_6;
    CurrentLimitationChange currentLimitChange[MAX_ITER_COUNT];
    /** Balise Group ***/
    UINT8 N_ITER_ATO_7;
    BaliseGroup baliseGroup[MAX_ITER_COUNT];
    /** TimingPoints ***/
    UINT8 N_ITER_ATO_8 ;
    TimingPoints timingPoints[MAX_ITER_COUNT];
    /** PlatformArea ***/
    UINT8 N_ITER_ATO_9;
    PlatformArea platformArea[MAX_ITER_COUNT];
    /** Tunnel ***/
    UINT8 N_ITER_ATO_10;
    Tunnel tunnel[MAX_ITER_COUNT];
    UINT8 N_ITER_ATO_11;
    AxleLoadSpeedProfile axleLoadSpeedProfile[MAX_ITER_COUNT];
    UINT8 N_ITER_ATO_12;
    UnprotectedLevelCrossingStop unprotectedLevelCrossingStop[MAX_ITER_COUNT];
    UINT8 N_ITER_ATO_13;
    PermittedBrakingDistance permittedBrakingDistance[MAX_ITER_COUNT];
    UINT8 N_ITER_ATO_14;
    SwitchOffRegenerativeBrake switchOffRegenerativeBrake[MAX_ITER_COUNT];
    UINT8 N_ITER_ATO_15;
    SwitchOffEddyCurrentBrake switchOffEddyCurrentBrake[MAX_ITER_COUNT];
    UINT8 N_ITER_ATO_16;
    SwitchOffEddyEmergencyBrake switchOffEddyEmergencyBrake[MAX_ITER_COUNT];
    UINT8 N_ITER_ATO_17;
    SwitchOffMagmaticShoeBrake switchOffMagmaticShoeBrake[MAX_ITER_COUNT];
    UINT8 N_ITER_ATO_18;
    AreaWithDynamicBrakeForceInhibition areaWithDynamicBrakeForceInhibition[MAX_ITER_COUNT];
    UINT8 N_ITER_ATO_19;
    AreaWithLimitedDynamicBrakeForce areaWithLimitedDynamicBrakeForce[MAX_ITER_COUNT];
}SegmentProfileStatus;

typedef struct segmentProfile_
{
    ATO_Header header;
    UINT8 N_ITER_ATO;
    SegmentProfileStatus SPStatus[MAX_ITER_ATO_COUNT];
}TS_OB_Packet07_SPStruct;

/**************************************************************
 *
 *      Status Report Packet 관련 구조체 정의
 *      (Pkt NO :  8)
 *
 **************************************************************/

typedef struct
{
    UINT16 NID_C;
    UINT32 NID_SP;
    UINT32 D_Sending_Position;
}STRSendingLocation;

typedef struct
{
    UINT16 NID_C;
    UINT32 NID_TP;
    UINT8 Q_Pass_Stop_Depart;
    UINT8 Q_Accurate_Stopping;
}PreviousTPInformation;

typedef struct
{
    UINT16 NID_C;
    UINT32 NID_TP;
    UINT16 T_Arrival_Date;
    UINT32 T_Arrival_Seconds;
}TimingPointEstimation;

typedef struct statusReport_
{
    ATO_Header header;
    UINT8 M_ATO_State;
    UINT16 Q_STR_Indication;
    UINT16 V_TRAIN_ATO;
    UINT16 L_TRAIN;
    UINT8 DRIVER_ID[LEN_DRIVER_ID_ARRAY];
    STRSendingLocation sendingLocation;
    PreviousTPInformation previousTpInfo;
    UINT8 N_ITER;
    TimingPointEstimation TPEstimation[MAX_ITER_COUNT];
}OB_TS_Packet08_STRStruct;



/**************************************************************
 *
 *      Status Report Ack 관련 구조체 정의
 *      (Pkt NO :  9)
 *
 **************************************************************/


typedef struct StatusReportAcknowledgement_
{
    ATO_Header header;
    UINT16 T_STR_Ref_TimeStamp_Date;
    UINT32 T_STR_Ref_TimeStamp_Seconds;
    UINT8 N_STR_Ref_Packet_Counter;
}TS_OB_Packet09_STRAckStruct;


/**************************************************************
 *
 *      Session Termination Request 패킷 관련 구조체 정의
 *      (Pkt NO :  10)
 *
 **************************************************************/

typedef struct sessionTerminationRequest_
{
    ATO_Header header;
}TS_OB_Packet10_SESSTermReqStruct;



/**************************************************************
 *
 *      Session Termination 패킷 관련 구조체 정의
 *      (Pkt NO :  11)
 *
 **************************************************************/

typedef struct sessionTermination_
{
    ATO_Header header;
    UINT8 Q_Termination_Reason;
} OB_TS_Packet11_SESSTermStruct;
#pragma pack(pop)

#endif //F110_RADIO_PACKET_H_
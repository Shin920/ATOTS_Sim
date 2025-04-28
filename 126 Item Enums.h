#pragma once

enum PacketPropertyCtrlIndex
{
	HEADER,
	BODY_DATA
};

namespace PacketHeaderEnum
{
	enum PacketHeaderIDX
	{
		IDX_NID_PACKET_ATO,
		IDX_NID_OPERATIONAL,
		IDX_NID_ENGINE,
		IDX_T_Timestamp_Date,
		IDX_T_Timestamp_Seconds,
		IDX_N_Packet_Counter
	};
}


namespace PacketHSReqEnum
{
	enum HSAckPropertyName
	{
		STATIC_DATA_HSR
	};

	enum HSReqItemIndex
	{
		IDX_HSR_N_ITER,
		IDX_HSR_N_ITER_Property
	};

	enum HSReqItemIndex_Ver
	{
		IDX_HSR_M_ATO_Version
	};
}

namespace PacketHSAckEnum
{
	enum HSAckPropertyName
	{
		STATIC_DATA
	};

	enum HSAckItemIndex
	{
		IDX_HSA_NID_C,
		IDX_HSA_NID_ATOTS,
		IDX_HSA_M_ATO_Version,
		IDX_HSA_T_Timeout_ATOTS_Response,
		IDX_HSA_T_Reporting_Time
	};
}

namespace PacketHSRejEnum
{
	enum HSRejItemIndex
	{
		IDX_Q_Reject_Reason,
		IDX_HSRJ_NID_C,
		IDX_HSRJ_NID_ATOTS
	};
}

namespace PacketJPReqEnum
{
	enum JPReqItemIndex
	{
		IDX_JPR_NID_C,
		IDX_JPR_NID_SP
	};
}

namespace PacketJPEnum
{
	enum JPItemIndex
	{
		IDX_Q_JP_Status,
		IDX_N_ITER_SP,
		IDX_SP_ITER_Property
	};

	enum JPItemIndex_SP
	{
		IDX_SPNJP_NID_C,
		IDX_SPNJP_NID_SP,
		IDX_SPNJP_M_SP_Version,
		IDX_SPNJP_Q_SP_DIR,
		IDX_SPNJP_N_ITER_TP,
		IDX_TIMING_POINT_CONSTRAINTS,
		IDX_SPNJP_N_ITER_TC,
		IDX_TEMPORARY_CONSTRAINTS
	};

	enum JPItemIndex_SP_TP
	{
		IDX_TPNSPNJP_NID_TP,
		IDX_TPNSPNJP_T_Latest_Arrival_Date,
		IDX_TPNSPNJP_T_Latest_Arrival_Seconds,
		IDX_TPNSPNJP_T_Arrival_Window,
		IDX_TPNSPNJP_Q_TP_Alignment,
		IDX_TPNSPNJP_Q_Stop_Skip_Pass,
		IDX_TPNSPNJP_Q_TP_Information,
		IDX_TPNSPNJP_Q_Day_Light_Saving,
		IDX_TPNSPNJP_Q_Opening_Door_Side,
		IDX_TPNSPNJP_Q_Centralised_Opening,
		IDX_TPNSPNJP_Q_Relaxed_Coupler,
		IDX_TPNSPNJP_Q_Train_Hold,
		IDX_TPNSPNJP_T_Departure_Date,
		IDX_TPNSPNJP_T_Departure_Seconds,
		IDX_TPNSPNJP_T_Minimum_Dwell_Time,
		IDX_TPNSPNJP_Q_Automatic_Closing
	};

	enum JPItemIndex_SP_TC
	{
		IDX_TCNSPNJP_Q_TC_Type,
		IDX_TCNSPNJP_Q_Range,
		IDX_TCNSPNJP_D_TC_Start_Location,
		IDX_TCNSPNJP_D_TC_End_Location,
		IDX_TCNSPNJP_Q_FRONT,
		IDX_TCNSPNJP_V_Speed_Level,
		IDX_TCNSPNJP_Q_Adhesion_Category,
		IDX_TCNSPNJP_M_CURRENT
	};
}

namespace PacketJPAckEnum
{
	enum JPAckItemIndex
	{
		IDX_T_JP_Reference_Timestamp_Date,
		IDX_T_JP_Reference_Timestamp_Seconds,
		IDX_N_JP_Reference_Packet_Counter
	};
}

namespace PacketSPReqEnum
{
	enum SPReqItemIndex
	{
		IDX_SPR_N_ITER,
		IDX_SPR_ITER_Property
	};

	enum SPReqItemIndex_SP
	{
		IDX_SPR_NID_C,
		IDX_SPR_NID_SP
	};
}

namespace PacketSPEnum
{
	enum SPItemIndex
	{
		IDX_SP_N_ITER,
		IDX_SPNSP_ITER_Property
	};
	enum SPItemIndex_SP
	{
		IDX_SPNSP_NID_C,
		IDX_SPNSP_NID_SP,
		IDX_SPNSP_Q_SP_Status,
		IDX_SPNSP_M_SP_Version,
		IDX_SPNSP_L_SP,
		IDX_SPNSP_D_EoA_Offset,
		IDX_SPNSP_Q_UTC_Offset,
		IDX_SPNSP_M_SP_Altitude,
		IDX_SPNSP_Q_ATOTS_Contact_Info_Dir,
		IDX_SPNSP_NID_C_TS,
		IDX_SPNSP_NID_ATOTS,
		IDX_SPNSP_NID_C_SPC,
		IDX_SPNSP_NID_SP_F,
		IDX_SPNSP_V_STATIC,
		IDX_SPNSP_Q_FRONT,
		IDX_SPNSP_N_ITER_SSP,
		IDX_SPNSP_SSP_ITER_Property,
		IDX_SPNSP_N_ITER_SSPC,
		IDX_SPNSP_SSPC_ITER_Property,
		IDX_SPNSP_G_New_Gradient,
		IDX_SPNSP_Q_GDIR,
		IDX_SPNSP_N_ITER_GC,
		IDX_SPNSP_GC_ITER_Property,
		IDX_SPNSP_Q_Radius_Category,
		IDX_SPNSP_N_ITER_CC,
		IDX_SPNSP_CC_ITER_Property,
		IDX_SPNSP_M_VOLTAGE,
		IDX_SPNSP_NID_CTRACTION,
		IDX_SPNSP_N_ITER_PVC,
		IDX_SPNSP_PVC_ITER_Property,
		IDX_SPNSP_M_CURRENT,
		IDX_SPNSP_N_ITER_CLC,
		IDX_SPNSP_CLC_ITER_Property,
		IDX_SPNSP_N_ITER_BG,
		IDX_SPNSP_BG_ITER_Property,
		IDX_SPNSP_N_ITER_TP,
		IDX_SPNSP_TP_ITER_Property,
		IDX_SPNSP_N_ITER_PA,
		IDX_SPNSP_PA_ITER_Property,
		IDX_SPNSP_N_ITER_T,
		IDX_SPNSP_T_ITER_Property,
		IDX_SPNSP_N_ITER_ALSP,
		IDX_SPNSP_ALSP_ITER_Property,
		IDX_SPNSP_N_ITER_ULCS,
		IDX_SPNSP_ULCS_ITER_Property,
		IDX_SPNSP_N_ITER_PBD,
		IDX_SPNSP_PBD_ITER_Property,
		IDX_SPNSP_N_ITER_SORB,
		IDX_SPNSP_SORB_ITER_Property,
		IDX_SPNSP_N_ITER_SOECB,
		IDX_SPNSP_SOECB_ITER_Property,
		IDX_SPNSP_N_ITER_SOECEB,
		IDX_SPNSP_SOECEB_ITER_Property,
		IDX_SPNSP_N_ITER_SOMSB,
		IDX_SPNSP_SOMSB_ITER_Property,
		IDX_SPNSP_N_ITER_ADBFI,
		IDX_SPNSP_ADBFI_ITER_Property,
		IDX_SPNSP_N_ITER_ALDBF,
		IDX_SPNSP_ALDBF_ITER_Property
	};

	enum SPItemIndex_SP_SSP
	{
		IDX_Q_DIFF_SSP,
		IDX_NC_CDDIFF_SSP,
		IDX_NC_DIFF_SSP,
		IDX_V_DIFF_SSP
	};

	enum SPItemIndex_SP_SSPC
	{
		IDX_D_Location_SSPC,
		IDX_V_STATIC_SSPC,
		IDX_Q_FRONT_SSPC,
		IDX_N_ITER_SSPC_N,
		IDX_SPNSP_SSPC_P_ITER_Property
	};

	enum SPItemIndex_SP_SSPC_N
	{
		IDX_Q_DIFF_N,
		IDX_NC_CDDIFF_N,
		IDX_NC_DIFF_N,
		IDX_V_DIFF_N
	};

	enum SPItemIndex_SP_GC
	{
		IDX_D_Location_GC,
		IDX_G_New_Gradient_GC,
		IDX_Q_GDIR_GC
	};

	enum SPItemIndex_SP_CC
	{
		IDX_D_Location_CC,
		IDX_Q_Radius_Category_CC
	};

	enum SPItemIndex_SP_PVC
	{
		IDX_D_Location_PVC,
		IDX_M_VOLTAGE_PVC,
		IDX_NID_CTRACTION_PVC
	};

	enum SPItemIndex_SP_CLC
	{
		IDX_D_Location_CLC,
		IDX_M_CURRENT_CLC
	};

	enum SPItemIndex_SP_BG
	{
		IDX_Q_NEWNID_C,
		IDX_NID_C_BG,
		IDX_NID_BG,
		IDX_N_ITER_BG,
		IDX_SPNSP_B_ITER_Property
	};

	enum SPItemIndex_SP_BG_N
	{
		IDX_N_PIG,
		IDX_D_Location_N
	};

	enum SPItemIndex_SP_TP
	{
		IDX_NID_TP,
		IDX_D_Location_TP,
		IDX_Q_Stop_Location_Tolerance,
		IDX_Q_STP_Reached,
		IDX_L_TEXT,
		IDX_SPNSP_LTEXT_ITER_Property
	};

	enum SPItemIndex_SP_TP_L
	{
		IDX_X_TEXT
	};

	enum SPItemIndex_SP_PA
	{
		IDX_Q_Range_PA,
		IDX_D_Start_Location_PA,
		IDX_D_End_Location_PA
	};

	enum SPItemIndex_SP_T
	{
		IDX_Q_Range_T,
		IDX_Q_Tunnel_Category,
		IDX_D_Start_Location_T,
		IDX_D_End_Location_T
	};

	enum SPItemIndex_SP_ALSP
	{
		IDX_Q_Range_ALSP,
		IDX_M_AXLELOADCAT,
		IDX_V_New_Speed_Level,
		IDX_Q_FRONT_ALSP,
		IDX_D_Start_Location_ALSP,
		IDX_D_End_Location_ALSP
	};

	enum SPItemIndex_SP_ULCS
	{
		IDX_D_UnprotectedLx_Stop_Nominal,
		IDX_D_UnprotectedLx_Stop_Reverse
	};

	enum SPItemIndex_SP_PBD
	{
		IDX_Q_Range_PBD,
		IDX_D_Permitted_Braking_Distance,
		IDX_Q_PBD_SBEB,
		IDX_G_PBD,
		IDX_Q_GDIR_PBD,
		IDX_D_Start_Location_PBD,
		IDX_D_End_Location_PBD
	};

	enum SPItemIndex_SP_SORB
	{
		IDX_Q_Range_SORB,
		IDX_D_Start_Location_SORB,
		IDX_D_End_Location_SORB
	};

	enum SPItemIndex_SP_SOECB
	{
		IDX_Q_Range_SOECB,
		IDX_D_Start_Location_SOECB,
		IDX_D_End_Location_SOECB
	};

	enum SPItemIndex_SP_SOECEB
	{
		IDX_Q_Range_SOECEB,
		IDX_D_Start_Location_SOECEB,
		IDX_D_End_Location_SOECEB
	};

	enum SPItemIndex_SP_SOMSB
	{
		IDX_Q_Range_SOMSB,
		IDX_D_Start_Location_SOMSB,
		IDX_D_End_Location_SOMSB
	};

	enum SPItemIndex_SP_ADBFI
	{
		IDX_Q_Range_ADBFI,
		IDX_D_Start_Location_ADBFI,
		IDX_D_End_Location_ADBFI
	};

	enum SPItemIndex_SP_ALDBF
	{
		IDX_Q_Range_ALDBF,
		IDX_M_Dynamic_Brake_Force_Limit,
		IDX_D_Start_Location_ALDBF,
		IDX_D_End_Location_ALDBF
	};
}

namespace PacketSTREnum
{
	enum STRItemIndex
	{
		IDX_STR_M_ATO_State,
		IDX_STR_Q_STR_Indicators,
		IDX_STR_V_TRAIN_ATO,
		IDX_STR_L_TRAIN,
		IDX_STR_DRIVER_ID,
		IDX_STR_NID_C_SP,
		IDX_STR_NID_SP,
		IDX_STR_D_Sending_Position,
		IDX_STR_NID_C_TP,
		IDX_STR_NID_TP,
		IDX_STR_Q_Pass_Stop_Depart,
		IDX_STR_Q_Accurate_Stopping,
		IDX_STR_N_ITER,
		IDX_STR_TPE_ITER_Property
	};

	enum STRItemIndex_TPE
	{
		IDX_NID_C_TPE,
		IDX_NID_TP_TPE,
		IDX_T_Arrival_Date_TPE,
		IDX_T_Arrival_Seconds_TPE
	};
}

namespace PacketSTRAckEnum

{
	enum STRAckItemIndex
	{
		IDX_T_STR_Reference_Timestamp_Date,
		IDX_T_STR_Reference_Timestamp_Seconds,
		IDX_N_STR_Reference_Packet_Counter
	};
}

namespace PacketSESSTermEnum
{
	enum SESSTermItemIndex
	{
		IDX_Q_Termination_Reason
	};
}
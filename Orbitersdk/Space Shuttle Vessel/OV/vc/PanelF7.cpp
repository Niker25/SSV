/******* SSV File Modification Notice *******
Date         Developer
2020/04/01   GLS
2020/06/20   GLS
2020/06/23   GLS
2021/06/02   GLS
2021/08/23   GLS
2021/08/24   GLS
2022/03/24   GLS
2022/03/26   GLS
2022/04/20   GLS
2022/05/29   GLS
2022/07/02   GLS
2022/07/17   GLS
2022/07/24   GLS
2022/08/05   GLS
2022/09/25   GLS
2022/09/29   GLS
2024/12/30   GLS
********************************************/
#include "PanelF7.h"
#include "MDU.h"
#include "StandardSwitch.h"
#include "StandardLight.h"
#include "7SegDisp_EventTime.h"
#include <discsignals.h>
#include "../Atlantis.h"
#include "../meshres_vc.h"
#include "../ParameterValues.h"
#include "vc_defs.h"
#include "../meshres_vc_f7.h"
#include "../dps/IDP.h"


namespace vc {

	PanelF7::PanelF7(Atlantis* _sts, bool hasMEDS)
		: AtlantisPanel(_sts, "F7")
	{
		bHasMEDS = hasMEDS;

		if (bHasMEDS )DefineMesh( MESHNAME_PANELF7 );
		else DefineMesh(MESHNAME_PANELF7_MCD);

		Add(pCRT1 = new MDU(_sts, "CRT1", MDUID_CRT1));
		Add(pCRT2 = new MDU(_sts, "CRT2", MDUID_CRT2));
		Add(pCRT3 = new MDU(_sts, "CRT3", MDUID_CRT3));
		Add(pMFD1 = new MDU(_sts, "MFD1", MDUID_MFD1));
		Add(pMFD2 = new MDU(_sts, "MFD2", MDUID_MFD2));

		Add( pEventTime = new _7SegDisp_EventTime( _sts, "EVENT TIME" ) );

		Add( pCWLight[0] = new StandardSingleLight( _sts, "CW O2 PRESS" ) );
		Add( pCWLight[1] = new StandardSingleLight( _sts, "CW CABIN ATM" ) );
		Add( pCWLight[2] = new StandardSingleLight( _sts, "CW FREON LOOP" ) );
		Add( pCWLight[3] = new StandardSingleLight( _sts, "CW H2O LOOP" ) );
		Add( pCWLight[4] = new StandardSingleLight( _sts, "CW SPARE 5A" ) );
		Add( pCWLight[5] = new StandardSingleLight( _sts, "CW PAYLOAD WARNING" ) );
		Add( pCWLight[6] = new StandardSingleLight( _sts, "CW PAYLOAD CAUTION" ) );
		Add( pCWLight[7] = new StandardSingleLight( _sts, "CW BACKUP C/W ALARM" ) );

		Add( pCWLight[8] = new StandardSingleLight( _sts, "CW H2 PRESS" ) );
		Add( pCWLight[9] = new StandardSingleLight( _sts, "CW O2 HEATER TEMP" ) );
		Add( pCWLight[10] = new StandardSingleLight( _sts, "CW AV BAY/CABIN AIR" ) );
		Add( pCWLight[11] = new StandardSingleLight( _sts, "CW RGA/ACCEL" ) );
		Add( pCWLight[12] = new StandardSingleLight( _sts, "CW LEFT RHC" ) );
		Add( pCWLight[13] = new StandardSingleLight( _sts, "CW GPC" ) );
		Add( pCWLight[14] = new StandardSingleLight( _sts, "CW PRIMARY C/W" ) );
		Add( pCWLight[15] = new StandardSingleLight( _sts, "CW APU TEMP" ) );

		Add( pCWLight[16] = new StandardSingleLight( _sts, "CW FUEL CELL REAC" ) );
		Add( pCWLight[17] = new StandardSingleLight( _sts, "CW MAIN BUS UNDERVOLT" ) );
		Add( pCWLight[18] = new StandardSingleLight( _sts, "CW IMU" ) );
		Add( pCWLight[19] = new StandardSingleLight( _sts, "CW AIR DATA" ) );
		Add( pCWLight[20] = new StandardSingleLight( _sts, "CW RIGHT/AFT RHC" ) );
		Add( pCWLight[21] = new StandardSingleLight( _sts, "CW FCS SATURATION" ) );
		Add( pCWLight[22] = new StandardSingleLight( _sts, "CW FCS CHANNEL" ) );
		Add( pCWLight[23] = new StandardSingleLight( _sts, "CW APU OVERSPEED" ) );

		Add( pCWLight[24] = new StandardSingleLight( _sts, "CW FUEL CELL STACK TEMP" ) );
		Add( pCWLight[25] = new StandardSingleLight( _sts, "CW AC VOLTAGE" ) );
		Add( pCWLight[26] = new StandardSingleLight( _sts, "CW FWD RCS" ) );
		Add( pCWLight[27] = new StandardSingleLight( _sts, "CW LEFT RCS" ) );
		Add( pCWLight[28] = new StandardSingleLight( _sts, "CW LEFT OMS" ) );
		Add( pCWLight[29] = new StandardSingleLight( _sts, "CW OMS KIT" ) );
		Add( pCWLight[30] = new StandardSingleLight( _sts, "CW MPS" ) );
		Add( pCWLight[31] = new StandardSingleLight( _sts, "CW APU UNDERSPEED" ) );

		Add( pCWLight[32] = new StandardSingleLight( _sts, "CW FUEL CELL PUMP" ) );
		Add( pCWLight[33] = new StandardSingleLight( _sts, "CW AC OVERLOAD" ) );
		Add( pCWLight[34] = new StandardSingleLight( _sts, "CW RCS JET" ) );
		Add( pCWLight[35] = new StandardSingleLight( _sts, "CW RIGHT RCS" ) );
		Add( pCWLight[36] = new StandardSingleLight( _sts, "CW RIGHT OMS" ) );
		Add( pCWLight[37] = new StandardSingleLight( _sts, "CW OMS TVC" ) );
		Add( pCWLight[38] = new StandardSingleLight( _sts, "CW SPARE 7E" ) );
		Add( pCWLight[39] = new StandardSingleLight( _sts, "CW HYD PRESS" ) );

		Add( pMainEngStatus[0] = new StandardDoubleLight( _sts, "MAIN ENGINE STATUS CTR" ) );
		Add( pMainEngStatus[1] = new StandardDoubleLight( _sts, "MAIN ENGINE STATUS LEFT" ) );
		Add( pMainEngStatus[2] = new StandardDoubleLight( _sts, "MAIN ENGINE STATUS RIGHT" ) );

		Add( pSMAlert = new StandardSingleLight( _sts, "SM ALERT" ) );

		if (!bHasMEDS)
		{
			Add(pFltCntlrPower = new LockableLever2(_sts, "FLT CNTLR POWER"));
			pFltCntlrPower->SetLabel(0, "OFF");
			pFltCntlrPower->SetLabel(1, "ON");

			Add(pRDRALTM = new StdSwitch2(_sts, "RDR ALTM"));
			pRDRALTM->SetLabel(0, "2");
			pRDRALTM->SetLabel(1, "1");
			pRDRALTM->SetInitialPosition(1);
		}
	}

	PanelF7::~PanelF7()
	{
		delete mgr_needle_oms_pc_left;
		delete mgr_needle_oms_pc_right;
		delete mgr_needle_oms_he_left;
		delete mgr_needle_oms_he_right;
		delete mgr_needle_mps_pc_L;
		delete mgr_needle_mps_pc_C;
		delete mgr_needle_mps_pc_R;
		delete mgr_needle_mps_LO2;
		delete mgr_needle_mps_LH2;
		delete mgr_needle_mps_he_PNEU;
		delete mgr_needle_mps_he_L;
		delete mgr_needle_mps_he_C;
		delete mgr_needle_mps_he_R;
		delete mgr_needle_g_meter;
		delete mgr_needle_LOB;
		delete mgr_needle_LIB;
		delete mgr_needle_RIB;
		delete mgr_needle_ROB;
		delete mgr_needle_DAFB;
		delete mgr_needle_DRFB;
		delete mgr_needle_DBFOFB;
		delete mgr_needle_Speedbrake_Pos;
		delete mgr_needle_Speedbrake_Cmd;
	}

	void PanelF7::DefineVC()
	{
		AddAIDToMouseEventList(AID_F7);

		pCRT1->DefineVCGroup( GRP_CRT1_F7_VC );
		if (bHasMEDS) pCRT1->DefineMaterial( MAT_MDU_CRT1_F7_VC );
		else pCRT1->DefineMaterial(5);
		if (bHasMEDS) pCRT1->SetMouseRegion(AID_F7, 0.0f, 0.0f, 0.324694f, 0.490948f);
		else pCRT1->SetMouseRegion( AID_F7, 0.003304, 0.0, 0.370636, 0.473526);

		pCRT2->DefineVCGroup( GRP_CRT2_F7_VC );
		if (bHasMEDS) pCRT2->DefineMaterial( MAT_MDU_CRT2_F7_VC );
		else pCRT2->DefineMaterial(6);
		if (bHasMEDS) pCRT2->SetMouseRegion(AID_F7, 0.675193f, 0.0f, 1.0f, 0.490948f);
		else pCRT2->SetMouseRegion(AID_F7, 0.630505, 0.0, 0.997837, 0.473526);

		pCRT3->DefineVCGroup( GRP_CRT3_F7_VC );
		if (bHasMEDS) pCRT3->DefineMaterial( MAT_MDU_CRT3_F7_VC );
		else pCRT3->DefineMaterial(7);
		if (bHasMEDS) pCRT3->SetMouseRegion(AID_F7, 0.337995f, 0.257779f, 0.663350f, 0.750545f);
		else pCRT3->SetMouseRegion( AID_F7, 0.316554, 0.478625, 0.683886, 0.952131);

		pMFD1->DefineVCGroup( GRP_MFD1_F7_VC );
		if (bHasMEDS) pMFD1->DefineMaterial( MAT_MDU_MFD1_F7_VC );
		else pMFD1->DefineMaterial(9);
		if (bHasMEDS) pMFD1->SetMouseRegion(AID_F7, 0.0f, 0.510035f, 0.324694f, 1.0f);

		pMFD2->DefineVCGroup( GRP_MFD2_F7_VC );
		if (bHasMEDS) pMFD2->DefineMaterial( MAT_MDU_MFD2_F7_VC );
		else pMFD2->DefineMaterial(10);
		if (bHasMEDS) pMFD2->SetMouseRegion(AID_F7, 0.675193f, 0.510035f, 1.0f, 1.0f);

		pEventTime->DefineMesh( GetVCMeshIndex() );
		pEventTime->DefineComponent( GRP_EVENT_S1_F7_VC, true, false, false, _7SD_STATE_NUM0 );
		pEventTime->DefineComponent( GRP_EVENT_S10_F7_VC, true, false, false, _7SD_STATE_NUM0 );
		pEventTime->DefineComponent( GRP_EVENT_M1_F7_VC, true, false, false, _7SD_STATE_NUM0 );
		pEventTime->DefineComponent( GRP_EVENT_M10_F7_VC, true, false, false, _7SD_STATE_NUM0 );

		pCWLight[0]->DefineMeshGroup( GetVCMeshIndex(), GRP_CW_O2_PRESS_F7_VC );
		pCWLight[0]->SetStateOffset( 1, 0.0f, 0.177246f );
		pCWLight[1]->DefineMeshGroup( GetVCMeshIndex(), GRP_CW_CABIN_ATM_F7_VC );
		pCWLight[1]->SetStateOffset( 1, 0.0f, 0.177246f );
		pCWLight[2]->DefineMeshGroup( GetVCMeshIndex(), GRP_CW_FREON_LOOP_F7_VC );
		pCWLight[2]->SetStateOffset( 1, 0.0f, 0.177246f );
		pCWLight[3]->DefineMeshGroup( GetVCMeshIndex(), GRP_CW_H2O_LOOP_F7_VC );
		pCWLight[3]->SetStateOffset( 1, 0.0f, 0.177246f );
		pCWLight[4]->DefineMeshGroup( GetVCMeshIndex(), GRP_CW_BLANK_F7_VC );
		pCWLight[4]->SetStateOffset( 1, 0.0f, 0.177246f );
		pCWLight[5]->DefineMeshGroup( GetVCMeshIndex(), GRP_CW_PAYLOAD_WARNING_F7_VC );
		pCWLight[5]->SetStateOffset( 1, 0.0f, 0.177246f );
		pCWLight[6]->DefineMeshGroup( GetVCMeshIndex(), GRP_CW_PAYLOAD_CAUTION_F7_VC );
		pCWLight[6]->SetStateOffset( 1, 0.0f, 0.177246f );
		pCWLight[7]->DefineMeshGroup( GetVCMeshIndex(), GRP_CW_BACKUP_CW_ALARM_F7_VC );
		pCWLight[7]->SetStateOffset( 1, 0.0f, 0.177246f );

		pCWLight[8]->DefineMeshGroup( GetVCMeshIndex(), GRP_CW_H2_PRESS_F7_VC );
		pCWLight[8]->SetStateOffset( 1, 0.0f, 0.177246f );
		pCWLight[9]->DefineMeshGroup( GetVCMeshIndex(), GRP_CW_O2_HEATER_TEMP_F7_VC );
		pCWLight[9]->SetStateOffset( 1, 0.0f, 0.177246f );
		pCWLight[10]->DefineMeshGroup( GetVCMeshIndex(), GRP_CW_AV_BAY_CABIN_AIR_F7_VC );
		pCWLight[10]->SetStateOffset( 1, 0.0f, 0.177246f );
		pCWLight[11]->DefineMeshGroup( GetVCMeshIndex(), GRP_CW_RGA_ACCEL_F7_VC );
		pCWLight[11]->SetStateOffset( 1, 0.0f, 0.177246f );
		pCWLight[12]->DefineMeshGroup( GetVCMeshIndex(), GRP_CW_LEFT_RHC_F7_VC );
		pCWLight[12]->SetStateOffset( 1, 0.0f, 0.177246f );
		pCWLight[13]->DefineMeshGroup( GetVCMeshIndex(), GRP_CW_GPC_F7_VC );
		pCWLight[13]->SetStateOffset( 1, 0.0f, 0.177246f );
		pCWLight[14]->DefineMeshGroup( GetVCMeshIndex(), GRP_CW_PRIMARY_CW_F7_VC );
		pCWLight[14]->SetStateOffset( 1, 0.0f, 0.177246f );
		pCWLight[15]->DefineMeshGroup( GetVCMeshIndex(), GRP_CW_APU_TEMP_F7_VC );
		pCWLight[15]->SetStateOffset( 1, 0.0f, 0.177246f );

		pCWLight[16]->DefineMeshGroup( GetVCMeshIndex(), GRP_CW_FUEL_CELL_REAC_F7_VC );
		pCWLight[16]->SetStateOffset( 1, 0.0f, 0.177246f );
		pCWLight[17]->DefineMeshGroup( GetVCMeshIndex(), GRP_CW_MAIN_BUS_UNDERVOLT_F7_VC );
		pCWLight[17]->SetStateOffset( 1, 0.0f, 0.177246f );
		pCWLight[18]->DefineMeshGroup( GetVCMeshIndex(), GRP_CW_IMU_F7_VC );
		pCWLight[18]->SetStateOffset( 1, 0.0f, 0.177246f );
		pCWLight[19]->DefineMeshGroup( GetVCMeshIndex(), GRP_CW_AIR_DATA_F7_VC );
		pCWLight[19]->SetStateOffset( 1, 0.0f, 0.177246f );
		pCWLight[20]->DefineMeshGroup( GetVCMeshIndex(), GRP_CW_RIGHT_AFT_RHC_F7_VC );
		pCWLight[20]->SetStateOffset( 1, 0.0f, 0.177246f );
		pCWLight[21]->DefineMeshGroup( GetVCMeshIndex(), GRP_CW_FCS_SATURATION_F7_VC );
		pCWLight[21]->SetStateOffset( 1, 0.0f, 0.177246f );
		pCWLight[22]->DefineMeshGroup( GetVCMeshIndex(), GRP_CW_FCS_CHANNEL_F7_VC );
		pCWLight[22]->SetStateOffset( 1, 0.0f, 0.177246f );
		pCWLight[23]->DefineMeshGroup( GetVCMeshIndex(), GRP_CW_APU_OVERSPEED_F7_VC );
		pCWLight[23]->SetStateOffset( 1, 0.0f, 0.177246f );

		pCWLight[24]->DefineMeshGroup( GetVCMeshIndex(), GRP_CW_FUEL_CELL_STACK_TEMP_F7_VC );
		pCWLight[24]->SetStateOffset( 1, 0.0f, 0.177246f );
		pCWLight[25]->DefineMeshGroup( GetVCMeshIndex(), GRP_CW_AC_VOLTAGE_F7_VC );
		pCWLight[25]->SetStateOffset( 1, 0.0f, 0.177246f );
		pCWLight[26]->DefineMeshGroup( GetVCMeshIndex(), GRP_CW_FWD_RCS_F7_VC );
		pCWLight[26]->SetStateOffset( 1, 0.0f, 0.177246f );
		pCWLight[27]->DefineMeshGroup( GetVCMeshIndex(), GRP_CW_LEFT_RCS_F7_VC );
		pCWLight[27]->SetStateOffset( 1, 0.0f, 0.177246f );
		pCWLight[28]->DefineMeshGroup( GetVCMeshIndex(), GRP_CW_LEFT_OMS_F7_VC );
		pCWLight[28]->SetStateOffset( 1, 0.0f, 0.177246f );
		pCWLight[29]->DefineMeshGroup( GetVCMeshIndex(), GRP_CW_OMS_KIT_F7_VC );
		pCWLight[29]->SetStateOffset( 1, 0.0f, 0.177246f );
		pCWLight[30]->DefineMeshGroup( GetVCMeshIndex(), GRP_CW_MPS_F7_VC );
		pCWLight[30]->SetStateOffset( 1, 0.0f, 0.177246f );
		pCWLight[31]->DefineMeshGroup( GetVCMeshIndex(), GRP_CW_APU_UNDERSPEED_F7_VC );
		pCWLight[31]->SetStateOffset( 1, 0.0f, 0.177246f );

		pCWLight[32]->DefineMeshGroup( GetVCMeshIndex(), GRP_CW_FUEL_CELL_PUMP_F7_VC );
		pCWLight[32]->SetStateOffset( 1, 0.0f, 0.177246f );
		pCWLight[33]->DefineMeshGroup( GetVCMeshIndex(), GRP_CW_AC_OVERLOAD_F7_VC );
		pCWLight[33]->SetStateOffset( 1, 0.0f, 0.177246f );
		pCWLight[34]->DefineMeshGroup( GetVCMeshIndex(), GRP_CW_RCS_JET_F7_VC );
		pCWLight[34]->SetStateOffset( 1, 0.0f, 0.177246f );
		pCWLight[35]->DefineMeshGroup( GetVCMeshIndex(), GRP_CW_RIGHT_RCS_F7_VC );
		pCWLight[35]->SetStateOffset( 1, 0.0f, 0.177246f );
		pCWLight[36]->DefineMeshGroup( GetVCMeshIndex(), GRP_CW_RIGHT_OMS_F7_VC );
		pCWLight[36]->SetStateOffset( 1, 0.0f, 0.177246f );
		pCWLight[37]->DefineMeshGroup( GetVCMeshIndex(), GRP_CW_OMS_TVC_F7_VC );
		pCWLight[37]->SetStateOffset( 1, 0.0f, 0.177246f );
		pCWLight[38]->DefineMeshGroup( GetVCMeshIndex(), GRP_CW_BLANK2_F7_VC );
		pCWLight[38]->SetStateOffset( 1, 0.0f, 0.177246f );
		pCWLight[39]->DefineMeshGroup( GetVCMeshIndex(), GRP_CW_HYD_PRESS_F7_VC );
		pCWLight[39]->SetStateOffset( 1, 0.0f, 0.177246f );

		pMainEngStatus[0]->DefineMeshGroup( GetVCMeshIndex(), GRP_XDS1_F7_VC );
		pMainEngStatus[0]->SetStateOffset( 1, 0.120117f, 0.344727f );// R
		pMainEngStatus[0]->SetStateOffset( 2, 0.120117f, 0.478516f );// Y
		pMainEngStatus[0]->SetStateOffset( 3, 0.120117f, 0.619141f );// RY

		pMainEngStatus[1]->DefineMeshGroup( GetVCMeshIndex(), GRP_XDS2_F7_VC );
		pMainEngStatus[1]->SetStateOffset( 1, 0.120117f, 0.344727f );// R
		pMainEngStatus[1]->SetStateOffset( 2, 0.120117f, 0.478516f );// Y
		pMainEngStatus[1]->SetStateOffset( 3, 0.120117f, 0.619141f );// RY

		pMainEngStatus[2]->DefineMeshGroup( GetVCMeshIndex(), GRP_XDS3_F7_VC );
		pMainEngStatus[2]->SetStateOffset( 1, 0.120117f, 0.344727f );// R
		pMainEngStatus[2]->SetStateOffset( 2, 0.120117f, 0.478516f );// Y
		pMainEngStatus[2]->SetStateOffset( 3, 0.120117f, 0.619141f );// RY

		pSMAlert->DefineMeshGroup( GetVCMeshIndex(), GRP_XDS4_F7_VC );
		pSMAlert->SetStateOffset( 1, 0.120117f, 0.619141f );

		if (!bHasMEDS)
		{
			VECTOR3 switch_rot = _V(1, 0, 0);
			VECTOR3 push_dir = _V(0.0, -0.258681, 0.965963);
			VECTOR3 pull_dir = -push_dir;

			pFltCntlrPower->DefineGroup(70);
			pFltCntlrPower->SetInitialAnimState(0.5);
			pFltCntlrPower->SetReference(_V(-0.363014, 1.97877, 14.6817), switch_rot);
			pFltCntlrPower->SetMouseRegion(AID_F7, 0.0274, 0.9065, 0.0557, 0.9692);
			pFltCntlrPower->SetPullDirection(pull_dir);

			pRDRALTM->DefineGroup(71);
			pRDRALTM->SetInitialAnimState(0.5);
			pRDRALTM->SetReference(_V(-0.330845, 1.97951, 14.6821), _V(1, 0, 0));
			pRDRALTM->SetMouseRegion(AID_F7, 0.0686f, 0.9065f, 0.1026f, 0.9692f);

			DefineVCAnimations();
		}

		return;
	}

	void PanelF7::RegisterVC()
	{
		AtlantisPanel::RegisterVC();
		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;
		oapiVCRegisterArea(AID_F7, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED);

		if (bHasMEDS)
		{
			oapiVCSetAreaClickmode_Quadrilateral(AID_F7,
				_V(-0.3879, 2.4088, 14.7837) + ofs, _V(0.385, 2.4088, 14.7837) + ofs,
				_V(-0.3879, 1.9679, 14.6604) + ofs, _V(0.385, 1.9679, 14.6604) + ofs);
		}
		else
		{
			oapiVCSetAreaClickmode_Quadrilateral(AID_F7,
				_V(-0.397143, 2.390688, 14.797020) + ofs, _V(0.391856, 2.390688, 14.797020) + ofs,
				_V(-0.397143, 1.951031, 14.673615) + ofs, _V(0.391856, 1.951031, 14.673615) + ofs);
		}

		return;
	}

	void PanelF7::Realize()
	{
		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle( "CW_ANNUNCIATOR_A", 16 );
		for (int i = 0; i < 16; i++) lightcmd[i].Connect( pBundle, i );

		pBundle = STS()->BundleManager()->CreateBundle( "CW_ANNUNCIATOR_B", 16 );
		for (int i = 16; i < 32; i++) lightcmd[i].Connect( pBundle, i - 16 );

		pBundle = STS()->BundleManager()->CreateBundle( "CW_ANNUNCIATOR_C", 16 );
		for (int i = 32; i < 40; i++) lightcmd[i].Connect( pBundle, i - 32 );

		pBundle = STS()->BundleManager()->CreateBundle( "F7LIGHTS_A", 16 );
		for (int i = 0; i < 16; i++)
		{
			pCWLight[i]->ConnectLight( 0, pBundle, i );
			lightoutput[i].Connect( pBundle, i );
		}

		pBundle = STS()->BundleManager()->CreateBundle( "F7LIGHTS_B", 16 );
		for (int i = 16; i < 32; i++)
		{
			pCWLight[i]->ConnectLight( 0, pBundle, i - 16 );
			lightoutput[i].Connect( pBundle, i - 16 );
		}

		pBundle = STS()->BundleManager()->CreateBundle( "F7LIGHTS_C", 16 );
		for (int i = 32; i < 40; i++)
		{
			pCWLight[i]->ConnectLight( 0, pBundle, i - 32 );
			lightoutput[i].Connect( pBundle, i - 32 );
		}

		pBundle = STS()->BundleManager()->CreateBundle( "ACA", 16 );
		lighttestleft[0].Connect( pBundle, 6 );
		lighttestright[0].Connect( pBundle, 7 );
		lighttestleft[1].Connect( pBundle, 8 );
		lighttestright[1].Connect( pBundle, 9 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA1_1", 16 );
		pMainEngStatus[1]->ConnectLight( 0, pBundle, 15 );
		pBundle = STS()->BundleManager()->CreateBundle( "ACA1_2", 16 );
		pMainEngStatus[1]->ConnectLight( 2, pBundle, 15 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA2_4", 16 );
		pSMAlert->ConnectLight( 0, pBundle, 9 );
		pMainEngStatus[2]->ConnectLight( 0, pBundle, 15 );
		pBundle = STS()->BundleManager()->CreateBundle( "ACA2_5", 16 );
		pMainEngStatus[2]->ConnectLight( 2, pBundle, 11 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA3_1", 16 );
		pMainEngStatus[0]->ConnectLight( 0, pBundle, 15 );
		pBundle = STS()->BundleManager()->CreateBundle( "ACA3_2", 16 );
		pMainEngStatus[0]->ConnectLight( 2, pBundle, 15 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA3_4", 16 );
		pSMAlert->ConnectLight( 1, pBundle, 9 );

		pBundle = STS()->BundleManager()->CreateBundle( "FwdEventTimer_A", 16 );
		pEventTime->Mode_Test.Connect( pBundle, 0 );
		pEventTime->Mode_Up.Connect( pBundle, 1 );
		pEventTime->Control_Stop.Connect( pBundle, 2 );
		pEventTime->Control_Start.Connect( pBundle, 3 );
		pEventTime->Timer_Reset.Connect( pBundle, 4 );
		pEventTime->Timer_Set.Connect( pBundle, 5 );
		pEventTime->AbortReset.Connect( pBundle, 6 );
		pEventTime->LiftoffStart.Connect( pBundle, 7 );

		pBundle = STS()->BundleManager()->CreateBundle( "FwdEventTimer_B", 16 );
		pEventTime->Seconds_1.Connect( pBundle, 0 );
		pEventTime->Seconds_2.Connect( pBundle, 1 );
		pEventTime->Seconds_4.Connect( pBundle, 2 );
		pEventTime->Seconds_8.Connect( pBundle, 3 );
		pEventTime->Seconds_10.Connect( pBundle, 4 );
		pEventTime->Seconds_20.Connect( pBundle, 5 );
		pEventTime->Seconds_40.Connect( pBundle, 6 );
		pEventTime->Minutes_1.Connect( pBundle, 7 );
		pEventTime->Minutes_2.Connect( pBundle, 8 );
		pEventTime->Minutes_4.Connect( pBundle, 9 );
		pEventTime->Minutes_8.Connect( pBundle, 10 );
		pEventTime->Minutes_10.Connect( pBundle, 11 );
		pEventTime->Minutes_20.Connect( pBundle, 12 );
		pEventTime->Minutes_40.Connect( pBundle, 13 );

		pBundle = STS()->BundleManager()->CreateBundle( "CW_SW_2", 16 );
		modeascent.Connect( pBundle, 6 );
		modenorm.Connect( pBundle, 7 );
		mar[0].Connect( pBundle, 9 );
		mar[1].Connect( pBundle, 10 );
		cw_a.Connect( pBundle, 11 );
		AtlantisPanel::Realize();

		if (!bHasMEDS)
		{
			pBundle = STS()->BundleManager()->CreateBundle("DDU_POWER", 16);
			pFltCntlrPower->ConnectPort(1, pBundle, 2);

			pBundle = STS()->BundleManager()->CreateBundle("RDRALTM", 16);
			pRDRALTM->ConnectPort(1, pBundle, 0);// CDR RDR ALTM
		}
	}

	void PanelF7::OnPostStep( double simt, double simdt, double mjd )
	{
		AtlantisPanel::OnPostStep( simt, simdt, mjd );

		if (!bHasMEDS)
		{
			bool power = (mar[0] || mar[1] || modenorm || modeascent) && cw_a;
			bool testleft = lighttestleft[0].IsSet() || lighttestleft[1].IsSet();
			bool testright = lighttestright[0].IsSet() || lighttestright[1].IsSet();

			for (int i = 0; i < 20; i++)
			{
				lightoutput[i].SetLine((int)(power && (lightcmd[i].IsSet() || testleft)) * 5.0f);
			}
			for (int i = 20; i < 40; i++)
			{
				lightoutput[i].SetLine((int)(power && (lightcmd[i].IsSet() || testright)) * 5.0f);
			}

			unsigned short PC_OMS_L;
			unsigned short PC_OMS_R;
			unsigned short He_L;
			unsigned short He_R;
			unsigned short N2_L;
			unsigned short N2_R;
			pCRT1->GetIDP()->GetOMSdata(PC_OMS_L, PC_OMS_R, He_L, He_R, N2_L, N2_R);

			unsigned short PC_C;
			unsigned short PC_L;
			unsigned short PC_R;
			unsigned short HeTk_C;
			unsigned short HeTk_L;
			unsigned short HeTk_R;
			unsigned short HeTk_Pneu;
			unsigned short HeReg_C;
			unsigned short HeReg_L;
			unsigned short HeReg_R;
			unsigned short HeReg_Pneu;
			unsigned short LH2_Manif;
			unsigned short LO2_Manif;
			pCRT1->GetIDP()->GetMPSdata(PC_C, PC_L, PC_R, HeTk_C, HeTk_L, HeTk_R, HeTk_Pneu, HeReg_C, HeReg_L, HeReg_R, HeReg_Pneu, LH2_Manif, LO2_Manif);

			//OMS Gauges Animation
			if (PC_OMS_L > 150.0) PC_OMS_L = 150.0;
			if (PC_OMS_R > 150.0) PC_OMS_R = 150.0;
			if (He_L > 5000.0) He_L = 5000.0;
			if (He_R > 5000.0) He_R = 5000.0;
			if (N2_L > 5000.0) N2_L = 5000.0;
			if (N2_R > 5000.0) N2_R = 5000.0;

			STS()->SetAnimation(anim_needle_oms_pc_left, PC_OMS_L / 150.0);
			STS()->SetAnimation(anim_needle_oms_pc_right, PC_OMS_R / 150.0);

			STS()->SetAnimation(anim_needle_oms_he_left, He_L / 5000.0);
			STS()->SetAnimation(anim_needle_oms_he_right, He_R / 5000.0);

			//MPS Gauges Animation
			if (PC_C > 115.0) PC_C = 115.0;
			if (PC_L > 115.0) PC_L = 115.0;
			if (PC_R > 115.0) PC_R = 115.0;

			if (LH2_Manif > 100.0) LH2_Manif = 100.0;
			if (LO2_Manif > 300.0) LO2_Manif = 300.0;

			if (HeReg_Pneu > 5000) HeReg_Pneu = 5000.0;
			if (HeReg_C > 5000.0) HeReg_C = 5000.0;
			if (HeReg_L > 5000.0) HeReg_L = 5000.0;
			if (HeReg_R > 5000.0) HeReg_R = 5000.0;

			if (HeTk_Pneu > 5000.0) HeTk_Pneu = 5000.0;
			if (HeTk_C > 5000.0) HeTk_C = 5000.0;
			if (HeTk_L > 5000.0) HeTk_L = 5000.0;
			if (HeTk_R > 5000.0) HeTk_R = 5000.0;

			STS()->SetAnimation(anim_needle_mps_pc_C, PC_C / 115.0);
			STS()->SetAnimation(anim_needle_mps_pc_L, PC_L / 115.0);
			STS()->SetAnimation(anim_needle_mps_pc_R, PC_R / 115.0);

			STS()->SetAnimation(anim_needle_mps_LH2, LH2_Manif / 100.0);
			STS()->SetAnimation(anim_needle_mps_LO2, LO2_Manif / 300.0);

			STS()->SetAnimation(anim_needle_mps_he_PNEU, HeReg_Pneu / 5000.0);
			STS()->SetAnimation(anim_needle_mps_he_C, HeReg_C / 5000.0);
			STS()->SetAnimation(anim_needle_mps_he_L, HeReg_L / 5000.0);
			STS()->SetAnimation(anim_needle_mps_he_R, HeReg_R / 5000.0);

			//G-Meter
			VECTOR3 f;
			STS()->GetForceVector(f);
			double dtmp = (f.z / (STS()->GetMass() * G)) + sin(STS()->GetPitch());

			if (dtmp > 4.0) dtmp = 4.0;
			else if (dtmp < -2.0) dtmp = -2.0;

			STS()->SetAnimation(anim_needle_g_meter, (dtmp + 2.0) / 6.0);

			//SPI
			double LOB;
			double LIB;
			double RIB;
			double ROB;
			double DAFB;
			double DRFB;
			double DBFOFB;
			double Speedbrake_Pos;
			double Speedbrake_Cmd;
			pCRT1->GetIDP()->GetAerosurfacePositions(LOB, LIB, RIB, ROB, DAFB, DRFB, DBFOFB, Speedbrake_Pos, Speedbrake_Cmd);

			//SPI Needles animation
			if (LOB > 20.0) LOB = 20.0;
			else if (LOB < -35.0) LOB = -35.0;

			if (LIB > 20.0) LIB = 20.0;
			else if (LIB < -35.0) LIB = -35.0;

			if (ROB > 20.0) ROB = 20.0;
			else if (ROB < -35.0) ROB = -35.0;

			if (RIB > 20.0) RIB = 20.0;
			else if (RIB < -35.0) RIB = -35.0;

			if (DBFOFB > 100.0) DBFOFB = 100.0;
			else if (DBFOFB < 0.0) DBFOFB = 0.0;

			if (DRFB > 30.0) DRFB = 30.0;
			else if (DRFB < -30.0) DRFB = -30.0;

			if (DAFB > 5.0) DAFB = 5.0;
			else if (DAFB < -5.0) DAFB = -5.0;

			if (Speedbrake_Pos > 100.0) Speedbrake_Pos = 100.0;
			else if (Speedbrake_Pos < 0.0) Speedbrake_Pos = 0.0;

			if (Speedbrake_Cmd > 100.0) Speedbrake_Cmd = 100.0;
			else if (Speedbrake_Cmd < 0.0) Speedbrake_Cmd = 0.0;

			STS()->SetAnimation(anim_needle_LOB, (20.0 - LOB) / 55.0);
			STS()->SetAnimation(anim_needle_LIB, (20.0 - LIB) / 55.0);
			STS()->SetAnimation(anim_needle_ROB, (20.0 - ROB) / 55.0);
			STS()->SetAnimation(anim_needle_RIB, (20.0 - RIB) / 55.0);

			STS()->SetAnimation(anim_needle_DBFOFB, DBFOFB / 100);

			STS()->SetAnimation(anim_needle_DRFB, (DRFB + 30.0) / 60.0);

			STS()->SetAnimation(anim_needle_DAFB, (5.0 - DAFB) / 10.0);

			STS()->SetAnimation(anim_needle_Speedbrake_Pos, Speedbrake_Pos / 100);
			STS()->SetAnimation(anim_needle_Speedbrake_Cmd, Speedbrake_Cmd / 100);
		}

		return;
	}

	void PanelF7::DefineVCAnimations()
	{
		//OMS Press
		//Pc
		static UINT needle_oms_pc_left[1] = { 58 };
		static UINT needle_oms_pc_right[1] = { 59 };
		mgr_needle_oms_pc_left = new MGROUP_ROTATE(GetVCMeshIndex(), needle_oms_pc_left, 1, _V(-0.358495, 2.017144, 14.927303), _V(1,0,0), (float)(RAD * 16.35));
		mgr_needle_oms_pc_right = new MGROUP_ROTATE(GetVCMeshIndex(), needle_oms_pc_right, 1, _V(-0.358495, 2.017144, 14.927303), _V(1, 0, 0), (float)(RAD * 16.35));
		anim_needle_oms_pc_left = STS()->CreateAnimation(0.0);
		anim_needle_oms_pc_right = STS()->CreateAnimation(0.0);
		STS()->AddAnimationComponent(anim_needle_oms_pc_left, 0, 1, mgr_needle_oms_pc_left);
		STS()->AddAnimationComponent(anim_needle_oms_pc_right, 0, 1, mgr_needle_oms_pc_right);

		//He/N2
		static UINT needle_oms_he_left[1] = { 58 };
		static UINT needle_oms_he_right[1] = { 59 };
		mgr_needle_oms_he_left = new MGROUP_ROTATE(GetVCMeshIndex(), needle_oms_he_left, 1, _V(-0.303327, 2.01714, 14.9273), _V(1, 0, 0), (float)(RAD * 17.4));
		mgr_needle_oms_he_right = new MGROUP_ROTATE(GetVCMeshIndex(), needle_oms_he_right, 1, _V(-0.303327, 2.01714, 14.9273), _V(1, 0, 0), (float)(RAD * 17.4));
		anim_needle_oms_he_left = STS()->CreateAnimation(0.0);
		anim_needle_oms_he_right = STS()->CreateAnimation(0.0);
		STS()->AddAnimationComponent(anim_needle_oms_he_left, 0, 1, mgr_needle_oms_he_left);
		STS()->AddAnimationComponent(anim_needle_oms_he_right, 0, 1, mgr_needle_oms_he_right);

		//MPS Press
		//Pc
		static UINT needle_mps_pc_L[1] = { 62 };
		static UINT needle_mps_pc_C[2] = { 63, 64 };
		static UINT needle_mps_pc_R[1] = { 65 };
		mgr_needle_mps_pc_L = new MGROUP_TRANSLATE(GetVCMeshIndex(), needle_mps_pc_L, 1, _V(0.0, 0.068063, 0.018901));
		mgr_needle_mps_pc_C = new MGROUP_TRANSLATE(GetVCMeshIndex(), needle_mps_pc_C, 2, _V(0.0, 0.068063, 0.018901));
		mgr_needle_mps_pc_R = new MGROUP_TRANSLATE(GetVCMeshIndex(), needle_mps_pc_R, 1, _V(0.0, 0.068063, 0.018901));
		anim_needle_mps_pc_L = STS()->CreateAnimation(0.0);
		anim_needle_mps_pc_C = STS()->CreateAnimation(0.0);
		anim_needle_mps_pc_R = STS()->CreateAnimation(0.0);
		STS()->AddAnimationComponent(anim_needle_mps_pc_L, 0, 1, mgr_needle_mps_pc_L);
		STS()->AddAnimationComponent(anim_needle_mps_pc_C, 0, 1, mgr_needle_mps_pc_C);
		STS()->AddAnimationComponent(anim_needle_mps_pc_R, 0, 1, mgr_needle_mps_pc_R);

		//LO2
		static UINT needle_mps_LO2[1] = { 66 };
		mgr_needle_mps_LO2 = new MGROUP_TRANSLATE(GetVCMeshIndex(), needle_mps_LO2, 1, _V(0.0, 0.063708, 0.017692));
		anim_needle_mps_LO2 = STS()->CreateAnimation(0.0);
		STS()->AddAnimationComponent(anim_needle_mps_LO2, 0, 1, mgr_needle_mps_LO2);

		//LH2
		static UINT needle_mps_LH2[1] = { 67 };
		mgr_needle_mps_LH2 = new MGROUP_TRANSLATE(GetVCMeshIndex(), needle_mps_LH2, 1, _V(0.0, 0.065329, 0.018142));
		anim_needle_mps_LH2 = STS()->CreateAnimation(0.0);
		STS()->AddAnimationComponent(anim_needle_mps_LH2, 0, 1, mgr_needle_mps_LH2);

		//He
		static UINT needle_mps_he_PNEU[1] = { 91 };
		static UINT needle_mps_he_L[1] = { 68 };
		static UINT needle_mps_he_C[1] = { 69 };
		static UINT needle_mps_he_R[1] = { 92 };
		mgr_needle_mps_he_PNEU = new MGROUP_TRANSLATE(GetVCMeshIndex(), needle_mps_he_PNEU, 1, _V(0.0, 0.065589, 0.018057));
		mgr_needle_mps_he_L = new MGROUP_TRANSLATE(GetVCMeshIndex(), needle_mps_he_L, 1, _V(0.0, 0.065589, 0.018057));
		mgr_needle_mps_he_C = new MGROUP_TRANSLATE(GetVCMeshIndex(), needle_mps_he_C, 1, _V(0.0, 0.065589, 0.018057));
		mgr_needle_mps_he_R = new MGROUP_TRANSLATE(GetVCMeshIndex(), needle_mps_he_R, 1, _V(0.0, 0.065589, 0.018057));
		anim_needle_mps_he_PNEU = STS()->CreateAnimation(0.0);
		anim_needle_mps_he_L = STS()->CreateAnimation(0.0);
		anim_needle_mps_he_C = STS()->CreateAnimation(0.0);
		anim_needle_mps_he_R = STS()->CreateAnimation(0.0);
		STS()->AddAnimationComponent(anim_needle_mps_he_PNEU, 0, 1, mgr_needle_mps_he_PNEU);
		STS()->AddAnimationComponent(anim_needle_mps_he_L, 0, 1, mgr_needle_mps_he_L);
		STS()->AddAnimationComponent(anim_needle_mps_he_C, 0, 1, mgr_needle_mps_he_C);
		STS()->AddAnimationComponent(anim_needle_mps_he_R, 0, 1, mgr_needle_mps_he_R);

		//G-Meter
		static UINT needle_g_meter[1] = { 78 };
		mgr_needle_g_meter = new MGROUP_ROTATE(GetVCMeshIndex(), needle_g_meter, 1, _V(-0.21095, 1.99888, 14.6831), _V(0.000000, -0.273178, 0.961963), (float)(RAD * -323.23));
		anim_needle_g_meter = STS()->CreateAnimation(0.0);
		STS()->AddAnimationComponent(anim_needle_g_meter, 0, 1, mgr_needle_g_meter);

		//SPI
		//LOB
		static UINT needle_LOB[1] = { 79 };
		mgr_needle_LOB = new MGROUP_TRANSLATE(GetVCMeshIndex(), needle_LOB, 1, _V(0.0, 0.069643, 0.01934));
		anim_needle_LOB = STS()->CreateAnimation(0.0);
		STS()->AddAnimationComponent(anim_needle_LOB, 0, 1, mgr_needle_LOB);

		//LIB
		static UINT needle_LIB[1] = { 80 };
		mgr_needle_LIB = new MGROUP_TRANSLATE(GetVCMeshIndex(), needle_LIB, 1, _V(0.0, 0.069643, 0.01934));
		anim_needle_LIB = STS()->CreateAnimation(0.0);
		STS()->AddAnimationComponent(anim_needle_LIB, 0, 1, mgr_needle_LIB);

		//ROB
		static UINT needle_ROB[1] = { 82 };
		mgr_needle_ROB = new MGROUP_TRANSLATE(GetVCMeshIndex(), needle_ROB, 1, _V(0.0, 0.069643, 0.01934));
		anim_needle_ROB = STS()->CreateAnimation(0.0);
		STS()->AddAnimationComponent(anim_needle_ROB, 0, 1, mgr_needle_ROB);

		//RIB
		static UINT needle_RIB[1] = { 81 };
		mgr_needle_RIB = new MGROUP_TRANSLATE(GetVCMeshIndex(), needle_RIB, 1, _V(0.0, 0.069643, 0.01934));
		anim_needle_RIB = STS()->CreateAnimation(0.0);
		STS()->AddAnimationComponent(anim_needle_RIB, 0, 1, mgr_needle_RIB);

		//DBFOFB
		static UINT needle_DBFOFB[1] = { 83 };
		mgr_needle_DBFOFB = new MGROUP_TRANSLATE(GetVCMeshIndex(), needle_DBFOFB, 1, _V(0.0, -0.069765, -0.019374));
		anim_needle_DBFOFB = STS()->CreateAnimation(0.0);
		STS()->AddAnimationComponent(anim_needle_DBFOFB, 0, 1, mgr_needle_DBFOFB);

		//DRFB
		static UINT needle_DRFB[1] = { 84 };
		mgr_needle_DRFB = new MGROUP_TRANSLATE(GetVCMeshIndex(), needle_DRFB, 1, _V(0.03182, 0.0, 0.0));
		anim_needle_DRFB = STS()->CreateAnimation(0.5);
		STS()->AddAnimationComponent(anim_needle_DRFB, 0, 1, mgr_needle_DRFB);

		//DAFB
		static UINT needle_DAFB[1] = { 85 };
		mgr_needle_DAFB = new MGROUP_TRANSLATE(GetVCMeshIndex(), needle_DAFB, 1, _V(0.032573, 0.0, 0.0));
		anim_needle_DAFB = STS()->CreateAnimation(0.5);
		STS()->AddAnimationComponent(anim_needle_DAFB, 0, 1, mgr_needle_DAFB);

		//Speedbrake_Pos
		static UINT needle_Speedbrake_Pos[1] = { 86 };
		mgr_needle_Speedbrake_Pos = new MGROUP_TRANSLATE(GetVCMeshIndex(), needle_Speedbrake_Pos, 1, _V(0.064782, 0.0, 0.0));
		anim_needle_Speedbrake_Pos = STS()->CreateAnimation(0.0);
		STS()->AddAnimationComponent(anim_needle_Speedbrake_Pos, 0, 1, mgr_needle_Speedbrake_Pos);

		//Speedbrake_Pos
		static UINT needle_Speedbrake_Cmd[1] = { 87 };
		mgr_needle_Speedbrake_Cmd = new MGROUP_TRANSLATE(GetVCMeshIndex(), needle_Speedbrake_Cmd, 1, _V(0.064782, 0.0, 0.0));
		anim_needle_Speedbrake_Cmd = STS()->CreateAnimation(0.0);
		STS()->AddAnimationComponent(anim_needle_Speedbrake_Cmd, 0, 1, mgr_needle_Speedbrake_Cmd);


	}
}

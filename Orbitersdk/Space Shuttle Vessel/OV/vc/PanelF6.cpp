/******* SSV File Modification Notice *******
Date         Developer
2020/05/10   GLS
2020/06/20   GLS
2020/06/23   GLS
2021/06/02   GLS
2021/08/24   GLS
2022/03/24   GLS
2022/03/26   GLS
2022/04/20   GLS
2022/05/29   GLS
2022/08/05   GLS
2022/08/27   GLS
2022/09/29   GLS
2024/12/30   GLS
********************************************/
#include "PanelF6.h"
#include "MDU.h"
#include "StandardSwitch.h"
#include "Talkback.h"
#include "PushButtonIndicator.h"
#include "StandardSwitchCover.h"
#include "StandardLight.h"
#include "RotarySwitchPotentiometer.h"
#include "../Atlantis.h"
#include "../meshres_vc.h"
#include "../ParameterValues.h"
#include "vc_defs.h"
#include "../meshres_vc_f6.h"
#include "../dps/IDP.h"


namespace vc {

	PanelF6::PanelF6(Atlantis* _sts, bool hasMEDS)
		: AtlantisPanel(_sts, "F6")
	{
		bHasMEDS = hasMEDS;

		if (bHasMEDS) DefineMesh(MESHNAME_PANELF6);
		else DefineMesh( MESHNAME_PANELF6_MCD );

		Add(pCDR1 = new MDU(_sts, "CDR1", MDUID_CDR1));
		Add(pCDR2 = new MDU(_sts, "CDR2", MDUID_CDR2));
		if (bHasMEDS) Add(pFltCntlrPower = new LockableLever2(_sts, "FLT CNTLR POWER"));
		Add( pADIAttitude = new StdSwitch3( _sts, "ADI ATTITUDE" ) );
		Add( pADIError = new StdSwitch3( _sts, "ADI ERROR" ) );
		Add( pADIRate = new StdSwitch3( _sts, "ADI RATE" ) );

		if (bHasMEDS)
		{
			pFltCntlrPower->SetLabel(0, "OFF");
			pFltCntlrPower->SetLabel(1, "ON");
		}
		pADIAttitude->SetLabel( 0, "REF" );
		pADIAttitude->SetLabel( 1, "LVLH" );
		pADIAttitude->SetLabel( 2, "INRTL" );
		pADIError->SetLabel( 0, "LOW" );
		pADIError->SetLabel( 1, "MED" );
		pADIError->SetLabel( 2, "HIGH" );
		pADIRate->SetLabel( 0, "LOW" );
		pADIRate->SetLabel( 1, "MED" );
		pADIRate->SetLabel( 2, "HIGH" );

		Add( pLandingGearTB[0] = new StandardTalkback3( _sts, "LANDING GEAR NOSE" ) );
		Add( pLandingGearTB[1] = new StandardTalkback3( _sts, "LANDING GEAR LEFT" ) );
		Add( pLandingGearTB[2] = new StandardTalkback3( _sts, "LANDING GEAR RIGHT" ) );

		Add( pLandingGearArmDeployCover[0] = new StandardSwitchCover( _sts, "LANDING GEAR ARM COVER" ) );
		Add( pLandingGearArmDeployCover[1] = new StandardSwitchCover( _sts, "LANDING GEAR DN COVER" ) );

		Add( pLandingGearArmDeploy[0] = new PushButtonIndicatorSingleLight( _sts, "LANDING GEAR ARM" ) );
		Add( pLandingGearArmDeploy[1] = new PushButtonIndicatorSingleLight( _sts, "LANDING GEAR DN" ) );

		Add( pRCSCommand[0] = new StandardDoubleLight( _sts, "RCS COMMAND ROLL" ) );
		Add( pRCSCommand[1] = new StandardDoubleLight( _sts, "RCS COMMAND PITCH" ) );
		Add( pRCSCommand[2] = new StandardDoubleLight( _sts, "RCS COMMAND YAW" ) );

		Add( pRangeSafeArm = new StandardDoubleLight( _sts, "RANGE SAFE ARM" ) );// using double light because of 4 inputs

		Add( pAbort = new PushButtonIndicatorSingleLight( _sts, "ABORT" ) );

		Add( pHUDMode = new StdSwitch3( _sts, "HUD MODE" ) );
		Add( pHUDBrightness = new RotarySwitchPotentiometer( _sts, "HUD BRIGHTNESS" ) );
		Add( pHUDBright = new StdSwitch3( _sts, "HUD BRIGHT" ) );

		pHUDMode->SetLabel( 0, "DCLT" );
		pHUDMode->SetLabel( 1, "NORM" );
		pHUDMode->SetLabel( 2, "TEST" );
		pHUDMode->SetSpringLoaded( true, 0 );

		pHUDBright->SetLabel( 0, "MAN NIGHT" );
		pHUDBright->SetLabel( 1, "AUTO" );
		pHUDBright->SetLabel( 2, "MAN DAY" );

		if (bHasMEDS)
		{
			Add( pRDRALTM = new StdSwitch2( _sts, "RDR ALTM" ) );
			pRDRALTM->SetLabel( 0, "2" );
			pRDRALTM->SetLabel( 1, "1" );
			pRDRALTM->SetInitialPosition( 1 );
		}
	}

	PanelF6::~PanelF6()
	{
		delete mgr_adiR;
		delete mgr_adiP;
		delete mgr_adiY;
		delete mgr_needle_RollRateCDR;
		delete mgr_needle_PitchRateCDR;
		delete mgr_needle_YawRateCDR;
		delete mgr_needle_RollErrorCDR;
		delete mgr_needle_PitchErrorCDR;
		delete mgr_needle_YawErrorCDR;
		delete mgr_needle_alpha;
		delete mgr_needle_alt;
	}

	void PanelF6::DefineVC()
	{
		VECTOR3 switch_rot = _V(1, 0, 0);
		VECTOR3 push_dir = _V( 0.0, -0.258681, 0.965963 );
		VECTOR3 pull_dir = -push_dir;

		AddAIDToMouseEventList( AID_F6 );
		AddAIDToMouseEventList( AID_F6_MDU );
		AddAIDToMouseEventList( AID_F6_HUD );

		pCDR1->DefineVCGroup( GRP_CDR1_F6_VC );
		if (bHasMEDS) pCDR1->DefineMaterial( MAT_MDU_CDR1_F6_VC );
		else pCDR1->DefineMaterial(15);
		pCDR1->SetMouseRegion( AID_F6_MDU, 0.0f, 0.0f, 0.490755f, 1.0f );

		pCDR2->DefineVCGroup( GRP_CDR2_F6_VC );
		if (bHasMEDS) pCDR2->DefineMaterial( MAT_MDU_CDR2_F6_VC );
		else pCDR2->DefineMaterial(16);
		pCDR2->SetMouseRegion( AID_F6_MDU, 0.514292f, 0.0f, 1.0f, 1.0f );

		if (bHasMEDS)
		{
			pFltCntlrPower->DefineGroup(GRP_SWITCH_5_F6_VC);
			pFltCntlrPower->SetInitialAnimState(0.5);
			pFltCntlrPower->SetReference(_V(-0.734, 1.99, 14.685), switch_rot);
			pFltCntlrPower->SetMouseRegion(AID_F6, 0.383447f, 0.622820f, 0.424394f, 0.795206f);
			pFltCntlrPower->SetPullDirection(pull_dir);
		}

		pADIAttitude->DefineGroup( GRP_SWITCH_6_F6_VC );
		pADIAttitude->SetInitialAnimState( 0.5 );

		if (bHasMEDS)
		{
			pADIAttitude->SetReference(_V(0.633, 2.045, 14.7004), switch_rot);
			pADIAttitude->SetMouseRegion(AID_F6, 0.576813f, 0.195323f, 0.617059f, 0.362889f);
		}
		else
		{
			pADIAttitude->SetReference(_V(-0.914530, 2.188198, 14.738081), switch_rot);
			pADIAttitude->SetMouseRegion(AID_F6, 0.023270f, 0.324666f, 0.085643f, 0.393350f);
		}

		pADIError->DefineGroup( GRP_SWITCH_7_F6_VC );
		pADIError->SetInitialAnimState( 0.5 );

		if (bHasMEDS)
		{
			pADIError->SetReference(_V(0.5921, 2.0469, 14.7009), switch_rot);
			pADIError->SetMouseRegion(AID_F6, 0.653861f, 0.186497f, 0.699455f, 0.366683f);
		}
		else
		{
			pADIError->SetReference(_V(-0.878025, 2.189399, 14.738814), switch_rot);
			pADIError->SetMouseRegion(AID_F6, 0.104524f, 0.325578f, 0.143636f, 0.390902f);
		}

		pADIRate->DefineGroup( GRP_SWITCH_8_F6_VC );
		pADIRate->SetInitialAnimState( 0.5 );

		if (bHasMEDS)
		{
			pADIRate->SetReference(_V(0.5627, 2.0463, 14.7008), switch_rot);
			pADIRate->SetMouseRegion(AID_F6, 0.709568f, 0.184330f, 0.752249f, 0.356840f);
		}
		else
		{
			pADIRate->SetReference(_V(-0.851712, 2.188689, 14.738619), switch_rot);
			pADIRate->SetMouseRegion(AID_F6, 0.155135f, 0.325578f, 0.198606f, 0.390902f);
		}

		pLandingGearTB[0]->DefineMeshGroup( GetVCMeshIndex(), GRP_DS1_F6_VC );

		pLandingGearTB[1]->DefineMeshGroup( GetVCMeshIndex(), GRP_DS2_F6_VC );

		pLandingGearTB[2]->DefineMeshGroup( GetVCMeshIndex(), GRP_DS3_F6_VC );

		if (bHasMEDS)
		{
			pLandingGearArmDeployCover[0]->SetMouseRegion(AID_F6, 0, 0.054526f, 0.528985f, 0.125257f, 0.754980f);
			pLandingGearArmDeployCover[0]->SetMouseRegion(AID_F6, 1, 0.054526f, 0.528985f, 0.070585f, 0.754980f);
			pLandingGearArmDeployCover[0]->SetReference(_V(-0.92, 1.99996, 14.68735), _V(0, 0.965408, 0.260745));
		}
		else
		{
			pLandingGearArmDeployCover[0]->SetMouseRegion(AID_F6, 0, 0.055047f, 0.681387f, 0.125259f, 0.760543f);
			pLandingGearArmDeployCover[0]->SetMouseRegion(AID_F6, 1, 0.055047f, 0.681387f, 0.070949f, 0.760543f);
			pLandingGearArmDeployCover[0]->SetReference(_V(-0.918465, 2.054663, 14.701912), _V(0, 0.965408, 0.260745));
		}
		pLandingGearArmDeployCover[0]->DefineGroup( GRP_A5_COVER1_F6_VC );

		if (bHasMEDS)
		{
			pLandingGearArmDeployCover[1]->SetMouseRegion(AID_F6, 0, 0.145214f, 0.527563f, 0.213629f, 0.753547f);
			pLandingGearArmDeployCover[1]->SetMouseRegion(AID_F6, 1, 0.145214f, 0.527563f, 0.159221f, 0.652332f);
			pLandingGearArmDeployCover[1]->SetReference(_V(-0.87370, 1.99996, 14.68735), _V(0, 0.965408, 0.260745));
		}
		else
		{
			pLandingGearArmDeployCover[1]->SetMouseRegion(AID_F6, 0, 0.143043f, 0.681387f, 0.213269f, 0.760543f);
			pLandingGearArmDeployCover[1]->SetMouseRegion(AID_F6, 1, 0.143043f, 0.681387f, 0.158969f, 0.760543f);
			pLandingGearArmDeployCover[1]->SetReference(_V(-0.872168, 2.054663, 14.701912), _V(0, 0.965408, 0.260745));
		}
		pLandingGearArmDeployCover[1]->DefineGroup( GRP_A5_COVER2_F6_VC );

		pLandingGearArmDeploy[0]->SetStateOffset( 1, 0.0f, 0.488281f );
		pLandingGearArmDeploy[0]->SetDirection( push_dir );
		if (bHasMEDS) pLandingGearArmDeploy[0]->SetMouseRegion(AID_F6, 0.075215f, 0.560928f, 0.116564f, 0.730217f);
		else pLandingGearArmDeploy[0]->SetMouseRegion( AID_F6, 0.073839f, 0.690727f, 0.116178f, 0.750794f);
		pLandingGearArmDeploy[0]->DefineMeshGroup( GetVCMeshIndex(), GRP_A5_S1_F6_VC );

		pLandingGearArmDeploy[1]->SetStateOffset( 1, 0.0f, 0.488281f );
		pLandingGearArmDeploy[1]->SetDirection( push_dir );
		if (bHasMEDS) pLandingGearArmDeploy[1]->SetMouseRegion(AID_F6, 0.162630f, 0.559619f, 0.204250f, 0.726978f);
		else pLandingGearArmDeploy[1]->SetMouseRegion( AID_F6, 0.160635f, 0.690727f, 0.202732f, 0.750794f);
		pLandingGearArmDeploy[1]->DefineMeshGroup( GetVCMeshIndex(), GRP_A5_S2_F6_VC );

		pRCSCommand[0]->DefineMeshGroup( GetVCMeshIndex(), GRP_XDS1_R_F6_VC );
		pRCSCommand[0]->SetStateOffset( 1, 0.0f, 0.126953f );// L
		pRCSCommand[0]->SetStateOffset( 2, 0.139648f, 0.126953f );// R
		pRCSCommand[0]->SetStateOffset( 3, 0.139648f, 0.0f );// LR

		pRCSCommand[1]->DefineMeshGroup( GetVCMeshIndex(), GRP_XDS1_P_F6_VC );
		pRCSCommand[1]->SetStateOffset( 1, 0.0f, 0.126953f );// U
		pRCSCommand[1]->SetStateOffset( 2, 0.139648f, 0.126953f );// D
		pRCSCommand[1]->SetStateOffset( 3, 0.139648f, 0.0f );// UD

		pRCSCommand[2]->DefineMeshGroup( GetVCMeshIndex(), GRP_XDS1_Y_F6_VC );
		pRCSCommand[2]->SetStateOffset( 1, 0.0f, 0.126953f );// L
		pRCSCommand[2]->SetStateOffset( 2, 0.139648f, 0.126953f );// R
		pRCSCommand[2]->SetStateOffset( 3, 0.139648f, 0.0f );// LR

		pRangeSafeArm->DefineMeshGroup( GetVCMeshIndex(), GRP_XDS3_F6_VC );
		pRangeSafeArm->SetStateOffset( 1, 0.139648f, 0.0f );
		pRangeSafeArm->SetStateOffset( 2, 0.139648f, 0.0f );
		pRangeSafeArm->SetStateOffset( 3, 0.139648f, 0.0f );

		pAbort->SetStateOffset( 1, -0.139648f, 0.0f );
		pAbort->SetDirection( push_dir );
		if (bHasMEDS) pAbort->SetMouseRegion(AID_F6, 0.908141f, 0.214696f, 0.939683f, 0.348886f);
		else pAbort->SetMouseRegion( AID_F6, 0.931817f, 0.564081f, 0.965701f, 0.609725f);
		pAbort->DefineMeshGroup( GetVCMeshIndex(), GRP_A8_S2_F6_VC );

		pHUDMode->DefineGroup( GRP_HUDTEST_F6_VC );
		pHUDMode->SetInitialAnimState( 0.5 );
		pHUDMode->SetReference( _V( 0.7052, 2.4685, 14.5712 ), _V( 1, 0, 0 ) );
		pHUDMode->SetMouseRegion( AID_F6_HUD, 0.046968f, 0.392141f, 0.177257f, 0.644709f );

		pHUDBrightness->DefineGroup( GRP_HUDDIM_F6_VC );
		pHUDBrightness->SetInitialAnimState( 0.5 );
		pHUDBrightness->SetReference( _V( -0.6552, 2.4697, 14.5635 ), _V( 0.0, 0.275637, -0.961262 ) );
		pHUDBrightness->SetAngleRange( 180.0 );
		pHUDBrightness->SetMouseRegion( AID_F6_HUD, 0.401231f, 0.353931f, 0.605132f, 0.718589f );

		pHUDBright->DefineGroup( GRP_HUDBRT_F6_VC );
		pHUDBright->SetInitialAnimState( 0.5 );
		pHUDBright->SetReference( _V( 0.5978, 2.4679, 14.5712 ), _V( 1, 0, 0 ) );
		pHUDBright->SetMouseRegion( AID_F6_HUD, 0.814331f, 0.392763f, 0.946918f, 0.646512f );

		if (bHasMEDS)
		{
			pRDRALTM->DefineGroup(GRP_SWITCH_4_F6_VC);
			pRDRALTM->SetInitialAnimState(0.5);
			pRDRALTM->SetReference(_V(-0.771385, 1.99079, 14.6858), _V(1, 0, 0));
			pRDRALTM->SetMouseRegion(AID_F6, 0.324223f, 0.622842f, 0.371621f, 0.797419f);
		}

		DefineVCAnimations();

		return;
	}

	void PanelF6::RegisterVC()
	{
		AtlantisPanel::RegisterVC();
		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		if (bHasMEDS)
		{
			oapiVCRegisterArea(AID_F6, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED);
			oapiVCSetAreaClickmode_Quadrilateral(AID_F6,
				_V(-0.9453, 2.0815, 14.7095) + ofs, _V(-0.4221, 2.0815, 14.7095) + ofs,
				_V(-0.9453, 1.9531, 14.6751) + ofs, _V(-0.4221, 1.9531, 14.6751) + ofs);

			oapiVCRegisterArea(AID_F6_MDU, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED);
			oapiVCSetAreaClickmode_Quadrilateral(AID_F6_MDU,
				_V(-0.9395, 2.3122, 14.7566) + ofs, _V(-0.4259, 2.3122, 14.7566) + ofs,
				_V(-0.9395, 2.0986, 14.6968) + ofs, _V(-0.4259, 2.0986, 14.6968) + ofs);
		}
		else
		{
			oapiVCRegisterArea(AID_F6, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED);
			oapiVCSetAreaClickmode_Quadrilateral(AID_F6,
				_V(-0.943685, 2.320348, 14.773584) + ofs, _V(-0.416913, 2.320348, 14.773584) + ofs,
				_V(-0.943685, 1.951079, 14.674603) + ofs, _V(-0.416913, 1.951079, 14.674603) + ofs);

			oapiVCRegisterArea(AID_F6_MDU, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED);
			oapiVCSetAreaClickmode_Quadrilateral(AID_F6_MDU,
				_V(-0.9395, 2.3122, 15.4043) + ofs, _V(-0.4259, 2.3122, 15.4043) + ofs,
				_V(-0.9395, 2.0986, 15.4043) + ofs, _V(-0.4259, 2.0986, 15.4043) + ofs);
		}

		oapiVCRegisterArea( AID_F6_HUD, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED );
		oapiVCSetAreaClickmode_Quadrilateral( AID_F6_HUD,
			_V( -0.7256, 2.5085, 14.5803 ) + ofs, _V( -0.5853, 2.5085, 14.5803 ) + ofs,
			_V( -0.7256, 2.4326, 14.5593 ) + ofs, _V( -0.5853, 2.4326, 14.5593 ) + ofs );

		return;
	}

	void PanelF6::Realize()
	{
		DiscreteBundle* pBundle;

		if (bHasMEDS)
		{
			pBundle = STS()->BundleManager()->CreateBundle("DDU_POWER", 16);
			pFltCntlrPower->ConnectPort(1, pBundle, 2);

		}

		pBundle = STS()->BundleManager()->CreateBundle("ADI_Switches_F6_F8", 12);
		pADIAttitude->ConnectPort( 0, pBundle, 0 );// REF
		pADIAttitude->ConnectPort( 2, pBundle, 1 );// INRTL
		pADIError->ConnectPort( 0, pBundle, 2 );// LOW
		pADIError->ConnectPort( 2, pBundle, 3 );// HIGH
		pADIRate->ConnectPort( 0, pBundle, 4 );// LOW
		pADIRate->ConnectPort( 2, pBundle, 5 );// HIGH

		pBundle = STS()->BundleManager()->CreateBundle( "LANDING_GEAR", 16 );
		pLandingGearArmDeploy[0]->ConnectPushButton( pBundle, 0 );// arm pb
		pLandingGearArmDeploy[1]->ConnectPushButton( pBundle, 2 );// dn pb
		pLandingGearTB[0]->SetInput( 0, pBundle, 5, TB_UP );
		pLandingGearTB[0]->SetInput( 1, pBundle, 6, TB_DN );
		pLandingGearTB[1]->SetInput( 0, pBundle, 7, TB_UP );
		pLandingGearTB[1]->SetInput( 1, pBundle, 8, TB_DN );
		pLandingGearTB[2]->SetInput( 0, pBundle, 9, TB_UP );
		pLandingGearTB[2]->SetInput( 1, pBundle, 10, TB_DN );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA1_4", 16 );
		pAbort->ConnectLight( 0, pBundle, 13 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA1_5", 16 );
		pRangeSafeArm->ConnectLight( 0, pBundle, 5 );
		pRangeSafeArm->ConnectLight( 1, pBundle, 13 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA2_3", 16 );
		pRCSCommand[0]->ConnectLight( 0, pBundle, 1 );
		pRCSCommand[0]->ConnectLight( 2, pBundle, 5 );
		pRCSCommand[1]->ConnectLight( 0, pBundle, 9 );
		pRCSCommand[1]->ConnectLight( 2, pBundle, 13 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA2_4", 16 );
		pRCSCommand[2]->ConnectLight( 0, pBundle, 1 );
		pRCSCommand[2]->ConnectLight( 2, pBundle, 5 );
		pAbort->ConnectLight( 1, pBundle, 13 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA2_5", 16 );
		pRangeSafeArm->ConnectLight( 2, pBundle, 13 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA3_3", 16 );
		pRCSCommand[0]->ConnectLight( 1, pBundle, 1 );
		pRCSCommand[0]->ConnectLight( 3, pBundle, 5 );
		pRCSCommand[1]->ConnectLight( 1, pBundle, 9 );
		pRCSCommand[1]->ConnectLight( 3, pBundle, 13 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA3_4", 16 );
		pRCSCommand[2]->ConnectLight( 1, pBundle, 1 );
		pRCSCommand[2]->ConnectLight( 3, pBundle, 5 );
		pLandingGearArmDeploy[0]->ConnectLight( 0, pBundle, 13 );// arm light

		pBundle = STS()->BundleManager()->CreateBundle( "ACA3_5", 16 );
		pLandingGearArmDeploy[1]->ConnectLight( 0, pBundle, 1 );// dn light
		pRangeSafeArm->ConnectLight( 1, pBundle, 13 );

		pBundle = STS()->BundleManager()->CreateBundle( "HUD_SWITCHES", 16 );
		// power cdr
		pHUDMode->ConnectPort( 0, pBundle, 1 );// mode dclt cdr
		// TODO mode norm cdr
		pHUDMode->ConnectPort( 2, pBundle, 3 );// mode test cdr
		pHUDBrightness->Connect( pBundle, 4 );// brightness cdr
		pHUDBright->ConnectPort( 0, pBundle, 5 );// bright man night cdr
		// TODO bright auto cdr
		pHUDBright->ConnectPort( 2, pBundle, 7 );// bright man day cdr

		if (bHasMEDS)
		{
			pBundle = STS()->BundleManager()->CreateBundle("RDRALTM", 16);
			pRDRALTM->ConnectPort(1, pBundle, 0);// CDR RDR ALTM
		}

		AtlantisPanel::Realize();
	}

	void PanelF6::DefineVCAnimations()
	{
		if (!bHasMEDS)
		{
			//CDR ADI Animation defs
			// Panel tilt
			const double P1_3_TILT = 15.52 * RAD;
			// Constants
			const VECTOR3 fdairollaxis = { -0.00, sin(P1_3_TILT), -cos(P1_3_TILT) };
			const VECTOR3 fdaiyawvaxis = { -0.00, sin(P1_3_TILT + (90.0 * RAD)), -cos(P1_3_TILT + (90.0 * RAD)) };
			const VECTOR3 needlexvector = { 0.00, 0.05 * cos(P1_3_TILT), 0.05 * sin(P1_3_TILT) };
			const VECTOR3 needleyvector = { 0.05, 0, 0 };
			const VECTOR3 ratexvector = { 0.00, 0.064 * cos(P1_3_TILT), 0.064 * sin(P1_3_TILT) };
			const VECTOR3 rateyvector = { 0.064, 0, 0 };
			const VECTOR3 FDAI_PIVOT_L = { -0.617523, 2.212802, 14.805983 }; // L FDAI Pivot Point

			//CDR ADI Ball
			static UINT meshgroup_Fdai1_L = { 44 }; // Roll gimbal meshgroup (includes roll indicator)
			static UINT meshgroup_Fdai2_L = { 45 };  // Pitch gimbal meshgroup (visible ball)
			static UINT meshgroup_Fdai3_L = { 46 }; // Yaw gimbal meshgroup
			static MGROUP_ROTATE mgt_FDAIRoll_L(GetVCMeshIndex(), &meshgroup_Fdai1_L, 1, FDAI_PIVOT_L, fdairollaxis, (float)(RAD * 360));
			static MGROUP_ROTATE mgt_FDAIPitch_L(GetVCMeshIndex(), &meshgroup_Fdai2_L, 1, FDAI_PIVOT_L, _V(-1, 0, 0), (float)(RAD * 360));
			static MGROUP_ROTATE mgt_FDAIYaw_L(GetVCMeshIndex(), &meshgroup_Fdai3_L, 1, FDAI_PIVOT_L, fdaiyawvaxis, (float)(RAD * 360));
			anim_adiR = STS()->CreateAnimation(0.0);
			anim_adiP = STS()->CreateAnimation(0.0);
			anim_adiY = STS()->CreateAnimation(0.0);
			ach_ADIroll = STS()->AddAnimationComponent(anim_adiR, 0.0f, 1.0f, &mgt_FDAIRoll_L);
			ach_ADIyaw = STS()->AddAnimationComponent(anim_adiY, 0.0f, 1.0f, &mgt_FDAIYaw_L, ach_ADIroll);
			ach_ADIpitch = STS()->AddAnimationComponent(anim_adiP, 0.0f, 1.0f, &mgt_FDAIPitch_L, ach_ADIyaw);

			//CDR ADI Rate needles
			static UINT needle_RollRateCDR_group[1] = { 51 };
			static UINT needle_PitchRateCDR_group[1] = { 50 };
			static UINT needle_YawRateCDR_group[1] = { 52 };
			mgr_needle_RollRateCDR = new MGROUP_TRANSLATE(GetVCMeshIndex(), needle_RollRateCDR_group, 1, _V(0.030845, 0, 0) * 2);
			mgr_needle_PitchRateCDR = new MGROUP_TRANSLATE(GetVCMeshIndex(), needle_PitchRateCDR_group, 1, _V(0, -0.029832, -0.008284) * 2);
			mgr_needle_YawRateCDR = new MGROUP_TRANSLATE(GetVCMeshIndex(), needle_YawRateCDR_group, 1, _V(0.030845, 0, 0) * 2);

			anim_needle_RollRateCDR = STS()->CreateAnimation(0.5);
			STS()->AddAnimationComponent(anim_needle_RollRateCDR, 0, 1, mgr_needle_RollRateCDR);

			anim_needle_PitchRateCDR = STS()->CreateAnimation(0.5);
			STS()->AddAnimationComponent(anim_needle_PitchRateCDR, 0, 1, mgr_needle_PitchRateCDR);

			anim_needle_YawRateCDR = STS()->CreateAnimation(0.5);
			STS()->AddAnimationComponent(anim_needle_YawRateCDR, 0, 1, mgr_needle_YawRateCDR);

			//CDR ADI Error needles
			static UINT needle_RollErrorCDR_group[1] = { 53 };
			static UINT needle_PitchErrorCDR_group[1] = { 49 };
			static UINT needle_YawErrorCDR_group[1] = { 54 };
			mgr_needle_RollErrorCDR = new MGROUP_TRANSLATE(GetVCMeshIndex(), needle_RollErrorCDR_group, 1, _V(-0.021349, 0, 0) * 2);
			mgr_needle_PitchErrorCDR = new MGROUP_TRANSLATE(GetVCMeshIndex(), needle_PitchErrorCDR_group, 1, _V(0, 0.020215, 0.005614) * 2);
			mgr_needle_YawErrorCDR = new MGROUP_TRANSLATE(GetVCMeshIndex(), needle_YawErrorCDR_group, 1, _V(-0.021349, 0, 0) * 2);

			anim_needle_RollErrorCDR = STS()->CreateAnimation(0.5);
			STS()->AddAnimationComponent(anim_needle_RollErrorCDR, 0, 1, mgr_needle_RollErrorCDR);

			anim_needle_PitchErrorCDR = STS()->CreateAnimation(0.5);
			STS()->AddAnimationComponent(anim_needle_PitchErrorCDR, 0, 1, mgr_needle_PitchErrorCDR);

			anim_needle_YawErrorCDR = STS()->CreateAnimation(0.5);
			STS()->AddAnimationComponent(anim_needle_YawErrorCDR, 0, 1, mgr_needle_YawErrorCDR);

			//Alpha needle
			static UINT needle_alpha[1] = { 85 };
			mgr_needle_alpha = new MGROUP_TRANSLATE(GetVCMeshIndex(), needle_alpha, 1, _V(0, 0.096221, 0.025155));
			anim_needle_alpha = STS()->CreateAnimation(0.0);
			STS()->AddAnimationComponent(anim_needle_alpha, 0, 1, mgr_needle_alpha);

			//Alt needle
			static UINT needle_alt[1] = { 87 };
			mgr_needle_alt = new MGROUP_TRANSLATE(GetVCMeshIndex(), needle_alt, 1, _V(0, 0.04019, 0.01067) * 2);
			anim_needle_alt = STS()->CreateAnimation(0.5);
			STS()->AddAnimationComponent(anim_needle_alt, 0, 1, mgr_needle_alt);
		}
	}

	void PanelF6::OnPreStep(double simt, double simdt, double mjd)
	{
		if (!bHasMEDS)
		{
			int MM = STS()->GetGPCMajorMode();
			VECTOR3 atterr;
			//ADI
			VECTOR3 av;// x = pitch, y = yaw, z = roll
			STS()->GetAngularVel(av);
			av *= DEG;
			av.z = -av.z;
			av.x = -av.x;

			switch (MM)
			{
			case 101:
			case 102:
			case 103:
			case 104:
			case 105:
			case 106:
				atterr = pCDR1->GetIDP()->GetAttitudeErrors_AscentDAP();
				break;
			case 201:
			case 202:
			case 801:
				atterr = pCDR1->GetIDP()->GetAttitudeCommandErrors();
				break;
			case 301:
			case 302:
			case 303:
			case 304:
			case 305:
				atterr = pCDR1->GetIDP()->GetAttitudeErrors_AerojetDAP();
				break;
			case 601:
				atterr = pCDR1->GetIDP()->GetAttitudeErrors_AscentDAP();
				break;
			case 602:
			case 603:
			case 901:
				atterr = pCDR1->GetIDP()->GetAttitudeErrors_AerojetDAP();
				break;
				break;
			default:
				break;
			}

			double attRoll = -STS()->GetBank();
			double attPitch = STS()->GetPitch();
			double attYaw = STS()->GetSlipAngle();

			if (cos(attRoll) < 0) attYaw = -attYaw;

			if (attRoll >= PI2) attRoll -= PI2;
			if (attRoll < 0) attRoll += PI2;

			if (attPitch >= PI2) attPitch -= PI2;
			if (attPitch < 0) attPitch += PI2;

			if (attYaw >= PI2) attYaw -= PI2;
			if (attYaw < 0) attYaw += PI2;

			STS()->SetAnimation(anim_adiR, attRoll / 360.0 * DEG);
			STS()->SetAnimation(anim_adiP, attPitch / 360.0 * DEG);
			STS()->SetAnimation(anim_adiY, attYaw / 360.0 * DEG);

			// Rate needles
			int adirate = pCDR2->GetIDP()->GetADIRate();
			double maxRate = (adirate == 1) ? 5.0 : (adirate == 2) ? 10.0 : 1.0;

			STS()->SetAnimation(anim_needle_RollRateCDR, max(0.0, min(1.0, 0.5 + av.z / (2.0 * maxRate))));
			STS()->SetAnimation(anim_needle_PitchRateCDR, max(0.0, min(1.0, 0.5 + av.x / (2.0 * maxRate))));
			STS()->SetAnimation(anim_needle_YawRateCDR, max(0.0, min(1.0, 0.5 + av.y / (2.0 * maxRate))));

			// Error needles
			int adierr = pCDR2->GetIDP()->GetADIError();
			double maxRate2 = (adierr == 1) ? 5.0 : (adierr == 2) ? 10.0 : 1.0;

			STS()->SetAnimation(anim_needle_RollErrorCDR, max(0.0, min(1.0, 0.5 + atterr.z / (2.0 * maxRate2))));
			STS()->SetAnimation(anim_needle_PitchErrorCDR, max(0.0, min(1.0, 0.5 + atterr.x / (2.0 * maxRate2))));
			STS()->SetAnimation(anim_needle_YawErrorCDR, max(0.0, min(1.0, 0.5 + atterr.y / (2.0 * maxRate2))));

			// Tapemeters
			// Alpha
			double alpha = STS()->GetAOA() * DEG;

			auto updateAlpha = [&](DWORD group, const std::vector<NTVERTEX>& originalVtx, const std::vector<float>& originalTV, std::vector<WORD>& idx, DWORD nVtx, double minAlpha, double maxAlpha, float range)
				{
					if (!hF6_dev || originalVtx.empty()) return;

					GROUPEDITSPEC ges = {};
					ges.flags = GRPEDIT_VTXTEX;
					ges.nVtx = nVtx;

					std::vector<NTVERTEX> vtx = originalVtx;
					double limitedAlpha = (alpha < minAlpha && alpha < maxAlpha) ? ((minAlpha < maxAlpha) ? minAlpha : maxAlpha) : ((alpha > minAlpha && alpha > maxAlpha) ? ((minAlpha > maxAlpha) ? minAlpha : maxAlpha) : alpha);
					float dy = (range / 1719.0f) * static_cast<float>((limitedAlpha - minAlpha) / (maxAlpha - minAlpha));

					for (DWORD i = 0; i < ges.nVtx; i++)
						vtx[i].tv = originalTV[i] - dy;

					ges.Vtx = vtx.data();
					ges.vIdx = idx.data();

					oapiEditMeshGroup(hF6_dev, group, &ges);
				};

			updateAlpha(88, alpha1Vtx, alpha1TV, alpha1Idx, alpha1Edit.nVtx, 28.0, 60.0, 255.76f);
			updateAlpha(80, alpha2Vtx, alpha2TV, alpha2Idx, alpha2Edit.nVtx, -4.0, -25.0, -168.2);

			////Accel tapemeter
			//if (hF6_dev && !accelVtx.empty())
			//{
			//	GROUPEDITSPEC ges = {};
			//	ges.flags = GRPEDIT_VTXTEX;
			//	ges.nVtx = accelEdit.nVtx;

			//	std::vector<NTVERTEX> vtx = accelVtx;
			//	double limitedAccel = (alpha < -25.0) ? -25.0 : ((alpha > -4.0) ? -4.0 : alpha);
			//	float dy = (-200.0f / 2048.0f) * static_cast<float>((limitedAccel + 4.0) / -21.0);

			//	for (DWORD i = 0; i < ges.nVtx; i++)
			//		vtx[i].tv = accelTV[i] - dy;

			//	ges.Vtx = vtx.data();
			//	ges.vIdx = accelIdx.data();

			//	oapiEditMeshGroup(hF6_dev, 76, &ges);
			//}

			// Mach tapemeter
			if (hF6_dev && !mVtx.empty())
			{
				OBJHANDLE hEarth = STS()->GetGravityRef();
				VECTOR3 ECEF_vel = GetVelocity_ECEF(STS(), hEarth) * MPS2FPS;
				double VE = length(ECEF_vel);

				VECTOR3 v3vi;
				STS()->GetRelativeVel(STS()->GetSurfaceRef(), v3vi);
				double vi = length(v3vi) * MPS2FPS;

				double mach = STS()->GetMachNumber();

				GROUPEDITSPEC ges = {};
				ges.flags = GRPEDIT_VTXTEX;
				ges.nVtx = mEdit.nVtx;

				std::vector<NTVERTEX> vtx = mVtx;
				double dy;

				if (mach < 4.0)
					dy = (1438 / 4.0) * ((mach < 0.0) ? 0.0 : mach);
				else
					dy = 1692.0 + (vi - 4500.0) * ((9432.34 - 1692.0) / (26000.0 - 4500.0));

				if (dy < 0.0) dy = 0.0;
				if (dy > 9432.34) dy = 9432.34;

				float uvOffset = static_cast<float>(dy / 9845);

				for (DWORD i = 0; i < ges.nVtx; i++)
					vtx[i].tv = mTV[i] - uvOffset;

				ges.Vtx = vtx.data();
				ges.vIdx = mIdx.data();

				oapiEditMeshGroup(hF6_dev, 77, &ges);
			}

			//KT tapemeter
			if (hF6_dev && !ktVtx.empty())
			{
				VECTOR3 vel;
				STS()->GetAirspeedVector(FRAME_LOCAL, vel);
				double airspeed = length(vel) * 1.943844; // knots (kt)

				GROUPEDITSPEC ges = {};
				ges.flags = GRPEDIT_VTXTEX;
				ges.nVtx = ktEdit.nVtx;

				std::vector<NTVERTEX> vtx = ktVtx;
				double limitedkt = (airspeed < 0.0) ? 0.0 : ((airspeed > 500) ? 500 : airspeed);
				float dy = (-3600.0f / 4069.0f) * static_cast<float>((limitedkt + 0.0) / 500);

				for (DWORD i = 0; i < ges.nVtx; i++)
					vtx[i].tv = ktTV[i] - dy;

				ges.Vtx = vtx.data();
				ges.vIdx = ktIdx.data();

				oapiEditMeshGroup(hF6_dev, 78, &ges);
			}

			//Hdot tapemeter
			if (hF6_dev && !hdotVtx.empty())
			{
				VECTOR3 vel;
				STS()->GetGroundspeedVector(FRAME_HORIZON, vel);
				vel *= MPS2FPS;

				GROUPEDITSPEC ges = {};
				ges.flags = GRPEDIT_VTXTEX;
				ges.nVtx = hdotEdit.nVtx;

				std::vector<NTVERTEX> vtx = hdotVtx;
				double limitedhdot = (vel.y < -740.0) ? -740.0 : ((vel.y > 740.0) ? 740.0 : vel.y);
				float dy = (limitedhdot >= 0.0)
					? (5325.0f / 10860.0f) * static_cast<float>(limitedhdot / 740.0)
					: -(5331.0f / 10860.0f) * static_cast<float>(-limitedhdot / 740.0);

				for (DWORD i = 0; i < ges.nVtx; i++)
					vtx[i].tv = hdotTV[i] - dy;

				ges.Vtx = vtx.data();
				ges.vIdx = hdotIdx.data();

				oapiEditMeshGroup(hF6_dev, 81, &ges);
			}

			//Alt tapemeter
			if (hF6_dev && !altVtx.empty())
			{
				double Altitude_ft = STS()->GetAltitude() * MPS2FPS;

				GROUPEDITSPEC ges = {};
				ges.flags = GRPEDIT_VTXTEX;
				ges.nVtx = altEdit.nVtx;

				std::vector<NTVERTEX> vtx = altVtx;

				const double altScale[][2] = {
					{0.0, 0.0},
					{50.0, -35.5},
					{100.0, -73.5},
					{1000.0, -406.5},
					{1200.0, -444.5},
					{2000.0, -593.0},
					{3000.0, -628.5},
					{100000.0, -4218.5},
					{250000.0, -4363.8},
					{400000.0, -4808.23},
					{70.0 * 6076.11549, -4957.45},
					{165.0 * 6076.11549, -7769.5}
				};

				const int n = sizeof(altScale) / sizeof(altScale[0]);
				double dy = altScale[n - 1][1];

				if (Altitude_ft <= altScale[0][0])
					dy = altScale[0][1];
				else
				{
					for (int i = 1; i < n; i++)
					{
						if (Altitude_ft <= altScale[i][0])
						{
							double t = (Altitude_ft - altScale[i - 1][0]) / (altScale[i][0] - altScale[i - 1][0]);
							dy = altScale[i - 1][1] + t * (altScale[i][1] - altScale[i - 1][1]);
							break;
						}
					}
				}

				float uvOffset = -static_cast<float>(dy / 9295.0);

				for (DWORD i = 0; i < ges.nVtx; i++)
					vtx[i].tv = altTV[i] + uvOffset;

				ges.Vtx = vtx.data();
				ges.vIdx = altIdx.data();

				oapiEditMeshGroup(hF6_dev, 82, &ges);
			}

			//Alpha needle
			double limitedAlpha = (alpha < -4.0) ? -4.0 : ((alpha > 28.0) ? 28.0 : alpha);
			STS()->SetAnimation(anim_needle_alpha, (limitedAlpha + 4.0) / 32.0);

			//Alt needle
			int altAcc = pCDR2->GetIDP()->GetVacc();
			double limitedAlt = (altAcc < -10.0) ? -10.0 : ((altAcc > 10.0) ? 10.0 : altAcc);
			STS()->SetAnimation(anim_needle_alt, (limitedAlt + 10.0) / 20.0);
		}
	}

	void PanelF6::VisualCreated()
	{
		if (!bHasMEDS)
		{
			hF6_dev = STS()->GetDevMesh(STS()->Get_vis(), GetVCMeshIndex());

			MESHGROUP* gAlpha1 = oapiMeshGroup(GetVCMeshHandle(), 88);
			MESHGROUP* gAlpha2 = oapiMeshGroup(GetVCMeshHandle(), 80);
			MESHGROUP* gAccel = oapiMeshGroup(GetVCMeshHandle(), 76);
			MESHGROUP* gM = oapiMeshGroup(GetVCMeshHandle(), 77);
			MESHGROUP* gKT = oapiMeshGroup(GetVCMeshHandle(), 78);
			MESHGROUP* gHdot = oapiMeshGroup(GetVCMeshHandle(), 81);
			MESHGROUP* gAlt = oapiMeshGroup(GetVCMeshHandle(), 82);

			if (!hF6_dev || !gAlpha1 || !gAlpha2 || !gAccel || !gM || !gKT || !gHdot || !gAlt) return;

			auto initVTX = [](MESHGROUP* g, std::vector<NTVERTEX>& vtx, std::vector<float>& tv, std::vector<WORD>& idx, GROUPEDITSPEC& edit)
				{
					vtx.assign(g->Vtx, g->Vtx + g->nVtx);
					tv.resize(g->nVtx);
					idx.resize(g->nVtx);

					for (DWORD i = 0; i < g->nVtx; i++)
					{
						tv[i] = g->Vtx[i].tv;
						idx[i] = static_cast<WORD>(i);
					}

					edit.flags = GRPEDIT_VTXTEXV;
					edit.nVtx = g->nVtx;
					edit.Vtx = vtx.data();
					edit.vIdx = idx.data();
				};

			initVTX(gAlpha1, alpha1Vtx, alpha1TV, alpha1Idx, alpha1Edit);
			initVTX(gAlpha2, alpha2Vtx, alpha2TV, alpha2Idx, alpha2Edit);
			initVTX(gAccel, accelVtx, accelTV, accelIdx, accelEdit);
			initVTX(gM, mVtx, mTV, mIdx, mEdit);
			initVTX(gKT, ktVtx, ktTV, ktIdx, ktEdit);
			initVTX(gHdot, hdotVtx, hdotTV, hdotIdx, hdotEdit);
			initVTX(gAlt, altVtx, altTV, altIdx, altEdit);
		}
	}
}

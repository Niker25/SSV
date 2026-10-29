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
2022/10/04   GLS
2024/12/30   GLS
********************************************/
#include "PanelF8.h"
#include "MDU.h"
#include "StandardSwitch.h"
#include "Talkback.h"
#include "PushButtonIndicator.h"
#include "StandardSwitchCover.h"
#include "RotarySwitchPotentiometer.h"
#include "../Atlantis.h"
#include "../meshres_vc.h"
#include "../ParameterValues.h"
#include "vc_defs.h"
#include "../meshres_vc_f8.h"
#include "../dps/IDP.h"


namespace vc {

	PanelF8::PanelF8(Atlantis* _sts)
		: AtlantisPanel(_sts, "F8")
	{
		DefineMesh( MESHNAME_PANELF8_MCD );

		Add(pPLT1 = new MDU(_sts, "PLT1", MDUID_PLT1));
		Add(pPLT2 = new MDU(_sts, "PLT2", MDUID_PLT2));
		Add(pFltCntlrPower = new LockableLever2(_sts, "FLT CNTLR POWER"));
		Add( pADIAttitude = new StdSwitch3( _sts, "ADI ATTITUDE" ) );
		Add( pADIError = new StdSwitch3( _sts, "ADI ERROR" ) );
		Add( pADIRate = new StdSwitch3( _sts, "ADI RATE" ) );

		pFltCntlrPower->SetLabel(0, "OFF");
		pFltCntlrPower->SetLabel(1, "ON");
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

		Add( pRDRALTM = new StdSwitch2( _sts, "RDR ALTM" ) );
		pRDRALTM->SetLabel( 0, "2" );
		pRDRALTM->SetLabel( 1, "1" );
		pRDRALTM->SetInitialPosition( 0 );
	}

	PanelF8::~PanelF8()
	{
	}

	void PanelF8::DefineVC()
	{
		VECTOR3 switch_rot = _V(1, 0, 0);
		VECTOR3 push_dir = _V( 0.0, -0.258681, 0.965963 );
		VECTOR3 pull_dir = -push_dir;

		AddAIDToMouseEventList( AID_F8 );
		AddAIDToMouseEventList( AID_F8_MDU );
		AddAIDToMouseEventList( AID_F8_HUD );

		pPLT1->DefineVCGroup( GRP_PLT1_F8_VC );
		pPLT1->DefineMaterial( MAT_MDU_PLT1_F8_VC );
		pPLT1->SetMouseRegion( AID_F8_MDU, 0.0f, 0.0f, 0.489801f, 1.0f );

		pPLT2->DefineVCGroup( GRP_PLT2_F8_VC );
		pPLT2->DefineMaterial( MAT_MDU_PLT2_F8_VC );
		pPLT2->SetMouseRegion( AID_F8_MDU, 0.509333f, 0.0f, 1.0f, 1.0f );

		pFltCntlrPower->DefineGroup(GRP_SWITCH5_F8_VC);
		pFltCntlrPower->SetInitialAnimState(0.5);
		pFltCntlrPower->SetReference(_V(0.740634, 1.975803, 14.681213), switch_rot);
		pFltCntlrPower->SetMouseRegion( AID_F8, 0.597045, 0.894284, 0.638862, 0.96521);
		pFltCntlrPower->SetPullDirection( pull_dir );

		pADIAttitude->DefineGroup( GRP_SWITCH6_F8_VC );
		pADIAttitude->SetInitialAnimState( 0.5 );
		pADIAttitude->SetReference( _V(0.845344, 2.188198, 14.738081), switch_rot );
		pADIAttitude->SetMouseRegion( AID_F8, 0.794606, 0.324753, 0.836424, 0.38584);

		pADIError->DefineGroup( GRP_SWITCH7_F8_VC );
		pADIError->SetInitialAnimState( 0.5 );
		pADIError->SetReference( _V(0.881845, 2.188198, 14.738081), switch_rot );
		pADIError->SetMouseRegion( AID_F8, 0.865369f, 0.324666f, 0.905219f, 0.393350f);

		pADIRate->DefineGroup( GRP_SWITCH8_F8_VC );
		pADIRate->SetInitialAnimState( 0.5 );
		pADIRate->SetReference( _V(0.908158, 2.188198, 14.738081), switch_rot );
		pADIRate->SetMouseRegion( AID_F8, 0.91516f, 0.324666f, 0.955133f, 0.393350f);

		pLandingGearTB[0]->DefineMeshGroup( GetVCMeshIndex(), GRP_DS1_F8_VC );

		pLandingGearTB[1]->DefineMeshGroup( GetVCMeshIndex(), GRP_DS2_F8_VC );

		pLandingGearTB[2]->DefineMeshGroup( GetVCMeshIndex(), GRP_DS3_F8_VC );

		pLandingGearArmDeployCover[0]->SetMouseRegion( AID_F8, 0, 0.034026, 0.678355, 0.118336, 0.759889);
		pLandingGearArmDeployCover[0]->SetMouseRegion( AID_F8, 1, 0.034026, 0.678355, 0.051717, 0.759889);
		pLandingGearArmDeployCover[0]->SetReference( _V( 0.444, 1.99996, 14.68735 ), _V( 0, 0.965408, 0.260745 ) );
		pLandingGearArmDeployCover[0]->DefineGroup( GRP_A5_COVER1_F8_VC );

		pLandingGearArmDeployCover[1]->SetMouseRegion( AID_F8, 0, 0.121914, 0.678355, 0.206224, 0.759889);
		pLandingGearArmDeployCover[1]->SetMouseRegion( AID_F8, 1, 0.121914, 0.678355, 0.139603, 0.759889);
		pLandingGearArmDeployCover[1]->SetReference( _V( 0.4913, 1.99996, 14.68735 ), _V( 0, 0.965408, 0.260745 ) );
		pLandingGearArmDeployCover[1]->DefineGroup( GRP_A5_COVER2_F8_VC );

		pLandingGearArmDeploy[0]->SetStateOffset( 1, 0.0f, 0.488281f );
		pLandingGearArmDeploy[0]->SetDirection( push_dir );
		pLandingGearArmDeploy[0]->SetMouseRegion( AID_F8, 0.060019, 0.69123, 0.102178, 0.750808);
		pLandingGearArmDeploy[0]->DefineMeshGroup( GetVCMeshIndex(), GRP_A5_S1_F8_VC );

		pLandingGearArmDeploy[1]->SetStateOffset( 1, 0.0f, 0.488281f );
		pLandingGearArmDeploy[1]->SetDirection( push_dir );
		pLandingGearArmDeploy[1]->SetMouseRegion( AID_F8, 0.146963, 0.69123, 0.189122, 0.750808);
		pLandingGearArmDeploy[1]->DefineMeshGroup( GetVCMeshIndex(), GRP_A5_S2_F8_VC );

		pHUDMode->DefineGroup( GRP_HUDTEST_F8_VC );
		pHUDMode->SetInitialAnimState( 0.5 );
		pHUDMode->SetReference( _V( -0.5996, 2.4685, 14.5712 ), _V( 1, 0, 0 ) );
		pHUDMode->SetMouseRegion( AID_F8_HUD, 0.042951f, 0.399852f, 0.177495f, 0.643278f );

		pHUDBrightness->DefineGroup( GRP_HUDDIM_F8_VC );
		pHUDBrightness->SetInitialAnimState( 0.5 );
		pHUDBrightness->SetReference( _V( 0.6496, 2.4697, 14.5635 ), _V( 0, 0.275637, -0.961262 ) );
		pHUDBrightness->SetAngleRange( 180.0 );
		pHUDBrightness->SetMouseRegion( AID_F8_HUD, 0.390848f, 0.343682f, 0.605650f, 0.723217f );

		pHUDBright->DefineGroup( GRP_HUDBRT_F8_VC );
		pHUDBright->SetInitialAnimState( 0.5 );
		pHUDBright->SetReference( _V( -0.7070, 2.4680, 14.5712 ), _V( 1, 0, 0 ) );
		pHUDBright->SetMouseRegion( AID_F8_HUD, 0.810735f, 0.390512f, 0.943742f, 0.641940f );

		pRDRALTM->DefineGroup( GRP_SWITCH4_F8_VC );
		pRDRALTM->SetInitialAnimState( 0.5 );
		pRDRALTM->SetReference( _V(0.770647, 1.976258, 14.681389), _V( 1, 0, 0 ) );
		pRDRALTM->SetMouseRegion( AID_F8, 0.650529, 0.894284, 0.693987, 0.96521);

		DefineVCAnimations();
	}

	void PanelF8::RegisterVC()
	{
		AtlantisPanel::RegisterVC();
		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;
		oapiVCRegisterArea( AID_F8, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED );
		oapiVCSetAreaClickmode_Quadrilateral( AID_F8,
			_V(0.415603, 2.320348, 14.773583) + ofs, _V(0.942375, 2.320348, 14.773585) + ofs,
			_V(0.415603, 1.951080, 14.674603) + ofs, _V(0.942375, 1.951079, 14.674605) + ofs );

		oapiVCRegisterArea( AID_F8_MDU, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED );
		oapiVCSetAreaClickmode_Quadrilateral( AID_F8_MDU,
			_V( 0.4227, 2.3119, 15.4043) + ofs, _V( 0.9362, 2.3119, 15.4043) + ofs,
			_V( 0.4227, 2.0984, 15.4043) + ofs, _V( 0.9362, 2.0984, 15.4043) + ofs );

		oapiVCRegisterArea( AID_F8_HUD, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED );
		oapiVCSetAreaClickmode_Quadrilateral( AID_F8_HUD,
			_V( 0.5799, 2.5083, 14.5803 ) + ofs, _V( 0.7200, 2.5083, 14.5803 ) + ofs,
			_V( 0.5799, 2.4325, 14.5593 ) + ofs, _V( 0.7200, 2.4325, 14.5593 ) + ofs );

		return;
	}

	void PanelF8::Realize()
	{
		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle( "DDU_POWER", 16 );
		pFltCntlrPower->ConnectPort( 1, pBundle, 6 );

		pBundle = STS()->BundleManager()->CreateBundle( "ADI_Switches_F6_F8", 12 );
		pADIAttitude->ConnectPort( 0, pBundle, 6 );// REF
		pADIAttitude->ConnectPort( 2, pBundle, 7 );// INRTL
		pADIError->ConnectPort( 0, pBundle, 8 );// LOW
		pADIError->ConnectPort( 2, pBundle, 9 );// HIGH
		pADIRate->ConnectPort( 0, pBundle, 10 );// LOW
		pADIRate->ConnectPort( 2, pBundle, 11 );// HIGH

		pBundle = STS()->BundleManager()->CreateBundle( "LANDING_GEAR", 16 );
		pLandingGearArmDeploy[0]->ConnectPushButton( pBundle, 1 );// arm pb
		pLandingGearArmDeploy[1]->ConnectPushButton( pBundle, 3 );// dn pb
		pLandingGearTB[0]->SetInput( 0, pBundle, 5, TB_UP );
		pLandingGearTB[0]->SetInput( 1, pBundle, 6, TB_DN );
		pLandingGearTB[1]->SetInput( 0, pBundle, 7, TB_UP );
		pLandingGearTB[1]->SetInput( 1, pBundle, 8, TB_DN );
		pLandingGearTB[2]->SetInput( 0, pBundle, 9, TB_UP );
		pLandingGearTB[2]->SetInput( 1, pBundle, 10, TB_DN );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA2_5", 16 );
		pLandingGearArmDeploy[0]->ConnectLight( 0, pBundle, 7 );// arm light
		pLandingGearArmDeploy[1]->ConnectLight( 0, pBundle, 15 );// dn light

		pBundle = STS()->BundleManager()->CreateBundle( "HUD_SWITCHES", 16 );
		// power plt
		pHUDMode->ConnectPort( 0, pBundle, 9 );// mode dclt plt
		// TODO mode norm plt
		pHUDMode->ConnectPort( 2, pBundle, 11 );// mode test plt
		pHUDBrightness->Connect( pBundle, 12 );// brightness plt
		pHUDBright->ConnectPort( 0, pBundle, 13 );// bright man night plt
		// TODO bright auto plt
		pHUDBright->ConnectPort( 2, pBundle, 15 );// bright man day plt

		pBundle = STS()->BundleManager()->CreateBundle( "RDRALTM", 16 );
		pRDRALTM->ConnectPort( 1, pBundle, 1 );// PLT RDR ALTM

		AtlantisPanel::Realize();
	}

	void PanelF8::DefineVCAnimations()
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
		const VECTOR3 FDAI_PIVOT_L = { 0.622361, 2.212802, 14.805983 }; // L FDAI Pivot Point

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

		//Hydraulic Needles
		static UINT needle_hyd_press_1[1] = { 101 };
		static UINT needle_hyd_press_2[2] = { 35, 36 };
		static UINT needle_hyd_press_3[1] = { 37 };
		mgr_needle_hyd_press_1 = new MGROUP_ROTATE(GetVCMeshIndex(), needle_hyd_press_1, 1, _V(0.735534, 2.013132, 14.920000), _V(1.0, 0.0, 0.0), (float)(RAD * 16.9334));
		mgr_needle_hyd_press_2 = new MGROUP_ROTATE(GetVCMeshIndex(), needle_hyd_press_2, 2, _V(0.735534, 2.013132, 14.920000), _V(1.0, 0.0, 0.0), (float)(RAD * 16.9334));
		mgr_needle_hyd_press_3 = new MGROUP_ROTATE(GetVCMeshIndex(), needle_hyd_press_3, 1, _V(0.735534, 2.013132, 14.920000), _V(1.0, 0.0, 0.0), (float)(RAD * 16.9334));
		anim_needle_hyd_press_1 = STS()->CreateAnimation(0.0);
		anim_needle_hyd_press_2 = STS()->CreateAnimation(0.0);
		anim_needle_hyd_press_3 = STS()->CreateAnimation(0.0);
		STS()->AddAnimationComponent(anim_needle_hyd_press_1, 0, 1, mgr_needle_hyd_press_1);
		STS()->AddAnimationComponent(anim_needle_hyd_press_2, 0, 1, mgr_needle_hyd_press_2);
		STS()->AddAnimationComponent(anim_needle_hyd_press_3, 0, 1, mgr_needle_hyd_press_3);

		static UINT needle_hyd_qty_1[1] = { 92 };
		static UINT needle_hyd_qty_2[2] = { 91, 41 };
		static UINT needle_hyd_qty_3[1] = { 32 };
		mgr_needle_hyd_qty_1 = new MGROUP_ROTATE(GetVCMeshIndex(), needle_hyd_qty_1, 1, _V(0.735534, 2.013132, 14.920000), _V(1.0, 0.0, 0.0), (float)(RAD * 16.9334));
		mgr_needle_hyd_qty_2 = new MGROUP_ROTATE(GetVCMeshIndex(), needle_hyd_qty_2, 2, _V(0.735534, 2.013132, 14.920000), _V(1.0, 0.0, 0.0), (float)(RAD * 16.9334));
		mgr_needle_hyd_qty_3 = new MGROUP_ROTATE(GetVCMeshIndex(), needle_hyd_qty_3, 1, _V(0.735534, 2.013132, 14.920000), _V(1.0, 0.0, 0.0), (float)(RAD * 16.9334));
		anim_needle_hyd_qty_1 = STS()->CreateAnimation(0.0);
		anim_needle_hyd_qty_2 = STS()->CreateAnimation(0.0);
		anim_needle_hyd_qty_3 = STS()->CreateAnimation(0.0);
		STS()->AddAnimationComponent(anim_needle_hyd_qty_1, 0, 1, mgr_needle_hyd_qty_1);
		STS()->AddAnimationComponent(anim_needle_hyd_qty_2, 0, 1, mgr_needle_hyd_qty_2);
		STS()->AddAnimationComponent(anim_needle_hyd_qty_3, 0, 1, mgr_needle_hyd_qty_3);

		//APU Needles
		static UINT needle_fuel_qty_1[1] = { 33 };
		static UINT needle_fuel_qty_2[2] = { 34, 93 };
		static UINT needle_fuel_qty_3[1] = { 94 };
		mgr_needle_fuel_qty_1 = new MGROUP_ROTATE(GetVCMeshIndex(), needle_fuel_qty_1, 1, _V(0.735534, 2.013132, 14.920000), _V(1.0, 0.0, 0.0), (float)(RAD * 16.9334));
		mgr_needle_fuel_qty_2 = new MGROUP_ROTATE(GetVCMeshIndex(), needle_fuel_qty_2, 2, _V(0.735534, 2.013132, 14.920000), _V(1.0, 0.0, 0.0), (float)(RAD * 16.9334));
		mgr_needle_fuel_qty_3 = new MGROUP_ROTATE(GetVCMeshIndex(), needle_fuel_qty_3, 1, _V(0.735534, 2.013132, 14.920000), _V(1.0, 0.0, 0.0), (float)(RAD * 16.9334));
		anim_needle_fuel_qty_1 = STS()->CreateAnimation(0.0);
		anim_needle_fuel_qty_2 = STS()->CreateAnimation(0.0);
		anim_needle_fuel_qty_3 = STS()->CreateAnimation(0.0);
		STS()->AddAnimationComponent(anim_needle_fuel_qty_1, 0, 1, mgr_needle_fuel_qty_1);
		STS()->AddAnimationComponent(anim_needle_fuel_qty_2, 0, 1, mgr_needle_fuel_qty_2);
		STS()->AddAnimationComponent(anim_needle_fuel_qty_3, 0, 1, mgr_needle_fuel_qty_3);

		static UINT needle_apu_egt[1] = { 97 };
		static UINT needle_apu_fuel_press[1] = { 96 };
		static UINT needle_apu_oil_temp[1] = { 95 };
		mgr_needle_apu_egt = new MGROUP_ROTATE(GetVCMeshIndex(), needle_apu_egt, 1, _V(0.735534, 2.013132, 14.920000), _V(1.0, 0.0, 0.0), (float)(RAD * 16.054));
		mgr_needle_apu_fuel_press = new MGROUP_ROTATE(GetVCMeshIndex(), needle_apu_fuel_press, 1, _V(0.735534, 2.013132, 14.920000), _V(1.0, 0.0, 0.0), (float)(RAD * 16.054));
		mgr_needle_apu_oil_temp = new MGROUP_ROTATE(GetVCMeshIndex(), needle_apu_oil_temp, 1, _V(0.735534, 2.013132, 14.920000), _V(1.0, 0.0, 0.0), (float)(RAD * 16.054));
		anim_needle_apu_egt = STS()->CreateAnimation(0.0);
		anim_needle_apu_fuel_press = STS()->CreateAnimation(0.0);
		anim_needle_apu_oil_temp = STS()->CreateAnimation(0.0);
		STS()->AddAnimationComponent(anim_needle_apu_egt, 0, 1, mgr_needle_apu_egt);
		STS()->AddAnimationComponent(anim_needle_apu_fuel_press, 0, 1, mgr_needle_apu_fuel_press);
		STS()->AddAnimationComponent(anim_needle_apu_oil_temp, 0, 1, mgr_needle_apu_oil_temp);
	}

	void PanelF8::OnPreStep(double simt, double simdt, double mjd)
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
			atterr = pPLT1->GetIDP()->GetAttitudeErrors_AscentDAP();
			break;
		case 201:
		case 202:
		case 801:
			atterr = pPLT1->GetIDP()->GetAttitudeCommandErrors();
			break;
		case 301:
		case 302:
		case 303:
		case 304:
		case 305:
			atterr = pPLT1->GetIDP()->GetAttitudeErrors_AerojetDAP();
			break;
		case 601:
			atterr = pPLT1->GetIDP()->GetAttitudeErrors_AscentDAP();
			break;
		case 602:
		case 603:
		case 901:
			atterr = pPLT1->GetIDP()->GetAttitudeErrors_AerojetDAP();
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
		int adirate = pPLT1->GetIDP()->GetADIRate();
		double maxRate = (adirate == 1) ? 5.0 : (adirate == 2) ? 10.0 : 1.0;

		STS()->SetAnimation(anim_needle_RollRateCDR, max(0.0, min(1.0, 0.5 + av.z / (2.0 * maxRate))));
		STS()->SetAnimation(anim_needle_PitchRateCDR, max(0.0, min(1.0, 0.5 + av.x / (2.0 * maxRate))));
		STS()->SetAnimation(anim_needle_YawRateCDR, max(0.0, min(1.0, 0.5 + av.y / (2.0 * maxRate))));

		// Error needles
		int adierr = pPLT1->GetIDP()->GetADIError();
		double maxRate2 = (adierr == 1) ? 5.0 : (adierr == 2) ? 10.0 : 1.0;

		STS()->SetAnimation(anim_needle_RollErrorCDR, max(0.0, min(1.0, 0.5 + atterr.z / (2.0 * maxRate2))));
		STS()->SetAnimation(anim_needle_PitchErrorCDR, max(0.0, min(1.0, 0.5 + atterr.x / (2.0 * maxRate2))));
		STS()->SetAnimation(anim_needle_YawErrorCDR, max(0.0, min(1.0, 0.5 + atterr.y / (2.0 * maxRate2))));

		// Tapemeters
		// Alpha
		double alpha = STS()->GetAOA() * DEG;

		auto updateAlpha = [&](DWORD group, const std::vector<NTVERTEX>& originalVtx, const std::vector<float>& originalTV, std::vector<WORD>& idx, DWORD nVtx, double minAlpha, double maxAlpha, float range)
			{
				if (!hF8_dev || originalVtx.empty()) return;

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

				oapiEditMeshGroup(hF8_dev, group, &ges);
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
		if (hF8_dev && !mVtx.empty())
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

			oapiEditMeshGroup(hF8_dev, 77, &ges);
		}

		//KT tapemeter
		if (hF8_dev && !ktVtx.empty())
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

			oapiEditMeshGroup(hF8_dev, 78, &ges);
		}

		//Hdot tapemeter
		if (hF8_dev && !hdotVtx.empty())
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

			oapiEditMeshGroup(hF8_dev, 81, &ges);
		}

		//Alt tapemeter
		if (hF8_dev && !altVtx.empty())
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

			oapiEditMeshGroup(hF8_dev, 82, &ges);
		}

		//Alpha needle
		double limitedAlpha = (alpha < -4.0) ? -4.0 : ((alpha > 28.0) ? 28.0 : alpha);
		STS()->SetAnimation(anim_needle_alpha, (limitedAlpha + 4.0) / 32.0);

		//Alt needle
		int altAcc = pPLT1->GetIDP()->GetVacc();
		double limitedAlt = (altAcc < -10.0) ? -10.0 : ((altAcc > 10.0) ? 10.0 : altAcc);
		STS()->SetAnimation(anim_needle_alt, (limitedAlt + 10.0) / 20.0);

		//HYDRAULIC
		unsigned short Qty[3];
		unsigned short Press[3];
		pPLT1->GetIDP()->GetHYDdata(Qty[0], Qty[1], Qty[2], Press[0], Press[1], Press[2]);

		for (int i = 0; i < 3; i++) if (Qty[i] > 100.0) Qty[i] = 100.0;

		STS()->SetAnimation(anim_needle_hyd_qty_1, Qty[0] / 100.0);
		STS()->SetAnimation(anim_needle_hyd_qty_2, Qty[1] / 100.0);
		STS()->SetAnimation(anim_needle_hyd_qty_3, Qty[2] / 100.0);

		for (int i = 0; i < 3; i++) if (Press[i] > 4000.0) Press[i] = 4000.0;

		STS()->SetAnimation(anim_needle_hyd_press_1, Press[0] / 4000.0);
		STS()->SetAnimation(anim_needle_hyd_press_2, Press[1] / 4000.0);
		STS()->SetAnimation(anim_needle_hyd_press_3, Press[2] / 4000.0);

		//APU
		unsigned short FuQty[3];
		unsigned short Fu_Press[3];
		unsigned short H2OQty[3];
		unsigned short OilIn[3];
		pPLT1->GetIDP()->GetAPUdata(FuQty[0], FuQty[1], FuQty[2], Fu_Press[0], Fu_Press[1], Fu_Press[2], H2OQty[0], H2OQty[1], H2OQty[2], OilIn[0], OilIn[1], OilIn[2]);

		for (int i = 0; i < 3; i++) if (FuQty[i] > 100.0) FuQty[i] = 100.0;
		STS()->SetAnimation(anim_needle_fuel_qty_1, FuQty[0] / 100.0);
		STS()->SetAnimation(anim_needle_fuel_qty_2, FuQty[1] / 100.0);
		STS()->SetAnimation(anim_needle_fuel_qty_3, FuQty[2] / 100.0);

		for (int i = 0; i < 3; i++) if (Fu_Press[i] > 500.0) Fu_Press[i] = 500.0;
		STS()->SetAnimation(anim_needle_apu_fuel_press, Fu_Press[0] / 500.0);

		for (int i = 0; i < 3; i++) if (OilIn[i] > 500.0) OilIn[i] = 500.0;
		STS()->SetAnimation(anim_needle_apu_oil_temp, OilIn[0] / 500.0);

		//sprintf(oapiDebugString(), "Fu_Press:%i", Fu_Press[0]);
	}

	void PanelF8::VisualCreated()
	{
		hF8_dev = STS()->GetDevMesh(STS()->Get_vis(), GetVCMeshIndex());

		MESHGROUP* gAlpha1 = oapiMeshGroup(GetVCMeshHandle(), 88);
		MESHGROUP* gAlpha2 = oapiMeshGroup(GetVCMeshHandle(), 80);
		MESHGROUP* gAccel = oapiMeshGroup(GetVCMeshHandle(), 76);
		MESHGROUP* gM = oapiMeshGroup(GetVCMeshHandle(), 77);
		MESHGROUP* gKT = oapiMeshGroup(GetVCMeshHandle(), 78);
		MESHGROUP* gHdot = oapiMeshGroup(GetVCMeshHandle(), 81);
		MESHGROUP* gAlt = oapiMeshGroup(GetVCMeshHandle(), 82);

		if (!hF8_dev || !gAlpha1 || !gAlpha2 || !gAccel || !gM || !gKT || !gHdot || !gAlt) return;

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

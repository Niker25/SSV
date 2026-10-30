/******* SSV File Modification Notice *******
Date         Developer
2020/06/20   GLS
2020/06/23   GLS
2021/06/28   GLS
2021/08/23   GLS
2021/08/24   GLS
2021/09/20   GLS
2021/10/23   GLS
2022/02/07   GLS
2022/03/22   GLS
2022/03/24   GLS
2022/03/26   GLS
2022/04/19   GLS
2022/04/20   GLS
2022/08/05   GLS
2022/09/29   GLS
2024/12/30   GLS
********************************************/
#include "AftMDU.h"
#include "MDU.h"
#include "../Atlantis.h"
#include "../meshres_vc.h"
#include "../ParameterValues.h"
#include "vc_defs.h"
#include "../meshres_vc_a134.h"
#include "../dps/IDP.h"


namespace vc
{
	AftMDU::AftMDU(Atlantis* _sts, bool hasMEDS)
		: AtlantisPanel(_sts, "AftMDU")
	{
		bHasMEDS = hasMEDS;

		if (bHasMEDS) DefineMesh(MESHNAME_PANELA134);
		else DefineMesh( MESHNAME_PANELA134_MCD );

		Add(pAFD = new MDU(_sts, "AFD1", MDUID_AFD1));
	}

	AftMDU::~AftMDU()
	{
		delete mgr_adiR, mgr_adiP, mgr_adiY;
		delete ach_ADIroll, ach_ADIpitch, ach_ADIyaw;
		delete mgr_needle_RollRateCDR, mgr_needle_PitchRateCDR, mgr_needle_YawRateCDR;
		delete mgr_needle_RollErrorCDR, mgr_needle_PitchErrorCDR, mgr_needle_YawErrorCDR;
	}

	void AftMDU::DefineVC()
	{
		AddAIDToMouseEventList(AID_AFTMDU);

		if (bHasMEDS) pAFD->SetMouseRegion(AID_AFTMDU, 0.0, 0.0, 1.0, 1.0);
		pAFD->DefineVCGroup( GRP_AFD1_A134_VC );

		if (bHasMEDS) pAFD->DefineMaterial(MAT_MDU_AFD1_A134_VC);
		else pAFD->DefineMaterial( 2 );

		DefineVCAnimations();
	}

	void AftMDU::RegisterVC()
	{
		AtlantisPanel::RegisterVC();

		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea( AID_AFTMDU, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED );

		oapiVCSetAreaClickmode_Quadrilateral( AID_AFTMDU,
			_V( 0.776, 3.314, 12.458 ) + ofs, _V( 0.543, 3.297, 12.359 ) + ofs,
			_V( 0.793, 3.089, 12.45 ) + ofs, _V( 0.562, 3.072, 12.352 ) + ofs );
	}

	void AftMDU::OnPreStep(double simt, double simdt, double mjd)
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
				atterr = pAFD->GetIDP()->GetAttitudeErrors_AscentDAP();
				break;
			case 201:
			case 202:
			case 801:
				atterr = pAFD->GetIDP()->GetAttitudeCommandErrors();
				break;
			case 301:
			case 302:
			case 303:
			case 304:
			case 305:
				atterr = pAFD->GetIDP()->GetAttitudeErrors_AerojetDAP();
				break;
			case 601:
				atterr = pAFD->GetIDP()->GetAttitudeErrors_AscentDAP();
				break;
			case 602:
			case 603:
			case 901:
				atterr = pAFD->GetIDP()->GetAttitudeErrors_AerojetDAP();
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
			int adirate = pAFD->GetIDP()->GetADIRate();
			double maxRate = (adirate == 1) ? 5.0 : (adirate == 2) ? 10.0 : 1.0;

			STS()->SetAnimation(anim_needle_RollRateCDR, max(0.0, min(1.0, 0.5 + av.z / (2.0 * maxRate))));
			STS()->SetAnimation(anim_needle_PitchRateCDR, max(0.0, min(1.0, 0.5 + av.x / (2.0 * maxRate))));
			STS()->SetAnimation(anim_needle_YawRateCDR, max(0.0, min(1.0, 0.5 + av.y / (2.0 * maxRate))));

			// Error needles
			int adierr = pAFD->GetIDP()->GetADIError();
			double maxRate2 = (adierr == 1) ? 5.0 : (adierr == 2) ? 10.0 : 1.0;

			STS()->SetAnimation(anim_needle_RollErrorCDR, max(0.0, min(1.0, 0.5 + atterr.z / (2.0 * maxRate2))));
			STS()->SetAnimation(anim_needle_PitchErrorCDR, max(0.0, min(1.0, 0.5 + atterr.x / (2.0 * maxRate2))));
			STS()->SetAnimation(anim_needle_YawErrorCDR, max(0.0, min(1.0, 0.5 + atterr.y / (2.0 * maxRate2))));
		}
	}

	void AftMDU::DefineVCAnimations()
	{
		if (!bHasMEDS)
		{
			//CDR ADI Ball
			static UINT meshgroup_Fdai1_L = { 5 }; // Roll gimbal meshgroup (includes roll indicator)
			static UINT meshgroup_Fdai2_L = { 6 };  // Pitch gimbal meshgroup (visible ball)
			static UINT meshgroup_Fdai3_L = { 7 }; // Yaw gimbal meshgroup
			static MGROUP_ROTATE mgt_FDAIRoll_L(GetVCMeshIndex(), &meshgroup_Fdai1_L, 1, _V(0.692683, 3.196220, 12.342807), _V(-0.393054, 0.000323, 0.919515), (float)(RAD * 360));
			static MGROUP_ROTATE mgt_FDAIPitch_L(GetVCMeshIndex(), &meshgroup_Fdai2_L, 1, _V(0.692683, 3.196220, 12.342807), _V(0.919509, 0.000000, 0.392867), (float)(RAD * 360));
			static MGROUP_ROTATE mgt_FDAIYaw_L(GetVCMeshIndex(), &meshgroup_Fdai3_L, 1, _V(0.692683, 3.196220, 12.342807), _V(0.000129, 1.000000, -0.000296), (float)(RAD * 360));
			anim_adiR = STS()->CreateAnimation(0.0);
			anim_adiP = STS()->CreateAnimation(0.0);
			anim_adiY = STS()->CreateAnimation(0.0);
			ach_ADIroll = STS()->AddAnimationComponent(anim_adiR, 0.0f, 1.0f, &mgt_FDAIRoll_L);
			ach_ADIyaw = STS()->AddAnimationComponent(anim_adiY, 0.0f, 1.0f, &mgt_FDAIYaw_L, ach_ADIroll);
			ach_ADIpitch = STS()->AddAnimationComponent(anim_adiP, 0.0f, 1.0f, &mgt_FDAIPitch_L, ach_ADIyaw);

			//CDR ADI Rate needles
			static UINT needle_RollRateCDR_group[1] = { 12 };
			static UINT needle_PitchRateCDR_group[1] = { 11 };
			static UINT needle_YawRateCDR_group[1] = { 13 };
			mgr_needle_RollRateCDR = new MGROUP_TRANSLATE(GetVCMeshIndex(), needle_RollRateCDR_group, 1, _V(-0.028345, 0, -0.01211) * 2);
			mgr_needle_PitchRateCDR = new MGROUP_TRANSLATE(GetVCMeshIndex(), needle_PitchRateCDR_group, 1, _V(0, -0.030902, 0) * 2);
			mgr_needle_YawRateCDR = new MGROUP_TRANSLATE(GetVCMeshIndex(), needle_YawRateCDR_group, 1, _V(-0.028345, 0, -0.01211) * 2);

			anim_needle_RollRateCDR = STS()->CreateAnimation(0.5);
			STS()->AddAnimationComponent(anim_needle_RollRateCDR, 0, 1, mgr_needle_RollRateCDR);

			anim_needle_PitchRateCDR = STS()->CreateAnimation(0.5);
			STS()->AddAnimationComponent(anim_needle_PitchRateCDR, 0, 1, mgr_needle_PitchRateCDR);

			anim_needle_YawRateCDR = STS()->CreateAnimation(0.5);
			STS()->AddAnimationComponent(anim_needle_YawRateCDR, 0, 1, mgr_needle_YawRateCDR);

			//CDR ADI Error needles
			static UINT needle_RollErrorCDR_group[1] = { 14 };
			static UINT needle_PitchErrorCDR_group[1] = { 10 };
			static UINT needle_YawErrorCDR_group[1] = { 15 };
			mgr_needle_RollErrorCDR = new MGROUP_TRANSLATE(GetVCMeshIndex(), needle_RollErrorCDR_group, 1, _V(0.019733, 0.008431, 0) * 2);
			mgr_needle_PitchErrorCDR = new MGROUP_TRANSLATE(GetVCMeshIndex(), needle_PitchErrorCDR_group, 1, _V(0, 0.020829, 0.0) * 2);
			mgr_needle_YawErrorCDR = new MGROUP_TRANSLATE(GetVCMeshIndex(), needle_YawErrorCDR_group, 1, _V(0.019733, 0.008431, 0) * 2);

			anim_needle_RollErrorCDR = STS()->CreateAnimation(0.5);
			STS()->AddAnimationComponent(anim_needle_RollErrorCDR, 0, 1, mgr_needle_RollErrorCDR);

			anim_needle_PitchErrorCDR = STS()->CreateAnimation(0.5);
			STS()->AddAnimationComponent(anim_needle_PitchErrorCDR, 0, 1, mgr_needle_PitchErrorCDR);

			anim_needle_YawErrorCDR = STS()->CreateAnimation(0.5);
			STS()->AddAnimationComponent(anim_needle_YawErrorCDR, 0, 1, mgr_needle_YawErrorCDR);
		}
	}
}

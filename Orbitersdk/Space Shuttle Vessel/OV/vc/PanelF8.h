/****************************************************************************
  This file is part of Space Shuttle Vessel

  Panel F8 definition



  Space Shuttle Vessel is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Space Shuttle Vessel is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Space Shuttle Vessel; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  See https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html or
  file SSV-LICENSE.txt for more details.

  **************************************************************************/
/******* SSV File Modification Notice *******
Date         Developer
2020/04/01   GLS
2020/05/08   GLS
2020/05/10   GLS
2020/06/20   GLS
2021/08/24   GLS
2021/12/26   GLS
2022/08/05   GLS
2022/08/27   GLS
2022/09/29   GLS
********************************************/
/****************************************************************************
  This file is part of Space Shuttle Ultra

  Panel F8 definition



  Space Shuttle Ultra is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Space Shuttle Ultra is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Space Shuttle Ultra; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  See https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html or
  file Doc\Space Shuttle Ultra\GPL.txt for more details.

  **************************************************************************/
#ifndef _VC_PANELF8_H_
#define _VC_PANELF8_H_


#include "AtlantisPanel.h"


namespace vc
{
	class MDU;
	class StdSwitch2;
	class StdSwitch3;
	class LockableLever2;
	class StandardTalkback3;
	class StandardSwitchCover;
	class PushButtonIndicatorSingleLight;
	class RotarySwitchPotentiometer;


	class PanelF8: public AtlantisPanel
	{
		MDU* pPLT1;
		MDU* pPLT2;
		//StdSwitch3* pHSISelectMode;
		//StdSwitch3* pHSISelectSource;
		//StdSwitch3* pHSISelectChannel;
		LockableLever2* pFltCntlrPower;
		StdSwitch3* pADIAttitude;
		StdSwitch3* pADIError;
		StdSwitch3* pADIRate;
		//StdSwitch3* pAirData;

		StandardTalkback3* pLandingGearTB[3];

		StandardSwitchCover* pLandingGearArmDeployCover[2];
		PushButtonIndicatorSingleLight *pLandingGearArmDeploy[2];

		StdSwitch3* pHUDMode;
		RotarySwitchPotentiometer* pHUDBrightness;
		StdSwitch3* pHUDBright;

		StdSwitch2* pRDRALTM;

	public:
		explicit PanelF8(Atlantis* _sts, bool hasMEDS);
		virtual ~PanelF8();

		void DefineVC() override;
		void RegisterVC() override;
		void Realize() override;
		void OnPreStep(double simt, double simdt, double mjd) override;
		void DefineVCAnimations();
		void VisualCreated() override;

		UINT anim_adiR = -1, anim_adiP = -1, anim_adiY = -1;
		MGROUP_TRANSFORM* mgr_adiR = nullptr, * mgr_adiP = nullptr, * mgr_adiY = nullptr;
		ANIMATIONCOMPONENT_HANDLE ach_ADIroll = nullptr, ach_ADIpitch = nullptr, ach_ADIyaw = nullptr;

		//Anim Error Needles//////////////////////////////////////////////////
		UINT anim_needle_RollRateCDR = -1, anim_needle_PitchRateCDR = -1, anim_needle_YawRateCDR = -1;
		MGROUP_TRANSFORM* mgr_needle_RollRateCDR = nullptr, * mgr_needle_PitchRateCDR = nullptr, * mgr_needle_YawRateCDR = nullptr;

		UINT anim_needle_RollErrorCDR = -1, anim_needle_PitchErrorCDR = -1, anim_needle_YawErrorCDR = -1;
		MGROUP_TRANSFORM* mgr_needle_RollErrorCDR = nullptr, * mgr_needle_PitchErrorCDR = nullptr, * mgr_needle_YawErrorCDR = nullptr;

		//AMI
		DEVMESHHANDLE hF8_dev = nullptr;
		std::vector<NTVERTEX> alpha1Vtx, alpha2Vtx, accelVtx, mVtx, ktVtx, hdotVtx, altVtx;
		std::vector<WORD> alpha1Idx, alpha2Idx, accelIdx, mIdx, ktIdx, hdotIdx, altIdx;
		std::vector<float> alpha1TV, alpha2TV, accelTV, mTV, ktTV, hdotTV, altTV;
		GROUPEDITSPEC alpha1Edit = {}, alpha2Edit = {}, accelEdit = {}, mEdit = {}, ktEdit = {}, hdotEdit = {}, altEdit = {};

		UINT anim_needle_alpha = -1, anim_needle_alt = -1;
		MGROUP_TRANSFORM* mgr_needle_alpha = nullptr, * mgr_needle_alt = nullptr;

		//Hydraulic Needles
		UINT anim_needle_hyd_press_1 = -1, anim_needle_hyd_press_2 = -1, anim_needle_hyd_press_3 = -1;
		MGROUP_TRANSFORM* mgr_needle_hyd_press_1 = nullptr, * mgr_needle_hyd_press_2 = nullptr, * mgr_needle_hyd_press_3 = nullptr;
		UINT anim_needle_hyd_qty_1 = -1, anim_needle_hyd_qty_2 = -1, anim_needle_hyd_qty_3 = -1;
		MGROUP_TRANSFORM* mgr_needle_hyd_qty_1 = nullptr, * mgr_needle_hyd_qty_2 = nullptr, * mgr_needle_hyd_qty_3 = nullptr;

		//APU Needles
		UINT anim_needle_fuel_qty_1 = -1, anim_needle_fuel_qty_2 = -1, anim_needle_fuel_qty_3 = -1;
		MGROUP_TRANSFORM* mgr_needle_fuel_qty_1 = nullptr, * mgr_needle_fuel_qty_2 = nullptr, * mgr_needle_fuel_qty_3 = nullptr;

		UINT anim_needle_apu_egt = -1, anim_needle_apu_fuel_press = -1, anim_needle_apu_oil_temp = -1;
		MGROUP_TRANSFORM* mgr_needle_apu_egt = nullptr, * mgr_needle_apu_fuel_press = nullptr, * mgr_needle_apu_oil_temp = nullptr;

		bool bHasMEDS = false;

	};
}

#endif// _VC_PANELF8_H_

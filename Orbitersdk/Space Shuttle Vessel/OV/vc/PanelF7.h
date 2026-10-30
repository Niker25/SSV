/****************************************************************************
  This file is part of Space Shuttle Vessel

  Panel F7 definition



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
2021/08/23   GLS
2021/08/24   GLS
2021/12/26   GLS
2022/07/02   GLS
2022/07/24   GLS
2022/09/25   GLS
2022/09/29   GLS
********************************************/
/****************************************************************************
  This file is part of Space Shuttle Ultra

  Panel F7 definition



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
#ifndef _VC_PANELF7_H_
#define _VC_PANELF7_H_


#include "AtlantisPanel.h"
#include <discsignals.h>


namespace vc
{
	class MDU;
	class _7SegDisp_EventTime;
	class StandardSingleLight;
	class StandardDoubleLight;
	class LockableLever2;
	class StdSwitch2;

	using namespace discsignals;


	class PanelF7: public AtlantisPanel
	{
	private:
		MDU* pCRT1;
		MDU* pCRT2;
		MDU* pCRT3;
		MDU* pMFD1;
		MDU* pMFD2;

		_7SegDisp_EventTime* pEventTime;

		StandardSingleLight* pCWLight[40];
		StandardDoubleLight* pMainEngStatus[3];
		StandardSingleLight* pSMAlert;

		DiscInPort lighttestleft[2];
		DiscInPort lighttestright[2];
		DiscInPort lightcmd[40];
		DiscOutPort lightoutput[40];

		DiscInPort mar[2];// F2, F4
		DiscInPort modenorm;
		DiscInPort modeascent;
		DiscInPort cw_a;

		LockableLever2* pFltCntlrPower;
		StdSwitch2* pRDRALTM;

	public:
		explicit PanelF7(Atlantis* _sts, bool hasMEDS);
		virtual ~PanelF7();

		void DefineVC() override;
		void RegisterVC() override;
		void Realize() override;
		void OnPostStep( double simt, double simdt, double mjd ) override;
		void DefineVCAnimations();

		//OMS Press
		UINT anim_needle_oms_pc_left = -1, anim_needle_oms_pc_right = -1, anim_needle_oms_he_left = -1, anim_needle_oms_he_right = -1;
		MGROUP_TRANSFORM* mgr_needle_oms_pc_left = nullptr, *mgr_needle_oms_pc_right = nullptr, *mgr_needle_oms_he_left = nullptr, *mgr_needle_oms_he_right = nullptr;

		//MPS Press
		UINT anim_needle_mps_pc_L = -1,
			anim_needle_mps_pc_C = -1,
			anim_needle_mps_pc_R = -1,
			anim_needle_mps_LO2 = -1,
			anim_needle_mps_LH2 = -1,
			anim_needle_mps_he_PNEU = -1,
			anim_needle_mps_he_L = -1,
			anim_needle_mps_he_C = -1,
			anim_needle_mps_he_R = -1;
		MGROUP_TRANSFORM* mgr_needle_mps_pc_L = nullptr,
			* mgr_needle_mps_pc_C = nullptr,
			* mgr_needle_mps_pc_R = nullptr,
			* mgr_needle_mps_LO2 = nullptr,
			* mgr_needle_mps_LH2 = nullptr,
			* mgr_needle_mps_he_PNEU = nullptr,
			* mgr_needle_mps_he_L = nullptr,
			* mgr_needle_mps_he_C = nullptr,
			* mgr_needle_mps_he_R = nullptr;

		//G-Meter
		UINT anim_needle_g_meter = -1;
		MGROUP_TRANSFORM* mgr_needle_g_meter = nullptr;

		//SPI
		UINT anim_needle_LOB = -1, anim_needle_LIB = -1, anim_needle_RIB = -1, anim_needle_ROB = -1, anim_needle_DAFB = -1, anim_needle_DRFB = -1, anim_needle_DBFOFB = -1, anim_needle_Speedbrake_Pos = -1, anim_needle_Speedbrake_Cmd = -1;
		MGROUP_TRANSFORM *mgr_needle_LOB = nullptr, * mgr_needle_LIB = nullptr, * mgr_needle_RIB = nullptr, * mgr_needle_ROB = nullptr, * mgr_needle_DAFB = nullptr, * mgr_needle_DRFB = nullptr, * mgr_needle_DBFOFB = nullptr, * mgr_needle_Speedbrake_Pos = nullptr, * mgr_needle_Speedbrake_Cmd = nullptr;

		bool bHasMEDS = false;
	};
}

#endif// _VC_PANELF7_H_

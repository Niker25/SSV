/****************************************************************************
  This file is part of Space Shuttle Vessel

  Aft MDU panel definition


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
2020/05/08   GLS
2020/05/10   GLS
2020/06/20   GLS
2021/08/24   GLS
2021/12/26   GLS
2022/08/05   GLS
2022/09/29   GLS
********************************************/
/****************************************************************************
  This file is part of Space Shuttle Ultra

  Aft MDU panel definition

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
#ifndef _AFTMDU_H
#define _AFTMDU_H


#include "AtlantisPanel.h"


namespace vc
{
	class MDU;


	class AftMDU : public AtlantisPanel
	{
	private:
		MDU* pAFD;
	public:
		explicit AftMDU(Atlantis* _sts, bool hasMEDS);
		virtual ~AftMDU();

		void DefineVC() override;
		void RegisterVC() override;

		void DefineVCAnimations();

		void OnPreStep(double simt, double simdt, double mjd) override;

		UINT anim_adiR = -1, anim_adiP = -1, anim_adiY = -1;
		MGROUP_TRANSFORM* mgr_adiR = nullptr, * mgr_adiP = nullptr, * mgr_adiY = nullptr;
		ANIMATIONCOMPONENT_HANDLE ach_ADIroll = nullptr, ach_ADIpitch = nullptr, ach_ADIyaw = nullptr;

		//Anim Error Needles//////////////////////////////////////////////////
		UINT anim_needle_RollRateCDR = -1, anim_needle_PitchRateCDR = -1, anim_needle_YawRateCDR = -1;
		MGROUP_TRANSFORM* mgr_needle_RollRateCDR = nullptr, * mgr_needle_PitchRateCDR = nullptr, * mgr_needle_YawRateCDR = nullptr;

		UINT anim_needle_RollErrorCDR = -1, anim_needle_PitchErrorCDR = -1, anim_needle_YawErrorCDR = -1;
		MGROUP_TRANSFORM* mgr_needle_RollErrorCDR = nullptr, * mgr_needle_PitchErrorCDR = nullptr, * mgr_needle_YawErrorCDR = nullptr;

		bool bHasMEDS = false;
	};
}

#endif //_AFTMDU_H

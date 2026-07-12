modded class DayZPlayerImplementAiming {

	static vector lastWeaponDir, lastCameraDir;

	static void UpdateLastPositions() {
		PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
		if (!player)
			return;
		Weapon_Base wpn = Weapon_Base.Cast(player.GetHumanInventory().GetEntityInHands());
		if (!wpn)
			return;
		vector konec_hlavne_position = wpn.ModelToWorld(wpn.GetMemoryPointPos("konec hlavne"));
		vector usti_hlavne_position = wpn.ModelToWorld(wpn.GetMemoryPointPos("usti hlavne"));
		lastWeaponDir = usti_hlavne_position - konec_hlavne_position;
		lastCameraDir = g_Game.GetCurrentCameraDirection();
	}

	override bool ProcessAimFilters(float pDt, SDayZPlayerAimingModel pModel, int stance_index) {
		UpdateLastPositions();
		bool result = super.ProcessAimFilters(pDt, pModel, stance_index);
		PlayerBase.lbOffsets = 0;
		PlayerBase.lbOffsets += Math.AbsFloat(pModel.m_fAimXMouseShift);
		PlayerBase.lbOffsets += Math.AbsFloat(pModel.m_fAimYMouseShift);
		PlayerBase.lbOffsets += Math.AbsFloat(pModel.m_fAimXHandsOffset);
		PlayerBase.lbOffsets += Math.AbsFloat(pModel.m_fAimYHandsOffset);
		PlayerBase.lbOffsets += Math.AbsFloat(pModel.m_fAimXCamOffset);
		PlayerBase.lbOffsets += Math.AbsFloat(pModel.m_fAimYCamOffset);
		return result;
	}

}
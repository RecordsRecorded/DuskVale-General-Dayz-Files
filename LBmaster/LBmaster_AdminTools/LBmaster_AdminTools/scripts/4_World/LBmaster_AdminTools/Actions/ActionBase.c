#ifndef SERVER
modded class ActionBase {

	override bool Can(PlayerBase player, ActionTarget target, ItemBase item, int condition_mask) {
		if (LBFreecamCamera.instance && LBAdmins.Get().HasPermission("admin.execute.freecam", player)) {
			if (m_ConditionTarget)
				m_ConditionTarget.LBOverwriteDistance();
			bool ok = super.Can(player, target, item, condition_mask);
			if (m_ConditionTarget)
				m_ConditionTarget.LBResetOverwriteDistance();
			return ok;
		}
		return super.Can(player, target, item, condition_mask);
	}

	override protected bool IsInReach(PlayerBase player, ActionTarget target, float maxDistance = 1.0) {
		if (LBFreecamCamera.instance && LBAdmins.Get().HasPermission("admin.execute.freecam", player))
			return true;
		return super.IsInReach(player, target, maxDistance);
	}

}
#endif
modded class ActionTargets {

	override private float ComputeUtility(Object pTarget, vector pRayStart, vector pRayEnd, Object cursorTarget, vector hitPos) {
		if (LBFreecamCamera.instance && LBAdmins.Get().HasPermission("admin.execute.freecam", m_Player)) {
			return super.ComputeUtility(pTarget, pRayStart, pRayEnd, cursorTarget, m_Player.GetPosition());
		}
		return super.ComputeUtility(pTarget, pRayStart, pRayEnd, cursorTarget, hitPos);
	}

}
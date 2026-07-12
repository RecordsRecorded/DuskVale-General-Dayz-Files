modded class InventoryMenu {

	override void OnShow() {
		super.OnShow();
		PlayerBase myPlayer = PlayerBase.Cast(g_Game.GetPlayer());
		if (DayZPlayerCameraSpectator.currentSpecCamera && DayZPlayerCameraSpectator.currentSpecCamera.IsActive() && DayZPlayerCameraSpectator.currentSpecCamera.targetObj) {
			m_Inventory.LBSetPlayer(DayZPlayerCameraSpectator.currentSpecCamera.targetObj);
			m_Inventory.LBGetPlayerPreview().LBOverwritePlayer(DayZPlayerCameraSpectator.currentSpecCamera.targetObj);
		} else if (myPlayer && PlayerBase.lb_wasSpectating) {
			m_Inventory.LBResetPlayer();
			m_Inventory.LBGetPlayerPreview().LBResetPlayer();
			PlayerBase.lb_wasSpectating = false;
		} else if (myPlayer && myPlayer.GetCurrentCommandID() == -1) {
			g_Game.RPCSingleParam(null, LBAdminToolRPCs.GROUND_INFO, new Param1<vector>(myPlayer.GetPosition()), true);
		}
	}

}
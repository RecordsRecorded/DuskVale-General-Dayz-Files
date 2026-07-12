#ifdef LBmaster_GroupDLCPlotpole
modded class ActionDigGardenPlot {

	override bool Can(PlayerBase player, ActionTarget target, ItemBase item, int condition_mask) {
		if (!super.Can(player, target, item, condition_mask))
			return false;
		vector pos = player.GetPosition();
		if (g_Game.IsClient())
			pos = player.GetHologramLocal().GetProjectionEntity().GetPosition();
		else if (g_Game.IsServer() && player.GetHologramServer() && player.GetHologramServer().GetProjectionEntity()) {
			pos = player.GetHologramServer().GetProjectionEntity().GetPosition();
		}
		return LBTerritoryConfig.Get.CanExecuteAction(player, pos, 0, item);
	}

}
#endif
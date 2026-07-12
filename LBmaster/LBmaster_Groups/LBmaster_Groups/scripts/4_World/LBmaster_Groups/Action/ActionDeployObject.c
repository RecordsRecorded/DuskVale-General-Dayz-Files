modded class ActionDeployObject {

	override bool SetupAction(PlayerBase player, ActionTarget target, ItemBase item, out ActionData action_data, Param extra_data = NULL) {
		LB_NoBuildEntry zone;
		float additional = 0.0;
#ifdef LBmaster_GroupDLCPlotpole
		if (item.IsInherited(TerritoryFlagKit)) {
			additional = LBTerritoryConfig.Get.GetMaxRadius();
		}
#endif
		if (!LBAdmins.Get().HasPermissionActive("groups.ignore.nobuildzones", player) && LB_NoBuildConfig.Get.IsInZone(player.GetPosition(), zone, additional, player)) {
			if (LB_NoBuildConfig.Get.ignoreItemsAll.Find(item.GetType()) == -1) {
				string mess = LBStringTools.MultiTranslateString(LB_NoBuildConfig.Get.notificationMessage);
				mess.Replace("{pos}", zone.name);
				NotificationSystem.AddNotificationExtended(5, LB_NoBuildConfig.Get.notificationTitle, mess, "set:ccgui_enforce image:HudBuild");
				return false;
			}
		}
		return super.SetupAction(player, target, item, action_data, extra_data);
	}

#ifdef LBmaster_GroupDLCPlotpole
	override bool Can(PlayerBase player, ActionTarget target, ItemBase item, int condition_mask) {
		if (!super.Can(player, target, item, condition_mask))
			return false;
		int type = 0;
		if (item.IsInherited(TerritoryFlagKit)) {
			type = 4;
		}
		vector pos = player.GetPosition();
		if (g_Game.IsClient() && player.GetHologramLocal())
			pos = player.GetHologramLocal().GetProjectionEntity().GetPosition();
		else if (g_Game.IsServer() && player.GetHologramServer() && player.GetHologramServer().GetProjectionEntity()) {
			pos = player.GetHologramServer().GetProjectionEntity().GetPosition();
		}
		return LBTerritoryConfig.Get.CanExecuteAction(player, pos, type, item);
	}
#endif

}

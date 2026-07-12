modded class ActionArmExplosive {

	override bool SetupAction(PlayerBase player, ActionTarget target, ItemBase item, out ActionData action_data, Param extra_data = NULL) {
		LB_NoBuildEntry zone;
		ItemBase obj = ItemBase.Cast(target.GetObject());
		if (!obj)
			return super.SetupAction(player, target, item, action_data, extra_data);
		if (!LBAdmins.Get().HasPermissionActive("groups.ignore.nobuildzones", player) && LB_NoBuildConfig.Get.IsInZone(player.GetPosition(), zone, 0, player)) {
			if (LB_NoBuildConfig.Get.ignoreItemsAll.Find(obj.GetType()) == -1) {
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
		ItemBase obj = ItemBase.Cast(target.GetObject());
		if (obj)
			return LBTerritoryConfig.Get.CanExecuteAction(player, obj.GetPosition(), 0, obj);
		return true;
	}
#endif
}
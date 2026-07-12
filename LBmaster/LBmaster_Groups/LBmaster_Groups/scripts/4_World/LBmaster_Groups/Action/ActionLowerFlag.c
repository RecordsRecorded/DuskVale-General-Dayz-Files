#ifdef LBmaster_GroupDLCPlotpole
modded class ActionLowerFlag {

	override bool Can(PlayerBase player, ActionTarget target, ItemBase item, int condition_mask) {
		if (!super.Can(player, target, item, condition_mask))
			return false;
		Object obj = target.GetObject();
		if (!obj)
			obj = target.GetParent();
		if (!obj || vector.Distance(player.GetPosition(), obj.GetPosition()) > LBTerritoryConfig.Get.maxDistanceToLowerFlag) {
			return false;
		}
		return LBTerritoryConfig.Get.CanExecuteAction(player, target, 6, item);
	}

}
#endif
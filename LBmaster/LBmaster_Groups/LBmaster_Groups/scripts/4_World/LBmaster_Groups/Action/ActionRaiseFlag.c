#ifdef LBmaster_GroupDLCPlotpole
modded class ActionRaiseFlag {

	override bool Can(PlayerBase player, ActionTarget target, ItemBase item, int condition_mask) {
		if (!super.Can(player, target, item, condition_mask))
			return false;
		return LBTerritoryConfig.Get.CanExecuteAction(player, target, 7, item);
	}

}
#endif
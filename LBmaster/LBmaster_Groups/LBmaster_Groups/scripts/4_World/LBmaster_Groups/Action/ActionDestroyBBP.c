#ifdef LBmaster_GroupDLCPlotpole
#ifdef BaseBuildingPlus
modded class ActionDestroyBBP {

	override bool Can(PlayerBase player, ActionTarget target, ItemBase item, int condition_mask) {
		if (!super.Can(player, target, item, condition_mask))
			return false;
		int type = 3;
		if (target.GetObject() && target.GetObject().IsInherited(TerritoryFlag)) {
			type = 5;
		}
		return LBTerritoryConfig.Get.CanExecuteAction(player, target, type, item);
	}

}
#endif
#endif
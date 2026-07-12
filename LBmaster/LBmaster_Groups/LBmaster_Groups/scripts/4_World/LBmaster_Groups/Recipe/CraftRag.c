#ifdef LBmaster_GroupDLCPlotpole
modded class CraftRag {

	override bool CanDo(ItemBase ingredients[], PlayerBase player) {
		if (!LBTerritoryConfig.Get.allowDestroyWithFlagAttached) {
			InventoryLocation loc = new InventoryLocation();
			ingredients[1].GetInventory().GetCurrentInventoryLocation(loc);
			if (loc.GetType() == InventoryLocationType.ATTACHMENT && TerritoryFlag.Cast(loc.GetParent()))
				return false;
		}
		return super.CanDo(ingredients, player);
	}

}
#endif

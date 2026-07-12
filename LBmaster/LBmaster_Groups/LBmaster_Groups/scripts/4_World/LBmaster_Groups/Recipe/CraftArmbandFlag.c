#ifdef LBmaster_GroupDLCPlotpole
modded class CraftArmbandFlag {

	override bool CanDo(ItemBase ingredients[], PlayerBase player) {
		return LBTerritoryConfig.Get.allowDestroyWithFlagAttached && super.CanDo(ingredients, player);
	}

}
#endif

#ifdef ND_RP
modded class LBGroupUI {

	override void AddCustomMarkersOnMapOpen() {
		super.AddCustomMarkersOnMapOpen();
		if (!mapWidget)
			return;

		MapMenu.ShowALLMissionsALP(mapWidget);
		MapMenu.ShowALLRestictedAreasALP(mapWidget);
	}
}
#endif
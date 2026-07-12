#ifdef CarePackageV2
#ifndef CarePackageV2AdvancedGroups
modded class LBGroupUI {

	override void AddCustomMarkersOnMapOpen() {

		GetRPCManager().SendRPC("CarePackage_Server", "CarePackageLocation_Server", NULL);

		super.AddCustomMarkersOnMapOpen();

		if (mapWidget) {
			for (int i = 0; i < g_Game.GetClientCarePackageMarkerArrayLength(); i++) {
				if (g_Game.GetClientCarePackageMarker(i) != "0 0 0") {
					mapWidget.AddUserMark(g_Game.GetClientCarePackageMarker(i), "CarePackage", ARGB(255, 0, 255, 0), "CarePackage\\Icon.paa");
				}
			}
		}
	}
}
#endif
#endif

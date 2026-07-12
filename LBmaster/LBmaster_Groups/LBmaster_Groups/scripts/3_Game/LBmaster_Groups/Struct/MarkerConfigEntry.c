class MarkerConfigEntry {

	LBMarkerType type; // Type of the Marker Config. 0 = Static Marker (persistent), 1 = Dynamic Marker (deleted after server restart), 2 = Ping Marker, 3 = Group Marker, 4 = Group Player Marker, 5 = Private Marker
	string typeString; // String represenation of the Type. The important thing is the type above
	int maxDistance; // Maximum Distance the Marker Type will be visible in 3D and on the Compass
	bool display3d; // enable / disable if the Marker Type is Visible in 3D
	bool displayDistance; // enable / disable if the Marker Type should have a distance displayed in 3D
	bool displayMap; // enable / disable if the Marker Type is Visible on the Map
	bool displayCompass; // enable / disable if the Marker Type is Visible on the Compass
	bool displayGPS; // enable / disable if the Marker Type is Visible on the GPS

	static MarkerConfigEntry Init(LBMarkerType type2, int maxDist, bool disp3d, bool dispDist, bool dispMap, bool dispComp, bool dispGPS) {
		MarkerConfigEntry ent = new MarkerConfigEntry;
		ent.type = type2;
		ent.typeString = typename.EnumToString(LBMarkerType, type2);
		ent.maxDistance = maxDist;
		ent.display3d = disp3d;
		ent.displayDistance = dispDist;
		ent.displayMap = dispMap;
		ent.displayCompass = dispComp;
		ent.displayGPS = dispGPS;
		return ent;
	}

}
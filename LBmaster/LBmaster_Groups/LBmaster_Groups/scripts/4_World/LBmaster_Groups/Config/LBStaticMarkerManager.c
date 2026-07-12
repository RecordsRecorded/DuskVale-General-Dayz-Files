class LBStaticMarkerManager : LBConfigLoader<LBStaticMarkerManager_> {

	override void InitVars() {
		InitVarsInternal("LBGroup", "StaticMarkers.json", LBConfigType.CONFIG, false); // (easy)
	}

	override string GetRequiredVersion() {
		return "Advanced_Groups";
	}

}
/*This config contains all static markers on the server. It is possible to add the static markers here, especially if you need a little more control over the markers, than you have with the ingame admin menu

I still highly recomment using the ingame Admin menu to add static markers on your server. It's a lot easier for the initial setup.

Make sure you are added to the Admins.json in your Config/Common/Admins.json file and have admin mode enabled. Then just place a marker on the map and change the type to Server Marker (perm) to add a static marker. You can define Name, Icon, Color and Radius ingame, which should be enough for everyone.

Make sure if you do want to edit this file, to do that while the server is offline, otherwise changes are likely to be overwritten by the server.
*/
class LBStaticMarkerManager_ : LBConfigBaseSingle {

	protected ref array<ref LBServerMarker> staticMarkers = new array<ref LBServerMarker>();
	protected ref array<ref LBServerMarker> tempStaticMarkers = new array<ref LBServerMarker>();

	override bool JsonLoadVar(string path, bool forceValid, out bool overwriteTest) {
		return LBJsonLoader<array<ref LBServerMarker>>.JsonLoadFile(path, staticMarkers, forceValid);
	}

	override void JsonSaveVar(string path, out bool overwriteTest) {
		LBJsonLoader<array<ref LBServerMarker>>.JsonSaveFile(path, staticMarkers);
	}

	array<ref LBServerMarker> GetStaticMarkers() {
		return staticMarkers;
	}

	array<ref LBServerMarker> GetTempMarkers() {
		return tempStaticMarkers;
	}

	LBServerMarker FindMarker(int uid, array<ref LBServerMarker> lst) {
		foreach (LBServerMarker marker : lst) {
			if (!marker)
				continue;
			if (marker.uid == uid)
				return marker;
		}
		return null;
	}

	LBServerMarker FindAnyMarker(int uid) {
		LBServerMarker found = FindPermMarker(uid);
		if (!found)
			found = FindTempMarker(uid);
		return found;
	}

	LBServerMarker FindTempMarker(int uid) {
		return FindMarker(uid, tempStaticMarkers);
	}

	LBServerMarker FindPermMarker(int uid) {
		return FindMarker(uid, staticMarkers);
	}

	bool RemoveServerMarker(LBServerMarker marker) {
		return false; // Implemented in Server Side Part
	}

	bool RemoveServerMarker(int uid) {
		return false; // Implemented in Server Side Part
	}

	LBServerMarker AddTempServerMarker(string name, vector position, string icon, int color, bool toSurface = true, bool display3d = true, bool displayMap = true, bool displayGPS = true) {
		return null; // Implemented in Server Side Part
	}

	LBServerMarker AddPermServerMarker(string name, vector position, string icon, int color, bool toSurface = true, bool display3d = true, bool displayMap = true, bool displayGPS = true) {
		return null; // Implemented in Server Side Part
	}

	LBServerMarker AddTempCircleMarker(vector position, float radius, int color, bool striked, bool toSurface = true, bool display3d = true, bool displayMap = true, bool displayGPS = true) {
		LBServerMarker marker = AddTempServerMarker("", position, "DZ\\gear\\navigation\\data\\map_tree_ca.paa", ARGB(0, 255, 255, 255), toSurface, display3d, displayMap, displayGPS);
		marker.SetRadius(radius, color, striked, true);
		return marker;
	}

	LBServerMarker AddPermCircleMarker(vector position, float radius, int color, bool striked, bool toSurface = true, bool display3d = true, bool displayMap = true, bool displayGPS = true) {
		LBServerMarker marker = AddPermServerMarker("", position, "DZ\\gear\\navigation\\data\\map_tree_ca.paa", ARGB(0, 255, 255, 255), toSurface, display3d, displayMap, displayGPS);
		marker.SetRadius(radius, color, striked, true);
		return marker;
	}

	protected LBServerMarker AddServerMarker(string name, vector position, string icon, int color, bool toSurface, bool display3d, bool displayMap, bool displayGPS) {
		return null; // Implemented in Server Side Part
	}
}

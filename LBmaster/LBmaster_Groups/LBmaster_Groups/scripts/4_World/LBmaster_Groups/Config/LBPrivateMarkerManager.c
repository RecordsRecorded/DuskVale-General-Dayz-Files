class LBPrivateMarkerManager : LBConfigLoader<LBPrivateMarkerManager_> {

	override void InitVars() {
		InitVarsInternal("LBGroup", "PrivateMarkers.json");
	}

	override string GetRequiredVersion() {
		return "Advanced_Groups";
	}

	override bool IsClientSideConfig() {
		return true;
	}

}

class LBPrivateMarkerManager_ : LBConfigBase {

	static const int CURRENT_VERSION = 0;

	ref array<ref Param2<string, ref array<ref LBMarker>>> markers = new array<ref Param2<string, ref array<ref LBMarker >>> ();
	ref array<ref LBMarker> privateMarkers = new array<ref LBMarker>();
	string currentServer;

	override bool JsonLoadVar(string path, bool forceValid, out bool overwriteTest) {
		return LBJsonLoader<array<ref Param2<string, ref array<ref LBMarker>>>>.JsonLoadFile(path, markers, forceValid);
	}

	override void JsonSaveVar(string path, out bool overwriteTest) {
		LBJsonLoader<array<ref Param2<string, ref array<ref LBMarker>>>>.JsonSaveFile(path, markers);
	}

	void InitMarkers(string servername) {
		currentServer = servername;
		privateMarkers = GetPrivateMarkersFromList();
		foreach (LBMarker mark : privateMarkers) {
			mark.InitMarker();
			mark.AddToAllList();
		}
	}

	void DeleteOldDeathMarker() {
		LBMarker marker = null;
		foreach (LBMarker mar : privateMarkers) {
			if (mar && mar.creatorSteamID == "Death") {
				marker = mar;
				break;
			}
		}
		if (marker)
			RemoveMarker(marker);
	}

	private array<ref LBMarker> GetPrivateMarkersFromList() {
		foreach (Param2<string, ref array<ref LBMarker>> param : markers) {
			if (param.param1 == currentServer) {
				return param.param2;
			}
		}
		privateMarkers = new array<ref LBMarker>();
		markers.Insert(new Param2<string, ref array<ref LBMarker>>(currentServer, privateMarkers));
		return privateMarkers;
	}

	void AddMarker(LBMarker marker) {
		privateMarkers.Insert(marker);
		marker.InitMarker();
		LBPrivateMarkerManager.Loader.Save();
	}

	void RemoveMarker(LBMarker marker) {
		privateMarkers.RemoveItem(marker);
		delete marker;
		LBPrivateMarkerManager.Loader.Save();
	}

	LBMarker FindMarkerByUID(int uid) {
		foreach (LBMarker marker : privateMarkers) {
			if (marker.uid == uid)
				return marker;
		}
		return null;
	}

	bool FindMarkerByIcon(string icon, out LBMarker mark) {
		foreach (LBMarker marker : privateMarkers) {
			if (marker.icon == icon) {
				mark = marker;
				return true;
			}
		}
		return false;
	}

	bool FindNearestMarker(vector position, out LBMarker markero, out float distance) {
		if (privateMarkers.Count() == 0)
			return false;
		float bestDist = 0;
		LBMarker bestMarker = null;
		foreach (LBMarker marker : privateMarkers) {
			if (!bestMarker || vector.Distance(position, marker.position) < bestDist) {
				bestMarker = marker;
				bestDist = vector.Distance(position, marker.position) < bestDist;
			}
		}
		if (bestMarker) {
			markero = bestMarker;
			distance = bestDist;
			return true;
		}
		return false;
	}

}
class LBMarkerVisibilityManager : LBConfigLoader<LBMarkerVisibilityManager_> {

	override void InitVars() {
		InitVarsInternal("LBGroup", "PrivateMarkerDisplaystates.json");
	}

	override string GetRequiredVersion() {
		return "Advanced_Groups";
	}

	override bool IsClientSideConfig() {
		return true;
	}

}

class LBMarkerVisibilityManager_ : LBConfigBase {

	static int CURRENT_VERSION = 8;

	ref array<ref LBMarkerVisibilityEntry> entries = new array<ref LBMarkerVisibilityEntry>();
	ref array<ref LBGlobalVisibilityEntry> globalentries = new array<ref LBGlobalVisibilityEntry>();
	ref array<ref Param2<string, bool>> hiddenChannels = new array<ref Param2<string, bool >> ();

	int globalVisiblityState;
	string pingMarkerIcon = "LBmaster_Groups\\gui\\icons\\ping.paa";
	string playerMarkerIcon = "LBmaster_Groups\\gui\\icons\\player.paa";
	bool compassEnabled = true;
	bool playerlistEnabled = true;
	bool showNoBuildZones = true;
	bool disableShowClantextures = false;
	bool gpsEnabled = true;
	bool centerMapOnPlayer = false;
	int pingSize = 16;
	int playerSize = 16;
	int chatSize = 15;
	int chatBorderVisibility = 100;

	[NonSerialized()]
	ref map<int, LBMarkerVisibilityEntry> entriesCache = new map<int, LBMarkerVisibilityEntry>();

	override void LoadDefault() {
		version = CURRENT_VERSION;
		globalVisiblityState = 0;
		pingMarkerIcon = "LBmaster_Groups\\gui\\icons\\ping.paa";
		compassEnabled = true;
		playerlistEnabled = true;
		showNoBuildZones = true;
		gpsEnabled = true;
	}

	override bool OnLoad() {
		entriesCache = new map<int, LBMarkerVisibilityEntry>();
		foreach (LBMarkerVisibilityEntry entry : entries) {
			entriesCache.Insert(entry.uid, entry);
		}
		return false;
	}

	override void UpdateVersion() {
		if (version < 1) {
			pingMarkerIcon = "LBmaster_Groups\\gui\\icons\\ping.paa";
			compassEnabled = true;
			playerlistEnabled = true;
		}
		if (version < 2) {
			chatSize = 15;
		}
		if (version < 3) {
			pingSize = 16;
		}
		if (version < 4) {
			showNoBuildZones = true;
		}
		if (version < 5) {
			playerSize = 16;
			playerMarkerIcon = "LBmaster_Groups\\gui\\icons\\player.paa";
		}
		if (version < 6) {
			hiddenChannels = new array<ref Param2<string, bool>>();
		}
		if (version < 7) {
			chatBorderVisibility = 100;
		}
	}

	float GetChatBorderVisibility() {
		return Math.Clamp(chatBorderVisibility / 100.0, 0, 1);
	}

	bool IsChannelHidden(string name) {
		bool hidden = false;
		if (!hiddenChannels)
			hiddenChannels = new array<ref Param2<string, bool>>();
		foreach (Param2<string, bool> hiddenParam : hiddenChannels) {
			if (hiddenParam.param1 == name) {
				hidden = hiddenParam.param2;
				break;
			}
		}
		LBLogger.Debug("Is Channel Hidden ? " + name + "=" + hidden, "AdvancedGroups");
		return hidden;
	}

	void SetChannelHidden(string name, bool hidden) {
		if (!hiddenChannels)
			hiddenChannels = new array<ref Param2<string, bool>>();
		LBLogger.Debug("Setting Channel Hidden ? " + name + "=" + hidden, "AdvancedGroups");
		foreach (Param2<string, bool> hiddenParam : hiddenChannels) {
			if (hiddenParam.param1 == name) {
				hiddenParam.param2 = hidden;
				return;
			}
		}
		hiddenChannels.Insert(new Param2<string, bool>(name, hidden));

	}

	int GetChatSize() {
		return Math.Max(7, chatSize);
	}

	void ResetPingToDefault() {
		SetPingMarkerIcon("LBmaster_Groups\\gui\\icons\\ping.paa");
		SetPlayerMarkerIcon("LBmaster_Groups\\gui\\icons\\player.paa");
		pingSize = 16;
		playerSize = 16;
		chatSize = 15;
	}

	void SetPingMarkerIcon(string icon) {
		this.pingMarkerIcon = icon;
	}

	void SetPlayerMarkerIcon(string icon) {
		this.playerMarkerIcon = icon;
	}

	TStringArray GetPingMarkerIcons() {
		TStringArray arr = new TStringArray();
		foreach (string icon : LBMarkerSettingsConfig.Get.availableIcons) {
			arr.Insert(icon);
		}
		if (arr.Find("LBmaster_Groups\\gui\\icons\\ping.paa") == -1)
			arr.Insert("LBmaster_Groups\\gui\\icons\\ping.paa");
		return arr;
	}

	TStringArray GetPlayerMarkerIcons() {
		TStringArray arr = new TStringArray();
		foreach (string icon : LBMarkerSettingsConfig.Get.availableIcons) {
			arr.Insert(icon);
		}
		if (arr.Find("LBmaster_Groups\\gui\\icons\\player.paa") == -1)
			arr.Insert("LBmaster_Groups\\gui\\icons\\player.paa");
		return arr;
	}

	LBMarkerVisibilityEntry GetVisibilityOrAdd(int uid) {
		LBMarkerVisibilityEntry entry;
		if (!entriesCache.Find(uid, entry)) {
			entry = new LBMarkerVisibilityEntry();
			entry.uid = uid;
			entry.displaystate = 0;
			entriesCache.Insert(entry.uid, entry);
			entries.Insert(entry);
		}
		return entry;
	}

	LBGlobalVisibilityEntry GetGlobalVisibilityOrAdd(LBMarkerType type) {
		foreach (LBGlobalVisibilityEntry entry : globalentries) {
			if (entry.type == type)
				return entry;
		}
		LBGlobalVisibilityEntry ent = new LBGlobalVisibilityEntry();
		ent.type = type;
		ent.displaystate = 0;
		globalentries.Insert(ent);
		return ent;
	}

	bool Is3DVisiblie(LBMarker marker, bool testType = true) {
		if (marker.GetVisibilityEntry().displaystate != 0)
			return false;
		return !testType || IsGlobal3DVisible(marker.type);
	}

	bool IsMapVisible(LBMarker marker, bool testType = true) {
		if (marker.GetVisibilityEntry().displaystate == 2)
			return false;
		return !testType || IsGlobal2DVisible(marker.type);
	}

	bool IsGlobal3DVisible(LBMarkerType type) {
		return GetGlobalVisibilityOrAdd(type).displaystate == 0;
	}

	bool IsGlobal2DVisible(LBMarkerType type) {
		return GetGlobalVisibilityOrAdd(type).displaystate != 2;
	}

	int GetNextState() {
		globalVisiblityState++;
		globalVisiblityState = globalVisiblityState % 6;
		if (globalVisiblityState == 0) {
			SetAllGlobalStates(0);
		} else if (globalVisiblityState == 1) {
			SetAllGlobalStates(0);
			GetGlobalVisibilityOrAdd(LBMarkerType.SERVER_STATIC).displaystate = 1;
			GetGlobalVisibilityOrAdd(LBMarkerType.SERVER_DYNAMIC).displaystate = 1;
		} else if (globalVisiblityState == 2) {
			SetAllGlobalStates(0);
			GetGlobalVisibilityOrAdd(LBMarkerType.SERVER_STATIC).displaystate = 1;
			GetGlobalVisibilityOrAdd(LBMarkerType.SERVER_DYNAMIC).displaystate = 1;
			GetGlobalVisibilityOrAdd(LBMarkerType.PRIVATE_MARKER).displaystate = 1;
		} else if (globalVisiblityState == 3) {
			SetAllGlobalStates(1);
			GetGlobalVisibilityOrAdd(LBMarkerType.GROUP_PLAYER_MARKER).displaystate = 0;
			GetGlobalVisibilityOrAdd(LBMarkerType.GROUP_PING).displaystate = 0;
		} else if (globalVisiblityState == 4) {
			SetAllGlobalStates(1);
			GetGlobalVisibilityOrAdd(LBMarkerType.SERVER_STATIC).displaystate = 0;
			GetGlobalVisibilityOrAdd(LBMarkerType.SERVER_DYNAMIC).displaystate = 0;
		} else {
			SetAllGlobalStates(1);
		}
		LBMarkerVisibilityManager.Loader.Save();
		return globalVisiblityState;
	}

	void SetAllGlobalStates(int state) {
		GetGlobalVisibilityOrAdd(LBMarkerType.SERVER_STATIC).displaystate = state;
		GetGlobalVisibilityOrAdd(LBMarkerType.SERVER_DYNAMIC).displaystate = state;
		GetGlobalVisibilityOrAdd(LBMarkerType.GROUP_PING).displaystate = state;
		GetGlobalVisibilityOrAdd(LBMarkerType.GROUP_MARKER).displaystate = state;
		GetGlobalVisibilityOrAdd(LBMarkerType.GROUP_PLAYER_MARKER).displaystate = state;
		GetGlobalVisibilityOrAdd(LBMarkerType.PRIVATE_MARKER).displaystate = state;
	}

	string GetCurrentStateName() {
		if (globalVisiblityState == 0) {
			return "#lb_ag_st_all";
		} else if (globalVisiblityState == 1) {
			return "#lb_ag_st_no_server";
		} else if (globalVisiblityState == 2) {
			return "#lb_ag_st_only_group";
		} else if (globalVisiblityState == 3) {
			return "#lb_ag_st_only_player";
		} else if (globalVisiblityState == 4) {
			return "#lb_ag_st_only_server";
		}
		return "#lb_ag_st_all_hidden";
	}

}

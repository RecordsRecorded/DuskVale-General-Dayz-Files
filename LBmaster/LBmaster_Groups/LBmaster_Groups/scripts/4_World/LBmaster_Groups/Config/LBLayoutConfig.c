class LBLayoutConfig : LBConfigLoader<LBLayoutConfig_> {

	override void InitVars() {
		InitVarsInternal("LBGroup", "LayoutManager_TEMP.json");
	}

	override string GetRequiredVersion() {
		return "Advanced_Groups";
	}

	override bool IsClientSideConfig() {
		return true;
	}

}

class LBLayoutConfig_ : LBConfigBase {

	static const int CURRENT_VERSION = 1;
	int playerlistLayoutIndex = 0;
	int gpsSizeIndex = 0;
	int playerMarkerPosIndex = 0;
	int playerMarkerStyleIndex = 0;
	float gpsZoom = 0.2;
	bool streamerModeEnabled = false;

	static ref ScriptInvoker Event_OnLayoutChanged = new ScriptInvoker();
	static ref ScriptInvoker Event_StreamerModeChanged = new ScriptInvoker();
	static ref ScriptInvoker Event_GPSChanged = new ScriptInvoker();

	void ResetAll() {
		playerlistLayoutIndex = 0;
		streamerModeEnabled = false;
		gpsSizeIndex = 0;
		playerMarkerPosIndex = 0;
		playerMarkerStyleIndex = 0;
		gpsZoom = 0.2;
		InvokeOnLayoutChanged();
	}

	override bool OnLoad() {
		InvokeOnLayoutChanged();
		return false;
	}

	static void InvokeOnLayoutChanged() {
		Event_OnLayoutChanged.Invoke();
	}

	static void InvokeGPSChanged() {
		Event_GPSChanged.Invoke();
	}

	override void UpdateVersion() {
		if (version < 1) {
			gpsZoom = 0.2;
			gpsSizeIndex = 0;
			playerMarkerPosIndex = 0;
			playerMarkerStyleIndex = 0;
		}
	}

	void SetStreamerMode(bool enabled) {
		this.streamerModeEnabled = enabled;
		LBLogger.Debug("Streamer Mode changed to: " + enabled, "AdvancedGroups");
		Event_StreamerModeChanged.Invoke(enabled);
	}

	void SetPlayerlistLayout(int index) {
		playerlistLayoutIndex = index;
		InvokeOnLayoutChanged();
	}

	string GetPlayerListLayoutName() {
		if (playerlistLayoutIndex == 0) {
			return "PlayerList_Normal";
		} else if (playerlistLayoutIndex == 1) {
			return "PlayerList_Small";
		} else if (playerlistLayoutIndex == 2) {
			return "PlayerList_Tiny";
		} else if (playerlistLayoutIndex == 3) {
			return "PlayerList_Cross";
		}
		return "";

	}

}
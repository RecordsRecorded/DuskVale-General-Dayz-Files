class LBColorManager : LBConfigLoader<LBColorManager_> {

	override void InitVars() {
		InitVarsInternal("LBGroup", "ColorManager.json");
	}

	override string GetRequiredVersion() {
		return "Advanced_Groups";
	}

	override bool IsClientSideConfig() {
		return true;
	}

}

class LBColorManager_ : LBConfigBase {

	static const int CURRENT_VERSION = 0;

	ref array<ref Param2<string, int>> colors = new array<ref Param2<string, int >> ();
	string changedColor = "";

	[NonSerialized()]
	bool changed = false;

	static ref ScriptInvoker Event_OnColorChange = new ScriptInvoker();

	override bool JsonLoadVar(string path, bool forceValid, out bool overwriteTest) {
		return LBJsonLoader<array<ref Param2<string, int>>>.JsonLoadFile(path, colors, forceValid);
	}

	override void JsonSaveVar(string path, out bool overwriteTest) {
		LBJsonLoader<array<ref Param2<string, int>>>.JsonSaveFile(path, colors);
	}

	override bool OnLoad() {
		changed = false;
		SetDefaultColors();
		InvokeOnChanged();
		return changed;
	}

	void ResetAll() {
		colors.Clear();
		SetDefaultColors();
		InvokeOnChanged();
	}

	static void InvokeOnChanged() {
		Event_OnColorChange.Invoke();
	}

	int GetColor(string colorStr, int defaultColor = -1) {
		if (changedColor == colorStr) {
			SetColor(colorStr, defaultColor);
		}
		if (changedColor != "")
			return - 1;
		foreach (Param2<string, int> color : colors) {
			if (color && color.param1 == colorStr)
				return color.param2;
		}
		changed = true;
		colors.Insert(new Param2<string, int>(colorStr, defaultColor));
		return defaultColor;
	}

	void SetColor(string colorStr, int colorARGB, bool add = true) {
		foreach (Param2<string, int> color : colors) {
			if (color && color.param1 == colorStr) {
				color.param2 = colorARGB;
				return;
			}
		}
		if (add)
			colors.Insert(new Param2<string, int>(colorStr, colorARGB));
	}

	void SetDefaultColors() {
		GetColor("Player 3D Marker", ARGB(255, 255, 255, 255));
		GetColor("Own Player Map Marker", ARGB(255, 255, 51, 51));
		GetColor("Player Online", ARGB(255, 0, 255, 0));
		GetColor("Player Offline", ARGB(255, 255, 0, 0));
		GetColor("Ping 3D Marker", ARGB(255, 255, 255, 0));
		GetColor("Compass", ARGB(255, 255, 255, 255));
		GetColor("Compass Line", ARGB(255, 255, 0, 0));
		GetColor("Playerlist entry full health", ARGB(255, 255, 255, 255));
		GetColor("Playerlist entry zero health", ARGB(255, 255, 0, 0));
		GetColor("Playerlist entry border", ARGB(255, 255, 255, 255));
	}

	void ResetColorToDefault(string colorStr) {
		changedColor = colorStr;
		SetDefaultColors();
		changedColor = "";
	}

	TStringArray GetColorStrings() {
		TStringArray arr = new TStringArray();
		foreach (Param2<string, int> color : colors) {
			arr.Insert(color.param1);
		}
		return arr;
	}

}
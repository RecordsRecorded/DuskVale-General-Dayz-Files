class LBAdminMenuWindowConfig : LBConfigLoader<LBAdminMenuWindowConfig_> {

	override void InitVars() {
		InitVarsInternal("AdminTool", "windows.json", LBConfigType.CONFIG, true); // (easy) Do not touch
	}

	override string GetRequiredVersion() {
		return "Admin_Tools";
	}

	override bool IsClientSideConfig() {
		return true;
	}

}
// This config hold information about window stats including positions, size and other information specific to a window. This also contains window presets
class LBAdminMenuWindowConfig_ : LBConfigBase {

	const static int CURRENT_VERSION = 3;

	int selectedEntry = 0;
	bool expanded = true;
	ref array<ref array<ref LBAdminMenuWindowConfigEntry>> entries = new array<ref array<ref LBAdminMenuWindowConfigEntry >> (); // List of all Window states
	ref TStringArray frameOrders = new TStringArray();
	ref TStringArray frameNames = new TStringArray();

	[NonSerialized()]
	ref ScriptInvoker Event_Frame_Name_Changed;

	override bool OnLoad() {
		Event_Frame_Name_Changed = new ScriptInvoker();
		bool save = false;
		for (int i = entries.Count(); i < 9; ++i) {
			entries.Insert(new array<ref LBAdminMenuWindowConfigEntry>());
			save = true;
		}
		if (frameNames == null || frameNames.Count() == 0)
			LoadDefaultFrameNames();
		for (i = frameNames.Count(); i < 9; ++i) {
			frameNames.Insert("" + (i + 1));
			save = true;
		}
		return save;
	}

	void LoadDefaultFrameNames() {
		frameNames = {"Player", "Items", "ESP", "Ban", "Mon", "Items", "Const", "Look", "Builder"};
	}

	string GetPresetName(int index) {
		return frameNames.Get(index);
	}

	void SetPresetName(int index, string name) {
		if (name == "")
			name = "" + (index + 1);
		frameNames.Set(index, name);
		LBAdminMenuWindowConfig.Loader.Save();
		Event_Frame_Name_Changed.Invoke(index, name);
	}

	override void LoadDefault() {
		for (int i = entries.Count(); i < 9; ++i) {
			entries.Insert(new array<ref LBAdminMenuWindowConfigEntry>());
		}
		frameOrders = {"LBAdminMenuFrame_PlayerList", "LBAdminMenuFrame_PlayerInfo", "LBAdminMenuFrame_TeleportPos", "LBAdminMenuFrame_PlayerMap", "LBAdminMenuFrame_Teams", "LBAdminMenuFrame_ItemSpawner", "HERBERT_NEW_FRAME", "LBAdminMenuFrame_PresetSpawner", "LBAdminMenuFrame_ObjectBuilder", "LBAdminMenuFrame_WeatherManager", "LBAdminMenuFrame_ESPSettings", "LBAdminMenuFrame_ESPDragging", "LBAdminMenuFrame_SelectedItems", "LBAdminMenuFrame_PlayerCheats", "LBAdminMenuFrame_Banmanager", "LBAdminMenuFrame_Watchlist", "PETER_NEW_FRAME", "LBAdminMenuFrame_ConstructionEditor", "LBAdminMenuFrame_ActiveModifiers", "LBAdminMenuFrame_ItemList", "LBAdminMenuFrame_ServerMonitor", "LBAdminMenuFrame_Settings"};
		SelectEntry(0);
		SaveDefaultWindowProperties("LBAdminMenuFrame_PlayerInfo", 284.5, 0.0, 580.7500610351563, 635.75, 0, 1);
		SaveDefaultWindowProperties("LBAdminMenuFrame_PlayerMap", 875.25, 35.7498779296875, 740.500244140625, 1044.2501220703126, 0, 1);
		SaveDefaultWindowProperties("LBAdminMenuFrame_TeleportPos", 283.5, 645.4999389648438, 579.5, 434.5000305175781, 0, 1);
		SaveDefaultWindowProperties("LBAdminMenuFrame_PlayerList", 0.0, 0.0, 275.5, 1078.5, 0, 1);
		SelectEntry(1);
		SaveDefaultWindowProperties("LBAdminMenuFrame_ItemSpawner", 0.0, 16.999969482421876, 819.500244140625, 1063.0, 0, 1);
		SaveDefaultWindowProperties("LBAdminMenuFrame_PresetSpawner", 833.25, 38.499847412109378, 775.0, 1041.5001220703126, 0, 1);
		SelectEntry(2);
		SaveDefaultWindowProperties("LBAdminMenuFrame_PlayerCheats", 669.75, 790.7499389648438, 488.75006103515627, 289.2500305175781, 0, 1);
		SaveDefaultWindowProperties("LBAdminMenuFrame_ActiveModifiers", 0.0, 0.0, 393.75, 116.25, 1, 1);
		SaveDefaultWindowProperties("LBAdminMenuFrame_ESPSettings", 0.0, 803.25, 600.0, 276.75, 0, 1);
		SaveDefaultWindowProperties("LBAdminMenuFrame_ESPDragging", 0.0, 691.5, 400.0000305175781, 82.5, 0, 1);
		SaveDefaultWindowProperties("LBAdminMenuFrame_SelectedItems", 1237.5, 58.5, 375.0, 1021.5, 0, 1);
		SelectEntry(3);
		SaveDefaultWindowProperties("LBAdminMenuFrame_Watchlist", 0.0, 4.5, 802.5, 1071.0, 0, 1);
		SaveDefaultWindowProperties("LBAdminMenuFrame_Banmanager", 816.75, 56.25, 790.2500610351563, 1021.7501220703125, 0, 1);
		SelectEntry(4);
		SaveDefaultWindowProperties("LBAdminMenuFrame_WeatherManager", 0.0, 0.0, 698.25, 519.2501220703125, 0, 1);
		SaveDefaultWindowProperties("LBAdminMenuFrame_Settings", 0.0, 536.25, 700.0000610351563, 543.75, 0, 1);
		SaveDefaultWindowProperties("LBAdminMenuFrame_ServerMonitor", 744.0, 0.0, 375.0, 185.25, 0, 1);
		SelectEntry(5);
		SaveDefaultWindowProperties("LBAdminMenuFrame_ItemList", 0.0, 16.9998779296875, 1514.25, 1063.0001220703126, 0, 1);
		SaveDefaultWindowProperties("LBAdminMenuFrame_ServerMonitor", 1530.75, 0.0, 389.25, 161.25, 1, 1);
		SelectEntry(6);
		SaveDefaultWindowProperties("LBAdminMenuFrame_ConstructionEditor", 372.75, 345.75, 863.25, 450.75, 0, 1);
		SelectEntry(7);
		SaveDefaultWindowProperties("LBAdminMenuFrame_LookingAt", 408.75, 1013.25, 1052.25, 66.75, 1, 1);
		SaveObjectBuilderFrames();
		SelectEntry(0);

	}

	void SaveObjectBuilderFrames() {
		for (int i = 0; i < 9; ++i) {
			SelectEntry(i);
			bool visible = false;
			if (i == 8)
				visible = true;
			SaveDefaultWindowProperties("LBAdminMenuFrame_ObjectBuilder", 0.0, 0.0, 306.0, 1078.5, 0, visible);
			SaveDefaultWindowProperties("LBAdminMenuFrame_BuildingSets", 1173.75, 0.0, 384.75, 204.75, 0, visible);
			SaveDefaultWindowProperties("LBAdminMenuFrame_ObjectSelection", 1557.75, 0.0, 301.5, 851.25, 0, visible);
			SaveDefaultWindowProperties("LBAdminMenuFrame_ObjectBuilderControls", 306.0, 0.0, 868.5, 72.75, 0, visible);
		}
	}

	override void UpdateVersion() {
		if (version < 2)
			expanded = true;
		if (version < 3)
			SaveObjectBuilderFrames();
	}

	int GetFrameOrder(string classname) {
		int i = 0;
		for (; i < frameOrders.Count(); ++i) {
			if (frameOrders.Get(i) == classname)
				return i;
		}
		frameOrders.Insert(classname);
		return i;
	}

	void MoveFrameOrder(string classname, string after) {
		frameOrders.RemoveItem(classname);
		int afterIndex = frameOrders.Find(after);
		frameOrders.InsertAt(classname, afterIndex + 1);
	}

	bool EntryHasValues(int index) {
		return entries.Get(index).Count() > 0;
	}

	void SelectEntry(int index) {
		selectedEntry = index;
		if (index < 0)
			selectedEntry = 0;
		else if (index >= 10)
			selectedEntry = 9;
	}

	void ResetEntry(int index) {
		array<ref LBAdminMenuWindowConfigEntry> arr = entries.Get(index);
		arr.Clear();
	}

	LBAdminMenuWindowConfigEntry GetEntry(string name) {
		array<ref LBAdminMenuWindowConfigEntry> arr = entries.Get(selectedEntry);
		foreach (LBAdminMenuWindowConfigEntry entry : arr) {
			if (entry.windowClassname == name)
				return entry;
		}
		LBAdminMenuWindowConfigEntry found = new LBAdminMenuWindowConfigEntry();
		found.windowClassname = name;
		arr.Insert(found);
		return found;
	}

	void SaveDefaultWindowProperties(string name, float windowX, float windowY, float windowW, float windowH, bool pinned, bool visible) {
		LBAdminMenuWindowConfigEntry entry = GetEntry(name);
		entry.windowX = windowX;
		entry.windowY = windowY;
		entry.windowW = windowW;
		entry.windowH = windowH;
		entry.pinned = pinned;
		entry.visible = visible;
		entry.newEntry = false;
	}

	void SaveWindowProperties(string name, Widget widget, bool pinned) {
		LBAdminMenuWindowConfigEntry entry = GetEntry(name);
		float x, y, w, h;
		widget.GetScreenSize(w, h);
		widget.GetScreenPos(x, y);
		entry.windowX = x / LBWidgetUtils.heightScale;
		entry.windowY = y / LBWidgetUtils.heightScale;
		entry.windowW = w / LBWidgetUtils.heightScale;
		entry.windowH = h / LBWidgetUtils.heightScale;
		entry.pinned = pinned;
		LBAdminMenuWindowConfig.Loader.Save();
	}

	void SaveWindowProperties(string name, bool visible, bool pinned) {
		LBAdminMenuWindowConfigEntry entry = GetEntry(name);
		entry.visible = visible;
		entry.pinned = pinned;
		LBAdminMenuWindowConfig.Loader.Save();
	}

	bool RestoreWindowProperties(string name, Widget widget, out bool pinned, out bool visible) {
		LBAdminMenuWindowConfigEntry entry = GetEntry(name);
		if (entry.newEntry) {
			entry.newEntry = false;
			return false;
		}
		widget.SetScreenPos(entry.windowX * LBWidgetUtils.heightScale, entry.windowY * LBWidgetUtils.heightScale);
		widget.SetScreenSize(entry.windowW * LBWidgetUtils.heightScale, entry.windowH * LBWidgetUtils.heightScale);
		visible = entry.visible;
		if (!visible)
			entry.pinned = false;
		pinned = entry.pinned;
		return true;
	}
}
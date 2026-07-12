class LBAdminItemPresetConfigServer : LBConfigLoader<LBAdminItemPresetConfigServer_> {

	override void InitVars() {
		InitVarsInternal("AdminTool", "itempresets.json", LBConfigType.CONFIG, true, "admin.change.itempresets", "admin.use.itempresets"); // (medium)
	}

	override string GetRequiredVersion() {
		return "Admin_Tools";
	}

}
// Should not be edited manually, but only ingame through the admin menu
class LBAdminItemPresetConfigServer_ : LBConfigBase {

	static const int CURRENT_VERSION = 1;

	ref array<ref LBAdminItemPresetItem> presets = new array<ref LBAdminItemPresetItem>(); // All presets saved on the server

	[NonSerialized()]
	static bool wasEdited = false;

	void SavePreset(LBAdminItemPresetItem preset) {
		g_Game.RPCSingleParam(null, LBAdminToolRPCs.PRESET_SAVE, new Param1<ref LBAdminItemPresetItem>(preset), true);
	}

	int GetPresetIndex(string name) {
		for (int i = 0; i < presets.Count(); ++i) {
			if (presets.Get(i).itemname == name)
				return i;
		}
		return -1;
	}

	bool OnDeleteRequest(Param1<string> param) {
		int index = GetPresetIndex(param.param1);
		if (index < 0)
			return false;
		presets.RemoveOrdered(index);
		wasEdited = true;
		return true;
	}

	bool OnRenameRequest(Param2<string, string> param) {
		int index = GetPresetIndex(param.param1);
		if (index < 0)
			return false;
		int newNameIndex = GetPresetIndex(param.param2);
		if (newNameIndex >= 0)
			return false;
		presets.Get(index).itemname = param.param2;
		wasEdited = true;
		return true;
	}

	bool OnCreateRequest(Param1<string> param) {
		int index = GetPresetIndex(param.param1);
		if (index >= 0)
			return false;
		LBAdminItemPresetItem preset = new LBAdminItemPresetItem();
		preset.itemname = param.param1;
		presets.Insert(preset);
		wasEdited = true;
		return true;
	}

	bool OnSaveRequest(Param1<ref LBAdminItemPresetItem> param) {
		int index = GetPresetIndex(param.param1.itemname);
		if (index < 0)
			return false;
		presets.Set(index, param.param1);
		wasEdited = true;
		return true;
	}

}
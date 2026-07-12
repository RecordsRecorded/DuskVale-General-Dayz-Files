class LBAdminItemPresetConfig : LBConfigLoader<LBAdminItemPresetConfig_> {

	override void InitVars() {
		InitVarsInternal("AdminTool", "itempresets.json"); // (medium)
	}

	override string GetRequiredVersion() {
		return "Admin_Tools";
	}

	override bool IsClientSideConfig() {
		return true;
	}

}
// Should not be edited manually, but only ingame through the admin menu
class LBAdminItemPresetConfig_ : LBConfigBase {

	static const int CURRENT_VERSION = 1;

	ref array<ref LBAdminItemPresetItem> presets = new array<ref LBAdminItemPresetItem>(); // List of all saved presets;

}
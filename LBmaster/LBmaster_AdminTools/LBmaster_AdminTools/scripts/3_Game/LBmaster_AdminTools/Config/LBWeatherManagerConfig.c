class LBWeatherManagerConfig : LBConfigLoader<LBWeatherManagerConfig_> {

	override void InitVars() {
		InitVarsInternal("AdminTool", "weatherPresets.json");
	}

	override string GetRequiredVersion() {
		return "Admin_Tools";
	}

	override bool IsClientSideConfig() {
		return true;
	}

}
class LBWeatherManagerConfig_ : LBConfigBase {

	static const int CURRENT_VERSION = 0;

	ref array<ref LBNamedTimePreset> timePresets = new array<ref LBNamedTimePreset>();
	ref array<ref LBNamedWeatherPreset> weatherPresets = new array<ref LBNamedWeatherPreset>();

	override void LoadDefault() {
		timePresets.Insert(LBNamedTimePreset.Init("#lb_adm_preset_day", -1, -1, -1, 12, 0));
		timePresets.Insert(LBNamedTimePreset.Init("#lb_adm_preset_night", -1, -1, -1, 0, 0));
		timePresets.Insert(LBNamedTimePreset.Init("#lb_adm_preset_morning", -1, -1, -1, 6, 0));
		timePresets.Insert(LBNamedTimePreset.Init("#lb_adm_preset_evening", -1, -1, -1, 18, 0));
		timePresets.Insert(LBNamedTimePreset.Init("#lb_adm_preset_january", -1, 1, 1, -1, -1));
		timePresets.Insert(LBNamedTimePreset.Init("#lb_adm_preset_juli", -1, 7, 1, -1, -1));

		weatherPresets.Insert(LBNamedWeatherPreset.Init("#lb_adm_preset_clear", new Param3<float, float, float>(0, 0, 0), new Param4<float, float, float, float>(0, 0, 0, 0), new Param2<float, float>(0, 0), 10, 600));
		weatherPresets.Insert(LBNamedWeatherPreset.Init("#lb_adm_preset_stormy", new Param3<float, float, float>(1, 1, 0), new Param4<float, float, float, float>(0, 0, 0, 0), new Param2<float, float>(0, 0), 10, 600));
		weatherPresets.Insert(LBNamedWeatherPreset.Init("#lb_adm_preset_foggy", new Param3<float, float, float>(0.2, 0, 0), new Param4<float, float, float, float>(0.8, 0, 0, 0), new Param2<float, float>(0, 0), 10, 600));
	}

}
[DayZGame.RegisterLBAdminPage("LBSafezoneAdminPage", "LBmaster_Groups/logo/logo.paa", "#lb_ag_admin_menu_safezone_marker", "groups.change.safezonemarkers")]
class LBSafezoneAdminPage : LBAdmin_Menu_Page {

	XComboBoxWidget combo_config;
	TextWidget player_color;
	CheckBoxWidget chckbx_enable, chckbx_showEverywhere, chckbx_lineofsight, chckbx_obfuscate, chckbx_ownTag, chckbx_ignore_obf;
	Widget player_color_out;
	ButtonWidget btn_add, btn_del, btn_change_color;
	EditBoxWidget edit_distance, edit_player_prefix, edit_player_perm;

	override array<string> GetEditedConfigs() {
		return {"LBSafezoneMarkers"};
	}

	override void OnShow() {
		super.OnShow();
		linked.LoadLinkedVars();
	}

	override void RegisterAllLinkedVars() {
		linked.RegisterLinkedVar("GetConfig().enablePlayerMarkers", chckbx_enable);
		linked.RegisterLinkedVar("GetConfig().showMarkersEverywhere", chckbx_showEverywhere);
		linked.RegisterLinkedVar("GetConfig().checkLineOfSight", chckbx_lineofsight);
		linked.RegisterLinkedVar("GetConfig().obfuscatePlayerNames", chckbx_obfuscate);
		linked.RegisterLinkedVar("GetConfig().showOwnPlayerTag", chckbx_ownTag);
		linked.RegisterLinkedVar("GetConfig().maxPlayerDistance", edit_distance);

		linked.RegisterLinkedList("GetConfig().safezoneMarkers", combo_config, btn_add, btn_del).SetListManager(new LBListManager<LBSafezoneMarkerEntry>()).SetChangeTrigger("UpdateColorText()");
		linked.RegisterLinkedVar("GetConfig().safezoneMarkers[combo_config].prefix", edit_player_prefix).SetReloadTrigger(combo_config);
		linked.RegisterLinkedVar("GetConfig().safezoneMarkers[combo_config].permission", edit_player_perm).SetReloadTrigger(combo_config);
		linked.RegisterLinkedVar("GetConfig().safezoneMarkers[combo_config].ignoreNameObfuscationSetting", chckbx_ignore_obf).SetReloadTrigger(combo_config);
		linked.RegisterLinkedVar("GetConfig().safezoneMarkers[combo_config].color", btn_change_color).EnableColorPicker(true, "", "", "", "", player_color_out).SetReloadTrigger(combo_config);
	}

	LBSafezoneMarkers_ GetConfig() {
		return LBSafezoneMarkers.Get;
	}
}
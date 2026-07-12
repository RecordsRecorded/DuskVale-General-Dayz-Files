[DayZGame.RegisterLBAdminPage("LBLevelsAdminPage", "LBmaster_Groups/logo/logo.paa", "#lb_ag_admin_menu_levels", "groups.change.levels")]
class LBLevelsAdminPage : LBAdmin_Menu_Page {

	TextListboxWidget list_levels, list_subgroups;
	ButtonWidget btn_add, btn_remove, btn_up, btn_down, btn_add_subgroup, btn_remove_subgroup;
	EditBoxWidget edit_max, edit_cost, edit_markers, edit_pings, edit_plotpole, edit_number, edit_sub_size, edit_sub_name;
	CheckBoxWidget chckbx_sub_offline;

	override array<string> GetEditedConfigs() {
		return {"LBGroupLevels"};
	}

	override void OnShow() {
		super.OnShow();
		linked.LoadLinkedVars();
	}

	override void RegisterAllLinkedVars() {
		linked.RegisterLinkedList("GetConfig().levels", list_levels, btn_add, btn_remove).SetListManager(new LBListManager<LBGroupLevel>(), true);
		linked.RegisterLinkedVar("GetConfig().levels[list_levels].maxPlayerCount", edit_max);
		linked.RegisterLinkedVar("GetConfig().levels[list_levels].upgradeCost", edit_cost);
		linked.RegisterLinkedVar("GetConfig().levels[list_levels].groupMarkerLimitAdded", edit_markers);
		linked.RegisterLinkedVar("GetConfig().levels[list_levels].groupPingsLimitAdded", edit_pings);
		linked.RegisterLinkedVar("GetConfig().levels[list_levels].groupPlotpolesLimitAdded", edit_plotpole);
		linked.RegisterLinkedVar("GetConfig().levels[list_levels].level", edit_number, true);
		linked.RegisterLinkedList("GetConfig().levels[list_levels].subgroups", list_subgroups, btn_add_subgroup, btn_remove_subgroup).SetListManager(new LBListManager<LBGroupLevelSubgroupInfo>());
		linked.RegisterLinkedVar("GetConfig().levels[list_levels].subgroups[list_subgroups].size", edit_sub_size, true);
		linked.RegisterLinkedVar("GetConfig().levels[list_levels].subgroups[list_subgroups].offlineOnly", chckbx_sub_offline);
		linked.RegisterLinkedVar("GetConfig().levels[list_levels].subgroups[list_subgroups].nameOverride", edit_sub_name, true);
	}

	LBGroupLevels_ GetConfig() {
		return LBGroupLevels.Get;
	}
}
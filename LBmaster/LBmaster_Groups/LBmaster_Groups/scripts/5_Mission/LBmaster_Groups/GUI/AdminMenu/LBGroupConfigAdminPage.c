[DayZGame.RegisterLBAdminPage("LBGroupConfigAdminPage", "LBmaster_Groups/logo/logo.paa", "Group Settings", "groups.change.groupsettings")]
class LBGroupConfigAdminPage : LBAdmin_Menu_Page {

	TextListboxWidget list_sub_names;
	CheckBoxWidget chckbx_invite_action, chckbx_invite_name, chckbx_friendly_fire, chckbx_friendly_fire_sub, chckbx_secondary_group, chckbx_rem_temp_restart;
	CheckBoxWidget chckbx_rem_temp_leave, chckbx_subgroups, chckbx_subgroups_player, chckbx_subgroups_ping, chckbx_upgrade_bene, chckbx_obfuscate_list, chckbx_playerlist_enable;
	CheckBoxWidget chckbx_playerlist_distance, chckbx_obfuscate;
	ButtonWidget btn_add_subname, btn_del_subname;
	EditBoxWidget edit_inactive_del, edit_invite_cooldown, edit_sub_name;

	override void OnPageShown() {
		super.OnPageShown();
		linked.LoadLinkedVars();
	}

	override array<string> GetEditedConfigs() {
		return {"LBGroupSettingsConfig"};
	}

	override void RegisterAllLinkedVars() {
		linked.RegisterLinkedVar("GetGroupsConfig().inactiveGroupLifetimeDays", edit_inactive_del);
		linked.RegisterLinkedVar("GetGroupsConfig().inviteActionEnabled", chckbx_invite_action);
		linked.RegisterLinkedVar("GetGroupsConfig().inviteActionShowName", chckbx_invite_name);
		linked.RegisterLinkedVar("GetGroupsConfig().inviteCooldownSeconds", edit_invite_cooldown);
		linked.RegisterLinkedVar("GetGroupsConfig().preventFriendlyFire", chckbx_friendly_fire);
		linked.RegisterLinkedVar("GetGroupsConfig().preventFriendlyFireOnlyInSameSubgroup", chckbx_friendly_fire_sub);
		linked.RegisterLinkedVar("GetGroupsConfig().allowJoinSecondGroupTemporarily", chckbx_secondary_group);
		linked.RegisterLinkedVar("GetGroupsConfig().removeTemporaryMemberOnServerStart", chckbx_rem_temp_restart);
		linked.RegisterLinkedVar("GetGroupsConfig().removeTemporaryMemberOnPlayerLeave", chckbx_rem_temp_leave);
		linked.RegisterLinkedVar("GetGroupsConfig().enableSubGroups", chckbx_subgroups);
		linked.RegisterLinkedVar("GetGroupsConfig().enableSubGroupSharedPlayerMapMarker", chckbx_subgroups_player);
		linked.RegisterLinkedVar("GetGroupsConfig().enableSubGroupSharedPingMapMarker", chckbx_subgroups_ping);
		linked.RegisterLinkedVar("GetGroupsConfig().groupManagePageObfuscatePlayernames", chckbx_obfuscate_list);
		linked.RegisterLinkedVar("GetGroupsConfig().showLevelUpgradeBenefits", chckbx_upgrade_bene);
		linked.RegisterLinkedList("GetGroupsConfig().subGroupNames", list_sub_names, btn_add_subname, btn_del_subname, edit_sub_name);
		linked.RegisterLinkedVar("GetGroupsConfig().enablePlayerList", chckbx_playerlist_enable);
		linked.RegisterLinkedVar("GetGroupsConfig().enablePlayerListDistance", chckbx_playerlist_distance);
		linked.RegisterLinkedVar("GetGroupsConfig().groupManagePageObfuscatePlayernames", chckbx_obfuscate);

	}

	LBGroupSettingsConfig_ GetGroupsConfig() {
		return LBGroupSettingsConfig.Get;
	}
}
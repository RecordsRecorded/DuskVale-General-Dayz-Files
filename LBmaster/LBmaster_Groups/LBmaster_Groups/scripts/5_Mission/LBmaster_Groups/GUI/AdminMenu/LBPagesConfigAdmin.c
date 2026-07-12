[DayZGame.RegisterLBAdminPage("LBPagesConfigAdmin", "LBmaster_Groups/logo/logo.paa", "Map Pages", "groups.change.pages")]
class LBPagesConfigAdmin : LBAdmin_Menu_Page {

	CheckBoxWidget chckbx_survivor_count, chckbx_cursor_coords, chckbx_player_pos, chckbx_ingame_time, chckbx_real_time, chckbx_creator_mention, chckbx_info_enabled;
	CheckBoxWidget chckbx_group_enabled, chckbx_markers_enabled, chckbx_settings_enabled, chckbx_achiev_enabled, chckbx_rest_enabled, chckbx_lead_enabled;
	Widget panel_achievement, panel_item_rest, panel_leaderboard;
	XComboBoxWidget combo_info_buttons;
	EditBoxWidget edit_button_title, edit_button_subtext, edit_button_link, edit_info_priority, edit_info_text, edit_button_tooltip, edit_group_priority, edit_group_text;
	EditBoxWidget edit_markers_priority, edit_markers_text, edit_settings_priority, edit_settings_text, edit_achiev_priority, edit_achiev_text, edit_rest_priority;
	EditBoxWidget edit_rest_text, edit_lead_priority, edit_lead_text;

	override void OnPageShown() {
		super.OnPageShown();
		linked.LoadLinkedVars();
	}

	override array<string> GetEditedConfigs() {
		return {"LBPagesConfig"};
	}

	override void RegisterAllLinkedVars() {
		// Info Page
		linked.RegisterLinkedVar("GetConfig().infoPageSettings.showSurvivorCount", chckbx_survivor_count);
		linked.RegisterLinkedVar("GetConfig().infoPageSettings.showCursorCoordinates", chckbx_cursor_coords);
		linked.RegisterLinkedVar("GetConfig().infoPageSettings.showPlayerPositionCoordinates", chckbx_player_pos);
		linked.RegisterLinkedVar("GetConfig().infoPageSettings.showIngameTime", chckbx_ingame_time);
		linked.RegisterLinkedVar("GetConfig().infoPageSettings.showRealTime", chckbx_real_time);
		linked.RegisterLinkedVar("GetConfig().infoPageSettings.enableModCreatorMention", chckbx_creator_mention);
		linked.RegisterLinkedVar("GetCurrentButtonConfig().buttonName", edit_button_title).SetReloadTrigger(combo_info_buttons);
		linked.RegisterLinkedVar("GetCurrentButtonConfig().subtext", edit_button_subtext).SetReloadTrigger(combo_info_buttons);
		linked.RegisterLinkedVar("GetCurrentButtonConfig().hoverTooltip", edit_button_tooltip).SetReloadTrigger(combo_info_buttons);
		linked.RegisterLinkedVar("GetCurrentButtonConfig().link", edit_button_link).SetReloadTrigger(combo_info_buttons);

		linked.RegisterLinkedVar("GetConfig().infoPageSettings.enabled", chckbx_info_enabled);
		linked.RegisterLinkedVar("GetConfig().infoPageSettings.priority", edit_info_priority);
		linked.RegisterLinkedVar("GetConfig().infoPageSettings.buttonText", edit_info_text);

		linked.RegisterLinkedVar("GetConfig().groupPageSettings.enabled", chckbx_group_enabled);
		linked.RegisterLinkedVar("GetConfig().groupPageSettings.priority", edit_group_priority);
		linked.RegisterLinkedVar("GetConfig().groupPageSettings.buttonText", edit_group_text);

		linked.RegisterLinkedVar("GetConfig().markersPageSettings.enabled", chckbx_markers_enabled);
		linked.RegisterLinkedVar("GetConfig().markersPageSettings.priority", edit_markers_priority);
		linked.RegisterLinkedVar("GetConfig().markersPageSettings.buttonText", edit_markers_text);

		linked.RegisterLinkedVar("GetConfig().settingsPageSettings.enabled", chckbx_settings_enabled);
		linked.RegisterLinkedVar("GetConfig().settingsPageSettings.priority", edit_settings_priority);
		linked.RegisterLinkedVar("GetConfig().settingsPageSettings.buttonText", edit_settings_text);

		linked.RegisterLinkedVar("GetConfig().achievementsPageSettings.enabled", chckbx_achiev_enabled);
		linked.RegisterLinkedVar("GetConfig().achievementsPageSettings.priority", edit_achiev_priority);
		linked.RegisterLinkedVar("GetConfig().achievementsPageSettings.buttonText", edit_achiev_text);

		linked.RegisterLinkedVar("GetConfig().itemRestrictionsPageSettings.enabled", chckbx_rest_enabled);
		linked.RegisterLinkedVar("GetConfig().itemRestrictionsPageSettings.priority", edit_rest_priority);
		linked.RegisterLinkedVar("GetConfig().itemRestrictionsPageSettings.buttonText", edit_rest_text);

		linked.RegisterLinkedVar("GetConfig().leaderboardPageSettings.enabled", chckbx_lead_enabled);
		linked.RegisterLinkedVar("GetConfig().leaderboardPageSettings.priority", edit_lead_priority);
		linked.RegisterLinkedVar("GetConfig().leaderboardPageSettings.buttonText", edit_lead_text);

#ifdef LBmaster_AchievementsGroupPage
		panel_achievement.Show(true);
#else
		panel_achievement.Show(false);
#endif
#ifdef LBmaster_ItemRestrictions
		panel_item_rest.Show(true);
#else
		panel_item_rest.Show(false);
#endif
#ifdef LBmaster_LeaderboardGroupPage
		panel_leaderboard.Show(true);
#else
		panel_leaderboard.Show(false);
#endif

	}

	LBPagesConfig_ GetConfig() {
		return LBPagesConfig.Get;
	}

	LBButtonConfig GetCurrentButtonConfig() {
		int selected = combo_info_buttons.GetCurrentItem();
		if (selected < 0 || selected >= 6)
			return null;
		while (selected >= GetConfig().infoPageSettings.buttonConfig.Count())
			GetConfig().infoPageSettings.buttonConfig.Insert(new LBButtonConfig());
		return GetConfig().infoPageSettings.buttonConfig.Get(selected);
	}

}
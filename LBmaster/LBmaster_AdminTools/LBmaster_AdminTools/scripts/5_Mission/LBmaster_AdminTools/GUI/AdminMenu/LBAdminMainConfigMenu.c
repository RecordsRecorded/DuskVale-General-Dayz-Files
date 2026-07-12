[DayZGame.RegisterLBAdminPage("LBAdminMainConfigMenu", "LBmaster_AdminTools/logo/logo.paa", "Main Config", "admin.change.mainconfig")]
class LBAdminMainConfigMenu : LBAdmin_Menu_Page {

	TextListboxWidget list_detections;
	CheckBoxWidget chckbx_possible_watchlist, chckbx_global_bans, chckbx_duped_login, chckbx_duped_restart, chckbx_prevent_teleport_hack, chckbx_prevent_loot_walls;
	CheckBoxWidget chckbx_validate_action_origin, chckbx_validate_action_distance, chckbx_log_inv_events, chckbx_prevent_invalid_mag;
	ButtonWidget btn_toggle_detection;
	EditBoxWidget edit_status_interval;

	override void OnShow() {
		linked.LoadLinkedVars();
	}

	override void RegisterAllLinkedVars() {
		linked.RegisterLinkedVar("GetConfig().serverStatusWebhookInterval", edit_status_interval);
		linked.RegisterLinkedVar("GetConfig().addPossibleCheatersToWatchlist", chckbx_possible_watchlist);
		linked.RegisterLinkedVar("GetConfig().contributeToGlobalBanlist", chckbx_global_bans);
		linked.RegisterLinkedVar("GetConfig().deleteDupedItemsOnLogin", chckbx_duped_login);
		linked.RegisterLinkedVar("GetConfig().deleteDupedItemsOnServerRestart", chckbx_duped_restart);
		linked.RegisterLinkedVar("GetConfig().preventItemTeleportHacks", chckbx_prevent_teleport_hack);
		linked.RegisterLinkedVar("GetConfig().preventLootThroughWalls", chckbx_prevent_loot_walls);
		linked.RegisterLinkedVar("GetConfig().validateActionOrigin", chckbx_validate_action_origin);
		linked.RegisterLinkedVar("GetConfig().validateActionDistance", chckbx_validate_action_distance);
		linked.RegisterLinkedVar("GetConfig().logInventoryEvents", chckbx_log_inv_events);
		linked.RegisterLinkedVar("GetConfig().preventIllegalMagazines", chckbx_prevent_invalid_mag);
		linked.RegisterLinkedList("GetConfig().watchlistCheatDetections", list_detections, null, null, null).SetListManager(new LBListManagerMapStringBool()).SetColumnCount(2);
	}

	override bool OnItemSelected(Widget w, int x, int y, int row, int column, int oldRow, int oldColumn) {
		if (super.OnItemSelected(w, x, y, row, column, oldRow, oldColumn))
			return true;
		if (w == list_detections) {
			LoadDetectionValue();
			return true;
		}
		return false;
	}

	override bool OnClick(Widget w, int x, int y, int button) {
		if (super.OnClick(w, x, y, button))
			return true;
		if (w == btn_toggle_detection) {
			int selected;
			if (!LBWidgetUtils.CheckSelectedInRange(list_detections, selected))
				return true;
			bool enabled = GetConfig().watchlistCheatDetections.GetElement(selected);
			string name = GetConfig().watchlistCheatDetections.GetKey(selected);
			GetConfig().watchlistCheatDetections.Set(name, !enabled);
			LoadDetectionValue();
			linked.TriggerLoad("GetConfig().watchlistCheatDetections");
			return true;
		}
		return false;
	}

	void LoadDetectionValue() {
		int selected;
		if (!LBWidgetUtils.CheckSelectedInRange(list_detections, selected))
			return;
		bool enabled = GetConfig().watchlistCheatDetections.GetElement(selected);
		if (enabled)
			btn_toggle_detection.SetText("Disable");
		else
			btn_toggle_detection.SetText("Enable");
	}

	LBAdminToolsMainConfig_ GetConfig() {
		return LBAdminToolsMainConfig.Get;
	}

	override array<string> GetEditedConfigs() {
		return {"LBAdminToolsMainConfig"};
	}

}
[DayZGame.RegisterLBAdminPage("LBMarkerConfigAdminPage", "LBmaster_Groups/logo/logo.paa", "Marker Settings", "groups.change.markersettings")]
class LBMarkerConfigAdminPage : LBAdmin_Menu_Page {

	CheckBoxWidget chckbx_own_marker, chckbx_own_marker_item, chckbx_add_death, chckbx_death_private, chckbx_death_group, chckbx_death_delete, chckbx_marker_place;
	CheckBoxWidget chckbx_display3d, chckbx_displaydistance, chckbx_displaymap, chckbx_displaycompass, chckbx_displaygps, chckbx_group_see_item;
	CheckBoxWidget chckbx_group_seen_item;
	XComboBoxWidget combo_marker_config;
	ImageWidget img_death_icon, img_icon_preview;
	EditBoxWidget edit_death_icon, edit_marker_limit, edit_ping_lifetime, edit_ping_limit, edit_offline_distance, edit_max_distance;
	TextListboxWidget list_own_marker_items, list_icons, list_see_items;
	ButtonWidget btn_add_own_marker, btn_del_own_marker, btn_death_browse, btn_del_icon, btn_add_icon, btn_add_see, btn_del_see;

	string lastPath;
	ref TIntArray combo_to_id = new TIntArray();

	override void OnPageShown() {
		super.OnPageShown();
		linked.LoadLinkedVars();
	}

	override array<string> GetEditedConfigs() {
		return {"LBMarkerSettingsConfig"};
	}

	override void RegisterAllLinkedVars() {
		// Marker
		linked.RegisterLinkedVar("GetMarkerConfig().canSeeOwnPlayerOnMap", chckbx_own_marker);
		linked.RegisterLinkedVar("GetMarkerConfig().requireItemToSeeOwnPlayerOnMap", chckbx_own_marker_item);
		linked.RegisterLinkedVar("GetMarkerConfig().addPlayerDeathMarker", chckbx_add_death);
		linked.RegisterLinkedVar("GetMarkerConfig().deathMarkerPrivate", chckbx_death_private);
		linked.RegisterLinkedVar("GetMarkerConfig().deathMarkerGroup", chckbx_death_group);
		linked.RegisterLinkedVar("GetMarkerConfig().deleteOldDeathMarker", chckbx_death_delete);
		linked.RegisterLinkedVar("GetMarkerConfig().deathMarkerIcon", edit_death_icon).EnableFileSelector({".paa", ".edds"}, btn_death_browse).SetChangeTrigger("LoadDeathIcon()");
		linked.RegisterLinkedList("GetMarkerConfig().availableIcons", list_icons, null, btn_del_icon);
		linked.RegisterLinkedVar("GetMarkerConfig().disableMarkerPlacement", chckbx_marker_place);
		linked.RegisterLinkedVar("GetMarkerConfig().groupMarkerLimit", edit_marker_limit);
		linked.RegisterLinkedVar("GetMarkerConfig().tacticalPingLifetimeSeconds", edit_ping_lifetime);
		linked.RegisterLinkedVar("GetMarkerConfig().tacticalPingMaxConcurrentMarkersPerPlayer", edit_ping_limit);
		linked.RegisterLinkedVar("GetMarkerConfig().offlinePlayer3dMarkerDistance", edit_offline_distance);
		LoadMarkerConfigTypes();
		linked.RegisterLinkedVar("GetCurrentMarkerConfigEntry().maxDistance", edit_max_distance).SetReloadTrigger(combo_marker_config);
		linked.RegisterLinkedVar("GetCurrentMarkerConfigEntry().display3d", chckbx_display3d).SetReloadTrigger(combo_marker_config);
		linked.RegisterLinkedVar("GetCurrentMarkerConfigEntry().displayDistance", chckbx_displaydistance).SetReloadTrigger(combo_marker_config);
		linked.RegisterLinkedVar("GetCurrentMarkerConfigEntry().displayMap", chckbx_displaymap).SetReloadTrigger(combo_marker_config);
		linked.RegisterLinkedVar("GetCurrentMarkerConfigEntry().displayCompass", chckbx_displaycompass).SetReloadTrigger(combo_marker_config);
		linked.RegisterLinkedVar("GetCurrentMarkerConfigEntry().displayGPS", chckbx_displaygps).SetReloadTrigger(combo_marker_config);
		combo_marker_config.SetCurrentItem(0);
		// Items
		TStringSet itemSet = LBInherit.Get().GetAllChildren({"Inventory_Base"}, false, true, true);
		linked.RegisterLinkedVar("GetMarkerConfig().requireItemToSeeGroupMembers", chckbx_group_see_item);
		linked.RegisterLinkedVar("GetMarkerConfig().requireItemToBeSeenByGroupMembers", chckbx_group_seen_item);
		linked.RegisterLinkedList("GetMarkerConfig().playerMarkerItems", list_see_items, btn_add_see, btn_del_see, null).EnableItemSelector(true, "Inventory_Base").EnableInListSearch(itemSet);
		linked.RegisterLinkedList("GetMarkerConfig().ownPlayerMarkerItems", list_own_marker_items, btn_add_own_marker, btn_del_own_marker, null).EnableItemSelector(true, "Inventory_Base").EnableInListSearch(itemSet);

	}

	void LoadMarkerConfigTypes() {
		combo_marker_config.ClearAll();
		typename tn = LBMarkerType;
		int count = tn.GetVariableCount();
		for (int i = 0; i < count; ++i) {
			string name = tn.GetVariableName(i);
			combo_marker_config.AddItem(name);
			combo_to_id.Insert(typename.StringToEnum(LBMarkerType, name));
		}
	}

	void LoadDeathIcon() {
		img_death_icon.LoadImageFile(0, GetMarkerConfig().deathMarkerIcon);
	}

	LBMarkerSettingsConfig_ GetMarkerConfig() {
		return LBMarkerSettingsConfig.Get;
	}

	MarkerConfigEntry GetCurrentMarkerConfigEntry() {
		int selected = combo_marker_config.GetCurrentItem();
		if (selected < 0 || selected >= GetMarkerConfig().markerConfig.Count())
			return null;
		return GetMarkerConfig().GetMarkerConfigEntry(combo_to_id.Get(selected), false);
	}

	MarkerConfigEntry GetCurrentMarkerConfigEntryFOW() {
		int selected = combo_marker_config.GetCurrentItem();
		if (selected < 0 || selected >= GetMarkerConfig().markerConfig.Count())
			return null;
		return GetMarkerConfig().GetMarkerConfigEntry(combo_to_id.Get(selected), true);
	}

	void OnIconSelected(string value) {
		value.Replace("/", "\\");
		lastPath = value;
		if (GetMarkerConfig().availableIcons.Find(value) != -1)
			return;
		GetMarkerConfig().availableIcons.Insert(value);
		linked.TriggerLoad("GetMarkerConfig().availableIcons");
	}

	override bool OnClick(Widget w, int x, int y, int button) {
		if (super.OnClick(w, x, y, button))
			return true;
		if (w == btn_add_icon) {
			LBMenuManager.Get().OpenMenu("LBFileSelectorDialog", new LBMenuDataFileSelector(lastPath, "#lb_file_selector", "#lb_select", {".paa", ".edds"}, ScriptCaller.Create(OnIconSelected)), false, LBMenuManager.Get().GetOpenLBMenu());
			return true;
		}
		return false;
	}

	override bool OnItemSelected(Widget w, int x, int y, int row, int column, int oldRow, int oldColumn) {
		if (super.OnItemSelected(w, x, y, row, column, oldRow, oldColumn))
			return true;
		if (w == list_icons) {
			int selected = list_icons.GetSelectedRow();
			if (selected < 0 || selected > GetMarkerConfig().availableIcons.Count())
				return true;
			img_icon_preview.LoadImageFile(0, GetMarkerConfig().availableIcons.Get(selected));
			return true;
		}
		return false;
	}
}
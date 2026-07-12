class LBAdminMenuFrame_Settings : LBAdminMenuFrame {

	override string GetTitle() {
		return "#lb_adm_title_settings";
	}

	override string GetButtonIcon() {
		return "LBmaster_AdminTools/gui/icons/settings.paa";
	}

	override void GetMinSize(out int width, out int height) {
		width = 300;
		height = 421;
	}

	override void GetInitialSize(out int width, out int height) {
		width = 700;
		height = 421;
	}

	override TStringArray GetRequiredPermissionsOne() {
		return new TStringArray();
	}

	override string GetInfoString() {
		return "#lb_adm_desc_settings";
	}

	CheckBoxWidget chckbx_invis_player, chckbx_watch_local, chckbx_watch_server, chckbx_watch_global, chckbx_esp_detail, chckbx_esp_bones, chckbx_skeleton_ig_dist;
	CheckBoxWidget chckbx_display_group_tags;
	XComboBoxWidget combo_esp_filter, combo_spawner_cats, combo_preset_name;
	EditBoxWidget edit_esp_name, edit_spawner_name, edit_preset_name;
	TextListboxWidget list_esp_items, list_spawner_items;
	ButtonWidget btn_add_esp, btn_remove_esp, btn_add_esp_item, btn_remove_esp_item, btn_esp_color, btn_add_spawner_item, btn_remove_spawner_item, btn_add_spawner;
	ButtonWidget btn_remove_spawner, btn_frame_color, btn_skeleton_color, btn_joint_color, btn_skeleton_color_dead, btn_map_player_color, btn_map_vehicle_color;
	ButtonWidget btn_map_boat_color, btn_map_heli_color, btn_highlighted_player_color;
	Widget panel_esp_color, panel_frame_color, panel_skeleton_color, panel_joint_color, panel_skeleton_color_dead, panel_map_player_color, panel_map_vehicle_color;
	Widget panel_map_boat_color, panel_map_heli_color, panel_highlighted_player_color;

	ref LBColorPicker colorPicker;

	override void Show(bool show) {
		super.Show(show);
		if (show) {
			LoadConfig();
		}
	}

	void LoadConfig() {
		chckbx_esp_detail.SetChecked(LBAdminMenuSettings.Get.espSkeletonHighDetail);
		chckbx_esp_bones.SetChecked(LBAdminMenuSettings.Get.espSkeletonDrawJoints);
		chckbx_skeleton_ig_dist.SetChecked(LBAdminMenuSettings.Get.espSkeletonIgnoreDistance);
		panel_skeleton_color.SetColor(LBAdminMenuSettings.Get.espSkeletonColor);
		panel_joint_color.SetColor(LBAdminMenuSettings.Get.espJointsColor);
		panel_skeleton_color_dead.SetColor(LBAdminMenuSettings.Get.espSkeletonColorDead);
		panel_highlighted_player_color.SetColor(LBAdminMenuSettings.Get.highlightedPlayerColor);

		chckbx_invis_player.SetChecked(LBAdminMenuSettings.Get.showPlayerInInvisible);
		chckbx_watch_local.SetChecked(LBAdminMenuSettings.Get.alertJoinWatchlistLocal);
		chckbx_watch_server.SetChecked(LBAdminMenuSettings.Get.alertJoinWatchlistServer);
		chckbx_watch_global.SetChecked(LBAdminMenuSettings.Get.alertJoinWatchlistGlobal);
		chckbx_display_group_tags.SetChecked(LBAdminMenuSettings.Get.displayPlayerTagsInPlayerList);

		combo_esp_filter.ClearAll();
		foreach (LBESPFilter filter : LBAdminMenuSettings.Get.espFilters) {
			combo_esp_filter.AddItem(filter.name);
		}

		LoadSpawnerCategories();

		panel_frame_color.SetColor(LBAdminMenuSettings.Get.GetFrameColor());
		panel_map_player_color.SetColor(LBAdminMenuSettings.Get.mapPlayerColor);
		panel_map_vehicle_color.SetColor(LBAdminMenuSettings.Get.mapVehicleColor);
		panel_map_boat_color.SetColor(LBAdminMenuSettings.Get.mapBoatColor);
		panel_map_heli_color.SetColor(LBAdminMenuSettings.Get.mapHeliColor);
		LoadSelectedSpawnerCategory();
		LoadSelectedESPFilter();
		LoadWindowPresetName();
	}

	void LoadWindowPresetName() {
		int selected = combo_preset_name.GetCurrentItem();
		string name = LBAdminMenuWindowConfig.Get.GetPresetName(selected);
		edit_preset_name.SetText(name);
	}

	void LoadSpawnerCategories() {
		combo_spawner_cats.ClearAll();
		int spawnerCategories = LBAdminMenuSettings.Get.itemspawnerCategoryNames.Count();
		if (spawnerCategories > LBAdminMenuSettings.Get.itemspawnerCategories.Count())
			spawnerCategories = LBAdminMenuSettings.Get.itemspawnerCategories.Count();
		foreach (string catName : LBAdminMenuSettings.Get.itemspawnerCategoryNames)
			combo_spawner_cats.AddItem(catName);
	}

	void LoadSelectedSpawnerCategory() {
		list_spawner_items.ClearItems();
		int index;
		if (!LBWidgetUtils.CheckSelectedInRange(combo_spawner_cats, index))
			return;
		TStringArray items = LBAdminMenuSettings.Get.itemspawnerCategories.Get(index);
		string name = LBAdminMenuSettings.Get.itemspawnerCategoryNames.Get(index);
		foreach (string item : items) {
			list_spawner_items.AddItem(item, null, 0);
		}
		edit_spawner_name.SetText(name);
	}

	void LoadSelectedESPFilter() {
		list_esp_items.ClearItems();
		int index;
		if (!LBWidgetUtils.CheckSelectedInRange(combo_esp_filter, index))
			return;
		LBESPFilter filter = LBAdminMenuSettings.Get.espFilters.Get(index);
		panel_esp_color.SetColor(filter.color.GetColorARGB());
		foreach (string item : filter.items) {
			list_esp_items.AddItem(item, null, 0);
		}
		edit_esp_name.SetText(filter.name);
	}

	override bool OnClick(Widget w, int x, int y, int button) {
		if (super.OnClick(w, x, y, button))
			return true;
		bool save = true;
		int index, index2;
		if (w == btn_add_esp) {
			LBESPFilter newFilter = new LBESPFilter("#lb_new", new TStringArray(), LBColorConfig.Init(255, 255, 255, 255));
			if (LBWidgetUtils.CheckSelectedInRange(combo_esp_filter, index)) {
				LBAdminMenuSettings.Get.espFilters.InsertAt(newFilter, index + 1);
				combo_esp_filter.ClearAll();
				foreach (LBESPFilter filter : LBAdminMenuSettings.Get.espFilters) {
					combo_esp_filter.AddItem(filter.name);
				}
				combo_esp_filter.SetCurrentItem(index + 1);
			} else {
				LBAdminMenuSettings.Get.espFilters.Insert(newFilter);
				combo_esp_filter.AddItem(newFilter.name);
				combo_esp_filter.SetCurrentItem(LBAdminMenuSettings.Get.espFilters.Count() - 1);
			}
			LoadSelectedESPFilter();
		} else if (w == btn_remove_esp) {
			if (LBWidgetUtils.CheckSelectedInRange(combo_esp_filter, index)) {
				combo_esp_filter.RemoveItem(index);
				LBAdminMenuSettings.Get.espFilters.RemoveOrdered(index);
			}
		} else if (w == btn_add_spawner) {
			index = combo_spawner_cats.GetCurrentItem();
			LBAdminMenuSettings.Get.itemspawnerCategoryNames.InsertAt("#lb_new", index + 1);
			LBAdminMenuSettings.Get.itemspawnerCategories.InsertAt(new TStringArray(), index + 1);
			LoadSpawnerCategories();
			combo_spawner_cats.SetCurrentItem(index + 1);
			LoadSelectedSpawnerCategory();
			LBAdminMenuFrame_ItemSpawner.needItemCategoryRefresh = true;
		} else if (w == btn_remove_spawner) {
			if (LBWidgetUtils.CheckSelectedInRange(combo_spawner_cats, index)) {
				combo_spawner_cats.RemoveItem(index);
				LBAdminMenuSettings.Get.itemspawnerCategoryNames.RemoveOrdered(index);
				LBAdminMenuSettings.Get.itemspawnerCategories.RemoveOrdered(index);
				combo_spawner_cats.SetCurrentItem(index - 1);
				LoadSelectedSpawnerCategory();
				LBAdminMenuFrame_ItemSpawner.needItemCategoryRefresh = true;
			}
		} else if (w == combo_esp_filter) {
			LoadSelectedESPFilter();
		} else if (w == combo_spawner_cats) {
			LoadSelectedSpawnerCategory();
		} else if (w == btn_add_esp_item) {
			LBMenuManager.Get().OpenMenu("LBItemSelectorDialog", new LBMenuDataItemSelector(ScriptCaller.Create(AddESPItem), false), false, parent);
			return true;
		} else if (w == btn_add_spawner_item) {
			LBMenuManager.Get().OpenMenu("LBItemSelectorDialog", new LBMenuDataItemSelector(ScriptCaller.Create(AddSpawnerItem), false), false, parent);
			return true;
		} else if (w == btn_remove_esp_item) {
			if (LBWidgetUtils.CheckSelectedInRange(list_esp_items, index2) && LBWidgetUtils.CheckSelectedInRange(combo_esp_filter, index)) {
				filter = LBAdminMenuSettings.Get.espFilters.Get(index);
				filter.items.RemoveOrdered(index2);
				filter.UpdateChildren();
				list_esp_items.RemoveRow(index2);
				LBESPManager.UpdateColors();
				LBESPManager.UpdateFilter();
			}
		} else if (w == btn_remove_spawner_item) {
			if (LBWidgetUtils.CheckSelectedInRange(list_spawner_items, index2) && LBWidgetUtils.CheckSelectedInRange(combo_spawner_cats, index)) {
				TStringArray items = LBAdminMenuSettings.Get.itemspawnerCategories.Get(index);
				items.RemoveOrdered(index2);
				list_spawner_items.RemoveRow(index2);
				LBAdminMenuFrame_ItemSpawner.needItemCategoryRefresh = true;
			}
		} else if (w == btn_esp_color) {
			if (LBWidgetUtils.CheckSelectedInRange(combo_esp_filter, index)) {
				filter = LBAdminMenuSettings.Get.espFilters.Get(index);
				colorPicker = new LBColorPicker(btn_esp_color, ScriptCaller.Create(SetESPColor), -1, filter.color, true);
			}
		} else if (w == btn_frame_color) {
			colorPicker = new LBColorPicker(btn_frame_color, ScriptCaller.Create(SetFrameColor), -1, LBAdminMenuSettings.Get.frameColor, true);
		} else if (w == chckbx_watch_local) {
			LBAdminMenuSettings.Get.alertJoinWatchlistLocal = chckbx_watch_local.IsChecked();
		} else if (w == chckbx_watch_server) {
			LBAdminMenuSettings.Get.alertJoinWatchlistServer = chckbx_watch_server.IsChecked();
		} else if (w == chckbx_watch_global) {
			LBAdminMenuSettings.Get.alertJoinWatchlistGlobal = chckbx_watch_global.IsChecked();
		} else if (w == chckbx_display_group_tags) {
			LBAdminMenuSettings.Get.displayPlayerTagsInPlayerList = chckbx_display_group_tags.IsChecked();
		} else if (w == chckbx_invis_player) {
			LBAdminMenuSettings.Get.showPlayerInInvisible = chckbx_invis_player.IsChecked();
#ifndef SERVER
			PlayerBase pb = PlayerBase.Cast(g_Game.GetPlayer());
			if (pb)
				pb.UpdateInvisibility();
#endif
		} else if (w == chckbx_esp_bones) {
			LBAdminMenuSettings.Get.espSkeletonDrawJoints = chckbx_esp_bones.IsChecked();
		} else if (w == chckbx_esp_detail) {
			LBAdminMenuSettings.Get.espSkeletonHighDetail = chckbx_esp_detail.IsChecked();
		} else if (w == chckbx_skeleton_ig_dist) {
			LBAdminMenuSettings.Get.espSkeletonIgnoreDistance = chckbx_skeleton_ig_dist.IsChecked();
		} else if (w == btn_skeleton_color) {
			colorPicker = new LBColorPicker(btn_skeleton_color, ScriptCaller.Create(SetSkeletonColor), LBAdminMenuSettings.Get.espSkeletonColor);
		} else if (w == btn_joint_color) {
			colorPicker = new LBColorPicker(btn_joint_color, ScriptCaller.Create(SetJointColor), LBAdminMenuSettings.Get.espJointsColor);
		} else if (w == btn_skeleton_color_dead) {
			colorPicker = new LBColorPicker(btn_skeleton_color_dead, ScriptCaller.Create(SetSkeletonDeadColor), LBAdminMenuSettings.Get.espSkeletonColorDead);
		} else if (w == btn_highlighted_player_color) {
			colorPicker = new LBColorPicker(btn_highlighted_player_color, ScriptCaller.Create(SetHighlightedPlayerColor), LBAdminMenuSettings.Get.highlightedPlayerColor);
		} else if (w == btn_map_player_color) {
			colorPicker = new LBColorPicker(btn_map_player_color, ScriptCaller.Create(SetMapPlayerColor), LBAdminMenuSettings.Get.mapPlayerColor);
		} else if (w == btn_map_vehicle_color) {
			colorPicker = new LBColorPicker(btn_map_vehicle_color, ScriptCaller.Create(SetMapVehicleColor), LBAdminMenuSettings.Get.mapVehicleColor);
		} else if (w == btn_map_boat_color) {
			colorPicker = new LBColorPicker(btn_map_boat_color, ScriptCaller.Create(SetMapBoatColor), LBAdminMenuSettings.Get.mapBoatColor);
		} else if (w == btn_map_heli_color) {
			colorPicker = new LBColorPicker(btn_map_heli_color, ScriptCaller.Create(SetMapHeliColor), LBAdminMenuSettings.Get.mapHeliColor);
		} else if (w == combo_preset_name) {
			LoadWindowPresetName();
			return save;
		} else {
			save = false;
		}
		if (save)
			LBAdminMenuSettings.Loader.Save();
		return save;
	}

	void SetMapPlayerColor(int color) {
		LBAdminMenuSettings.Get.mapPlayerColor = color;
		LBAdminMenuSettings.Loader.Save();
		panel_map_player_color.SetColor(color);
	}

	void SetMapVehicleColor(int color) {
		LBAdminMenuSettings.Get.mapVehicleColor = color;
		LBAdminMenuSettings.Loader.Save();
		panel_map_vehicle_color.SetColor(color);
	}

	void SetMapBoatColor(int color) {
		LBAdminMenuSettings.Get.mapBoatColor = color;
		LBAdminMenuSettings.Loader.Save();
		panel_map_boat_color.SetColor(color);
	}

	void SetMapHeliColor(int color) {
		LBAdminMenuSettings.Get.mapHeliColor = color;
		LBAdminMenuSettings.Loader.Save();
		panel_map_heli_color.SetColor(color);
	}

	void SetSkeletonDeadColor(int color) {
		LBAdminMenuSettings.Get.espSkeletonColorDead = color;
		LBAdminMenuSettings.Loader.Save();
		panel_skeleton_color_dead.SetColor(color);
	}

	void SetHighlightedPlayerColor(int color) {
		LBAdminMenuSettings.Get.highlightedPlayerColor = color;
		LBAdminMenuSettings.Loader.Save();
		panel_highlighted_player_color.SetColor(color);
		LBESPManager.UpdateColors();
	}

	void SetSkeletonColor(int color) {
		LBAdminMenuSettings.Get.espSkeletonColor = color;
		LBAdminMenuSettings.Loader.Save();
		panel_skeleton_color.SetColor(color);
	}

	void SetJointColor(int color) {
		LBAdminMenuSettings.Get.espJointsColor = color;
		LBAdminMenuSettings.Loader.Save();
		panel_joint_color.SetColor(color);
	}

	void SetFrameColor(int color) {
		parent.UpdateFrameColors();
		panel_frame_color.SetColor(color);
		LBAdminMenuSettings.Loader.Save();
	}

	void SetESPColor(int color) {
		panel_esp_color.SetColor(color);
		LBESPManager.UpdateFilter();
		LBESPManager.UpdateColors();
		LBAdminMenuSettings.Loader.Save();
	}

	void AddSpawnerItem(string itemname) {
		int index;
		if (!LBWidgetUtils.CheckSelectedInRange(combo_spawner_cats, index))
			return;
		TStringArray items = LBAdminMenuSettings.Get.itemspawnerCategories.Get(index);
		items.Insert(itemname);
		LBAdminMenuFrame_ItemSpawner.needItemCategoryRefresh = true;
		LBAdminMenuSettings.Loader.Save();
	}

	void AddESPItem(string itemname) {
		int index;
		if (!LBWidgetUtils.CheckSelectedInRange(combo_esp_filter, index))
			return;
		LBESPFilter filter = LBAdminMenuSettings.Get.espFilters.Get(index);
		filter.items.Insert(itemname);
		filter.UpdateChildren();
		LBESPManager.UpdateColors();
		LBESPManager.UpdateFilter();
		LBAdminMenuSettings.Loader.Save();
	}

	override bool OnChange(Widget w, int x, int y, bool finished) {
		if (super.OnChange(w, x, y, finished))
			return true;
		int index;
		if (w == edit_spawner_name) {
			if (LBWidgetUtils.CheckSelectedInRange(combo_spawner_cats, index)) {
				LBAdminMenuSettings.Get.itemspawnerCategoryNames.Set(index, edit_spawner_name.GetText());
				combo_spawner_cats.SetItem(index, edit_spawner_name.GetText());
				LBAdminMenuSettings.Loader.Save();
			}
			return true;
		} else if (w == edit_esp_name) {
			if (LBWidgetUtils.CheckSelectedInRange(combo_esp_filter, index)) {
				LBESPFilter filter = LBAdminMenuSettings.Get.espFilters.Get(index);
				filter.name = edit_esp_name.GetText();
				combo_esp_filter.SetItem(index, filter.name);
				LBAdminMenuSettings.Loader.Save();
			}
			return true;
		} else if (w == edit_preset_name) {
			index = combo_preset_name.GetCurrentItem();
			LBAdminMenuWindowConfig.Get.SetPresetName(index, edit_preset_name.GetText());
		}
		return false;
	}

}
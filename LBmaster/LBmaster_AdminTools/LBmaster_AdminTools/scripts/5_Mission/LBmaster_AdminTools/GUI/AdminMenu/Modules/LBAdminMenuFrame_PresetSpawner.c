class LBAdminMenuFrame_PresetSpawner : LBAdminMenuFrame {

	override string GetTitle() {
		return "#lb_adm_title_preset_spawner";
	}

	override string GetButtonIcon() {
		return "LBmaster_AdminTools/gui/icons/item_preset.paa";
	}

	override string GetInfoString() {
		return (new LBTranslatedString("#lb_adm_desc_preset_spawner")).Get();
	}

	override void GetMinSize(out int width, out int height) {
		width = 500;
		height = 400;
	}

	override void GetInitialSize(out int width, out int height) {
		width = 1000;
		height = 500;
	}

	override TStringArray GetRequiredPermissionsOne() {
		return {"admin.spawn.preset"};
	}

	XComboBoxWidget combo_spawn_in, combo_health, combo_state, combo_spawn_in_spawn, combo_from;
	SliderWidget slider_health, slider_quantity;
	CheckBoxWidget chckbx_debug, chckbx_ce;
	ButtonWidget btn_add_item, btn_remove_item, btn_spawn, btn_rename, btn_remove_preset, btn_add_preset, btn_save_server, btn_add_preset_from_char;
	EditBoxWidget edit_search, edit_health, edit_quantity;
	TextListboxWidget list_items, list_presets;
	Widget panel_add_remove, panel_item_config, panel_health_color;

	bool setLiquid = false;

	ref TIntArray availableLiquids = new TIntArray();

	override void OnInit() {
		LoadPresets();
		LBConfigManager.Get().GetEventOnConfigReceived("LBAdminItemPresetConfigServer").Insert(LoadPresets);
	}

	array<ref LBAdminItemPresetItem> GetItemPresets() {
		if (combo_from.GetCurrentItem() == 0)
			return LBAdminItemPresetConfig.Get.presets;
		return LBAdminItemPresetConfigServer.Get.presets;
	}

	override void Show(bool show) {
		super.Show(show);
		FillSpawnerLocationTypes();
		combo_from.ClearAll();
		combo_from.AddItem("#lb_adm_personal");
		if (LBAdmins.Get().HasPermission("admin.use.itempresets"))
			combo_from.AddItem("#lb_adm_server");
		int lastSelected = LBAdminMenuSettings.Get.presetSpawnerCategory;
		if (combo_from.GetNumItems() > lastSelected)
			combo_from.SetCurrentItem(lastSelected);
		UpdateSaveButtonVisible();
	}

	override bool SetPrio() {
		if (super.SetPrio()) {
			FillSpawnerLocationTypes();
			return true;
		}
		return false;
	}

	void FillSpawnerLocationTypes() {
		combo_spawn_in_spawn.ClearAll();
		combo_spawn_in_spawn.AddItem("#lb_adm_item_spawner_inventory");
		combo_spawn_in_spawn.AddItem("#lb_adm_item_spawner_ground");
		combo_spawn_in_spawn.AddItem("#lb_adm_item_spawner_cursor");
		if (LBAdmins.Get().HasPermission("admin.spawn.item_others")) {
			combo_spawn_in_spawn.AddItem("#lb_adm_item_spawner_target");
			combo_spawn_in_spawn.AddItem("#lb_adm_item_spawner_selected_player");
		}
		combo_spawn_in_spawn.SetCurrentItem(LBAdminMenuSettings.Get.itemSpawnerDefaultSpawn);
	}

	void Save() {
		if (combo_from.GetCurrentItem() == 0)
			LBAdminItemPresetConfig.Loader.Save();
	}

	void SavePresetToServer() {
		int index;
		if (!LBWidgetUtils.CheckSelectedInRange(list_presets, index))
			return;
		LBAdminItemPresetItem preset;
		list_presets.GetItemData(index, 0, preset);
		LBAdminItemPresetConfigServer.Get.SavePreset(preset);
	}

	void LoadPresets() {
		list_items.ClearItems();
		list_presets.ClearItems();
		string search = edit_search.GetText();
		array<ref LBAdminItemPresetItem> presets = GetItemPresets();
		foreach (LBAdminItemPresetItem preset : presets) {
			if (search == "" || LBStringTools.ContainsIgnoreCase(preset.itemname, search))
				list_presets.AddItem(preset.itemname, preset, 0);
		}
	}

	override bool OnClick(Widget w, int x, int y, int button) {
		if (super.OnClick(w, x, y, button))
			return true;
		if (w == btn_add_item) {
			NotificationSystem.AddNotificationExtended(10.0, "#lb_adm_admin_tools", "#lb_adm_message_select_to_add_to_preset", LBIconConfig.Get.info);
			parent.EnsureVisible("LBAdminMenuFrame_ItemSpawner");
			return true;
		} else if (w == btn_remove_item) {
			RemoveItemFromPreset();
			return true;
		} else if (w == btn_spawn) {
			RequestDefaultSpawn();
			return true;
		} else if (w == btn_rename) {
			LBSimpleInputPopup.Get().Show("#lb_adm_rename", "#lb_adm_rename_your_preset", "", ScriptCaller.Create(OnRenamePreset));
			return true;
		} else if (w == btn_remove_preset) {
			int index;
			if (!LBWidgetUtils.CheckSelectedInRange(list_presets, index))
				return true;
			LBAdminItemPresetItem preset;
			list_presets.GetItemData(index, 0, preset);
			LBWarningPopup.Get().Show("#lb_delete", LBTranslatedString("#lb_adm_message_delete_building_set", {"%name%", preset.itemname}).Get(), ScriptCaller.Create(DeleteSelectedPreset));
			return true;
		} else if (w == btn_add_preset) {
			LBSimpleInputPopup.Get().Show("#lb_add", "#lb_adm_name_your_new_preset", "", ScriptCaller.Create(OnCreateNewPreset));
			return true;
		} else if (w == btn_add_preset_from_char) {
			LBSimpleInputPopup.Get().Show("#lb_add", "#lb_adm_name_your_new_preset", "", ScriptCaller.Create(OnCreateNewPresetFromChar));
			return true;
		} else if (w == chckbx_debug) {
			LBAdminItemPresetItem item = GetSelectedItem();
			if (!item)
				return true;
			item.debugSpawn = chckbx_debug.IsChecked();
			Save();
			return true;
		} else if (w == chckbx_ce) {
			item = GetSelectedItem();
			if (!item)
				return true;
			item.ceSpawn = chckbx_ce.IsChecked();
			Save();
			return true;
		} else if (w == combo_spawn_in) {
			item = GetSelectedItem();
			if (!item)
				return true;
			item.spawnIn = combo_spawn_in.GetCurrentItem();
			Save();
			return true;
		} else if (w == combo_from) {
			LoadPresets();
			bool hasPerm = LBAdmins.Get().HasPermission("admin.change.itempresets");
			UpdateSaveButtonVisible();
			panel_add_remove.Show(hasPerm);
			panel_item_config.Show(hasPerm);
			btn_add_preset.Show(hasPerm);
			btn_add_preset_from_char.Show(hasPerm);
			btn_remove_preset.Show(hasPerm);
			btn_rename.Show(hasPerm);
			LBAdminMenuSettings.Get.presetSpawnerCategory = combo_from.GetCurrentItem();
			LBAdminMenuSettings.Loader.Save();
			return true;
		} else if (w == combo_health) {
			UpdateHealthFromCombo();
			return true;
		} else if (w == combo_state) {
			item = GetSelectedItem();
			if (!item)
				return true;
			if (LBAdminMenuFrame_ItemSpawner.HasFoodStage(item.itemname)) {
				item.foodStage = GetSelectedFoodStage();
				Save();
			} else if (LBAdminMenuFrame_ItemSpawner.HasLiquid(item.itemname)) {
				item.foodStage = GetSelectedLiquidType();
				Save();
			} else {
				LBLogger.Error("Pressed on combo_state, but item did not have foodstage or liquid", "AdminTools");
			}
		} else if (w == combo_spawn_in_spawn) {
			LBAdminMenuSettings.Get.itemSpawnerDefaultSpawn = combo_spawn_in_spawn.GetCurrentItem();
			return true;
		} else if (w == btn_save_server) {
			SavePresetToServer();
			return true;
		}
		return false;
	}

	void UpdateSaveButtonVisible() {
		bool hasPerm = LBAdmins.Get().HasPermission("admin.change.itempresets");
		if (combo_from.GetCurrentItem() == 0) {
			btn_save_server.Show(false);
			hasPerm = true;
		} else if (combo_from.GetCurrentItem() == 1) {
			btn_save_server.Show(hasPerm);
		}
	}

	void DeleteSelectedPreset() {
		int index;
		if (!LBWidgetUtils.CheckSelectedInRange(list_presets, index))
			return;
		LBAdminItemPresetItem preset;
		list_presets.GetItemData(index, 0, preset);
		GetItemPresets().RemoveItem(preset);
		list_presets.RemoveRow(index);
		Save();
		if (combo_from.GetCurrentItem() == 1) {
			g_Game.RPCSingleParam(null, LBAdminToolRPCs.PRESET_DELETE, new Param1<string>(preset.itemname), true);
		}
		if (list_presets.GetSelectedRow() >= list_presets.GetNumItems())
			list_presets.SelectRow(list_presets.GetNumItems() - 1);
	}

	override void Update(float timeslice, bool otherMenu, bool leftClicked) {
		super.Update(timeslice, otherMenu, leftClicked);
		if (hasPrio) {
			if (GetUApi().GetInputByName("UALBMADMSpawnerGround").LocalPress()) {
				RequestSpawn(LBItemSpawnerLocation.GROUND);
				GetUApi().SupressNextFrame(true);
			} else if (GetUApi().GetInputByName("UALBMADMSpawnerInventory").LocalPress()) {
				RequestSpawn(LBItemSpawnerLocation.INVENTORY);
				GetUApi().SupressNextFrame(true);
			} else if (GetUApi().GetInputByName("UALBMADMSpawnerCursor").LocalPress()) {
				RequestSpawn(LBItemSpawnerLocation.CURSOR);
				GetUApi().SupressNextFrame(true);
			} else if (GetUApi().GetInputByName("UALBMADMSpawnerTarget").LocalPress()) {
				RequestSpawn(LBItemSpawnerLocation.TARGET);
				GetUApi().SupressNextFrame(true);
			} else if (g_Game.LBIsKeyPressed(KeyCode.KC_DELETE)) {
				RemoveItemFromPreset();
			} else if (g_Game.LBIsKeyDown(KeyCode.KC_LCONTROL) && g_Game.LBIsKeyPressed(KeyCode.KC_D)) {
				LBAdminItemPresetItem itemParent = GetSelectedItemParent();
				LBAdminItemPresetItem selected = GetSelectedItem();
				if (itemParent && selected) {
					if (g_Game.LBIsKeyDown(KeyCode.KC_LSHIFT)) {
						itemParent.children.Insert(selected.Copy(true));
					} else {
						itemParent.children.Insert(selected.Copy(false));
					}
					LoadSelectedPreset(selected);
				}
			}
		}
		if (IsVisible() && LBAdminItemPresetConfigServer_.wasEdited && combo_from.GetCurrentItem() == 1) {
			LBAdminItemPresetConfigServer_.wasEdited = false;
			int oldPreset, oldItem;
			if (LBWidgetUtils.CheckSelectedInRange(list_presets, oldPreset) && LBWidgetUtils.CheckSelectedInRange(list_items, oldItem)) {
				LBAdminItemPresetItem preset;
				list_presets.GetItemData(oldPreset, 0, preset);
				string oldPresetName = preset.itemname;
				LoadPresets();
				int index = LBAdminItemPresetConfigServer.Get.GetPresetIndex(oldPresetName);
				if (index >= 0) {
					list_presets.SelectRow(index);
					list_items.SelectRow(oldItem);
				}
			} else {
				LoadPresets();
				list_presets.SelectRow(0);
			}

		}
	}

	void RequestDefaultSpawn() {
		RequestSpawn(LBAdminMenuSettings.Get.itemSpawnerDefaultSpawn);
	}

	void RequestSpawn(LBItemSpawnerLocation type) {
		if (type == LBItemSpawnerLocation.TARGET || type == LBItemSpawnerLocation.SELECTED_PLAYERS) {
			if (!LBAdmins.Get().HasPermission("admin.spawn.preset_others"))
				return;
		}
		LBAdminItemPresetItem item = GetSelectedPreset();
		if (GetWidgetUnderCursor() == list_items) {
			LBAdminItemPresetItem selected = GetSelectedItem();
			if (selected && selected != item) {
				LBAdminItemPresetItem temp = new LBAdminItemPresetItem();
				temp.itemname = item.itemname + " Subpreset " + selected.itemname;
				temp.children.Insert(selected);
				item = temp;
			}
		}
		if (!item)
			return;
		vector pos = vector.Zero;
		Object obj;
		array<PlayerIdentity> selectedPlayers = null;
		if (type == 1) {
			pos = g_Game.GetPlayer().GetPosition();
		} else if (type == LBItemSpawnerLocation.CURSOR || type == LBItemSpawnerLocation.TARGET) {
			if (!LBAdminMenuFrame_ItemSpawner.GetCursorLocation(pos, obj))
				return;
		} else if (type == LBItemSpawnerLocation.SELECTED_PLAYERS) {
			LBAdminMenuFrame_PlayerList list = LBAdminMenuFrame_PlayerList.Cast(parent.GetFrame("LBAdminMenuFrame_PlayerList"));
			if (!list.selectedPlayers || list.selectedPlayers.Count() <= 0) {
				return;
			}
			selectedPlayers = list.selectedPlayers;
		}
		g_Game.RPCSingleParam(null, LBAdminToolRPCs.PRESET_SPAWN, new Param5<ref LBAdminItemPresetItem, vector, LBItemSpawnerLocation, Object, ref array<PlayerIdentity>>(item, pos, type, obj, selectedPlayers), true);
	}

	void OnRenamePreset(string name) {
		if (name == "")
			return;
		int index;
		if (!LBWidgetUtils.CheckSelectedInRange(list_presets, index))
			return;
		LBAdminItemPresetItem preset;
		list_presets.GetItemData(index, 0, preset);
		string oldName = preset.itemname;
		preset.itemname = name;
		list_presets.SetItem(index, name, preset, 0);
		Save();
		if (combo_from.GetCurrentItem() == 1)
			g_Game.RPCSingleParam(null, LBAdminToolRPCs.PRESET_RENAME, new Param2<string, string>(oldName, name), true);
	}

	void OnCreateNewPreset(string name) {
		if (name == "")
			return;
		LBAdminItemPresetItem preset = new LBAdminItemPresetItem();
		preset.itemname = name;
		GetItemPresets().Insert(preset);
		list_presets.AddItem(name, preset, 0);
		Save();
		if (combo_from.GetCurrentItem() == 1)
			g_Game.RPCSingleParam(null, LBAdminToolRPCs.PRESET_CREATE, new Param1<string>(name), true);
		list_presets.SelectRow(list_presets.GetNumItems() - 1);
	}

	void OnCreateNewPresetFromChar(string name) {
		if (name == "")
			return;
		LBAdminItemPresetItem preset = LBAdminItemPresetItem.FromItem(g_Game.GetPlayer(), 0);
		preset.itemname = name;
		GetItemPresets().Insert(preset);
		list_presets.AddItem(name, preset, 0);
		Save();
		if (combo_from.GetCurrentItem() == 1)
			g_Game.RPCSingleParam(null, LBAdminToolRPCs.PRESET_CREATE, new Param1<string>(name), true);
		list_presets.SelectRow(list_presets.GetNumItems() - 1);
	}

	LBAdminItemPresetItem GetSelectedPreset() {
		int index;
		if (!LBWidgetUtils.CheckSelectedInRange(list_presets, index))
			return null;
		LBAdminItemPresetItem preset;
		list_presets.GetItemData(index, 0, preset);
		return preset;
	}

	LBAdminItemPresetItem GetSelectedItem() {
		int index;
		if (!LBWidgetUtils.CheckSelectedInRange(list_items, index))
			return null;
		LBAdminItemPresetItem preset;
		list_items.GetItemData(index, 0, preset);
		return preset;
	}

	LBAdminItemPresetItem GetSelectedItemParent() {
		LBAdminItemPresetItem item = GetSelectedItem();
		LBAdminItemPresetItem root = GetSelectedPreset();
		return GetParent(item, root);
	}

	LBAdminItemPresetItem GetParent(LBAdminItemPresetItem search, LBAdminItemPresetItem parentItem) {
		if (!parentItem)
			return null;
		foreach (LBAdminItemPresetItem child : parentItem.children) {
			if (child == search)
				return parentItem;
			LBAdminItemPresetItem sup = GetParent(search, child);
			if (sup)
				return sup;
		}
		return null;
	}

	void LoadSelectedPreset(LBAdminItemPresetItem selected = null) {
		LBAdminItemPresetItem preset = GetSelectedPreset();
		list_items.ClearItems();
		if (!preset)
			return;
		AddPresetItems(preset, selected);
		LoadSelectedItem();
	}

	void AddItemToPreset(string itemname) {
		LBAdminItemPresetItem preset = GetSelectedItem();
		if (!preset)
			return;
		LBAdminItemPresetItem newItem = new LBAdminItemPresetItem();
		newItem.itemname = itemname;
		preset.children.Insert(newItem);
		LoadSelectedPreset(preset);
		Save();
	}

	void RemoveItemFromPreset() {
		LBAdminItemPresetItem preset = GetSelectedPreset();
		if (!preset)
			return;
		int index;
		if (!LBWidgetUtils.CheckSelectedInRange(list_items, index))
			return;
		LBAdminItemPresetItem item;
		list_items.GetItemData(index, 0, item);
		if (item == preset)
			return;
		int oldRow = list_items.GetSelectedRow();
		DeletePreset(preset, item);
		LoadSelectedPreset();
		if (oldRow >= list_items.GetNumItems())
			list_items.SelectRow(list_items.GetNumItems() - 1);
		else
			list_items.SelectRow(oldRow);
		Save();
	}

	bool DeletePreset(LBAdminItemPresetItem parentPreset, LBAdminItemPresetItem item) {
		int index = parentPreset.children.Find(item);
		if (index != -1) {
			parentPreset.children.RemoveOrdered(index);
			return true;
		}
		foreach (LBAdminItemPresetItem child : parentPreset.children) {
			if (DeletePreset(child, item))
				return true;
		}
		return false;
	}

	void LoadSelectedItem() {
		LBAdminItemPresetItem preset = GetSelectedItem();
		if (!preset)
			return;
		combo_spawn_in.SetCurrentItem(preset.spawnIn);
		slider_health.SetCurrent(preset.health * 100);
		combo_health.SetCurrentItem(LBAdminMenuFrame_ItemSpawner.GetHealthIndex(preset.health));
		panel_health_color.SetColor(LBAdminMenuFrame_ItemSpawner.GetHealthColor(preset.health) | 0xFF000000);
		edit_health.SetText("" + ((int)(preset.health * 100)) + "%");
		chckbx_debug.SetChecked(preset.debugSpawn);
		chckbx_ce.SetChecked(preset.ceSpawn);
		MarkItemEntries();
		SetupItemSettings(preset);
	}

	void AddPresetItems(LBAdminItemPresetItem preset, LBAdminItemPresetItem selected, string level = "", bool last = false) {
		if (level == "")
			list_items.AddItem("ROOT", preset, 0);
		else if (!last)
			list_items.AddItem(level + preset.itemname, preset, 0);
		else
			list_items.AddItem(level + preset.itemname, preset, 0);
		if (preset == selected)
			list_items.SelectRow(list_items.GetNumItems() - 1);
		for (int i = 0; i < preset.children.Count(); ++i) {
			LBAdminItemPresetItem child = preset.children.Get(i);
			AddPresetItems(child, selected, level + "  ", i == preset.children.Count() - 1);
		}
	}

	void MarkItemEntries() {
		LBAdminItemPresetItem selected = GetSelectedItem();
		LBAdminItemPresetItem parentItem = GetSelectedItemParent();
		for (int i = 0; i < list_items.GetNumItems(); ++i) {
			LBAdminItemPresetItem entry;
			list_items.GetItemData(i, 0, entry);
			if (entry == parentItem) {
				list_items.SetItemColor(i, 0, ARGB(255, 255, 0, 0));
			} else if (selected.children.Find(entry) != -1) {
				list_items.SetItemColor(i, 0, ARGB(255, 0, 255, 0));
			} else {
				list_items.SetItemColor(i, 0, ARGB(255, 255, 255, 255));
			}
		}
	}

	override bool OnChange(Widget w, int x, int y, bool finished) {
		if (super.OnChange(w, x, y, finished))
			return true;
		if (w == edit_search) {
			LoadPresets();
			return true;
		} else if (w == slider_health) {
			UpdateHealthFromSlider();
			return true;
		} else if (w == edit_health) {
			UpdateHealthFromInput();
			return true;
		} else if (w == edit_quantity) {
			LBAdminItemPresetItem preset = GetSelectedItem();
			if (!preset)
				return true;
			float max = LBAdminMenuFrame_ItemSpawner.GetMaxQuantity(preset.itemname);
			if (max > 0) {
				float current = edit_quantity.GetText().ToFloat();
				float percent = current / max;
				preset.quantity = percent;
				Save();
				slider_quantity.SetCurrent(percent);
				if (edit_quantity.GetText() != "")
					edit_quantity.SetText("" + ((int) current));
			}
			SetPrio();
			return true;
		} else if (w == slider_quantity) {
			preset = GetSelectedItem();
			if (!preset)
				return true;
			current = slider_quantity.GetCurrent() * LBAdminMenuFrame_ItemSpawner.GetMaxQuantity(preset.itemname);
			edit_quantity.SetText("" + ((int) current));
			preset.quantity = slider_quantity.GetCurrent();
			Save();
			SetPrio();
			return true;
		}
		return false;
	}

	override bool OnItemSelected(Widget w, int x, int y, int row, int column, int oldRow, int oldColumn) {
		if (super.OnItemSelected(w, x, y, row, column, oldRow, oldColumn))
			return true;
		if (w == list_presets) {
			LoadSelectedPreset();
			return true;
		} else if (w == list_items) {
			LoadSelectedItem();
			return true;
		}
		return false;
	}

	void SavePresetHealth() {
		LBAdminItemPresetItem preset = GetSelectedItem();
		if (!preset)
			return;
		preset.health = slider_health.GetCurrent() / 100.0;
		Save();
	}

	void UpdateHealthFromSlider() {
		float health = slider_health.GetCurrent();
		int hp = health;
		edit_health.SetText("" + hp + "%");
		combo_health.SetCurrentItem(LBAdminMenuFrame_ItemSpawner.GetHealthIndex(health / 100.0));
		panel_health_color.SetColor(LBAdminMenuFrame_ItemSpawner.GetHealthColor(health / 100.0) | 0xFF000000);
		SavePresetHealth();
	}

	void UpdateHealthFromInput() {
		int health = edit_health.GetText().ToInt();
		if (health < 0) {
			health = 0;
			edit_health.SetText("0");
		} else if (health > 100) {
			health = 100;
			edit_health.SetText("100");
		}
		slider_health.SetCurrent(health);
		combo_health.SetCurrentItem(LBAdminMenuFrame_ItemSpawner.GetHealthIndex(health / 100.0));
		panel_health_color.SetColor(LBAdminMenuFrame_ItemSpawner.GetHealthColor(health / 100.0) | 0xFF000000);
		SavePresetHealth();
	}

	void UpdateHealthFromCombo() {
		int index = combo_health.GetCurrentItem();
		int hp = LBAdminMenuFrame_ItemSpawner.GetHealthFromIndex(index);
		edit_health.SetText("" + hp + "%");
		slider_health.SetCurrent(hp);
		panel_health_color.SetColor(LBAdminMenuFrame_ItemSpawner.GetHealthColor(hp / 100.0) | 0xFF000000);
		SavePresetHealth();
	}

	override bool OnMouseEnter(Widget w, int x, int y) {
		if (super.OnMouseEnter(w, x, y))
			return true;
		if (w == edit_health) {
			string text = edit_health.GetText();
			if (LBStringTools.EndsWith(text, "%")) {
				text = text.Substring(0, text.Length() - 1);
				edit_health.SetText(text);
			}
			SetFocus(edit_health);
			return true;
		}
		return false;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y) {
		if (super.OnMouseLeave(w, enterW, x, y))
			return true;
		if (w == edit_health) {
			if (edit_health.GetText() == "")
				edit_health.SetText("0%");
			else
				edit_health.SetText(edit_health.GetText() + "%");
		}
		return false;
	}

	int GetSelectedLiquidType() {
		int index = combo_state.GetCurrentItem();
		if (index < 0 || index > availableLiquids.Count() || !setLiquid)
			return -1;
		return availableLiquids.Get(index);
	}

	void SetupItemSettings(LBAdminItemPresetItem preset) {
		string itemname = preset.itemname;
		if (LBAdminMenuFrame_ItemSpawner.HasQuantity(itemname)) {
			edit_quantity.Show(true);
			slider_quantity.Show(true);
			int current = LBAdminMenuFrame_ItemSpawner.GetMaxQuantity(itemname) * preset.quantity;
			edit_quantity.SetText("" + current);
			slider_quantity.SetCurrent(preset.quantity);
		} else {
			edit_quantity.Show(false);
			slider_quantity.Show(false);
		}
		if (LBAdminMenuFrame_ItemSpawner.HasFoodStage(itemname)) {
			combo_state.Show(true);
			combo_state.ClearAll();
			int selected = 0;
			int index = 0;
			for (int i = 1; i < FoodStageType.COUNT; ++i) {
				string name = FoodStage.GetFoodStageName(i);
				if (g_Game.ConfigIsExisting("CfgVehicles " + itemname + " Food FoodStages " + name)) {
					if (i == preset.foodStage)
						selected = index;
					++index;
					combo_state.AddItem(name);
				}
			}
			combo_state.SetCurrentItem(selected);
		} else if (LBAdminMenuFrame_ItemSpawner.HasLiquid(itemname)) {
			int initLiquid = g_Game.ConfigGetInt("CfgVehicles " + itemname + " varLiquidTypeInit");
			if (preset.foodStage != 0)
				initLiquid = preset.foodStage;
			int mask = LBAdminMenuFrame_ItemSpawner.GetLiquidContainerType(itemname);
			combo_state.Show(true);
			combo_state.ClearAll();
			availableLiquids.Clear();
			setLiquid = false;
			for (i = 0; i < 32; ++i) {
				int type = 1 << i;
				if ((mask & type) != 0) {
					LiquidInfo info = Liquid.m_LiquidInfosByType.Get(type);
					if (info) {
						name = Widget.TranslateString(info.m_LiquidDisplayName);
						if (name.IndexOf("$UNT$") != -1)
							name.Replace("$UNT$", "");
						combo_state.AddItem(name);
						availableLiquids.Insert(type);
						if (type == initLiquid) {
							combo_state.SetCurrentItem(combo_state.GetNumItems() - 1);
							setLiquid = true;
						}
					}
				}
			}
		} else {
			combo_state.Show(false);
		}
	}

	int GetSelectedFoodStage() {
		LBAdminItemPresetItem preset = GetSelectedItem();
		if (!preset)
			return 0;
		int current = combo_state.GetCurrentItem();
		int index = 0;
		for (int i = 1; i < FoodStageType.COUNT; ++i) {
			string name = FoodStage.GetFoodStageName(i);
			if (g_Game.ConfigIsExisting("CfgVehicles " + preset.itemname + " Food FoodStages " + name)) {
				if (current == index) {
					return i;
				}
				++index;
			}
		}
		return 0;
	}
}
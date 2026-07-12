class LBAdminMenuFrame_ItemSpawner : LBAdminMenuFrame {

	override string GetTitle() {
		return "#lb_adm_title_item_spawner";
	}

	override string GetButtonIcon() {
		return "LBmaster_AdminTools/gui/icons/itemspawner.paa";
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
		return {"admin.spawn.item_self", "admin.spawn.preset"};
	}

	override string GetInfoString() {
		return LBTranslatedString("#lb_adm_desc_item_spawner", {"%ground_keybind%", LBInputUtils.GetInputKeybindOut("UALBMADMSpawnerGround"), "%inventory_keybind%", LBInputUtils.GetInputKeybindOut("UALBMADMSpawnerInventory"), "%cursor_keybind%", LBInputUtils.GetInputKeybindOut("UALBMADMSpawnerCursor")}).Get();
	}

	XComboBoxWidget combo_category, combo_state, combo_health, combo_temp, combo_spawn_in, combo_export;
	SliderWidget slider_quantity, slider_health, slider_temp;
	CheckBoxWidget chckbx_spawnable, chckbx_enable_preview, chckbx_debug, chckbx_ce;
	ItemPreviewWidget item_preview;
	EditBoxWidget edit_search, edit_quantity, edit_health, edit_temp;
	TextListboxWidget list_items;
	ButtonWidget btn_select, btn_show_children, btn_find_attachments, btn_export;
	Widget panel_state_color, panel_health_color, panel_temp_color;

	Object previewItem = null;
	ref TStringArray currentParent = new TStringArray();
	ref TStringArray currentItems = new TStringArray();
	ref array<ref TStringArray> categories;
	ref TIntArray availableLiquids = new TIntArray();
	ref TStringArray categoryNames;
	ref TIntArray temps = new TIntArray();
	string spawnLastItem = "", lastDisplayedItem = "";
	bool setLiquid = false;

	ref LBAdminSpawnRequest requestCache = new LBAdminSpawnRequest();

	static bool needItemCategoryRefresh = false;

	override void OnInit() {
		slider_quantity.Show(false);
		combo_state.Show(false);
		edit_quantity.Show(false);
		LoadSettings();
		SetItemHelper(item_preview, true, true);
		chckbx_spawnable.SetChecked(true);
		InitCategories();
		combo_health.SetCurrentItem(4);
		panel_health_color.SetColor(GetHealthColor(1) | 0xff000000);
	}

	void LoadSettings() {
		categories = new array<ref TStringArray>();
		foreach (TStringArray categoryItem : LBAdminMenuSettings.Get.itemspawnerCategories) {
			TStringArray categoryItems = new TStringArray();
			categories.Insert(categoryItems);
			foreach (string item : categoryItem) {
				categoryItems.Insert(item);
			}
		}
		categoryNames = new TStringArray();
		foreach (string category : LBAdminMenuSettings.Get.itemspawnerCategoryNames)
			categoryNames.Insert(category);

	}

	void FillSpawnerLocationTypes() {
		combo_spawn_in.ClearAll();
		combo_spawn_in.AddItem("#lb_adm_item_spawner_inventory");
		combo_spawn_in.AddItem("#lb_adm_item_spawner_ground");
		combo_spawn_in.AddItem("#lb_adm_item_spawner_cursor");
		if (LBAdmins.Get().HasPermission("admin.spawn.item_others")) {
			combo_spawn_in.AddItem("#lb_adm_item_spawner_target");
			combo_spawn_in.AddItem("#lb_adm_item_spawner_selected_player");
		}
		combo_spawn_in.SetCurrentItem(LBAdminMenuSettings.Get.itemSpawnerDefaultSpawn);
	}

	override void Update(float timeslice, bool otherMenu, bool leftClicked) {
		super.Update(timeslice, otherMenu, leftClicked);
		if (IsMouseOver() && IsVisible()) {
			vector pos;
			Object obj;
			if (GetUApi().GetInputByName("UALBMADMSpawnerGround").LocalPress()) {
				RequestSpawn(LBItemSpawnerLocation.GROUND, g_Game.GetPlayer().GetPosition(), null, null);
				GetUApi().SupressNextFrame(true);
			} else if (GetUApi().GetInputByName("UALBMADMSpawnerInventory").LocalPress()) {
				RequestSpawn(LBItemSpawnerLocation.INVENTORY, g_Game.GetPlayer().GetPosition(), null, null);
				GetUApi().SupressNextFrame(true);
			} else if (GetUApi().GetInputByName("UALBMADMSpawnerCursor").LocalPress()) {
				if (GetCursorLocation(pos, obj)) {
					RequestSpawn(LBItemSpawnerLocation.CURSOR, pos, null, null);
				}
				GetUApi().SupressNextFrame(true);
			} else if (GetUApi().GetInputByName("UALBMADMSpawnerTarget").LocalPress()) {
				if (GetCursorLocation(pos, obj)) {
					RequestSpawn(LBItemSpawnerLocation.TARGET, pos, obj, null);
				}
				GetUApi().SupressNextFrame(true);
			} else if (g_Game.LBIsKeyDown(KeyCode.KC_LCONTROL) && g_Game.LBIsKeyPressed(KeyCode.KC_W)) {
				LBAdminMenuFrame_PresetSpawner presetSpawner = LBAdminMenuFrame_PresetSpawner.Cast(parent.GetFrame("LBAdminMenuFrame_PresetSpawner"));
				string selected = GetSelectedItemname();
				if (presetSpawner && presetSpawner.IsVisible() && selected != "") {
					presetSpawner.AddItemToPreset(selected);
				}
			}
		}
	}

	void RequestSpawnInDefault() {
		vector pos;
		Object obj;
		int loc = LBAdminMenuSettings.Get.itemSpawnerDefaultSpawn;
		if (loc == LBItemSpawnerLocation.INVENTORY) {
			RequestSpawn(LBItemSpawnerLocation.INVENTORY, g_Game.GetPlayer().GetPosition(), null, null);
		} else if (loc == LBItemSpawnerLocation.GROUND) {
			RequestSpawn(LBItemSpawnerLocation.GROUND, g_Game.GetPlayer().GetPosition(), null, null);
		} else if (loc == LBItemSpawnerLocation.CURSOR) {
			if (GetCursorLocation(pos, obj)) {
				RequestSpawn(LBItemSpawnerLocation.CURSOR, pos, null, null);
			}
		} else if (loc == LBItemSpawnerLocation.TARGET) {
			if (GetCursorLocation(pos, obj)) {
				RequestSpawn(LBItemSpawnerLocation.TARGET, pos, obj, null);
			}
		} else if (loc == LBItemSpawnerLocation.SELECTED_PLAYERS) {
			LBAdminMenuFrame_PlayerList list = LBAdminMenuFrame_PlayerList.Cast(parent.GetFrame("LBAdminMenuFrame_PlayerList"));
			if (list.selectedPlayers && list.selectedPlayers.Count() > 0) {
				RequestSpawn(LBItemSpawnerLocation.SELECTED_PLAYERS, vector.Zero, null, list.selectedPlayers);
			}
		}
	}

	void RequestSpawn(int type, vector position, Object target, array<PlayerIdentity> selectedPlayers) {
		if (type == LBItemSpawnerLocation.TARGET || type == LBItemSpawnerLocation.SELECTED_PLAYERS) {
			if (!LBAdmins.Get().HasPermission("admin.spawn.item_others"))
				return;
		}
		string itemname = GetSelectedItemname();
		if (itemname == "")
			return;
		int foodstage = GetSelectedFoodStage(itemname);
		int liquidtype = GetSelectedLiquidType(itemname);
		float quantity = -1;
		if (slider_quantity.IsVisible())
			quantity = edit_quantity.GetText().ToFloat();
		int temp = 0;
		if (HasTemperature(itemname)) {
			float tempMin = GetTemperatureMin(itemname);
			float tempMax = GetTemperatureMax(itemname);
			float tempDiff = tempMax - tempMin;
			temp = slider_temp.GetCurrent() * tempDiff + tempMin;
		}
		requestCache.Load(type, target, itemname, position, quantity, spawnLastItem, foodstage, liquidtype, slider_health.GetCurrent(), chckbx_debug.IsChecked(), chckbx_ce.IsChecked(), temp, selectedPlayers);
		g_Game.RPCSingleParam(null, LBAdminToolRPCs.SPAWN_ITEM_SELF, new Param1<ref LBAdminSpawnRequest>(requestCache), true);
	}

	static bool GetCursorLocation(out vector pos, out Object hitObj) {
		vector camPos = g_Game.GetCurrentCameraPosition();
		vector camDir = g_Game.GetCurrentCameraDirection().Normalized() * 100.0;
		vector hitPos, hitNormal;
		float fraction;
		PhxInteractionLayers layers = PhxInteractionLayers.ITEM_SMALL | PhxInteractionLayers.ITEM_LARGE | PhxInteractionLayers.VEHICLE_NOTERRAIN | PhxInteractionLayers.BUILDING | PhxInteractionLayers.CHARACTER | PhxInteractionLayers.VEHICLE | PhxInteractionLayers.ROADWAY | PhxInteractionLayers.FIREGEOM | PhxInteractionLayers.DOOR | PhxInteractionLayers.WATERLAYER | PhxInteractionLayers.TERRAIN | PhxInteractionLayers.WATERLAYER | PhxInteractionLayers.FENCE | PhxInteractionLayers.AI;
		RaycastRVParams params = new RaycastRVParams(camPos, camPos + camDir, g_Game.GetPlayer());
		array<ref RaycastRVResult> results = new array<ref RaycastRVResult>();
		if (DayZPhysics.RaycastRVProxy(params, results) && results.Count() > 0) {
			pos = results.Get(0).pos;
			hitObj = results.Get(0).obj;
			if (!hitObj)
				hitObj = results.Get(0).parent;
			return true;
		}
		return false;
	}

	override void Show(bool show) {
		super.Show(show);
		if (show) {
			FillSpawnerLocationTypes();
			SetFocus(edit_search);
			if (needItemCategoryRefresh) {
				LoadSettings();
				InitCategories();
				needItemCategoryRefresh = false;
			}
			UpdatePreview();
		}
	}

	override bool SetPrio() {
		if (super.SetPrio()) {
			FillSpawnerLocationTypes();
			SetFocus(edit_search);
			if (needItemCategoryRefresh) {
				needItemCategoryRefresh = false;
				LoadSettings();
				InitCategories();
			}
			return true;
		}
		return false;
	}

	override void OnHide() {
		super.OnHide();
		ResetPreview();
	}

	void InitCategories() {
		combo_category.ClearAll();
		for (int i = 0; i < categoryNames.Count(); ++i) {
			combo_category.AddItem(categoryNames.Get(i));
		}
		combo_category.SetCurrentItem(0);
		OnCategoryChanged();
	}

	override bool OnClick(Widget w, int x, int y, int button) {
		if (super.OnClick(w, x, y, button))
			return true;
		if (w == btn_select) {
			RequestSpawnInDefault();
			return true;
		} else if (w == btn_show_children) {
			string itemname = GetSelectedItemname();
			if (itemname == "")
				return false;
			AddCustomCategory(itemname);
			return true;
		} else if (w == chckbx_enable_preview) {
			UpdatePreview();
			return true;
		} else if (w == combo_category) {
			OnCategoryChanged();
			return true;
		} else if (w == chckbx_spawnable) {
			FillItemList();
			return true;
		} else if (w == btn_find_attachments) {
			itemname = GetSelectedItemname();
			if (itemname == "")
				return false;
			TStringArray attachments = GetAttachments(itemname);
			LBLogger.Verbose("Attachments for " + itemname + ": " + attachments.Count(), "AdminTools");
			if (attachments.Count() == 0)
				return false;
			categories.Insert(attachments);
			categoryNames.Insert("Att: " + itemname);
			combo_category.AddItem("Att: " + itemname);
			combo_category.SetCurrentItem(categories.Count() - 1);
			edit_search.SetText("");
			OnCategoryChanged();
			spawnLastItem = itemname;
		} else if (w == combo_health) {
			UpdateHealthFromCombo();
			return true;
		} else if (w == combo_temp) {
			UpdateTempFromCombo();
			return true;
		} else if (w == combo_state) {
			UpdateStateColor();
			return true;
		} else if (w == combo_spawn_in) {
			LBAdminMenuSettings.Get.itemSpawnerDefaultSpawn = combo_spawn_in.GetCurrentItem();
			return true;
		} else if (w == btn_export) {
			ExportList();
		}
		return false;
	}

	void ExportList() {
		int type = combo_export.GetCurrentItem();
		int count = list_items.GetNumItems();
		string name;
		if (type == 0) {
			string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<types>";
			for (int i = 0; i < count; ++i) {
				list_items.GetItemText(i, 0, name);
				string xmlType = "\n\t<type name=\"" + name + "\">\n\t\t<nominal>0</nominal>\n\t\t<lifetime>14400</lifetime>\n\t\t<restock>0</restock>\n\t\t<min>0</min>\n\t\t<quantmin>-1</quantmin>\n\t\t<quantmax>-1</quantmax>\n\t\t<cost>100</cost>\n\t\t<flags count_in_cargo=\"0\" count_in_hoarder=\"0\" count_in_map=\"1\" count_in_player=\"0\" crafted=\"0\" deloot=\"0\"/>\n\t</type>";
				xml += xmlType;
			}
			xml += "\n</types>";
			g_Game.CopyToClipboard(xml);
			LBCopiedPopup.ShowAtWidget(btn_export);
		} else if (type == 1) {
			string list = "";
			for (i = 0; i < count; ++i) {
				list_items.GetItemText(i, 0, name);
				list += name + "\n";
			}
			g_Game.CopyToClipboard(list);
			LBCopiedPopup.ShowAtWidget(btn_export);
		} else if (type == 2) {
			LBExportExpansionMarketDealer market = new LBExportExpansionMarketDealer(edit_search.GetText());
			for (i = 0; i < count; ++i) {
				list_items.GetItemText(i, 0, name);
				market.AddItem(name);
			}
			JsonSerializer serial = new JsonSerializer();
			string output;
			serial.WriteToString(market, true, output);
			g_Game.CopyToClipboard(output);
			LBCopiedPopup.ShowAtWidget(btn_export);
		} else if (type == 3) {
			string txt = "\t{\n\t\t\"CategoryName\": \"Export\",\n\t\t\"Products\": [\n";
			for (i = 0; i < count; ++i) {
				list_items.GetItemText(i, 0, name);
				if (i + i == count)
					txt += "\t\t\t\"" + name + ",1,-1,1,1000,100\"\n";
				else
					txt += "\t\t\t\"" + name + ",1,-1,1,1000,100\",\n";
			}
			txt += "\t\t]\n\t}";
			g_Game.CopyToClipboard(txt);
			LBCopiedPopup.ShowAtWidget(btn_export);
		}
	}

	void UpdateStateColor() {
		string itemname = GetSelectedItemname();
		if (HasFoodStage(itemname)) {
			int currentState = GetSelectedFoodStage(itemname);
			int stageColor = GetFoodStageColor(currentState);
			panel_state_color.SetColor(stageColor | 0xff000000);
		} else if (HasLiquid(itemname)) {
			panel_state_color.SetColor(Colors.COLOR_LIQUID | 0xff000000);
		} else {
			panel_state_color.SetColor(0);
		}
	}

	override bool OnItemSelected(Widget w, int x, int y, int row, int column,	int oldRow, int oldColumn) {
		if (super.OnItemSelected(w, x, y, row, column, oldRow, oldColumn))
			return true;
		if (w == list_items) {
			UpdatePreview();
			SetPrio();
		}
		return false;
	}

	override bool OnDoubleClick(Widget w, int x, int y, int button) {
		if (super.OnDoubleClick(w, x, y, button))
			return true;
		if (w == list_items) {
			RequestSpawnInDefault();
			return true;
		}
		return false;
	}
	void AddCustomCategory(string itemname) {
		TStringArray temp = new TStringArray();
		temp.Insert(itemname);
		categories.Insert(temp);
		categoryNames.Insert(itemname);
		combo_category.AddItem(itemname);
		combo_category.SetCurrentItem(categories.Count() - 1);
		edit_search.SetText("");
		OnCategoryChanged();
	}

	void ResetPreview() {
		if (previewItem)
			g_Game.ObjectDelete(previewItem);
		previewItem = null;
	}

	void UpdatePreview() {
		float width = 1.0;
		bool show = chckbx_enable_preview.IsChecked();
		if (show) {
			width = 0.6;
		}
		LBGapHandler handler;
		list_items.GetScript(handler);
		if (handler)
			handler.SetSize(width, 1.0);
		else
			list_items.SetSize(width, 1.0);
		item_preview.Show(show);
		string itemname = GetSelectedItemname();
		if (!show || !previewItem || previewItem.GetType() != itemname)
			ResetPreview();
		if (itemname != lastDisplayedItem)
			SetupItemSettings(itemname);
		lastDisplayedItem = itemname;
		if (!show)
			return;
		LBLogger.Info("Trying to spawn preview item: " + itemname + " Shown: " + show, "AdminTools");
		if (!previewItem)
			previewItem = LBWidgetUtils.SpawnAndSetItemPreview(item_preview, itemname);
		UpdateStateColor();
		UpdateFoodStage();
		UpdateHealthFromSlider(true);
	}

	void UpdateHealthFromSlider(bool initialUpdate) {
		float health = slider_health.GetCurrent();
		int hp = health;
		edit_health.SetText("" + hp + "%");
		combo_health.SetCurrentItem(GetHealthIndex(health / 100.0));
		panel_health_color.SetColor(GetHealthColor(health / 100.0) | 0xFF000000);
		if (previewItem && (!initialUpdate || health < 80.0) && ItemBase.Cast(previewItem) && previewItem.ConfigIsExisting("DamageSystem"))
			previewItem.SetHealth01("", "", health / 100.0);
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
		combo_health.SetCurrentItem(GetHealthIndex(health / 100.0));
		panel_health_color.SetColor(GetHealthColor(health / 100.0) | 0xFF000000);
		if (previewItem)
			previewItem.SetHealth01("", "", health / 100.0);
	}

	void UpdateHealthFromCombo() {
		int index = combo_health.GetCurrentItem();
		int hp = GetHealthFromIndex(index);
		edit_health.SetText("" + hp + "%");
		slider_health.SetCurrent(hp);
		panel_health_color.SetColor(GetHealthColor(hp / 100.0) | 0xFF000000);
		if (previewItem)
			previewItem.SetHealth01("", "", hp / 100.0);
	}

	static int GetHealthFromIndex(int index) {
		switch (index) {
			case 0:
				return 0;
			case 1:
				return 30;
			case 2:
				return 50;
			case 3:
				return 70;
		}
		return 100;
	}

	static int GetHealthColor(float health) {
		if (health <= 0)
			return Colors.COLOR_RUINED;
		if (health <= 0.3)
			return Colors.COLOR_BADLY_DAMAGED;
		if (health <= 0.5)
			return Colors.COLOR_DAMAGED;
		if (health <= 0.7)
			return Colors.COLOR_WORN;
		return Colors.COLOR_PRISTINE;
	}

	static int GetHealthStateColor(int state) {
		switch (state) {
			case 0:
				return Colors.COLOR_RUINED;
			case 1:
				return Colors.COLOR_BADLY_DAMAGED;
			case 2:
				return Colors.COLOR_DAMAGED;
			case 3:
				return Colors.COLOR_WORN;

		}
		return Colors.COLOR_PRISTINE;
	}

	static int GetHealthIndex(float health) {
		if (health <= 0)
			return 0;
		if (health <= 0.3)
			return 1;
		if (health <= 0.5)
			return 2;
		if (health <= 0.7)
			return 3;
		return 4;
	}

	FoodStageType GetSelectedFoodStage(string itemname) {
		if (!HasFoodStage(itemname))
			return FoodStageType.NONE;
		int current = combo_state.GetCurrentItem();
		int index = 0;
		for (int i = 1; i < FoodStageType.COUNT; ++i) {
			string name = FoodStage.GetFoodStageName(i);
			if (g_Game.ConfigIsExisting("CfgVehicles " + itemname + " Food FoodStages " + name)) {
				if (current == index) {
					return i;
				}
				++index;
			}
		}
		return FoodStageType.NONE;
	}

	int GetFoodStageColor(FoodStageType stage) {
		switch (stage) {
			case FoodStageType.RAW: {
				return Colors.COLOR_RAW;
				break;
			}
			case FoodStageType.BAKED: {
				return Colors.COLOR_BAKED;
				break;
			}
			case FoodStageType.BOILED: {
				return Colors.COLOR_BOILED;
				break;
			}
			case FoodStageType.DRIED: {
				return Colors.COLOR_DRIED;
				break;
			}
			case FoodStageType.BURNED: {
				return Colors.COLOR_BURNED;
				break;
			}
			case FoodStageType.ROTTEN: {
				return Colors.COLOR_ROTTEN;
				break;
			}
		}
		return 0;
	}

	int GetSelectedLiquidType(string itemname) {
		if (!HasLiquid(itemname) || !setLiquid)
			return -1;
		int index = combo_state.GetCurrentItem();
		if (index < 0 || index > availableLiquids.Count())
			return -1;
		return availableLiquids.Get(index);
	}

	void UpdateFoodStage() {
		Edible_Base edible = Edible_Base.Cast(previewItem);
		if (!edible)
			return;
		int current = combo_state.GetCurrentItem();
		int index = 0;
		for (int i = 1; i < FoodStageType.COUNT; ++i) {
			string name = FoodStage.GetFoodStageName(i);
			if (g_Game.ConfigIsExisting("CfgVehicles " + edible.GetType() + " Food FoodStages " + name)) {
				if (current == index) {
					edible.GetFoodStage().ChangeFoodStage(i);
					edible.GetFoodStage().UpdateVisualsEx(true);
					break;
				}
				++index;
			}
		}
	}

	void SetupItemSettings(string itemname) {
		if (HasQuantity(itemname)) {
			edit_quantity.Show(true);
			slider_quantity.Show(true);
			int maxQuantity = GetMaxQuantity(itemname);
			edit_quantity.SetText("" + maxQuantity);
			slider_quantity.SetCurrent(1.0);
		} else {
			edit_quantity.Show(false);
			slider_quantity.Show(false);
		}
		if (HasFoodStage(itemname)) {
			combo_state.Show(true);
			combo_state.ClearAll();
			for (int i = 1; i < FoodStageType.COUNT; ++i) {
				string name = FoodStage.GetFoodStageName(i);
				if (g_Game.ConfigIsExisting("CfgVehicles " + itemname + " Food FoodStages " + name)) {
					combo_state.AddItem(name);
				}
			}
			combo_state.SetCurrentItem(0);
		} else if (HasLiquid(itemname)) {
			int initLiquid = g_Game.ConfigGetInt("CfgVehicles " + itemname + " varLiquidTypeInit");
			int mask = GetLiquidContainerType(itemname);
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
		if (HasTemperature(itemname)) {
			slider_temp.Show(true);
			edit_temp.Show(true);
			combo_temp.Show(true);
			float min = GetTemperatureMin(itemname);
			float max = GetTemperatureMax(itemname);
			float diff = max - min;
			combo_temp.ClearAll();
			temps.Clear();
			if (min <= GameConstants.STATE_COLD_LVL_FOUR && max >= GameConstants.STATE_COLD_LVL_FOUR) {
				temps.Insert(GameConstants.STATE_COLD_LVL_FOUR);
				combo_temp.AddItem("#inv_inspect_cold_lvl_four");
			}
			if (min <= GameConstants.STATE_COLD_LVL_THREE && max >= GameConstants.STATE_COLD_LVL_THREE) {
				temps.Insert(GameConstants.STATE_COLD_LVL_THREE);
				combo_temp.AddItem("#inv_inspect_cold_lvl_three");
			}
			if (min <= GameConstants.STATE_COLD_LVL_TWO && max >= GameConstants.STATE_COLD_LVL_TWO) {
				temps.Insert(GameConstants.STATE_COLD_LVL_TWO);
				combo_temp.AddItem("#inv_inspect_cold_lvl_two");
			}
			if (min <= GameConstants.STATE_COLD_LVL_ONE && max >= GameConstants.STATE_COLD_LVL_ONE) {
				temps.Insert(GameConstants.STATE_COLD_LVL_ONE);
				combo_temp.AddItem("#inv_inspect_cold_lvl_one");
			}
			if (min <= GameConstants.STATE_NEUTRAL_TEMP && max >= GameConstants.STATE_NEUTRAL_TEMP) {
				temps.Insert(GameConstants.STATE_NEUTRAL_TEMP);
				combo_temp.AddItem("NEUTRAL");
			}
			if (min <= GameConstants.STATE_HOT_LVL_ONE && max >= GameConstants.STATE_HOT_LVL_ONE) {
				temps.Insert(GameConstants.STATE_HOT_LVL_ONE);
				combo_temp.AddItem("#inv_inspect_hot_lvl_one");
			}
			if (min <= GameConstants.STATE_HOT_LVL_TWO && max >= GameConstants.STATE_HOT_LVL_TWO) {
				temps.Insert(GameConstants.STATE_HOT_LVL_TWO);
				combo_temp.AddItem("#inv_inspect_hot_lvl_two");
			}
			if (min <= GameConstants.STATE_HOT_LVL_THREE && max >= GameConstants.STATE_HOT_LVL_THREE) {
				temps.Insert(GameConstants.STATE_HOT_LVL_THREE);
				combo_temp.AddItem("#inv_inspect_hot_lvl_three");
			}
			if (min <= GameConstants.STATE_HOT_LVL_FOUR && max >= GameConstants.STATE_HOT_LVL_FOUR) {
				temps.Insert(GameConstants.STATE_HOT_LVL_FOUR);
				combo_temp.AddItem("#inv_inspect_hot_lvl_four");
			}
			if (min <= GameConstants.STATE_NEUTRAL_TEMP && max >= GameConstants.STATE_NEUTRAL_TEMP) {
				edit_temp.SetText("" + GameConstants.STATE_NEUTRAL_TEMP);
				float at = (GameConstants.STATE_NEUTRAL_TEMP - min) / diff;
				slider_temp.SetCurrent(at);
			} else if (min > GameConstants.STATE_NEUTRAL_TEMP) {
				edit_temp.SetText("" + min);
				slider_temp.SetCurrent(0);
			} else {
				edit_temp.SetText("" + max);
				slider_temp.SetCurrent(1);
			}
			UpdateTempFromSlider();
			UpdateTemperatureColor(itemname);
		} else {
			slider_temp.Show(false);
			edit_temp.Show(false);
			combo_temp.Show(false);
		}
	}

	void UpdateTempFromCombo() {
		int comboIndex = combo_temp.GetCurrentItem();
		if (temps.Count() == 0)
			return;
		string itemname = GetSelectedItemname();
		int temp = temps.Get(comboIndex);
		edit_temp.SetText("" + temp + "°");
		float min = GetTemperatureMin(itemname);
		float max = GetTemperatureMax(itemname);
		float diff = max - min;
		float at = (temp - min) / diff;
		slider_temp.SetCurrent(at);
		UpdateTemperatureColor(itemname);
	}

	void UpdateTempFromEdit() {
		int input = edit_temp.GetText().ToInt();
		string itemname = GetSelectedItemname();
		float min = GetTemperatureMin(itemname);
		float max = GetTemperatureMax(itemname);
		float diff = max - min;
		float at = (input - min) / diff;
		slider_temp.SetCurrent(at);
		SelectTemp(input);
		UpdateTemperatureColor(itemname);
	}

	void UpdateTempFromSlider() {
		string itemname = GetSelectedItemname();
		int temp = UpdateTemperatureColor(itemname);
		edit_temp.SetText("" + temp + "°");
		SelectTemp(temp);
	}

	void SelectTemp(int temp) {
		int a = 0;
		for (int i = 0; i < temps.Count(); ++i) {
			if (temps.Get(i) <= temp)
				a = i;
		}
		combo_temp.SetCurrentItem(a);
	}

	float UpdateTemperatureColor(string itemname) {
		float min = GetTemperatureMin(itemname);
		float max = GetTemperatureMax(itemname);
		float diff = max - min;
		float temp = slider_temp.GetCurrent() * diff + min;
		ObjectTemperatureState tempInfo = ObjectTemperatureState.GetStateData(temp);
		panel_temp_color.SetColor(tempInfo.m_Color | 0xff000000);
		return temp;
	}

	static int GetLiquidContainerType(string itemname) {
		int type = g_Game.ConfigGetType("CfgVehicles " + itemname + " liquidContainerType");
		if (type == CT_INT)
			return g_Game.ConfigGetInt("CfgVehicles " + itemname + " liquidContainerType");
		else if (type == CT_STRING) {
			string str = g_Game.ConfigGetTextOut("CfgVehicles " + itemname + " liquidContainerType");
			bool add = true;
			int current = 0;
			int index = 0;
			TIntArray bracketPrefixes = new TIntArray();
			while (true) {
				int plus = str.IndexOfFrom(index, "+");
				int minus = str.IndexOfFrom(index, "-");
				int start = index;
				if (plus == -1 && minus == -1)
					index = str.Length();
				else if (plus == -1 || minus < plus)
					index = minus;
				else if (minus == -1 || plus < minus)
					index = plus;
				if (index - start <= 0)
					break;
				string sub = str.Substring(start, index - start);
				sub = sub.Trim();
				if (sub.IndexOf("(") != -1) {
					sub.Replace("(", "");
					bracketPrefixes.Insert(add);
					add = true;
				}
				int num = sub.ToInt();
				bool realAdd = add;
				foreach (int addMul : bracketPrefixes)
					if (addMul == 0)
						realAdd = !realAdd;
				if (realAdd)
					current += num;
				else
					current -= num;
				if (index == minus)
					add = false;
				else
					add = true;

				if (sub.IndexOf(")") != -1) {
					sub.Replace(")", "");
					bracketPrefixes.Remove(bracketPrefixes.Count() - 1);
				}
				++index;
				if (index >= str.Length() || index == 0)
					break;
			}
			return current;
		} else {
			LBLogger.Error("Could not get Liquid container type of " + itemname, "AdminTools");
		}
		return 0;
	}

	static bool HasFoodStage(string itemname) {
		return g_Game.ConfigIsExisting("CfgVehicles " + itemname + " Food FoodStages");
	}

	static bool HasLiquid(string itemname) {
		return g_Game.ConfigIsExisting("CfgVehicles " + itemname + " liquidContainerType");
	}

	static bool HasQuantity(string itemname) {
		return (GetMaxQuantity(itemname) - GetMinQuantity(itemname)) != 0;
	}

	static float GetMaxQuantity(string itemname) {
		if (g_Game.ConfigIsExisting("CfgMagazines " + itemname)) {
			return g_Game.ConfigGetInt("CfgMagazines " + itemname + " count");
		} else if (g_Game.ConfigIsExisting("CfgVehicles " + itemname)) {
			int quantMax = g_Game.ConfigGetInt("CfgVehicles " + itemname + " varQuantityMax");
			if (g_Game.ConfigIsExisting("CfgVehicles " + itemname + " varStackMax"))
				quantMax = g_Game.ConfigGetInt("CfgVehicles " + itemname + " varStackMax");
			return quantMax;
		}
		return 0;
	}

	static float GetMinQuantity(string itemname) {
		if (g_Game.ConfigIsExisting("CfgMagazines " + itemname)) {
			return 0;
		} else if (g_Game.ConfigIsExisting("CfgVehicles " + itemname)) {
			return g_Game.ConfigGetInt("CfgVehicles " + itemname + " varQuantityMin");
		}
		return 0;
	}

	static float GetTemperatureMin(string itemname) {
		if (g_Game.ConfigIsExisting("CfgVehicles " + itemname + " varTemperatureMin"))
			return g_Game.ConfigGetFloat("CfgVehicles " + itemname + " varTemperatureMin");
		return 0.0;
	}

	static float GetTemperatureMax(string itemname) {
		if (g_Game.ConfigIsExisting("CfgVehicles " + itemname + " varTemperatureMax"))
			return g_Game.ConfigGetFloat("CfgVehicles " + itemname + " varTemperatureMax");
		return 0.0;
	}

	static bool HasTemperature(string itemname) {
		return GetTemperatureMin(itemname) < GetTemperatureMax(itemname);
	}

	void OnCategoryChanged() {
		currentParent = categories.Get(combo_category.GetCurrentItem());
		FillItemList();
		spawnLastItem = "";
		SetFocus(edit_search);
	}

	void FillItemList() {
		list_items.ClearItems();
		int scope = -1;
		if (chckbx_spawnable.IsChecked())
			scope = 2;
		currentItems.Clear();
		TStringSet found = LBInherit.Get().GetAllChildren(currentParent, false, true, true, scope);
		string search = edit_search.GetText();
		search.ToLower();
		foreach (string item : found) {
			if (IsSearched(item, search)) {
				list_items.AddItem(item, null, 0);
				currentItems.Insert(item);
			}
		}
		LBLogger.Verbose("ItemList of " + currentParent + " Scope: " + scope + " Items: " + currentItems.Count(), "AdminTools");
		list_items.SelectRow(0);
	}

	TStringArray GetAttachments(string item) {
		TStringArray output = new TStringArray();
		TStringArray attNames = new TStringArray();
		LBInheritEntry entry = LBInherit.Get().GetEntry(item);
		if (!entry)
			return output;
		g_Game.ConfigGetTextArray(entry.category + " " + item + " attachments", attNames);
		foreach (string att : attNames) {
			output.InsertArray(LBInherit.Get().GetItemsIntoSlot(att));
		}
		g_Game.ConfigGetTextArray(entry.category + " " + item + " magazines", attNames);
		foreach (string att2 : attNames) {
			output.Insert(att2);
		}
		return output;
	}

	static bool IsSearched(string itemname, string searchStr) {
		if (searchStr == "")
			return true;
		itemname.ToLower();
		return itemname.IndexOf(searchStr) != -1;
	}

	string GetSelectedItemname() {
		int row = list_items.GetSelectedRow();
		if (row < 0 || row >= currentItems.Count())
			return "";
		return currentItems.Get(row);
	}

	override bool OnChange(Widget w, int x, int y, bool finished) {
		if (super.OnChange(w, x, y, finished))
			return true;
		if (w == edit_search) {
			FillItemList();
			SetPrio();
			return true;
		} else if (w == edit_quantity) {
			string itemname = GetSelectedItemname();
			float max = GetMaxQuantity(itemname);
			if (max > 0) {
				float current = edit_quantity.GetText().ToFloat();
				float percent = current / max;
				slider_quantity.SetCurrent(percent);
				if (edit_quantity.GetText() != "")
					edit_quantity.SetText("" + ((int) current));
			}
			SetPrio();
			return true;
		} else if (w == slider_quantity) {
			itemname = GetSelectedItemname();
			current = slider_quantity.GetCurrent() * GetMaxQuantity(itemname);
			edit_quantity.SetText("" + ((int) current));
			SetPrio();
			return true;
		} else if (w == combo_state) {
			UpdateFoodStage();
			SetPrio();
			return true;
		} else if (w == slider_health) {
			UpdateHealthFromSlider(false);
			SetPrio();
			return true;
		} else if (w == edit_health) {
			UpdateHealthFromInput();
			SetPrio();
			return true;
		} else if (w == slider_temp) {
			UpdateTempFromSlider();
			return true;
		} else if (w == edit_temp) {
			UpdateTempFromEdit();
			return true;
		}
		return false;
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
		} else if (w == edit_temp) {
			text = edit_temp.GetText();
			if (LBStringTools.EndsWith(text, "°")) {
				text = text.Substring(0, text.Length() - 1);
				edit_temp.SetText(text);
			}
			SetFocus(edit_temp);
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
		} else if (w == edit_temp) {
			if (edit_temp.GetText() == "") {
				int min = GetTemperatureMin(GetSelectedItemname());
				edit_temp.SetText("" + min + "°");
			} else
				edit_temp.SetText(edit_temp.GetText() + "°");
		}
		return false;
	}

}

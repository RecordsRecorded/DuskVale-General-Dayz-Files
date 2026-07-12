class LBAdminMenuFrame_ConstructionEditor : LBAdminMenuFrame {

	override string GetTitle() {
		return "#lb_adm_title_construction_edit";
	}

	override string GetButtonIcon() {
		return "LBmaster_AdminTools/gui/icons/construction.paa";
	}

	override void GetMinSize(out int width, out int height) {
		width = 300;
		height = 60;
	}

	override void GetInitialSize(out int width, out int height) {
		width = 600;
		height = 200;
	}

	override TStringArray GetRequiredPermissionsOne() {
		return {"admin.use.constructioneditor"};
	}

	override string GetInfoString() {
		return "#lb_adm_desc_construction_edit";
	}

	WrapSpacerWidget parts_list;
	TextListboxWidget list_presets;
	ButtonWidget btn_apply, btn_save_preset, btn_delete, btn_fav, btn_select_item;
	ItemPreviewWidget item_preview;

	ref map<string, CheckBoxWidget> builtParts = new map<string, CheckBoxWidget>();

	BaseBuildingBase itemCopy, original;
	bool manuallySelectedItem = false;

	vector lastCameraPos, lastCameraDir;

	override void OnInit() {
		super.OnInit();
		SetItemHelper(item_preview, true, true);
	}

	override void Show(bool show) {
		super.Show(show);
		if (show)
			LoadItemInSight();
		else {
			if (itemCopy && g_Game)
				g_Game.ObjectDelete(itemCopy);
			manuallySelectedItem = false;
			original = null;
		}
	}

	override void OnHide() {
		super.OnHide();
		if (itemCopy && g_Game)
			g_Game.ObjectDelete(itemCopy);
		manuallySelectedItem = false;
		original = null;
	}

	void LoadItemInSight() {
		if (manuallySelectedItem)
			return;
		lastCameraPos = g_Game.GetCurrentCameraPosition();
		lastCameraDir = g_Game.GetCurrentCameraDirection();
		vector to = lastCameraDir.Normalized() * 100;
		LBWidgetUtils.DeleteAllChildren(parts_list);
		builtParts.Clear();
		RaycastRVParams rayInput = new RaycastRVParams(lastCameraPos, lastCameraPos + to, g_Game.GetPlayer(), 0.4);
		rayInput.flags = CollisionFlags.FIRSTCONTACT;
		//rayInput.sorted = true;
		array<ref RaycastRVResult> results = new array<ref RaycastRVResult>;

		if (DayZPhysics.RaycastRVProxy(rayInput, results) && results.Count() > 0 && results.Get(0).obj) {
			original = BaseBuildingBase.Cast(results.Get(0).obj);
			if (!original) {
				list_presets.ClearItems();
				return;
			}
			Object obj = null;
			if (!itemCopy || itemCopy.GetType() != original.GetType()) {
				if (itemCopy)
					g_Game.ObjectDelete(itemCopy);
				obj = g_Game.CreateObjectEx(original.GetType(), vector.Zero, ECE_LOCAL);
				itemCopy = BaseBuildingBase.Cast(obj);
			}
			if (!itemCopy) {
				if (obj)
					g_Game.ObjectDelete(obj);
				list_presets.ClearItems();
				return;
			}
			item_preview.SetItem(itemCopy);
			itemCopy.m_HasBase = original.m_HasBase;
			itemCopy.m_SyncParts01 = original.m_SyncParts01;
			itemCopy.m_SyncParts02 = original.m_SyncParts02;
			itemCopy.m_SyncParts03 = original.m_SyncParts03;
			itemCopy.OnSynchronizedClientLB();
			Construction constr = itemCopy.GetConstruction();
			map<string, ref ConstructionPart> parts = constr.GetConstructionParts();
			TStringArray partNames = new TStringArray();
			foreach (string part_name2, ConstructionPart part2 : parts) {
				partNames.Insert(part_name2);
			}
			partNames.Sort();
			foreach (string part_name : partNames) {
				ConstructionPart part = parts.Get(part_name);
				Widget root = LBLayoutManager.Get().CreateLayout("ConstructionEditorEntry", parts_list);
				TextWidget name = TextWidget.Cast(root.FindAnyWidget("name"));
				CheckBoxWidget chckbx = CheckBoxWidget.Cast(root.FindAnyWidget("built"));
				name.SetText(part_name);
				builtParts.Insert(part_name, chckbx);
			}
			UpdatePartsBuilt();
			LoadPresets();
		} else {
			if (itemCopy)
				itemCopy.Delete();
			list_presets.ClearItems();
		}
	}

	override void Update(float timeslice, bool otherMenu, bool leftClicked) {
		super.Update(timeslice, otherMenu, leftClicked);
		vector camPos = g_Game.GetCurrentCameraPosition();
		vector camDir = g_Game.GetCurrentCameraDirection();
		if (IsVisible() && (vector.DistanceSq(lastCameraPos, camPos) > 0.1 || vector.DistanceSq(lastCameraDir, camDir) > 0.01)) {
			LoadItemInSight();
		}
	}

	void LoadPresets() {
		TStringSet presets = LBAdminMenuSettings.Get.GetAvailablePresets(itemCopy.GetType());
		list_presets.ClearItems();
		string favourite = LBAdminMenuSettings.Get.GetFavouriteConstructionConfig(itemCopy.GetType());
		foreach (string preset : presets) {
			int row = list_presets.AddItem(preset, null, 0);
			if (preset == favourite)
				list_presets.SetItemColor(row, 0, ARGB(255, 0, 255, 0));
		}
	}

	void SetNewFavourite() {
		int index;
		if (!LBWidgetUtils.CheckSelectedInRange(list_presets, index) || !itemCopy)
			return;
		string favourite;
		list_presets.GetItemText(index, 0, favourite);
		string current = LBAdminMenuSettings.Get.GetFavouriteConstructionConfig(itemCopy.GetType());
		if (favourite == current) {
			favourite = "";
			LBAdminMenuSettings.Get.SetContructionConfigFavourite(itemCopy.GetType(), "");
		} else {
			LBAdminMenuSettings.Get.SetContructionConfigFavourite(itemCopy.GetType(), favourite);
		}
		LBAdminMenuSettings.Loader.Save();
		int count = list_presets.GetNumItems();
		for (int i = 0; i < count; ++i) {
			string name;
			list_presets.GetItemText(i, 0, name);
			if (name == favourite) {
				list_presets.SetItemColor(i, 0, ARGB(255, 0, 255, 0));
			} else {
				list_presets.SetItemColor(i, 0, -1);
			}
		}
	}

	void UpdatePartsBuilt() {
		if (!itemCopy)
			return;
		Construction constr = itemCopy.GetConstruction();
		int hologram = 0;
		foreach (string part_name, CheckBoxWidget chckbx : builtParts) {
			bool built = constr.IsPartConstructed(part_name);
			bool canBuild = constr.LBCanBuildPart(part_name);
			chckbx.Enable(canBuild);
			chckbx.SetChecked(built);
			if (built)
				hologram = 1;
		}
		if (itemCopy.GetType().IndexOf("BBP_") == 0)
			itemCopy.SetAnimationPhase("Hologram", hologram);
	}

	void SetPartBuilt(string part_name, bool built) {
		if (!itemCopy)
			return;
		Construction constr = itemCopy.GetConstruction();
		ConstructionPart part = constr.GetConstructionPart(part_name);
		if (built) {
			itemCopy.RegisterPartForSync(part.GetId());
			constr.AddToConstructedParts(part_name);
			if (part.IsBase())
				itemCopy.m_HasBase = true;
		} else {
			itemCopy.UnregisterPartForSync(part.GetId());
			constr.RemoveFromConstructedParts(part_name);
			if (part.IsBase())
				itemCopy.m_HasBase = false;
		}
		itemCopy.UpdateVisuals();
	}

	override bool OnClick(Widget w, int x, int y, int button) {
		if (super.OnClick(w, x, y, button))
			return true;
		foreach (string part_name, CheckBoxWidget chckbx : builtParts) {
			if (w == chckbx) {
				SetPartBuilt(part_name, chckbx.IsChecked());
				UpdatePartsBuilt();
				return true;
			}
		}
		if (w == btn_apply && !manuallySelectedItem) {
			ApplyPreset();
			return true;
		} else if (w == btn_save_preset) {
			LBSimpleInputPopup.Get().Show("#lb_save", "#lb_adm_save_as", "", ScriptCaller.Create(SavePopup));
			return true;
		} else if (w == btn_delete) {
			int index;
			if (!LBWidgetUtils.CheckSelectedInRange(list_presets, index) || !itemCopy)
				return true;
			string name;
			list_presets.GetItemText(index, 0, name);
			LBAdminMenuSettings.Get.DeletePreset(itemCopy.GetType(), name);
			LoadPresets();
			return true;
		} else if (w == btn_fav) {
			SetNewFavourite();
			return true;
		} else if (w == btn_select_item) {
			TStringSet items = LBInherit.Get().GetAllChildren({"BaseBuildingBase"}, false, true, true, 2);
			TStringArray itemsArr = LBArrayTools<string>.ToArray(items);
			LBMenuManager.Get().OpenMenu("LBListSelectorDialog", new LBMenuDataListSelector(ScriptCaller.Create(SetItemManually), itemsArr, "#lb_adm_construction_editor_select_list"), false, LBMenuManager.Get().GetOpenLBMenu());
			return true;
		}
		return false;
	}

	override bool OnDoubleClick(Widget w, int x, int y, int button) {
		if (super.OnDoubleClick(w, x, y, button))
			return true;
		if (w == list_presets && list_presets.GetNumItems() > list_presets.GetSelectedRow() && list_presets.GetSelectedRow() >= 0) {
			ApplyPreset();
			return true;
		}
		return false;
	}

	void ApplyPreset() {
		Param5<BaseBuildingBase, int, int, int, bool> param = new Param5<BaseBuildingBase, int, int, int, bool>(original, itemCopy.m_SyncParts01, itemCopy.m_SyncParts02, itemCopy.m_SyncParts03, itemCopy.m_HasBase);
		g_Game.RPCSingleParam(null, LBAdminToolRPCs.BASEBUILDING_SET, param, true);
	}

	void SetItemManually(string itemname) {
		manuallySelectedItem = true;
		Object obj = null;
		if (itemCopy)
			g_Game.ObjectDelete(itemCopy);
		obj = g_Game.CreateObjectEx(itemname, vector.Zero, ECE_LOCAL);
		itemCopy = BaseBuildingBase.Cast(obj);
		if (!itemCopy) {
			if (obj)
				g_Game.ObjectDelete(obj);
			list_presets.ClearItems();
			return;
		}
		item_preview.SetItem(itemCopy);
		itemCopy.OnSynchronizedClientLB();
		Construction constr = itemCopy.GetConstruction();
		map<string, ref ConstructionPart> parts = constr.GetConstructionParts();
		TStringArray partNames = new TStringArray();
		foreach (string part_name2, ConstructionPart part2 : parts) {
			partNames.Insert(part_name2);
		}
		partNames.Sort();
		foreach (string part_name : partNames) {
			ConstructionPart part = parts.Get(part_name);
			Widget root = LBLayoutManager.Get().CreateLayout("ConstructionEditorEntry", parts_list);
			TextWidget name = TextWidget.Cast(root.FindAnyWidget("name"));
			CheckBoxWidget chckbx = CheckBoxWidget.Cast(root.FindAnyWidget("built"));
			name.SetText(part_name);
			builtParts.Insert(part_name, chckbx);
		}
		UpdatePartsBuilt();
		LoadPresets();
	}

	override bool OnItemSelected(Widget w, int x, int y, int row, int column, int oldRow, int oldColumn) {
		if (super.OnItemSelected(w, x, y, row, column, oldRow, oldColumn))
			return true;
		if (w == list_presets) {
			int index;
			if (!LBWidgetUtils.CheckSelectedInRange(list_presets, index) || !itemCopy)
				return true;
			string name;
			list_presets.GetItemText(index, 0, name);
			TStringSet presetParts = LBAdminMenuSettings.Get.GetConstructedParts(itemCopy.GetType(), name);
			foreach (string part_name, CheckBoxWidget chckbx : builtParts) {
				bool built = false;
				if (presetParts.Find(part_name) != -1)
					built = true;
				if (chckbx.IsChecked() != built) {
					SetPartBuilt(part_name, built);
					chckbx.SetChecked(built);
				}
			}
			UpdatePartsBuilt();
			return true;
		}
		return false;
	}

	void SavePopup(string name) {
		if (name == "" || !itemCopy)
			return;
		TStringSet built = new TStringSet();
		foreach (string part_name, CheckBoxWidget chckbx : builtParts) {
			if (chckbx.IsChecked())
				built.Insert(part_name);
		}
		LBAdminMenuSettings.Get.SaveConstructedParts(itemCopy.GetType(), name, built);
		LoadPresets();
	}

}
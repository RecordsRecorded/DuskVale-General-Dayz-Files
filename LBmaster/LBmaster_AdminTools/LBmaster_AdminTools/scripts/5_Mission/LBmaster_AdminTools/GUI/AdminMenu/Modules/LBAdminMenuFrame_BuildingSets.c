class LBAdminMenuFrame_BuildingSets : LBAdminMenuFrame {

	ref array<ref Param3<string, bool, int>> buildingSets = new array<ref Param3<string, bool, int >> ();

	override string GetSharedButton() {
		return "LBAdminMenuFrame_ObjectBuilder";
	}

	override string GetTitle() {
		return "#lb_adm_title_building_sets";
	}

	override void GetMinSize(out int width, out int height) {
		width = 60;
		height = 0;
	}

	override void GetInitialSize(out int width, out int height) {
		width = 600;
		height = 200;
	}

	override string GetInfoString() {
		return "#lb_adm_desc_building_sets";
	}

	TextWidget txt_selected;
	CheckBoxWidget chckbx_enabled;
	EditBoxWidget edit_search;
	TextListboxWidget list_available;
	ButtonWidget btn_delete, btn_edit, btn_save, btn_rename, btn_new, btn_export;

	ref Param3<string, bool, int> toggleBuildingSet = null;
	bool sentChange = false;
	bool savedChanges = true;
	string selectedSet = "";

	override void Show(bool show) {
		super.Show(show);
		g_Game.RPCSingleParam(null, LBAdminToolRPCs.REQUEST_BUILDING_SETS, new Param1<bool>(true), true);
		LoadBuildingSets();
		bool canEdit = LBAdmins.Get().HasPermission("admin.edit.buildingset");
		btn_delete.Show(canEdit);
		btn_edit.Show(canEdit);
		btn_rename.Show(canEdit);
		btn_new.Show(canEdit);
		btn_export.Show(canEdit);
		chckbx_enabled.Show(LBAdmins.Get().HasPermission("admin.toggle.buildingset"));
	}

	void SetSelected(string name) {
		txt_selected.SetText("#lb_adm_selected: " + name);
		selectedSet = name;
	}

	override void OnRPC(Object target, int rpc_type, ParamsReadContext ctx) {
		if (rpc_type == LBAdminToolRPCs.REQUEST_BUILDING_SETS) {
			ctx.Read(buildingSets);
			LoadBuildingSets();
		} else if (rpc_type == LBAdminToolRPCs.TOGGLE_BUILDING_SET) {
			Param2<string, bool> param;
			if (ctx.Read(param)) {
				foreach (Param3<string, bool, int> bSet : buildingSets) {
					if (bSet.param1 == param.param1)
						bSet.param2 = param.param2;
				}
				if (sentChange)
					LBLoadingIcon.Hide();
				sentChange = false;
				LoadBuildingSets();
			}
		} else if (rpc_type == LBAdminToolRPCs.SAVE_BUILDING_SET) {
			string name, editorSteamid, editorName;
			int objectCount;
			if (!ctx.Read(name) || !ctx.Read(objectCount) || !ctx.Read(editorSteamid) || !ctx.Read(editorName))
				return;
			LBAdminMenuFrame_ObjectBuilder builder = LBAdminMenuFrame_ObjectBuilder.Cast(parent.GetFrame("LBAdminMenuFrame_ObjectBuilder"));
			if (builder && builder.currentBuildingSet) {
				if (builder.currentBuildingSet.name == name) {
					if (LBAdmins.Get().GetMySteamid() != editorSteamid)
						NotificationSystem.AddNotificationExtended(10.0, "#lb_adm_title_object_builder", LBTranslatedString("#lb_adm_message_buildingset_modified_by_other", {"%name%", name, "%player%", editorName}).Get(), LBIconConfig.Get.info);
					else {
						LBLoadingIcon.Hide();
						UnselectBuildingSet();
					}
				}
			}
			foreach (Param3<string, bool, int> bSet2 : buildingSets) {
				if (bSet2.param1 == name)
					bSet2.param3 = objectCount;
			}
			LoadBuildingSets();
		} else if (rpc_type == LBAdminToolRPCs.CREATE_BUILDING_SET) {
			if (!ctx.Read(name))
				return;
			buildingSets.Insert(new Param3<string, bool, int>(name, false, 0));
			LoadBuildingSets();
		} else if (rpc_type == LBAdminToolRPCs.RENAME_BUILDING_SET) {
			Param2<string, string> renameParam;
			if (!ctx.Read(renameParam))
				return;
			foreach (Param3<string, bool, int> bSet3 : buildingSets) {
				if (bSet3.param1 == renameParam.param1)
					bSet3.param1 = renameParam.param2;
			}
			LoadBuildingSets();
		} else if (rpc_type == LBAdminToolRPCs.DELETE_BUILDING_SET) {
			if (ctx.Read(name)) {
				for (int i = 0; i < buildingSets.Count(); ++i) {
					if (buildingSets.Get(i).param1 == name) {
						buildingSets.RemoveOrdered(i);
						--i;
					}
				}
				LoadBuildingSets();
			}
		}
	}

	void LoadBuildingSets() {
		if (!buildingSets)
			return;
		int i = 0, a = 0;
		string search = edit_search.GetText();
		foreach (Param3<string, bool, int> bSet : buildingSets) {
			if (!LBStringTools.ContainsIgnoreCase(bSet.param1, search))
				continue;
			if (list_available.GetNumItems() <= a)
				list_available.AddItem(bSet.param1 + "(" + bSet.param3 + ")", bSet, 0);
			else
				list_available.SetItem(a, bSet.param1 + "(" + bSet.param3 + ")", bSet, 0);
			if (bSet.param2)
				list_available.SetItemColor(a, 0, ARGB(255, 0, 255, 0));
			else
				list_available.SetItemColor(a, 0, ARGB(255, 255, 200, 200));
			++a;
		}
		for (int j = list_available.GetNumItems() - 1; j >= 0; --j) {
			if (j >= a) {
				list_available.RemoveRow(j);
			}
		}
		if (list_available.GetSelectedRow() >= a) {
			list_available.SelectRow(a - 1);
			list_available.EnsureVisible(a - 1);
		}
		OnBuildingSetSelected();
	}

	void OnBuildingSetSelected() {
		int index;
		if (!LBWidgetUtils.CheckSelectedInRange(list_available, index))
			return;
		Param3<string, bool, int> param;
		list_available.GetItemData(index, 0, param);
		chckbx_enabled.SetChecked(param.param2);
	}

	string GetSelectedBuildingSetName() {
		int index;
		if (!LBWidgetUtils.CheckSelectedInRange(list_available, index))
			return "";
		Param3<string, bool, int> param;
		list_available.GetItemData(index, 0, param);
		return param.param1;
	}

	void ContinueUnsavedSelect() {
		SetSavedChanges(true);
		StartEditBuildingSet();
	}

	void SetSavedChanges(bool saved) {
		savedChanges = saved;
		btn_save.Show(!saved);
	}

	void UnselectBuildingSet() {
		LBAdminMenuFrame_ObjectBuilder builder = LBAdminMenuFrame_ObjectBuilder.Cast(parent.GetFrame("LBAdminMenuFrame_ObjectBuilder"));
		SetSavedChanges(true);
		builder.currentBuildingSet = null;
		builder.ShowHiddenObjects();
		txt_selected.SetText("");
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ResetSelectedSetString, 300, false);
		LBAdminMenuFrame_ObjectSelection selection = LBAdminMenuFrame_ObjectSelection.Cast(parent.GetFrame("LBAdminMenuFrame_ObjectSelection"));
		selection.LoadBuildings(null);
	}

	void ResetSelectedSetString() {
		selectedSet = "";
	}

	void StartEditBuildingSet() {
		if (!LBAdmins.Get().HasPermission("admin.edit.buildingset"))
			return;
		if (!savedChanges) {
			LBWarningPopup.Get().Show("#continue", LBTranslatedString("#lb_adm_message_unsaved_building_changes", {"%name%", selectedSet}).Get(), ScriptCaller.Create(ContinueUnsavedSelect));
			return;
		}
		string name = GetSelectedBuildingSetName();
		if (name == selectedSet) {
			UnselectBuildingSet();
			return;
		}
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(ResetSelectedSetString);
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(name);
		rpc.Write(false);
		LBLoadingIcon.Show();
		rpc.Send(null, LBAdminToolRPCs.LOAD_BUILDING_SET, true);
	}

	override bool OnClick(Widget w, int x, int y, int button) {
		if (super.OnClick(w, x, y, button))
			return true;
		if (w == btn_edit) {
			StartEditBuildingSet();
			return true;
		} else if (w == btn_delete) {
			string name = GetSelectedBuildingSetName();
			if (name != "")
				LBWarningPopup.Get().Show("#delete", LBTranslatedString("#lb_adm_message_delete_building_set", {"%name%", name}).Get(), ScriptCaller.Create(ConfirmDelete));
		} else if (w == chckbx_enabled) {
			int index;
			if (!LBWidgetUtils.CheckSelectedInRange(list_available, index))
				return true;
			list_available.GetItemData(index, 0, toggleBuildingSet);
			toggleBuildingSet.param2 = chckbx_enabled.IsChecked();
			if (chckbx_enabled.IsChecked()) {
				LBWarningPopup.Get().Show("#lb_adm_enable", LBTranslatedString("#lb_adm_message_enable_buildingset", {"%name%", toggleBuildingSet.param1}).Get(), ScriptCaller.Create(ConfirmToggleBuildingSet));
			} else {
				LBWarningPopup.Get().Show("#lb_adm_disable", LBTranslatedString("#lb_adm_message_disabled_buildingset", {"%name%", toggleBuildingSet.param1}).Get(), ScriptCaller.Create(ConfirmToggleBuildingSet));
			}
			chckbx_enabled.SetChecked(!chckbx_enabled.IsChecked());

		} else if (w == btn_save) {
			LBAdminMenuFrame_ObjectBuilder builder = LBAdminMenuFrame_ObjectBuilder.Cast(parent.GetFrame("LBAdminMenuFrame_ObjectBuilder"));
			builder.FinishInfoObjectEdit();
			if (builder && builder.currentBuildingSet) {
				LBLoadingIcon.Show();
				ScriptRPC rpc = new ScriptRPC();
				rpc.Write(builder.currentBuildingSet.name);
				rpc.Write(builder.currentBuildingSet.objects);
				rpc.Send(null, LBAdminToolRPCs.SAVE_BUILDING_SET, true);
			}

		} else if (w == btn_new) {
			LBSimpleInputPopup.Get().Show("#lb_adm_create", "#lb_adm_message_enter_buildingset_name", "", ScriptCaller.Create(CreateNewBuildingSet));
		} else if (w == btn_rename) {
			if (!LBWidgetUtils.CheckSelectedInRange(list_available, index))
				return true;
			list_available.GetItemData(index, 0, toggleBuildingSet);
			if (toggleBuildingSet)
				LBSimpleInputPopup.Get().Show("#lb_adm_rename_2", LBTranslatedString("#lb_adm_message_enter_new_buildingset_name", {"%name%", toggleBuildingSet.param1}).Get(), "", ScriptCaller.Create(RenameBuildingSet));
		} else if (w == btn_export) {
			ExportSelectedBuildingSet();
		}
		return false;
	}

	void ExportSelectedBuildingSet() {
		ScriptRPC rpc = new ScriptRPC();
		string name = GetSelectedBuildingSetName();
		if (name == "")
			return;
		rpc.Write(name);
		rpc.Write(true);
		LBLoadingIcon.Show();
		rpc.Send(null, LBAdminToolRPCs.LOAD_BUILDING_SET, true);
	}

	void RenameBuildingSet(string name) {
		if (name.Trim() == "") {
			NotificationSystem.AddNotificationExtended(4.0, "#lb_adm_title_object_builder", "#lb_adm_message_name_not_empty", LBIconConfig.Get.info);
			return;
		}
		g_Game.RPCSingleParam(null, LBAdminToolRPCs.RENAME_BUILDING_SET, new Param2<string, string>(toggleBuildingSet.param1, name), true);
	}

	void CreateNewBuildingSet(string name) {
		if (name.Trim() == "") {
			NotificationSystem.AddNotificationExtended(4.0, "#lb_adm_title_object_builder", "#lb_adm_message_name_not_empty", LBIconConfig.Get.info);
			return;
		}
		g_Game.RPCSingleParam(null, LBAdminToolRPCs.CREATE_BUILDING_SET, new Param1<string>(name), true);
	}

	void ConfirmToggleBuildingSet() {
		g_Game.RPCSingleParam(null, LBAdminToolRPCs.TOGGLE_BUILDING_SET, new Param2<string, bool>(toggleBuildingSet.param1, toggleBuildingSet.param2), true);
		sentChange = true;
		LBLoadingIcon.Show();
	}

	void ConfirmDelete() {
		string name = GetSelectedBuildingSetName();
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(name);
		rpc.Send(null, LBAdminToolRPCs.DELETE_BUILDING_SET, true);
	}

	override bool OnDoubleClick(Widget w, int x, int y, int button) {
		if (super.OnDoubleClick(w, x, y, button))
			return true;
		if (w == list_available) {
			StartEditBuildingSet();
			return true;
		}
		return false;
	}

	override bool OnItemSelected(Widget w, int x, int y, int row, int column,	int oldRow, int oldColumn) {
		if (super.OnItemSelected(w, x, y, row, column, oldRow, oldColumn))
			return true;
		if (w == list_available) {
			OnBuildingSetSelected();
			return true;
		}
		return false;
	}

	override bool OnChange(Widget w, int x, int y, bool finished) {
		if (super.OnChange(w, x, y, finished))
			return true;
		if (w == edit_search) {
			LoadBuildingSets();
			return true;
		}
		return false;
	}

}
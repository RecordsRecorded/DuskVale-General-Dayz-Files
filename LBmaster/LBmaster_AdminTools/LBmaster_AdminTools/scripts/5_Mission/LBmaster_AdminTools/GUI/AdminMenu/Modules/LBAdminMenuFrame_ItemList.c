class LBAdminMenuFrame_ItemList : LBAdminMenuFrame {

	override string GetTitle() {
		return "#lb_adm_title_items_on_map";
	}

	override string GetButtonIcon() {
		return "LBmaster_AdminTools/gui/icons/item_search.paa";
	}

	override void GetMinSize(out int width, out int height) {
		width = 300;
		height = 300;
	}

	override void GetInitialSize(out int width, out int height) {
		width = 600;
		height = 400;
	}

	override TStringArray GetRequiredPermissionsOne() {
		return {"admin.view.items"};
	}

	override string GetInfoString() {
		return "#lb_adm_desc_items_on_map";
	}

	TextWidget txt_info;
	CheckBoxWidget chckbx_include_empty, chckbx_show_all, chckbx_show_names, chckbx_show_child;
	MapWidget mapWidget;
	EditBoxWidget edit_search;
	TextListboxWidget item_list;
	ButtonWidget btn_del, btn_del_all;

	string search;
	string selected;
	ref TStringArray list_items = new TStringArray();
	ref TStringSet searchItems = new TStringSet();
	ref TStringSet allItems = new TStringSet();
	ref TStringSet deleteItems = new TStringSet();
	ref map<string, int> count = new map<string, int>();
	int filterCount = 0;
	ref array<Param4<string, vector, int, int>> selectedEntries = new array<Param4<string, vector, int, int >> ();
	ref array<Param4<string, vector, int, int>> visibleEntries = new array<Param4<string, vector, int, int >> ();

	override void Show(bool show) {
		super.Show(show);
		if (!show)
			return;
		AdminToolsRPCHandlerClient handler = AdminToolsRPCHandlerClient.Cast(g_Game.LBGetRPCHandler("AdminToolsRPCHandlerClient"));
		allItems = LBInherit.Get().GetAllChildren({"Weapon_Base", "DefaultMagazine", "Inventory_Base", "Transport", "CarWheel", "CarDoor", "DayZInfected", "SurvivorBase", "AnimalBase"}, true, true, true, 2);
		handler.Event_Map_Items_Received.Insert(HandleNewItems);
		UpdateList();
		UpdateSearch();
		if (!handler.requestedAllItems) {
			handler.requestedAllItems = true;
			mapWidget.ClearUserMarks();
			item_list.ClearItems();
			txt_info.SetText("#lb_adm_waiting_for_itemlist");
			g_Game.RPCSingleParam(null, LBAdminToolRPCs.REQUEST_ITEMS, new Param1<bool>(true), true);
		}
		btn_del.Show(LBAdmins.Get().HasPermission("admin.delete.item"));
		btn_del_all.Show(LBAdmins.Get().HasPermission("admin.delete.items"));
		chckbx_show_child.SetChecked(LBAdminMenuSettings.Get.itemMapListShowChildCount);
		chckbx_show_names.SetChecked(LBAdminMenuSettings.Get.itemMapListShowNames);
	}

	void HandleNewItems() {
		AdminToolsRPCHandlerClient handler = AdminToolsRPCHandlerClient.Cast(g_Game.LBGetRPCHandler("AdminToolsRPCHandlerClient"));
		mapWidget.ClearUserMarks();
		UpdateList();
		visibleEntries.Clear();
		foreach (Param4<string, vector, int, int> param : handler.receivedMapItems) {
			AddItem(param);
		}
	}

	void AddItem(Param4<string, vector, int, int> param) {
		if (param.param1 == selected || ((filterCount <= 1000 || chckbx_show_all.IsChecked()) && list_items.Find(param.param1) != -1)) {
			int color = ARGB(255, 255, 255, 255);
			if (selectedEntries.Find(param) != -1)
				color = ARGB(255, 255, 0, 0);
			else if (param.param1 == selected)
				color = ARGB(255, 0, 255, 0);
			visibleEntries.Insert(param);
			if (param.param3 > 0)
				mapWidget.AddUserMark(param.param2, GetName(param.param1) + GetItemExtraInfo("(" + param.param3 + "/" + param.param4 + ")"), color, "\\dz\\gear\\navigation\\data\\map_tree_ca.paa");
			else
				mapWidget.AddUserMark(param.param2, GetName(param.param1), color, "\\dz\\gear\\navigation\\data\\map_tree_ca.paa");
		}
	}

	string GetItemExtraInfo(string info) {
		if (chckbx_show_child.IsChecked())
			return info;
		return "";
	}

	string GetName(string name) {
		if (chckbx_show_names.IsChecked())
			return name;
		return "";
	}

	void UpdateSearch() {
		mapWidget.ClearUserMarks();
		AdminToolsRPCHandlerClient handler = AdminToolsRPCHandlerClient.Cast(g_Game.LBGetRPCHandler("AdminToolsRPCHandlerClient"));
		visibleEntries.Clear();
		foreach (Param4<string, vector, int, int> param : handler.receivedMapItems) {
			AddItem(param);
		}
	}

	void UpdateList() {
		count.Clear();
		int children = 0;
		AdminToolsRPCHandlerClient handler = AdminToolsRPCHandlerClient.Cast(g_Game.LBGetRPCHandler("AdminToolsRPCHandlerClient"));
		foreach (Param4<string, vector, int, int> param : handler.receivedMapItems) {
			children += param.param4;
			int i = 0;
			if (!count.Find(param.param1, i))
				i = 0;
			++i;
			count.Set(param.param1, i);
		}
		item_list.ClearItems();
		list_items.Clear();
		int displayed = 0;
		foreach (string name, int cnt : count) {
			if (search == "" || name.IndexOf(search) != -1 || searchItems.Find(name) != -1) {
				Param3<int, int, int> ceParam = handler.ceValues.Get(name);
				if (ceParam) {
					string lifetime = LBDate.Init(ceParam.param3).ToDiffString();
					item_list.AddItem(name + " (" + ceParam.param1 + "/" + cnt + "/" + ceParam.param2 + ") " + lifetime, null, 0);
				} else {
					item_list.AddItem(name + " (" + cnt + ") #lb_adm_not_in_types", null, 0);
				}
				list_items.Insert(name);
				displayed += cnt;
			}
		}
		filterCount = displayed;
		txt_info.SetText(LBTranslatedString("#lb_adm_items_on_map_info", {"%displayed%", "" + displayed, "%received%", "" + handler.receivedMapItems.Count(), "%children%", "" + children}).Get());
		if (chckbx_include_empty.IsChecked()) {
			foreach (string item : allItems) {
				if (search == "" || item.IndexOf(search) != -1 || searchItems.Find(item) != -1) {
					if (!count.Contains(item)) {
						item_list.AddItem(item + " (" + 0 + ")", null, 0);
						list_items.Insert(item);
					}
				}
			}
		}
	}

	override bool OnClick(Widget w, int x, int y, int button) {
		if (super.OnClick(w, x, y, button))
			return true;
		if (w == chckbx_include_empty || w == chckbx_show_all || w == chckbx_show_child || w == chckbx_show_names) {
			UpdateList();
			UpdateSearch();
			LBAdminMenuSettings.Get.itemMapListShowChildCount = chckbx_show_child.IsChecked();
			LBAdminMenuSettings.Get.itemMapListShowNames = chckbx_show_names.IsChecked();
			LBAdminMenuSettings.Loader.Save();
			return true;
		} else if (w == btn_del) {
			deleteItems.Clear();
			deleteItems.Insert(selected);
			LBWarningPopup.Get().Show("#lb_delete", LBTranslatedString("#lb_adm_delete_on_map", {"%name%", selected, "%count%", "" + count.Get(selected)}).Get(), ScriptCaller.Create(OnConfirmDelete));
			return true;
		} else if (w == btn_del_all) {
			deleteItems.Clear();
			int total = 0;
			foreach (string item : list_items) {
				deleteItems.Insert(item);
				total += count.Get(item);
			}
			LBWarningPopup.Get().Show("#lb_delete", LBTranslatedString("#lb_adm_delete_on_map_all", {"%count%", "" + deleteItems.Count(), "%total%", "" + total}).Get(), ScriptCaller.Create(OnConfirmDelete));
			return true;
		} else if (w == mapWidget) {
			vector worldpos = mapWidget.ScreenToMap(Vector(x, y, 0));
			worldpos[1] = 0;
			float nearest = 200 * 200;
			if (!g_Game.LBIsKeyDown(KeyCode.KC_LCONTROL))
				selectedEntries.Clear();
			Param4<string, vector, int, int> best = null;
			foreach (Param4<string, vector, int, int> entry : visibleEntries) {
				float dist = vector.DistanceSq(entry.param2, worldpos);
				if (dist < nearest && selectedEntries.Find(entry) == -1) {
					nearest = dist;
					best = entry;
				}
			}
			if (best)
				selectedEntries.Insert(best);
			UpdateSearch();
			return true;
		}
		return false;
	}

	override bool OnDoubleClick(Widget w, int x, int y, int button) {
		if (super.OnDoubleClick(w, x, y, button))
			return true;
		if (w == mapWidget) {
			vector mousePos = Vector(x, y, 0);
			vector mapPos = mapWidget.ScreenToMap(mousePos);
			mapPos[1] = g_Game.SurfaceY(mapPos[0], mapPos[2]);
			if (LBFreecamCamera.instance) {
				LBFreecamCamera.position = mapPos + "0 1 0";
				return true;
			}
			g_Game.RPCSingleParam(null, LBAdminToolRPCs.TP_MAP, new Param2<vector, vector>(mapPos, vector.Zero), true);
			return true;
		} else if (w == item_list) {
			edit_search.SetText(selected);
			SetSearch(selected);
			return true;
		}
		return false;
	}

	void OnConfirmDelete() {
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(deleteItems);
		rpc.Send(null, LBAdminToolRPCs.DELETE_ITEMS, true);
	}

	override bool OnChange(Widget w, int x, int y, bool finished) {
		if (super.OnChange(w, x, y, finished))
			return true;
		if (w == edit_search) {
			SetSearch(edit_search.GetText());
			return true;
		}
		return false;
	}

	override void Update(float timeslice, bool otherMenu, bool leftClicked) {
		super.Update(timeslice, otherMenu, leftClicked);
		if (selectedEntries.Count() > 0 && g_Game.LBIsKeyPressed(KeyCode.KC_DELETE) && LBAdmins.Get().HasPermission("admin.delete.item")) {
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write(selectedEntries);
			rpc.Send(null, LBAdminToolRPCs.DELETE_ITEM_AT, true);
			AdminToolsRPCHandlerClient handler = AdminToolsRPCHandlerClient.Cast(g_Game.LBGetRPCHandler("AdminToolsRPCHandlerClient"));
			foreach (Param4<string, vector, int, int> param : selectedEntries)
				handler.receivedMapItems.RemoveItem(param);
			UpdateList();
			UpdateSearch();
			selectedEntries.Clear();
		}
	}

	void SetSearch(string searchText) {
		search = searchText;
		selected = "";
		search.ToLower();
		searchItems = LBInherit.Get().GetAllChildren({search}, true, true, true, 2);
		UpdateList();
		UpdateSearch();
	}

	override bool OnItemSelected(Widget w, int x, int y, int row, int column, int oldRow, int oldColumn) {
		if (super.OnItemSelected(w, x, y, row, column, oldRow, oldColumn))
			return true;
		if (w == item_list) {
			if (row < 0 || row >= list_items.Count())
				return true;
			selected = list_items.Get(row);
			UpdateSearch();
		}
		return false;
	}

}
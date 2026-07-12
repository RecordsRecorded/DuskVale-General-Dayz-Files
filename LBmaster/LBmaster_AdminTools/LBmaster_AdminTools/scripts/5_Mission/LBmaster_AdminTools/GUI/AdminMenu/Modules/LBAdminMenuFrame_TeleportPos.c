class LBAdminMenuFrame_TeleportPos : LBAdminMenuFrame {

	override string GetTitle() {
		return "#lb_adm_title_teleport_manager";
	}

	override string GetButtonIcon() {
		return "LBmaster_AdminTools/gui/icons/markers.paa";
	}

	override void GetMinSize(out int width, out int height) {
		width = 160;
		height = 200;
	}

	override void GetInitialSize(out int width, out int height) {
		width = 400;
		height = 700;
	}

	override string GetInfoString() {
		return "#lb_adm_desc_teleport_manager";
	}

	override TStringArray GetRequiredPermissionsOne() {
		return {"admin.view.teleport_positions"};
	}

	TextListboxWidget list_positions;
	ButtonWidget btn_add, btn_delete, btn_edit, btn_teleport, btn_teleport_other, btn_teleport_input;
	XComboBoxWidget combo_list_select;
	EditBoxWidget edit_search;

	ref LBTeleportAddPopup addPopup;
	ref LB_TeleportPos oldPosCopy = null, selectedPos = null;

	void LBAdminMenuFrame_TeleportPos() {
		LBConfigManager.Get().GetEventOnConfigReceived("LBPositionsListServer").Insert(LoadPositions);
	}

	override void Show(bool show) {
		super.Show(show);

		int lastSelected = LBAdminMenuSettings.Get.teleportManagerCategory;
		if (combo_list_select.GetNumItems() > lastSelected)
			combo_list_select.SetCurrentItem(lastSelected);

		SetButtonVisibility();
		LoadPositions();
		if (!show)
			addPopup = null;
		LBAdminMenuFrame_PlayerMap mapFrame = LBAdminMenuFrame_PlayerMap.Cast(parent.GetFrame("LBAdminMenuFrame_PlayerMap"));
		if (mapFrame && mapFrame.visible && mapFrame.mapWidget)
			mapFrame.UpdateMarkers();
	}

	override void OnHide() {
		super.OnHide();
		addPopup = null;
	}

	override bool OnClick(Widget w, int x, int y, int button) {
		if (super.OnClick(w, x, y, button))
			return true;
		if (w == combo_list_select) {
			SetButtonVisibility();
			LoadPositions();
			LBAdminMenuFrame_PlayerMap mapFrame = LBAdminMenuFrame_PlayerMap.Cast(parent.GetFrame("LBAdminMenuFrame_PlayerMap"));
			if (mapFrame && mapFrame.IsVisible()) {
				mapFrame.UpdateMarkers();
				mapFrame.OnPlayerSelected(null);
			}
			LBAdminMenuSettings.Get.teleportManagerCategory = combo_list_select.GetCurrentItem();
			LBAdminMenuSettings.Loader.Save();
			return true;
		} else if (w == btn_add) {
			addPopup = new LBTeleportAddPopup(new LB_TeleportPos(), true, this);
			return true;
		} else if (w == btn_delete) {
			LBPositionsListBase config = GetSelectedPositions();
			LB_TeleportPos current = GetCurrentPosition();
			if (config && current)
				config.RemovePosition(current);
			LoadPositions();
			return true;
		} else if (w == btn_edit) {
			current = GetCurrentPosition();
			if (current) {
				oldPosCopy = current.Copy();
				addPopup = new LBTeleportAddPopup(current, false, this);
			}
			return true;
		} else if (w == btn_teleport) {
			TeleportToSelected();
			return true;
		} else if (w == btn_teleport_other) {
			array<PlayerIdentity> selected = LBAdminMenuFrame_PlayerList.Cast(parent.GetFrame("LBAdminMenuFrame_PlayerList")).selectedPlayers;
			if (selected.Count() == 0) {
				NotificationSystem.AddNotificationExtended(5.0, "#lb_adm_admin_tools", "#lb_adm_no_player_selected", LBIconConfig.Get.warning);
			} else {
				selectedPos = GetCurrentPosition();
				if (!selectedPos)
					return true;
				LBWarningPopup.Get().Show("#lb_adm_teleport", LBTranslatedString("#lb_adm_teleport_muiltiple_confirm", {"%count%", "" + selected.Count(), "%name%", selectedPos.name}).Get(), ScriptCaller.Create(ConfirmTeleportSelectedPlayers));
				return true;
			}
		} else if (w == btn_teleport_input) {
			LBSimpleInputPopup.Get().Show("#lb_adm_teleport", "#lb_adm_enter_the_coordinates_to_teleport_to", "", ScriptCaller.Create(OnTeleportToCoords));
			return true;
		}
		return false;
	}

	void OnTeleportToCoords(string text) {
		vector pos;
		if (LBConsoleCommandTeleport.TryParsePos(text, pos)) {
			if (LBFreecamCamera.instance) {
				LBFreecamCamera.position = pos + "0 1 0";
				return;
			}
			vector orient = vector.Zero;
			if (g_Game.GetPlayer())
				orient = g_Game.GetPlayer().GetOrientation();
			g_Game.RPCSingleParam(null, LBAdminToolRPCs.TP_MAP, new Param2<vector, vector>(pos, orient), true);
		} else {
			NotificationSystem.AddNotificationExtended(5.0, "#lb_adm_admin_tools", "#lb_adm_could_not_parse_position", LBIconConfig.Get.warning);
		}
	}

	void ConfirmTeleportSelectedPlayers() {
		ScriptRPC rpc = new ScriptRPC();
		array<PlayerIdentity> selected = LBAdminMenuFrame_PlayerList.Cast(parent.GetFrame("LBAdminMenuFrame_PlayerList")).selectedPlayers;
		rpc.Write(selected);
		rpc.Write(selectedPos.pos);
		rpc.Write(selectedPos.dir);
		rpc.Send(null, LBAdminToolRPCs.TP_MAP_OTHERS, true);
	}

	override bool OnDoubleClick(Widget w, int x, int y, int button) {
		if (w == list_positions) {
			TeleportToSelected();
			return true;
		}
		return super.OnDoubleClick(w, x, y, button);
	}

	override bool OnChange(Widget w, int x, int y, bool finished) {
		if (super.OnChange(w, x, y, finished))
			return true;
		if (w == edit_search) {
			LoadPositions();
			return true;
		}
		return false;
	}

	override bool OnItemSelected(Widget w, int x, int y, int row, int column, int oldRow, int oldColumn) {
		if (super.OnItemSelected(w, x, y, row, column, oldRow, oldColumn))
			return true;
		if (w == list_positions) {
			LBAdminMenuFrame_PlayerMap mapFrame = LBAdminMenuFrame_PlayerMap.Cast(parent.GetFrame("LBAdminMenuFrame_PlayerMap"));
			if (!mapFrame || !mapFrame.IsVisible())
				return true;
			LB_TeleportPos current = GetCurrentPosition();
			if (!current)
				return true;
			mapFrame.OnPlayerSelected(null);
			mapFrame.ZoomToPos(current.pos, true, true);
		}
		return false;
	}

	void TeleportToSelected() {
		LB_TeleportPos current = GetCurrentPosition();
		if (LBFreecamCamera.instance) {
			LBFreecamCamera.position = current.pos + "0 1 0";
			LBFreecamCamera.orientation = current.dir;
			return;
		}
		if (!LBAdmins.Get().HasPermission("admin.teleport.map"))
			return;
		if (current) {
			g_Game.RPCSingleParam(null, LBAdminToolRPCs.TP_MAP, new Param2<vector, vector>(current.pos, current.dir), true);
		}
	}

	LB_TeleportPos GetCurrentPosition() {
		LBPositionsListBase config = GetSelectedPositions();
		if (!config || !config.GetPositions())
			return null;
		int index = list_positions.GetSelectedRow();
		if (index < 0 || index >= list_positions.GetNumItems())
			return null;
		LB_TeleportPos pos;
		list_positions.GetItemData(index, 0, pos);
		return pos;
	}

	void CloseAddPopup() {
		addPopup = null;
	}

	void OnPositionAdd(LB_TeleportPos pos) {
		LBPositionsListBase config = GetSelectedPositions();
		if (config) {
			config.AddPosition(pos);
			LoadPositions();
		}
	}

	void OnPositionEdit(LB_TeleportPos pos) {
		LBPositionsListBase config = GetSelectedPositions();
		if (config) {
			config.ChangePosition(pos, oldPosCopy);
			LoadPositions();
		}
	}

	void LoadPositions() {
		LBPositionsListBase posConfig = GetSelectedPositions();
		if (!posConfig || !posConfig.GetPositions()) {
			list_positions.ClearItems();
			list_positions.SelectRow(-1);
			return;
		}
		int i = 0, a = 0;
		string searched = edit_search.GetText();
		searched.ToLower();
		for (; i < posConfig.GetPositions().Count(); ++i) {
			LB_TeleportPos pos = posConfig.GetPositions().Get(i);
			if (pos && (searched == "" || LBStringTools.ContainsIgnoreCase(pos.name, searched))) {
				if (list_positions.GetNumItems() <= a)
					list_positions.AddItem(pos.name, pos, 0);
				else
					list_positions.SetItem(a, pos.name, pos, 0);
				++a;
			}
		}
		for (int j = list_positions.GetNumItems() - 1; j >= 0; --j) {
			if (j >= a) {
				list_positions.RemoveRow(j);
			}
		}
		if (list_positions.GetSelectedRow() >= a) {
			list_positions.SelectRow(a - 1);
			list_positions.EnsureVisible(a - 1);
		}
	}

	LBPositionsListBase GetSelectedPositions() {
		int selected = combo_list_select.GetCurrentItem();
		if (selected == 0) {
			return LBPositionsListPrivate.Get;
		} else {
			return LBPositionsListServer.Get;
		}
	}

	void SetButtonVisibility() {
		int selected = combo_list_select.GetCurrentItem();
		if (selected == 0) {
			btn_add.Enable(true);
			btn_delete.Enable(true);
			btn_edit.Enable(true);
		} else {
			btn_add.Enable(LBAdmins.Get().HasPermission("admin.add.teleport_positions"));
			bool canRemove = LBAdmins.Get().HasPermission("admin.remove.teleport_positions");
			btn_delete.Enable(canRemove);
			btn_edit.Enable(canRemove);
		}
		btn_teleport.Enable(LBAdmins.Get().HasPermission("admin.teleport.map"));
		btn_teleport_other.Enable(LBAdmins.Get().HasPermission("admin.bring.players"));
	}
}
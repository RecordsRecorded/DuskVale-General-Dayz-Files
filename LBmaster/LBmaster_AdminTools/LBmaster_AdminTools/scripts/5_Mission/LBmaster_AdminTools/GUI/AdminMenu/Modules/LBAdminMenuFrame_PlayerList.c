class LBAdminMenuFrame_PlayerList : LBAdminMenuFrame {

	ref ScriptInvoker Event_Players_Selected = new ScriptInvoker();
	ref ScriptInvoker Event_Player_Selected = new ScriptInvoker();
	ref ScriptInvoker Event_Player_Checked = new ScriptInvoker();

	WrapSpacerWidget online_content;
	TextWidget txt_info;
	CheckBoxWidget chckbx_all;
	XComboBoxWidget combo_sort;
	EditBoxWidget edit_search;
	ScrollWidget list_online;
	Widget panelBackground;

	PlayerIdentity selectedPlayer;
	ref array<PlayerIdentity> selectedPlayers = new array<PlayerIdentity>();
	ref TStringArray selectedPlayerSteamids = new TStringArray();
	ref array<ref Param2<CheckBoxWidget, PlayerIdentity>> playerEntries = new array<ref Param2<CheckBoxWidget, PlayerIdentity >> ();

	override string GetTitle() {
		return "#lb_adm_title";
	}

	override string GetButtonIcon() {
		return "LBmaster_AdminTools/gui/icons/player.paa";
	}

	override void GetMinSize(out int width, out int height) {
		width = 150;
		height = 150;
	}

	override void GetInitialSize(out int width, out int height) {
		width = 400;
		height = 500;
	}

	override TStringArray GetRequiredPermissionsOne() {
		return {"admin.view.playerinfo", "admin.view.playerpositions", "admin.view.playerlist"};
	}

	override string GetInfoString() {
		return "#lb_adm_desc_players";
	}

	override void UpdateSlow() {
		super.UpdateSlow();
		if (IsVisible())
			UpdatePlayerList();
	}

	void RemoveInvalidPlayerIdentities() {
		bool changed = false;
		for (int i = 0; i < selectedPlayers.Count(); ++i) {
			if (!selectedPlayers.Get(i)) {
				selectedPlayers.Remove(i);
				selectedPlayerSteamids.Remove(i);
				--i;
				changed = true;
			}
		}
		if (changed)
			InvokePlayersSelected();
	}

	private string GetPlayerName(PlayerIdentity player) {
		if (!player)
			return "";
		if (LBAdminMenuSettings.Get.displayPlayerTagsInPlayerList) {
#ifdef LBmaster_Groups
			string groupname, groupshortname;
			if (LBGroupInfoShare.GetPlayerGroup(player.GetPlainId(), groupname, groupshortname)) {
				return "[" + groupshortname + "] " + player.GetName();
			}
#endif
		}
		return player.GetName();
	}

	// Returns the next entry index
	int SetPlayerEntry(int index, PlayerIdentity player) {
		string name = GetPlayerName(player);
		if (playerEntries.Count() > index) {
			Param2<CheckBoxWidget, PlayerIdentity> param = playerEntries.Get(index);
			if (!player) {
				playerEntries.Remove(index);
				param.param1.Unlink();
				return index;
			}
			TextWidget txt = TextWidget.Cast(param.param1.FindAnyWidget("temp_text"));
			param.param2 = player;
			txt.SetText(name);
			return index + 1;
		} else {
			Widget root = LBLayoutManager.Get().CreateLayout("SimpleCheckbox", online_content);
			txt = TextWidget.Cast(root.FindAnyWidget("temp_text"));
			txt.SetText(name);
			playerEntries.Insert(new Param2<CheckBoxWidget, PlayerIdentity>(CheckBoxWidget.Cast(root), player));
			return playerEntries.Count();
		}
	}

	void SetSelectedPlayers() {
		foreach (Param2<CheckBoxWidget, PlayerIdentity> param : playerEntries) {
			if (param.param2 && selectedPlayerSteamids.Find(param.param2.GetPlainId()) != -1) {
				param.param1.FindAnyWidget("background").SetColor(ARGB(100, 0, 255, 0));
				param.param1.SetChecked(true);
			} else {
				param.param1.FindAnyWidget("background").SetColor(0);
				param.param1.SetChecked(false);
			}
			if (selectedPlayer && selectedPlayer.GetPlainId() == param.param2.GetPlainId()) {
				param.param1.FindAnyWidget("background").SetColor(ARGB(100, 0, 255, 255));
			}
		}
		SyncPlayerList playerList = ClientData.m_PlayerList;
		int selectedCount = selectedPlayers.Count();
		int totalPlayers = playerList.m_PlayerList.Count();
		txt_info.SetText(LBTranslatedString(" #lb_adm_players_frame_info", {"%selected%", "" + selectedCount, "%total%", "" + totalPlayers, "%visible%", "" + playerEntries.Count()}).Get());
		if (selectedCount == playerEntries.Count())
			chckbx_all.SetChecked(true);
		else
			chckbx_all.SetChecked(false);
	}

	void UpdatePlayerList() {
		int a = 0;
		array<PlayerIdentity> identities = GetSortedPlayers();
		foreach (PlayerIdentity player : identities) {
			a = SetPlayerEntry(a, player);
		}
		for (int j = playerEntries.Count() - 1; j >= 0; --j) {
			if (j >= a) {
				SetPlayerEntry(j, null);
			}
		}
		RemoveInvalidPlayerIdentities();
		SetSelectedPlayers();
		if (list_online.GetVScrollPos01() > 1.0) {
			list_online.VScrollToPos01(0);
			list_online.VScrollToPos01(1);
		} else if (list_online.GetVScrollPos01() == 0.0) {
			list_online.VScrollToPos01(0);
		}
	}

	array<PlayerIdentity> GetSortedPlayers() {
		string searched = edit_search.GetText();
		searched.ToLower();
		array<PlayerIdentity> identities = new array<PlayerIdentity>();
		SyncPlayerList playerList = ClientData.m_PlayerList;
		for (int i = 0; i < playerList.m_PlayerList.Count(); ++i) {
			SyncPlayer player = playerList.m_PlayerList.Get(i);
			PlayerIdentity identity = player.m_Identity;
			if (identity && (searched == "" || LBStringTools.ContainsIgnoreCase(GetPlayerName(identity), searched) || identity.GetPlainId().IndexOf(searched) != -1)) {
				identities.Insert(identity);
			}
		}
		if (combo_sort.GetCurrentItem() == 1) {
			SortIdentitiesList(identities, false);
		} else if (combo_sort.GetCurrentItem() == 2) {
			SortIdentitiesList(identities, true);
		}
		return identities;
	}

	void SortIdentitiesList(inout array<PlayerIdentity> identities, bool ascending) {
		bool sorted = false;
		while (!sorted) {
			sorted = true;
			for (int i = 0; i < identities.Count() - 1; ++i) {
				PlayerIdentity prev = identities.Get(i);
				PlayerIdentity next = identities.Get(i + 1);
				string lowerPrev = LBStringTools.ToLowerString(GetPlayerName(prev));
				string lowerNext = LBStringTools.ToLowerString(GetPlayerName(next));
				if ((lowerPrev < lowerNext) == ascending) {
					identities.Set(i, next);
					identities.Set(i + 1, prev);
					sorted = false;
				}
			}
		}
	}

	override bool OnChange(Widget w, int x, int y, bool finished) {
		if (super.OnChange(w, x, y, finished))
			return true;
		if (w == edit_search) {
			UpdatePlayerList();
			return true;
		}
		return false;
	}

	override bool OnClick(Widget w, int x, int y, int button) {
		if (super.OnClick(w, x, y, button))
			return true;
		foreach (Param2<CheckBoxWidget, PlayerIdentity> param : playerEntries) {
			if (w == param.param1 && param.param2) {
				float cw, ch, cX, cY;
				param.param1.GetScreenSize(cw, ch);
				param.param1.GetScreenPos(cX, cY);
				if (x < cX + ch) {
					string targetSteamid = param.param2.GetPlainId();
					if (param.param1.IsChecked()) {
						selectedPlayers.Insert(param.param2);
						selectedPlayerSteamids.Insert(targetSteamid);
					} else {
						selectedPlayers.RemoveItemUnOrdered(param.param2);
						selectedPlayerSteamids.RemoveItemUnOrdered(targetSteamid);
					}
					Event_Player_Checked.Invoke(targetSteamid, param.param1.IsChecked());
					InvokePlayersSelected();
					SetSelectedPlayers();
				} else {
					if (selectedPlayer != param.param2) {
						InvokePlayerSelected(param.param2);
					} else {
						InvokePlayerSelected(null);
						if (selectedPlayers.Count() > 0) {
							InvokePlayersSelected();
						}
					}
				}
				return true;
			} else if (w == param.param1.FindAnyWidget("temp_text")) {
				InvokePlayerSelected(param.param2);
				return true;
			}
		}
		if (w == chckbx_all) {
			if (!chckbx_all.IsChecked()) {
				UnselectAll(true);
			} else {
				selectedPlayers.Clear();
				selectedPlayerSteamids.Clear();
				foreach (Param2<CheckBoxWidget, PlayerIdentity> displayed : playerEntries) {
					if (displayed.param2) {
						selectedPlayers.Insert(displayed.param2);
						selectedPlayerSteamids.Insert(displayed.param2.GetPlainId());
						Event_Player_Checked.Invoke(displayed.param2.GetPlainId(), true);
					}
				}
				InvokePlayersSelected();
				SetSelectedPlayers();
			}
		} else if (w == combo_sort) {
			UpdatePlayerList();
			LBAdminMenuSettings.Get.playerListSort = combo_sort.GetCurrentItem();
			LBAdminMenuSettings.Loader.Save();
			return true;
		}
		return false;
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button) {
		if (super.OnMouseButtonDown(w, x, y, button))
			return true;
		if (w == panelBackground) {
			InvokePlayerSelected(null);
			if (selectedPlayers.Count() > 0) {
				InvokePlayersSelected();
			}
			return true;
		}
		return false;
	}
	
	void SelectAll(TStringSet steamids, bool sendCheckedEvent) {
		selectedPlayers.Clear();
		selectedPlayerSteamids.Clear();
		foreach (Param2<CheckBoxWidget, PlayerIdentity> displayed : playerEntries) {
			if (displayed.param2 && steamids && steamids.Find(displayed.param2.GetPlainId()) != -1) {
				selectedPlayers.Insert(displayed.param2);
				selectedPlayerSteamids.Insert(displayed.param2.GetPlainId());
				if (sendCheckedEvent)
					Event_Player_Checked.Invoke(displayed.param2.GetPlainId(), true);
			}
		}
		InvokePlayersSelected();
		SetSelectedPlayers();
	}
	
	void UnselectAll(bool sendCheckedEvent) {
		if (sendCheckedEvent) {
			foreach (string unselected : selectedPlayerSteamids)
				Event_Player_Checked.Invoke(unselected, false);
		}
		selectedPlayers.Clear();
		selectedPlayerSteamids.Clear();
		InvokePlayersSelected();
		SetSelectedPlayers();
	}

	override bool SetPrio() {
		if (super.SetPrio()) {
			combo_sort.SetCurrentItem(LBAdminMenuSettings.Get.playerListSort);
			UpdatePlayerList();
			SetFocus(edit_search);
			return true;
		}
		return false;
	}

	override void Show(bool show) {
		super.Show(show);
		if (show) {
			SetFocus(edit_search);
		}
	}

	void InvokePlayersSelected() {
		Event_Players_Selected.Invoke(selectedPlayers);
		LBAdminMenuSettings_.highlightedPlayers = LBArrayTools<string>.ToSet(selectedPlayerSteamids);
		LBESPManager.UpdateColors();
	}

	void InvokePlayerSelected(PlayerIdentity player) {
		selectedPlayer = player;
		SetSelectedPlayers();
		Event_Player_Selected.Invoke(player);
	}
}
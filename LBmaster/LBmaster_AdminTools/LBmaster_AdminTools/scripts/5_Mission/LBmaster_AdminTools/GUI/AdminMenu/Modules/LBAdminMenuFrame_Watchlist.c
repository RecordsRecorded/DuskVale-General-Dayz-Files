class LBAdminMenuFrame_Watchlist : LBAdminMenuFrame {

	override string GetTitle() {
		return "#lb_adm_watchlist";
	}

	override string GetButtonIcon() {
		return "LBmaster_AdminTools/gui/icons/eye.paa";
	}

	override void GetMinSize(out int width, out int height) {
		width = 600;
		height = 400;
	}

	override void GetInitialSize(out int width, out int height) {
		width = 1000;
		height = 600;
	}

	override string GetInfoString() {
		return "#lb_adm_desc_watchlist";
	}

	override TStringArray GetRequiredPermissionsOne() {
		return {"admin.view.watchlist_local", "admin.view.watchlist_server", "admin.view.watchlist_global"};
	}

	TextWidget txt_added_date, txt_added_by, txt_steamid, txt_name;
	MultilineEditBoxWidget edit_message;
	XComboBoxWidget combo_type;
	EditBoxWidget edit_reason, edit_confidence;
	TextListboxWidget list_entries;
	Widget panel_info;
	ButtonWidget btn_copy_steamid, btn_add, btn_del, btn_save, btn_copy_name, btn_ban;

	string lastSelected = "";

	ref TIntArray allowedWatchlists = new TIntArray();

	override void Show(bool show) {
		super.Show(show);
		if (show) {
			LoadWatchlistList();
		}
	}

	override bool OnClick(Widget w, int x, int y, int button) {
		if (super.OnClick(w, x, y, button))
			return true;
		if (w == combo_type) {
			LoadSelectedWatchlist();
			LBAdminMenuSettings.Get.watchlistLastCategory = allowedWatchlists.Get(combo_type.GetCurrentItem());
			LBAdminMenuSettings.Loader.Save();
			return true;
		} else if (w == btn_save) {
			SaveEntry(GetSelectedWatchlistEntry());
			LoadSelectedWatchlist();
			return true;
		} else if (w == btn_add) {
			LBMenuManager.Get().OpenMenu("LBPlayerSelectorDialog", new LBMenuDataPlayerSelector(ScriptCaller.Create(OnAddPlayer), false), false, LBMenuManager.Get().GetOpenLBMenu());
			return false;
		} else if (w == btn_del) {
			LBWatchlistBase watchlist = GetCurrentWatchlist();
			LBWatchlistEntry entry = GetSelectedWatchlistEntry();
			if (watchlist && entry) {
				LBWarningPopup.Get().Show("#lb_delete", LBTranslatedString("#lb_adm_watchlist_delete_confirm", {"%steamid%", entry.steamid, "%name%", entry.name}).Get(), ScriptCaller.Create(DeleteSelectedWatchlistEntry));
			}
			return true;
		} else if (w == btn_ban) {
			LBWatchlistEntry current = GetSelectedWatchlistEntry();
			if (current && parent.EnsureVisible("LBAdminMenuFrame_Banmanager")) {
				LBAdminMenuFrame_Banmanager banMgr = LBAdminMenuFrame_Banmanager.Cast(parent.GetFrame("LBAdminMenuFrame_Banmanager"));
				banMgr.CreateNewBan(current.steamid, current.name, current.reason);
			}
		} else if (w == btn_copy_steamid) {
			current = GetSelectedWatchlistEntry();
			if (current) {
				g_Game.CopyToClipboard(current.steamid);
				LBCopiedPopup.ShowAtCursor();
				return true;
			}
		} else if (w == btn_copy_name) {
			current = GetSelectedWatchlistEntry();
			if (current) {
				g_Game.CopyToClipboard(current.name);
				LBCopiedPopup.ShowAtCursor();
				return true;
			}
		}
		return false;
	}

	void DeleteSelectedWatchlistEntry() {
		LBWatchlistBase watchlist = GetCurrentWatchlist();
		LBWatchlistEntry entry = GetSelectedWatchlistEntry();
		if (watchlist && entry) {
			watchlist.RemoveFromWatchlist(entry.steamid);
		}
		LoadSelectedWatchlist();
	}

	void OnAddPlayer(PlayerNameSave_ player) {
		LBWatchlistBase watchlist = GetCurrentWatchlist();
		if (!watchlist)
			return;
		LBWatchlistEntry found = watchlist.GetWatchlistEntry(player.GetSteamid());
		if (found) {
			list_entries.SelectRow(watchlist.entries.Find(found));
			NotificationSystem.AddNotificationExtended(5.0, "#lb_adm_admin_tools", "#lb_adm_player_already_on_watchlist", LBIconConfig.Get.warning);
			return;
		}
		LBWatchlistEntry entry = new LBWatchlistEntry(player.GetSteamid(), LBAdmins.Get().GetMySteamid(), "", "", 0.0);
		entry.name = player.GetName();
		watchlist.AddToWatchlist(entry);
		int index = watchlist.entries.Find(entry);
		LoadSelectedWatchlist();
		if (index != -1) {
			list_entries.SelectRow(index);
		}
	}

	override bool OnItemSelected(Widget w, int x, int y, int row, int column,	int oldRow, int oldColumn) {
		if (super.OnItemSelected(w, x, y, row, column, oldRow, oldColumn))
			return true;
		if (w == list_entries) {
			lastSelected = LoadSelectedWatchlistEntry();
			return true;
		}
		return false;
	}

	override bool OnMouseEnter(Widget w, int x, int y) {
		if (super.OnMouseEnter(w, x, y))
			return true;
		if (w == edit_confidence) {
			string text = edit_confidence.GetText();
			if (LBStringTools.EndsWith(text, "%")) {
				text = text.Substring(0, text.Length() - 1);
				edit_confidence.SetText(text);
			}
			SetFocus(edit_confidence);
			return true;
		}
		return false;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y) {
		if (super.OnMouseLeave(w, enterW, x, y))
			return true;
		if (w == edit_confidence) {
			int conf = edit_confidence.GetText().ToInt();
			if (conf < 0)
				conf = 0;
			else if (conf > 100)
				conf = 100;
			edit_confidence.SetText("" + conf + "%");
		}
		return false;
	}

	string LoadSelectedWatchlistEntry() {
		LBWatchlistEntry current = GetSelectedWatchlistEntry();
		if (!current) {
			return "";
		}
		txt_steamid.SetText(current.steamid);
		string addedDate = LBDate.Init((int) current.addedDate).ToFormattedString();
		string modDate = LBDate.Init((int) current.lastModified).ToFormattedString();
		txt_added_date.SetText("#lb_adm_added: " + addedDate + " #lb_adm_last_modified: " + modDate);
		txt_name.SetText(current.name);
		txt_added_by.SetText("#lb_adm_added_by: " + current.addedBySteamid + " (" + current.addedByName + ")");
		edit_reason.SetText(current.reason);
		int conf = current.confidence * 100;
		edit_confidence.SetText("" + conf + "%");
		edit_message.SetText(current.message);
		return current.steamid;
	}

	void SaveEntry(LBWatchlistEntry entry) {
		if (!entry)
			return;
		bool save = false;
		if (entry.reason != edit_reason.GetText()) {
			entry.reason = edit_reason.GetText();
			save = true;
		}
		float conf = edit_confidence.GetText().ToFloat() / 100.0;
		if (entry.confidence != conf) {
			entry.confidence = conf;
			save = true;
		}
		string mess;
		edit_message.GetText(mess);
		if (entry.message != mess) {
			entry.message = mess;
			save = true;
		}
		if (save) {
			entry.lastModified = LBDate.Init(true).GetTimestamp();
			LBWatchlistBase current = GetCurrentWatchlist();
			if (!current)
				return;
			current.ModifyWatchlistEntry(entry);
		}
	}

	override void Update(float timeslice, bool otherMenu, bool leftClicked) {
		super.Update(timeslice, otherMenu, leftClicked);
		if (LBWatchlistServer_.watchlistUpdateUI) {
			LBWatchlistServer_.watchlistUpdateUI = false;
			if (IsVisible() && GetCurrentWatchlist() == LBWatchlistServer.Get) {
				LoadSelectedWatchlist();
				LBWatchlistBase current = GetCurrentWatchlist();
				int i = 0;
				if (current) {
					foreach (LBWatchlistEntry entry : current.entries) {
						if (entry.steamid == lastSelected) {
							list_entries.SelectRow(i);
							LoadSelectedWatchlistEntry();
							break;
						}
						++i;
					}
				}
			}
		} else if (LBWatchlistGlobal_.watchlistUpdateUI) {
			LBWatchlistGlobal_.watchlistUpdateUI = false;
			if (IsVisible() && GetCurrentWatchlist() == LBWatchlistGlobal.Get) {
				LoadSelectedWatchlist();
				current = GetCurrentWatchlist();
				i = 0;
				if (current) {
					foreach (LBWatchlistEntry entry2 : current.entries) {
						if (entry2.steamid == lastSelected) {
							list_entries.SelectRow(i);
							LoadSelectedWatchlistEntry();
							break;
						}
						++i;
					}
				}
			}
		}
	}

	LBWatchlistEntry GetSelectedWatchlistEntry() {
		LBWatchlistBase current = GetCurrentWatchlist();
		if (!current) {
			return null;
		}
		int row = list_entries.GetSelectedRow();
		if (row < 0 || row >= current.entries.Count())
			return null;
		return current.entries.Get(row);
	}

	void LoadSelectedWatchlist() {
		LBWatchlistBase current = GetCurrentWatchlist();
		if (!current) {
			ClearRemainingEntries(0);
			return;
		}
		if (current == LBWatchlistLocal.Get) {
			btn_add.Show(true);
			btn_save.Show(true);
			btn_del.Show(true);
		} else if (current == LBWatchlistServer.Get) {
			btn_add.Show(LBAdmins.Get().HasPermission("admin.add.watchlist_server"));
			btn_save.Show(LBAdmins.Get().HasPermission("admin.remove.watchlist_server"));
			btn_del.Show(LBAdmins.Get().HasPermission("admin.remove.watchlist_server"));
		} else if (current == LBWatchlistGlobal.Get) {
			btn_add.Show(LBAdmins.Get().HasPermission("admin.add.watchlist_global"));
			btn_save.Show(LBAdmins.Get().HasPermission("admin.remove.watchlist_global"));
			btn_del.Show(LBAdmins.Get().HasPermission("admin.remove.watchlist_global"));
		}
		int i = 0;
		foreach (LBWatchlistEntry entry : current.entries) {
			entry.AddToList(list_entries, i);
			++i;
		}
		ClearRemainingEntries(i);
		LoadSelectedWatchlistEntry();
	}

	void ClearRemainingEntries(int startRow) {
		int selected = list_entries.GetSelectedRow();
		int count = list_entries.GetNumItems();
		for (; count > startRow; --count) {
			list_entries.RemoveRow(count - 1);
		}
		if (selected > startRow) {
			list_entries.SelectRow(startRow - 1);
		}
	}

	LBWatchlistBase GetCurrentWatchlist() {
		int index = combo_type.GetCurrentItem();
		if (index < 0 || index >= allowedWatchlists.Count())
			return null;
		int watchlistEntry = allowedWatchlists.Get(index);
		if (watchlistEntry == 0)
			return LBWatchlistLocal.Get;
		if (watchlistEntry == 1)
			return LBWatchlistServer.Get;
		if (watchlistEntry == 2)
			return LBWatchlistGlobal.Get;
		return null;
	}

	void LoadWatchlistList() {
		allowedWatchlists.Clear();
		combo_type.ClearAll();
		if (LBAdmins.Get().HasPermission("admin.view.watchlist_local")) {
			combo_type.AddItem("#lb_adm_personal");
			allowedWatchlists.Insert(0);
		}
		if (LBAdmins.Get().HasPermission("admin.view.watchlist_server")) {
			combo_type.AddItem("#lb_adm_server");
			allowedWatchlists.Insert(1);
		}
		if (LBAdmins.Get().HasPermission("admin.view.watchlist_global")) {
			combo_type.AddItem("#lb_adm_global");
			allowedWatchlists.Insert(2);
		}
		int index = allowedWatchlists.Find(LBAdminMenuSettings.Get.watchlistLastCategory);
		if (index != -1)
			combo_type.SetCurrentItem(index);
		LoadSelectedWatchlist();
	}

}
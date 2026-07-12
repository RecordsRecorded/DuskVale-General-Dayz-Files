class LBAdminMenuFrame_Banmanager : LBAdminMenuFrame {

	override string GetTitle() {
		return "#lb_adm_title_ban_manager";
	}

	override string GetButtonIcon() {
		return "LBmaster_AdminTools/gui/icons/banhammer.paa";
	}

	override string GetInfoString() {
		if (LBBanManager.Get && LBBanManager.Get.deleteExpiredBans)
			return "#lb_adm_desc_ban_manager_delete_expired";
		else
			return "#lb_adm_desc_ban_manager";
	}

	override void GetMinSize(out int width, out int height) {
		width = 600;
		height = 500;
	}

	override void GetInitialSize(out int width, out int height) {
		width = 1200;
		height = 700;
	}

	override TStringArray GetRequiredPermissionsOne() {
		return {"admin.ban.player", "admin.unban.player", "admin.globalban.player", "admin.globalunban.player"};
	}

	XComboBoxWidget combo_category;
	TextWidget txt_admin, txt_banned_since, txt_remain;
	MultilineEditBoxWidget edit_comment;
	CheckBoxWidget chckbx_perm, chckbx_search_player, chckbx_search_bannedby, chckbx_search_reason, chckbx_search_expired;
	ButtonWidget btn_apply, btn_unban, btn_ban;
	EditBoxWidget edit_weeks, edit_days, edit_hours, edit_min, edit_sec, edit_reason, edit_steamid, edit_name, edit_search;
	TextListboxWidget list_bans;
	Widget panel_duration;

	ref TStringArray categories = new TStringArray();

	ref LBBanEntry newEntry;
	static bool reloadEntries = false;

	void LBAdminMenuFrame_Banmanager() {
		LBConfigManager.Get().GetEventOnConfigReceived("LBBanManager").Insert(LoadEntries);
	}

	override void Init(LBAdminMenuMainButton parent_) {
		super.Init(parent_);
		chckbx_search_player.SetChecked(true);
		chckbx_search_reason.SetChecked(true);
		chckbx_search_expired.SetChecked(true);
	}

	override void Show(bool show) {
		super.Show(show);
		combo_category.ClearAll();
		categories.Clear();
		if (LBAdmins.Get().HasPermission("admin.ban.player")) {
			combo_category.AddItem("#lb_adm_server");
			categories.Insert("#lb_adm_server");
		}
		if (LBAdmins.Get().HasPermission("admin.globalban.player") && LBGlobalBanManager.Get && LBGlobalBanManager.Get.enabled) {
			combo_category.AddItem("#lb_adm_global");
			categories.Insert("#lb_adm_global");
		}
		if (!LBBanManager.Get)
			LBBanManager.Loader.Load();
		if (!LBGlobalBanManager.Get)
			LBGlobalBanManager.Loader.Load();

		if (combo_category.GetNumItems() > LBAdminMenuSettings.Get.banlistLastCategory)
			combo_category.SetCurrentItem(LBAdminMenuSettings.Get.banlistLastCategory);
		LoadEntries();
	}

	override void Update(float timeslice, bool otherMenu, bool leftClicked) {
		super.Update(timeslice, otherMenu, leftClicked);
		if (reloadEntries) {
			reloadEntries = false;
			LoadEntries();
		}
	}

	override bool OnItemSelected(Widget w, int x, int y, int row, int column, int oldRow, int oldColumn) {
		if (super.OnItemSelected(w, x, y, row, column, oldRow, oldColumn))
			return true;
		if (w == list_bans) {
			newEntry = null;
			LoadBan(GetSelectedBan());
			return true;
		}
		return false;
	}

	bool IsEntrySearched(LBBanEntry entry, string search, int now) {
		if (!chckbx_search_expired.IsChecked() && entry.bannedTimestamp + entry.duration - now < 0)
			return false; // Expired never show even when search is empty when checkbox disabled
		if (search == "")
			return true;
		if (chckbx_search_player.IsChecked() && (LBStringTools.ContainsIgnoreCase(entry.steamid, search) || LBStringTools.ContainsIgnoreCase(entry.name, search)))
			return true;
		if (chckbx_search_bannedby.IsChecked() && (LBStringTools.ContainsIgnoreCase(entry.bannedBy, search) || LBStringTools.ContainsIgnoreCase(entry.bannedByName, search)))
			return true;
		if (chckbx_search_reason.IsChecked() && LBStringTools.ContainsIgnoreCase(entry.reason, search))
			return true;
		return false;
	}

	void LoadEntries() {
		string search = edit_search.GetText();
		int row = 0;
		int count = list_bans.GetNumItems();
		int now = LBDate.Init(true).GetTimestamp();
		array<ref LBBanEntry> bans;
		if (categories.Get(combo_category.GetCurrentItem()) == "#lb_adm_server") {
			if (LBBanManager.Get)
				bans = LBBanManager.Get.bans;
		} else {
			if (LBGlobalBanManager.Get)
				bans = LBGlobalBanManager.Get.bans;
		}
		if (!bans) {
			list_bans.ClearItems();
			return;
		}

		foreach (LBBanEntry entry : bans) {
			if (IsEntrySearched(entry, search, now)) {
				if (count <= row) {
					list_bans.AddItem(entry.steamid + " (" + entry.name + ")", entry, 0);
				} else {
					list_bans.SetItem(row, entry.steamid + " (" + entry.name + ")", entry, 0);
				}
				list_bans.SetItem(row, entry.reason, entry, 1);
				int remain = entry.bannedTimestamp + entry.duration - now;
				string remainStr = "";
				if (entry.duration < 0) {
					remainStr = "#lb_adm_permanent";
				} else if (remain < 0) {
					string timeStr = LBDate.Init(-remain).ToDiffString();
					remainStr = LBTranslatedString("#lb_adm_expired_xxx_ago", {"%time%", timeStr}).Get();
				} else {
					remainStr = LBDate.Init(remain).ToDiffString();
				}
				list_bans.SetItem(row, remainStr, entry, 2);
				++row;
			}
		}
		for (int i = list_bans.GetNumItems(); i > row; --i) {
			list_bans.RemoveRow(i - 1);
		}
		if (list_bans.GetSelectedRow() >= list_bans.GetNumItems()) {
			list_bans.SelectRow(list_bans.GetNumItems() - 1);
		}
		if (categories.Get(combo_category.GetCurrentItem()) == "#lb_adm_server") {
			btn_unban.Show(LBAdmins.Get().HasPermission("admin.unban.player"));
			btn_ban.Show(LBAdmins.Get().HasPermission("admin.ban.player"));
		} else {
			btn_unban.Show(LBAdmins.Get().HasPermission("admin.globalunban.player"));
			btn_ban.Show(LBAdmins.Get().HasPermission("admin.globalban.player"));
		}
	}

	LBBanEntry GetSelectedBan() {
		if (newEntry)
			return newEntry;
		int index;
		if (!LBWidgetUtils.CheckSelectedInRange(list_bans, index))
			return null;
		LBBanEntry entry;
		list_bans.GetItemData(index, 0, entry);
		return entry;
	}

	void LoadBan(LBBanEntry entry) {
		if (!entry) {
			return;
		}
		edit_steamid.SetText(entry.steamid);
		edit_name.SetText(entry.name);
		txt_admin.SetText(entry.bannedBy + " (" + entry.bannedByName + ")");
		txt_banned_since.SetText(LBDate.Init(entry.bannedTimestamp).ToFormattedString());
		SetRemainText(entry, entry.duration);
		chckbx_perm.SetChecked(entry.duration < 0);
		panel_duration.Show(!chckbx_perm.IsChecked());
		edit_reason.SetText(entry.reason);
		edit_comment.SetText(entry.comment);
		btn_apply.Show(false);
		btn_apply.SetText("#apply");
		if (entry.duration >= 0)
			LoadDuration(entry.duration);
		bool canEdit = false;
		bool canUnbanAny;
		if (categories.Get(combo_category.GetCurrentItem()) == "#lb_adm_server")
			canUnbanAny = LBAdmins.Get().HasPermission("admin.unban.any_player");
		else
			canUnbanAny = LBAdmins.Get().HasPermission("admin.globalunban.any_player");
		if (entry.bannedBy == "" || entry.bannedBy == LBAdmins.Get().GetMySteamid() || canUnbanAny)
			canEdit = true;
		edit_comment.Enable(canEdit);
		edit_reason.Enable(canEdit);
		edit_weeks.Enable(canEdit);
		edit_days.Enable(canEdit);
		edit_hours.Enable(canEdit);
		edit_min.Enable(canEdit);
		edit_sec.Enable(canEdit);
		chckbx_perm.Enable(canEdit);
		edit_steamid.Enable(entry.bannedBy == "");
		edit_name.Enable(entry.bannedBy == "");
	}

	void SetRemainText(LBBanEntry entry, int duration) {
		int now = LBDate.Init(true).GetTimestamp();
		int remain = entry.bannedTimestamp + duration - now;
		if (duration < 0) {
			txt_remain.SetText("#lb_adm_permanent");
		} else if (remain < 0) {
			string timeStr = LBDate.Init(-remain).ToDiffString();
			txt_remain.SetText(LBTranslatedString("#lb_adm_expired_xxx_ago", {"%time%", timeStr}).Get());
		} else {
			txt_remain.SetText(LBDate.Init(remain).ToDiffString());
		}
	}

	override bool OnClick(Widget w, int x, int y, int button) {
		if (super.OnClick(w, x, y, button))
			return true;
		if (w == chckbx_perm) {
			LBBanEntry selected = GetSelectedBan();
			if (!selected)
				return true;
			if (chckbx_perm.IsChecked()) {
				txt_remain.SetText("#lb_adm_permanent");
				panel_duration.Show(false);
			} else {
				edit_weeks.SetText("0");
				edit_days.SetText("0");
				edit_hours.SetText("0");
				edit_min.SetText("0");
				edit_sec.SetText("0");
				panel_duration.Show(true);
				if (newEntry)
					newEntry.bannedTimestamp = LBDate.Init(true).GetTimestamp();
				SetRemainText(selected, 0);
			}
			btn_apply.Show(true);
			return true;
		} else if (w == btn_ban) {
			list_bans.SelectRow(-1);
			newEntry = new LBBanEntry();
			newEntry.bannedTimestamp = LBDate.Init(true).GetTimestamp();
			LoadBan(newEntry);
			btn_apply.SetText("#lb_adm_ban");
			LBMenuManager.Get().OpenMenu("LBPlayerSelectorDialog", new LBMenuDataPlayerSelector(ScriptCaller.Create(SetBanInfo)), false, parent);
		} else if (w == btn_apply) {
			ScriptRPC rpc = new ScriptRPC();
			selected = GetSelectedBan();
			if (!selected)
				return true;
			selected.steamid = edit_steamid.GetText();
			selected.name = edit_name.GetText();
			if (chckbx_perm.IsChecked()) {
				selected.duration = -1;
			} else {
				selected.duration = GetRemainFromEdit();
			}
			selected.reason = edit_reason.GetText();
			edit_comment.GetText(selected.comment);
			if (categories.Get(combo_category.GetCurrentItem()) == "#lb_adm_server") {
				if (newEntry) {
					rpc.Write(newEntry);
					rpc.Send(null, LBAdminToolRPCs.BAN_PLAYER, true);
					newEntry = null;
				} else {
					rpc.Write(selected);
					rpc.Send(null, LBAdminToolRPCs.CHANGE_BAN_PLAYER, true);
				}
			} else {
				if (newEntry) {
					rpc.Write(newEntry);
					rpc.Send(null, LBAdminToolRPCs.GLOBAL_BANLIST_ADD, true);
					newEntry = null;
				} else {
					rpc.Write(selected);
					rpc.Send(null, LBAdminToolRPCs.GLOBAL_BANLIST_CHANGE, true);
				}
			}
			btn_apply.Show(false);
		} else if (w == btn_unban) {
			rpc = new ScriptRPC();
			selected = GetSelectedBan();
			if (!selected)
				return true;
			rpc.Write(selected);
			if (categories.Get(combo_category.GetCurrentItem()) == "#lb_adm_server")
				rpc.Send(null, LBAdminToolRPCs.UNBAN_PLAYER, true);
			else
				rpc.Send(null, LBAdminToolRPCs.GLOBAL_BANLIST_REMOVE, true);
		} else if (w == chckbx_search_reason) {
			LoadEntries();
			return true;
		} else if (w == chckbx_search_player) {
			LoadEntries();
			return true;
		} else if (w == chckbx_search_expired) {
			LoadEntries();
			return true;
		} else if (w == chckbx_search_bannedby) {
			LoadEntries();
			return true;
		} else if (w == combo_category) {
			LoadEntries();
			list_bans.SelectRow(-1);
			LBAdminMenuSettings.Get.banlistLastCategory = combo_category.GetCurrentItem();
			LBAdminMenuSettings.Loader.Save();
			return true;
		}
		return false;
	}

	void SetBanInfo(PlayerNameSave_ info) {
		if (!newEntry)
			return;
		edit_steamid.SetText(info.GetSteamid());
		edit_name.SetText(info.GetName());
		edit_steamid.Enable(false);
		edit_name.Enable(false);
	}

	void CreateNewBan(string steamid, string name, string reason = "") {
		list_bans.SelectRow(-1);
		newEntry = new LBBanEntry();
		newEntry.bannedTimestamp = LBDate.Init(true).GetTimestamp();
		LoadBan(newEntry);
		edit_reason.SetText(reason);
		edit_steamid.SetText(steamid);
		edit_name.SetText(name);
		edit_steamid.Enable(false);
		edit_name.Enable(false);
	}

	int GetRemainFromEdit() {
		int sec = edit_sec.GetText().ToInt();
		int min = edit_min.GetText().ToInt();
		int hour = edit_hours.GetText().ToInt();
		int day = edit_days.GetText().ToInt();
		int week = edit_weeks.GetText().ToInt();
		return sec + (min * 60) + (hour * 3600) + (day * 86400) + (week * 604800);
	}

	void LoadDuration(int time) {
		edit_weeks.SetText("" + (time / 604800));
		time = time % 604800;
		edit_days.SetText("" + (time / 86400));
		time = time % 86400;
		edit_hours.SetText("" + (time / 3600));
		time = time % 3600;
		edit_min.SetText("" + (time / 60));
		time = time % 60;
		edit_sec.SetText("" + (time % 60));

	}

	override void UpdateSlow() {
		super.UpdateSlow();
		LBBanEntry selected = GetSelectedBan();
		int remain = GetRemainFromEdit();
		if (chckbx_perm.IsChecked())
			remain = -1;
		if (selected && !newEntry)
			SetRemainText(selected, remain);
	}

	override bool OnChange(Widget w, int x, int y, bool finished) {
		if (super.OnChange(w, x, y, finished))
			return true;
		if (w == edit_reason || w == edit_comment) {
			btn_apply.Show(true);
			return true;
		} else if (w == edit_sec || w == edit_min || w == edit_hours || w == edit_days || w == edit_weeks) {
			if (newEntry)
				newEntry.bannedTimestamp = LBDate.Init(true).GetTimestamp();
			btn_apply.Show(true);
			LBBanEntry selected = GetSelectedBan();
			int remain = GetRemainFromEdit();
			if (selected)
				SetRemainText(selected, remain);
			return true;
		} else if (w == edit_search) {
			LoadEntries();
			return true;
		}
		return false;
	}

}
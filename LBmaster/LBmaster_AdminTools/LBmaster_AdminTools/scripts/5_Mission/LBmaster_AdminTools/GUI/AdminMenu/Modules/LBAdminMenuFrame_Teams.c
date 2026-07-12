class LBAdminMenuFrame_Teams : LBAdminMenuFrame {

	bool firstLoad = true;

	TextWidget txt_name;
	CheckBoxWidget chckbx_unique, chckbx_sync, chckbx_color;
	Widget panel_esp_color;
	EditBoxWidget edit_name;
	TextListboxWidget list_teams;
	ButtonWidget btn_new, btn_del, btn_export, btn_export_all, btn_clear_all, btn_clear_offline, btn_esp_color;
	
	ref LBColorPicker picker;

	override string GetTitle() {
		return "#lb_adm_teams";
	}

	override string GetButtonIcon() {
		return "LBmaster_AdminTools/gui/icons/teams.paa";
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
		return {"admin.view.teams", "admin.sync.teams"};
	}

	override string GetInfoString() {
		return "#lb_adm_desc_teams";
	}

	override void UpdateSlow() {
		super.UpdateSlow();
		if (IsVisible())
			UpdateTeamsList(false);
	}
	
	override void OnInit() {
		LBAdminMenuFrame_PlayerList list = LBAdminMenuFrame_PlayerList.Cast(parent.GetFrame("LBAdminMenuFrame_PlayerList"));
		list.Event_Player_Checked.Insert(OnPlayerSelected);
	}

	override bool OnChange(Widget w, int x, int y, bool finished) {
		if (super.OnChange(w, x, y, finished))
			return true;
		if (w == edit_name) {
			LBAdminPlayerTeam team = GetSelectedTeam();
			if (team) {
				if (team.synced && !chckbx_sync.IsVisible()) {
					edit_name.SetText(team.name);
					return true;
				}
				team.name = edit_name.GetText();
				if (team.synced && chckbx_sync.IsVisible())
					g_Game.RPCSingleParam(null, LBAdminToolRPCs.TEAM_RENAME, new Param2<int, string>(team.GetUID(), team.name), true);
				UpdateTeamsList(false);
				LBAdminPlayerTeamsList.Loader.Save();
			}
			return true;
		}
		return false;
	}
	
	override bool OnItemSelected(Widget w, int x, int y, int row, int column, int oldRow, int oldColumn) {
		if (super.OnItemSelected(w, x, y, row, column, oldRow, oldColumn))
			return true;
		bool show = false;
		if (row != 0)
			show = true;
		btn_del.Show(show);
		btn_clear_all.Show(show);
		btn_clear_offline.Show(show);
		chckbx_color.Show(show);
		chckbx_unique.Show(show);
		btn_esp_color.Show(show);
		edit_name.Show(show);
		txt_name.Show(show);
		chckbx_sync.Show(show);
		if (w == list_teams) {
			LoadSelectedTeamInfo(true);
			return true;
		}
		return false;
	}
	
	void OnPlayerSelected(string steamid, bool checked) {
		LBAdminPlayerTeam team = GetSelectedTeam();
		if (!team)
			return;
		if (team.synced && !chckbx_sync.IsVisible())
			return;
		if (checked) {
			team.playerSteamids.Insert(steamid);
			if (team.synced && chckbx_sync.IsVisible())
				g_Game.RPCSingleParam(null, LBAdminToolRPCs.TEAM_MEMBER, new Param3<int, string, bool>(team.GetUID(), steamid, true), true);
			if (team.uniquePlayers) {
				foreach (LBAdminPlayerTeam other : LBAdminPlayerTeamsList.Get.teams) {
					if (other == team)
						continue;
					other.playerSteamids.RemoveItem(steamid);
					if (other.synced && chckbx_sync.IsVisible())
						g_Game.RPCSingleParam(null, LBAdminToolRPCs.TEAM_MEMBER, new Param3<int, string, bool>(other.GetUID(), steamid, false), true);
				}
			}
		} else {
			team.playerSteamids.RemoveItem(steamid);
			if (team.synced && chckbx_sync.IsVisible())
				g_Game.RPCSingleParam(null, LBAdminToolRPCs.TEAM_MEMBER, new Param3<int, string, bool>(team.GetUID(), steamid, false), true);
		}
		LBAdminPlayerTeamsList.Loader.Save();
	}
	
	void DeleteSelectedTeam() {
		int selected;
		if (!LBWidgetUtils.CheckSelectedInRange(list_teams, selected) || selected == 0) {
			return;
		}
		LBAdminPlayerTeam team = LBAdminPlayerTeamsList.Get.teams.Get(selected);
		if (team && team.synced && chckbx_sync.IsVisible())
			g_Game.RPCSingleParam(null, LBAdminToolRPCs.TEAM_DELETE, new Param1<int>(team.GetUID()), true);
		LBAdminPlayerTeamsList.Get.teams.RemoveOrdered(selected);
		LBAdminPlayerTeamsList.Loader.Save();
		UpdateTeamsList(true);
	}
	
	LBAdminPlayerTeam GetSelectedTeam() {
		int selected;
		if (!LBWidgetUtils.CheckSelectedInRange(list_teams, selected) || selected == 0) {
			return null;
		}
		return LBAdminPlayerTeamsList.Get.teams.Get(selected);
	}
	
	void LoadSelectedTeamInfo(bool clearNotFound) {
		LBAdminMenuFrame_PlayerList list = LBAdminMenuFrame_PlayerList.Cast(parent.GetFrame("LBAdminMenuFrame_PlayerList"));
		LBAdminPlayerTeam team = GetSelectedTeam();
		if (!team) {
			edit_name.SetText("");
			if (clearNotFound) {
				list.InvokePlayerSelected(null);
				list.UnselectAll(false);
				list.SelectAll(null, false);
				list.InvokePlayersSelected();
			}
			return;
		}
		chckbx_sync.SetChecked(team.synced);
		edit_name.SetText(team.name);
		chckbx_color.SetChecked(team.overwriteESPColor);
		chckbx_unique.SetChecked(team.uniquePlayers);
		btn_esp_color.Show(team.overwriteESPColor);
		panel_esp_color.SetColor(team.espColor.GetColorARGB());
		list.InvokePlayerSelected(null);
		list.UnselectAll(false);
		list.SelectAll(team.playerSteamids, false);
		list.InvokePlayersSelected();
	}
	
	void UpdateTeamsList(bool updatePlayerList) {
		int i = 0, a = 0;
		array<ref SyncPlayer> onlinePlayers = ClientData.m_PlayerList.m_PlayerList;
		foreach (LBAdminPlayerTeam team : LBAdminPlayerTeamsList.Get.teams) {
			team.onlinePlayers = new array<PlayerIdentity>();
			foreach (SyncPlayer player : onlinePlayers) {
				if (player && player.m_Identity && team.playerSteamids.Find(player.m_Identity.GetPlainId()) != -1) {
					team.onlinePlayers.Insert(player.m_Identity);
				}
			}
			if (list_teams.GetNumItems() <= a)
				list_teams.AddItem(team.name + " (" + team.onlinePlayers.Count() + "/" + team.playerSteamids.Count() + ")", team, 0);
			else
				list_teams.SetItem(a, team.name + " (" + team.onlinePlayers.Count() + "/" + team.playerSteamids.Count() + ")", team, 0);
			list_teams.SetItemColor(a, 0, LBT<int>.T(team.synced, ARGB(255, 100, 255, 100), ARGB(255, 255, 255, 255)));
			++a;
		}
		for (int j = list_teams.GetNumItems() - 1; j >= 0; --j) {
			if (j >= a) {
				list_teams.RemoveRow(j);
			}
		}
		if (list_teams.GetSelectedRow() >= a) {
			list_teams.SelectRow(a - 1);
			list_teams.EnsureVisible(a - 1);
		}
		if (updatePlayerList)
			LoadSelectedTeamInfo(false);
	}
	
	void CreateNewTeam(string name) {
		LBAdminPlayerTeam team = new LBAdminPlayerTeam();
		team.name = name;
		LBAdminPlayerTeamsList.Get.teams.Insert(team);
		UpdateTeamsList(true);
		LBAdminPlayerTeamsList.Loader.Save();
	}
	
	void OnTeamColorChanged(int color) {
		LoadSelectedTeamInfo(false);
	}

	override bool OnClick(Widget w, int x, int y, int button) {
		if (super.OnClick(w, x, y, button))
			return true;
		if (w == btn_new) {
			LBSimpleInputPopup.Get().Show("#lb_create", "#lb_adm_txt_new_team_name", "", ScriptCaller.Create(CreateNewTeam));
			return true;
		} else if (w == chckbx_color) {
			LBAdminPlayerTeam team = GetSelectedTeam();
			if (team) {
				team.overwriteESPColor = chckbx_color.IsChecked();
				btn_esp_color.Show(team.overwriteESPColor);
				LBAdminPlayerTeamsList.Loader.Save();
			}
			return true;
		} else if (w == chckbx_unique) {
			team = GetSelectedTeam();
			if (team) {
				if (team.synced && !chckbx_sync.IsVisible()) {
					chckbx_unique.SetChecked(team.uniquePlayers);
					return true;
				}
				team.uniquePlayers = chckbx_unique.IsChecked();
				LBAdminPlayerTeamsList.Loader.Save();
				if (team.synced && chckbx_sync.IsVisible())
					g_Game.RPCSingleParam(null, LBAdminToolRPCs.TEAM_CONFIG, new Param3<int, string, bool>(team.GetUID(), "unique", team.uniquePlayers), true);
			}
			return true;
		} else if (w == btn_esp_color) {
			team = GetSelectedTeam();
			if (team) {
				picker = new LBColorPicker(w, ScriptCaller.Create(OnTeamColorChanged), -1, team.espColor, true);
			}
			return true;
		} else if (w == btn_del) {
			team = GetSelectedTeam();
			if (team && team.synced && !chckbx_sync.IsVisible())
				return true;
			if (team)
				LBWarningPopup.Get().Show("#lb_delete", LBTranslatedString("#lb_adm_txt_delete_team", {"%name%", team.name}).Get(), ScriptCaller.Create(DeleteSelectedTeam));
			return true;
		} else if (w == btn_clear_all) {
			team = GetSelectedTeam();
			if (!team)
				return true;
			if (team.synced && !chckbx_sync.IsVisible())
				return true;
			team.playerSteamids.Clear();
			team.onlinePlayers.Clear();
			if (team.synced && chckbx_sync.IsVisible())
				g_Game.RPCSingleParam(null, LBAdminToolRPCs.TEAM_MEMBERS, new Param2<int, ref TStringSet>(team.GetUID(), team.playerSteamids), true);
			UpdateTeamsList(true);
			LBAdminPlayerTeamsList.Loader.Save();
		} else if (w == btn_clear_offline) {
			team = GetSelectedTeam();
			if (!team)
				return true;
			if (team.synced && !chckbx_sync.IsVisible())
				return true;
			team.playerSteamids.Clear();
			foreach (PlayerIdentity online : team.onlinePlayers) {
				if (online)
					team.playerSteamids.Insert(online.GetPlainId());
			}
			if (team.synced && chckbx_sync.IsVisible())
				g_Game.RPCSingleParam(null, LBAdminToolRPCs.TEAM_MEMBERS, new Param2<int, ref TStringSet>(team.GetUID(), team.playerSteamids), true);
			UpdateTeamsList(true);
			LBAdminPlayerTeamsList.Loader.Save();
			return true;
		} else if (w == btn_export) {
			CopySelectedTeam();
			return true;
		} else if (w == btn_export_all) {
			CopyAllTeams();
			return true;
		} else if (w == chckbx_sync) {
			team = GetSelectedTeam();
			team.synced = chckbx_sync.IsChecked();
			if (team.synced) {
				g_Game.RPCSingleParam(null, LBAdminToolRPCs.TEAM_SYNC, new Param3<int, bool, ref LBAdminPlayerTeam>(team.GetUID(), true, team), true);
			} else {
				g_Game.RPCSingleParam(null, LBAdminToolRPCs.TEAM_SYNC, new Param3<int, bool, ref LBAdminPlayerTeam>(team.GetUID(), false, null), true);
			}
			LBAdminPlayerTeamsList.Loader.Save();
			return true;
		}
		return false;
	}
	
	void CopySelectedTeam() {
		LBAdminPlayerTeam team = GetSelectedTeam();
		if (!team)
			return;
		JsonSerializer serial = new JsonSerializer();
		string output;
		if (serial.WriteToString(team, true, output)) {
			g_Game.CopyToClipboard(output);
			LBCopiedPopup.ShowAtCursor();
		}
	}
	
	void CopyAllTeams() {
		JsonSerializer serial = new JsonSerializer();
		string output;
		array<ref LBAdminPlayerTeam> teams = LBAdminPlayerTeamsList.Get.teams;
		if (serial.WriteToString(teams, true, output)) {
			g_Game.CopyToClipboard(output);
			LBCopiedPopup.ShowAtCursor();
		}
	}
	
	void OverwriteTeamInfoOrInsert(LBAdminPlayerTeam singleTeam, bool checkSync) {
		bool found = false;
		foreach (LBAdminPlayerTeam team : LBAdminPlayerTeamsList.Get.teams) {
			if (team.GetUID() == singleTeam.GetUID()) {
				team.playerSteamids = singleTeam.playerSteamids;
				team.uniquePlayers = singleTeam.uniquePlayers;
				team.overwriteESPColor = singleTeam.overwriteESPColor;
				team.espColor = singleTeam.espColor;
				found = true;
				if (checkSync && team.synced && !chckbx_sync.IsVisible())
					return;
				team.synced = singleTeam.synced;
				break;
			}
		}
		if (!found) {
			LBAdminPlayerTeamsList.Get.teams.Insert(singleTeam);
		}
		LBAdminPlayerTeamsList.Loader.Save();
		UpdateTeamsList(true);
	}
	
	override void Show(bool show) {
		super.Show(show);
		if (show) {
			if (firstLoad) {
				LBConfigManager.Get().GetEventOnConfigReceived("LBAdminPlayerTeamsListServer").Insert(ServerConfigReceived);
				LBAdminPlayerTeamsListServer.Loader.Load();
				firstLoad = false;
			}
			UpdateTeamsList(true);
			chckbx_sync.Show(LBAdmins.Get().HasPermission("admin.sync.teams"));
		} else {
			picker = null;
		}
	}
	
	void ServerConfigReceived() {
		UpdateTeamsList(true);
	}
	
	override string GetTooltip(Widget w) {
		if (w == btn_new) {
			return "#lb_adm_desc_team_btn_new";
		} else if (w == btn_del) {
			return "#lb_adm_desc_team_btn_del";
		} else if (w == btn_clear_all) {
			return "#lb_adm_desc_team_btn_clear_all";
		} else if (w == btn_clear_offline) {
			return "#lb_adm_desc_team_btn_clear_offline";
		} else if (w == chckbx_unique) {
			return "#lb_adm_desc_team_chckbx_unique";
		} else if (w == chckbx_color) {
			return "#lb_adm_desc_team_chckbx_color";
		} else if (w == btn_esp_color) {
			return "#lb_adm_desc_team_btn_esp_color";
		}
		return super.GetTooltip(w);
	}
	
	LBAdminPlayerTeam FindTeamByUID(int uid) {
		foreach (LBAdminPlayerTeam team : LBAdminPlayerTeamsList.Get.teams) {
			if (team.GetUID() == uid)
				return team;
		}
		return null;
	}
	
	override void OnRPC(Object target, int rpc_type, ParamsReadContext ctx) {
		if (rpc_type == LBAdminToolRPCs.TEAM_SYNC) {
			Param3<int, bool, ref LBAdminPlayerTeam> paramSync;
			if (!ctx.Read(paramSync))
				return;
			if (paramSync.param2) {
				OverwriteTeamInfoOrInsert(paramSync.param3, false);
			} else {
				LBAdminPlayerTeam found = FindTeamByUID(paramSync.param1);
				if (found) {
					found.synced = false;
					UpdateTeamsList(GetSelectedTeam() == found);
					LBAdminPlayerTeamsList.Loader.Save();
				}
			}
			LBESPManager.UpdateColors();
		} else if (rpc_type == LBAdminToolRPCs.TEAM_RENAME) {
			Param2<int, string> paramRename;
			if (!ctx.Read(paramRename))
				return;
			found = FindTeamByUID(paramRename.param1);
			if (found) {
				found.name = paramRename.param2;
				UpdateTeamsList(false);
				LBAdminPlayerTeamsList.Loader.Save();
			}
		} else if (rpc_type == LBAdminToolRPCs.TEAM_MEMBER) {
			Param3<int, string, bool> memberParam;
			if (!ctx.Read(memberParam))
				return;
			found = FindTeamByUID(memberParam.param1);
			if (!found)
				return;
			if (memberParam.param3) {
				found.playerSteamids.Insert(memberParam.param2);
				if (found.uniquePlayers) {
					foreach (LBAdminPlayerTeam other : LBAdminPlayerTeamsList.Get.teams) {
						if (other == found)
							continue;
						other.playerSteamids.RemoveItem(memberParam.param2);
					}
				}
			} else {
				found.playerSteamids.RemoveItem(memberParam.param2);
			}
			if (GetSelectedTeam() == found || found.uniquePlayers)
				UpdateTeamsList(true);
			else
				UpdateTeamsList(false);
			LBAdminPlayerTeamsList.Loader.Save();
			LBESPManager.UpdateColors();
		} else if (rpc_type == LBAdminToolRPCs.TEAM_MEMBERS) {
			Param2<int, ref TStringSet> membersParam;
			if (!ctx.Read(membersParam))
				return;
			found = FindTeamByUID(membersParam.param1);
			if (!found)
				return;
			found.playerSteamids = membersParam.param2;
			UpdateTeamsList(GetSelectedTeam() == found);
			LBAdminPlayerTeamsList.Loader.Save();
			LBESPManager.UpdateColors();
		} else if (rpc_type == LBAdminToolRPCs.TEAM_CONFIG) {
			Param3<int, string, bool> configParam;
			if (!ctx.Read(configParam))
				return;
			found = FindTeamByUID(configParam.param1);
			if (!found)
				return;
			if (configParam.param2 == "unique") {
				found.uniquePlayers = configParam.param3;
				UpdateTeamsList(GetSelectedTeam() == found);
				LBAdminPlayerTeamsList.Loader.Save();
			}
			
		} else if (rpc_type == LBAdminToolRPCs.TEAM_DELETE) {
			Param1<int> deleteParam;
			if (!ctx.Read(deleteParam))
				return;
			found = FindTeamByUID(deleteParam.param1);
			if (!found)
				return;
			LBAdminPlayerTeamsList.Get.teams.RemoveItem(found);
			UpdateTeamsList(true);
			LBAdminPlayerTeamsList.Loader.Save();
			LBESPManager.UpdateColors();
		}
	}

}
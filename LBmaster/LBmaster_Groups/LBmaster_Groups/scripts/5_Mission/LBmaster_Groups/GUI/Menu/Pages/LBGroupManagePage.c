class LBGroupManagePage : LBGroupPage {

	EditBoxWidget searchbox;
	ButtonWidget buttonInvite;
	ButtonWidget btn_kick, btn_leave, btn_upgrade, btn_promote, btn_demote, btn_joinSubgroup, buttonInvite_alt, btn_change_tag;
	TextWidget txt_groupname;
	TextListboxWidget playerlist_online;
	Widget playerlist_members, panel_list;
	private ref LBGroupManagerPlayerList playerListManager;

	void LBGroupManagePage(LBPageSettingsType type_) {
		LBLayoutConfig_.Event_StreamerModeChanged.Insert(OnStreamerModeChange);
	}

	void ~LBGroupManagePage() {
		if (LBLayoutConfig_.Event_StreamerModeChanged)
			LBLayoutConfig_.Event_StreamerModeChanged.Remove(OnStreamerModeChange);
	}

	void OnStreamerModeChange(bool enabled) {
		playerlist_online.Show(!enabled);
		txt_groupname.Show(!enabled);
	}

	override bool InitPage(LBGroupUI parentUI) {
		bool worked = super.InitPage(parentUI, 1, 1, LBPagesConfig.Get.GetButtonText(pageType), false, LBPagesConfig.Get.GetPagePriority(pageType));
		if (buttonWidget)
			parentUI.groupButton = buttonWidget;
		return worked;
	}

	override bool OnTopButtonClicked(Widget w) {
		if (super.OnTopButtonClicked(w)) {
			PlayerBase pb = PlayerBase.Cast(g_Game.GetPlayer());
			if (!pb)
				return false;
			return pb.GetLBGroup() != null;
		}
		return false;
	}

	override bool CanDisplayButton() {
		return super.CanDisplayButton() && LBGroupLevels.Get.HasLevelZero();
	}

	override void OnShow() {
		super.OnShow();
		PlayerBase pb = PlayerBase.Cast(g_Game.GetPlayer());
		if (txt_groupname && pb)
			txt_groupname.SetText(pb.GetLBGroup().name + " (" + pb.GetLBGroup().members.Count() + "/" + pb.GetLBGroup().maxPlayers + ")");
		CreatePlayerListManager();
		FillOnlinePlayersList();
		FillMembersList();
		OnStreamerModeChange(LBLayoutConfig.Get.streamerModeEnabled);
		buttonInvite_alt.Show(LBGroupSettingsConfig.Get.inviteActionEnabled);

	}

	override void OnUpdateSlow() {
		FillOnlinePlayersList();
	}

	override bool OnClick(Widget w) {
		PlayerBase pb;
		if (w == buttonInvite) {
			Param1<string> steamIdParam;
			int row = playerlist_online.GetSelectedRow();
			if (row < 0 || row >= playerlist_online.GetNumItems())
				return true;
			playerlist_online.GetItemData(row, 0, steamIdParam);
			if (!steamIdParam)
				return true;
			string steamid = steamIdParam.param1;
			pb = PlayerBase.Cast(g_Game.GetPlayer());
			if (!pb || !pb.GetLBGroup())
				return true;
			pb.GetLBGroup().SendPlayerInviteClient(steamid);
			return true;
		} else if (w == buttonInvite_alt) {
			ActionInvitePlayerToGroup.EnableInviteAction();
			NotificationSystem.AddNotificationExtended(8.0, "#lb_message_group_system", "#lb_ag_info_invite_instructions", LBIconConfig.Get.info);
			parent.CloseMe();
			return true;
		} else if (w == btn_joinSubgroup) {
			JoinSubGroup(GetSelectedSubGroup());
			return true;
		} else if (w == btn_leave) {
			if (LBGroupTimeoutConfig.Get.enableTimeout) {
				if (LBGroupTimeoutConfig.Get.triggerOnLeave) {
					LBDate format = LBDate.Init(LBGroupTimeoutConfig.Get.timeoutDifferentGroupDurationSeconds);
					LBWarningPopup.Get().Show("#lb_leave", "#ag_warn_group_leave \nBefore joining another group you'll have to wait:\n" + format.ToDiffString(), ScriptCaller.Create(LeaveCurrentGroup));
					return true;
				} else {
					g_Game.RPCSingleParam(null, LBGroupRPCs.GROUP_LEAVE_TIMEOUTS, new Param1<bool>(true), true);
				}
			}
			LBWarningPopup.Get().Show("#lb_leave", "#ag_warn_group_leave", ScriptCaller.Create(LeaveCurrentGroup));
			return true;
		} else if (w == btn_promote) {
			PromoteSelectedPlayer();
			return true;
		} else if (w == btn_demote) {
			DemoteSelectedPlayer();
			return true;
		} else if (w == btn_kick) {
			LBWarningPopup.Get().Show("#lb_kick", "#lb_ag_kick_confirm", ScriptCaller.Create(KickSelectedPlayer));
			return true;
		} else if (w == btn_upgrade) {
			pb = PlayerBase.Cast(g_Game.GetPlayer());
			if (!pb || !pb.GetLBGroup())
				return true;
			string upgradeBenefits = GetUpgradeBenefits();
			LBWarningPopup.Get().Show("#lb_upgrade", LBTranslatedString("#lb_ag_upgrade_confirm" + upgradeBenefits, {"%cost%", LBCurrencyConfig.Get.GetFormattedMoneyString(pb.GetLBGroup().GetUpgradeCost())}).Get(), ScriptCaller.Create(UgradeGroup));
			return true;
		} else if (w == btn_change_tag) {
			parent.changeTagDialog.ShowPopup();
			return true;
		}
		return false;
	}

	string GetUpgradeBenefits() {
		if (!LBGroupSettingsConfig.Get.showLevelUpgradeBenefits)
			return "";
		string output = "\n#lb_ag_upgrade_benefits";
		PlayerBase pb = PlayerBase.Cast(g_Game.GetPlayer());
		LBGroupLevel current = pb.GetLBGroup().GetGroupLevel();
		if (!current)
			return "";
		LBGroupLevel next = LBGroupLevels.Get.GetNextLevel(current);
		if (!next)
			return "";
		output = output + "\n#lb_ag_upgrade_level " + current.level + " -> " + next.level;
		output = output + "\n#lb_ag_upgrade_max_players " + current.maxPlayerCount + " -> " + next.maxPlayerCount;
		if (LBGroupSettingsConfig.Get.enableSubGroups) {
			output = output + "\n#lb_ag_upgrade_subgroups " + current.subgroups.Count() + " -> " + next.subgroups.Count();
		}
		int markers = LBMarkerSettingsConfig.Get.groupMarkerLimit;
		output = output + "\n#lb_ag_upgrade_max_markers " + (markers + current.groupMarkerLimitAdded) + " -> " + (markers + next.groupMarkerLimitAdded);
		int pings = LBMarkerSettingsConfig.Get.tacticalPingMaxConcurrentMarkersPerPlayer;
		output = output + "\n#lb_ag_upgrade_max_pings " + (pings + current.groupPingsLimitAdded) + " -> " + (pings + next.groupPingsLimitAdded);
#ifdef LBmaster_GroupDLCPlotpole
		int plotpoles = LBTerritoryConfig.Get.defaultPlotpoleCountPerGroup;
		if (LBTerritoryConfig.Get.enablePlotpoleExtension)
			output = output + "\n#lb_ag_upgrade_max_plotpoles " + (plotpoles + current.groupPlotpolesLimitAdded) + " -> " + (plotpoles + next.groupPlotpolesLimitAdded);
#endif
		return output;
	}

	void UgradeGroup() {
		PlayerBase pb = PlayerBase.Cast(g_Game.GetPlayer());
		if (!pb || !pb.GetLBGroup())
			return;
		pb.GetLBGroup().UpgradeGroupClient();
	}

	override void OnRPC(int type, ParamsReadContext ctx) {
		super.OnRPC(type, ctx);
		if (type == LBGroupRPCs.GROUP_LEAVE_TIMEOUTS) {
			Param1<int> times;
			if (!ctx.Read(times))
				return;
			if (!LBWarningPopup.Get().IsVisible())
				return;
			LBDate format = LBDate.Init(times.param1);
			LBWarningPopup.Get().Show("#lb_leave", "#ag_warn_group_leave \nBefore joining another group you'll have to wait:\n" + format.ToDiffString(), ScriptCaller.Create(LeaveCurrentGroup));
		}
	}
	
	void LeaveCurrentGroup() {
		LBLogger.Verbose("Leaving current group...", "AdvancedGroups");
		PlayerBase pb = PlayerBase.Cast(g_Game.GetPlayer());
		if (!pb || !pb.GetLBGroup())
			return;
		pb.GetLBGroup().LeaveGroupClient();
	}

	override bool OnChange(Widget w) {
		if (w == searchbox) {
			FillOnlinePlayersList();
			return true;
		}
		return false;
	}
	override bool OnItemSelected(Widget w, int row, int column) {
		if (w == playerlist_online) {
			UpdateInviteButton();
			return true;
		}
		return false;
	}

	override void OnSizeChange() {
		if (playerListManager)
			playerListManager.OnSizeChange();
	}

	void UpdateInviteButton() {
		int row = playerlist_online.GetSelectedRow();
		if (row < 0 || row >= playerlist_online.GetNumItems()) {
			buttonInvite.Enable(false);
			return;
		}
		PlayerBase pb = PlayerBase.Cast(g_Game.GetPlayer());
		if (!pb || !pb.GetLBGroup()) {
			buttonInvite.Enable(true);
			return;
		}
		LBGroupPermission myPerm = pb.GetPermission();
		if (!myPerm || !myPerm.canInvite) {
			buttonInvite.Enable(false);
			buttonInvite_alt.Enable(false);
			return;
		}
		buttonInvite_alt.Enable(true);
		buttonInvite.Enable(true);
	}

	void UpdateGroupButtons() {
		MissionGameplay mission = MissionGameplay.Cast(g_Game.GetMission());
		PlayerBase pb = PlayerBase.Cast(g_Game.GetPlayer());
		if (!mission || !pb)
			return;
		LBGroupPermission myPerms = pb.GetPermission();
		if (!myPerms)
			return;
		if (btn_change_tag)
			btn_change_tag.Enable(myPerms.CanChangeClanTagColor());
		btn_upgrade.Enable(myPerms.canUpgrade);
		int selected = playerListManager.GetSelectedRow();
		btn_joinSubgroup.Enable(false);
		btn_upgrade.Enable(pb.GetLBGroup().GetUpgradeCost() >= 0);
		if (selected < 0 || selected >= playerListManager.GetItemCount()) {
			//LBLogger.Verbose("UpdateGroupButtons Out of Range", "AdvancedGroups");
			btn_kick.Enable(false);
			btn_promote.Enable(false);
			btn_demote.Enable(false);
		} else {
			LBGroupMember member = playerListManager.GetItemData(selected);
			if (!member) {
				//LBLogger.Verbose("UpdateGroupButtons No valid Member: " + member, "AdvancedGroups");
				int mySubgroup = pb.GetMyGroupMarker().currentSubgroup;
				GroupManagerPlayerListEntry entry = playerListManager.playerEntries.Get(selected);
				int subgroupIndex = entry.param4;
				int count = pb.GetLBGroup().GetSubgroupMemberCount(subgroupIndex);
				btn_joinSubgroup.Enable(count < pb.GetLBGroup().subgroups.Get(subgroupIndex).size && subgroupIndex != mySubgroup);
				btn_kick.Enable(false);
				btn_promote.Enable(false);
				btn_demote.Enable(false);
			} else if (member.steamid == LBAdmins.Get().GetMySteamid()) {
				//LBLogger.Verbose("UpdateGroupButtons Selected Me", "AdvancedGroups");
				btn_kick.Enable(false);
				btn_promote.Enable(false);
				btn_demote.Enable(false);
			} else {
				LBGroupPermission targetPerm = member.GetPermission();
				//LBLogger.Verbose("UpdateGroupButtons Checking Target: " + targetPerm.permName + " My Perm: " + myPerms.permName, "AdvancedGroups");
				btn_kick.Enable(myPerms.CanKick(targetPerm));
				btn_promote.Enable(myPerms.CanPromote(targetPerm));
				btn_demote.Enable(myPerms.CanDemote(targetPerm));
			}
		}
	}

	override void OnGroupChanged() {

	}

	void FillOnlinePlayersList() {
		if (LBGroupSettingsConfig.Get.inviteActionEnabled) {
			playerlist_online.ClearItems();
			panel_list.Show(false);
			return;
		} else {
			panel_list.Show(true);
		}

		LBLogger.Debug("Filling Online Players List...", "AdvancedGroups");
		if (!ClientData.m_PlayerList || !ClientData.m_PlayerList.m_PlayerList)
			return;
		array<ref SyncPlayer> list = ClientData.m_PlayerList.m_PlayerList;
		int items = playerlist_online.GetNumItems();
		int added = 0;
		for (int i = 0; i < list.Count(); i++) {
			SyncPlayer player = list.Get(i);
			string name = player.m_PlayerName;
			string steamid = player.m_UID;
			bool you = false;
			if (LBGroupSettingsConfig.Get.groupManagePageObfuscatePlayernames) {
				name = LBPlayerNameObfuscation.GetObfuscatedName(name, steamid);
				if (steamid == LBAdmins.Get().GetMySteamid()) {
					name = "#lb_ag_you " + name;
					you = true;
				}
			}
			Param1<string> param = new Param1<string>(steamid);
			if (IsSearched(name)) {
				if (you) {
					playerlist_online.AddItem(" " + name, param, 0, 0); // Begin
				} else if (items <= added) {
					playerlist_online.AddItem(" " + name, param, 0); // End after last Element
				} else {
					playerlist_online.SetItem(added, " " + name, param, 0); // Middle to last Element
				}
				added++;
			}
		}
		LBLogger.Debug("Added: " + added + " Items: " + items, "AdvancedGroups");
		while (added < playerlist_online.GetNumItems()) {
			playerlist_online.RemoveRow(added);
		}
		LBLogger.Debug("Left: " + playerlist_online.GetNumItems(), "AdvancedGroups");
	}

	bool IsSearched(string name) {
		string lowerName = name + "";
		lowerName.ToLower();
		string lowerSearch = searchbox.GetText();
		lowerSearch.ToLower();
		if (lowerSearch.Length() == 0)
			return true;
		return lowerName.IndexOf(lowerSearch) != -1;
	}

	void FillMembersList() {
		PlayerBase pb = PlayerBase.Cast(g_Game.GetPlayer());
		if (!pb || !pb.GetLBGroup())
			return;
		LBGroup grp = pb.GetLBGroup();
		int lastSubgroup = 0;
		array<ref LBGroupMember> sortedMembers = SortGroupMembers(grp);
		int index = 0;
		foreach (LBGroupMember member : sortedMembers) {
			string addName = "";
			Param3<string, ref LBGroupPermission, int> param;
			if (LBGroupSettingsConfig.Get.enableSubGroups) {
				while (member.currentSubgroup >= lastSubgroup) {
					param = new Param3<string, ref LBGroupPermission, int>("", null, lastSubgroup);
					int inCount = grp.GetSubgroupMemberCount(lastSubgroup);
					int subSize = grp.subgroups.Get(lastSubgroup).size;
					string subName = LBGroupSettingsConfig.Get.subGroupNames.Get(lastSubgroup);
					if (grp.subgroups.Get(lastSubgroup).nameOverride != "")
						subName = grp.subgroups.Get(lastSubgroup).nameOverride;
					if (!grp.subgroups.Get(lastSubgroup).offlineOnly)
						playerListManager.SetEntry(" " + subName + " (" + inCount + "/" + subSize + ")", index, ARGB(255, 255, 255, 255));
					else
						playerListManager.SetEntry(" " + subName + " (" + inCount + "/" + subSize + ")", index, ARGB(255, 255, 100, 100));
					lastSubgroup++;
					index++;
				}
				addName = "  ";
			}
			playerListManager.SetEntry(member, index);
			index++;
		}
		if (LBGroupSettingsConfig.Get.enableSubGroups) {
			while (grp.subgroups.Count() > lastSubgroup) {
				subSize = grp.subgroups.Get(lastSubgroup).size;
				subName = LBGroupSettingsConfig.Get.subGroupNames.Get(lastSubgroup);
				if (grp.subgroups.Get(lastSubgroup).nameOverride != "")
					subName = grp.subgroups.Get(lastSubgroup).nameOverride;
				if (!grp.subgroups.Get(lastSubgroup).offlineOnly)
					playerListManager.SetEntry(" " + subName + " (0/" + subSize + ")", index, ARGB(255, 255, 255, 255));
				else
					playerListManager.SetEntry(" " + subName + " (0/" + subSize + ")", index, ARGB(255, 255, 100, 100));
				lastSubgroup++;
				index++;
			}
		}
		playerListManager.RemoveLastEntries(index);
		UpdateGroupButtons();
	}

	array<ref LBGroupMember> SortGroupMembers(LBGroup grp) {
		if (!LBGroupSettingsConfig.Get.enableSubGroups)
			return grp.members;
		array<ref LBGroupMember> mem = new array<ref LBGroupMember>();
		int needed = grp.members.Count();
		int i = 0;
		while (mem.Count() < needed) {
			array<ref LBGroupMember> subgrpMembers = grp.GetSubgroupMembers(i++);
			foreach (LBGroupMember member : subgrpMembers)
				mem.Insert(member);
			if (i > LBGroupSettingsConfig.Get.subGroupNames.Count())
				break;
		}
		return mem;
	}

	void PromoteSelectedPlayer() {
		string steamid = GetSelectedPlayerSteamid();
		PlayerBase pb = PlayerBase.Cast(g_Game.GetPlayer());
		if (!pb || !pb.GetLBGroup())
			return;
		pb.GetLBGroup().PromotePlayerClient(steamid);
	}

	void DemoteSelectedPlayer() {
		string steamid = GetSelectedPlayerSteamid();
		PlayerBase pb = PlayerBase.Cast(g_Game.GetPlayer());
		if (!pb || !pb.GetLBGroup())
			return;
		pb.GetLBGroup().DemotePlayerClient(steamid);
	}

	void KickSelectedPlayer() {
		string steamid = GetSelectedPlayerSteamid();
		PlayerBase pb = PlayerBase.Cast(g_Game.GetPlayer());
		if (!pb || !pb.GetLBGroup())
			return;
		pb.GetLBGroup().KickPlayerClient(steamid);
	}

	void JoinSubGroup(int index) {
		if (index < 0)
			return;
		PlayerBase pb = PlayerBase.Cast(g_Game.GetPlayer());
		if (!pb || !pb.GetLBGroup() || pb.GetMySubGroup() == index)
			return;
		pb.GetLBGroup().JoinSubgroupRequest(index);
	}

	void MovePlayerToSubgroup(int markerUid, int index) {
		if (index < 0)
			return;
		PlayerBase pb = PlayerBase.Cast(g_Game.GetPlayer());
		if (!pb || !pb.GetLBGroup())
			return;
		pb.GetLBGroup().MoveSubgroupRequest(markerUid, index);

	}

	string GetSelectedPlayerSteamid() {
		int row = playerListManager.GetSelectedRow();
		if (row < 0 || row >= playerListManager.GetItemCount())
			return "";
		LBGroupMember member = playerListManager.GetItemData(row);
		if (member)
			return member.steamid;
		return "";
	}

	int GetSelectedSubGroup() {
		int row = playerListManager.GetSelectedRow();
		if (row < 0 || row >= playerListManager.GetItemCount())
			return -1;
		return playerListManager.GetSubgroup(row);
	}

	override void OnUpdateFrame() {
		super.OnUpdateFrame();
		FillMembersList();
	}

	override void InitMainWidget() {
		LBLogger.Verbose("Init Main Widget for Group Manager Page", "AdvancedGroups");
		ConnectClassWidgetVariables(this, rootWidget, {"rootWidget", "buttonWidget"});
	}

	void CreatePlayerListManager() {
		if (playerListManager)
			playerListManager.ClearEntries();
		playerListManager = new LBGroupManagerPlayerList(playerlist_members, this);
	}
}
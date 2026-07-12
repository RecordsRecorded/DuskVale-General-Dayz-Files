[DayZGame.RegisterLBAdminPage("LBGroupAdminPage", "LBmaster_Groups/logo/logo.paa", "#lb_ag_admin_menu_groups", "groups.view.groups")]
class LBGroupAdminPage : LBAdmin_Menu_Page {

	private ref array<ref LBGroup> groups = new array<ref LBGroup>();

	TextWidget level, created, lastactive, playercount, markercount, steamid, rank;
	CheckBoxWidget chckbxShortnames, chckbxNames, chckbxMemberNames, chckbxSteamids, chckbx_hasCustomTag, chckbx_show_territorry_flags;
	Widget chatColorPreview;
	MapWidget mapWidget;
	EditBoxWidget searchInput, shortname, name;
	TextListboxWidget groupsList, memberList;
	ButtonWidget btnRefresh, btnChangeGroupNames, btnLevelUp, btnLevelDown, btnJoin, btnDelete, btnApplyColor, btn_change_chat_color, btnDemote, btnPromote;
	ButtonWidget btnKick, btnToLeader, btnCopy;

	const string FLAG_POSITION_ICON = "LBmaster_Groups\\gui\\icons\\flag.paa";
	const int FLAG_POSITION_COLOR = ARGB(255, 255, 255, 255);
	ref array<ref Param2<ref vector, string>> flagPositions = new array<ref Param2<ref vector, string >> ();

	override void OnShow() {
		super.OnShow();
		RequestGroups();
	}
	override void OnHide() {
		super.OnHide();
		ClearGroups();
	}

	override array<string> GetEditedConfigs() {}

	override void RegisterAllLinkedVars() {
		linked.RegisterLinkedList("groups", groupsList).SetSearchBar(searchInput, {chckbxShortnames, chckbxNames, chckbxMemberNames, chckbxSteamids}).SetListManager(new LBListManagerSearch<LBGroup>());
		linked.RegisterLinkedVar("groups[groupsList].shortname", shortname).SetLoadOnly(true);
		linked.RegisterLinkedVar("groups[groupsList].name", name).SetLoadOnly(true);
		linked.RegisterLinkedVar("groups[groupsList].level", level);
		linked.RegisterLinkedVar("groups[groupsList].GetLastActiveDate()", lastactive);
		linked.RegisterLinkedVar("groups[groupsList].GetCreatedDate()", created);
		linked.RegisterLinkedVar("groups[groupsList].GetPlayerCountString()", playercount);
		linked.RegisterLinkedVar("groups[groupsList].GetMarkerCountString()", markercount);
		linked.RegisterLinkedVar("groups[groupsList].hasCustomChatColor", chckbx_hasCustomTag);
		linked.RegisterLinkedVar("groups[groupsList].chatColor", btn_change_chat_color).EnableColorPicker(false, "", "R", "G", "B", chatColorPreview);
		linked.RegisterLinkedList("groups[groupsList].members", memberList).SetListManager(new LBListManager<LBGroupMember>()).SetLoadFunctions("ClearMarkers", "OnGroupLoad");
		linked.RegisterLinkedVar("groups[groupsList].members[memberList].steamid", steamid);
		linked.RegisterLinkedVar("groups[groupsList].members[memberList].GetRankString()", rank);

		ApplyWidgetPermission("btnDelete", "groups.delete.group");
		ApplyWidgetPermission("btnJoin", "groups.join.group");
		ApplyWidgetPermission("btnKick", "groups.kick.player");
		ApplyWidgetPermission("btnLevelUp", "groups.change.grouplevel");
		ApplyWidgetPermission("btnLevelDown", "groups.change.grouplevel");
		ApplyWidgetPermission("btnChangeGroupNames", "groups.change.groupname");
		ApplyWidgetPermission("btnDemote", "groups.change.ranks");
		ApplyWidgetPermission("btnPromote", "groups.change.ranks");
		ApplyWidgetPermission("btnToLeader", "groups.change.ranks");
		ApplyWidgetPermission("chckbx_hasCustomTag", "groups.change.tagcolor");
		ApplyWidgetPermission("btnApplyColor", "groups.change.tagcolor");
		ApplyWidgetPermission("sliderR", "groups.change.tagcolor");
		ApplyWidgetPermission("sliderG", "groups.change.tagcolor");
		ApplyWidgetPermission("sliderB", "groups.change.tagcolor");

		chckbxShortnames.SetChecked(true);
		chckbxNames.SetChecked(true);
	}

	LBGroupMember GetSelectedGroupMember() {
		LBGroup selectedGroup = GetSelectedGroup();
		if (!selectedGroup)
			return null;
		int row = memberList.GetSelectedRow();
		if (row < 0 || row >= selectedGroup.members.Count())
			return null;
		return selectedGroup.members.Get(row);
	}

	LBGroup GetSelectedGroup() {
		int row = linked.SearchedIndexToListIndex(groupsList, groupsList.GetSelectedRow());
		if (row < 0 || row >= groups.Count())
			return null;
		return groups.Get(row);
	}

	override void OnRPC(Object target, int rpc_type, ParamsReadContext ctx) {
		if (rpc_type == LBGroupRPCs.GROUP_ADMIN_LIST) {
			OnAllGroupsReceived(ctx);
		} else if (rpc_type == LBGroupRPCs.GROUP_ADMIN_LIST_ADD) {
			OnAllGroupsReceivedAdd(ctx);
		} else if (rpc_type == LBGroupRPCs.GROUP_ADMIN_LIST_SINGLE) {
			OnGroupUpdateReceived(ctx);
		} else if (rpc_type == LBGroupRPCs.GROUP_ADMIN_DELETE) {
			OnGroupDeleteReceived(ctx);
		} else if (rpc_type == LBGroupRPCs.GROUP_ADMIN_FLAGS) {
			OnFlagpolesReceived(ctx);
		}

	}

	void OnFlagpolesReceived(ParamsReadContext ctx) {
		array<ref Param2<ref vector, string>> arr;
		if (!ctx.Read(arr))
			return;
		LBLogger.Debug("Read " + arr.Count() + " Flag Positions", "AdvancedGroups");
		SetFlagPositions(arr);
		ShowTerritorryCheckbox();
	}

	void ShowTerritorryCheckbox() {
		chckbx_show_territorry_flags.Show(true);
	}

	void OnAllGroupsReceived(ParamsReadContext ctx) {
		LBLogger.Debug("OnAllGroupsReceived " + g_Game.GetTime(), "AdvancedGroups");
		ClearGroups();
		OnAllGroupsReceivedAdd(ctx);
	}

	void OnAllGroupsReceivedAdd(ParamsReadContext ctx) {
		LBLogger.Debug("OnAllGroupsReceived " + g_Game.GetTime(), "AdvancedGroups");
		if (!layoutRoot.IsVisible())
			return;
		int count = 0;
		if (!ctx.Read(count))
			return;
		for (int i = 0; i < count; i++) {
			LBGroup grp = new LBGroup();
			bool exists = false;
			if (!ctx.Read(exists))
				return;
			if (!exists)
				continue;
			if (!grp.ReadGroupFromCtx(ctx))
				return;
			groups.Insert(grp);
			grp.RemoveMarkersForAdminPage();
		}
		linked.TriggerLoad("groups");
	}

	void ClearGroups() {
		groups.Clear();
	}

	void OnGroupUpdateReceived(ParamsReadContext ctx) {
		LBLogger.Debug("OnGroupUpdateReceived " + g_Game.GetTime(), "AdvancedGroups");
		LBGroup grp = new LBGroup();
		string oldShortname;
		if (!ctx.Read(oldShortname))
			return;
		if (!grp.ReadGroupFromCtx(ctx))
			return;
		bool found = false;
		for (int i = 0; i < groups.Count(); i++) {
			if (groups.Get(i).shortname == oldShortname) {
				groups.Set(i, grp);
				found = true;
				break;
			}
		}
		if (!found)
			groups.Insert(grp);
		linked.TriggerLoad("groups");
	}

	void SetFlagPositions(array<ref Param2<ref vector, string>> arr) {
		flagPositions = arr;
		DisplayFlagPositions();
	}

	void OnGroupLoad() {
		mapWidget.ClearUserMarks();
		LBGroup grp = GetSelectedGroup();
		if (!grp)
			return;
		MarkMemberPositions(grp);
		DisplayGroupMarkers(grp);
	}

	void DisplayGroupMarkers(LBGroup grp) {
		foreach (LBMarker marker : grp.markers) {
			mapWidget.AddUserMark(marker.position, marker.name, marker.GetColorARGB(), marker.icon);
		}
	}

	void DisplayFlagPositions() {
		if (!chckbx_show_territorry_flags.IsChecked()) {
			OnGroupLoad();
			return;
		}
		mapWidget.ClearUserMarks();
		foreach (Param2<ref vector, string> param : flagPositions) {
			mapWidget.AddUserMark(param.param1, param.param2, FLAG_POSITION_COLOR, FLAG_POSITION_ICON);
		}
	}

	void OnGroupDeleteReceived(ParamsReadContext ctx) {
		LBLogger.Debug("OnGroupDeleteReceived " + g_Game.GetTime(), "AdvancedGroups");
		Param1<string> shortnameParam;
		if (!ctx.Read(shortnameParam))
			return;
		string shortname_ = shortnameParam.param1;
		for (int i = 0; i < groups.Count(); i++) {
			if (groups.Get(i).shortname == shortname_) {
				groups.RemoveOrdered(i);
				break;
			}
		}
		linked.TriggerLoad("groups");
	}

	void ClearMarkers() {
		mapWidget.ClearUserMarks();
	}

	void MarkMemberPositions(LBGroup grp) {
		foreach (LBGroupMember member : grp.members) {
			mapWidget.AddUserMark(member.position, member.name, member.GetColorARGB(), member.icon);
		}
	}

	void ChangeGroupName(LBGroup grp) {
		string newName = name.GetText();
		string newShortname = shortname.GetText();
		if (newName == grp.name && newShortname == grp.shortname)
			return;
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(grp.shortname);
		rpc.Write(newShortname);
		rpc.Write(newName);
		rpc.Send(null, LBGroupRPCs.GROUP_ADMIN_NAMES, true);
	}

	void OnMemberSelected(LBGroupMember member) {
		steamid.SetText(member.steamid);
		LBGroupPermission perm = LBGroupPermissions.Get.FindPermissionGroupByUID(member.permissionGroup);
		rank.SetText(perm.permName);
	}

	void AddGroupLevel(int level_) {
		LBGroup grp = GetSelectedGroup();
		LBLogger.Verbose("Adding " + level_ + " level to " + grp, "AdvancedGroups");
		if (!grp)
			return;
		if (level_ == 0)
			return;
		level_ += grp.level;
		if (level_ < 0)
			return;
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(grp.shortname);
		rpc.Write(level_);
		rpc.Send(null, LBGroupRPCs.GROUP_ADMIN_LEVEL, true);
	}

	override bool OnClick(Widget w, int x, int y, int button) {
		super.OnClick(w, x, y, button);

		if (w == btnChangeGroupNames) {
			ChangeGroupName(GetSelectedGroup());
			return true;
		} else if (w == btnLevelDown) {
			AddGroupLevel(-1);
			return true;
		} else if (w == btnLevelUp) {
			AddGroupLevel(1);
			return true;
		} else if (w == btnRefresh) {
			ClearGroups();
			RequestGroups();
			return true;
		} else if (w == btnJoin) {
			ScriptRPC rpc = new ScriptRPC();
			LBGroup grp = GetSelectedGroup();
			if (grp) {
				rpc.Write(grp.shortname);
			} else {
				return true;
			}
			rpc.Send(null, LBGroupRPCs.GROUP_ADMIN_JOIN, true);
			return true;
		} else if (w == btnDelete) {
			grp = GetSelectedGroup();
			if (grp) {
				LBWarningPopup.Get().Show("#lb_txt_delete", LBTranslatedString("#lb_ag_group_delete_confirm", {"%name%", grp.name}).Get(), ScriptCaller.Create(DeleteSelectedGroup));
			}
		} else if (w == btnKick) {
			SendSelectedGroupMemberRPC(LBGroupRPCs.GROUP_ADMIN_KICK);
		} else if (w == btnDemote) {
			SendSelectedGroupMemberRPC(LBGroupRPCs.GROUP_ADMIN_DEMOTE);
		} else if (w == btnPromote) {
			SendSelectedGroupMemberRPC(LBGroupRPCs.GROUP_ADMIN_PROMOTE);
		} else if (w == btnToLeader) {
			SendSelectedGroupMemberRPC(LBGroupRPCs.GROUP_ADMIN_TOLEADER);
		} else if (w == btnCopy) {
			CopySelectedUserData();
		} else if (w == chckbx_show_territorry_flags) {
			DisplayFlagPositions();
		} else if (w == btnApplyColor) {
			ApplyChatColorChange();
		}
		return false;
	}

	override bool OnDoubleClick(Widget w, int x, int y, int button) {
		if (super.OnDoubleClick(w, x, y, button))
			return true;
		if (w == mapWidget) {
			if (!LBAdmins.Get().HasPermission("groups.teleport.map"))
				return true;
			ScriptRPC rpc = new ScriptRPC();
			vector worldPos = mapWidget.ScreenToMap(Vector(x, y, 0));
			rpc.Write(worldPos);
			rpc.Send(null, LBGroupRPCs.ADMIN_TELEPORT_ON_MAP, true);
			LBMenuManager.Get().CloseAllMenus();
			return true;
		}
		return false;
	}

	void DeleteSelectedGroup() {
		ScriptRPC rpc = new ScriptRPC();
		LBGroup grp = GetSelectedGroup();
		if (grp) {
			rpc.Write(grp.shortname);
			rpc.Send(null, LBGroupRPCs.GROUP_ADMIN_DELETE, true);
		}
	}

	void ApplyChatColorChange() {
		bool hasTag = chckbx_hasCustomTag.IsChecked();
		LBGroup grp = GetSelectedGroup();

		if (!grp)
			return;
		string shortname_ = grp.shortname;
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(shortname_);
		rpc.Write(hasTag);
		rpc.Write(grp.chatColorR);
		rpc.Write(grp.chatColorG);
		rpc.Write(grp.chatColorB);
		rpc.Send(null, LBGroupRPCs.GROUP_ADMIN_CHAT_COLOR, true);
	}

	void CopySelectedUserData() {
		LBGroupMember member = GetSelectedGroupMember();
		if (!member)
			return;
		string cop = member.steamid + " " + member.name;
		g_Game.CopyToClipboard(cop);
		LBCopiedPopup.ShowAtWidget(btnCopy);
	}

	void SendSelectedGroupMemberRPC(int rpc_type) {
		ScriptRPC rpc = new ScriptRPC();
		LBGroup grp = GetSelectedGroup();
		string steamid_;
		if (!grp || !GetSelectedPlayerSteamId(steamid_))
			return;
		string shortname_ = grp.shortname;
		rpc.Write(shortname_);
		rpc.Write(steamid_);
		rpc.Send(null, rpc_type, true);
	}

	bool GetSelectedPlayerSteamId(out string steamid_) {
		LBGroupMember member = GetSelectedGroupMember();
		if (!member)
			return false;
		steamid_ = member.steamid;
		return true;
	}

	void RequestGroups() {
		if (!LBAdmins.Get().HasPermission("groups.view.groups")) {
			return;
		}
		LBLogger.Debug("Requesting all Groups:  " + g_Game.GetTime(), "AdvancedGroups");
		g_Game.RPCSingleParam(null, LBGroupRPCs.GROUP_ADMIN_LIST, new Param1<bool>(true), true);
		g_Game.RPCSingleParam(null, LBGroupRPCs.GROUP_ADMIN_FLAGS, new Param1<bool>(true), true);
	}

}
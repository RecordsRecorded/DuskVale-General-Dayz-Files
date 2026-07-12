modded class PlayerBase {

	bool lb_is_speaking = false;
	int steamidHash = 0;
	int lastSteamidHash = 0;
	string lbSafezoneTitle = "NONE";
	int lbSafezoneTitleColor = -1, safezoneMarkerIndex = -1, safezoneMarkerIndexLocal = -1, safezoneMarkerTries = 10;
	private ref LBSafezoneMarker safezoneMarker;

#ifdef SERVER
	LBGroup lbgroup;
#else
	ref LBGroup lbgroup;
#endif
	LBGroupMember memberCache;

	void PlayerBase() {
		RegisterNetSyncVariableInt("steamidHash");
		RegisterNetSyncVariableInt("lb_is_speaking");
		RegisterNetSyncVariableInt("safezoneMarkerIndex");
#ifndef SERVER
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ReinitSafezoneMarker, 100, true);
#ifdef LBmaster_Achievements
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Call(RequestSafezoneTitle);
#endif
#endif
	}

#ifdef LBmaster_GroupDLCPlotpole
	override bool IsTargetInActiveRefresherRange(EntityAI target) {
		TerritoryFlag nearest = TerritoryFlag.FindNearestFlag(target.GetPosition());
		return nearest && nearest.m_RefresherActive && nearest.IsInRadius(target.GetPosition());
	}
#endif

	void RequestSafezoneTitle() {
		if (g_Game.GetGameState() == DayZGameState.MAIN_MENU)
			return;
#ifdef LBmaster_Achievements
		g_Game.RPCSingleParam(this, LBGroupRPCs.TITLE, new Param1<bool>(true), true);
#endif
	}

	override void OnVariablesSynchronized() {
		super.OnVariablesSynchronized();
		if (lastSteamidHash != steamidHash || safezoneMarkerIndex != safezoneMarkerIndexLocal) {
			safezoneMarkerIndexLocal = safezoneMarkerIndex;
			ReinitSafezoneMarker();
		}
	}

	static void ReinitAllSafezoneMarkers() {
		if (!g_Game.IsClient())
			return;
		foreach (Man pb : LBPlayerUtils.players) {
			PlayerBase.Cast(pb).ReinitSafezoneMarker();
		}
	}

	void ReinitSafezoneMarker() {
		lastSteamidHash = steamidHash;
		safezoneMarker = null;
		if (g_Game.IsClient() && GetIdentity() && GetIdentity().GetName().Length() > 0) {
			safezoneMarker = LBSafezoneMarker.CreateSafezoneMarker(this);
			if (safezoneMarker)
				safezoneMarker.UpdateTitle(lbSafezoneTitle, lbSafezoneTitleColor);
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(ReinitSafezoneMarker);
		} else {
			--safezoneMarkerTries;
			if (safezoneMarkerTries <= 0)
				g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(ReinitSafezoneMarker);
		}
	}

	LBGroup GetLBGroup() {
		return lbgroup;
	}

	bool IsInMyLBGroup(bool mustBeVisible = false) {
		PlayerBase me = PlayerBase.Cast(g_Game.GetPlayer());
		if (!me)
			return true;
		if (!me.GetLBGroup() || me == this)
			return false;
		foreach (LBGroupMember member : me.GetLBGroup().members) {
			if (member.clientPBFound == this) {
				if (!mustBeVisible || (!member.disable3dDifferentSubgroup && member.show3D))
					return true;
			}
		}

		return false;
	}

	LBGroupPermission GetPermission() {
		LBGroupMember member = GetMyGroupMarker();
		if (!member)
			return null;
		return LBGroupPermissions.Get.FindPermissionGroupByUID(member.permissionGroup);
	}

	LBGroupMember GetMyGroupMarker(string steamid = "") {
		if (!GetLBGroup()) {
			LBLogger.Spam("No Group Found", "AdvancedGroups");
			return null;
		}
		if (memberCache)
			return memberCache;
		if (steamid == "")
			steamid = GetMySteamId();
		LBGroupMember member = GetLBGroup().GetMemberBySteamid(steamid);
		memberCache = member;
		LBLogger.Debug("Steamid: " + steamid + " Member: " + member, "AdvancedGroups");
		return member;
	}

	int GetMySubGroup() {
		LBGroupMember memb = GetMyGroupMarker();
		if (memb)
			return memb.currentSubgroup;
		return -1;
	}

	string GetMySteamId() {
		if (g_Game.IsServer()) {
			if (!GetIdentity())
				return "";
			return GetIdentity().GetPlainId();
		} else {
			return LBAdmins.Get().GetMySteamid();
		}
	}

	void SetLBGroup(LBGroup grp) {
		lbgroup = grp;
		OnGroupChanged();
	}

	void OnGroupChanged() {
		memberCache = null;
		LB_Elevator_World elevator = GetElevatorWorld();
		if (elevator)
			elevator.OnGroupChanged();
	}

	void AddSimpleClientMarker(string name, string icon, vector position, int color, string creatorId = "Server") {
		if (!g_Game.IsServer() || !GetIdentity())
			return;
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(name);
		rpc.Write(icon);
		rpc.Write(position);
		rpc.Write(color);
		rpc.Write(creatorId);
		rpc.Send(null, LBGroupRPCs.GROUP_ADD_CLIENT_MARKER, true, GetIdentity());
	}

	void OnTeamkilled(PlayerBase killer, EntityAI source) {
		// This can be overwritten by any mod to see when a player was teamkilled to give them some kind of penalty. The method is called serverside only and called on the victim PlayerBase object
	}

	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx) {
		super.OnRPC(sender, rpc_type, ctx);
		if (rpc_type == LBGroupRPCs.GROUP_SYNC && g_Game.IsClient()) {
			bool has = false;
			if (!ctx.Read(has))
				return;
			if (!has) {
				SetLBGroup(null);
				return;
			}
			LBGroup grp = new LBGroup();
			if (!grp.ReadGroupFromCtx(ctx)) {
				LBLogger.Error("Failed to receive Group from Server !", "AdvancedGroups");
				return;
			}
			LBLogger.Debug("Successfully received Group from Server", "AdvancedGroups");
			SetLBGroup(grp);
			grp.InitMarkers();
		} else if (rpc_type == LBGroupRPCs.GROUP_INVITE && g_Game.IsClient()) {
			Param1<string> shortnameParam;
			if (!ctx.Read(shortnameParam)) {
				LBLogger.Error("Failed to receive Shortname of Group Invite !", "AdvancedGroups");
				return;
			}
			LB_Elevator_World elevator = GetElevatorWorld();
			elevator.lastInvite = shortnameParam.param1;
			LBLogger.Debug("Recevied Invite to Group " + elevator.lastInvite, "AdvancedGroups");
			elevator.OnInviteReceived();
		} else if (rpc_type == LBGroupRPCs.TITLE && g_Game.IsClient()) {
			Param2<string, int> titleParams;
			if (!ctx.Read(titleParams))
				return;
			lbSafezoneTitle = titleParams.param1;
			lbSafezoneTitleColor = titleParams.param2;
			if (safezoneMarker) {
				safezoneMarker.UpdateTitle(lbSafezoneTitle, lbSafezoneTitleColor);
			}
		}
	}

	override void SetActionsRemoteTarget(out TInputActionMap InputActionMap) {
		super.SetActionsRemoteTarget(InputActionMap);
		AddAction(ActionInvitePlayerToGroup, InputActionMap);
	}

}
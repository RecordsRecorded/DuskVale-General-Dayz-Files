class LBGroupMember : LBMarker {

	static const string DISABLED_STEAMID_PREFIX = "d_";

	string steamid; // Steamid of the Player in the Group
	int permissionGroup; // ID of the Permission group found in the Permissions.json
	bool online = false; // will be updated by the server if the player is currently online
	float health; // HP of the Player (0 - 100.0)
	bool tempMember = false; // Set to 1 if the player is only temporarily part of the group (joined as secondary group)
	int lastSubgroup = 0; // Save the last subgroup the player was in when he left the server and he is moved to an offline subgroup
	bool moveToLastSubgroup = false; // If the player should be moved back to a different subgroup when he joins the server
	[NonSerialized()]
	PlayerBase clientPBFound;
	[NonSerialized()]
	string loadedIcon = "";

	void FindPlayerBase() {
		clientPBFound = null;
		foreach (Man man : LBPlayerUtils.players) {
			PlayerBase player = PlayerBase.Cast(man);
			if (player && player.IsAlive() && player.GetIdentity()) {
				string steamidOther = player.GetIdentity().GetPlainId();
				if (steamid == steamidOther) {
					clientPBFound = player;
					break;
				}
			}
		}
	}

	string ToListString(int column) {
		return name;
	}

	LBGroupPermission GetPermission() {
		return LBGroupPermissions.Get.FindPermissionGroupByUID(permissionGroup);
	}

	bool IsTemporaryMember() {
		return tempMember;
	}

	bool IsDisabled() {
		return steamid.IndexOf(DISABLED_STEAMID_PREFIX) == 0;
	}

	string GetOriginalSteamid() {
		if (IsDisabled())
			return steamid.Substring(DISABLED_STEAMID_PREFIX.Length(), steamid.Length() - DISABLED_STEAMID_PREFIX.Length());
		return steamid;
	}

	// Set this player as disabled for allowing secondary groups. Returns true if the state changed from the previous state
	bool SetDisabled(bool disabled) {
		if (disabled == IsDisabled())
			return false;
		if (disabled) {
			SetSteamid(DISABLED_STEAMID_PREFIX + steamid);
		} else {
			SetSteamid(steamid.Substring(DISABLED_STEAMID_PREFIX.Length(), steamid.Length() - DISABLED_STEAMID_PREFIX.Length()));
		}
		return true;
	}

	void SetSteamid(string steamid_) {
		this.steamid = steamid_;
		FindPlayerBase();
	}

	override void SetPosition(vector pos) {
		if (!IsValidPlayerBase())
			super.SetPosition(pos);
	}

	vector GetMarkerWorldPos() {
		if (LBLayoutConfig.Get.playerMarkerPosIndex == 0) {
			vector vec = "0 0 0";
			MiscGameplayFunctions.GetHeadBonePos(clientPBFound, vec);
			vec = vec + "0 0.2 0";
			return vec;
		} else if (LBLayoutConfig.Get.playerMarkerPosIndex == 1) {
			return LBPlayerUtils.GetHeadPosition(clientPBFound);
		}
		return clientPBFound.GetPosition();
	}

	override bool ShowMarker3D() {
		if (!super.ShowMarker3D() || !mainWidget)
			return false;
		//LBLogger.Debug("Marker Show was ok. My Steamid: " + MissionBaseWorld.mySteamid + " This Steamid: " + steamid + " Name: " + name + " Online: " + online);
		return ShowPlayerMarker() && ClientPlayerHasRadio();
	}

	static bool ClientPlayerHasRadio() {
		if (!LBMarkerSettingsConfig.Get.requireItemToSeeGroupMembers)
			return true;
		return LBPlayerUtils.HasItemsInInventory(LBMarkerSettingsConfig.Get.playerMarkerItems);
	}

	bool ShowPlayerMarker() {
		if (LBAdmins.Get().GetMySteamid() == steamid)
			return false;
		if (online)
			return true;
		return LBMarkerSettingsConfig.Get.offlinePlayer3dMarkerDistance > dist;
	}

	override string GetIcon() {
		return LBMarkerVisibilityManager.Get.playerMarkerIcon;
	}

	override void UpdateMarkerSlow() {
		FindPlayerBase();
		if (IsValidPlayerBase()) {
			position = GetMarkerWorldPos();
		}
		super.UpdateMarkerSlow();
		int index = LBLayoutConfig.Get.playerMarkerStyleIndex;
		if (nameWidget)
			nameWidget.Show((index & 0x02) == 0, false);
		if (iconWidget) {
			iconWidget.Show((index & 0x01) == 0, false);
			string icon_ = GetIcon();
			if (icon_ != loadedIcon) {
				int size = LBMarkerVisibilityManager.Get.playerSize;
				iconWidget.LoadImageFile(0, icon_);
				iconWidget.SetSize(size, size, false);
				loadedIcon = icon_;
			}
		}
	}

	override bool ShowMarkerMapOrGPS(bool isMap) {
		if (!super.ShowMarkerMapOrGPS(isMap))
			return false;
		//LBLogger.Debug("Marker Show was ok. My Steamid: " + MissionBaseWorld.mySteamid + " This Steamid: " + steamid + " Name: " + name + " Online: " + online);
		if (LBAdmins.Get().GetMySteamid() == steamid)
			return false;
		return ClientPlayerHasRadio();
	}

	override bool UpdateMarkerClient() {
		if (!super.UpdateMarkerClient())
			return false;
		if (IsValidPlayerBase()) {
			position = GetMarkerWorldPos();
		}
		return true;
	}

	override void OnMarkerRPCClient(int type_, ParamsReadContext ctx) {
		super.OnMarkerRPCClient(type_, ctx);
		if (type_ == LBGroupRPCs.HEALTH) {
			float health_ = 0;
			if (!ctx.Read(health_))
				return;
			SetHealth(health_);
		} else if (type_ == LBGroupRPCs.PERMISSION) {
			int perm = 0;
			if (!ctx.Read(perm))
				return;
			SetPermission(perm);
		} else if (type_ == LBGroupRPCs.ONLINE) {
			bool online_ = 0;
			string hashedId_;
			if (!ctx.Read(online_))
				return;
			if (!ctx.Read(hashedId_))
				return;
			SetOnline(online_, hashedId_, null);
		} else if (type_ == LBGroupRPCs.STEAMID) {
			string steamid_;
			if (!ctx.Read(steamid_))
				return;
			SetSteamid(steamid_);
		}
	}

	void SetPermission(int perm) {
		permissionGroup = perm;
	}

	string GetRankString() {
		LBGroupPermission perm = LBGroupPermissions.Get.FindPermissionGroupByUID(permissionGroup);
		if (!perm)
			return "#lb_ag_unknown";
		return perm.permName;
	}

	void SetOnline(bool on, string hashedId_, LBGroup grp) {
		online = on;
		if (g_Game.IsClient())
			SetColor();
	}

	void SetHealth(float health_) {
		this.health = health_;
	}

	bool IsValidPlayerBase() {
		return clientPBFound && clientPBFound.IsAlive();
	}

	bool CreateMember(PlayerBase pb) {
		if (!pb)
			return false;
		PlayerIdentity ident = pb.GetIdentity();
		if (!ident)
			return false;
		this.steamid = ident.GetPlainId();
		this.SetupMarker(LBMarkerType.GROUP_PLAYER_MARKER, ident.GetName(), "", pb.GetPosition());
		return true;
	}

	override int GetColorARGB() {
		if (online) {
			return LBColorManager.Get.GetColor("Player Online");
		} else {
			return LBColorManager.Get.GetColor("Player Offline");
		}
	}

	override int Get3DColorARGB() {
		if (online) {
			return LBColorManager.Get.GetColor("Player 3D Marker");
		} else {
			return super.Get3DColorARGB();
		}
	}

	override int CalcHash(bool includePosition = true) {
		return super.CalcHash(includePosition) + online * 5743 + currentSubgroup * 87877;
	}

	override bool ReadFromCtx(ParamsReadContext ctx) {
		if (!super.ReadFromCtx(ctx))
			return false;
		if (!ctx.Read(steamid))
			return false;
		if (!ctx.Read(permissionGroup))
			return false;
		if (!ctx.Read(currentSubgroup))
			return false;
		if (!ctx.Read(online))
			return false;
		if (!ctx.Read(health))
			return false;
		return true;
	}

	override void WriteToCtx(ParamsWriteContext ctx, bool steamids_ = true, bool positions_ = true) {
		super.WriteToCtx(ctx, steamids_, positions_);
		if (steamids_)
			ctx.Write(steamid);
		else
			ctx.Write("");
		ctx.Write(permissionGroup);
		ctx.Write(currentSubgroup);
		ctx.Write(online);
		ctx.Write(health);
	}

}
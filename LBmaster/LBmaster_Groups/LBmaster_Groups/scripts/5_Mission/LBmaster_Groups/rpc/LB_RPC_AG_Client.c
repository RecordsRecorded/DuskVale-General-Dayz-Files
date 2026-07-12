[RegisterLBRPCHandler(LBConverter.TypenameToString(LB_RPC_AG_Client), LBRPCHandlerType.CLIENT)]
class LB_RPC_AG_Client : LB_RPCHandler {

	void LB_RPC_AG_Client() {
		RegisterRPC(LBGroupRPCs.GROUP_RPC, ScriptCaller.Create(OnGroupRPC));
#ifndef LB_DISABLE_CHAT
		RegisterRPC(LBGroupRPCs.LB_GLOBAL_CHAT, ScriptCaller.Create(OnChatMessage));
		RegisterRPC(LBGroupRPCs.LB_GLOBAL_MUTELIST, ScriptCaller.Create(OnMuteParamReceived));
		RegisterRPC(LBGroupRPCs.LB_GLOBAL_CHANNELS, ScriptCaller.Create(OnChannelConfigReceived));
		RegisterRPC(LBGroupRPCs.LB_GLOBAL_CHAT_CLEAR, ScriptCaller.Create(ClearChat));
#endif
		RegisterRPC(LBGroupRPCs.CONFIG_SYNC_SERVER_TIME, ScriptCaller.Create(OnServerTimeSync));
		RegisterRPC(LBGroupRPCs.CHANGE_TAG_COLOR, ScriptCaller.Create(OnGroupTagColorChanged));
		RegisterRPC(LBGroupRPCs.FORCE_PLAYERLIST_UPDATE, ScriptCaller.Create(OnForcePlacerListUpdate));
		RegisterRPC(LBGroupRPCs.CONFIG_SYNC_STATIC_MARKERS, ScriptCaller.Create(StaticMarkersReceivedRPC));
		RegisterRPC(LBGroupRPCs.CONFIG_GLOBAL_MARKER_ADD, ScriptCaller.Create(StaticMarkerAddedRPC));
		RegisterRPC(LBGroupRPCs.CONFIG_GLOBAL_MARKER_REMOVE, ScriptCaller.Create(StaticMarkerRemovedRPC));
		RegisterRPC(LBGroupRPCs.CONFIG_GLOBAL_MARKER_CHANGE, ScriptCaller.Create(StaticMarkerChangedRPC));
		RegisterRPC(LBGroupRPCs.MARKER_RPC, ScriptCaller.Create(MarkerRPC));
		RegisterRPC(LBGroupRPCs.GROUP_ADD_CLIENT_MARKER, ScriptCaller.Create(AddMarkerRPC));

		LBConfigManager.Get().GetEventOnConfigReceived("LBGroupMiscConfig").Insert(OnMiscConfigReceived);
		LBConfigManager.Get().GetEventOnConfigReceived("LBGroupSettingsConfig").Insert(OnGroupConfigReceived);
	}

	void OnMiscConfigReceived() {
		LBLogger.Debug("OnMiscConfigReceived start", "AdvancedGroups");
		LBLogger.Debug("Getting Static Markers...", "AdvancedGroups");
		LBStaticMarkerManagerClient.Get();
		LBLogger.Debug("Loading Private Markers....", "AdvancedGroups");
		LBPrivateMarkerManager.Get.InitMarkers(LBWorldTools.GetServerString());
		LBLogger.Debug("OnMiscConfigReceived finish", "AdvancedGroups");
		MissionGameplay mission = MissionGameplay.Cast(g_Game.GetMission());
		mission.OnGPSAndCompassChange();
		GetElevatorMission().OnItemInInventoryChanged();
	}

	void OnGroupConfigReceived() {
		LBLogger.Debug("Finished Loading Marker Visibility Manager", "AdvancedGroups");
		if (LBGroupSettingsConfig.Get.enablePlayerList) {
			LBLogger.Debug("Loading Player List...", "AdvancedGroups");
			LBPlayerList.Get();
		} else {
			LBPlayerList.Delete();
		}
	}

	void AddMarkerRPC() {
		string name, icon, creatorId;
		vector position;
		int color;
		if (!ctx.Read(name) || !ctx.Read(icon) || !ctx.Read(position) || !ctx.Read(color) || !ctx.Read(creatorId))
			return;

		if (creatorId == "Death" && LBMarkerSettingsConfig.Get.deleteOldDeathMarker) {
			LBPrivateMarkerManager.Get.DeleteOldDeathMarker();
			LBLogger.Debug("Removed old Death marker", "AdvancedGroups");
		}

		AddClientMarkerFromServer(name, icon, position, color, creatorId);
	}

	void OnGroupTagColorChanged() {
		bool has, byPlayer;
		int r, g, b;
		if (!ctx.Read(has) || !ctx.Read(r) || !ctx.Read(g) || !ctx.Read(b) || !ctx.Read(byPlayer))
			return;
		LBGroup grp = PlayerBase.Cast(g_Game.GetPlayer()).GetLBGroup();
		if (!grp)
			return;
		grp.chatColorR = r;
		grp.chatColorG = g;
		grp.chatColorB = b;
		grp.hasCustomChatColor = has;
		grp.customChatColorAppliedByPlayer = byPlayer;
	}

	void AddClientMarkerFromServer(string name, string icon, vector position, int color, string creatorId = "Server") {
		LBLogger.Verbose("Received Client Marker from Server: " + name + " Icon: " + icon + " color: " + color, "AdvancedGroups");
		LBMarker marker = new LBMarker();
		marker.SetupMarker(LBMarkerType.PRIVATE_MARKER, name, icon, position);
		marker.SetColorInt(color);
		marker.creatorSteamID = creatorId;
		LBPrivateMarkerManager.Get.AddMarker(marker);
	}

	void StaticMarkersReceivedRPC() {
		LBStaticMarkerManagerClient.Get().StaticMarkersReceivedRPC(ctx);
	}

	void MarkerRPC() {
		LBStaticMarkerManagerClient.Get().MarkerRPC(ctx);
	}

	void StaticMarkerAddedRPC() {
		LBStaticMarkerManagerClient.Get().StaticMarkerAddedRPC(ctx);
	}

	void StaticMarkerRemovedRPC() {
		LBStaticMarkerManagerClient.Get().StaticMarkerRemovedRPC(ctx);
	}

	void StaticMarkerChangedRPC() {
		LBStaticMarkerManagerClient.Get().StaticMarkerChangedRPC(ctx);
	}

	void OnGroupRPC() {
		LBLogger.Verbose("Receivd Group RPC", "AdvancedGroups");
		PlayerBase pb = PlayerBase.Cast(g_Game.GetPlayer());
		if (!pb || !pb.GetLBGroup())
			return;
		int type = 0;
		if (!ctx.Read(type))
			return;
		LBLogger.Verbose("Received Group RPC and Found Group. " + type, "AdvancedGroups");
		pb.GetLBGroup().OnRPCClient(type, ctx);
	}

#ifndef LB_DISABLE_CHAT
	void ClearChat() {
		MissionGameplay mission = MissionGameplay.Cast(g_Game.GetMission());
		if (mission)
			mission.m_Chat.ClearChatLines();
	}

	void OnChatMessage() {
		bool changeGroupTagColor;
		int channel;
		string name, message, extra, prefix;
		int prefixColor;
		string groupPrefix, channelName;
		int channelColor, groupPrefixColor;
		if (!ctx.Read(channel) || !ctx.Read(name) || !ctx.Read(message) || !ctx.Read(extra) || !ctx.Read(prefix) || !ctx.Read(prefixColor) || !ctx.Read(groupPrefix) || !ctx.Read(channelColor) || !ctx.Read(changeGroupTagColor) || !ctx.Read(groupPrefixColor) || !ctx.Read(channelName))
			return;
		message = message.Substring(1, message.Length() - 1);
		//LBLogger.Debug("Chat Message Received: " + channel + " " + name + " " + message + " " + extra, "AdvancedGroups");

		MissionGameplay mission = MissionGameplay.Cast(g_Game.GetMission());
		if (mission)
			mission.m_Chat.AddLBChat(channel, channelName, name, message, extra, prefix, prefixColor, groupPrefix, channelColor, groupPrefixColor, changeGroupTagColor);
	}

	void OnMuteParamReceived() {
		Param1<bool> muteParam;
		if (!ctx.Read(muteParam))
			return;
		MissionGameplay mission = MissionGameplay.Cast(g_Game.GetMission());
		if (mission)
			mission.muted = muteParam.param1;
	}
	void OnChannelConfigReceived() {
		MissionGameplay mission = MissionGameplay.Cast(g_Game.GetMission());
		if (mission)
			mission.OnChannelConfigReceived(ctx);
	}
#endif
	void OnServerTimeSync() {
		Param3<int, int, int> timeParam;
		if (!ctx.Read(timeParam))
			return;
		int hour, min, sec;
		GetHourMinuteSecond(hour, min, sec);
		int timestampServer = timeParam.param1 * 3600 + timeParam.param2 * 60 + timeParam.param3;
		int timestampClient = hour * 3600 + min * 60 + sec;
		MissionGameplay mission = MissionGameplay.Cast(g_Game.GetMission());
		mission.serverToClientTimeOffset = timestampServer - timestampClient;
		LBLogger.Debug("Servertime offset received: " + mission.serverToClientTimeOffset, "AdvancedGroups");
	}

	void OnForcePlacerListUpdate() {
		if (LBGroupSettingsConfig.Get.enablePlayerList) {
			LBPlayerList.Get().UpdateEntries(true);
		}
	}
}
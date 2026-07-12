#ifndef SERVER
modded class DayZGame {

	static ref Param2<string, int> lb_kick_param;

	static ref TStringSet onlinePlayerSteamids = new TStringSet();

	static int startupMods = 0, startupPatches = 0;
	static bool checkModules = true;
	static string moduleWorldInfo = "";
	static string moduleMissionInfo = "";

	void DayZGame() {
		startupMods = ConfigGetChildrenCount("CfgMods");
		startupPatches = ConfigGetChildrenCount("CfgPatches");
	}

	void LBOnPlayerJoined(PlayerIdentity player) {
		if (!LBAdmins.Get().HasPermission("admin.view.watchlist_local"))
			return;
		if (LBWatchlistLocal.Get && LBAdminMenuSettings.Get && LBAdminMenuSettings.Get.alertJoinWatchlistLocal) {
			LBWatchlistEntry entry = LBWatchlistLocal.Get.GetWatchlistEntry(player.GetPlainId());
			if (entry) {
				NotificationSystem.AddNotificationExtended(5.0, "#lb_adm_watchlist", LBTranslatedString("#lb_adm_watchlist_joined_local", {"%player%", player.GetName(), "%reason%", entry.reason}).Get(), LBIconConfig.Get.info);
				return;
			}
		}
		if (LBWatchlistServer.Get && LBAdminMenuSettings.Get && LBAdminMenuSettings.Get.alertJoinWatchlistServer) {
			entry = LBWatchlistServer.Get.GetWatchlistEntry(player.GetPlainId());
			if (entry) {
				NotificationSystem.AddNotificationExtended(5.0, "#lb_adm_watchlist", LBTranslatedString("#lb_adm_watchlist_joined_server", {"%player%", player.GetName(), "%reason%", entry.reason}).Get(), LBIconConfig.Get.info);
				return;
			}
		}
	}

	bool LBIsPlayerOnline(string steamid) {
		return onlinePlayerSteamids.Find(steamid) != -1;
	}

	void CheckAllPlayersJoined() {
		if (!ClientData.m_PlayerList || !ClientData.m_PlayerList.m_PlayerList)
			return;
		foreach (SyncPlayer player : ClientData.m_PlayerList.m_PlayerList) {
			if (player && player.m_Identity)
				LBOnPlayerJoined(player.m_Identity);
		}
	}

	override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx) {
		if (rpc_type == ERPCs.RPC_SYNC_EVENT) {
			SyncPlayerList lastNewPlayers = ClientData.m_LastNewPlayers;
			super.OnRPC(sender, target, rpc_type, ctx);
			if (!LBAdmins.Get().received) {
				LBAdmins.Get().onReceivedCall.Insert(CheckAllPlayersJoined);
			} else {
				if (lastNewPlayers != ClientData.m_LastNewPlayers && ClientData.m_LastNewPlayers) {
					foreach (SyncPlayer player : ClientData.m_LastNewPlayers.m_PlayerList) {
						if (player && player.m_Identity)
							LBOnPlayerJoined(player.m_Identity);
					}
				}
			}
			onlinePlayerSteamids.Clear();
			if (ClientData.m_PlayerList && ClientData.m_PlayerList.m_PlayerList) {
				foreach (SyncPlayer player2 : ClientData.m_PlayerList.m_PlayerList) {
					if (player2.m_Identity)
						onlinePlayerSteamids.Insert(player2.m_Identity.GetPlainId());
				}
			}
			return;
		}
		super.OnRPC(sender, target, rpc_type, ctx);
		if (rpc_type == LBAdminToolRPCs.DISCONNECT) {
			ctx.Read(lb_kick_param);
			OnEvent(MPSessionEndEventTypeID, null);
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Call(DisconnectSessionForce);
		}
	}

	override void SetLoadState(DayZLoadState state) {
		super.SetLoadState(state);
		if (lb_kick_param && state == DayZLoadState.MAIN_MENU_CONTROLLER_SELECT) {
			ErrorModuleHandler.ThrowError(ErrorCategory.ClientKicked, lb_kick_param.param2, lb_kick_param.param1);
			lb_kick_param = null;
		}
	}

	override void OnEvent(EventType eventTypeId, Param params) {
		super.OnEvent(eventTypeId, params);
		Param1<string> printParam;
		if (Class.CastTo(printParam, params)) {
			int index = printParam.param1.IndexOf("SCRIPT");
			if (index >= 0 && index < 3) {
				if (printParam.param1.IndexOf("ScriptDebugger") != -1) {
					g_Game.RPCSingleParam(null, LBAdminToolRPCs.SEND_PRINT, printParam, true);
				} else if (checkModules && printParam.param1.IndexOf("SCRIPT       : Module: ") != -1) {
					int start = printParam.param1.IndexOf("SCRIPT       : Module: ");
					int end = printParam.param1.IndexOf(" of static memory;");
					if (end != -1) {
						string info = printParam.param1.Substring(start + 23, end - 23 - start);
						if (info.IndexOf("World") == 0) {
							moduleWorldInfo = info;
						} else if (info.IndexOf("Mission") == 0) {
							moduleMissionInfo = info;
						}
					}
				}
			}
		}
	}

}
#endif
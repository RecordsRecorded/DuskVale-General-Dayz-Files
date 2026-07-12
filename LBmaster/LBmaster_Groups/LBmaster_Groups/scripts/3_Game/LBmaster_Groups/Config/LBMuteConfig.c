#ifndef LB_DISABLE_CHAT
class LBMuteConfig : LBConfigLoader<LBMuteConfig_> {

	override void InitVars() {
		InitVarsInternal("LBGroup", "LBMuteConfig.json", LBConfigType.DATA, true, "groups.mute.player", "groups.mute.player"); // (easy)
	}

	override string GetRequiredVersion() {
		return "Advanced_Groups";
	}

	override string GetOldConfigPath() {
		return "$profile:LBmaster/Config/LBGroup/LBMuteConfig.json";
	}

}
/*Contains a list of all Muted Players. This will be overwritten while the Server is running !

If you want to unmute a player or mute a player, use the ingame Admin Menu
*/
class LBMuteConfig_ : LBConfigBase {

	const static int CURRENT_VERSION = 0;
	ref array<ref LBMutedPlayerEntry> mutedPlayers = new array<ref LBMutedPlayerEntry>(); // List of all Muted Players. Param1 = Players Steamid, Param2 = Unix Timestamp when mute expires, Param3 = Player Name
	[NonSerialized()]
	ref TStringArray mutedPlayersSteamids = new TStringArray();

	void UpdateList() {
		int timeNow = LBDate.Init(true).GetTimestamp();
		int before = mutedPlayers.Count();
		mutedPlayersSteamids.Clear();
		for (int i = 0; i < mutedPlayers.Count(); i++) {
			Param3<string, int, string> muteParams = mutedPlayers.Get(i);
			if (!muteParams) {
				mutedPlayers.Remove(i);
				i--;
				continue;
			}
			if (timeNow >= muteParams.param2) {
				PlayerIdentity ident = LBPlayerUtils.GetPlayerIdentityById(muteParams.param1);
				if (ident)
					NotificationSystem.SendNotificationToPlayerIdentityExtended(ident, 4.0, "#lb_ag_mute", "#lb_ag_mute_removed", LBIconConfig.Get.info);
				LB_Webhook_Manager.Get.SendMessage("PlayerUnmute", {muteParams.param3, muteParams.param1, "Timeout"});
				mutedPlayers.Remove(i);
				i--;
				continue;
			}
			mutedPlayersSteamids.Insert(muteParams.param1);
		}
		if (before != mutedPlayers.Count()) {
			LBMuteConfig.Loader.Save();
		}
		LBLogger.Debug("Mute Update from " + before + " to " + mutedPlayers.Count() + " Muted Players Now: " + timeNow, "AdvancedGroups");
	}

	void SendMuteList() {
		UpdateList();
		array<PlayerIdentity> identities = new array<PlayerIdentity>();
		g_Game.GetPlayerIndentities(identities);
		g_Game.RPCSingleParam(null, LBGroupRPCs.LB_GLOBAL_MUTELIST, new Param1<bool>(false), true); // Unmute All
		foreach (PlayerIdentity ident : identities) {
			if (!ident)
				continue;
			string steamid = ident.GetPlainId();
			if (mutedPlayersSteamids.Find(steamid) != -1) {
				LBLogger.Debug("Player Muted: " + steamid, "AdvancedGroups");
				SendMute(ident); // Mute other Players again
			}
		}
	}

	void MutePlayer(string name, string steamid, int minutes, string mutedBy, bool removeOld = true) {
		int now = LBDate.Init(true).GetTimestamp() + minutes * 60;
		bool skipped = false;
		if (removeOld)
			skipped = UnMutePlayer(steamid, "", false, -1);
		else
			skipped = UnMutePlayer(steamid, "", false, now);
		if (skipped)
			return;
		mutedPlayers.Insert(new LBMutedPlayerEntry(steamid, now, name));
		SendMuteList();
		string timeStr = LBDate.Init(minutes * 60).ToDiffString();
		LB_Webhook_Manager.Get.SendMessage("PlayerMute", {name, steamid, timeStr, mutedBy}); // Sent when a player is muted
		// Player Name, Player Steamid, Mute duration, Muted by (can be an admin or automatic mute)
	}

	bool IsMuted(string steamid) {
		for (int i = 0; i < mutedPlayers.Count(); i++) {
			LBMutedPlayerEntry muteParams = mutedPlayers.Get(i);
			if (!muteParams) {
				mutedPlayers.Remove(i);
				i--;
				continue;
			} else if (muteParams.param1 == steamid) {
				return true;
			}
		}
		return false;
	}

	LBMutedPlayerEntry GetMuteInfo(string steamid) {
		for (int i = 0; i < mutedPlayers.Count(); i++) {
			LBMutedPlayerEntry muteParams = mutedPlayers.Get(i);
			if (!muteParams) {
				mutedPlayers.Remove(i);
				i--;
				continue;
			} else if (muteParams.param1 == steamid) {
				return muteParams;
			}
		}
		return null;
	}

	bool UnMutePlayer(string steamid, string reason, bool sendWebhook = true, int newTime = -1) {
		for (int i = 0; i < mutedPlayers.Count(); i++) {
			LBMutedPlayerEntry muteParams = mutedPlayers.Get(i);
			if (!muteParams) {
				mutedPlayers.Remove(i);
				i--;
				continue;
			} else if (muteParams.param1 == steamid) {
				if (sendWebhook) {
					LB_Webhook_Manager.Get.SendMessage("PlayerUnmute", {muteParams.param3, steamid, reason}); // Sent when a player is unmuted
				// Player Name, Player Steamid, Unmute reason (Timeout or unmuted by admin)
				} else if (newTime != -1 && (mutedPlayers.Get(i).param2 == -1 || mutedPlayers.Get(i).param2 > newTime)) {
					LBLogger.Info("Skipping mute as the player is already muted for longer", "AdvancedGroups");
					return true;
				}
				mutedPlayers.Remove(i);
				SendMuteList();
				return false;
			}
		}
		SendMuteList();
		return false;
	}

	void SendMute(PlayerIdentity ident) {
		g_Game.RPCSingleParam(null, LBGroupRPCs.LB_GLOBAL_MUTELIST, new Param1<bool>(true), true, ident);
	}

}
#endif
#ifndef LB_DISABLE_CHAT
class ChatConfig : LBConfigLoader<ChatConfig_> {

	override void InitVars() {
		InitVarsInternal("LBGroup", "ChatConfig.json", LBConfigType.CONFIG, true, "groups.change.chat", "groups.change.chat"); // (medium)
	}

	override string GetRequiredVersion() {
		return "Advanced_Groups";
	}

}
/***Disable with LBmaster_GroupDisableChat.pbo** If you want to use your own Chat Mod
*/
class ChatConfig_ : LBConfigBase {

	const static int CURRENT_VERSION = 8;

	bool displayGroupTagsInfrontOfName = true; // enable / disable if group Tags should be displayed infront of the Playername in chat. (Each Group Leader can decide to hide their tag too)
	bool forceDisplayGroupTagsInChat = false; // Set to 1 so Group Leaders cannot disable that their Group Tag is hidden in chat
	bool enableMuteVote = false; // Enable if players should be able to vote if another player should be muted
	int muteVoteMinPlayers = 20; // Min Players online for a vote to begin
	int muteVoteMuteTimeMins = 10; // time a player should be muted if the vote was successful
	float muteVotePercentile = 0.6; // Percentile of players, that need to vote for the player (0 = 0%, 0.5 = 50%, 1.0 = 100%)
	ref array<ref ChannelCfg> channels = new array<ref ChannelCfg>(); // All channels available
	ref LBColorConfig colorBattleyeMessage = LBColorConfig.Init(255, 255, 0, 0); // argb Color for Battleye messages (e.g. RCON / Server Messages from CF Tools)
	ref array<ref PrefixGroup> prefixGroups = new array<ref PrefixGroup>(); // List of custom prefixes for players (e.g. Admins)
	ref TStringArray badWords = new TStringArray(); // List of all Bad words, which should be censored. (Not Casesensitive)
	bool enabledBadWordsCensor = false; // enable / disable if Bad Words should be censored / punished with chat mute
	bool blockBadWordContainingMessages = false; // enable / disable if the message should be sent even if bad words were detected
	string badWordsBlockedMessage = "#lb_ag_message_contains_bad_word"; // message sent to the user when bad words were detected
	int badWordsMuteTime = 0; // Time in minutes the player will be muted if bad words were detected

	void SendChatList(PlayerIdentity ident = null) {
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(channels.Count());
		foreach (ChannelCfg cfg : channels) {
			rpc.Write(cfg);
		}
		GetBattleyeColor();
		rpc.Write(colorBattleyeMessage);
		rpc.Send(null, LBGroupRPCs.LB_GLOBAL_CHANNELS, true, ident);
	}

	PrefixGroup GetPrefixByName(string name) {
		foreach (PrefixGroup grp : prefixGroups) {
			if (grp.prefix == name)
				return grp;
		}
		return null;
	}

	PrefixGroup GetPrefixForSteamid(string steamid, bool checkSteamidOnly = false) {
		if (steamid == "")
			return null;
		foreach (PrefixGroup grp : prefixGroups) {
			if (grp.IsMember(steamid, checkSteamidOnly))
				return grp;
		}
		return null;
	}

	ChannelCfg FindChannelByName(string name) {
		string lowerName = "" + name;
		lowerName.ToLower();
		foreach (ChannelCfg cfg : channels) {
			string lowerChannelName = "" + cfg.channelName;
			lowerChannelName.ToLower();
			if (lowerChannelName == lowerName)
				return cfg;
		}
		return null;
	}

	int GetBattleyeColor() {
		if (!colorBattleyeMessage) {
			colorBattleyeMessage = LBColorConfig.Init(255, 255, 0, 0);
			ChatConfig.Loader.Save();
		}
		return colorBattleyeMessage.GetColorARGB();
	}
}
#endif
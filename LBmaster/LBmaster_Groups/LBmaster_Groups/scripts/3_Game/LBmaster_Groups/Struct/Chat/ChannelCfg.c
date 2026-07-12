#ifndef LB_DISABLE_CHAT
class ChannelCfg {
	string channelName = "New Channel"; // Name of the Channel displayed when switching channels and when opening the chat
	ref LBColorConfig channelColor = new LBColorConfig(); // Color of the Playername in the Chat (argb colors from 0 to 255 for Alpha, Red, Green and Blue Channel)
	bool canReceiveMessagesFromPlayers = true; // Enable / Disable if any player can write in this channel or the channel should be used by a mod and messages can only be sent via script
	string writeChannelPermission = ""; // Permission needed to write in this channel. Empty string = everyone can write
	string readChannelPermission = ""; // Permission needed to read this channel. Empty string = everyone can read
	bool globalChannel = true; // set to 1 if Channel is a global channel
	bool groupChannel = true; // set to 1 if Channel is a group channel
	bool directChannel = true; // set to 1 if Channel is a direct (proximity) channel
	bool defaultChannel = false; // set to 1 if this is the default channel selected when the player joins
	bool applyChatFilter = false; // set to 1 if this channel should be filtered by the bad words filter if it's enabled
	int slowModeSecondsBetweenMessages = 0; // Time players need to wait to send another message

	bool muted = false; // When set to 1, this channel is currently muted and only admins can write in the channel
	[NonSerialized()]
	ref map<string, int> lastSentMessages = new map<string, int>();

	static ChannelCfg Init(string name, int r, int g, int b, bool globalC, bool groupC, bool directC, bool def, bool playersCanWrite, string readPerm, string writePerm) {
		ChannelCfg cfg = new ChannelCfg();
		cfg.channelName = name;
		cfg.channelColor = LBColorConfig.Init(255, r, g, b);
		cfg.applyChatFilter = globalC;
		cfg.globalChannel = globalC;
		cfg.groupChannel = groupC;
		cfg.directChannel = directC;
		cfg.defaultChannel = def;
		cfg.canReceiveMessagesFromPlayers = playersCanWrite;
		cfg.writeChannelPermission = writePerm;
		cfg.readChannelPermission = readPerm;
		return cfg;
	}

	int GetSlowModeRemaining(string steamid) {
		if (slowModeSecondsBetweenMessages <= 0)
			return 0;
		int time;
		if (!lastSentMessages.Find(steamid, time)) {
			time = -10000000;
		}
		int now = g_Game.GetTime() / 1000;
		return slowModeSecondsBetweenMessages + time - now - 1;
	}

	bool CheckAndSetSlowModeTimeout(string steamid) {
		if (!lastSentMessages)
			lastSentMessages = new map<string, int>();
		if (GetSlowModeRemaining(steamid) > 0)
			return false;
		int now = g_Game.GetTime() / 1000;
		lastSentMessages.Set(steamid, now);
		return true;
	}

	string ToListString(int column) {
		return channelName;
	}
}
#endif
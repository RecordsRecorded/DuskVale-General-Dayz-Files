class LBAdminPlayerTeam {
	
	int uid = -1;
	string name;
	ref TStringSet playerSteamids = new TStringSet();
	bool uniquePlayers;
	bool synced = false;
	bool overwriteESPColor;
	ref LBColorConfig espColor = new LBColorConfig();
	
	[NonSerialized()]
	ref array<PlayerIdentity> onlinePlayers = new array<PlayerIdentity>();
	
	void LBAdminPlayerTeam() {
		GetUID();
	}
	
	int GetUID() {
		if (uid == -1)
			uid = Math.RandomIntInclusive(0, 2000000000);
		return uid;
	}
	
}
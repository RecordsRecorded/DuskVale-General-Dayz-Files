class LBWatchlistEntry {

	string steamid; // Steamid of the player
	string name; // Name of the player at the time of being added to the watchlist
	int addedDate; // Unix timestamp when the player was added
	int lastModified; // Unix timestamp when the entry was last edited
	string addedBySteamid; // Steamid of the admin, who added the player to the watchlist
	string addedByName; // Name of the admin at the time of adding the player
	string reason; // Reason the player was added to the watchlist
	float confidence; // value between 0.0 and 1.0 to indicate how confident the admin is, that the player is duping, cheating, trolling etc.
	string message; // Field for the admins to take further notes on the player

	void OnLoad() {
#ifdef SERVER
		PlayerNameSave_ watched = LBLastPlayernames.Get.GetPlayerInfo(steamid);
		if (watched && watched.GetName() != "")
			name = watched.GetName();
		PlayerNameSave_ addedBy = LBLastPlayernames.Get.GetPlayerInfo(addedBySteamid);
		if (addedBy && addedBy.GetName() != "")
			addedByName = addedBy.GetName();
#endif
	}

	void LBWatchlistEntry(string steamid_, string addedBy, string message_, string reason_, float confidence_) {
		this.steamid = steamid_;
		this.addedDate = LBDate.Init(true).GetTimestamp();
		this.lastModified = this.addedDate;
		this.addedBySteamid = addedBy;
		this.message = message_;
		this.reason = reason_;
		this.confidence = Math.Clamp(confidence_, 0, 1);
	}

	void AddToList(TextListboxWidget list, int row) {
		if (list.GetNumItems() <= row)
			list.AddItem(steamid, this, 0, row);
		else
			list.SetItem(row, steamid, this, 0);
#ifndef SERVER
		if (g_Game.LBIsPlayerOnline(steamid)) {
			list.SetItemColor(row, 0, ARGB(255, 0, 255, 0));
		} else {
			list.SetItemColor(row, 0, ARGB(255, 255, 255, 255));
		}
#endif

		string addedDateString = LBDate.Init(addedDate).ToFormattedString();
		list.SetItem(row, addedDateString, null, 1);
		list.SetItem(row, reason, null, 2);
		int conf = confidence * 100.0;
		list.SetItem(row, "" + conf + "%", null, 3);
	}

}
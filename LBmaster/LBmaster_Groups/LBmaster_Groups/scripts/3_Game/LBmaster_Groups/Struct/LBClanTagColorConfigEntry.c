class LBClanTagColorConfigEntry {

	string steamid; // Steamid of the player
	string comment; // Comment to find player entries easily
	int remainingCount; // Remaining count of how often they can change the tag color. -1 for infinite

	void LBClanTagColorConfigEntry(string steamid_, string comment_, int remain) {
		this.steamid = steamid_;
		this.comment = comment_;
		this.remainingCount = remain;
	}

	void ToExpected() {
		steamid = "{STEAMID}";
		comment = "{COMMENT}";
		remainingCount = -1;
	}

}
class LBBanEntry {

	string steamid; // Steamid of the player
	string name; // Name of the player when he was banned
	string bannedBy; // Steamid of the admin, who banned the player
	string bannedByName; // Name of the admin, who banned the player at the time of the ban
	int bannedTimestamp; // Unix timestamp when the ban was issued
	int duration; // Duration in seconds of the ban. -1 for permanent bans
	string reason; // Reason of the ban, set by the admin
	string comment; // Internal comment (only visible to admins)

}
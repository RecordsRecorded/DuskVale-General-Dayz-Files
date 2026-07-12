class LBBanManager : LBConfigLoader<LBBanManager_> {

	override void InitVars() {
		InitVarsInternal("AdminTool", "bans.json", LBConfigType.CONFIG, true, "admin.ban.player", "admin.ban.player"); // (do not edit)
	}

	override string GetRequiredVersion() {
		return "Admin_Tools";
	}

}
// Should not be edited manually, but only ingame through the admin menu
class LBBanManager_ : LBConfigBase {

	static const int CURRENT_VERSION = 1;
	bool showRemainingTime = true; // Display the remaining time when a player is kicked
	bool deleteExpiredBans = false; // Delete bans from the list or only set them to expired and keep the entries
	ref array<ref LBBanEntry> bans = new array<ref LBBanEntry>(); // list of banned players
	[NonSerialized()]
	bool lockedServer = false;
	[NonSerialized()]
	string lockedMessage;

	[NonSerialized()]
	ref map<string, ref array<LBBanEntry>> cache = new map<string, ref array<LBBanEntry >> ();

}
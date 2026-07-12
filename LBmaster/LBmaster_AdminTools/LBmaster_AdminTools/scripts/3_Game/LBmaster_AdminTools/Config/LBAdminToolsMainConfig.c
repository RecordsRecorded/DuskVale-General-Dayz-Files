class LBAdminToolsMainConfig : LBConfigLoader<LBAdminToolsMainConfig_> {

	override void InitVars() {
		super.InitVarsInternal("AdminTool", "mainconfig.json", LBConfigType.CONFIG, true, "admin.change.mainconfig", "admin.change.mainconfig"); // (easy)
	}

	override string GetRequiredVersion() {
		return "Admin_Tools";
	}

}
// this config contains some very basic settings
class LBAdminToolsMainConfig_ : LBConfigBase {

	static const int CURRENT_VERSION = 10;

	int serverStatusWebhookInterval = 60; // Interval in seconds the status webhook will be sent
	bool addPossibleCheatersToWatchlist = true; // Enable this to automatically get players added to the server watchlist, who are potential cheaters
	ref map<string, bool> watchlistCheatDetections = new map<string, bool>(); // List of possible cheats detected and which should add a player to the watchlist
	bool deleteDupedItemsOnLogin = true; // If duped items are detected on login (most common known dupe method), all duped items are deleted (original item is not deleted)
	bool deleteDupedItemsOnServerRestart = false; // If the server detects a duped item on server restart, it will delete these duped items and will keep the original item
	bool preventItemTeleportHacks = true; // Cheats can teleport any item including bases, items or any other item. Even buildings placed via the Editor. This will prevent these hacks from working
	bool preventLootThroughWalls = false; // Additional checks when players try to loot containers through walls
	bool validateActionOrigin = true; // Fixes an expoit in the vanilla action system allowing users to execute actions they should not be able to
	bool validateActionDistance = true; // Fixes an expoit in the vanilla action system allowing users to execute some actions from very far away
	bool preventIllegalMagazines = true; // Prevents players for abusing a bug allowing them to load anything as a weapon magazine
	bool logInventoryEvents = false; // Log any inventory event for all players to the profiles:LBmaster_InvLog_TIMESTAMP.log
	bool contributeToGlobalBanlist = true; // This will add cheaters, which are 100% cheating automatically to a global CF Tools banlist. You can use the banlist by adding `c89df4f2-9162-4ab9-9d17-38f16ef0bf24` to your Banlists in CF Cloud

	static ref TStringArray lbServerInfoParams;

	[NonSerialized()]
	int currentFPSMin = -1;
	[NonSerialized()]
	int currentFPSMax = -1;
	[NonSerialized()]
	int lastStatusWebhookSent = 0;
	[NonSerialized()]
	int averageCounter = 0;

}
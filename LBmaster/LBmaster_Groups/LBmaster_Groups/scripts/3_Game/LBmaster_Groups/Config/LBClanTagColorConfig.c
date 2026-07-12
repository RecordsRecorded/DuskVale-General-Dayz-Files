class LBClanTagColorConfig : LBConfigLoader<LBClanTagColorConfig_> {

	override void InitVars() {
		InitVarsInternal("LBGroup", "ClanTagColorChangePermissions.json", LBConfigType.CONFIG, true, "groups.change.colors"); // (easy)
	}

	override string GetRequiredVersion() {
		return "Advanced_Groups";
	}

}
/*This file keeps track of which players have permissions to update clan tag colors.

If a player updates a tag color, the color will be applied until no player in the group is left with this permission.

This ensures players don't join groups to give them a clan tag and then leaving again
*/
class LBClanTagColorConfig_ : LBConfigBase {

	static const int CURRENT_VERSION = 1;

	bool deleteEmptyUses = false; // enable this option to automatically delete entries, which reached 0 remaining uses
	ref array<ref LBClanTagColorConfigEntry> authorizedPlayers = new array<ref LBClanTagColorConfigEntry>(); // A simple list of Steamids, which can change clan tag color

	bool IsAuthorized(string steamid) {
		LBClanTagColorConfigEntry entry = GetEntry(steamid);
		return entry && entry.remainingCount != 0;
	}

	LBClanTagColorConfigEntry GetEntry(string steamid) {
		foreach (LBClanTagColorConfigEntry entry : authorizedPlayers) {
			if (entry.steamid == steamid)
				return entry;
		}
		return null;
	}
}
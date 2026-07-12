class LBAdminPlayerTeamsList : LBConfigLoader<LBAdminPlayerTeamsList_> {

	override void InitVars() {
		InitVarsInternal("AdminTool", "teams.json", LBConfigType.CONFIG, false);
	}
	
	override bool IsClientSideConfig() {
		return true;
	}
	
}
class LBAdminPlayerTeamsList_ : LBConfigBase {

	static const int CURRENT_VERSION = 1;
	
	ref array<ref LBAdminPlayerTeam> teams = new array<ref LBAdminPlayerTeam>(); // All the different teams created
	
	override void LoadDefault() {
		LBAdminPlayerTeam empty = new LBAdminPlayerTeam();
		empty.name = "Empty";
		empty.uid = 0;
		teams.Insert(empty);
	}
	
	override bool OnLoad() {
		if (teams.Count() <= 0)
			return false;
		LBAdminPlayerTeam empty = teams.Get(0);
		empty.playerSteamids.Clear();
		empty.synced = false;
		empty.overwriteESPColor = false;
		empty.uniquePlayers = false;
		return false;
	}
	
	void OverwriteTeamInfoOrInsert(LBAdminPlayerTeam singleTeam) {
		bool found = false;
		foreach (LBAdminPlayerTeam team : teams) {
			if (team.GetUID() == singleTeam.GetUID()) {
				team.playerSteamids = singleTeam.playerSteamids;
				team.uniquePlayers = singleTeam.uniquePlayers;
				team.overwriteESPColor = singleTeam.overwriteESPColor;
				team.espColor = singleTeam.espColor;
				found = true;
				team.synced = singleTeam.synced;
				break;
			}
		}
		if (!found) {
			teams.Insert(singleTeam);
		}
	}
	
}
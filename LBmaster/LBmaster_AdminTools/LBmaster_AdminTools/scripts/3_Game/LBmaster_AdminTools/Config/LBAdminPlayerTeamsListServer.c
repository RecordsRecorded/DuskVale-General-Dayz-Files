class LBAdminPlayerTeamsListServer : LBConfigLoader<LBAdminPlayerTeamsListServer_> {

	override void InitVars() {
		InitVarsInternal("AdminTool", "teams.json", LBConfigType.CONFIG, true, "admin.view.teams", "admin.view.teams");
	}
	
}
class LBAdminPlayerTeamsListServer_ : LBConfigBase {

	static const int CURRENT_VERSION = 1;
	
	ref array<ref LBAdminPlayerTeam> teams = new array<ref LBAdminPlayerTeam>(); // All the different teams created
	
#ifndef SERVER
	override void OnReceivedFromRPC(PlayerIdentity sender) {
		TIntSet syncedEnabled = new TIntSet();
		foreach (LBAdminPlayerTeam team : teams) {
			LBAdminPlayerTeamsList.Get.OverwriteTeamInfoOrInsert(team);
			syncedEnabled.Insert(team.GetUID());
		}
		for(int i = 0; i < LBAdminPlayerTeamsList.Get.teams.Count(); ++i) {
			LBAdminPlayerTeam team2 = LBAdminPlayerTeamsList.Get.teams.Get(i);
			if (team2.synced && syncedEnabled.Find(team2.GetUID()) == -1) {
				LBAdminPlayerTeamsList.Get.teams.RemoveOrdered(i);
				--i;
			}
		}
	}
#endif
	
}
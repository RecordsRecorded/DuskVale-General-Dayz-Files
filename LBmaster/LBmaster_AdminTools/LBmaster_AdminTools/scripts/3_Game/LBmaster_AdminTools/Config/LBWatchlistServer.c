class LBWatchlistServer : LBConfigLoader<LBWatchlistServer_> {

	override void InitVars() {
		InitVarsInternal("AdminTool", "watchlist.json", LBConfigType.CONFIG, true, "admin.view.watchlist_server", "admin.view.watchlist_server"); // (do not edit)
	}

	override string GetRequiredVersion() {
		return "Admin_Tools";
	}

}
// Should not be edited manually, but only ingame through the admin menu
class LBWatchlistServer_ : LBWatchlistBase {

	static const int CURRENT_VERSION = 3;

	static bool watchlistUpdateUI = false;
#ifndef SERVER
	override void OnReceivedFromRPC(PlayerIdentity sender) {
		OnLoad();
	}
#endif

	override void AddToWatchlist(LBWatchlistEntry entry) {
		super.AddToWatchlist(entry);
#ifndef SERVER
		g_Game.RPCSingleParam(null, LBAdminToolRPCs.WATCHLIST_ADD, new Param1<ref LBWatchlistEntry>(entry), true);
#endif
	}

	override void RemoveFromWatchlist(string steamid) {
		super.RemoveFromWatchlist(steamid);
#ifndef SERVER
		g_Game.RPCSingleParam(null, LBAdminToolRPCs.WATCHLIST_REMOVE, new Param1<string>(steamid), true);
#endif
	}

	override void ModifyWatchlistEntry(LBWatchlistEntry entry) {
#ifndef SERVER
		g_Game.RPCSingleParam(null, LBAdminToolRPCs.WATCHLIST_MODIFY, new Param1<ref LBWatchlistEntry>(entry), true);
#endif
	}

	void HandleAddRPC(ParamsReadContext ctx) {
		Param1<ref LBWatchlistEntry> param;
		if (!ctx.Read(param))
			return;
		super.AddToWatchlist(param.param1);
		watchlistUpdateUI = true;
	}

	void HandleRemoveRPC(ParamsReadContext ctx) {
		Param1<string> param;
		if (!ctx.Read(param))
			return;
		super.RemoveFromWatchlist(param.param1);
		watchlistUpdateUI = true;
	}

}
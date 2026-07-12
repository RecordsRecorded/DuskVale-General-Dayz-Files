class LBWatchlistGlobal : LBConfigLoader<LBWatchlistGlobal_> {

	override void InitVars() {
		InitVarsInternal("AdminTool", "watchlistGlobal.json", LBConfigType.CONFIG, true, "admin.view.watchlist_global", "admin.view.watchlist_global"); // (do not edit!) Synced automatically
	}

	override string GetRequiredVersion() {
		return "Admin_Tools";
	}

}
// This file contains the current global watchlist. This file is automatically overwritten by the server when changes are detected
class LBWatchlistGlobal_ : LBWatchlistBase {

	static const int CURRENT_VERSION = 1;

	[NonSerialized()]
	ref Timer timer;
	[NonSerialized()]
	int currentIncrement = -1;
	[NonSerialized()]
	static bool watchlistUpdateUI = false;

	void OnWatchlistInfoReceived(LBAdminWebRequestWatchlist info) {
		watchlistUpdateUI = true;
		if (currentIncrement == -1) {
			entries = info.entries;
			UpdateCache();
			currentIncrement = info.highestIncrement;
		} else {
			foreach (string deleted : info.deletedEntries) {
				super.RemoveFromWatchlist(deleted);
			}
			foreach (LBWatchlistEntry changedNewEntry : info.entries) {
				super.AddToWatchlist(changedNewEntry);
			}
			currentIncrement = info.highestIncrement;
		}
	}

	void UpdateCache() {
		cache.Clear();
		foreach (LBWatchlistEntry entry : entries) {
			cache.Insert(entry.steamid, entry);
		}
	}

#ifndef SERVER
	override void OnReceivedFromRPC(PlayerIdentity sender) {
		currentIncrement = 0;
		OnLoad();
	}
#endif

	override void AddToWatchlist(LBWatchlistEntry entry) {
		super.AddToWatchlist(entry);
#ifndef SERVER
		g_Game.RPCSingleParam(null, LBAdminToolRPCs.GLOBAL_WATCHLIST_ADD, new Param1<ref LBWatchlistEntry>(entry), true);
#endif
	}

	override void RemoveFromWatchlist(string steamid) {
		super.RemoveFromWatchlist(steamid);
#ifndef SERVER
		g_Game.RPCSingleParam(null, LBAdminToolRPCs.GLOBAL_WATCHLIST_REMOVE, new Param1<string>(steamid), true);
#endif
	}

	override void ModifyWatchlistEntry(LBWatchlistEntry entry) {
#ifndef SERVER
		g_Game.RPCSingleParam(null, LBAdminToolRPCs.GLOBAL_WATCHLIST_MODIFY, new Param1<ref LBWatchlistEntry>(entry), true);
#endif
	}
}
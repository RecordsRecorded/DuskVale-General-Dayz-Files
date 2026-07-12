class LBWatchlistLocal : LBConfigLoader<LBWatchlistLocal_> {

	override void InitVars() {
		InitVarsInternal("AdminTool", "watchlist.json", LBConfigType.CONFIG, false);
	}

	override string GetRequiredVersion() {
		return "Admin_Tools";
	}

	override bool IsClientSideConfig() {
		return true;
	}

}
class LBWatchlistLocal_ : LBWatchlistBase {

	static const int CURRENT_VERSION = 1;

	override void AddToWatchlist(LBWatchlistEntry entry) {
		super.AddToWatchlist(entry);
		LBWatchlistLocal.Loader.Save();
	}

	override void RemoveFromWatchlist(string steamid) {
		super.RemoveFromWatchlist(steamid);
		LBWatchlistLocal.Loader.Save();
	}

	override void ModifyWatchlistEntry(LBWatchlistEntry entry) {
		AddToWatchlist(entry);
	}
}
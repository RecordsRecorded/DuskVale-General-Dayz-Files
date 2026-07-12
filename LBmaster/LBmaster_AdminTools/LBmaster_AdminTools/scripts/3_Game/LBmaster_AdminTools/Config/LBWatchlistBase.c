class LBWatchlistBase : LBConfigBase {

	ref array<ref LBWatchlistEntry> entries = new array<ref LBWatchlistEntry>(); // List of all players on the watchlist
	[NonSerialized()]
	ref map<string, LBWatchlistEntry> cache = new map<string, LBWatchlistEntry>();

	override bool OnLoad() {
		cache.Clear();
		foreach (LBWatchlistEntry entry : entries) {
			cache.Insert(entry.steamid, entry);
		}
		return false;
	}

	bool IsOnWatchlist(string steamid) {
		return cache.Contains(steamid);
	}

	LBWatchlistEntry GetWatchlistEntry(string steamid) {
		return cache.Get(steamid);
	}

	void AddToWatchlist(LBWatchlistEntry entry) {
		RemoveEntryInternal(entry.steamid);
		entries.Insert(entry);
		cache.Insert(entry.steamid, entry);
	}

	void RemoveFromWatchlist(string steamid) {
		RemoveEntryInternal(steamid);
	}

	void ModifyWatchlistEntry(LBWatchlistEntry entry) {

	}

	void RemoveEntryInternal(string steamid) {
		LBWatchlistEntry entry;
		if (cache.Find(steamid, entry)) {
			entries.RemoveItem(entry);
			cache.Remove(steamid);
		}
	}

}
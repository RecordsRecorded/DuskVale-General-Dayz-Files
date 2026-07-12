class LBGlobalBanManager : LBConfigLoader<LBGlobalBanManager_> {

	override void InitVars() {
		InitVarsInternal("AdminTool", "globalBans.json", LBConfigType.CONFIG, true, "admin.globalban.player", "admin.globalban.player"); // (Easy)
	}

	override string GetRequiredVersion() {
		return "Admin_Tools";
	}

}
// Here you can enable / disable if the server should use the "global" banlist system
// Global means all the servers running this Admin Tools license will share one banlist
// Adding someone to the banlist will sync the ban to all servers within a maximum on 1 minute delay
class LBGlobalBanManager_ : LBConfigBase {

	static const int CURRENT_VERSION = 1;
	bool enabled = true; // Enable / Disable if this server should use the global banlist
	ref array<ref LBBanEntry> bans = new array<ref LBBanEntry>; // All entries on the global banlist (editing this will not change anything since the data is not saved here and only for cases where to banlist system cannot be reached)

	[NonSerialized()]
	int currentIncrement = -1;
	[NonSerialized()]
	ref Timer updateTimer;
	[NonSerialized()]
	ref map<string, LBBanEntry> cache = new map<string, LBBanEntry>();
	[NonSerialized()]
	ref map<string, ref array<LBBanEntry>> cacheSteamid = new map<string, ref array<LBBanEntry >> ();

	override void OnReceivedFromRPC(PlayerIdentity sender) {
		currentIncrement = 0;
		UpdateCache();
	}

	void OnBanInfoReceived(LBAdminWebRequestBanlist info) {
		if (currentIncrement == -1) {
			bans = info.bans;
			UpdateCache();
			currentIncrement = info.highestIncrement;
		} else {
			foreach (LBBanEntry changedNewBan : info.bans) {
				if (!UpdateEntry(changedNewBan)) {
					AddBan(changedNewBan);
				}
			}
			foreach (LBAdminWebObjectDeletedBan deleted : info.deletedBans) {
				RemoveBan(deleted.steamid, deleted.bannedTimestamp);
			}
			currentIncrement = info.highestIncrement;
		}
	}

	void UpdateCache() {
		cache.Clear();
		cacheSteamid.Clear();
		foreach (LBBanEntry entry : bans) {
			AddBanToCache(entry);
		}

	}

	void RemoveBan(string steamid, int bannedTimestamp) {
		LBBanEntry instance = GetEntry(steamid, bannedTimestamp);
		if (instance)
			bans.RemoveItem(instance);
		cache.Remove(steamid + "#" + bannedTimestamp);
		array<LBBanEntry> banList;
		if (cacheSteamid.Find(steamid, banList)) {
			banList.RemoveItem(instance);
		}
	}

	void AddBan(LBBanEntry entry) {
		bans.Insert(entry);
		AddBanToCache(entry);
	}

	void AddBanToCache(LBBanEntry entry) {
		cache.Insert(entry.steamid + "#" + entry.bannedTimestamp, entry);
		array<LBBanEntry> banList;
		if (!cacheSteamid.Find(entry.steamid, banList)) {
			banList = new array<LBBanEntry>();
			cacheSteamid.Insert(entry.steamid, banList);
		}
		banList.Insert(entry);
	}

	bool UpdateEntry(LBBanEntry entry) {
		LBBanEntry instance = GetEntry(entry.steamid, entry.bannedTimestamp);
		if (instance) {
			instance.name = entry.name;
			instance.bannedBy = entry.bannedBy;
			instance.bannedByName = entry.bannedByName;
			instance.duration = entry.duration;
			instance.reason = entry.reason;
			instance.comment = entry.comment;
			return true;
		}
		return false;
	}

	LBBanEntry GetEntry(string steamid, int bannedTimestamp) {
		return cache.Get(steamid + "#" + bannedTimestamp);
	}

}
class LBAdminWebRequestWatchlist {

	ref array<ref LBWatchlistEntry> entries = new array<ref LBWatchlistEntry>;
	ref TStringArray deletedEntries = new TStringArray();
	int highestIncrement;

}
class LBAdminWebRequestBanlist {

	ref array<ref LBBanEntry> bans = new array<ref LBBanEntry>;
	ref array<ref LBAdminWebObjectDeletedBan> deletedBans = new array<ref LBAdminWebObjectDeletedBan>;
	int highestIncrement;

}
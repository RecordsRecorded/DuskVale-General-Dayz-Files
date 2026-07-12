class LBGroupSettingsConfig : LBConfigLoader<LBGroupSettingsConfig_> {

	[SetPriority(LBConfigPriority.HIGHEST)]
	override void InitVars() {
		InitVarsInternal("LBGroup", "GroupSettings.json", LBConfigType.CONFIG, true, "groups.change.groupsettings"); // (easy)
	}

	override string GetRequiredVersion() {
		return "Advanced_Groups";
	}

}
// This config handles all group related settings except for levels. They are setup in the Levels.json file.
class LBGroupSettingsConfig_ : LBConfigBase {

	const static int CURRENT_VERSION = 1;

	int inactiveGroupLifetimeDays = 30; // Time after a group will be deleted when no member was active for x Days
	int inviteCooldownSeconds = 120; // Time a player needs to wait to invite the same player again to prevent spam
	bool inviteActionEnabled = false; // When enabled, players can only use the ingame action to invite people and cannot see the list of players to invite them on the map menu. If you don't see the list of online players on the Group manage page, but you want to have it, disable this option
	bool inviteActionShowName = false; // Show the name of the player you invite in the action text
	bool preventFriendlyFire = false; // Set to 1 to disable players taking damage from players, which are in the same group as them. That only prevents the player from dying from direct hits like bullets fired by the other player or melee hits. Things like clothing will still be damaged
	bool preventFriendlyFireOnlyInSameSubgroup = true; // Set to 1 to only prevent friendly fire when they are in the same subgroup (`enableSubGroups` must be enabled for that). Otherwise players will still take damage even though they are in the same group
	bool groupManagePageObfuscatePlayernames = false; // enable / disable player Names being displayed in the Player list. If disabled the Names will be shown as usual. If enabled, the names will just be random numbers, which change every restart
	bool enablePlayerList = true; // enable / disable the Player List in the top left corner
	bool enablePlayerListDistance = true; // enable / disable if the Distance to the other Players should be displayed in the Player List
	bool allowJoinSecondGroupTemporarily = false; // Enable secondary groups to not have to leave the main group to temporarily join another group
	bool removeTemporaryMemberOnServerStart = true; // Enable if players should be removed from the temporary group on restart. Otherwise the player needs to leave the group or you enable `removeTemporaryMemberOnPlayerLeave`
	bool removeTemporaryMemberOnPlayerLeave = false; // Enable if players should be removed from the temporary group when he leaves the server / logs out or server restart. Otherwise players need to actively leave the temporary group
	bool enableSubGroups = true; // enable / disable subgroups / firegroups to split larger groups into smaller groups. This should make it harder for larger groups to run around as on big squad and wipe out every small group
	bool enableSubGroupSharedPlayerMapMarker = false; // If enabled, the players can still see eachother even if not in the same subgroup, which makes the subgroups pointless
	bool enableSubGroupSharedPingMapMarker = false; // Enable if the Ping Markers should be shared in the subgroups
	bool showLevelUpgradeBenefits = true; // Show new benefits when upgrading the Group (Player limit, marker limit, plotpole limit, subgroup limits)
	ref TStringArray subGroupNames = {"Alpha", "Bravo", "Charlie", "Delta", "Echo", "Foxtrot", "Golf", "Hotel", "India", "Juliett"}; // Names of the Subgroups. Make sure you have enough names for the amount of subgroups for the highest level (see levels.json)
	ref TStringArray blacklistedGroupNames = new TStringArray(); // List of group names, which are not allowed on your server. the names are not case sensitive, but have to match, so adding another character at the start or end will no longer match. This is working for group names and group tags. Player will get an error message telling them the name is already used when trying to use a blacklisted name

	[NonSerialized()]
	int groupCreationCost = 0;
	[NonSerialized()]
	ref TStringSet blacklistedGroupNamesLower = new TStringSet();

	override bool OnLoad() {
		SetGroupCreationCost();
		blacklistedGroupNamesLower = new TStringSet();
		foreach (string blacklisted : blacklistedGroupNames) {
			blacklistedGroupNamesLower.Insert(LBStringTools.ToLowerString(blacklisted));
		}
		return false;
	}

	override void WriteExtraCtx(ParamsWriteContext ctx) {
		super.WriteExtraCtx(ctx);
		ctx.Write(groupCreationCost);
	}

	override bool ReadExtraCtx(ParamsReadContext ctx) {
		if (!super.ReadExtraCtx(ctx))
			return false;
		if (!ctx.Read(groupCreationCost))
			return false;
		return true;
	}

	void SetGroupCreationCost() {
	}

}
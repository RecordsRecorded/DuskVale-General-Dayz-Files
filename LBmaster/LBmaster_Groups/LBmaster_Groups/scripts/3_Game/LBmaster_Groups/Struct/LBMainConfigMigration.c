class LBMainConfigMigration {

	int version = 0;

	// Pages.json
	bool enableInfoPanelSurvivorCount = true; // Show the Amount of online Players on the Info Panel of the Map
	bool enableInfoPanelCursorCoordinates = true; // Show the current cursor Coordinates on the Info Panel of the Map
	bool enableUIPlayerPosition = true; // Show the current Player Position on the Info Panel of the Map
	bool enableInfoPanelIngameTime = true; // Show the current Ingame Time on the Info Panel of the Map
	bool enableInfoPanelRealTime = true; // Show the current Real Time on the Info Panel of the Map (Time set as system Time on the server)
	bool disableInfoPanelModCreatorMention = false; // disable if the small message with my Name should be hidden on the Info panel
	bool disableMarkerPageOnMap = false; // Disable the Marker Page on the Map
	bool disableSettingsPageOnMap = false; // Disable the Settings Page on the Map
	bool disableGroupPageOnMap = false; // Disable the Group Page on the Map, effectively disabling the creation of groups
	ref array<ref LBButtonConfig> buttonConfig = new array<ref LBButtonConfig>(); // List of all Buttons shown on the Info Panel:

	// MarkerSettings.json
	bool canSeeOwnPlayerOnMap = true; // enable / disable if players should be able to see their own position on the map
	ref array<ref MarkerConfigEntry> markerConfig = new array<ref MarkerConfigEntry>(); // Config for all Marker Types available:
	bool addPlayerDeathMarker = true; // enable / disable if Death Markers should be added at the Death position of a player
	bool deathMarkerPrivate = false; // enable / disable if the Death Marker should be added as a private Marker
	bool deathMarkerGroup = true; // enable / disable if the Death Marker should be added as a group Marker. If both are enabled, the group marker is prefered if the player has a group. Otherwise the private marker will be added, but never both
	bool deleteOldDeathMarker = true; // enable / disable if old Death Markers should be deleted or must be deleted manually by the player
	string deathMarkerIcon = "LBmaster_Groups\\gui\\icons\\skull.paa"; // Icon of the Death Marker
	ref TStringArray availableIcons = new TStringArray(); // List of all Icons the Players can choose from when creating a new Marker
	bool requireItemToSeeGroupMembers = false; // Enable is you need to have an item with you to see other group members
	bool requireItemToBeSeenByGroupMembers = false; // Enable is you need to have an item to be seen by the other group members
	ref TStringArray playerMarkerItems = {"PersonalRadio"}; // One of the items is requires in the inventory for the 2 options above
	bool disableMarkerPlacement = false; // Disable if players should be able to place any markers. Includes group and private markers.
	int groupMarkerLimit = 20; // Base limit of Group Markers, that can be added (can be expanded per level in the levels.json)
	int tacticalPingLifetimeSeconds = 8; // Time before a Tactical Ping will be deleted automatically
	int tacticalPingMaxConcurrentMarkersPerPlayer = 1; // Amount of tactical Pings each player can place. This can be increased per level in the [Levels.json](?id=6)
	float offlinePlayer3dMarkerDistance = 20.0; // Distance the Offline Member Markers should be shown in 3D (numbers smaller than 0 will disable it)

	// GroupSettings.json
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

	// MainConfig.json
	bool enableMapLegend = false; // Show the Map legend in the bottom right to make the map less cluttered with markers, which are placed a lot on the map
	string mapLegendTitle = "#lb_ag_map_legend"; // Title which is displayed on the Map Legend
	ref array<ref MapLegendItem> mapLegend = new array<ref MapLegendItem>(); // List of entries, which will be displayed on the Map Legend. The order will be the same as in this list
	bool enableCompassHud = true; // enable / disable the compass at the top of the Players screen
	bool compassRequireItem = false; // make the Compass require an ingame item (like a compass or similar items)
	ref TStringArray compassItems = {"Compass"}; // List of items counting as a compass item. (only one of the items in the list is needed. Not all together)
	bool enableGPS = true; // enable / disable the GPS (due to the implementation of the DayZ Map, this will reduce the Client FPS noticable if shown. Players can still disable the GPS in their Client Config)
	bool gpsDisplayAngle = true; // Display the Angle the Player is looking to on the GPS
	bool gpsDisplaySpeed = true; // Display the current Speed of the Player on the GPS
	bool gpsDisplayCoords = true; // Display the current Coordinates on the GPS
	bool gpsRequireItem = false; // Require the player to have an item in their inventory to enable the GPS
	bool gpsOnlyInVehicles = false; // Force the GPS only to show if the Player is in a vehicle and never outside of vehicles
	ref TStringArray vehiclesWithGPS = {"CivilianSedan"}; // List of vehicle with an "inbuild GPS" (do not require the GPS Item if needed)
	ref TStringArray gpsItems = {"Compass", "PersonalRadio"}; // Items that count as a GPS (only one of the items in the list is needed. Not all together)
	bool mapRequireItem = false; // Require the player to have an item in their inventory to see the map. Otherwise the text below is displayed instead of the map and markers cannot be added or removed from the map
	ref TStringArray mapItems = {"ChernarusMap"}; // Items that count as a Map (only one of the items in the list is needed. Not all together)
	string mapNotFoundText = "#lb_ag_map_not_found"; // Text, which is displayed when `mapRequireItem` is enabled and the player does not have any of the items listed in `mapItems`
	string mapNotFoundImage = "LBmaster_Groups/gui/images/missing.edds"; // Path to the image displayed together with the `mapNotFoundText`

	static LBMainConfigMigration Load(string folder) {
		string file = folder + "/MainConfig.json";
		if (!FileExist(file))
			return null;
		LBMainConfigMigration mig;
		LBJsonLoader<LBMainConfigMigration>.JsonLoadFile(file, mig, true);
		if (mig.version >= 27)
			return null; // Already migrated. Do not try to import again
		return mig;
	}

}
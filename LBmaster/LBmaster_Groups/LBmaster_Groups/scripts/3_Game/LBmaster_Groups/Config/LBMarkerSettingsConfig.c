class LBMarkerSettingsConfig : LBConfigLoader<LBMarkerSettingsConfig_> {

	[SetPriority(LBConfigPriority.HIGHEST)]
	override void InitVars() {
		InitVarsInternal("LBGroup", "MarkerSettings.json", LBConfigType.CONFIG, true, "groups.change.markersettings"); // (medium)
	}

	override string GetRequiredVersion() {
		return "Advanced_Groups";
	}

}
// This config handles all Marker related settings. From visibility of markers, over max distance to death marker settings. Everything is handled here
class LBMarkerSettingsConfig_ : LBConfigBase {

	const static int CURRENT_VERSION = 3;

	bool canSeeOwnPlayerOnMap = true; // enable / disable if players should be able to see their own position on the map
	bool requireItemToSeeOwnPlayerOnMap = false; // Enable if a player needs an item from the list below in their inventory too see their own marker on the map
	ref TStringArray ownPlayerMarkerItems = {"GPSReceiver"}; // One of the items is required in the inventory if an item is required to see yourself on the map
	ref array<ref MarkerConfigEntry> markerConfig = new array<ref MarkerConfigEntry>(); // Config for all Marker Types available:
	ref array<ref MarkerConfigEntry> markerConfigFOW = new array<ref MarkerConfigEntry>(); // Config for all Marker Types available when the marker is inside FOW:
	bool addPlayerDeathMarker = true; // enable / disable if Death Markers should be added at the Death position of a player
	bool deathMarkerPrivate = false; // enable / disable if the Death Marker should be added as a private Marker
	bool deathMarkerGroup = true; // enable / disable if the Death Marker should be added as a group Marker. If both are enabled, the group marker is prefered if the player has a group. Otherwise the private marker will be added, but never both
	bool deleteOldDeathMarker = true; // enable / disable if old Death Markers should be deleted or must be deleted manually by the player
	string deathMarkerIcon = "LBmaster_Groups\\gui\\icons\\skull.paa"; // Icon of the Death Marker
	ref TStringArray availableIcons = new TStringArray(); // List of all Icons the Players can choose from when creating a new Marker
	bool requireItemToSeeGroupMembers = false; // Enable is you need to have an item with you to see other group members
	bool requireItemToBeSeenByGroupMembers = false; // Enable is you need to have an item to be seen by the other group members
	ref TStringArray playerMarkerItems = {"PersonalRadio"}; // One of the items is required in the inventory for the 2 options above
	bool disableMarkerPlacement = false; // Disable if players should be able to place any markers. Includes group and private markers.
	int groupMarkerLimit = 20; // Base limit of Group Markers, that can be added (can be expanded per level in the levels.json)
	int tacticalPingLifetimeSeconds = 8; // Time before a Tactical Ping will be deleted automatically
	int tacticalPingMaxConcurrentMarkersPerPlayer = 1; // Amount of tactical Pings each player can place. This can be increased per level in the [Levels.json](?id=6). Trying to place more than the limit will delete the oldest ping
	float offlinePlayer3dMarkerDistance = 20.0; // Distance the Offline Member Markers should be shown in 3D (numbers smaller than 0 will disable it)

#ifdef DIAG
	override void OnReceivedFromRPC(PlayerIdentity sender) {
		PrintMarkerConfigEntries();
	}
#endif

	bool CanUsePing() {
		MarkerConfigEntry pingEntry = GetMarkerConfigEntry(LBMarkerType.GROUP_PING, false);
		if (!pingEntry)
			return false;
		return pingEntry.maxDistance != 0 && (pingEntry.display3d || pingEntry.displayMap || pingEntry.displayCompass || pingEntry.displayGPS);
	}

	void PrintMarkerConfigEntries() {
		LBLogger.Debug("Marker config Entires: " + markerConfig.Count(), "AdvancedGroups");
		foreach (MarkerConfigEntry entry2 : markerConfig) {
			LBLogger.Debug("" + entry2.type + " " + entry2.maxDistance + " " + entry2.display3d + " " + entry2.displayDistance + " " + entry2.displayMap + " " + entry2.displayGPS, "AdvancedGroups");
		}
		LBLogger.Debug("Marker config Entires Fog of War: " + markerConfigFOW.Count(), "AdvancedGroups");
		foreach (MarkerConfigEntry entry : markerConfigFOW) {
			LBLogger.Debug("" + entry.type + " " + entry.maxDistance + " " + entry.display3d + " " + entry.displayDistance + " " + entry.displayMap + " " + entry.displayGPS, "AdvancedGroups");
		}
	}

	MarkerConfigEntry GetMarkerConfigEntry(LBMarkerType type, bool inFogOfWar) {
		if (!inFogOfWar) {
			foreach (MarkerConfigEntry entry2 : markerConfig) {
				if (entry2.type == type)
					return entry2;
			}
		} else {
			foreach (MarkerConfigEntry entry : markerConfigFOW) {
				if (entry.type == type)
					return entry;
			}
		}
		return null;
	}

}
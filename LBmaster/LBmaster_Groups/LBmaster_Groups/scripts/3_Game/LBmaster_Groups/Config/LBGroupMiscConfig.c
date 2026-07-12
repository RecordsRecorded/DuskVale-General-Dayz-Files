class LBGroupMiscConfig : LBConfigLoader<LBGroupMiscConfig_> {

	[SetPriority(LBConfigPriority.HIGH)]
	override void InitVars() {
		InitVarsInternal("LBGroup", "MiscConfig.json", LBConfigType.CONFIG, true, "groups.change.miscconfig"); // (medium)
	}

	override string GetRequiredVersion() {
		return "Advanced_Groups";
	}

}
/*This is the Misc Config of the Advanced Groups Mod. You can tweak a lot of things here, but these are the most important settings you find in this file:

- Map Legend to have a small Legend on the Map explaining different icons to make the Map a bit more clean and better to read

- Compass, GPS (Minimap) Settings

- And more, just read through the settings if you find anything interesting
*/
class LBGroupMiscConfig_: LBConfigBase {

	static const int CURRENT_VERSION = 28;

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
	bool disableMapMenu = false; // Disable the whole map menu opened with M / P

	override bool OnLoad() {
		LBConfigManager.Get().LoadImmediate("LBTerritoryConfig_");
		LBConfigManager.Get().LoadImmediate("LB_Webhook_Manager_");
		return false;
	}
}

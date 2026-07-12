class LB_NoBuildConfig : LBConfigLoader<LB_NoBuildConfig_> {

	override void InitVars() {
		InitVarsInternal("LBGroup", "NoBuildZones.json", LBConfigType.CONFIG, true, "groups.change.nobuildzones"); // (easy)
	}

	override string GetRequiredVersion() {
		return "Advanced_Groups";
	}

}
/*This file contains a list of all no build zones and some additional settings like if they should be displayed on the map, which color the markers should have and if certain items can still be placed in no build zones (for example Traps)

The ingame Admin menu allows you to change almost all of these settings and add and remove no build zones, so check out the ingame admin menu too!
*/
class LB_NoBuildConfig_: LBConfigBase {

	static const int CURRENT_VERSION = 2;

	bool enabled = false; // Enabled (1) or Disable (0) the whole system
	bool displayOnMap = true; // Set to 1 if players can see the zones on the map (they can turn them off in the client settings to hide them). Or set it to 0 to hide all zones for all players. No player will be able to enable the zones for them then
	string notificationTitle = "#lb_ag_no_build_title"; // Title of the notification when a player cannot build there
	string notificationMessage = "#lb_ag_no_build_here"; // The text of the notification. `{pos}` will be replaced with the name of the current Zone the player is in
	private ref TStringArray ignoreItems = {"LandMineTrap", "BearTrap", "Fireplace"}; // Items which can be placed inside of No Build Zones. For example Traps. You can enter Baseclasses like Trap_Base here. Be aware, that you need to enter the name of the config.cpp class, which is `Trap_Base` and not `TrapBase`
	int circleColorR = 255; // Red Color of the Circles shown on the map
	int circleColorG = 100; // Green Color of the Circles shown on the map
	int circleColorB = 50; // Blue Color of the Circles shown on the map
	ref array<ref LB_NoBuildEntry> zones = new array<ref LB_NoBuildEntry>(); // List of all no build zones

	[NonSerialized()]
	ref TStringSet ignoreItemsAll = new TStringSet();

	override void OnReceivedFromRPC(PlayerIdentity sender) {
		ignoreItemsAll = LBInherit.Get().GetAllChildren(ignoreItems, false, true, true, 2);
	}

	override bool OnLoad() {
		ignoreItemsAll = LBInherit.Get().GetAllChildren(ignoreItems, false, true, true, 2);
		return false;
	}

	bool IsInZone(vector v, out LB_NoBuildEntry zone_, float additionalRadius, Man player) {
		if (!enabled)
			return false;
		foreach (LB_NoBuildEntry zone : zones) {
			if (zone && zone.InZone(v, additionalRadius)) {
				zone_ = zone;
				return true;
			}
		}
		return false;
	}

	int GetCircleColor() {
		return ARGB(200, circleColorR, circleColorG, circleColorB);
	}

}
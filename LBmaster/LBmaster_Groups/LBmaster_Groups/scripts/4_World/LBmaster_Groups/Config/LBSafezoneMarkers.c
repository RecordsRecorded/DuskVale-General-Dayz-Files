class LBSafezoneMarkers : LBConfigLoader<LBSafezoneMarkers_> {

	override void InitVars() {
		InitVarsInternal("LBGroup", "SafezoneMarkers.json", LBConfigType.CONFIG, true, "groups.change.safezonemarkers"); // (easy)
	}

	override string GetRequiredVersion() {
		return "Advanced_Groups";
	}

}
/*This file contains the config for Safezone Markers. They are only markers and do not add any safezone functionality.

This config also doesn't automatically take the safezones from other mods. They need to be marked by setting the `showAllPlayerNametags` for static markers to 1 or check the box ingame to make it a safezone marker.

The zones themselves are static markers with the `showAllPlayerNametags` parameter set to 1. They need a radius set too. The color of the radius can be invisible (circleColorA set to 0), if you don't want to show the circle on the map, but still want the nametags to be visible
*/
class LBSafezoneMarkers_ : LBConfigBase {

	static const int CURRENT_VERSION = 3;

	bool enablePlayerMarkers = false; // Enable / disable the whole feature
	bool showMarkersEverywhere = false; // Enable if markers should be visible everywhere on the whole map ignoring the zones of the static markers
	bool checkLineOfSight = false; // Check the line of sight from the player to the other players head
	bool obfuscatePlayerNames = false; // Show the obfuscated version of the Playernames instead of the real names. Will behave exactly the same like the obfuscation of player names on the online players list for the group invites. The IDs of the players will also be printed in the LBmaster log when they login so you can find out who the person was
	bool showOwnPlayerTag = true; // Enable if the tag of the own player should be displayed or hidden
	float maxPlayerDistance = 100.0; // Maximum distance the nametags will be shown. Cannot exceed the network far bubble (~1000m by default)
	ref array<ref LBSafezoneMarkerEntry> safezoneMarkers = new array<ref LBSafezoneMarkerEntry>(); // List of all prefixes. You should always have one with no permissions, otherwise normal players will not have a prefix!

	override void OnReceivedFromRPC(PlayerIdentity sender) {
		if (g_Game.IsClient())
			PlayerBase.ReinitAllSafezoneMarkers();
	}

	bool IsInRadius(vector position, vector myPosition) {
		if (!enablePlayerMarkers)
			return false;
		if (vector.Distance(position, myPosition) > maxPlayerDistance)
			return false;
		if (checkLineOfSight && !DoLineOfSightCheck(position, myPosition))
			return false;
		if (showMarkersEverywhere)
			return true;
		foreach (LBServerMarker zone : LBStaticMarkerManagerClient.Get().staticMarkers) {
			if (zone.showAllPlayerNametags && zone.IsInRadius2D(position))
				return true;
		}
		return false;
	}

	bool DoLineOfSightCheck(vector position, vector myPosition) {
		int layer = PhxInteractionLayers.NOCOLLISION | PhxInteractionLayers.BUILDING | PhxInteractionLayers.DEFAULT | PhxInteractionLayers.VEHICLE | PhxInteractionLayers.DYNAMICITEM | PhxInteractionLayers.DYNAMICITEM_NOCHAR | PhxInteractionLayers.ROADWAY | PhxInteractionLayers.TERRAIN | PhxInteractionLayers.FENCE | PhxInteractionLayers.ITEM_SMALL | PhxInteractionLayers.ITEM_LARGE;
		Object hitObject;
		vector hitPos, hitNormal;
		float hitFraction;
		DayZPhysics.SphereCastBullet(myPosition, position, 0.10, layer, g_Game.GetPlayer(), hitObject, hitPos, hitNormal, hitFraction);
		if (!hitObject || (PlayerBase.Cast(hitObject) && hitObject != g_Game.GetPlayer())) {
			set<Object> results = new set<Object>();
			int hitComponent;
			if (DayZPhysics.RaycastRV(myPosition, position, hitPos, hitNormal, hitComponent, results, null, g_Game.GetPlayer())) {
				foreach (Object obj : results) {
					if (!PlayerBase.Cast(obj) && obj)
						return false;
				}
			}
			return true;
		}
		return false;
	}

}
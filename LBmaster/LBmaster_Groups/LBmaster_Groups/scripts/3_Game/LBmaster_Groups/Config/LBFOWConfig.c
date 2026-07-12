class LBFOWConfig : LBConfigLoader<LBFOWConfig_> {

	[SetPriority(LBConfigPriority.HIGH)]
	override void InitVars() {
		InitVarsInternal("LBGroup", "FogOfWarConfig.json", LBConfigType.CONFIG, true); // (easy)
	}

	override string GetRequiredVersion() {
		return "Advanced_Groups";
	}

}
// This configs allows you to configure the basics of the Fog of War feature
// To change how markers are displayed when hidden by the Fog of War, check the Marker Settings
class LBFOWConfig_ : LBConfigBase {

	static const int CURRENT_VERSION = 2;

	bool enabled = false; // Enable / disable the whole Fog of War feature
	bool hideCursorCoordinates = true; // Show ?? ?? instead of the correct coordinates on the info page when hovering over an unrevealed area
	bool canAddMarkers = false; // Can add markers in unrevealed areas. This could be abused when enabled depeding on your marker settings, but generally it's recommended to not allow players to place down markers
	bool hideServerMarkersInList = true; // Hide server markers in the Markers list when they are not revealed yet
	bool hideGroupMarkersInList = false; // Hide group markers in the Markers list when they are not revealed yet
	bool hidePrivateMarkersInList = false; // Hide private markers in the Markers list when they are not revealed yet
	int sectorCount = 150; // Amount of sectors per x and y axis. The total count is the value squared. So 200 would be 200 * 200 = 40000 sectors total. Due to render limits and performance concerns, this value is capped at 200 sectors. It's mostly useful if you run smaller maps and want less sectors. Be aware, that changing this will break the current players revealed areas as the coordinates change!!
	float discoveryRadius = 150; // Radius in Meter around the player where any touching sector is discovered. 0 means the player only discovers sectors he really entered. Anything else check if the sectors around the player intersect the circle around the player with this radius set here. Be aware, that a bigger radius means more checks for the client!
	ref LBColorConfig fogColor = LBColorConfig.Init(255, 200, 200, 200); // Color of the fog. Make sure to set the Alpha value to maximum (255), otherwise players can see the map through the fog
	bool drawMapGridOverFog = true; // Enable to draw the grid lines over the FOW to make it easier for players to see what is going on
	bool overwriteColorFromConfig = true; // Overwrite the grid lines color or use the color from the RscMapControl config.cpp class
	ref LBColorConfig gridColorOverwrite = LBColorConfig.Init(50, 255, 255, 255); // The color the grid should have if overwritten
	bool centerOnPlayerIfNotOnScreen = true; // Move the map to the player position if hw is not on screen already, so players don't have to search for their player in the Fog

	bool CanIgnoreFOW() {
		return LBAdmins.Get().HasPermissionActive("groups.ignore.fow");
	}

	bool CannotInteractWith(vector pos, bool configOption) {
		return enabled && configOption && !CanIgnoreFOW() && LBFOWPlayerInfo_.CLIENT_RECEIVED_INFO && !LBFOWPlayerInfo_.CLIENT_RECEIVED_INFO.IsPositionRevealed(pos);
	}

}
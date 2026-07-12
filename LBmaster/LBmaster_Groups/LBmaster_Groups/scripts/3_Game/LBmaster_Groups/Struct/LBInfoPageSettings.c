class LBInfoPageSettings : LBPageSettings {

	bool showSurvivorCount = true; // Show the Amount of online Players on the Info Panel of the Map
	bool showCursorCoordinates = true; // Show the current cursor Coordinates on the Info Panel of the Map
	bool showPlayerPositionCoordinates = true; // Show the current Player Position on the Info Panel of the Map
	bool showIngameTime = true; // Show the current Ingame Time on the Info Panel of the Map
	bool showRealTime = true; // Show the current Real Time on the Info Panel of the Map (Time set as system Time on the server)
	bool enableModCreatorMention = true; // disable if the small message with my Name should be hidden on the Info panel
	ref array<ref LBButtonConfig> buttonConfig = new array<ref LBButtonConfig>(); // List of all Buttons shown on the Info Panel:

}
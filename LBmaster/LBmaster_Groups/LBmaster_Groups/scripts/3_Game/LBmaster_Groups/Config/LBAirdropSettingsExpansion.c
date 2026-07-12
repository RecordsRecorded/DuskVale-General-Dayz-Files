class LBAirdropSettingsExpansion : LBConfigLoader<LBAirdropSettingsExpansion_> {

	override void InitVars() {
		InitVarsInternal("LBGroup", "ExpansionAirdropSettings.json", LBConfigType.CONFIG, false); // (easy)
	}

	override string GetRequiredVersion() {
		return "Advanced_Groups";
	}

}
// When you have expansion Missions / Expansion Airdrops installed, this config will be generated
// Here you can change the settings for Expansions Airdrop Markers for Advanced Groups
class LBAirdropSettingsExpansion_ : LBConfigBase {

	static const int CURRENT_VERSION = 1;
	bool createMarker = true; // Enable / Disable if markers should be added to the map. You might need to enable the Marker in the expansion config somewhere
	string markerName = "#STR_EXPANSION_AIRDROP_SYSTEM_TITLE"; // Default Marker name. Will be replaced with the container name when ShowAirdropTypeOnMarker is enabled in the expansion settings
	string markerIcon = "LBmaster_Groups\\gui\\icons\\circle.paa"; // Path of the icon to use for the Airdrop marker
	ref LBColorConfig color = LBColorConfig.Init(255, 235, 59, 90); // Color of the Airdrop Marker

}
class LBPagesConfig : LBConfigLoader<LBPagesConfig_> {

	[SetPriority(LBConfigPriority.HIGHEST)]
	override void InitVars() {
		InitVarsInternal("LBGroup", "Pages.json", LBConfigType.CONFIG, true, "groups.change.pages"); // (medium)
	}

	override string GetRequiredVersion() {
		return "Advanced_Groups";
	}

}
// This config controls the pages on the map menu. Read this guide here fully, before you edit it.
class LBPagesConfig_ : LBConfigBase {

	static const int CURRENT_VERSION = 2;

	ref LBInfoPageSettings infoPageSettings = new LBInfoPageSettings(500, "#lb_page_info"); // Settings related to the info page
	ref LBPageSettings groupPageSettings = new LBPageSettings(450, "#lb_page_group"); // Settings related to the groups create and manage page
	ref LBPageSettings markersPageSettings = new LBPageSettings(400, "#lb_page_markers"); // Settings related to the markers page
	ref LBPageSettings settingsPageSettings = new LBPageSettings(350, "#lb_page_settings"); // Settings for the client settings page
	ref LBPageSettings achievementsPageSettings = new LBPageSettings(120, "#lb_ac_title_achievements"); // Settings for the achievements page when the achievements mod is loaded
	ref LBPageSettings itemRestrictionsPageSettings = new LBPageSettings(100, "#lb_ir_text_restrictions"); // Settings for the item restrictions page when Enhanced Item restrictions is loaded
	ref LBPageSettings leaderboardPageSettings = new LBPageSettings(110, "Leaderboard"); // Settings for the leaderboard page when my Leaderboard mod is loaded
	ref array<ref LBCustomPageSettings> customPages = new array<ref LBCustomPageSettings>(); // Custom pages you can add for any information you want to display like ingame guides, rules or basically anything else

	LBPageSettings GetSettings(LBPageSettingsType type) {
		if (type == LBPageSettingsType.INFO)
			return infoPageSettings;
		if (type == LBPageSettingsType.GROUP)
			return groupPageSettings;
		if (type == LBPageSettingsType.MARKERS)
			return markersPageSettings;
		if (type == LBPageSettingsType.SETTINGS)
			return settingsPageSettings;
		if (type == LBPageSettingsType.ACHIEVEMENTS)
			return achievementsPageSettings;
		if (type == LBPageSettingsType.LEADERBOARD)
			return leaderboardPageSettings;
		if (type == LBPageSettingsType.ITEM_RESTRICTIONS)
			return itemRestrictionsPageSettings;
		return null;
	}

	string GetButtonText(LBPageSettingsType type, string default_ = "") {
		LBPageSettings settings = GetSettings(type);
		if (!settings)
			return default_;
		return settings.buttonText;
	}

	int GetPagePriority(LBPageSettingsType type, int default_ = 0) {
		LBPageSettings settings = GetSettings(type);
		if (!settings)
			return default_;
		return settings.priority;
	}

}
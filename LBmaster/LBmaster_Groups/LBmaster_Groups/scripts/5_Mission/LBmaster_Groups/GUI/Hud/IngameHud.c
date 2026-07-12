modded class IngameHud {

	bool LBIsHudVisible() {
		return !GetHudVisibility().IsContextFlagActive(IngameHudVisibility.HUD_HIDE_FLAGS);
	}

}
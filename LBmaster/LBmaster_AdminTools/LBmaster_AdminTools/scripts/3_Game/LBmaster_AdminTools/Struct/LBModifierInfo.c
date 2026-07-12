class LBModifierInfo {

	LBPlayerModifier id;
	bool toggleOnCheatsPage = false;
	bool syncedForOtherPlayers = false;
	string displayName, permOwn, permOther, permMulti, description, descrption_player_info, txt_enabled, txt_disabled, layout_button_name;

	void LBModifierInfo(LBPlayerModifier id_, string name, bool toggleCheat, bool syncedOthers, string own, string other, string multi, string desc, string desc_player_info, string enabled, string disabled, string button) {
		this.id = id_;
		this.displayName = name;
		this.toggleOnCheatsPage = toggleCheat;
		this.permOwn = own;
		this.permOther = other;
		this.permMulti = multi;
		this.description = desc;
		this.syncedForOtherPlayers = syncedOthers;
		this.layout_button_name = button;
		this.txt_enabled = enabled;
		this.txt_disabled = disabled;
		this.descrption_player_info = desc_player_info;
	}

}
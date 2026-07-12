class LBPlayerInfoHealth : LBPlayerInfoEntry {

	string zone, type;

	void LBPlayerInfoHealth(string zone_, string type_, string name_) {
		this.zone = zone_;
		this.type = type_;
		SetName(name_);
	}

	override float GetValueFromPlayer(PlayerBase player) {
		return player.GetHealth01(zone, type);
	}

	override void SetPlayerValue(PlayerBase player, float value) {
		player.SetHealth01(zone, type, value);
	}

	override float GetMax() {
		return 100.0;
	}

	override void SetFromServer(float value_) {
		super.SetFromServer(value_);
		if (!btn_apply.IsVisible()) {
			slider.SetCurrent(value_ * 100.0);
		}
	}

	override float GetClientValue() {
		return slider.GetCurrent() / 100.0;
	}

	override string GetSliderText() {
		return "" + ((int)(clientValue * 100.0)) + "%";
	}

}
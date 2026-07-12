class LBPlayerInfoTemperature : LBPlayerInfoEntry {

	void LBPlayerInfoTemperature(string name_) {
		SetName(name_);
	}

	override float GetValueFromPlayer(PlayerBase player) {
		return player.GetTemperature();
	}

	override void SetPlayerValue(PlayerBase player, float value) {
		player.SetTemperature(value);
	}

	override float GetMax() {
		return 100;
	}

	override float GetMin() {
		return 0;
	}

	override void SetFromServer(float value_) {
		super.SetFromServer(value_);
		if (!btn_apply.IsVisible()) {
			slider.SetCurrent(value_);
		}
	}

	override float GetClientValue() {
		return slider.GetCurrent();
	}

	override string GetSliderText() {
		return "" + ((int) clientValue);
	}

}
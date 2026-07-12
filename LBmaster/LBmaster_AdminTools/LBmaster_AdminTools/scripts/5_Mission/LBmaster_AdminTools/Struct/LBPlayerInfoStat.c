class LBPlayerInfoStat : LBPlayerInfoEntry {

	EPlayerStats_current statType;
	float statMax = 100, statMin = 0, sliderStep = 1.0;

	void LBPlayerInfoStat(EPlayerStats_current type, float max, float min, string name_, float steps = 1.0) {
		statType = type;
		statMax = max;
		statMin = min;
		sliderStep = steps;
		SetName(name_);
	}

	override float GetValueFromPlayer(PlayerBase player) {
		PlayerStat<float> stat = PlayerStat<float>.Cast(player.GetPlayerStats().GetStatObject(statType));
		return stat.Get();
	}

	override void SetPlayerValue(PlayerBase player, float value) {
		PlayerStat<float> stat = PlayerStat<float>.Cast(player.GetPlayerStats().GetStatObject(statType));
		stat.Set(value);
	}

	override float GetAccuracy() {
		return sliderStep;
	}

	override float GetMax() {
		return statMax;
	}

	override float GetMin() {
		return statMin;
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
		if (GetAccuracy() >= 1.0)
			return "" + ((int) clientValue);
		return "" + clientValue;
	}

}
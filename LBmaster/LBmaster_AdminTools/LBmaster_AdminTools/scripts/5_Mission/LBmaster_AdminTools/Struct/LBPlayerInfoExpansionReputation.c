#ifdef DZ_Expansion_Hardline
class LBPlayerInfoExpansionReputation : LBPlayerInfoEntry {

	void LBPlayerInfoExpansionReputation() {
		SetName("Reputation:");
	}

	override float GetValueFromPlayer(PlayerBase player) {
		return player.Expansion_GetReputation();
	}

	override void SetPlayerValue(PlayerBase player, float value) {
		player.Expansion_SetReputation(value, true);
	}

	override float GetMax() {
		return GetExpansionSettings().GetHardline().MaxReputation;
	}

	override bool IsVisible() {
		return GetExpansionSettings().GetHardline().MaxReputation > 0;
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
#endif
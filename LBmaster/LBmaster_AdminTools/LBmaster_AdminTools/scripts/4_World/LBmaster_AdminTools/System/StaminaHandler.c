modded class StaminaHandler {

	bool lb_disabled = false;

	void LBSetDisabled(bool disabled) {
		lb_disabled = disabled;
		if (disabled) {
			m_StaminaCap = CfgGameplayHandler.GetStaminaMax();
			SetStamina(m_StaminaCap);
		}
		RecalculateDepletionMultiplier();
	}

	override void RecalculateDepletionMultiplier() {
		if (lb_disabled) {
			m_StaminaDepletionMultiplier = 0.0;
			return;
		}
		super.RecalculateDepletionMultiplier();
	}

}
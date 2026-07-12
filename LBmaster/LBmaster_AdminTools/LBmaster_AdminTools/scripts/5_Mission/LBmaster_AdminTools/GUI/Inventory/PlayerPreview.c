modded class PlayerPreview {

	PlayerBase overwrittenPlayer;

	void LBOverwritePlayer(PlayerBase player) {
		overwrittenPlayer = player;
		m_CharacterPanelWidget.SetPlayer(overwrittenPlayer);
		UpdateInterval();
	}

	void LBResetPlayer() {
		overwrittenPlayer = null;
		m_CharacterPanelWidget.SetPlayer(null);
		m_CharacterPanelWidget.SetPlayer(g_Game.GetPlayer());
		m_CharacterPanelWidget.UpdateItemInHands(null);
		m_CharacterPanelWidget.Refresh();
		UpdateInterval();
	}

	override void UpdateInterval() {
		if (!overwrittenPlayer) {
			super.UpdateInterval();
			return;
		}
		// item in hands update
		m_CharacterPanelWidget.UpdateItemInHands(overwrittenPlayer.GetHumanInventory().GetEntityInHands());

		DayZPlayer dummyPlayer = m_CharacterPanelWidget.GetDummyPlayer();
		if (overwrittenPlayer && dummyPlayer) {
			// injury animation update
			HumanCommandAdditives hca = dummyPlayer.GetCommandModifier_Additives();
			if (hca && overwrittenPlayer.m_InjuryHandler)
				hca.SetInjured(overwrittenPlayer.m_InjuryHandler.GetInjuryAnimValue(), overwrittenPlayer.m_InjuryHandler.IsInjuryAnimEnabled());
		}
	}
}
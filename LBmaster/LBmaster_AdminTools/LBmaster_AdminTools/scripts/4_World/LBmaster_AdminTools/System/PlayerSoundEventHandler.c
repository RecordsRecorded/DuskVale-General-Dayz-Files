modded class PlayerSoundEventHandler {

	override bool PlayRequestEx(EPlayerSoundEventID id, bool sent_from_server = false, int param = 0) {
		if (m_Player.LBIsModifierActive(LBPlayerModifier.INVISIBLE) || m_Player.LBIsModifierActive(LBPlayerModifier.SILENT))
			return true;
		return super.PlayRequestEx(id, sent_from_server, param);
	}

}
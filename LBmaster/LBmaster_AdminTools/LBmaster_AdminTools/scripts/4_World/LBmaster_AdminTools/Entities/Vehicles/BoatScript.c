#ifndef SERVER
modded class BoatScript {

	void BoatScript() {
		if (LBESPManager.enabled)
			LBESPManager.AddItem(this);
	}
	override void EEHealthLevelChanged(int oldLevel, int newLevel, string zone) {
		super.EEHealthLevelChanged(oldLevel, newLevel, zone);
		if (LBESPManager.enabled)
			LBESPManager.UpdateHealthLevel(this);
	}

}
#endif
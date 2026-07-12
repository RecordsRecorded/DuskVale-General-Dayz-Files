#ifndef SERVER
modded class ZombieBase {

	void ZombieBase() {
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
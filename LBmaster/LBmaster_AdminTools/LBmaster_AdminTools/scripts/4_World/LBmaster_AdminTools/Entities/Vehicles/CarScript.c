#ifndef SERVER
modded class CarScript {

	void CarScript() {
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
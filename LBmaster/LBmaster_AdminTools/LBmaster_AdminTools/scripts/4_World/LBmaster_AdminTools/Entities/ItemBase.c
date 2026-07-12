#ifndef SERVER
modded class ItemBase {

	void ItemBase() {
		if (LBESPManager.enabled)
			LBESPManager.AddItem(this);
	}

	override void EEItemLocationChanged(notnull InventoryLocation oldLoc, notnull InventoryLocation newLoc) {
		super.EEItemLocationChanged(oldLoc, newLoc);
		if (newLoc.GetType() == InventoryLocationType.GROUND) {
			if (LBESPManager.enabled) {
				LBESPManager.RemoveItem(this);
				LBESPManager.AddItem(this);
			}
		} else if (LBESPManager.enabled) {
			LBESPManager.RemoveItem(this);
		}
	}
	override void EEHealthLevelChanged(int oldLevel, int newLevel, string zone) {
		super.EEHealthLevelChanged(oldLevel, newLevel, zone);
		if (LBESPManager.enabled)
			LBESPManager.UpdateHealthLevel(this);
	}

}
#endif
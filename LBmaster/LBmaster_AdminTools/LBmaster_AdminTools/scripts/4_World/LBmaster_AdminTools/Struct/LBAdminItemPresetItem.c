class LBAdminItemPresetItem {

	string itemname; // Itemname of the spawned item
	int spawnIn; // 0 = spawn in any, 1 = spawn in cargo, 2 = spawn as attachment
	int foodStage; // Number of the food stage or if it can hold liquid, the liquid type
	float quantity = 1.0; // Quantity of the item in Percent (0.0 = 0%, 1.0 = 100%)
	float health = 1.0; // Health of the item in Percent
	bool debugSpawn; // set to 1 to call the OnDebugSpawn() method after object was spawned
	bool ceSpawn; // Set to 1 to spawn the item with the Central Economy presets
	ref array<ref LBAdminItemPresetItem> children = new array<ref LBAdminItemPresetItem>(); // Items spawned as children on this item

	LBAdminItemPresetItem Copy(bool recursive) {
		LBAdminItemPresetItem copy = new LBAdminItemPresetItem();
		copy.itemname = itemname;
		copy.spawnIn = spawnIn;
		copy.foodStage = foodStage;
		copy.quantity = quantity;
		copy.health = health;
		copy.debugSpawn = debugSpawn;
		copy.ceSpawn = ceSpawn;
		if (recursive) {
			foreach (LBAdminItemPresetItem child : children) {
				copy.children.Insert(child.Copy(true));
			}
		}
		return copy;
	}

	static LBAdminItemPresetItem FromItem(EntityAI item, int spawnIn_) {
		if (Head.Cast(item))
			return null;
		LBAdminItemPresetItem preset = new LBAdminItemPresetItem();
		preset.itemname = item.GetType();
		preset.spawnIn = spawnIn_;
		GameInventory inv = item.GetInventory();
		ItemBase itemBs = ItemBase.Cast(item);
		switch (item.GetHealthLevel()) {
			case 0:
				preset.health = 1.0;
				break;
			case 1:
				preset.health = 0.70;
				break;
			case 2:
				preset.health = 0.50;
				break;
			case 3:
				preset.health = 0.30;
				break;
			case 4:
				preset.health = 0.0;
				break;
		}
		if (itemBs) {
			Magazine mag = Magazine.Cast(itemBs);
			if (mag) {
				preset.quantity = ((float) mag.GetAmmoCount()) / mag.GetAmmoMax();
			} else if (itemBs.HasQuantity()) {
				preset.quantity = itemBs.GetQuantity() / itemBs.GetQuantityMax();
			}
			if (itemBs.HasFoodStage()) {
				FoodStage fs = itemBs.GetFoodStage();
				if (fs)
					preset.foodStage = fs.GetFoodStageType();
			} else if (itemBs.IsLiquidContainer()) {
				preset.foodStage = itemBs.m_VarLiquidType;
			}
		}
		if (!inv)
			return preset;
		int attCount = inv.AttachmentCount();
		for (int i = 0; i < attCount; ++i) {
			LBAdminItemPresetItem newPreset = FromItem(inv.GetAttachmentFromIndex(i), 2);
			if (newPreset)
				preset.children.Insert(newPreset);
		}
		CargoBase cargo = inv.GetCargo();
		if (cargo) {
			int cargoItem = cargo.GetItemCount();
			for (i = 0; i < cargoItem; ++i) {
				newPreset = FromItem(cargo.GetItem(i), 1);
				if (newPreset)
					preset.children.Insert(newPreset);
			}
		}
		return preset;
	}

	int GetItemCountRecursive() {
		int count = children.Count();
		foreach (LBAdminItemPresetItem child : children) {
			count += child.GetItemCountRecursive();
		}
		return count;
	}

}
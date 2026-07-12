modded class Construction {

	bool LBCanBuildPart(string part_name) {
		return HasRequiredPart(part_name) && !HasConflictPart(part_name);
	}

}
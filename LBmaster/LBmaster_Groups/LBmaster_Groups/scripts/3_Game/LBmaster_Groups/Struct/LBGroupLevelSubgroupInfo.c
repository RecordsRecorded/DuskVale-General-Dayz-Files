class LBGroupLevelSubgroupInfo {

	int size; // Max amount of players that can be in this subgroup. This does not affect how many players in total can join the group. That is defined by the maxPlayerCount in the GroupLevels.json file. Make sure the total size of all subgroups is enough for all players to fit in
	bool offlineOnly; // Only for offline players. Will automatically move players into the subgroup when enabled and enough space is left in the subgroup!
	string nameOverride = ""; // Overwrite the default subgroup name

	void LBGroupLevelSubgroupInfo(int size_, bool offline_ = false) {
		size = size_;
		offlineOnly = offline_;
	}

	string ToListString(int column) {
		return "" + size + " (" + nameOverride + ")";
	}

}
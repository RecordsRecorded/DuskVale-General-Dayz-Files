class LBGroupLevelOld {

	int level;
	int maxPlayerCount;
	int upgradeCost;
	int subgroupCount;
	int subgroupSize;
	int groupMarkerLimitAdded;
	int groupPingsLimitAdded = 0;
	int groupPlotpolesLimitAdded = 0;

	LBGroupLevel ToNewLevel() {
		LBGroupLevel newLevel = new LBGroupLevel();
		newLevel.level = level;
		newLevel.maxPlayerCount = maxPlayerCount;
		newLevel.upgradeCost = upgradeCost;
		for (int i = 0; i < subgroupCount; ++i) {
			newLevel.subgroups.Insert(new LBGroupLevelSubgroupInfo(subgroupSize));
		}
		newLevel.groupMarkerLimitAdded = groupMarkerLimitAdded;
		newLevel.groupPingsLimitAdded = groupPingsLimitAdded;
		newLevel.groupPlotpolesLimitAdded = groupPlotpolesLimitAdded;
		return newLevel;
	}

}
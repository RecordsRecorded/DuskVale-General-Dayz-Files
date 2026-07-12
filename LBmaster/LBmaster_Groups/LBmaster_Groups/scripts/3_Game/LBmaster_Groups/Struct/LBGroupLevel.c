class LBGroupLevel {

	int level; // current level number (starts with 0. Do not use the same for different levels and do not skip numbers)
	int maxPlayerCount; // Maximum Total Player Count
	int upgradeCost; // Cost to upgrade the group to this level (`upgradeCost` for level 0 will be the creation cost of the group)
	ref array<ref LBGroupLevelSubgroupInfo> subgroups = new array<ref LBGroupLevelSubgroupInfo>(); // List of subgroups with their size and if they are offline subgroups
	int groupMarkerLimitAdded; // Group Marker Limit added to the `groupMarkerLimit` in the MainConfig.json
	int groupPingsLimitAdded = 0; // Added amount of Pings per player since players can set multiple pings
	int groupPlotpolesLimitAdded = 0; // If Plotpole DLC is used: Max Count of Plotpoles adding to the base Limit `defaultPlotpoleCountPerGroup` in PlotpoleConfig.json

	static LBGroupLevel InitLevel(int lvl, int maxPlayers, int cost, int subCount, int subSize, int markerAdded, int plotpoleAdded) {
		LBGroupLevel lev = new LBGroupLevel();
		lev.level = lvl;
		lev.maxPlayerCount = maxPlayers;
		lev.upgradeCost = cost;
		for (int i = 0; i < subCount; ++i) {
			lev.subgroups.Insert(new LBGroupLevelSubgroupInfo(subSize));
		}
		lev.groupMarkerLimitAdded = markerAdded;
		lev.groupPlotpolesLimitAdded = plotpoleAdded;
		return lev;
	}

	void Setup(string name) {
		subgroups.Insert(new LBGroupLevelSubgroupInfo(3));
		subgroups.Insert(new LBGroupLevelSubgroupInfo(3));
		subgroups.Insert(new LBGroupLevelSubgroupInfo(5));
		subgroups.Insert(new LBGroupLevelSubgroupInfo(5));
		subgroups.Insert(new LBGroupLevelSubgroupInfo(15, true));
	}

	bool HasNextLevel() {
		return GetNextLevel() != null;
	}

	LBGroupLevel GetNextLevel() {
		return LBGroupLevels.Get.GetNextLevel(this);
	}

	string ToListString(int column) {
		return "Level " + level;
	}

}
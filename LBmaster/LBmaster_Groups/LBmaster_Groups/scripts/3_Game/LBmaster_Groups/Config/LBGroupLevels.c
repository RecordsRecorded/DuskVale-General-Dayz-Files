class LBGroupLevels : LBConfigLoader<LBGroupLevels_> {

	[SetPriority(LBConfigPriority.HIGHEST)]
	override void InitVars() {
		InitVarsInternal("LBGroup", "GroupLevels.json", LBConfigType.CONFIG, true, "groups.change.levels"); // (easy)
	}

	override string GetRequiredVersion() {
		return "Advanced_Groups";
	}

}
// This file contains all possible group levels. The levels must start at 0 and must not have any gaps ! So after level 0 is level 1, then 2, then 3. If you remove level 2, then groups cannot be upgraded to level 3 anymore, because level 2 does not exist ! You could abuse this system to add an unreachable level and set that level for your Admins group to have higher limits, that normal groups.
class LBGroupLevels_ : LBConfigBase {

	static const int CURRENT_VERSION = 1;

	ref array<ref LBGroupLevel> levels = new array<ref LBGroupLevel>(); // List of all levels. Make sure there is a level with leve 0 and the levels have subgroups if you have subgroups enabled in the group settings

	LBGroupLevel FindLevelByUID(int level) {
		if (level == -1)
			return null;
		foreach (LBGroupLevel level2 : levels) {
			if (level2.level == level)
				return level2;
		}
		return null;
	}

	LBGroupLevel GetHighestLevel() {
		LBGroupLevel highest = null;
		foreach (LBGroupLevel level2 : levels) {
			if (!highest || level2.level > highest.level)
				highest = level2;
		}
		return highest;
	}

	LBGroupLevel GetNextLevel(LBGroupLevel level) {
		if (!level)
			return null;
		return FindLevelByUID(level.level + 1);
	}

	LBGroupLevel GetNextLevel(int level) {
		return GetNextLevel(FindLevelByUID(level));
	}

	bool HasLevelZero() {
		return FindLevelByUID(0) != null;
	}
	
	void RemoveCurrentLevel(int index) {
		int level = levels.Get(index).level;
		levels.RemoveOrdered(index);
		LBGroupLevel after = FindLevelByUID(level + 1);
		while (after != null) {
			--after.level;
			++level;
			after = FindLevelByUID(level + 1);
		}
		if (!HasLevelZero())
			CreateNewLevel("");
		OrderLevels();
	}

	void CreateNewLevel(string input) {
		int level = 0;
		while (FindLevelByUID(level) != null)
			++level;
		levels.Insert(LBGroupLevel.InitLevel(level, 10, 0, 2, 5, 0, 0));
		OrderLevels();
	}

	int GetLevelCount() {
		return levels.Count();
	}

	string GetLevel(int index) {
		return "Level " + levels.Get(index).level;
	}

	void OrderLevels() {
		bool sorted = false;
		while (!sorted) {
			sorted = true;
			for (int i = 1; i < levels.Count(); ++i) {
				LBGroupLevel prev = levels.Get(i - 1);
				LBGroupLevel current = levels.Get(i);
				if (current.level < prev.level) {
					levels.Set(i, prev);
					levels.Set(i - 1, current);
					sorted = false;
				}
			}
		}
	}

}

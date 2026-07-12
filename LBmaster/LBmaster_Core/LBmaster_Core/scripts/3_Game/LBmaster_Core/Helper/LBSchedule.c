class LBSchedule {

	bool enabled = true;
	ref array<ref LBScheduleEntry> entries = new array<ref LBScheduleEntry>(); // All entries of the Schedule. It's recommended to have them in order, but this is not required. Make sure entries are always start = true, start = false, start = true, ...
	bool default_active = true; // If no entries are found, active is this value (always active or always inactive)
	
	[NonSerialized()]
	int nextChange = -1;
	[NonSerialized()]
	int nextChangeEntry = -1;
	
	void LBSchedule(bool default_active_, bool enabled_ = true) {
		default_active = default_active_;
		enabled = enabled_;
	}
	
	int GetNextChangeRemainingTime() {
		LBDate nowUTC = LBDate.Init(true);
		return GetNextChangeRemainingTime(nowUTC);
	}
	
	int GetNextChangeRemainingTime(LBDate nowUTC) {
		if (entries.Count() == 0)
			return -1;
		int nowTimestamp = nowUTC.GetTimestamp();
		if (nextChange < nowTimestamp) {
			LBDate nowLocal = null;
			int bestIndex = -1;
			int bestTime = -1;
			for (int i = 0; i < entries.Count(); ++i) {
				LBScheduleEntry entry = entries.Get(i);
				if (!entry.utc && !nowLocal)
					nowLocal = LBDate.Init(false);
				int entryTime = 0;
				if (entry.utc) {
					entryTime = entry.GetNextMatchingTimestamp(nowUTC);
				} else {
					entryTime = entry.GetNextMatchingTimestamp(nowLocal);
				}
				if (bestIndex == -1 || entryTime < bestTime) {
					bestIndex = i;
					bestTime = entryTime;
				}
			}
			nextChange = bestTime;
			nextChangeEntry = bestIndex;
		}
		return nextChange - nowTimestamp;
	}
	
	bool IsActive(LBDate nowUTC) {
		if (!enabled)
			return default_active;
		GetNextChangeRemainingTime(nowUTC);
		if (nextChangeEntry != -1) {
			return !entries.Get(nextChangeEntry).start;
		}
		return default_active;
	}
	
	bool IsActive() {
		LBDate nowUTC = LBDate.Init(true);
		return IsActive(nowUTC);
	}
	
	[LBTestManager.StartTest(ScriptCaller.Create(TestGetNextChangeRemainingTime))]
	static void TestGetNextChangeRemainingTime() {
		LBDate now = LBDate.Init(2026, 06, 08, 12, 00, 00); // Monday
		LBSchedule schedule = new LBSchedule(true);
		schedule.entries.Insert(new LBScheduleEntry("fr", 12, 00, true, true));
		schedule.entries.Insert(new LBScheduleEntry("su", 18, 00, false, true));
		LBTest<bool>.Assert(schedule.IsActive(now), false);
		LBTest<int>.Assert(schedule.GetNextChangeRemainingTime(now), 4 * 86400);
		LBTest<int>.Assert(schedule.nextChange, now.GetTimestamp() + 4 * 86400);
		LBTest<int>.Assert(schedule.nextChangeEntry, 0);
		
		schedule = new LBSchedule(false);
		LBTest<bool>.Assert(schedule.IsActive(now), false);
		LBTest<int>.Assert(schedule.GetNextChangeRemainingTime(now), -1);
		LBTest<int>.Assert(schedule.nextChange, -1);
		LBTest<int>.Assert(schedule.nextChangeEntry, -1);
		
		schedule = new LBSchedule(true);
		schedule.entries.Insert(new LBScheduleEntry("su", 12, 00, false, true));
		schedule.entries.Insert(new LBScheduleEntry("we", 05, 15, false, true));
		schedule.entries.Insert(new LBScheduleEntry("th", 04, 15, false, true));
		schedule.entries.Insert(new LBScheduleEntry("we", 06, 15, true, true));
		schedule.entries.Insert(new LBScheduleEntry("fr", 00, 00, true, true));
		schedule.entries.Insert(new LBScheduleEntry("tu", 15, 15, true, true));
		LBTest<bool>.Assert(schedule.IsActive(now), false);
		LBTest<int>.Assert(schedule.GetNextChangeRemainingTime(now), 86400 + 3 * 3600 + 15 * 60);
		LBTest<int>.Assert(schedule.nextChange, now.GetTimestamp() + 86400 + 3 * 3600 + 15 * 60);
		LBTest<int>.Assert(schedule.nextChangeEntry, 5);
		
		now = LBDate.Init(2026, 06, 10, 16, 00, 00); // Wednesday
		LBTest<bool>.Assert(schedule.IsActive(now), true);
		LBTest<int>.Assert(schedule.GetNextChangeRemainingTime(now), 12 * 3600 + 15 * 60);
		LBTest<int>.Assert(schedule.nextChange, now.GetTimestamp() + 12 * 3600 + 15 * 60);
		LBTest<int>.Assert(schedule.nextChangeEntry, 2);
	}
	
}
class LBScheduleEntry {

	string day; // Day name either full name or the first 2 letters (Monday, Mo, Tuesday, Tu, Wednesday, We, Thursday, Th, Friday, Fr, Saturday, Sa, Sunday, Su)
	int hour; // Hour of the Day
	int minute; // Minute of the Time
	bool start; // If the event this is used for is starting or ending
	bool utc; // If the time is in UTC or the local time the Server is set to
	
	void LBScheduleEntry(string day_, int hour_, int minute_, bool start_, bool utc_) {
		day = day_;
		hour = hour_;
		minute = minute_;
		start = start_;
		utc = utc_;
	}
	
	int GetNextMatchingTimestamp() {
		LBDate now = LBDate.Init(utc);
		return GetNextMatchingTimestamp(now);
	}
	
	int GetNextMatchingTimestamp(LBDate now) {
		int currentWeekday = now.GetDayOfWeek();
		int dayIndex = LBDate.DayStringToInt(day);
		if (dayIndex < 0)
			return -1;
	    int daysUntil = dayIndex - currentWeekday;
	    if (daysUntil < 0)
	        daysUntil += 7;
	
	    int currentTime = now.m_hour * 3600 + now.m_min * 60 + now.m_sec;
	
	    int targetTime = hour * 3600 + minute * 60;
	
	    if (daysUntil == 0 && targetTime < currentTime)
	        daysUntil = 7;
	
	    return now.GetTimestamp() + daysUntil * 86400 + (targetTime - currentTime);
	}
	
	[LBTestManager.StartTest(ScriptCaller.Create(TestGetNextMatchingTimestamp))]
	static void TestGetNextMatchingTimestamp() {
		LBDate now = LBDate.Init(2026, 06, 08, 12, 00, 00); // Monday
		LBScheduleEntry entry = new LBScheduleEntry("sa", 12, 00, true, true);
		LBTest<int>.Assert(entry.GetNextMatchingTimestamp(now), 1781352000);
		entry = new LBScheduleEntry("mo", 12, 00, true, true);
		LBTest<int>.Assert(entry.GetNextMatchingTimestamp(now), now.GetTimestamp());
		entry = new LBScheduleEntry("mo", 12, 01, true, true);
		LBTest<int>.Assert(entry.GetNextMatchingTimestamp(now), now.GetTimestamp() + 60);
		entry = new LBScheduleEntry("mo", 11, 59, true, true);
		LBTest<int>.Assert(entry.GetNextMatchingTimestamp(now), now.GetTimestamp() - 60 + 86400 * 7);
		entry = new LBScheduleEntry("fr", 0, 0, true, true);
		LBTest<int>.Assert(entry.GetNextMatchingTimestamp(now), 1781222400);
	}
	
}
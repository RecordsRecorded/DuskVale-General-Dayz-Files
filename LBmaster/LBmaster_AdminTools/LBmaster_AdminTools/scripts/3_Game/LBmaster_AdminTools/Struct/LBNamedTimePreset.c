class LBNamedTimePreset : LBTimePreset {

	string name;

	static LBNamedTimePreset Init(string name_, int year_, int month_, int day_, int hour_, int minute_) {
		LBNamedTimePreset n = new LBNamedTimePreset();
		n.name = name_;
		n.year = year_;
		n.month = month_;
		n.day = day_;
		n.hour = hour_;
		n.minute = minute_;
		return n;
	}

}
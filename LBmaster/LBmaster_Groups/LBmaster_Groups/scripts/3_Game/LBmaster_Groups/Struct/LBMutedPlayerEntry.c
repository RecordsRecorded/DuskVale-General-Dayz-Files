class LBMutedPlayerEntry : Param3<string, int, string> {

	string ToListString(int column) {
		if (column == 0)
			return param3;
		if (column == 1)
			return param1;
		if (column == 2) {
			int now = LBDate.Init(true).GetTimestamp();
			return LBDate.Init(param2 - now).ToDiffString();
		}
		return "";
	}

}
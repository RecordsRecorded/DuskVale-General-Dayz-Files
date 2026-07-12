class LBOnlinePlayerEntry : Param2<string, string> {

	string ToListString(int column) {
		if (column == 0) {
			return param1;
		}
		return param2;
	}

	bool IsSearched(string search, array<Widget> extraWidgets) {
		return LBStringTools.ContainsIgnoreCase(param1, search) || LBStringTools.ContainsIgnoreCase(param2, search);
	}

}
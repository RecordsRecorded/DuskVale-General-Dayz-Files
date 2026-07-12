class LBAPIPrefixSet {

	string steamid;
	string prefixGroupName;
	bool removeFromPreviousGroups = true;

	void ToExpected() {
		steamid = "{STEAMID}";
		prefixGroupName = "[PREFIX] ";
	}

}
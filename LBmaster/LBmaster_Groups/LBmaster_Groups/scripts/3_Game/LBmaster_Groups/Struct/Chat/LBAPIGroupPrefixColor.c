class LBAPIGroupPrefixColor {

	bool hasColoredTag;
	int r, g, b;
	string groupShortname;

	void ToExpected() {
		hasColoredTag = true;
		r = 255;
		g = 255;
		b = 255;
		groupShortname = "[SHORTNAME]";
	}

}
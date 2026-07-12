class LBGroupRef {

	private string shortname;
	private LBGroup linkedGroup;

	void LBGroupRef(string shortname_, LBGroup grp) {
		this.shortname = shortname_;
		this.linkedGroup = grp;
	}

	static bool ReadFromCtx(ParamsReadContext ctx, out LBGroupRef var) {
		string groupShortname;
		if (!ctx.Read(groupShortname))
			return false;
		var = Get(groupShortname);
		return true;
	}

	static LBGroupRef Get(string groupShortname) {
		LBGroup grp = LBGroupManager.Get().GetGroupByShortName(groupShortname);
		if (!grp) {
			return new LBGroupRef(groupShortname, null);
		}
		return grp.GetReference();
	}

	void WriteToCtx(ParamsWriteContext ctx) {
		string name = GetShortname();
		ctx.Write(name);
	}

	string GetShortname() {
		if (!IsValid())
			return shortname;
		return linkedGroup.shortname;
	}

	int GetShortnameHash() {
		string myshortname = GetShortname();
		myshortname.ToLower();
		return myshortname.Hash();
	}

	bool IsValid() {
		return linkedGroup != null && linkedGroup.GetReference() == this;
	}

}
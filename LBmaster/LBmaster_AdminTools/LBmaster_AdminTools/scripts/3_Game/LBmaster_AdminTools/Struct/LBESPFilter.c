class LBESPFilter {

	string name;
	ref TStringArray items = new TStringArray();
	ref LBColorConfig color;
	bool active = false;

	[NonSerialized()]
	ref TStringSet children = new TStringSet();

	void LBESPFilter(string name_, TStringArray items_, LBColorConfig color_) {
		this.name = name_;
		this.items = items_;
		this.color = color_;
		UpdateChildren();
	}

	void UpdateChildren() {
		children = LBInherit.Get().GetAllChildren(items, true, true, true);
	}

	bool IsInFilter(string typelower) {
		return children.Find(typelower) != -1;
	}

}
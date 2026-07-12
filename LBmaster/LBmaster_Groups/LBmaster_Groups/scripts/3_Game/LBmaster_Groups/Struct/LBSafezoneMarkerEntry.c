class LBSafezoneMarkerEntry {

	string prefix; // Prefix infront of the playername. Will not be displayed when playername obfuscation is enabled
	ref LBColorConfig color; // Color of the tag
	string permission = ""; // Permission required to have this prefix and color applied. Grant all permissions is ignored. Make sure to have exactly 1 entry with an empty permission
	bool ignoreNameObfuscationSetting = false; // When enabled and playername obfuscation is enabled in the config, players with this tag will still have their plain name displayed

	void LBSafezoneMarkerEntry(string prefix_, LBColorConfig color_, string permission_ = "") {
		this.prefix = prefix_;
		this.color = color_;
		this.permission = permission_;
	}

	string ToListString(int column) {
		return prefix;
	}

}
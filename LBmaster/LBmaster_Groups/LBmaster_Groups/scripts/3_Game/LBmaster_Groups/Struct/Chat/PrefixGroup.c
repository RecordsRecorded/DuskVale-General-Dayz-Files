#ifndef LB_DISABLE_CHAT
class PrefixGroup {

	string prefix = "[PREFIX] "; // text infront of the playername (adding a tailing space is recommended)
	int colorR; // Red Channel Color from 0 to 255
	int colorG; // Green Channel Color from 0 to 255
	int colorB; // Blue Channel Color from 0 to 255
	ref TStringArray members = new TStringArray(); // List of all Steamids, which should be in this group
	string permissionToApplyGroup = ""; // Permission, which can be added to the [Admins.json](index.php?id=83) to automatically assign this prefix for that admin

	int GetColor() {
		return ARGB(255, colorR, colorG, colorB);
	}

	bool IsMember(string steamid, bool checkSteamidOnly = false) {
		return members.Find(steamid) != -1 || !checkSteamidOnly && permissionToApplyGroup != "" && LBAdmins.Get().HasPermission(permissionToApplyGroup, steamid, true);
	}

	static PrefixGroup Init(string prefix_, int colorR_, int colorG_, int colorB_, string permission) {
		PrefixGroup grp = new PrefixGroup();
		grp.prefix = prefix_;
		grp.colorR = colorR_;
		grp.colorG = colorG_;
		grp.colorB = colorB_;
		grp.permissionToApplyGroup = permission;
		return grp;
	}

	PrefixGroup AddMember(string member) {
		members.Insert(member);
		return this;
	}

	string ToListString(int column) {
		return prefix;
	}

	void ToExpected() {
		prefix = "[PREFIX] ";
		colorR = 255;
		colorG = 255;
		colorB = 255;
	}
}
#endif
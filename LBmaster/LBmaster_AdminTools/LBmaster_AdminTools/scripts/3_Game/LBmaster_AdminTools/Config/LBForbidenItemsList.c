class LBForbidenItemsList : LBConfigLoader<LBForbidenItemsList_> {

	override void InitVars() {
		InitVarsInternal("AdminTool", "forbiddenItems.json", LBConfigType.CONFIG, true, "admin.change.forbidden_items", "core.toggle.adminmode"); // (easy)
	}

	override string GetRequiredVersion() {
		return "Admin_Tools";
	}

}
// contains items, which cannot be spawned by admin
class LBForbidenItemsList_ : LBConfigBase {

	static const int CURRENT_VERSION = 1;
	ref TStringSet forbiddenItems = new TStringSet(); // List of itemnames, which cannot be spawned by admins, who don't have the "admin.ignore.forbidden_items" permission

	bool CanSpawnItem(string steamid, string itemname) {
		return forbiddenItems.Find(itemname) == -1 || LBAdmins.Get().HasPermission("admin.ignore.forbidden_items", steamid);
	}

}
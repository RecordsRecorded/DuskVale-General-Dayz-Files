class LBGroupPermissions : LBConfigLoader<LBGroupPermissions_> {

	[SetPriority(LBConfigPriority.HIGHEST)]
	override void InitVars() {
		InitVarsInternal("LBGroup", "GroupPermissions.json", LBConfigType.CONFIG, true, "groups.change.permissions"); // (medium)
	}

	override string GetRequiredVersion() {
		return "Advanced_Groups";
	}

}
/*List of all Permission Groups / Ranks for the Clan (default: Temp, Trial, Member, Admin, Leader)

All Groups will inherit the permissions from the inherit Group, so if an admin can invite a player, Leaders automatically can invite players too.

Promote powers or other powers will at least be the same power as the inherit group and can never be lower.
*/
class LBGroupPermissions_ : LBConfigBase {

	static const int CURRENT_VERSION = 1;

	ref array<ref LBGroupPermission> allGroups = new array<ref LBGroupPermission>(); // List of all permissions / ranks for the groups

	// Used when a group is created and the leader should get his rank assigned
	// Can be overwritten to give certain special groups different permission ranks. for example a police or medic group would have other ranks than a civilian group. Make sure to also overwrite the FindLowestGroup method!
	LBGroupPermission FindHighestGroup(string groupTag = "") {
		foreach (LBGroupPermission perm : allGroups) {
			if (perm.nextGroupUID == -1)
				return perm;
		}
		return null;
	}

	// Used when a new member joins a group and gets the lowest permission rank assigned
	// Can be overwritten to give certain special groups different permission ranks. for example a police or medic group would have other ranks than a civilian group. Make sure to also overwrite the FindHighestGroup method!
	LBGroupPermission FindLowestGroup(string groupTag = "") {
		foreach (LBGroupPermission perm : allGroups) {
			if (perm.previousGroupUID == -1)
				return perm;
		}
		return null;
	}

	LBGroupPermission FindPermissionGroupByUID(int uid) {
		if (uid == -1)
			return null;
		foreach (LBGroupPermission perm : allGroups) {
			if (perm.UID == uid)
				return perm;
		}
		return null;
	}

	LBGroupPermission GetNextPermission(LBGroupPermission perm) {
		if (!perm)
			return null;
		return FindPermissionGroupByUID(perm.nextGroupUID);
	}

	LBGroupPermission GetNextPermission(int uid) {
		return GetNextPermission(FindPermissionGroupByUID(uid));
	}

	LBGroupPermission GetPreviousPermission(LBGroupPermission perm) {
		if (!perm)
			return null;
		return FindPermissionGroupByUID(perm.nextGroupUID);
	}

	LBGroupPermission GetPreviousPermission(int uid) {
		return GetPreviousPermission(FindPermissionGroupByUID(uid));
	}

}
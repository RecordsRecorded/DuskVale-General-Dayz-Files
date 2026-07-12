class LBGroupPermission {

	int UID; // Unique ID of this Rank (will be used to store the current Rank of the Player in the Groups json File)
	int nextGroupUID; // UID of the higher group (-1 if highest Rank)
	int previousGroupUID; // UID of the lower group (-1 if lowest Rank)
	int inheritGroupUID; // UID of the Rank, this Rank should inherit from (in most cases the `previousGroupUID`)
	string permName; // Displayname of the Rank shown behind the Playername in the Group overview
	ref array<ref Param2<int, bool>> markerPermissions = new array<ref Param2<int, bool >> (); /* List of all Permissions for each Marker Type (see MainConfig.json):

	- `param1`: type of the Marker (refer to MainConfig.json for all types available)

	- `param2`: 0 if not visible / 1 if visible

	*/
	bool canUpgrade; // enable / disable if the Player can Upgrade the Group to the next level
	bool canPromote; // enable / disable if the Player can Promote others
	bool canDemote; // enable / disable if the Player can Demote others
	bool canInvite; // allow the Player to invite new Players to the Group
	int promotePower; // Power the Player has to promote others. Needs to be greater than the `promoteNeedPower` of the target rank of the promoted player
	int demotePower; // Power the player has to demote others. Needs to be greater than the `demoteNeedPower` of the current rank of the demoted player
	int moveSubgroupPower; // Power the player has to move others to a different subgroup. Needs to be greater than the `moveSubgroupNeedPower` of the current rank of the moved player
	int promoteNeedPower; // Power needed to promote someone to this rank
	int demoteNeedPower; // Power needed to demote someone from this rank to a lower rank
	int moveSubgroupNeedPower; // Power needed to move the player to a different subgroup
	bool canChangeChatTagColor = false; // Allow the player to change the Chat Tag color if he is added to the `ClanTagColorChangePermissions.json` file. He needs to have both permissions
	bool canDoBasebuilding = false; // Allow the player to build / dismandle / destroy Basebuilding items
	bool canLowerFlag = false; // Allow the player to lower territory flags
	bool canRaiseFlag = false; // Allow the player to raise territory flags
	bool canPackPlotpole = false; // can destroy the Flagpole owned by the Group
	bool canCreateSeeATMAccount = false; // (only with ATM DLC) can see the Balance of the Group Account
	bool canCreateGroupATMAccount = false; // Can buy the Group account
	bool canWithdrawGroupMoney = false; // Can withdraw money from the group account
	bool canDepositGroupMoney = false; // Can deposit money to the group account
	bool canOpenGroupGarage = false; // Can open the Group Garage when combined with the [Virtual Garage Mod](/product.php?id=5)
	bool tempGroup = false; // if players in this group should be removed after server restart. 0 if group is persistent

	[NonSerialized()]
	bool inheritedMarkerPermissions = false;

	bool CanChangeClanTagColor(string steamid = "") {
		if (!canChangeChatTagColor)
			return false;
		if (steamid == "")
			steamid = LBAdmins.Get().GetMySteamid();
		return LBClanTagColorConfig.Get.IsAuthorized(steamid);
	}

	bool CanPromote(LBGroupPermission target) {
		if (!target || target.nextGroupUID == -1)
			return false;
		LBGroupPermission next = target.GetNextGroup();
		if (!next)
			return false;
		return promotePower >= next.promoteNeedPower;
	}

	bool CanDemote(LBGroupPermission target) {
		if (!target || target.previousGroupUID == -1)
			return false;
		return demotePower >= target.demoteNeedPower;
	}

	bool CanKick(LBGroupPermission target) {
		if (!target)
			return false;
		return demotePower >= target.demoteNeedPower;
	}

	bool CanMove(LBGroupPermission target) {
		if (!target)
			return false;
		return moveSubgroupPower >= target.moveSubgroupNeedPower;
	}

	bool CanSeeMarkerType(LBMarkerType type) {
		foreach (Param2<int, bool> allowed : markerPermissions) {
			if (allowed.param1 == type)
				return allowed.param2;
		}
		return false;
	}

	void FillInherited(LBGroupPermission inherited) {
		canInvite = canInvite || inherited.canInvite;
		canUpgrade = canUpgrade || inherited.canUpgrade;
		canPromote = canPromote || inherited.canPromote;
		canDoBasebuilding = canDoBasebuilding || inherited.canDoBasebuilding;
		canLowerFlag = canLowerFlag || inherited.canLowerFlag;
		canRaiseFlag = canRaiseFlag || inherited.canRaiseFlag;
		canPackPlotpole = canPackPlotpole || inherited.canPackPlotpole;
		canCreateGroupATMAccount = canCreateGroupATMAccount || inherited.canCreateGroupATMAccount;
		canCreateSeeATMAccount = canCreateSeeATMAccount || inherited.canCreateSeeATMAccount;
		canWithdrawGroupMoney = canWithdrawGroupMoney || inherited.canWithdrawGroupMoney;
		canDepositGroupMoney = canDepositGroupMoney || inherited.canDepositGroupMoney;
		canDemote = canDemote || inherited.canDemote;
		canOpenGroupGarage = canOpenGroupGarage || inherited.canOpenGroupGarage;
		promotePower = Math.Max(promotePower, inherited.promotePower);
		demotePower = Math.Max(demotePower, inherited.demotePower);
		moveSubgroupPower = Math.Max(moveSubgroupPower, inherited.moveSubgroupPower);
		promoteNeedPower = Math.Max(promoteNeedPower, inherited.promoteNeedPower);
		demoteNeedPower = Math.Max(demoteNeedPower, inherited.demoteNeedPower);
		moveSubgroupNeedPower = Math.Max(moveSubgroupNeedPower, inherited.moveSubgroupNeedPower);
		if (!inheritedMarkerPermissions) {
			inheritedMarkerPermissions = true;
			foreach (Param2<int, bool> othermarkerPerms : inherited.markerPermissions) {
				bool found = false;
				foreach (Param2<int, bool> mymarkerPerms : markerPermissions) {
					if (othermarkerPerms.param1 == mymarkerPerms.param1) {
						mymarkerPerms.param2 = mymarkerPerms.param2 || othermarkerPerms.param2;
						found = true;
					}
				}
				if (!found) {
					markerPermissions.Insert(new Param2<int, bool>(othermarkerPerms.param1, othermarkerPerms.param2));
				}
			}
		}
	}

	void PrintPermission() {
		LBLogger.Debug(permName + " Temp ? " + tempGroup + " (" + UID + ") < " + previousGroupUID + " > " + nextGroupUID + " : " + inheritGroupUID, "AdvancedGroups");
		LBLogger.Debug("canUpgrade: " + canUpgrade + " canPromote: " + canPromote + " canDemote: " + canDemote + " canInvite: " + canInvite + " canPackPlotpole: " + canPackPlotpole + " canDoBasebuilding: " + canDoBasebuilding + " canLower: " + canLowerFlag + " canRaise: " + canRaiseFlag, "AdvancedGroups");
		LBLogger.Debug("canCreateSeeATMAccount: " + canCreateSeeATMAccount + " canCreateGroupATMAccount: " + canCreateGroupATMAccount + " canWithdrawGroupMoney: " + canWithdrawGroupMoney + " canDepositGroupMoney: " + canDepositGroupMoney, "AdvancedGroups");
		LBLogger.Debug("promotePower: " + promotePower + " demotePower: " + demotePower + " promoteNeedPower: " + promoteNeedPower + " demoteNeedPower: " + demoteNeedPower, "AdvancedGroups");
		foreach (Param2<int, bool> markerPerms : markerPermissions) {
			LBLogger.Debug("Marker Perm: " + markerPerms.param1 + " : " + markerPerms.param2, "AdvancedGroups");
		}
	}

	void FillInheritedPermissions() {
		LBGroupPermission inherited = LBGroupPermissions.Get.FindPermissionGroupByUID(inheritGroupUID);
		if (inherited) {
			inherited.FillInheritedPermissions();
			FillInherited(inherited);
		}
	}

	LBGroupPermission GetPreviousGroup() {
		return LBGroupPermissions.Get.FindPermissionGroupByUID(previousGroupUID);
	}

	LBGroupPermission GetNextGroup() {
		return LBGroupPermissions.Get.FindPermissionGroupByUID(nextGroupUID);
	}

}
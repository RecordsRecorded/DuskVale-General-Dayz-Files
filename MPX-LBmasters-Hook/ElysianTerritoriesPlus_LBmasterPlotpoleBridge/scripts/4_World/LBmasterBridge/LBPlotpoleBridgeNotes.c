class ETP_LB_PlotpoleBridge
{
	static string GetVersion()
	{
		return "0.3";
	}

	static string GetGoal()
	{
		return "Bridge LB plotpoles into Elysian territory checks without editing either core mod.";
	}

	static bool CanRun()
	{
#ifdef LBmaster_GroupDLCPlotpole
		return GetGame() && LBTerritoryConfig.Get && LBTerritoryConfig.Get.enablePlotpoleExtension;
#endif
		return false;
	}

	static float GetBaseProtectionRadius()
	{
		return GetMultiTerritoriesConfig().GetProtectionRadius();
	}

	static float GetLookupRadius(bool checkTerritoryOverlap = false)
	{
		float baseRadius = GetBaseProtectionRadius();
		float lookupRadius = baseRadius;
		if (checkTerritoryOverlap)
		{
			lookupRadius = baseRadius * 2;
		}

#ifdef LBmaster_GroupDLCPlotpole
		if (CanRun())
		{
			float lbRadius = LBTerritoryConfig.Get.GetMaxRadius();
			if (checkTerritoryOverlap)
			{
				lbRadius = lbRadius + baseRadius;
			}

			lookupRadius = Math.Max(lookupRadius, lbRadius);
		}
#endif

		return lookupRadius;
	}

	static bool IsLBPlotpoleObject(TerritoryFlag flag)
	{
#ifdef LBmaster_GroupDLCPlotpole
		if (!CanRun() || !flag)
		{
			return false;
		}

		if (REV_TerritoryComputer.Cast(flag))
		{
			return false;
		}

		return true;
#endif
		return false;
	}

	static bool IsOwnedLBPlotpole(TerritoryFlag flag)
	{
#ifdef LBmaster_GroupDLCPlotpole
		return IsLBPlotpoleObject(flag) && flag.ETP_LB_IsClaimed();
#endif
		return false;
	}

	static bool IsLBPlotpoleProtectionActive(TerritoryFlag flag)
	{
#ifdef LBmaster_GroupDLCPlotpole
		if (!IsOwnedLBPlotpole(flag))
		{
			return false;
		}

		if (LBTerritoryConfig.Get.treatFullyLoweredPlotpolesLikeUnclaimedPlotpoles && !flag.m_RefresherActive)
		{
			return false;
		}

		return true;
#endif
		return false;
	}

	static bool IsPositionInLBPlotpoleScope(TerritoryFlag flag, vector pos, bool checkTerritoryOverlap = false)
	{
#ifdef LBmaster_GroupDLCPlotpole
		if (!IsLBPlotpoleObject(flag))
		{
			return false;
		}

		float additionalRadius = 0;
		if (checkTerritoryOverlap)
		{
			additionalRadius = GetBaseProtectionRadius();
		}

		return flag.ETP_LB_IsInsidePlotpoleRadius(pos, additionalRadius);
#endif
		return false;
	}

	static bool IsPositionInElysianScope(TerritoryFlag flag, vector pos, bool checkTerritoryOverlap = false)
	{
		if (!flag)
		{
			return false;
		}

		float radius = GetBaseProtectionRadius();
		if (checkTerritoryOverlap)
		{
			radius = radius * 2;
		}

		return vector.Distance(flag.GetPosition(), pos) <= radius;
	}

	static TerritoryFlag FindTerritoryAtPos(vector pos, bool checkTerritoryOverlap = false)
	{
		if (pos == vector.Zero)
		{
			return null;
		}

		array<Object> objects = new array<Object>;
		array<CargoBase> proxyCargos = new array<CargoBase>;
		GetGame().GetObjectsAtPosition(pos, GetLookupRadius(checkTerritoryOverlap), objects, proxyCargos);

		TerritoryFlag bestFlag;
		TerritoryFlag flag;
		float bestDistance = -1;
		float distance;

		for (int i = 0; i < objects.Count(); i++)
		{
			if (!Class.CastTo(flag, objects.Get(i)))
			{
				continue;
			}

			if (IsLBPlotpoleObject(flag))
			{
				if (!IsPositionInLBPlotpoleScope(flag, pos, checkTerritoryOverlap))
				{
					continue;
				}
			}
			else if (!IsPositionInElysianScope(flag, pos, checkTerritoryOverlap))
			{
				continue;
			}

			distance = vector.DistanceSq(flag.GetPosition(), pos);
			if (!bestFlag || bestDistance < 0 || distance < bestDistance)
			{
				bestFlag = flag;
				bestDistance = distance;
			}
		}

		return bestFlag;
	}

	static TerritoryFlag FindElysianTerritoryAtPos(vector pos, bool checkTerritoryOverlap = false)
	{
		if (pos == vector.Zero)
		{
			return null;
		}

		array<Object> objects = new array<Object>;
		array<CargoBase> proxyCargos = new array<CargoBase>;
		float radius = GetBaseProtectionRadius();
		if (checkTerritoryOverlap)
		{
			radius = radius * 2;
		}

		GetGame().GetObjectsAtPosition(pos, radius, objects, proxyCargos);

		TerritoryFlag bestFlag;
		TerritoryFlag flag;
		float bestDistance = -1;
		float distance;

		for (int i = 0; i < objects.Count(); i++)
		{
			if (!Class.CastTo(flag, objects.Get(i)))
			{
				continue;
			}

			if (IsLBPlotpoleObject(flag))
			{
				continue;
			}

			if (!IsPositionInElysianScope(flag, pos, checkTerritoryOverlap))
			{
				continue;
			}

			distance = vector.DistanceSq(flag.GetPosition(), pos);
			if (!bestFlag || bestDistance < 0 || distance < bestDistance)
			{
				bestFlag = flag;
				bestDistance = distance;
			}
		}

		return bestFlag;
	}

	static string GetPlayerGuid(PlayerBase player)
	{
		if (!player)
		{
			return "";
		}

		if (player.GetIdentity())
		{
			string guid = player.GetIdentity().GetId();
			if (guid != "")
			{
				return guid;
			}

			guid = player.GetIdentity().GetPlainId();
			if (guid != "")
			{
				return guid;
			}
		}

#ifdef LBmaster_GroupDLCPlotpole
		return player.GetMySteamId();
#endif
		return "";
	}

	static int GetElysianPermissionForLBAction(int actionType)
	{
		if (actionType == 0)
		{
			return TerritoryPerm.DEPLOY;
		}

		if (actionType == 1)
		{
			return TerritoryPerm.BUILD;
		}

		if (actionType == 2 || actionType == 3)
		{
			return TerritoryPerm.DISMANTLE;
		}

		return 0;
	}

	static bool CanElysianTerritorySatisfyLBAction(PlayerBase player, vector pos, int actionType, ItemBase itemInHands)
	{
#ifdef LBmaster_GroupDLCPlotpole
		if (!CanRun() || !player || pos == vector.Zero)
		{
			return false;
		}

		int permission = GetElysianPermissionForLBAction(actionType);
		if (permission == 0)
		{
			return false;
		}

		if (FindBlockingLBPlotpole(player, pos))
		{
			return false;
		}

		if (itemInHands && itemInHands.IsInherited(REV_TerritoryComputerKit))
		{
			return true;
		}

		TerritoryFlag territory = FindElysianTerritoryAtPos(pos);
		if (!territory)
		{
			return false;
		}

		string guid = GetPlayerGuid(player);
		if (guid == "")
		{
			return false;
		}

		return territory.CheckPlayerPermission(guid, permission);
#endif
		return false;
	}

	static PlayerBase FindOnlinePlayerByGuid(string guid)
	{
		if (!guid || guid == "")
		{
			return null;
		}

		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);

		for (int i = 0; i < players.Count(); i++)
		{
			PlayerBase player = PlayerBase.Cast(players.Get(i));
			if (!player || !player.GetIdentity())
			{
				continue;
			}

			if (player.GetIdentity().GetId() == guid || player.GetIdentity().GetPlainId() == guid)
			{
				return player;
			}
		}

		return null;
	}

	static bool IsPlayerInOwningLBGroup(TerritoryFlag flag, PlayerBase player)
	{
#ifdef LBmaster_GroupDLCPlotpole
		if (!IsOwnedLBPlotpole(flag) || !player)
		{
			return false;
		}

		LBGroup group = player.GetLBGroup();
		if (!group)
		{
			return false;
		}

		return group.GetTagHash() == flag.ETP_LB_GetOwnerHash();
#endif
		return false;
	}

	static bool IsGuidInOwningLBGroup(TerritoryFlag flag, string guid)
	{
		return IsPlayerInOwningLBGroup(flag, FindOnlinePlayerByGuid(guid));
	}

	static LBGroup GetLBGroupForGuid(string guid)
	{
#ifdef LBmaster_GroupDLCPlotpole
		PlayerBase player = FindOnlinePlayerByGuid(guid);
		if (player)
		{
			return player.GetLBGroup();
		}

		return LBGroupManager.Get().GetPlayersGroup(guid);
#endif
		return null;
	}

	static bool IsGuidInLBGroup(string guid, LBGroup group)
	{
#ifdef LBmaster_GroupDLCPlotpole
		if (!group)
		{
			return false;
		}

		LBGroup playerGroup = GetLBGroupForGuid(guid);
		if (!playerGroup)
		{
			return false;
		}

		return playerGroup.GetTagHash() == group.GetTagHash();
#endif
		return false;
	}

	static bool SharesLBGroupWithElysianTerritory(TerritoryFlag territory, PlayerBase player)
	{
#ifdef LBmaster_GroupDLCPlotpole
		if (!territory || !player)
		{
			return false;
		}

		LBGroup actorGroup = player.GetLBGroup();
		if (!actorGroup)
		{
			return false;
		}

		if (territory.m_TerritoryOwner != "" && IsGuidInLBGroup(territory.m_TerritoryOwner, actorGroup))
		{
			return true;
		}

		TStringArray members = territory.TerritoryMembers();
		if (!members)
		{
			return false;
		}

		for (int i = 0; i < members.Count(); i++)
		{
			if (IsGuidInLBGroup(members.Get(i), actorGroup))
			{
				return true;
			}
		}
#endif
		return false;
	}

	static bool IsElysianLBGroupMember(TerritoryFlag territory, string guid)
	{
#ifdef LBmaster_GroupDLCPlotpole
		return SharesLBGroupWithElysianTerritory(territory, FindOnlinePlayerByGuid(guid));
#endif
		return false;
	}

	static bool HasElysianLBGroupPermission(TerritoryFlag territory, string guid, int permission)
	{
#ifdef LBmaster_GroupDLCPlotpole
		PlayerBase player = FindOnlinePlayerByGuid(guid);
		if (!player)
		{
			return false;
		}

		if (HasLBAdminBypass(player))
		{
			return true;
		}

		return SharesLBGroupWithElysianTerritory(territory, player) && HasMappedLBPermission(player, permission);
#endif
		return false;
	}

	static bool HasElysianPublicPermission(int permission)
	{
		int publicPerms = GetMultiTerritoriesConfig().PublicPermission();
		int permsCheck = publicPerms & permission;
		return permsCheck == permission;
	}

	static bool HasLBAdminBypass(PlayerBase player)
	{
#ifdef LBmaster_GroupDLCPlotpole
		if (!player)
		{
			return false;
		}

		return LBAdmins.Get().HasPermissionActive("groups.build.enemy", player);
#endif
		return false;
	}

	static bool HasMappedLBPermission(PlayerBase player, int permission)
	{
#ifdef LBmaster_GroupDLCPlotpole
		if (!player)
		{
			return false;
		}

		LBGroupPermission perms = player.GetPermission();

		if ((permission & TerritoryPerm.ADMIN) == TerritoryPerm.ADMIN)
		{
			if (!perms)
			{
				return false;
			}

			return perms.canInvite || perms.canDemote || perms.canPackPlotpole;
		}

		if ((permission & TerritoryPerm.ADDMEMBER) == TerritoryPerm.ADDMEMBER)
		{
			if (!perms)
			{
				return false;
			}

			return perms.canInvite;
		}

		if ((permission & TerritoryPerm.REMOVEMEMBER) == TerritoryPerm.REMOVEMEMBER)
		{
			if (!perms)
			{
				return false;
			}

			return perms.canInvite || perms.canDemote;
		}

		if ((permission & TerritoryPerm.LOWERFLAG) == TerritoryPerm.LOWERFLAG)
		{
			if (!perms)
			{
				return false;
			}

			return perms.canLowerFlag;
		}

		if ((permission & TerritoryPerm.DEPLOY) == TerritoryPerm.DEPLOY)
		{
			return true;
		}

		if ((permission & TerritoryPerm.BUILD) == TerritoryPerm.BUILD)
		{
			return true;
		}

		if ((permission & TerritoryPerm.DISMANTLE) == TerritoryPerm.DISMANTLE)
		{
			return true;
		}

		if ((permission & TerritoryPerm.DEPLOYSPECIAL) == TerritoryPerm.DEPLOYSPECIAL)
		{
			return true;
		}

		if ((permission & TerritoryPerm.REMOVECARCOVER) == TerritoryPerm.REMOVECARCOVER)
		{
			return true;
		}
#endif
		return false;
	}

	static bool IsUnclaimedLBActionAllowed(int permission)
	{
#ifdef LBmaster_GroupDLCPlotpole
		if ((permission & TerritoryPerm.DISMANTLE) == TerritoryPerm.DISMANTLE)
		{
			return LBTerritoryConfig.Get.allowDismantleInUnclaimedTeritorry;
		}

		if ((permission & TerritoryPerm.LOWERFLAG) == TerritoryPerm.LOWERFLAG)
		{
			return LBTerritoryConfig.Get.allowDestroyInUnclaimedTeritorry;
		}

		if ((permission & TerritoryPerm.ADMIN) == TerritoryPerm.ADMIN)
		{
			return false;
		}

		return LBTerritoryConfig.Get.allowBuildInUnclaimedTeritorry;
#endif
		return true;
	}

	static bool CheckLBPlotpolePermission(TerritoryFlag flag, string guid, int permission)
	{
		GetMultiTerritoriesConfig().DiagnosticLog("[ETP_LB] CheckLBPlotpolePermission guid=" + guid + " permission=" + permission);

		if (!guid || guid == "")
		{
			return false;
		}

		if (GetMultiTerritoriesConfig().ServerAdmins.Find(guid) != -1)
		{
			return true;
		}

#ifdef LBmaster_GroupDLCPlotpole
		if (!IsOwnedLBPlotpole(flag))
		{
			return IsUnclaimedLBActionAllowed(permission);
		}

		if (!IsLBPlotpoleProtectionActive(flag))
		{
			return IsUnclaimedLBActionAllowed(permission);
		}

		PlayerBase player = FindOnlinePlayerByGuid(guid);
		if (HasLBAdminBypass(player))
		{
			return true;
		}

		if (IsPlayerInOwningLBGroup(flag, player) && HasMappedLBPermission(player, permission))
		{
			return true;
		}

		return HasElysianPublicPermission(permission);
#endif
		return true;
	}

	static TerritoryFlag FindBlockingLBPlotpole(PlayerBase player, vector pos)
	{
#ifdef LBmaster_GroupDLCPlotpole
		if (!CanRun() || !player || !player.GetIdentity() || !TerritoryFlag.all_Flags)
		{
			return null;
		}

		TerritoryFlag bestFlag;
		float bestDistance = -1;
		float distance;

		foreach (TerritoryFlag flag : TerritoryFlag.all_Flags)
		{
			if (!flag || !IsLBPlotpoleProtectionActive(flag))
			{
				continue;
			}

			if (!flag.ETP_LB_IsInsidePlotpoleRadius(pos))
			{
				continue;
			}

			if (IsGuidInOwningLBGroup(flag, player.GetIdentity().GetId()))
			{
				continue;
			}

			distance = vector.DistanceSq(flag.GetPosition(), pos);
			if (!bestFlag || bestDistance < 0 || distance < bestDistance)
			{
				bestFlag = flag;
				bestDistance = distance;
			}
		}

		return bestFlag;
#endif
		return null;
	}

	static bool IsSafeTeleportDestination(PlayerBase player, vector pos)
	{
		return FindBlockingLBPlotpole(player, pos) == null;
	}

	static void RemoveElysianFlagLocation(vector pos)
	{
		if (!TerritoryFlag.FLAG_LOCATIONS)
		{
			return;
		}

		for (int i = TerritoryFlag.FLAG_LOCATIONS.Count() - 1; i >= 0; i--)
		{
			if (vector.Distance(TerritoryFlag.FLAG_LOCATIONS.Get(i), pos) < 0.5)
			{
				TerritoryFlag.FLAG_LOCATIONS.Remove(i);
			}
		}
	}
}

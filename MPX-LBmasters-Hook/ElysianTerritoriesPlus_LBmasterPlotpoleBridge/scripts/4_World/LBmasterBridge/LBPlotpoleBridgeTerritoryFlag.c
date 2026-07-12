#ifdef LBmaster_GroupDLCPlotpole
modded class TerritoryFlag
{
	override void EEInit()
	{
		super.EEInit();

		if (GetGame() && GetGame().IsServer() && ETP_LB_PlotpoleBridge.IsLBPlotpoleObject(this))
		{
			ETP_LB_PlotpoleBridge.RemoveElysianFlagLocation(GetPosition());
		}
	}

	override void AfterStoreLoad()
	{
		super.AfterStoreLoad();

		if (GetGame() && GetGame().IsServer() && ETP_LB_PlotpoleBridge.IsLBPlotpoleObject(this))
		{
			ETP_LB_PlotpoleBridge.RemoveElysianFlagLocation(GetPosition());
		}
	}

	bool ETP_LB_IsBridgeCandidate()
	{
		return ETP_LB_PlotpoleBridge.IsLBPlotpoleObject(this);
	}

	string ETP_LB_GetOwnerKey()
	{
		return ownerGroupTag;
	}

	int ETP_LB_GetOwnerHash()
	{
		return ownerGroupTagHash;
	}

	bool ETP_LB_IsClaimed()
	{
		return ownerGroupTagHash != 0;
	}

	bool ETP_LB_IsInsidePlotpoleRadius(vector pos, float additionalRadius = 0)
	{
		return IsInRadius(pos, additionalRadius);
	}

	float ETP_LB_GetPlotpoleRadius()
	{
		float radius;
		float down;
		float up;
		GetPlotpoleSize(radius, down, up);
		return radius;
	}

	bool IsTerritoryOwner(string guid)
	{
		if (ETP_LB_PlotpoleBridge.IsOwnedLBPlotpole(this))
		{
			return ETP_LB_PlotpoleBridge.IsGuidInOwningLBGroup(this, guid);
		}

		if (!m_TerritoryOwner || m_TerritoryOwner == "")
		{
			return false;
		}

		return m_TerritoryOwner == guid;
	}

	bool CanReceiveNewOwner()
	{
		if (ETP_LB_PlotpoleBridge.IsOwnedLBPlotpole(this))
		{
			return false;
		}

		if (!m_TerritoryOwner || m_TerritoryOwner == "")
		{
			return true;
		}

		return false;
	}

	bool IsTerritoryMember(string guid)
	{
		if (ETP_LB_PlotpoleBridge.IsLBPlotpoleObject(this))
		{
			if (!ETP_LB_IsClaimed())
			{
				return true;
			}

			if (!ETP_LB_PlotpoleBridge.IsLBPlotpoleProtectionActive(this))
			{
				return true;
			}

			return ETP_LB_PlotpoleBridge.IsGuidInOwningLBGroup(this, guid);
		}

		if (m_TerritoryOwner == "")
		{
			return true;
		}

		if (guid == m_TerritoryOwner)
		{
			return true;
		}

		if (m_TerritoryMembers && m_TerritoryMembers.CheckId(guid))
		{
			return true;
		}

		if (ETP_LB_PlotpoleBridge.IsElysianLBGroupMember(this, guid))
		{
			return true;
		}

		return false;
	}

	bool IsTerritoryProtectionActive(bool refreshComputerState = true)
	{
		if (ETP_LB_PlotpoleBridge.IsLBPlotpoleObject(this))
		{
			return ETP_LB_PlotpoleBridge.IsLBPlotpoleProtectionActive(this);
		}

		REV_TerritoryComputer computer = REV_TerritoryComputer.Cast(this);
		if (computer)
		{
			if (refreshComputerState && GetGame().IsServer())
			{
				computer.SynchronizeRefresherTime();
			}

			bool active = computer.CanTerritoryBeActive();
			GetMultiTerritoriesConfig().DiagnosticLog("IsTerritoryProtectionActive computer active=" + active);
			return active;
		}

		if (FindAttachmentBySlotName("Material_FPole_Flag"))
		{
			float state = GetAnimationPhase("flag_mast");
			if (state <= TerritoryConst.FLAGDOWNSTATE)
			{
				return true;
			}
		}

		return false;
	}

	bool HasRaisedFlag()
	{
		return IsTerritoryProtectionActive(true);
	}

	bool CheckPlayerPermission(string guid, int permission)
	{
		if (ETP_LB_PlotpoleBridge.IsLBPlotpoleObject(this))
		{
			return ETP_LB_PlotpoleBridge.CheckLBPlotpolePermission(this, guid, permission);
		}

		GetMultiTerritoriesConfig().DiagnosticLog("CheckPlayerPermission guid=" + guid + " permission=" + permission);

		if (guid == "")
		{
			GetMultiTerritoriesConfig().DiagnosticLog("Empty guid - denying");
			return false;
		}

		int publicPerms = GetMultiTerritoriesConfig().PublicPermission();
		int permsCheck = publicPerms & permission;
		bool territoryActive = IsTerritoryProtectionActive(true);

		GetMultiTerritoriesConfig().DiagnosticLog("publicPerms=" + publicPerms + " PermsCheck=" + permsCheck + " territoryActive=" + territoryActive);

		if (GetMultiTerritoriesConfig().ServerAdmins.Find(guid) != -1)
		{
			GetMultiTerritoriesConfig().DiagnosticLog("Is server admin - allowing");
			return true;
		}

		if (guid == m_TerritoryOwner && m_TerritoryOwner != "")
		{
			GetMultiTerritoriesConfig().DiagnosticLog("Is owner=" + m_TerritoryOwner + " - allowing");
			return true;
		}

		if (!territoryActive)
		{
			GetMultiTerritoriesConfig().DiagnosticLog("Territory inactive - allowing action");
			return true;
		}

		if (m_TerritoryMembers && m_TerritoryMembers.Check(guid, permission))
		{
			GetMultiTerritoriesConfig().DiagnosticLog("Territory active and member permission matched - allowing");
			return true;
		}

		if (ETP_LB_PlotpoleBridge.HasElysianLBGroupPermission(this, guid, permission))
		{
			GetMultiTerritoriesConfig().DiagnosticLog("Territory active and LB group permission matched - allowing");
			return true;
		}

		if (permsCheck == permission)
		{
			GetMultiTerritoriesConfig().DiagnosticLog("Territory active but public perms allow action");
			return true;
		}

		GetMultiTerritoriesConfig().DiagnosticLog("Territory active and no permission matched - denying");
		return false;
	}

	static TerritoryFlag GetTerritoryAtPos(vector Pos, bool CheckTerritoryOverlap = false)
	{
		return ETP_LB_PlotpoleBridge.FindTerritoryAtPos(Pos, CheckTerritoryOverlap);
	}

	static bool HasTerritoryPermAtPos(string GUID, int Perm, vector Pos, bool CheckTerritoryOverlap = false)
	{
		if (GUID == "")
		{
			return false;
		}

		int curTime = GetGame().GetTime();
		m_LastCheckLocation = Pos;
		m_LastCheckLocationNextTime = curTime + 100;

		if (Pos == vector.Zero)
		{
			m_CachedHasTerritoryPerm = false;
			return m_CachedHasTerritoryPerm;
		}

		TerritoryFlag theFlag = GetTerritoryAtPos(Pos, CheckTerritoryOverlap);
		if (theFlag)
		{
			if (GetGame().IsClient())
			{
				theFlag.SyncTerritoryRateLimited();
			}

			if (CheckTerritoryOverlap)
			{
				m_CachedHasTerritoryPerm = false;
				return false;
			}

			m_CachedHasTerritoryPerm = theFlag.CheckPlayerPermission(GUID, Perm);
			return m_CachedHasTerritoryPerm;
		}

		m_CachedHasTerritoryPerm = true;
		return m_CachedHasTerritoryPerm;
	}
}
#endif

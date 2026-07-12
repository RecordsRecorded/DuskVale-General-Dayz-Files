modded class TerritoryFlag extends BaseBuildingBase
{
	// Store territory flag locations
	static ref array<vector> FLAG_LOCATIONS = new array<vector>;

	protected bool m_CanAddMember				= false;
	protected bool m_AwaitingReset				= false;
	string m_TerritoryOwner						= "";
	protected bool m_isRequestingSync			= false;
	protected int m_LastSyncTime				= 0;
	ref MultiTerritoryMembers m_TerritoryMembers = new MultiTerritoryMembers;
	
void TerritoryFlag()
{
	RegisterNetSyncVariableBool("m_CanAddMember");
	// m_TerritoryOwner and m_TerritoryMembers are complex; we sync via RPC
}

	void ~TerritoryFlag()
	{
		if (m_isRequestingSync)
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.DoFirstSync);
		}
	}
	
	override void EEInit()
	{
		super.EEInit();

		if (GetGame().IsClient())
		{
			DayZPlayer player = DayZPlayer.Cast(GetGame().GetPlayer());
			if (player)
			{
				m_isRequestingSync = true;
				int Distance = vector.Distance(player.GetPosition(), GetPosition()) + 1;
				int Time = Distance * 5;
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.DoFirstSync, Time, false);
			}

			AnimateFlag(1 - GetRefresherTime01());
		}
	}

	TStringArray TerritoryMembers()
	{
		return m_TerritoryMembers.GetMemberArray();
	}

	void DoFirstSync()
	{
		m_isRequestingSync = false;
		SyncTerritory();
	}

	bool IsTerritoryOwner(string guid)
	{
		if (!m_TerritoryOwner || m_TerritoryOwner == "")
		{
			return false;
		}

		return (m_TerritoryOwner == guid);
	}

	string GetTerritoryOwnerId()
	{
		return m_TerritoryOwner;
	}

	string ResolveIdentityName(string guid)
	{
		if (!guid || guid == "")
		{
			return "Unclaimed";
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

			if (player.GetIdentity().GetId() == guid)
			{
				string liveName = player.GetIdentity().GetName();
				MultiTerritoryIdentityCache.RememberName(guid, liveName);
				return liveName;
			}
		}

		string cachedName = MultiTerritoryIdentityCache.GetName(guid);
		if (cachedName != "")
		{
			return cachedName;
		}

		return guid;
	}

	PlayerBase FindOnlinePlayerByGuid(string guid)
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

			if (player.GetIdentity().GetId() == guid)
			{
				return player;
			}
		}

		return null;
	}

	string GetTerritoryOwnerDisplayName()
	{
		return ResolveIdentityName(m_TerritoryOwner);
	}

	string GetMembersDisplayText()
	{
		TStringArray members = TerritoryMembers();
		if (!members || members.Count() == 0)
		{
			return "None";
		}

		string output = "";
		for (int i = 0; i < members.Count(); i++)
		{
			if (i > 0)
			{
				output += "\n";
			}

			output += ResolveIdentityName(members.Get(i));
		}

		return output;
	}
	
	bool CanReceiveNewOwner()
	{
		if (!m_TerritoryOwner || m_TerritoryOwner == "")
		{
			return true;
		}

		return false;
	}
	
	int GetMemberCount()
	{
		return TerritoryMembers().Count();
	}

	int GetMemberPermissions(string guid)
	{
		if (!guid || guid == "")
		{
			return 0;
		}

		if (guid == m_TerritoryOwner)
		{
			return TerritoryPerm.ADMIN | TerritoryPerm.DEPLOY | TerritoryPerm.BUILD | TerritoryPerm.DISMANTLE | TerritoryPerm.LOWERFLAG | TerritoryPerm.ADDMEMBER | TerritoryPerm.REMOVEMEMBER | TerritoryPerm.DEPLOYSPECIAL | TerritoryPerm.REMOVECARCOVER;
		}

		if (!m_TerritoryMembers)
		{
			return 0;
		}

		return m_TerritoryMembers.GetPermissions(guid);
	}

	static int GetAllPermissionBits()
	{
		return TerritoryPerm.ADMIN | TerritoryPerm.DEPLOY | TerritoryPerm.BUILD | TerritoryPerm.DISMANTLE | TerritoryPerm.LOWERFLAG | TerritoryPerm.ADDMEMBER | TerritoryPerm.REMOVEMEMBER | TerritoryPerm.DEPLOYSPECIAL | TerritoryPerm.REMOVECARCOVER;
	}

	static bool IsSingleValidPermissionBit(int bit)
	{
		if (bit == 0)
		{
			return false;
		}

		if ((bit & (bit - 1)) != 0)
		{
			return false;
		}

		if ((bit & GetAllPermissionBits()) != bit)
		{
			return false;
		}

		return true;
	}

	void ResetMembers()
	{
		GetMultiTerritoriesConfig().DebugLog("Territory members reset");
		m_TerritoryMembers.Clear();
		SyncTerritory();
	}

	void ResetTerritoryAdministration()
	{
		GetMultiTerritoriesConfig().DebugLog("Territory administration reset");
		m_TerritoryOwner = "";
		m_TerritoryMembers.Clear();
		AllowMemberToBeAdded(false);
		SyncTerritory();
	}
	
	bool CanAddMember()
	{
		return m_CanAddMember;
	}
	
	void SetTerritoryOwner(string guid)
	{
		m_TerritoryOwner = guid;
		PlayerBase ownerPlayer = FindOnlinePlayerByGuid(guid);
		if (ownerPlayer && ownerPlayer.GetIdentity())
		{
			MultiTerritoryIdentityCache.RememberName(guid, ownerPlayer.GetIdentity().GetName());
		}
		SyncTerritory();
	}
	
	void AllowMemberToBeAdded(bool state = true)
	{
		m_CanAddMember = state;
		if (m_AwaitingReset && GetGame().IsServer())
		{
			m_AwaitingReset = false;
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.ResetAllowMemberToBeAdded);
		}

		if (state && GetGame().IsServer())
		{
			m_AwaitingReset = true;
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.ResetAllowMemberToBeAdded, 300 * 1000);
		}

		SetSynchDirty();
	}
	
	void ResetAllowMemberToBeAdded()
	{
		m_AwaitingReset = false;
		m_CanAddMember = false;
		SetSynchDirty();
	}
	
	void AddMember(string guid)
	{
		if (guid == m_TerritoryOwner)
		{
			return;
		}

		if (GetMultiTerritoriesConfig().HasMemberLimit() && m_TerritoryMembers.Count() >= GetMultiTerritoriesConfig().MaxMembersPerTerritory)
		{
			GetMultiTerritoriesConfig().DiagnosticLog("Member limit reached for territory owner=" + m_TerritoryOwner + " max=" + GetMultiTerritoriesConfig().MaxMembersPerTerritory);
			return;
		}
		else 
		{
			PlayerBase memberPlayer = FindOnlinePlayerByGuid(guid);
			if (memberPlayer && memberPlayer.GetIdentity())
			{
				MultiTerritoryIdentityCache.RememberName(guid, memberPlayer.GetIdentity().GetName());
			}

			m_TerritoryMembers.AddMember(guid);
			AllowMemberToBeAdded(false);
			SyncTerritory();
		}
	}
	
	void RemoveMember(string guid)
	{
		if (guid == m_TerritoryOwner)
		{
			return;
		} else 
		{
			m_TerritoryMembers.RemoveMember(guid);
			SyncTerritory();
		}
	}
	
	bool IsTerritoryMember(string guid)
	{
		if (m_TerritoryOwner == "")
		{
			return true;
		} else 
		if (guid == m_TerritoryOwner)
		{
			return true;
		} else 
		if (m_TerritoryMembers.CheckId(guid))
		{
			return true;
		} else 
		{
			return false;
		}
	}
	
	void AddMemberClient(string guid)
	{
		if (CanAddMember())
		{
			RPCSingleParam(MULTITERRITORYRPCs.ADD_MEMBER, new Param2< string, MultiTerritoryMembers >(guid, m_TerritoryMembers), true, NULL);
		}
	}

	protected string DebugOwnerSummary()
	{
		if (!m_TerritoryOwner || m_TerritoryOwner == "")
		{
			return "UNCLAIMED";
		}

		return m_TerritoryOwner;
	}

	protected string DebugMembersSummary()
	{
		if (!m_TerritoryMembers)
		{
			return "NULL";
		}

		return m_TerritoryMembers.DebugDump();
	}

	protected void DebugLogRestartState(string stage)
	{
		if (!GetGame() || !GetGame().IsServer())
		{
			return;
		}

		GetMultiTerritoriesConfig().DiagnosticLog("[TerritoryRestartDebug] stage=" + stage + " pos=" + GetPosition().ToString() + " owner=" + DebugOwnerSummary() + " memberCount=" + GetMemberCount().ToString() + " members=" + DebugMembersSummary() + " canReceiveNewOwner=" + CanReceiveNewOwner().ToString() + " canAddMember=" + CanAddMember().ToString());
	}
	
	override void OnStoreSave(ParamsWriteContext ctx)
	{   
		super.OnStoreSave(ctx);
		DebugLogRestartState("OnStoreSave_beforeWrite");
		ctx.Write(m_TerritoryOwner);
		ctx.Write(m_TerritoryMembers);
	}
	
	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		if (!super.OnStoreLoad(ctx, version)) 
		{
			GetMultiTerritoriesConfig().DiagnosticLog("[TerritoryRestartDebug] stage=OnStoreLoad_superFailed pos=" + GetPosition().ToString());
			return false;
		}

		if (!ctx.Read(m_TerritoryOwner)) 
		{
			GetMultiTerritoriesConfig().DiagnosticLog("[TerritoryRestartDebug] stage=OnStoreLoad_ownerReadFailed pos=" + GetPosition().ToString());
			return false;
		}

		if (!ctx.Read(m_TerritoryMembers)) 
		{
			GetMultiTerritoriesConfig().DiagnosticLog("[TerritoryRestartDebug] stage=OnStoreLoad_membersReadFailed pos=" + GetPosition().ToString() + " owner=" + DebugOwnerSummary());
			return false;
		}
		
		if (GetGame().IsServer())
		{
			DebugLogRestartState("OnStoreLoad_afterRead");
		}
		
		return true;
	}
	
	override void AfterStoreLoad()
	{
		super.AfterStoreLoad();

		if (GetGame().IsDedicatedServer())
		{
			FLAG_LOCATIONS.Insert(GetPosition());
			DebugLogRestartState("AfterStoreLoad_immediate");
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.DebugLogAfterRestartDelay, 3000, false);
		}
	}

	void DebugLogAfterRestartDelay()
	{
		DebugLogRestartState("AfterStoreLoad_delayed3s");
	}

void SyncTerritoryRateLimited()
{
	if (GetGame().IsServer()) 
	{
		return;
	}

	int curTime = GetGame().GetTime();
	if (m_LastSyncTime < curTime)
	{
		m_LastSyncTime = curTime + 10000; // 10 seconds
		MultiModLogger.Log("SyncTerritoryRateLimited: Triggering sync for client", "baseraid");
		SyncTerritory();
	}
}
	
	void SyncTerritory(PlayerIdentity identity = NULL)
	{
		if (GetGame().IsServer()) 
		{
			TStringArray nameLookupGuids = new TStringArray;
			if (m_TerritoryOwner && m_TerritoryOwner != "")
			{
				nameLookupGuids.Insert(m_TerritoryOwner);
			}

			TStringArray members = TerritoryMembers();
			for (int i = 0; i < members.Count(); i++)
			{
				nameLookupGuids.Insert(members.Get(i));
			}

			TStringArray cachedGuids;
			TStringArray cachedNames;
			MultiTerritoryIdentityCache.BuildNameArrays(nameLookupGuids, cachedGuids, cachedNames);

			SetSynchDirty();
			RPCSingleParam(MULTITERRITORYRPCs.SEND_DATA, new Param2< string, MultiTerritoryMembers >(m_TerritoryOwner, m_TerritoryMembers), true, identity);
			RPCSingleParam(MULTITERRITORYRPCs.SEND_IDENTITY_NAMES, new Param2< TStringArray, TStringArray >(cachedGuids, cachedNames), true, identity);
		} else 
		if (GetGame().IsClient())
		{
			RPCSingleParam(MULTITERRITORYRPCs.REQUEST_DATA, new Param1<bool>(true), true, NULL);
		}
	}
	
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);
		Param2< string, MultiTerritoryMembers > data;
		Param2< TStringArray, TStringArray > identityNames;
		if (rpc_type == MULTITERRITORYRPCs.SEND_DATA && GetGame().IsClient()) 
		{
			if (ctx.Read(data))	
			{
				m_TerritoryOwner = data.param1;
				m_TerritoryMembers = MultiTerritoryMembers.Cast(data.param2);
			}

			return;
		}

		if (rpc_type == MULTITERRITORYRPCs.SEND_IDENTITY_NAMES && GetGame().IsClient())
		{
			if (ctx.Read(identityNames))
			{
				MultiTerritoryIdentityCache.ReceiveNameArrays(identityNames.param1, identityNames.param2);
			}

			return;
		}

		if (rpc_type == MULTITERRITORYRPCs.REQUEST_DATA && GetGame().IsServer()) 
		{
			if (sender)
			{
				SyncTerritory(sender);
			} else 
			{
				SyncTerritory();
			}

			return;
		}

		if (rpc_type == MULTITERRITORYRPCs.ADD_MEMBER && GetGame().IsServer())
		{
			if (ctx.Read(data))
			{
				if (CanAddMember() && sender)
				{
					if (TerritoryMembers().Count() == data.param2.Count() && sender.GetId() == data.param1) // To ensure client and server were in sync
					{ 
						AddMember(data.param1);
						AllowMemberToBeAdded(false);
					}

					SyncTerritory();
				}
			}

			return;
		}

		if (GetGame().IsServer() && sender)
		{
			string guid = sender.GetId();
			PlayerBase senderPlayer = FindOnlinePlayerByGuid(guid);
			bool isServerAdmin = GetMultiTerritoriesConfig().ServerAdmins.Find(guid) != -1;
			bool isOwner = IsTerritoryOwner(guid);
			bool canSetOwner = CanReceiveNewOwner();
			bool canResetOwner = !canSetOwner && (isServerAdmin || isOwner) && GetMemberCount() == 0;
			bool canResetMembers = !canResetOwner && CheckPlayerPermission(guid, TerritoryPerm.REMOVEMEMBER);

			if (rpc_type == MULTITERRITORYRPCs.MENU_TOGGLE_INVITE)
			{
				if (!CheckPlayerPermission(guid, TerritoryPerm.ADDMEMBER))
				{
					return;
				}

				bool addingMember = !CanAddMember();
				if (addingMember && GetMultiTerritoriesConfig().HasMemberLimit() && GetMemberCount() >= GetMultiTerritoriesConfig().MaxMembersPerTerritory)
				{
					if (senderPlayer)
					{
						MultiFunctions.SendPlayerMessage(senderPlayer, "Territory member limit reached.");
					}
					return;
				}

				AllowMemberToBeAdded(addingMember);
				REV_TerritoryComputer inviteComputer = REV_TerritoryComputer.Cast(this);
				if (inviteComputer)
				{
					inviteComputer.UpdateIndicatorLights();
				}
				SyncTerritory();
				return;
			}

			if (rpc_type == MULTITERRITORYRPCs.MENU_JOIN_OR_LEAVE)
			{
				if (!CanAddMember() && (!IsTerritoryMember(guid) || IsTerritoryOwner(guid)))
				{
					return;
				}

				if (!IsTerritoryMember(guid))
				{
					if (GetMultiTerritoriesConfig().HasMemberLimit() && GetMemberCount() >= GetMultiTerritoriesConfig().MaxMembersPerTerritory)
					{
						if (senderPlayer)
						{
							MultiFunctions.SendPlayerMessage(senderPlayer, "This territory is already at the member limit.");
						}
						return;
					}

					AddMember(guid);
					if (senderPlayer)
					{
						MultiFunctions.SendPlayerMessage(senderPlayer, "You have been accepted into the Database!");
					}
				}
				else if (!IsTerritoryOwner(guid))
				{
					RemoveMember(guid);
					if (senderPlayer)
					{
						MultiFunctions.SendPlayerMessage(senderPlayer, "You have been removed from the Database.");
					}
				}

				REV_TerritoryComputer membershipComputer = REV_TerritoryComputer.Cast(this);
				if (membershipComputer)
				{
					membershipComputer.UpdateIndicatorLights();
				}
				SyncTerritory();
				return;
			}

			if (rpc_type == MULTITERRITORYRPCs.MENU_RESET_MEMBERS)
			{
				if (!canResetMembers && !CheckPlayerPermission(guid, TerritoryPerm.ADMIN))
				{
					return;
				}

				ResetMembers();
				REV_TerritoryComputer resetMembersComputer = REV_TerritoryComputer.Cast(this);
				if (resetMembersComputer)
				{
					resetMembersComputer.UpdateIndicatorLights();
				}
				SyncTerritory();
				return;
			}

			if (rpc_type == MULTITERRITORYRPCs.MENU_RESET_ADMIN)
			{
				if (!canResetOwner)
				{
					return;
				}

				ResetTerritoryAdministration();
				REV_TerritoryComputer resetAdminComputer = REV_TerritoryComputer.Cast(this);
				if (resetAdminComputer)
				{
					resetAdminComputer.UpdateIndicatorLights();
				}
				SyncTerritory();
				return;
			}

			if (rpc_type == MULTITERRITORYRPCs.MENU_CLAIM_ADMIN)
			{
				if (!canSetOwner)
				{
					return;
				}

				SetTerritoryOwner(guid);
				if (senderPlayer)
				{
					MultiFunctions.SendPlayerMessage(senderPlayer, "Territory claimed! Others can't build nearby, and your base stays longer while the computer is active.");
				}
				REV_TerritoryComputer claimComputer = REV_TerritoryComputer.Cast(this);
				if (claimComputer)
				{
					claimComputer.UpdateIndicatorLights();
				}
				SyncTerritory();
				return;
			}

			if (rpc_type == MULTITERRITORYRPCs.MENU_EJECT_FLOPPY)
			{
				REV_TerritoryComputer ejectComputer = REV_TerritoryComputer.Cast(this);
				if (!ejectComputer)
				{
					return;
				}

				// Only the territory owner (or a server admin) may remove the floppydisk
				if (!canSetOwner && !isOwner && !isServerAdmin)
				{
					return;
				}

				if (!ejectComputer.FindAttachmentBySlotName("Material_FPole_Flag"))
				{
					return;
				}

				ejectComputer.EjectFloppy(senderPlayer);
				SyncTerritory();
				return;
			}

			if (rpc_type == MULTITERRITORYRPCs.MENU_EDIT_PERMISSION)
			{
				Param3<string, int, bool> editData;
				if (!ctx.Read(editData))
				{
					return;
				}

				if (!isOwner && !isServerAdmin)
				{
					GetMultiTerritoriesConfig().DiagnosticLog("[PermEdit] Rejected: sender " + guid + " is not owner or admin");
					return;
				}

				string editTargetGuid = editData.param1;
				int permBit = editData.param2;
				bool grant = editData.param3;

				if (!editTargetGuid || editTargetGuid == "")
				{
					return;
				}

				if (editTargetGuid == m_TerritoryOwner)
				{
					GetMultiTerritoriesConfig().DiagnosticLog("[PermEdit] Rejected: cannot edit owner permissions");
					return;
				}

				if (!m_TerritoryMembers || !m_TerritoryMembers.CheckId(editTargetGuid))
				{
					GetMultiTerritoriesConfig().DiagnosticLog("[PermEdit] Rejected: target " + editTargetGuid + " is not a member");
					return;
				}

				if (!IsSingleValidPermissionBit(permBit))
				{
					GetMultiTerritoriesConfig().DiagnosticLog("[PermEdit] Rejected: invalid permission bit " + permBit);
					return;
				}

				if (permBit == TerritoryPerm.ADMIN && grant)
				{
					GetMultiTerritoriesConfig().DiagnosticLog("[PermEdit] Rejected: cannot grant ADMIN to non-owner");
					return;
				}

				int currentPerms = m_TerritoryMembers.GetPermissions(editTargetGuid);
				int newPerms;
				if (grant)
				{
					newPerms = currentPerms | permBit;
				}
				else
				{
					newPerms = currentPerms & ~permBit;
				}

				m_TerritoryMembers.SetPermissions(editTargetGuid, newPerms);
				GetMultiTerritoriesConfig().DiagnosticLog("[PermEdit] Applied: target=" + editTargetGuid + " bit=" + permBit + " grant=" + grant + " newPerms=" + newPerms);
				SyncTerritory();
				return;
			}

			if (rpc_type == MULTITERRITORYRPCs.MENU_KICK_MEMBER)
			{
				Param1<string> kickData;
				if (!ctx.Read(kickData))
				{
					return;
				}

				string kickTargetGuid = kickData.param1;

				if (!kickTargetGuid || kickTargetGuid == "")
				{
					return;
				}

				if (kickTargetGuid == m_TerritoryOwner)
				{
					return;
				}

				if (!isOwner && !isServerAdmin && !CheckPlayerPermission(guid, TerritoryPerm.REMOVEMEMBER))
				{
					GetMultiTerritoriesConfig().DiagnosticLog("[Kick] Rejected: sender " + guid + " lacks REMOVEMEMBER permission");
					return;
				}

				if (!m_TerritoryMembers || !m_TerritoryMembers.CheckId(kickTargetGuid))
				{
					return;
				}

				RemoveMember(kickTargetGuid);
				GetMultiTerritoriesConfig().DiagnosticLog("[Kick] Removed: target=" + kickTargetGuid + " by=" + guid);
				SyncTerritory();
				return;
			}

			// Server-side vitals push. Client menu sends this on Init/Refresh; we read
			// the sender's PlayerBase stats (GetHealth/GetStatWater/etc work fine
			// server-side, unlike client-side where they return 0 in MP) and ship the
			// percentages back. Pattern lifted from VPPAdminTools/PlayerManager.c:537.
			if (rpc_type == MULTITERRITORYRPCs.MENU_REQUEST_VITALS)
			{
				if (!senderPlayer) return;

				float hpVal    = senderPlayer.GetHealth("GlobalHealth", "");
				float hpMax    = senderPlayer.GetMaxHealth("GlobalHealth", "");
				float blVal    = senderPlayer.GetHealth("GlobalHealth", "Blood");
				float blMax    = senderPlayer.GetMaxHealth("GlobalHealth", "Blood");
				float waVal    = 0;
				float waMax    = 1;
				float enVal    = 0;
				float enMax    = 1;
				if (senderPlayer.GetStatWater())
				{
					waVal = senderPlayer.GetStatWater().Get();
					waMax = senderPlayer.GetStatWater().GetMax();
				}
				if (senderPlayer.GetStatEnergy())
				{
					enVal = senderPlayer.GetStatEnergy().Get();
					enMax = senderPlayer.GetStatEnergy().GetMax();
				}

				int hpPct = 0;
				int blPct = 0;
				int waPct = 0;
				int enPct = 0;
				if (hpMax > 0) hpPct = Math.Round(hpVal / hpMax * 100);
				if (blMax > 0) blPct = Math.Round(blVal / blMax * 100);
				if (waMax > 0) waPct = Math.Round(waVal / waMax * 100);
				if (enMax > 0) enPct = Math.Round(enVal / enMax * 100);

				// HEATCOMFORT is a PlayerStat<float> in -1..+1 range
				// (PlayerStatsPCO.c:294: RegisterStat(...HEATCOMFORT...new PlayerStat<float>(-1, 1, 0, "HeatComfort"...)).
				// Map to 0..100 so 50 = perfect comfort, <25 = cold, >75 = hot.
				int tmpPct = 50;
				if (senderPlayer.GetStatHeatComfort())
				{
					float heatComfort = senderPlayer.GetStatHeatComfort().Get();
					tmpPct = Math.Round((heatComfort + 1.0) * 50.0);
					if (tmpPct < 0) tmpPct = 0;
					if (tmpPct > 100) tmpPct = 100;
				}

				// Pack active symptoms + WET stat into a bitmask. SymptomManager.GetSymptomCount
				// returns >0 when an entry is currently in the queue (StateManager.c:405).
				int symBits = 0;
				BleedingSourcesManagerBase bm = senderPlayer.GetBleedingManagerServer();
				if (bm && bm.GetBleedingSourcesCount() > 0) symBits = symBits | MULTITERRITORY_VITAL_FLAGS.VFLAG_BLEED;

				SymptomManager sm = senderPlayer.GetSymptomManager();
				if (sm)
				{
					if (sm.GetSymptomCount(SymptomIDs.SYMPTOM_PAIN_LIGHT)       > 0) symBits = symBits | MULTITERRITORY_VITAL_FLAGS.VFLAG_PAIN;
					if (sm.GetSymptomCount(SymptomIDs.SYMPTOM_PAIN_HEAVY)       > 0) symBits = symBits | MULTITERRITORY_VITAL_FLAGS.VFLAG_PAIN_HEAVY;
					if (sm.GetSymptomCount(SymptomIDs.SYMPTOM_COUGH)            > 0) symBits = symBits | MULTITERRITORY_VITAL_FLAGS.VFLAG_COUGH;
					if (sm.GetSymptomCount(SymptomIDs.SYMPTOM_VOMIT)            > 0) symBits = symBits | MULTITERRITORY_VITAL_FLAGS.VFLAG_VOMIT;
					if (sm.GetSymptomCount(SymptomIDs.SYMPTOM_FEVERBLUR)        > 0) symBits = symBits | MULTITERRITORY_VITAL_FLAGS.VFLAG_FEVER;
					bool isCold = (sm.GetSymptomCount(SymptomIDs.SYMPTOM_FREEZE) > 0) || (sm.GetSymptomCount(SymptomIDs.SYMPTOM_FREEZE_RATTLE) > 0);
					if (isCold) symBits = symBits | MULTITERRITORY_VITAL_FLAGS.VFLAG_COLD;
					if (sm.GetSymptomCount(SymptomIDs.SYMPTOM_HOT)              > 0) symBits = symBits | MULTITERRITORY_VITAL_FLAGS.VFLAG_HOT;
					if (sm.GetSymptomCount(SymptomIDs.SYMPTOM_BLINDNESS)        > 0) symBits = symBits | MULTITERRITORY_VITAL_FLAGS.VFLAG_BLIND;
					if (sm.GetSymptomCount(SymptomIDs.SYMPTOM_UNCONSCIOUS)      > 0) symBits = symBits | MULTITERRITORY_VITAL_FLAGS.VFLAG_UNCNS;
					if (sm.GetSymptomCount(SymptomIDs.SYMPTOM_HMP_SEVERE)       > 0) symBits = symBits | MULTITERRITORY_VITAL_FLAGS.VFLAG_INFCT;
					if (sm.GetSymptomCount(SymptomIDs.SYMPTOM_DEAFNESS_COMPLETE) > 0) symBits = symBits | MULTITERRITORY_VITAL_FLAGS.VFLAG_DEAF;
					if (sm.GetSymptomCount(SymptomIDs.SYMPTOM_HAND_SHIVER)      > 0) symBits = symBits | MULTITERRITORY_VITAL_FLAGS.VFLAG_SHAKE;
				}
				// WET is a stat, not a symptom -- read the PCO directly.
				if (senderPlayer.GetPlayerStats())
				{
					PlayerStatBase wetStat = senderPlayer.GetPlayerStats().GetPCO().GetStatObject(EPlayerStats_current.WET);
					if (wetStat && wetStat.Get() > 0) symBits = symBits | MULTITERRITORY_VITAL_FLAGS.VFLAG_WET;
				}

				RPCSingleParam(MULTITERRITORYRPCs.MENU_VITALS_RESPONSE,
					new Param6<int, int, int, int, int, int>(hpPct, blPct, tmpPct, waPct, enPct, symBits),
					true, sender);
				return;
			}
		}

		// Client-side handler for the vitals push.
		if (rpc_type == MULTITERRITORYRPCs.MENU_VITALS_RESPONSE && GetGame().IsClient())
		{
			Param6<int, int, int, int, int, int> vData;
			if (!ctx.Read(vData)) return;
			MultiplixTerritoryMenuState.s_VitalHpPct       = vData.param1;
			MultiplixTerritoryMenuState.s_VitalBlPct       = vData.param2;
			MultiplixTerritoryMenuState.s_VitalTmpPct      = vData.param3;
			MultiplixTerritoryMenuState.s_VitalWaPct       = vData.param4;
			MultiplixTerritoryMenuState.s_VitalEnPct       = vData.param5;
			MultiplixTerritoryMenuState.s_VitalSymptomBits = vData.param6;
			MultiplixTerritoryMenuState.s_VitalLastUpdate  = GetGame().GetTime();
			return;
		}
	}

bool IsTerritoryProtectionActive(bool refreshComputerState = true)
{
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
		GetMultiTerritoriesConfig().DiagnosticLog("CheckPlayerPermission guid=" + guid + " permission=" + permission);

		if (guid == "")
		{
			GetMultiTerritoriesConfig().DiagnosticLog("Empty guid - denying");
			return false;
		}
		
		int publicPerms = GetMultiTerritoriesConfig().PublicPermission();
		int PermsCheck = publicPerms & permission;
		bool territoryActive = IsTerritoryProtectionActive(true);

		GetMultiTerritoriesConfig().DiagnosticLog("publicPerms=" + publicPerms + " PermsCheck=" + PermsCheck + " territoryActive=" + territoryActive);
		
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
		
		if (m_TerritoryMembers.Check(guid, permission))
		{
			GetMultiTerritoriesConfig().DiagnosticLog("Territory active and member permission matched - allowing");
			return true;
		}

		if (PermsCheck == permission)
		{
			GetMultiTerritoriesConfig().DiagnosticLog("Territory active but public perms allow action");
			return true;
		}
		
		GetMultiTerritoriesConfig().DiagnosticLog("Territory active and no permission matched - denying");
		return false;
	}
	
	
	override void SetActions()
	{
		super.SetActions();

		// Custom flag actions
		AddAction(ActionAddTerritoryMember);
		AddAction(ActionResetTerritoryMembers);
		AddAction(ActionAcceptMembership);
	}

	static vector m_LastCheckLocation = vector.Zero;
	static int m_LastCheckLocationNextTime = 0;
	static bool m_CachedHasTerritoryPerm = false;

	static TerritoryFlag GetTerritoryAtPos(vector Pos, bool CheckTerritoryOverlap = false)
	{
		if (Pos == vector.Zero)
		{
			return null;
		}

		array<Object> objects = new array<Object>;
		array<CargoBase> proxyCargos = new array<CargoBase>;
		float theRadius = GetMultiTerritoriesConfig().GetProtectionRadius();

		if (CheckTerritoryOverlap)
		{
			theRadius = theRadius * 2;
		}

		GetGame().GetObjectsAtPosition(Pos, theRadius, objects, proxyCargos);
		TerritoryFlag theFlag;

		if (objects)
		{
			for (int i = 0; i < objects.Count(); i++)
			{
				if (Class.CastTo(theFlag, objects.Get(i)))
				{
					return theFlag;
				}
			}
		}

		return null;
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

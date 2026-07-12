// DuskVale Territory Overrides - world scripts.
//
// Pure override mod on top of ElysianTerritoriesPlus (Multiplix Territory V2)
// and LBmaster Groups. No files in either mod are edited.
//
// Rules implemented here:
//  - Territories can only be placed or claimed by players in an LBmaster group
//  - Placing a territory creates an LBmaster group map marker "My Property" (flag icon)
//  - The owning group gets a decay warning every 2.5 hours while the countdown runs
//  - A vanilla Box Of Nails refreshes the countdown back to the full 7 days
//  - All player nametags are visible inside an active territory
//  - Only the territory owner (or server admin) can remove the floppydisk

class DVT_Bridge
{
	static const string FLAG_ICON = "LBmaster_Groups\\gui\\icons\\flag.paa";
	static const string MARKER_NAME = "My Property";

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

	static LBGroup GetGroupForGuid(string guid)
	{
		PlayerBase player = FindOnlinePlayerByGuid(guid);
		if (player)
		{
			return player.GetLBGroup();
		}

		return LBGroupManager.Get().GetPlayersGroup(guid);
	}

	static bool HasLBGroup(PlayerBase player)
	{
		return player && player.GetLBGroup() != null;
	}

	static bool IsServerAdmin(string guid)
	{
		return GetMultiTerritoriesConfig().ServerAdmins.Find(guid) != -1;
	}

	// Server side: create the group map marker at the exact territory position
	static void CreateMyPropertyMarker(PlayerBase placer, vector position)
	{
		if (!GetGame().IsServer() || !placer)
		{
			return;
		}

		LBGroup group = placer.GetLBGroup();
		if (!group)
		{
			return;
		}

		group.AddGroupMarker(MARKER_NAME, position, FLAG_ICON, ARGB(255, 255, 255, 255), placer.GetMySteamId());
		GetMultiTerritoriesConfig().DiagnosticLog("[DVT] Created '" + MARKER_NAME + "' group marker for group " + group.shortname + " at " + position.ToString());
	}

	// True when the given position is inside an active (protected) territory computer radius
	static bool IsInsideActiveTerritory(vector position)
	{
		if (position == vector.Zero)
		{
			return false;
		}

		array<Object> objects = new array<Object>;
		array<CargoBase> proxyCargos = new array<CargoBase>;
		GetGame().GetObjectsAtPosition(position, GetMultiTerritoriesConfig().GetProtectionRadius(), objects, proxyCargos);

		for (int i = 0; i < objects.Count(); i++)
		{
			REV_TerritoryComputer computer = REV_TerritoryComputer.Cast(objects.Get(i));
			if (computer && computer.IsTerritoryProtectionActive(false))
			{
				return true;
			}
		}

		return false;
	}
}

modded class ActionDeployObject
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		// No territory without a group
		if (item && item.IsInherited(REV_TerritoryComputerKit) && !DVT_Bridge.HasLBGroup(player))
		{
			if (GetGame().IsClient())
			{
				GetMultiTerritoriesConfig().SendNotification(DVTOverridesConfig.Get().GroupRequiredWarningMessage, TerritoryIcons.NoBuildZone);
			}

			return false;
		}

		return super.ActionCondition(player, target, item);
	}
}

modded class ActionTogglePlaceObject
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (item && item.IsInherited(REV_TerritoryComputerKit) && !DVT_Bridge.HasLBGroup(player))
		{
			if (GetGame().IsClient())
			{
				GetMultiTerritoriesConfig().SendNotification(DVTOverridesConfig.Get().GroupRequiredWarningMessage, TerritoryIcons.NoBuildZone);
			}

			return false;
		}

		return super.ActionCondition(player, target, item);
	}
}

modded class REV_TerritoryComputerKit
{
	override void OnPlacementComplete(Man player, vector position = "0 0 0", vector orientation = "0 0 0")
	{
		PlayerBase pb = PlayerBase.Cast(player);

		// Server-authoritative gate: without a group the kit stays a kit and
		// no territory computer is spawned.
		if (GetGame().IsServer() && pb && !DVT_Bridge.HasLBGroup(pb))
		{
			SetIsBeingPlaced(false);
			MultiFunctions.SendPlayerMessage(pb, DVTOverridesConfig.Get().GroupRequiredWarningMessage);
			return;
		}

		super.OnPlacementComplete(player, position, orientation);

		if (GetGame().IsServer() && pb)
		{
			DVT_Bridge.CreateMyPropertyMarker(pb, position);
		}
	}
}

modded class REV_TerritoryComputer
{
	override void EEInit()
	{
		super.EEInit();

		if (GetGame() && GetGame().IsServer())
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.DVT_SendDecayWarning, DVTOverridesConfig.Get().GetDecayWarningIntervalMs(), true);
		}
	}

	void ~REV_TerritoryComputer()
	{
		if (GetGame())
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.DVT_SendDecayWarning);
		}
	}

	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		if (GetGame().IsServer() && sender)
		{
			string senderGuid = sender.GetId();

			// No territory without a group: claiming requires an LBmaster group
			if (rpc_type == MULTITERRITORYRPCs.MENU_CLAIM_ADMIN && CanReceiveNewOwner())
			{
				PlayerBase claimant = DVT_Bridge.FindOnlinePlayerByGuid(senderGuid);
				if (claimant && !DVT_Bridge.HasLBGroup(claimant))
				{
					MultiFunctions.SendPlayerMessage(claimant, DVTOverridesConfig.Get().GroupRequiredClaimMessage);
					return;
				}
			}

			// Only the territory owner (or a server admin) may remove the floppydisk
			if (rpc_type == MULTITERRITORYRPCs.MENU_EJECT_FLOPPY)
			{
				if (!CanReceiveNewOwner() && !IsTerritoryOwner(senderGuid) && !DVT_Bridge.IsServerAdmin(senderGuid))
				{
					PlayerBase ejector = DVT_Bridge.FindOnlinePlayerByGuid(senderGuid);
					if (ejector)
					{
						MultiFunctions.SendPlayerMessage(ejector, DVTOverridesConfig.Get().OwnerOnlyEjectMessage);
					}
					return;
				}
			}
		}

		super.OnRPC(sender, rpc_type, ctx);
	}

	// Refresh the territory decay countdown with a vanilla Box Of Nails.
	// Consumes the box and resets the countdown back to the full configured
	// lifetime (7 days by default).
	void RefreshTerritoryWithNails(PlayerBase player, ItemBase nails)
	{
		if (!GetGame().IsServer())
		{
			return;
		}

		Territory_Floppydisk floppy = Territory_Floppydisk.Cast(FindAttachmentBySlotName("Material_FPole_Flag"));
		if (!floppy || !IsTurnedOn() || !IsFloppyLocked())
		{
			return;
		}

		if (nails)
		{
			GetGame().ObjectDelete(nails);
		}

		floppy.SetHealth("", "", 100);
		m_RefresherTimeRemaining = Math.Round(GetEffectiveRefresherDuration());
		ForceProtectionRefresh(true);
		UpdateMainDisplayIntegrityBar();

		if (player)
		{
			int days = Math.Round(GetEffectiveRefresherDuration() / 86400);
			string message = DVTOverridesConfig.Get().TerritoryRefreshedMessage;
			message.Replace("$DAYS$", days.ToString());
			MultiFunctions.SendPlayerMessage(player, message);
		}

		GetMultiTerritoriesConfig().DiagnosticLog("[DVT] Territory refreshed with nails. Time remaining: " + m_RefresherTimeRemaining);
	}

	// Warn the owning group on a fixed interval (2.5 hours by default) while
	// the decay countdown is running.
	void DVT_SendDecayWarning()
	{
		if (!GetGame() || !GetGame().IsServer())
		{
			return;
		}

		if (!m_TerritoryOwner || m_TerritoryOwner == "")
		{
			return;
		}

		SynchronizeRefresherTime();
		if (!CanTerritoryBeActive())
		{
			return;
		}

		int hoursLeft = Math.Round(m_RefresherTimeRemaining / 3600.0);
		string groupName = "Your group";
		LBGroup group = DVT_Bridge.GetGroupForGuid(m_TerritoryOwner);
		if (group)
		{
			groupName = group.name;
		}

		string message = DVTOverridesConfig.Get().DecayWarningMessage;
		message.Replace("$GROUP$", groupName);
		message.Replace("$HOURS$", hoursLeft.ToString());

		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);

		for (int i = 0; i < players.Count(); i++)
		{
			PlayerBase pb = PlayerBase.Cast(players.Get(i));
			if (!pb || !pb.GetIdentity())
			{
				continue;
			}

			bool shouldReceive = false;
			if (group)
			{
				shouldReceive = pb.GetLBGroup() && pb.GetLBGroup().GetTagHash() == group.GetTagHash();
			}
			else
			{
				string guid = pb.GetIdentity().GetId();
				shouldReceive = IsTerritoryOwner(guid) || (m_TerritoryMembers && m_TerritoryMembers.CheckId(guid));
			}

			if (shouldReceive)
			{
				pb.Multi_SendMessage(message);
			}
		}

		GetMultiTerritoriesConfig().DiagnosticLog("[DVT] Decay warning sent for territory of group '" + groupName + "'. Hours left: " + hoursLeft);
	}
}

modded class ActionEjectFloppy
{
	override protected bool CanEject(PlayerBase player, REV_TerritoryComputer territoryComputer)
	{
		if (!player || !player.GetIdentity() || !territoryComputer)
		{
			return false;
		}

		// Only the territory owner (or a server admin) can remove the
		// floppydisk. Unclaimed computers can have their floppy ejected by anyone.
		string guid = player.GetIdentity().GetId();
		if (territoryComputer.CanReceiveNewOwner())
		{
			return true;
		}

		if (territoryComputer.IsTerritoryOwner(guid))
		{
			return true;
		}

		return DVT_Bridge.IsServerAdmin(guid);
	}
}

class ActionRefreshTerritoryNails : ActionInteractBase
{
	void ActionRefreshTerritoryNails()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_Text = "Refresh Territory (Box Of Nails)";
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTObject(UAMaxDistances.DEFAULT);
	}

	protected bool CanRefresh(PlayerBase player, REV_TerritoryComputer computer)
	{
		if (!player || !player.GetIdentity() || !computer)
		{
			return false;
		}

		// The countdown can only be refreshed while the territory is running:
		// powered on with a locked, valid floppydisk.
		if (!computer.IsTurnedOn() || !computer.IsFloppyLocked() || !computer.HasValidFloppy())
		{
			return false;
		}

		return computer.IsTerritoryMember(player.GetIdentity().GetId());
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		Object targetObject = target.GetObject();
		if (!targetObject || !item)
		{
			return false;
		}

		if (!item.IsInherited(NailBox))
		{
			return false;
		}

		REV_TerritoryComputer territoryComputer = REV_TerritoryComputer.Cast(targetObject);
		if (!territoryComputer)
		{
			return false;
		}

		if (GetGame().IsClient())
		{
			territoryComputer.SyncTerritoryRateLimited();
		}

		return CanRefresh(player, territoryComputer);
	}

	override typename GetInputType()
	{
		return ContinuousInteractActionInput;
	}

	override bool HasProgress()
	{
		return false;
	}

	override void OnExecute(ActionData action_data)
	{
		super.OnExecute(action_data);

		REV_TerritoryComputer target = REV_TerritoryComputer.Cast(action_data.m_Target.GetObject());
		if (target && action_data.m_Player && CanRefresh(action_data.m_Player, target))
		{
			target.RefreshTerritoryWithNails(action_data.m_Player, action_data.m_MainItem);
		}
	}

	override void OnExecuteClient(ActionData action_data)
	{
		super.OnExecuteClient(action_data);

		REV_TerritoryComputer target = REV_TerritoryComputer.Cast(action_data.m_Target.GetObject());
		if (target)
		{
			target.PlayInsertFloppySound();
		}
	}
};

modded class NailBox
{
	override void SetActions()
	{
		super.SetActions();

		AddAction(ActionRefreshTerritoryNails);
	}
};

modded class ActionConstructor
{
	override void RegisterActions(TTypenameArray actions)
	{
		super.RegisterActions(actions);

		actions.Insert(ActionRefreshTerritoryNails);
	}
}

modded class LBSafezoneMarkers_
{
	// Treat active territories as nametag zones: while a player is inside the
	// territory radius, their nametag is visible to everyone else inside it,
	// regardless of group or territory membership.
	override bool IsInRadius(vector position, vector myPosition)
	{
		if (super.IsInRadius(position, myPosition))
		{
			return true;
		}

		if (!enablePlayerMarkers)
		{
			return false;
		}

		if (vector.Distance(position, myPosition) > maxPlayerDistance)
		{
			return false;
		}

		if (checkLineOfSight && !DoLineOfSightCheck(position, myPosition))
		{
			return false;
		}

		return DVT_Bridge.IsInsideActiveTerritory(position);
	}
}

// LBmaster Groups <-> Elysian Territories integration.
//
// - Territories can only be created (placed or claimed) by players who are in an LBmaster group
// - Placing a territory creates an LBmaster group map marker named "My Property" (flag icon)
// - The owning group gets a periodic decay warning while the territory countdown runs
// - All player nametags are visible inside an active territory via the LBmaster safezone marker system
//
// Everything in this file only compiles when LBmaster_Groups is loaded.
#ifdef LBmaster_Groups
class ETP_LB_GroupBridge
{
	static const string FLAG_ICON = "LBmaster_Groups\\gui\\icons\\flag.paa";
	static const string MARKER_NAME = "My Property";
	static const string GROUP_REQUIRED_CLAIM_MESSAGE = "You must be in a group before you can claim a territory!";

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
		GetMultiTerritoriesConfig().DiagnosticLog("[ETP_LB] Created '" + MARKER_NAME + "' group marker for group " + group.shortname + " at " + position.ToString());
	}

	// True when the given position is inside an active (protected) Elysian territory
	static bool IsInsideActiveTerritory(vector position)
	{
		TerritoryFlag flag = ETP_LB_PlotpoleBridge.FindElysianTerritoryAtPos(position);
		if (!flag)
		{
			return false;
		}

		return flag.IsTerritoryProtectionActive(false);
	}
}

modded class ActionDeployObject
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		// No territory without a group: block deployment of the territory
		// computer kit for players who are not in an LBmaster group.
		if (item && item.IsInherited(REV_TerritoryComputerKit) && !ETP_LB_GroupBridge.HasLBGroup(player))
		{
			if (GetGame().IsClient())
			{
				GetMultiTerritoriesConfig().SendNotification(GetMultiTerritoriesConfig().GroupRequiredWarningMessage, TerritoryIcons.NoBuildZone);
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
		if (item && item.IsInherited(REV_TerritoryComputerKit) && !ETP_LB_GroupBridge.HasLBGroup(player))
		{
			if (GetGame().IsClient())
			{
				GetMultiTerritoriesConfig().SendNotification(GetMultiTerritoriesConfig().GroupRequiredWarningMessage, TerritoryIcons.NoBuildZone);
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
		if (GetGame().IsServer() && pb && !ETP_LB_GroupBridge.HasLBGroup(pb))
		{
			SetIsBeingPlaced(false);
			MultiFunctions.SendPlayerMessage(pb, GetMultiTerritoriesConfig().GroupRequiredWarningMessage);
			return;
		}

		super.OnPlacementComplete(player, position, orientation);

		if (GetGame().IsServer() && pb)
		{
			ETP_LB_GroupBridge.CreateMyPropertyMarker(pb, position);
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
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.ETP_LB_SendDecayWarning, GetMultiTerritoriesConfig().GetDecayWarningIntervalMs(), true);
		}
	}

	void ~REV_TerritoryComputer()
	{
		if (GetGame())
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.ETP_LB_SendDecayWarning);
		}
	}

	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		// No territory without a group: claiming ownership also requires an
		// LBmaster group.
		if (GetGame().IsServer() && sender && rpc_type == MULTITERRITORYRPCs.MENU_CLAIM_ADMIN && CanReceiveNewOwner())
		{
			PlayerBase senderPlayer = ETP_LB_GroupBridge.FindOnlinePlayerByGuid(sender.GetId());
			if (senderPlayer && !ETP_LB_GroupBridge.HasLBGroup(senderPlayer))
			{
				MultiFunctions.SendPlayerMessage(senderPlayer, ETP_LB_GroupBridge.GROUP_REQUIRED_CLAIM_MESSAGE);
				return;
			}
		}

		super.OnRPC(sender, rpc_type, ctx);
	}

	// Warn the owning group on a fixed interval (2.5 hours by default) while
	// the decay countdown is running.
	void ETP_LB_SendDecayWarning()
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
		LBGroup group = ETP_LB_GroupBridge.GetGroupForGuid(m_TerritoryOwner);
		if (group)
		{
			groupName = group.name;
		}

		string message = GetMultiTerritoriesConfig().DecayWarningMessage;
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

		GetMultiTerritoriesConfig().DiagnosticLog("[ETP_LB] Decay warning sent for territory of group '" + groupName + "'. Hours left: " + hoursLeft);
	}
}

modded class LBSafezoneMarkers_
{
	// Treat active Elysian territories as nametag zones: while a player is
	// inside the territory radius, their nametag is visible to everyone else
	// inside it, regardless of group or territory membership.
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

		return ETP_LB_GroupBridge.IsInsideActiveTerritory(position);
	}
}
#endif

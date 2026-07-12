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

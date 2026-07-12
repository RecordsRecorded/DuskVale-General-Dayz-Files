class ActionEjectFloppy : ActionInteractBase
{
	void ActionEjectFloppy()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_Text = "Hold To Eject Floppydisk";
	}

	protected bool CanEject(PlayerBase player, REV_TerritoryComputer territoryComputer)
	{
		if (!player || !player.GetIdentity() || !territoryComputer)
		{
			return false;
		}

		// Only the territory owner (or a server admin) can remove the floppydisk.
		// Unclaimed computers can have their floppy ejected by anyone.
		string guid = player.GetIdentity().GetId();
		if (territoryComputer.CanReceiveNewOwner())
		{
			return true;
		}

		if (territoryComputer.IsTerritoryOwner(guid))
		{
			return true;
		}

		return GetMultiTerritoriesConfig().ServerAdmins.Find(guid) != -1;
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTObject(UAMaxDistances.DEFAULT);
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		Object targetObject = target.GetObject();
		if (!targetObject)
		{
			return false;
		}

		REV_TerritoryComputer territoryComputer = REV_TerritoryComputer.Cast(targetObject);
		if (!territoryComputer || !territoryComputer.FindAttachmentBySlotName("Material_FPole_Flag"))
		{
			return false;
		}

		string selection = targetObject.GetActionComponentName(target.GetComponentIndex());
		if (selection != REV_TerritoryComputer.SEL_FLOPPY_INSERT && selection != REV_TerritoryComputer.SEL_KEYPAD)
		{
			return false;
		}

		return CanEject(player, territoryComputer);
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
		if (target && action_data.m_Player && CanEject(action_data.m_Player, target))
		{
			target.EjectFloppy(action_data.m_Player);
		}
	}

	override void OnExecuteClient(ActionData action_data)
	{
		super.OnExecuteClient(action_data);

		REV_TerritoryComputer target = REV_TerritoryComputer.Cast(action_data.m_Target.GetObject());
		if (target)
		{
			target.ClientPreviewPowerOff();
			target.PlayEjectFloppySound();
		}
	}
};

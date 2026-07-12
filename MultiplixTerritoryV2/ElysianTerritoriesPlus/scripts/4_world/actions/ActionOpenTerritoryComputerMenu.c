class ActionOpenTerritoryComputerMenu: ActionInteractBase
{
	void ActionOpenTerritoryComputerMenu()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_Text = "Open Territory Menu";
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!player || !player.GetIdentity() || !target || !target.GetObject())
		{
			return false;
		}

		REV_TerritoryComputer theComputer = REV_TerritoryComputer.Cast(target.GetObject());
		if (!theComputer)
		{
			return false;
		}

		string selection = target.GetObject().GetActionComponentName(target.GetComponentIndex());
		if (!REV_TerritoryComputer.IsPrimaryInteractionSelection(selection))
		{
			return false;
		}

		if (!theComputer.IsTurnedOn())
		{
			return false;
		}

		return theComputer.FindAttachmentBySlotName("Material_FPole_Flag") != null;
	}

	override void OnExecuteClient(ActionData action_data)
	{
		super.OnExecuteClient(action_data);

		if (!action_data || !action_data.m_Target)
		{
			return;
		}

		REV_TerritoryComputer theComputer = REV_TerritoryComputer.Cast(action_data.m_Target.GetObject());
		if (!theComputer)
		{
			return;
		}

		MultiplixTerritoryMenuState.OpenComputerMenu(theComputer);
	}
}

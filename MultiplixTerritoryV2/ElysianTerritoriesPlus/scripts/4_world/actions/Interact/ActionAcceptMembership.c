class ActionAcceptMembership extends ActionInteractBase
{
	bool IsMember = false;

	void ActionAcceptMembership()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ATTACHITEM;
	}
	
	override string GetText()
	{
		if (IsMember)
		{
			return "Delete Your User";
		}

		return "Join Database";
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		REV_TerritoryComputer theComputer = REV_TerritoryComputer.Cast(target.GetObject());
		PlayerIdentity ident = PlayerIdentity.Cast(player.GetIdentity());

		Object targetObject = target.GetObject();
		int component_id = target.GetComponentIndex();
		string selection = targetObject.GetActionComponentName(target.GetComponentIndex());

		if (ident && theComputer && selection == REV_TerritoryComputer.SEL_KEYPAD)
		{
			if ( theComputer.FindAttachmentBySlotName("Material_FPole_Flag") && theComputer.IsTurnedOn() )
			{
				IsMember = (theComputer.IsTerritoryMember(ident.GetId()) && !theComputer.CanReceiveNewOwner());
				if (theComputer.CanAddMember() && !IsMember && GetMultiTerritoriesConfig().HasMemberLimit() && theComputer.GetMemberCount() >= GetMultiTerritoriesConfig().MaxMembersPerTerritory)
				{
					return false;
				}

				if (theComputer.CanAddMember() && !IsMember)
				{
					return true;
				} 
				else 
				if (IsMember && !theComputer.IsTerritoryOwner(ident.GetId()))
				{
					return true;
				}
			}
		}

		return false;
	}
	
	override void OnExecuteServer(ActionData action_data)
	{
		if (action_data && action_data.m_Target && action_data.m_Player)
		{
			//Print("ActionAcceptMembership - OnExecuteServer");
			REV_TerritoryComputer theComputer = REV_TerritoryComputer.Cast(action_data.m_Target.GetObject());
			PlayerBase thePlayer = PlayerBase.Cast(action_data.m_Player);

			if (theComputer && thePlayer && thePlayer.GetIdentity())
			{
				//Print("ActionAcceptMembership - OnExecuteServer - AddMember - " + thePlayer.GetIdentity().GetId());
				if (!theComputer.IsTerritoryMember(thePlayer.GetIdentity().GetId()))
				{
					if (GetMultiTerritoriesConfig().HasMemberLimit() && theComputer.GetMemberCount() >= GetMultiTerritoriesConfig().MaxMembersPerTerritory)
					{
						thePlayer.Multi_SendMessage("This territory is already at the member limit.");
						return;
					}

					theComputer.AddMember(thePlayer.GetIdentity().GetId());
					thePlayer.Multi_SendMessage("You have been accepted into the Database!");
					theComputer.UpdateIndicatorLights();
				} else 
				{
					theComputer.RemoveMember(thePlayer.GetIdentity().GetId());
					thePlayer.Multi_SendMessage("You have been removed from the Database.");
					theComputer.UpdateIndicatorLights();
				}
			}
		}
	}

	override void OnExecuteClient(ActionData action_data)
	{
		super.OnExecuteClient(action_data);

		if (action_data && action_data.m_Target)
		{
			REV_TerritoryComputer theComputer = REV_TerritoryComputer.Cast(action_data.m_Target.GetObject());
			if (theComputer)
			{
				theComputer.ClientPreviewMemberDatabaseChanged();
			}
		}
	}
};

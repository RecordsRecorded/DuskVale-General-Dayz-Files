class ActionAddTerritoryMember extends ActionInteractBase
{
	protected bool AddingMember = true;

	void ActionAddTerritoryMember()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ATTACHITEM;
	}
	
	override string GetText()
	{
		if (AddingMember)
		{
			return "Add New User";
		} 
		
		return "Revoke Invitation";
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		PlayerIdentity ident = PlayerIdentity.Cast(player.GetIdentity());
		REV_TerritoryComputer theComputer = REV_TerritoryComputer.Cast(target.GetObject());

		Object targetObject = target.GetObject();
		int component_id = target.GetComponentIndex();
		string selection = targetObject.GetActionComponentName(target.GetComponentIndex());

		if (ident && theComputer && selection == REV_TerritoryComputer.SEL_KEYPAD)
		{
			if ( theComputer.FindAttachmentBySlotName("Material_FPole_Flag") && theComputer.IsTurnedOn() )
			{
				AddingMember = !theComputer.CanAddMember();
				if (AddingMember && GetMultiTerritoriesConfig().HasMemberLimit() && theComputer.GetMemberCount() >= GetMultiTerritoriesConfig().MaxMembersPerTerritory)
				{
					return false;
				}

				return theComputer.CheckPlayerPermission(ident.GetId(), TerritoryPerm.ADDMEMBER);
			}
		}

		return false;
	}
	
	override void OnExecuteServer( ActionData action_data )
	{
		if (action_data && action_data.m_Target)
		{
			REV_TerritoryComputer theComputer = REV_TerritoryComputer.Cast(action_data.m_Target.GetObject()	);
			PlayerBase thePlayer = PlayerBase.Cast(action_data.m_Player);
			if (theComputer && thePlayer && thePlayer.GetIdentity() && theComputer.CheckPlayerPermission(thePlayer.GetIdentity().GetId(), TerritoryPerm.ADDMEMBER))
			{
				bool addingMember = !theComputer.CanAddMember();
				if (addingMember && GetMultiTerritoriesConfig().HasMemberLimit() && theComputer.GetMemberCount() >= GetMultiTerritoriesConfig().MaxMembersPerTerritory)
				{
					MultiFunctions.SendPlayerMessage(action_data.m_Player, "Territory member limit reached.");
					return;
				}

				theComputer.AllowMemberToBeAdded(addingMember);

				if (addingMember)
				{
					MultiFunctions.SendPlayerMessage(action_data.m_Player, "Invitation sent! They must accept it at the computer.");
					theComputer.UpdateIndicatorLights();
				}
				if (!addingMember)
				{
					theComputer.UpdateIndicatorLights();
				}
			}
		}
	}

	override void OnExecuteClient( ActionData action_data )
	{
		super.OnExecuteClient(action_data);

		if (action_data && action_data.m_Target)
		{
			REV_TerritoryComputer theComputer = REV_TerritoryComputer.Cast(action_data.m_Target.GetObject());
			if (theComputer)
			{
				theComputer.ClientPreviewMemberInviteChanged(!theComputer.CanAddMember());
			}
		}
	}
};

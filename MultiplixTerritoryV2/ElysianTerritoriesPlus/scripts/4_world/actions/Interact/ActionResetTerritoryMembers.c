class ActionResetTerritoryMembers extends ActionInteractBase
{
	protected bool CanResetOwner = false;
	protected bool CanSetOwner = false;
	protected int NextSync = 0;
	
	override string GetText()
	{
		if (CanResetOwner) 
		{
			if (CanSetOwner)
			{
				return "Accept Admin";
			}

			return "Reset Admin";
		}

		return "Remove All Users";
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		PlayerIdentity ident = PlayerIdentity.Cast(player.GetIdentity());
		REV_TerritoryComputer theComputer = REV_TerritoryComputer.Cast(target.GetObject());
		CanResetOwner = false;
		CanSetOwner = false;

		Object targetObject = target.GetObject();
		int component_id = target.GetComponentIndex();
		string selection = targetObject.GetActionComponentName(target.GetComponentIndex());

		if (ident && theComputer && selection == REV_TerritoryComputer.SEL_KEYPAD)
		{
			int curTime = GetGame().GetTime();
			if (!theComputer.CanReceiveNewOwner() && GetMultiTerritoriesConfig().ServerAdmins.Find(ident.GetId()) != -1)
			{
				CanResetOwner = true;
				return true;
			}

			if (GetGame().IsClient() && theComputer.CanReceiveNewOwner() && NextSync <= curTime)
			{
				NextSync = curTime + 80000;
				theComputer.SyncTerritory();
				return false;
			}

			if ( theComputer.FindAttachmentBySlotName("Material_FPole_Flag") && theComputer.IsTurnedOn() )
			{
				CanResetOwner = (theComputer.GetMemberCount() == 0);
				CanSetOwner = theComputer.CanReceiveNewOwner();
				bool CanResetMembers = !CanResetOwner && theComputer.CheckPlayerPermission(ident.GetId(), TerritoryPerm.REMOVEMEMBER);

				return (theComputer.CheckPlayerPermission(ident.GetId(), TerritoryPerm.ADMIN) || CanResetMembers || CanSetOwner);
			}
		}

		return false;
	}

	override void OnExecuteServer( ActionData action_data )
	{
		if (action_data && action_data.m_Target && action_data.m_Player)
		{
			PlayerBase thePlayer = PlayerBase.Cast(action_data.m_Player);
			REV_TerritoryComputer theComputer = REV_TerritoryComputer.Cast(action_data.m_Target.GetObject());

			if (theComputer && thePlayer && thePlayer.GetIdentity())
			{
				string guid = thePlayer.GetIdentity().GetId();
				bool isServerAdmin = GetMultiTerritoriesConfig().ServerAdmins.Find(guid) != -1;
				bool isOwner = theComputer.IsTerritoryOwner(guid);
				bool canSetOwner = theComputer.CanReceiveNewOwner();
				bool canResetOwner = !canSetOwner && (isServerAdmin || isOwner) && theComputer.GetMemberCount() == 0;
				bool canResetMembers = !canResetOwner && theComputer.CheckPlayerPermission(guid, TerritoryPerm.REMOVEMEMBER);

				if (!canResetOwner && !canSetOwner && !theComputer.CheckPlayerPermission(guid, TerritoryPerm.ADMIN) && !canResetMembers)
				{
					return;
				}

				if (canResetOwner)
				{
					theComputer.ResetTerritoryAdministration();
					theComputer.UpdateIndicatorLights();
					return;
				}
				if (canSetOwner)
				{
					MultiFunctions.SendPlayerMessage(thePlayer, "Territory claimed! Others can't build nearby, and your base stays longer while the computer is active.");
					theComputer.SetTerritoryOwner(thePlayer.GetIdentity().GetId());
					theComputer.UpdateIndicatorLights();
					return;
				}

				theComputer.ResetMembers();
				theComputer.UpdateIndicatorLights();
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
				theComputer.ClientPreviewMemberDatabaseChanged();
			}
		}
	}
};

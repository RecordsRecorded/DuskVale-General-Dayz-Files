modded class ActionLowerFlag: ActionInteractBase
{		
	protected bool CanLowerTerritoryFlag(PlayerBase player, TerritoryFlag theFlag)
	{
		if (!player || !player.GetIdentity() || !theFlag)
		{
			return false;
		}

		return theFlag.CheckPlayerPermission(player.GetIdentity().GetId(), TerritoryPerm.LOWERFLAG);
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if (super.ActionCondition(player, target, item))
		{
			TerritoryFlag theFlag = TerritoryFlag.Cast( target.GetObject() );
			PlayerBase thePlayer = PlayerBase.Cast(player);

			if (theFlag && thePlayer && thePlayer.GetIdentity())
			{
				theFlag.SyncTerritoryRateLimited();
				if (vector.Distance(theFlag.GetPosition(), thePlayer.GetPosition()) > UAMisc.MAX_DISTANCE_FROM_FLAG)
				{
					return false;
				}

				if (!CanLowerTerritoryFlag(thePlayer, theFlag))
				{
					GetMultiTerritoriesConfig().SendNotification(GetMultiTerritoriesConfig().LowerFlagWarningMessage, TerritoryIcons.LowerFlagWarning);
					return false;
				}

				return true;
			}
		}

		return false;
	}

	override void OnExecuteServer(ActionData action_data)
	{
		if (!action_data || !action_data.m_Target)
		{
			return;
		}

		TerritoryFlag theFlag = TerritoryFlag.Cast(action_data.m_Target.GetObject());
		if (!CanLowerTerritoryFlag(action_data.m_Player, theFlag))
		{
			return;
		}

		super.OnExecuteServer(action_data);
	}
};

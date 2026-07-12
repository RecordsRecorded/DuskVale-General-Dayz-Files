modded class ActionDismantlePart : ActionContinuousBase
{
	protected bool HasDismantlePermission(PlayerBase player, ActionTarget target)
	{
		if (!player || !target)
		{
			return false;
		}

		ItemBase theTarget;
		if (Class.CastTo(theTarget, target.GetObject()) || Class.CastTo(theTarget, target.GetParent()))
		{
			string theGUID = "";
			if (player.GetIdentity())
			{
				theGUID = player.GetIdentity().GetId();
			}

			return TerritoryFlag.HasTerritoryPermAtPos(theGUID, TerritoryPerm.DISMANTLE, theTarget.GetPosition());
		}

		return true;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{	
		ItemBase theTarget;

		if (super.ActionCondition(player, target, item))
		{

			if (Class.CastTo(theTarget, target.GetObject()) || Class.CastTo(theTarget, target.GetParent()))
			{
				PlayerBase thePlayer = PlayerBase.Cast(player);
				if (theTarget && thePlayer) 
				{
					string theGUID = "";
					if (thePlayer.GetIdentity()) 
					{
						theGUID = thePlayer.GetIdentity().GetId();
					}

					if (!TerritoryFlag.HasTerritoryPermAtPos(theGUID, TerritoryPerm.DISMANTLE, theTarget.GetPosition()))
					{
						GetMultiTerritoriesConfig().SendNotification(GetMultiTerritoriesConfig().DismantleWarningMessage, TerritoryIcons.DismantleWarning);
						return false;
					}
				}
			}

			return true;
		}

		return false;
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		if (!HasDismantlePermission(action_data.m_Player, action_data.m_Target))
		{
			return;
		}

		super.OnFinishProgressServer(action_data);
	}
}

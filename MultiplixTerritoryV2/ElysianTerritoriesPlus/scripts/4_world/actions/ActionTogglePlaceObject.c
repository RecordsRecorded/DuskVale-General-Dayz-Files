modded class ActionTogglePlaceObject
{
	protected bool Multi_CanChoosePlacement(PlayerBase player, ItemBase item)
	{
		if (!player || !item || !player.GetIdentity())
		{
			GetMultiTerritoriesConfig().DiagnosticLog("ActionTogglePlaceObject allow: missing player/item/identity");
			return true;
		}

		if (GetMultiTerritoriesConfig().IsInWhiteList(item.GetType()))
		{
			GetMultiTerritoriesConfig().DiagnosticLog("ActionTogglePlaceObject allow: whitelist item=" + item.GetType());
			return true;
		}

		TerritoryFlag territory = TerritoryFlag.GetTerritoryAtPos(player.GetPosition(), item.IsInherited(REV_TerritoryComputerKit));
		if (!territory)
		{
			GetMultiTerritoriesConfig().DiagnosticLog("ActionTogglePlaceObject allow: no territory at player pos item=" + item.GetType());
			return true;
		}

		if (item.IsInherited(REV_TerritoryComputerKit))
		{
			GetMultiTerritoriesConfig().DiagnosticLog("ActionTogglePlaceObject allow: territory computer kit uses separate conflict logic");
			return true;
		}

		bool allowed = territory.CheckPlayerPermission(player.GetIdentity().GetId(), TerritoryPerm.DEPLOY);
		GetMultiTerritoriesConfig().DiagnosticLog("ActionTogglePlaceObject territory check item=" + item.GetType() + " playerPos=" + player.GetPosition() + " allowed=" + allowed);
		return allowed;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		bool baseAllowed = super.ActionCondition(player, target, item);
		GetMultiTerritoriesConfig().DiagnosticLog("ActionTogglePlaceObject ActionCondition baseAllowed=" + baseAllowed + " item=" + item);
		if (!baseAllowed)
		{
			return false;
		}

		return Multi_CanChoosePlacement(player, item);
	}
}

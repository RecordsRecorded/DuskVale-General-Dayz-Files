#ifdef LBmaster_GroupDLCPlotpole
modded class PlayerBase
{
	protected bool CanStartPlacementInCurrentPosition(ItemBase item)
	{
		if (!item || !GetIdentity())
		{
			return true;
		}

		if (GetMultiTerritoriesConfig().IsInWhiteList(item.GetType()))
		{
			return true;
		}

		TerritoryFlag territory = TerritoryFlag.GetTerritoryAtPos(GetPosition(), item.IsInherited(REV_TerritoryComputerKit));
		if (!territory)
		{
			return true;
		}

		if (item.IsInherited(REV_TerritoryComputerKit))
		{
			return true;
		}

		if (!territory.HasRaisedFlag())
		{
			return true;
		}

		return territory.CheckPlayerPermission(GetIdentity().GetId(), TerritoryPerm.DEPLOY);
	}
}
#endif

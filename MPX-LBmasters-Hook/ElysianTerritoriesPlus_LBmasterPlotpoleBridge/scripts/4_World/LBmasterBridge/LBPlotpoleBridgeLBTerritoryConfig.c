#ifdef LBmaster_GroupDLCPlotpole
modded class LBTerritoryConfig_
{
	override bool CanExecuteAction(PlayerBase player, vector pos, int type, ItemBase itemInHands)
	{
		if (ETP_LB_PlotpoleBridge.CanElysianTerritorySatisfyLBAction(player, pos, type, itemInHands))
		{
			return true;
		}

		return super.CanExecuteAction(player, pos, type, itemInHands);
	}
}
#endif

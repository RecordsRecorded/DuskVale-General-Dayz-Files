modded class MissionGameplay extends MissionBase
{	
	override UIScriptedMenu CreateScriptedMenu(int id)
	{
		if (id == MENU_MULTITERRITORY_COMPUTER)
		{
			return new MultiplixTerritoryComputerMenu();
		}

		return super.CreateScriptedMenu(id);
	}

	void MissionGameplay()
	{
		GetDayZGame().Event_OnRPC.Insert(OnRPC);
	}

	void ~MissionGameplay()
	{
		if (GetDayZGame())
		{
			GetDayZGame().Event_OnRPC.Remove(OnRPC);
		}
	}

	override void OnMissionStart()
	{
		super.OnMissionStart();
		GetGame().RPCSingleParam(NULL, MULTITERRITORYRPCs.REQUEST_START, new Param1<bool>(true), true, NULL);
	}

	void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{
		if (rpc_type != MULTITERRITORYRPCs.REQUEST_START || !GetGame().IsClient())
		{
			return;
		}

		Param1< MultiTerritoriesConfig > data; 
		if (!ctx.Read(data)) 
			return;

		m_MultiTerritoriesConfig = data.param1;
		
		#ifdef BASICMAP
		if (GetMultiTerritoriesConfig().NoBuildZones)
		{
			BasicMap().ClearMarkers("TerritoryNoBuildZones");
			bool SomeZonesOnTheMap = false;

			for (int i = 0; i < GetMultiTerritoriesConfig().NoBuildZones.Count(); i++)
			{
				if (GetMultiTerritoriesConfig().NoBuildZones.Get(i) && GetMultiTerritoriesConfig().NoBuildZones.Get(i).DrawOnMap)
				{
					BasicMapCircleMarker tmpMarker = new BasicMapCircleMarker("", GetMultiTerritoriesConfig().NoBuildZones.Get(i).GetPos(), "Multiplix\\images\\NoBuild.paa", {189, 38, 78},150);
					tmpMarker.SetRadius(GetMultiTerritoriesConfig().NoBuildZones.Get(i).R);
					tmpMarker.SetShowCenterMarker(true);
					tmpMarker.SetHideIntersects(true);
					tmpMarker.SetCanEdit(false);
					tmpMarker.SetGroup("TerritoryNoBuildZones");
					BasicMap().AddMarker("TerritoryNoBuildZones",tmpMarker);
					SomeZonesOnTheMap = true;
				}
			}

			if (SomeZonesOnTheMap)
			{
				BasicMap().RegisterGroup("TerritoryNoBuildZones", new BasicMapGroupMetaData("TerritoryNoBuildZones", "No Build Areas"), NULL);
			}
		}
		#endif
	}
	
	protected ref MultiTerritoriesConfig m_MultiTerritoriesConfig;
}

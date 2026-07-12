// Enforce the DuskVale territory life span on the Multiplix config without
// modding MultiTerritoriesConfig (which is not modding-safe). Runs once at
// mission start on the server and persists the value into
// $profile:Multiplix/MultiTerritoriesConfig.json.
modded class MissionServer
{
	override void OnMissionStart()
	{
		super.OnMissionStart();

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.DVT_EnforceTerritoryLifetime, 1000, false);
	}

	void DVT_EnforceTerritoryLifetime()
	{
		if (!GetGame().IsServer())
		{
			return;
		}

		MultiTerritoriesConfig territoriesConfig = GetMultiTerritoriesConfig();
		if (!territoriesConfig)
		{
			return;
		}

		float wantedDays = DVTOverridesConfig.Get().GetTerritoryLifetimeDays();
		if (territoriesConfig.FloppyLifetimeDays != wantedDays)
		{
			territoriesConfig.FloppyLifetimeDays = wantedDays;
			territoriesConfig.Save();
			Print("[DuskVale] FloppyLifetimeDays set to " + wantedDays + " (territory max life span)");
		}
	}
}

// DuskVale Territory Overrides - config additions.
//
// Adds the DuskVale message/interval settings to the Multiplix territory
// config and enforces the 7 day maximum territory life span default.
modded class MultiTerritoriesConfig
{
	string GroupRequiredWarningMessage = "You must be in a group before you can create a territory!";
	string TerritoryRefreshedMessage = "Territory refreshed! The decay countdown has been reset to $DAYS$ days.";
	string DecayWarningMessage = "$GROUP$ has $HOURS$ hours left before your base decays away, have you found any nails to prevent this?";
	float DecayWarningIntervalHours = 2.5;

	override void Load()
	{
		super.Load();

		if (!GetGame().IsServer())
		{
			return;
		}

		bool changed = false;

		// 7 day maximum territory life span before decay
		if (FloppyLifetimeDays <= 0)
		{
			FloppyLifetimeDays = 7;
			changed = true;
		}

		if (DecayWarningIntervalHours <= 0)
		{
			DecayWarningIntervalHours = 2.5;
			changed = true;
		}

		if (changed)
		{
			Save();
		}
	}

	int GetDecayWarningIntervalMs()
	{
		float hours = DecayWarningIntervalHours;
		if (hours <= 0)
		{
			hours = 2.5;
		}

		return hours * 3600 * 1000;
	}
}

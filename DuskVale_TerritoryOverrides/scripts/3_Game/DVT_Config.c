// DuskVale Territory Overrides - own config.
//
// NOTE: MultiTerritoriesConfig can NOT be modded (its Load()/Save() pass
// 'this' to JsonFileLoader, which does not compile against a modded class),
// so the DuskVale settings live in their own file:
//   $profile:DuskVale/TerritoryOverrides.json
// The 7-day FloppyLifetimeDays enforcement is done at mission start (see
// DVT_MissionServer in 5_Mission) by writing the Multiplix config directly.
class DVTOverridesConfig
{
	protected static string DIR_PATH = "$profile:DuskVale";
	protected static string CONFIG_PATH = DIR_PATH + "\\TerritoryOverrides.json";

	protected static ref DVTOverridesConfig s_Instance;

	string ConfigVersion = "1";
	float TerritoryLifetimeDays = 7; // forced into Multiplix FloppyLifetimeDays when that is 0
	float DecayWarningIntervalHours = 2.5;
	string GroupRequiredWarningMessage = "You must be in a group before you can create a territory!";
	string GroupRequiredClaimMessage = "You must be in a group before you can claim a territory!";
	string TerritoryRefreshedMessage = "Territory refreshed! The decay countdown has been reset to $DAYS$ days.";
	string DecayWarningMessage = "$GROUP$ has $HOURS$ hours left before your base decays away, have you found any nails to prevent this?";
	string OwnerOnlyEjectMessage = "Only the territory owner can remove the floppydisk.";

	static DVTOverridesConfig Get()
	{
		if (!s_Instance)
		{
			s_Instance = new DVTOverridesConfig;
			if (GetGame().IsServer())
			{
				s_Instance.Load();
			}
		}

		return s_Instance;
	}

	void Load()
	{
		if (FileExist(CONFIG_PATH))
		{
			JsonFileLoader<DVTOverridesConfig>.JsonLoadFile(CONFIG_PATH, this);
		}
		else
		{
			if (!FileExist(DIR_PATH))
			{
				MakeDirectory(DIR_PATH);
			}
		}

		Save();
	}

	void Save()
	{
		JsonFileLoader<DVTOverridesConfig>.JsonSaveFile(CONFIG_PATH, this);
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

	float GetTerritoryLifetimeDays()
	{
		if (TerritoryLifetimeDays <= 0)
		{
			return 7;
		}

		return TerritoryLifetimeDays;
	}
}

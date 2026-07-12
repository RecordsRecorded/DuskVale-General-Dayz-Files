class MultiTerritoriesConfig
{
	protected static string DirPATH = "$profile:Multiplix";
	protected static string ConfigPATH = DirPATH + "\\MultiTerritoriesConfig.json";

	string ConfigVersion = "5";
	ref TStringArray WhiteList = 
	{ 
		"Trap",
		"Paper",
		"Fireplace",
		"WrittenNote",
		"ClaymoreMine",
		"PowerGenerator",
		"Plastic_Explosive",
		"WoodenCrate",
		"ShelterKit",
		"MediumTent",
		"LargeTent",
		"CarTent",
		"PartyTent"
	};
	ref TStringArray ServerAdmins = 
	{
		""
	}; // GUIDs
	
	float TentRadius = 0;
	float TentCESpawnLifeTime = 5600;
	float BuildBonusSledgeDamage = 300;
	bool RequireTerritory = false;
	int PublicPermissions = 0;
	int MemberPermissions = 62;
	int Notifications = 0; // 0 Chat | 1 Notifications Mod
	int DebugLogging = 0;
	int TerritoryDiagnostics = 0;
	float ProtectionRadius = 0;
	float FloppyHealthDrainMultiplier = 1.0;
	float FloppyLifetimeDays = 7; // Maximum territory life span in days before it decays away
	float DecayWarningIntervalHours = 2.5; // How often the decay warning is sent to the owning group
	int ProtectionRefreshIntervalSeconds = 0;
	int AllowPublicBuildOutsideTerritoryWarnings = 1;
	int MaxMembersPerTerritory = 0;
	
	ref array< ref MultiTerritoriesNoBuildZones> NoBuildZones = new array< ref MultiTerritoriesNoBuildZones>;
	
	string NoBuildZoneMessage               = "Building is not allowed here—this is a restricted zone!";
    string TerritoryConflictMessage         = "You can't place a territory this close to another existing one!";
    string WithinTerritoryWarning           = "This area is too close to an enemy-controlled territory!";
    string DeSpawnWarningMessage            = "You're building outside a territory! $ITEMNAME$ will disappear in $LIFETIME$ if not within a Territory Flag.";
    string BuildPartWarningMessage          = "You lack the necessary permissions to build in this location!";
    string DismantleWarningMessage          = "You cannot dismantle structures near an active flag!";
    string LowerFlagWarningMessage          = "You don't have the authorization to lower this territory's flag!";
    string TerritoryRequiredWarningMessage  = "A territory is required before you can start building here!";
    string GroupRequiredWarningMessage      = "You must be in a group before you can create a territory!";
    string TerritoryRefreshedMessage        = "Territory refreshed! The decay countdown has been reset to $DAYS$ days.";
    string DecayWarningMessage              = "$GROUP$ has $HOURS$ hours left before your base decays away, have you found any nails to prevent this?";

	
	int FlagRefreshFrequency = 432000;
	
	ref map<string, int> KitLifeTimes = new map<string, int>;
	
	[NonSerialized()]
	protected int m_BlockWarnPlayer = 0;
	[NonSerialized()]
	protected string m_BlockLastMessage = "";
	[NonSerialized()]
	protected bool m_RaidHandlerLoaded = false;
	
	int DisableBaseDamage = 0;

	protected void AddDefaultKitLifeTimes()
	{
		KitLifeTimes.Insert("fencekit", 3888000);
		KitLifeTimes.Insert("watchtowerkit", 3888000);
		KitLifeTimes.Insert("BBP_Wall_Kit", 3888000);
		KitLifeTimes.Insert("BBP_Door_Kit", 3888000);
		KitLifeTimes.Insert("BBP_Floor_Kit", 3888000);
		KitLifeTimes.Insert("BBP_Roof_Hatch_Kit", 3888000);
		KitLifeTimes.Insert("BBP_Floor_Hatch_Kit", 3888000);
		KitLifeTimes.Insert("BBP_Foundation_Kit", 3888000);
		KitLifeTimes.Insert("BBP_Gate_Kit", 3888000);
		KitLifeTimes.Insert("BBP_SGate_Kit", 3888000);
		KitLifeTimes.Insert("BBP_Half_Wall_Kit", 3888000);
		KitLifeTimes.Insert("BBP_Pillar_Kit", 3888000);
		KitLifeTimes.Insert("BBP_Ramp_Kit", 3888000);
		KitLifeTimes.Insert("BBP_Roof_Kit", 3888000);
		KitLifeTimes.Insert("BBP_Hanger_Roof_Kit", 3888000);
		KitLifeTimes.Insert("BBP_Slope_Roof_Kit", 3888000);
		KitLifeTimes.Insert("BBP_Slope_Wall_Kit", 3888000);
		KitLifeTimes.Insert("BBP_Stair_Kit", 3888000);
		KitLifeTimes.Insert("BBP_Triangle_Floor_Kit", 3888000);
		KitLifeTimes.Insert("BBP_Triangle_Roof_Kit", 3888000);
		KitLifeTimes.Insert("BBP_Window_Kit", 3888000);
		KitLifeTimes.Insert("BBP_Step_Ladder_Kit", 3888000);
		KitLifeTimes.Insert("BBP_Workbench_Kit", 3888000);
		KitLifeTimes.Insert("ExpansionFloorKit", 3888000);
		KitLifeTimes.Insert("ExpansionWoodPillarKit", 3888000);
		KitLifeTimes.Insert("ExpansionRampKit", 3888000);
		KitLifeTimes.Insert("ExpansionStairKit", 3888000);
		KitLifeTimes.Insert("ExpansionWallKit", 3888000);
		KitLifeTimes.Insert("ExpansionCamoBoxKit", 3888000);
		KitLifeTimes.Insert("ExpansionBarrierGateKit", 3888000);
		KitLifeTimes.Insert("ExpansionCamoTentKit", 3888000);
		KitLifeTimes.Insert("ExpansionHelipadKit", 3888000);
		KitLifeTimes.Insert("ExpansionBarbedWireKit", 3888000);
		KitLifeTimes.Insert("ExpansionHescoKit", 3888000);
		KitLifeTimes.Insert("Msp_GunCabinet_Kit", 3888000);
		KitLifeTimes.Insert("Msp_GunRack_Kit", 3888000);
		KitLifeTimes.Insert("Msp_DoubleGunRack_Kit", 3888000);
		KitLifeTimes.Insert("Msp_MilitaryLocker_Kit", 3888000);
		KitLifeTimes.Insert("Msp_Locker_Kit", 3888000);
		KitLifeTimes.Insert("Msp_StorageBox_Kit", 3888000);
		KitLifeTimes.Insert("Msp_ToolsBench_Blue_Kit", 3888000);
		KitLifeTimes.Insert("Msp_ToolsBench_Grey_Kit", 3888000);
		KitLifeTimes.Insert("Msp_ToolsBench_Red_Kit", 3888000);
		KitLifeTimes.Insert("Msp_BlueFridge_Kit", 3888000);
		KitLifeTimes.Insert("Msp_RetroBlueFridge_Kit", 3888000);
		KitLifeTimes.Insert("Msp_RustyFridge_Kit", 3888000);
		KitLifeTimes.Insert("Msp_Blue50sFridge_Kit", 3888000);
		KitLifeTimes.Insert("Msp_50sFridge_Kit", 3888000);
		KitLifeTimes.Insert("Msp_Freezer_Kit", 3888000);
		KitLifeTimes.Insert("Msp_Gazebo_Kit", 3888000);
		KitLifeTimes.Insert("Msp_StoneFireplace_Kit", 3888000);
	}

	protected bool EnsureDefaultKitLifeTimes()
	{
		map<string, int> defaults = new map<string, int>;
		ref map<string, int> original = KitLifeTimes;
		KitLifeTimes = defaults;
		AddDefaultKitLifeTimes();

		if (!original)
		{
			original = new map<string, int>;
		}

		bool changed = false;
		foreach (string key, int lifetime : defaults)
		{
			if (!original.Contains(key))
			{
				original.Insert(key, lifetime);
				changed = true;
			}
		}

		KitLifeTimes = original;
		return changed;
	}
	
	void Load()
	{
		Print("[MultiTerritories] Loading Config");
		if (GetGame().IsServer())
		{
			if (FileExist(ConfigPATH))
			{ //If config exist load File
			    JsonFileLoader<MultiTerritoriesConfig>.JsonLoadFile(ConfigPATH, this);
				if (ConfigVersion != "5")
				{
					ConfigVersion = "5";
					if (FloppyLifetimeDays <= 0)
					{
						FloppyLifetimeDays = 7;
					}
					if (DecayWarningIntervalHours <= 0)
					{
						DecayWarningIntervalHours = 2.5;
					}
					Save();
				}

				SanitizePublicPermissions();
				if (EnsureDefaultKitLifeTimes())
				{
					Save();
				}
			}
			else
			{	//File does not exist create file
				if (!FileExist(DirPATH))
				{
					MakeDirectory(DirPATH);
				}
				NoBuildZones.Insert(new MultiTerritoriesNoBuildZones(3703.5, 5985.11, 100));
				NoBuildZones.Insert(new MultiTerritoriesNoBuildZones(8345.61, 5985.93, 100));
				FlagRefreshFrequency = GetCEApi().GetCEGlobalInt("FlagRefreshFrequency");
				if (FlagRefreshFrequency <= 0)
				{
					FlagRefreshFrequency = GameConstants.REFRESHER_FREQUENCY_DEFAULT;
				}
				AddDefaultKitLifeTimes();
				Save();
			}
		}
	}

	protected void SanitizePublicPermissions()
	{
		int blockedPublicPerms = TerritoryPerm.DISMANTLE;
		int blockedEnabled = PublicPermissions & blockedPublicPerms;
		if (blockedEnabled == 0)
		{
			return;
		}

		PublicPermissions = PublicPermissions - blockedEnabled;
		Print("[MultiTerritories] Removed unsafe public dismantle permission from PublicPermissions. New value=" + PublicPermissions);
		Save();
	}
	
	void Save()
	{
		JsonFileLoader<MultiTerritoriesConfig>.JsonSaveFile(ConfigPATH, this);
	}

	void DebugLog(string message)
	{
		if (DebugLogging > 0)
		{
			Print("[MultiTerritories] " + message);
		}
	}

	void DiagnosticLog(string message)
	{
		if (TerritoryDiagnostics > 0)
		{
			Print("[MultiTerritories][Diagnostics] " + message);
		}
	}

	int disableBaseDamage()
	{
		return DisableBaseDamage;
	}

	float GetProtectionRadius()
	{
		if (ProtectionRadius > 0)
		{
			return ProtectionRadius;
		}

		return GameConstants.REFRESHER_RADIUS;
	}

	int GetProtectionRefreshIntervalSeconds()
	{
		if (ProtectionRefreshIntervalSeconds > 0)
		{
			return ProtectionRefreshIntervalSeconds;
		}

		return FlagRefreshFrequency;
	}

	float GetFloppyLifetimeSeconds(float defaultDuration)
	{
		if (FloppyLifetimeDays > 0)
		{
			return FloppyLifetimeDays * 24 * 60 * 60;
		}

		if (FloppyHealthDrainMultiplier <= 0)
		{
			return defaultDuration;
		}

		return defaultDuration * FloppyHealthDrainMultiplier;
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

	bool ShouldWarnOutsideTerritoryBuild()
	{
		return AllowPublicBuildOutsideTerritoryWarnings > 0;
	}

	bool HasMemberLimit()
	{
		return MaxMembersPerTerritory > 0;
	}
	
	bool IsInWhiteList(string item)
	{
		if (WhiteList && WhiteList.Count() > 0)
		{
			for (int i = 0; i < WhiteList.Count(); i++)
			{
				item.ToLower();
				string wItem = WhiteList.Get(i);
				wItem.ToLower();
				if (item.Contains(wItem))
				{
					return true;
				}
			}
		}

		return false;
	}
	
	bool CanWarnPlayer(string message = "")
	{
		message.ToLower();
		int curTime = GetGame().GetTime();
		if ( curTime > m_BlockWarnPlayer || m_BlockLastMessage != message)
		{
			m_BlockLastMessage = message;
			m_BlockWarnPlayer = curTime + 6000;
			return true;
		}

		return false;
	}
	
	
	int PublicPermission()
	{
		return PublicPermissions;
	}
	
	int MemberPermission()
	{
		return MemberPermissions;
	}
	
	void SendNotification(string text, string icon = "Multiplix/images/NoBuild.paa")
	{
		if (GetGame().IsClient() && CanWarnPlayer(text))
		{
			if (GetGame().GetMission())
			{
				GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", text, ""));
			}
			else
			{
				GetGame().Chat(text, "");
			}
		}
	}
	
	bool CanBuildHere(vector pos, string item = "")
	{
		if (IsInWhiteList(item))
		{
			return true;
		}

		if (NoBuildZones)
		{
			for (int i = 0; i < NoBuildZones.Count(); i++)
			{
				if (NoBuildZones.Get(i) && NoBuildZones.Get(i).Check(pos))
				{
					return false;
				}
			}
		}

		return true;
	}
	
	string NiceExpireTime(float LifeTime)
	{
		float Hours = Math.Floor(LifeTime / 3600);
		
		DebugLog("Hour: " + Hours + " RefreshFrequency:" + FlagRefreshFrequency);
		int rtnValue = 0;
		if (LifeTime < FlagRefreshFrequency)
		{
			return ""; // Means that this item wouldn't get affected by refresh anyway.
		}

		int Days = Math.Floor(Hours / 24);
		int remander = Days % 7;
		if (remander >= 2)
		{
			return Days.ToString() + " Days";
		}

		int Weeks = Math.Floor(Days / 7);
		return Weeks.ToString() + " Weeks";
	}
	
	int GetKitLifeTime(string item)
	{
		item.ToLower();
		int lt = KitLifeTimes.Get(item);
		if (lt == 0)
		{
			foreach (string key, int lifetime : KitLifeTimes)
			{
				if (item.Contains(key))
				{
					return lifetime;
				}
			}
		} else 
		{
			return lt;
		}

		return 0;
	}
}

class MultiTerritoriesNoBuildZones
{
	string Name = "";
	float X;
	float Z;
	float R;
	bool DrawOnMap = false;
	
	void MultiTerritoriesNoBuildZones(float x, float z, float r)
	{
		X = x;
		Z = z;
		R = r;
	}
	
	//Returns True If is in zone
	bool Check(vector checkPos)
	{
		if (checkPos){
			vector ZeroedHeightPos = Vector(checkPos[0], 0,checkPos[2]);
			if (vector.Distance(ZeroedHeightPos, Vector(X, 0, Z)) <= R)
			{
				return true;
			}
		}

		return false;
	}
	
	vector GetPos()
	{
		return Vector(X, GetGame().SurfaceY(X,Z),Z);
	}
}

ref MultiTerritoriesConfig m_MultiTerritoriesConfig;

//Helper function to return Config
static MultiTerritoriesConfig GetMultiTerritoriesConfig()
{
	if (!m_MultiTerritoriesConfig)
	{
		m_MultiTerritoriesConfig = new MultiTerritoriesConfig;
			
		if ( GetGame().IsServer() ){
			m_MultiTerritoriesConfig.Load();
		}
	}

	return m_MultiTerritoriesConfig;
};

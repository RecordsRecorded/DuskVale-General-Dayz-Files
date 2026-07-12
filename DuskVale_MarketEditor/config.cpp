class CfgPatches
{
	class DuskVale_MarketEditor
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] =
		{
			"DZ_Data",
			"DZ_Scripts",
			"JM_CF_Scripts",
			"DayZExpansion_Market_Scripts",
			"LBmaster_Core",
			"LBmaster_AdminTools"
		};
	};
};

class CfgMods
{
	class DuskVale_MarketEditor
	{
		dir = "DuskVale_MarketEditor";
		name = "DuskVale Live Market Editor";
		credits = "DuskVale";
		author = "DuskVale";
		authorID = "";
		version = "1.1";
		extra = 0;
		type = "mod";
		dependencies[] =
		{
			"Game",
			"World",
			"Mission"
		};

		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] =
				{
					"DuskVale_MarketEditor/scripts/3_Game"
				};
			};
			class missionScriptModule
			{
				value = "";
				files[] =
				{
					"DuskVale_MarketEditor/scripts/5_Mission"
				};
			};
		};
	};
};

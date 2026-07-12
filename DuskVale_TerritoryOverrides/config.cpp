class CfgPatches
{
	class DuskVale_TerritoryOverrides
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] =
		{
			"DZ_Data",
			"DZ_Scripts",
			"ElysianTerritoriesPlus",
			"LBmaster_Groups"
		};
	};
};

class CfgMods
{
	class DuskVale_TerritoryOverrides
	{
		dir = "DuskVale_TerritoryOverrides";
		name = "DuskVale Territory Overrides";
		credits = "DuskVale";
		author = "DuskVale";
		authorID = "";
		version = "1.0";
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
					"DuskVale_TerritoryOverrides/scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value = "";
				files[] =
				{
					"DuskVale_TerritoryOverrides/scripts/4_World"
				};
			};
			class missionScriptModule
			{
				value = "";
				files[] =
				{
					"DuskVale_TerritoryOverrides/scripts/5_Mission"
				};
			};
		};
	};
};

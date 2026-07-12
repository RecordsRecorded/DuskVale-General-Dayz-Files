class CfgPatches
{
	class ElysianTerritoriesPlus_LBmasterPlotpoleBridge
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] =
		{
			"DZ_Data",
			"ElysianTerritoriesPlus",
			"LBmaster_Groups"
		};
	};
};

class CfgMods
{
	class ElysianTerritoriesPlus_LBmasterPlotpoleBridge
	{
		dir = "ElysianTerritoriesPlus_LBmasterPlotpoleBridge";
		name = "ElysianTerritoriesPlus LBmaster Plotpole Bridge";
		credits = "Multiplix";
		author = "Multiplix";
		authorID = "";
		version = "0.3";
		type = "mod";
		dependencies[] =
		{
			"World",
			"Mission"
		};

		class defs
		{
			class worldScriptModule
			{
				value = "";
				files[] =
				{
					"ElysianTerritoriesPlus_LBmasterPlotpoleBridge/scripts/4_World"
				};
			};
			class missionScriptModule
			{
				value = "";
				files[] =
				{
					"ElysianTerritoriesPlus_LBmasterPlotpoleBridge/scripts/5_Mission"
				};
			};
		};
	};
};

class CfgPatches {
	class LBmaster_Groups {
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {
			"DZ_Data",
			"DZ_Gear_Navigation",
			"LBmaster_Core"
		};
	};
};
class CfgMods {
	class LBmaster_Groups {
		dir = "LBmaster_Groups";
		name = "Advanced Groups";

		version = "2.0";
		type = "mod";
		author = "LBmaster";
		credits = "LBmaster";
		authorID = "76561198141097113";
		hideName = 0;
		hidePicture = 0;
		picture = "LBmaster_Groups\logo\logo.paa";
		logoSmall = "LBmaster_Groups\logo\logo.paa";
		logo = "LBmaster_Groups\logo\logo.paa";
		logoOver = "LBmaster_Groups\logo\logo.paa";
		tooltip = "LBmasters high end Groups Mod";
		overview = "The Most advanced DayZ Groups Mod. Tactical Ping, Subgroups, 3D Player Markers, GPS, Compass, Map, Group shared Markers, Private Markers and many more";
		action = "https://lbmaster.de";
		defines[] = {
			"LBGROUP_SYSTEM",
			"LBGROUP_SYSTEM_NEW"
		};
		inputs = "LBmaster_Groups/inputsLBmaster.xml";
		dependencies[] = {
			"Game",
			"World",
			"Mission"
		};
		class defs {
			class gameScriptModule {
				value = "";
				files[] = {
					"VPPAdminTools/Definitions",
					"KingOfTheHillAssets/scripts/Common",
					"DayZExpansion/Missions/Scripts/Common",
					"0_DayZExpansion_Missions_Preload/Common",
					"LBmaster_Groups/scripts/3_Game"
				};
			};
			class worldScriptModule {
				value = "";
				files[] = {
					"VPPAdminTools/Definitions",
					"KingOfTheHillAssets/scripts/Common",
					"DayZExpansion/Missions/Scripts/Common",
					"0_DayZExpansion_Missions_Preload/Common",
					"LBmaster_Groups/scripts/4_World"
				};
			};
			class missionScriptModule {
				value = "";
				files[] = {
					"VPPAdminTools/Definitions",
					"KingOfTheHillAssets/scripts/Common",
					"DayZExpansion/Missions/Scripts/Common",
					"0_DayZExpansion_Missions_Preload/Common",
					"LBmaster_Groups/scripts/5_Mission"
				};
			};
		};
	};
};

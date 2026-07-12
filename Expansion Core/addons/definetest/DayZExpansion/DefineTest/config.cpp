////////////////////////////////////////////////////////////////////
//DeRap: config.bin
//Produced from mikero's Dos Tools Dll version 10.14
//https://mikero.bytex.digital/Downloads
//'now' is Fri Jul 03 20:29:03 2026 : 'file' last modified on Tue Nov 18 17:20:40 2025
////////////////////////////////////////////////////////////////////

#define _ARMA_

class CfgPatches
{
	class DayZExpansion_DefineTest
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {};
	};
};
class CfgMods
{
	class DZ_Expansion_DefineTest
	{
		type = "mod";
		dependencies[] = {"Game","World","Mission"};
		class defs
		{
			class engineScriptModule
			{
				files[] = {"DayZExpansion/DefineTest/Common","DayZExpansion/DefineTest/1_Core"};
			};
			class gameLibScriptModule
			{
				files[] = {"DayZExpansion/DefineTest/Common","DayZExpansion/DefineTest/2_GameLib"};
			};
			class gameScriptModule
			{
				files[] = {"DayZExpansion/DefineTest/Common","DayZExpansion/DefineTest/3_Game"};
			};
			class worldScriptModule
			{
				files[] = {"DayZExpansion/DefineTest/Common","DayZExpansion/DefineTest/4_World"};
			};
			class missionScriptModule
			{
				files[] = {"DayZExpansion/DefineTest/Common","DayZExpansion/DefineTest/5_Mission"};
			};
		};
	};
};

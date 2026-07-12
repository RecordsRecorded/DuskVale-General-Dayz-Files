////////////////////////////////////////////////////////////////////
//DeRap: config.bin
//Produced from mikero's Dos Tools Dll version 10.14
//https://mikero.bytex.digital/Downloads
//'now' is Fri Jul 03 20:24:38 2026 : 'file' last modified on Thu Feb 19 11:33:41 2026
////////////////////////////////////////////////////////////////////

#define _ARMA_

class CfgPatches
{
	class JM_CF_Scripts
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgMods
{
	class JM_CommunityFramework
	{
		dir = "JM/CF";
		picture = "";
		action = "";
		hideName = 0;
		hidePicture = 1;
		name = "Community Framework";
		credits = "Jacob_Mango, Arkensor";
		creditsJson = "JM/CF/Scripts/Data/Credits.json";
		version = "1.5.8";
		author = "Jacob_Mango";
		authorID = "76561198103677868";
		extra = 0;
		type = "mod";
		defines[] = {"CF_MODULE_CONFIG","CF_EXPRESSION","CF_TRACE_STACK_NAME_ASSUMPTION_FIX","CF_GHOSTICONS","CF_MODSTORAGE","CF_SURFACES","CF_MODULES","CF_REF_FIX","CF_BUGFIX_REF","CF_BUGFIX_XML","CF_VIRTUALSTORAGE_FIX","CF_DOUBLYLINKEDNODES","CF_ONUPDATE_RATE_LIMIT","CF_LOG_TIMESTAMP"};
		dependencies[] = {"Core","Game","World","Mission"};
		class defs
		{
			class engineScriptModule
			{
				value = "";
				files[] = {"JM/CF/Scripts/1_Core"};
			};
			class gameLibScriptModule
			{
				value = "";
				files[] = {"JM/CF/Scripts/2_GameLib"};
			};
			class gameScriptModule
			{
				value = "CF_CreateGame";
				files[] = {"JM/CF/Scripts/3_Game"};
			};
			class worldScriptModule
			{
				value = "";
				files[] = {"JM/CF/Scripts/4_World"};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {"JM/CF/Scripts/5_Mission"};
			};
		};
	};
};

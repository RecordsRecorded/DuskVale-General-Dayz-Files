/**
 * Mission.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

static void Expansion_PrintLoadedMods()
{
	Print("[DefineTest] CfgMods:");

	int mod_count = g_Game.ConfigGetChildrenCount("CfgMods");
	int i;

	for (i = 0; i < mod_count; i++)
	{
		string mod_name;
		g_Game.ConfigGetChildName("CfgMods", i, mod_name);

		Print("" + mod_name);
	}

	#ifdef EXPANSION_LOADORDER_DEBUG
	Print("[DefineTest] CfgPatches:");

	int patch_count = g_Game.ConfigGetChildrenCount("CfgPatches");

	for (i = 0; i < patch_count; i++)
	{
		string patch_name;
		g_Game.ConfigGetChildName("CfgPatches", i, patch_name);

		Print("" + patch_name);
	}
	#endif
}

static void Expansion_PrintDefines()
{
	#ifdef EXPANSIONMOD
	Print("[DefineTest] EXPANSIONMOD is defined");
	#endif

	#ifdef EXPANSIONMODAI
	Print("[DefineTest] EXPANSIONMODAI is defined");
	#endif

	#ifdef EXPANSIONMODANIMATIONS
	Print("[DefineTest] EXPANSIONMODANIMATIONS is defined");
	#endif

	#ifdef EXPANSIONMODBASEBUILDING
	Print("[DefineTest] EXPANSIONMODBASEBUILDING is defined");
	#endif

	#ifdef EXPANSIONMODBOOK
	Print("[DefineTest] EXPANSIONMODBOOK is defined");
	#endif

	#ifdef EXPANSIONMODCHAT
	Print("[DefineTest] EXPANSIONMODCHAT is defined");
	#endif

	#ifdef EXPANSIONMODCORE
	Print("[DefineTest] EXPANSIONMODCORE is defined");
	#endif

	#ifdef EXPANSIONMODGARAGE
	Print("[DefineTest] EXPANSIONMODGARAGE is defined");
	#endif

	#ifdef EXPANSIONMODGROUPS
	Print("[DefineTest] EXPANSIONMODGROUPS is defined");
	#endif

	#ifdef EXPANSIONMODHARDLINE
	Print("[DefineTest] EXPANSIONMODHARDLINE is defined");
	#endif

	#ifdef EXPANSIONMODKILLFEED
	Print("[DefineTest] EXPANSIONMODKILLFEED is defined");
	#endif

	#ifdef EXPANSIONMODMAPASSETS
	Print("[DefineTest] EXPANSIONMODMAPASSETS is defined");
	#endif

	#ifdef EXPANSIONMODMARKET
	Print("[DefineTest] EXPANSIONMODMARKET is defined");
	#endif

	#ifdef EXPANSIONMODMISSIONS
	Print("[DefineTest] EXPANSIONMODMISSIONS is defined");
	#endif

	#ifdef EXPANSIONMODNAMETAGS
	Print("[DefineTest] EXPANSIONMODNAMETAGS is defined");
	#endif

	#ifdef EXPANSIONMODNAVIGATION
	Print("[DefineTest] EXPANSIONMODNAVIGATION is defined");
	#endif

	#ifdef EXPANSIONMODP2PMARKET
	Print("[DefineTest] EXPANSIONMODP2PMARKET is defined");
	#endif

	#ifdef EXPANSIONMODPERSONALSTORAGE
	Print("[DefineTest] EXPANSIONMODPERSONALSTORAGE is defined");
	#endif

	#ifdef EXPANSIONMODQUESTS
	Print("[DefineTest] EXPANSIONMODQUESTS is defined");
	#endif

	#ifdef EXPANSIONMODSPAWNSELECTION
	Print("[DefineTest] EXPANSIONMODSPAWNSELECTION is defined");
	#endif

	#ifdef EXPANSIONMODVEHICLE
	Print("[DefineTest] EXPANSIONMODVEHICLE is defined");
	#endif

	#ifdef EXPANSIONMODWEAPONS
	Print("[DefineTest] EXPANSIONMODWEAPONS is defined");
	#endif
}

modded class MissionServer
{
	void MissionServer()
	{
		Expansion_PrintDefines();

		Expansion_PrintLoadedMods();
	}
}

modded class MissionGameplay
{
	void MissionGameplay()
	{
		Expansion_PrintDefines();

		Expansion_PrintLoadedMods();
	}
}

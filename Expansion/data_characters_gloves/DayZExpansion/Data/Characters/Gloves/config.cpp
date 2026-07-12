////////////////////////////////////////////////////////////////////
//DeRap: config.bin
//Produced from mikero's Dos Tools Dll version 10.14
//https://mikero.bytex.digital/Downloads
//'now' is Fri Jul 03 20:27:09 2026 : 'file' last modified on Fri Sep 23 14:50:49 2022
////////////////////////////////////////////////////////////////////

#define _ARMA_

class CfgPatches
{
	class Expansion_Gloves
	{
		units[] = {"ExpansionTacticalGlovesDesert"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Characters_Gloves"};
	};
};
class CfgVehicles
{
	class TacticalGloves_ColorBase;
	class ExpansionTacticalGlovesDesert: TacticalGloves_ColorBase
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"DayZExpansion\data\characters\gloves\data\tacticalgloves_desertmarpat_co.paa","DayZExpansion\data\characters\gloves\data\tacticalgloves_desertmarpat_co.paa","DayZExpansion\data\characters\gloves\data\tacticalgloves_desertmarpat_co.paa"};
	};
};

////////////////////////////////////////////////////////////////////
//DeRap: config.bin
//Produced from mikero's Dos Tools Dll version 10.14
//https://mikero.bytex.digital/Downloads
//'now' is Fri Jul 03 20:28:41 2026 : 'file' last modified on Mon Apr 24 12:00:30 2023
////////////////////////////////////////////////////////////////////

#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Structures_Industrial_Misc
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Structures_Walls","DayZExpansion_Core_Scripts"};
	};
};
class CfgNonAIVehicles
{
	class ExpansionLampLightBase;
	class Static_Lamp_Ind: ExpansionLampLightBase{};
	class Static_Lamp_Rail: ExpansionLampLightBase{};
};

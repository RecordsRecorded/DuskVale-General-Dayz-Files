////////////////////////////////////////////////////////////////////
//DeRap: config.bin
//Produced from mikero's Dos Tools Dll version 10.14
//https://mikero.bytex.digital/Downloads
//'now' is Fri Jul 03 20:26:08 2026 : 'file' last modified on Thu Feb 29 13:26:14 2024
////////////////////////////////////////////////////////////////////

#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Lighthouse_Lamp
	{
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class Expansion_Lighthouse_Lamp: HouseNoDestruct
	{
		scope = 1;
		model = "DayZExpansion\Objects\Structures\LightHouse_Lamp\Expansion_Lighthouse_Lamp.p3d";
	};
};

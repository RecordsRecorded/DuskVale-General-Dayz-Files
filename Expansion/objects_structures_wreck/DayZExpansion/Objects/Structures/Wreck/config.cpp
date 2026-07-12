////////////////////////////////////////////////////////////////////
//DeRap: config.bin
//Produced from mikero's Dos Tools Dll version 10.14
//https://mikero.bytex.digital/Downloads
//'now' is Fri Jul 03 20:26:06 2026 : 'file' last modified on Fri Sep 23 14:58:34 2022
////////////////////////////////////////////////////////////////////

#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_Structures_Wreck
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class Expansion_Wreck_Volha_Police: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\wrecks\Vehicles\Wreck_Volha_Police.p3d";
	};
};

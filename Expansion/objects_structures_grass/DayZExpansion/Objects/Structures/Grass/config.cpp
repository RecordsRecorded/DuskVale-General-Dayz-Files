////////////////////////////////////////////////////////////////////
//DeRap: config.bin
//Produced from mikero's Dos Tools Dll version 10.14
//https://mikero.bytex.digital/Downloads
//'now' is Fri Jul 03 20:26:21 2026 : 'file' last modified on Fri Sep 23 14:53:20 2022
////////////////////////////////////////////////////////////////////

#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_Structures_Grass
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class HouseNoDestruct;
	class ExpansionGrass10: HouseNoDestruct
	{
		scope = 1;
		model = "\DayZExpansion\Objects\Structures\Grass\Grass10M.p3d";
		vehicleClass = "Expansion_Static";
	};
};

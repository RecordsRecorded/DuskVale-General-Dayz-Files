////////////////////////////////////////////////////////////////////
//DeRap: config.bin
//Produced from mikero's Dos Tools Dll version 10.14
//https://mikero.bytex.digital/Downloads
//'now' is Fri Jul 03 20:26:24 2026 : 'file' last modified on Thu Apr 18 15:27:39 2024
////////////////////////////////////////////////////////////////////

#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_Structures_ExpansionFlag
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
	class ExpansionFlagProp: HouseNoDestruct
	{
		scope = 1;
		shadow = 0;
		model = "\DayZExpansion\Objects\Structures\ExpansionFlag\ExpansionFlagProp.p3d";
		vehicleClass = "Expansion_Static";
	};
};

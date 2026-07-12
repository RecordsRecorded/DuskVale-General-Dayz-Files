////////////////////////////////////////////////////////////////////
//DeRap: config.bin
//Produced from mikero's Dos Tools Dll version 10.14
//https://mikero.bytex.digital/Downloads
//'now' is Fri Jul 03 20:29:25 2026 : 'file' last modified on Thu Mar 16 20:27:46 2023
////////////////////////////////////////////////////////////////////

#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Core_Objects_Bases
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
	class ExpansionStaticObjectBase: HouseNoDestruct
	{
		scope = 0;
		vehicleClass = "Expansion_Static";
	};
};

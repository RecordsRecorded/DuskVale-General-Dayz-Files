////////////////////////////////////////////////////////////////////
//DeRap: config.bin
//Produced from mikero's Dos Tools Dll version 10.14
//https://mikero.bytex.digital/Downloads
//'now' is Fri Jul 03 20:29:17 2026 : 'file' last modified on Fri May 03 10:16:39 2024
////////////////////////////////////////////////////////////////////

#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Core_NavmeshCarvers
	{
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class ExpansionNavmeshCarver: HouseNoDestruct
	{
		scope = 0;
		carveNavmesh = 1;
		physLayer = "item_large";
	};
	class ExpansionLargeAnimalNavmeshCarver: ExpansionNavmeshCarver
	{
		scope = 1;
		model = "DayZExpansion\Core\Objects\NavmeshCarvers\AnimalNavmeshCarver.p3d";
	};
};

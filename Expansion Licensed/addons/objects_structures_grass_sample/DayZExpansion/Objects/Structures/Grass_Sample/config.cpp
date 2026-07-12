////////////////////////////////////////////////////////////////////
//DeRap: config.bin
//Produced from mikero's Dos Tools Dll version 10.14
//https://mikero.bytex.digital/Downloads
//'now' is Fri Jul 03 20:31:27 2026 : 'file' last modified on Fri Sep 23 14:50:57 2022
////////////////////////////////////////////////////////////////////

#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_Structures_Grass_Sample
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
	class Expansion_Grass_color1: HouseNoDestruct
	{
		scope = 1;
		shadow = 0;
		model = "\DayZExpansion\Objects\Structures\Grass_Sample\Expansion_Grass_color1.p3d";
		vehicleClass = "Expansion_Static";
	};
	class Expansion_Grass_color2: HouseNoDestruct
	{
		scope = 1;
		shadow = 0;
		model = "\DayZExpansion\Objects\Structures\Grass_Sample\Expansion_Grass_color2.p3d";
		vehicleClass = "Expansion_Static";
	};
	class Expansion_Grass_color3: HouseNoDestruct
	{
		scope = 1;
		shadow = 0;
		model = "\DayZExpansion\Objects\Structures\Grass_Sample\Expansion_Grass_color3.p3d";
		vehicleClass = "Expansion_Static";
	};
	class Expansion_Grass_Sample_Cluter_1: HouseNoDestruct
	{
		scope = 1;
		shadow = 0;
		model = "\DayZExpansion\Objects\Structures\Grass_Sample\Grass_Sample_Cluter_1.p3d";
		vehicleClass = "Expansion_Static";
	};
	class Expansion_Grass_Sample_Cluter_2: HouseNoDestruct
	{
		scope = 1;
		shadow = 0;
		model = "\DayZExpansion\Objects\Structures\Grass_Sample\Grass_Sample_Cluter_2.p3d";
		vehicleClass = "Expansion_Static";
	};
};

////////////////////////////////////////////////////////////////////
//DeRap: config.bin
//Produced from mikero's Dos Tools Dll version 10.14
//https://mikero.bytex.digital/Downloads
//'now' is Fri Jul 03 20:31:23 2026 : 'file' last modified on Wed Mar 18 17:17:50 2026
////////////////////////////////////////////////////////////////////

#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_Explosives_Licensed
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
	class ExpansionExplosive: Inventory_Base
	{
		scope = 1;
		displayName = "$STR_EXPANSION_HANDMADE_EXPLOSIVE";
		descriptionShort = "$STR_EXPANSION_HANDMADE_EXPLOSIVE_DESC";
		vehicleClass = "Expansion_Explosives";
		model = "\DayZExpansion\Objects\Weapons\Explosives\explosive.p3d";
		weight = 4;
		itemSize[] = {2,3};
		rotationFlags = 17;
		canBeSplit = 0;
		stackedUnit = "l";
	};
	class ExpansionExplosivePlacing: ExpansionExplosive
	{
		scope = 1;
		model = "\DayZExpansion\Objects\Weapons\Explosives\satchel_preview.p3d";
		slopeTolerance = 0.95;
		hiddenSelections[] = {"placing"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(1,1,1,1.0,co)"};
	};
	class ExpansionSatchel: ExpansionExplosive
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SATCHEL_EXPLOSIVE";
		descriptionShort = "$STR_EXPANSION_SATCHEL_EXPLOSIVE_DESC";
		vehicleClass = "Expansion_Explosives";
		model = "\DayZExpansion\Objects\Weapons\Explosives\satchel.p3d";
		weight = 4;
		itemSize[] = {3,4};
		rotationFlags = 17;
		canBeSplit = 0;
		stackedUnit = "l";
	};
	class ExpansionSatchelPlacing: ExpansionSatchel
	{
		scope = 1;
		model = "\DayZExpansion\Objects\Weapons\Explosives\satchel_preview.p3d";
		slopeTolerance = 0.95;
		hiddenSelections[] = {"placing"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(1,1,1,1.0,co)"};
	};
	class ExpansionExplosiveBase: ExpansionExplosive
	{
		scope = 1;
	};
};

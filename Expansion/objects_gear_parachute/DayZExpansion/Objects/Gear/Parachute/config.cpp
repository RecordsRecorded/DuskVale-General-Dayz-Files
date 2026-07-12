////////////////////////////////////////////////////////////////////
//DeRap: config.bin
//Produced from mikero's Dos Tools Dll version 10.14
//https://mikero.bytex.digital/Downloads
//'now' is Fri Jul 03 20:26:31 2026 : 'file' last modified on Tue Nov 18 17:21:20 2025
////////////////////////////////////////////////////////////////////

#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Parachute
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
	class ExpansionParachute: Inventory_Base
	{
		scope = 0;
		displayName = "DOESN'T WORK - DO NOT USE";
		descriptionShort = "DOESN'T WORK - DO NOT USE";
		model = "\DayZExpansion\Objects\Gear\Parachute\ExpansionParachute.p3d";
		rotationFlags = 1;
		inventorySlot[] = {"expansionparachute","expansionparachutepacked"};
		weight = 100;
	};
};

////////////////////////////////////////////////////////////////////
//DeRap: config.bin
//Produced from mikero's Dos Tools Dll version 10.14
//https://mikero.bytex.digital/Downloads
//'now' is Fri Jul 03 20:26:28 2026 : 'file' last modified on Fri Sep 23 14:53:16 2022
////////////////////////////////////////////////////////////////////

#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_Structures_BMX
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
	class ExpansionBMXLoop: HouseNoDestruct
	{
		scope = 1;
		model = "DayZExpansion\Objects\Structures\BMX\BMXLoop.p3d";
		hiddenSelections[] = {"texture"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(1,0,0,1,co)"};
		bounding = "BSphere";
		overrideDrawArea = "8.0";
		forceFarBubble = "true";
		physLayer = "item_large";
		carveNavmesh = 0;
	};
};

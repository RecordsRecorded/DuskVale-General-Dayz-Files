////////////////////////////////////////////////////////////////////
//DeRap: config.bin
//Produced from mikero's Dos Tools Dll version 10.14
//https://mikero.bytex.digital/Downloads
//'now' is Fri Jul 03 20:26:11 2026 : 'file' last modified on Fri Sep 23 14:58:33 2022
////////////////////////////////////////////////////////////////////

#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_Structures_Signs
	{
		units[] = {"Expansion_Decal_WelcomeToHell"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class Expansion_Decal_WelcomeToHell: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Signs\Decal_WelcomeToHell.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
};

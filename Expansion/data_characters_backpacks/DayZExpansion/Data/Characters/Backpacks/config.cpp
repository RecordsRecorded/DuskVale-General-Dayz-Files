////////////////////////////////////////////////////////////////////
//DeRap: config.bin
//Produced from mikero's Dos Tools Dll version 10.14
//https://mikero.bytex.digital/Downloads
//'now' is Fri Jul 03 20:27:11 2026 : 'file' last modified on Fri Sep 23 14:50:46 2022
////////////////////////////////////////////////////////////////////

#define _ARMA_

class CfgPatches
{
	class Expansion_Backpacks
	{
		units[] = {"ExpansionCoyoteBlack","ExpansionDesertTortilla"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Characters_Backpacks"};
	};
};
class CfgVehicles
{
	class Clothing_Base;
	class Clothing;
	class AliceBag_ColorBase;
	class ChildBag_ColorBase;
	class HuntingBag;
	class TaloonBag_ColorBase;
	class TortillaBag;
	class GhillieSuit_ColorBase;
	class ImprovisedBag;
	class AssaultBag_ColorBase;
	class CoyoteBag_ColorBase;
	class ExpansionCoyoteBlack: CoyoteBag_ColorBase
	{
		scope = 2;
		visibilityModifier = 0.95;
		hiddenSelectionsTextures[] = {"DayZExpansion\Data\characters\backpacks\data\CoyoteBP_black_co.paa","DayZExpansion\Data\characters\backpacks\data\CoyoteBP_black_co.paa","DayZExpansion\Data\characters\backpacks\data\CoyoteBP_black_co.paa"};
	};
	class ExpansionDesertTortilla: TortillaBag
	{
		scope = 2;
		visibilityModifier = 0.95;
		hiddenSelections[] = {"zbytek"};
		hiddenSelectionsTextures[] = {"DayZExpansion\Data\characters\backpacks\data\tortila_desertmarpat_co.paa"};
	};
};

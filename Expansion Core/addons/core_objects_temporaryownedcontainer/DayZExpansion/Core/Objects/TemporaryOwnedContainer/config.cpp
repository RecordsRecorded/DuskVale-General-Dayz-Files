////////////////////////////////////////////////////////////////////
//DeRap: config.bin
//Produced from mikero's Dos Tools Dll version 10.14
//https://mikero.bytex.digital/Downloads
//'now' is Fri Jul 03 20:29:12 2026 : 'file' last modified on Fri Sep 23 14:50:59 2022
////////////////////////////////////////////////////////////////////

#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_TemporaryOwnedContainer
	{
		units[] = {"ExpansionTemporaryOwnedContainer"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgVehicles
{
	class Container_Base;
	class ExpansionTemporaryOwnedContainer: Container_Base
	{
		scope = 2;
		displayName = "#STR_EXPANSION_TEMPORARY_STORAGE";
		descriptionShort = "#STR_EXPANSION_TEMPORARY_STORAGE_DESC";
		model = "\DZ\gear\camping\wooden_case.p3d";
		itemSize[] = {10,5};
		attachments[] = {"Shoulder","Melee","Vest","Body","Hips","Legs","Back","Headgear","Mask","Eyewear","Gloves","Feet","Armband"};
		class Cargo
		{
			itemsCargoSize[] = {10,100};
		};
	};
};

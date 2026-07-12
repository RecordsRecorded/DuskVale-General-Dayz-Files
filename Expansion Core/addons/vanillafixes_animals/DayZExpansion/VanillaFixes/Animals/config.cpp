////////////////////////////////////////////////////////////////////
//DeRap: config.bin
//Produced from mikero's Dos Tools Dll version 10.14
//https://mikero.bytex.digital/Downloads
//'now' is Fri Jul 03 20:28:33 2026 : 'file' last modified on Fri May 03 10:17:06 2024
////////////////////////////////////////////////////////////////////

#define _ARMA_

class CfgPatches
{
	class DayZExpansion_VanillaFixes_Animals
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Animals_bos_taurus","DZ_Animals_bos_taurus_fem","DZ_Animals_ursus_arctos"};
	};
};
class CfgVehicles
{
	class AnimalBase;
	class Animal_BosTaurus: AnimalBase
	{
		collidesWithCharacterOnDeath = 0;
		useExpansionNavMeshCarver = 1;
	};
	class Animal_BosTaurusF: AnimalBase
	{
		collidesWithCharacterOnDeath = 0;
		useExpansionNavMeshCarver = 1;
	};
	class Animal_UrsusArctos: AnimalBase
	{
		collidesWithCharacterOnDeath = 0;
		useExpansionNavMeshCarver = 1;
	};
};

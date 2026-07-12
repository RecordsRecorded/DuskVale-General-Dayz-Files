////////////////////////////////////////////////////////////////////
//DeRap: config.bin
//Produced from mikero's Dos Tools Dll version 10.14
//https://mikero.bytex.digital/Downloads
//'now' is Fri Jul 03 20:31:42 2026 : 'file' last modified on Wed Mar 05 08:24:13 2025
////////////////////////////////////////////////////////////////////

#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_Airdrop
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
	class ExpansionAirdropPlane: HouseNoDestruct
	{
		vehicleClass = "Expansion_Airdrop";
		scope = 2;
		model = "\DayZExpansion\Objects\Airdrop\AirdropC130J.p3d";
		forceFarBubble = "true";
		class AnimationSources
		{
			class gear
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 1;
			};
			class rotor
			{
				source = "user";
				animPeriod = 0.0009999999;
				initPhase = 0;
			};
		};
	};
};

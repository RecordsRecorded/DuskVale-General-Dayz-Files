////////////////////////////////////////////////////////////////////
//DeRap: config.bin
//Produced from mikero's Dos Tools Dll version 10.14
//https://mikero.bytex.digital/Downloads
//'now' is Fri Jul 03 20:31:40 2026 : 'file' last modified on Tue Nov 18 17:17:15 2025
////////////////////////////////////////////////////////////////////

#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_BaseBuilding_Gates
	{
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgVehicles
{
	class ExpansionKitLarge;
	class ExpansionBaseBuilding;
	class ExpansionBarrierGateKit: ExpansionKitLarge
	{
		scope = 2;
		rotationFlags = 4;
		displayName = "$STR_EXPANSION_GATE";
		descriptionShort = "$STR_EXPANSION_GATE_DESC";
		model = "\DayZExpansion\Objects\Basebuilding\Gates\BarrierGateKit.p3d";
		itemSize[] = {10,10};
		physLayer = "item_large";
		itemBehaviour = 0;
		attachments[] = {};
	};
	class ExpansionBarrierGateKitPlacing: ExpansionBarrierGateKit
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Basebuilding\Gates\BarrierGate_Preview.p3d";
		slopeTolerance = 0.85;
	};
	class ExpansionBarrierGate: ExpansionBaseBuilding
	{
		scope = 2;
		vehicleClass = "Expansion_Construction";
		expansionIsOpenable = 1;
		displayName = "$STR_EXPANSION_GATE";
		descriptionShort = "$STR_EXPANSION_GATE_DESC";
		model = "\DayZExpansion\Objects\Basebuilding\Gates\BarrierGate.p3d";
		allowOwnedCargoManipulation = 1;
		forceFarBubble = "true";
		carveNavmesh = 1;
		physLayer = "item_large";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 1000;
					healthLevels[] = {{1.0,{""}},{0.7,{""}},{0.5,{""}},{0.3,{""}},{0.0,{""}}};
				};
			};
			class GlobalArmor
			{
				class Projectile
				{
					Health = 0;
					Blood = 0;
					Shock = 0;
				};
			};
		};
		class AnimationSources
		{
			class gate
			{
				source = "user";
				animPeriod = 2;
				initPhase = 0;
			};
		};
	};
};

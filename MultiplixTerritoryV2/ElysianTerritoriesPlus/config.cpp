class CfgPatches
{
	class ElysianTerritoriesPlus
	{
		units[]=
		{
			"REV_KitBase",
			"REV_StaticBase",
			"REV_TerritoryComputer",
			"REV_TerritoryComputerKit",
			"Territory_Floppydisk",
			"REV_TerritoryComputer_Hologram"
		};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data",
			"DZ_Structures_Specific",
			"DZ_Gear_Camping",
			"DZ_Scripts",
			"DZ_Gear_Consumables"
		};
	};
};
class CfgMods
{
	class ElysianTerritoriesPlus
	{
		dir="ElysianTerritoriesPlus";
		picture="";
		action="";
		hideName=1;
		hidePicture=1;
		name="ElysianTerritoriesPlus";
		credits="Multiplix";
		author="DeltaQ";
		authorID="0";
		version="1.0";
		extra=0;
		type="mod";
		dependencies[]=
		{
			"World"
		};
		class defs
		{
			class gameScriptModule
			{
				value="";
				files[]=
				{
					"ElysianTerritoriesPlus\scripts\3_Game"
				};
			};
			class worldScriptModule
			{
				value="";
				files[]=
				{
					"ElysianTerritoriesPlus\scripts\4_World"
				};
			};
			class missionScriptModule
			{
				value="";
				files[]=
				{
					"ElysianTerritoriesPlus\scripts\5_Mission"
				};
			};
		};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class Inventory_Base;
	class Container_Base;
	class WorldContainer_Base;
	class Static;
	class BaseBuildingBase;
	class WoodenCrate: Container_Base
	{
	};
	class REV_KitBase: WoodenCrate
	{
		scope=0;
		displayName="Delta Construction Kit";
		descriptionShort="Can be dismantled with a screwdriver";
		model="";
		itemSize[]={4,3};
		canBeDigged=0;
		weight=2500;
		reversed=0;
		itemBehaviour=2;
		minPlacingDist=10;
		absorbency=0;
		rotationFlags=0;
		isMeleeWeapon=0;
		physLayer="item_large";
		lootCategory="Materials";
		class Cargo
		{
			itemsCargoSize[]={0,0};
			openable=0;
			allowOwnedCargoManipulation=0;
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class movement
				{
					soundSet="seachest_movement_SoundSet";
					id=1;
				};
				class pickUpItem_Light
				{
					soundSet="pickUpSeaChest_SoundSet";
					id=796;
				};
				class pickUpItem
				{
					soundSet="pickUpSeaChest_SoundSet";
					id=797;
				};
				class drop
				{
					soundset="seachest_drop_SoundSet";
					id=898;
				};
			};
		};
	};
	class REV_StaticBase: Container_Base
	{
		scope=2;
		displayName="Placeable Object";
		descriptionshort="An object that's able to be placed in your base.";
		carveNavmesh=1;
		yawPitchRollLimit[]={45,45,45};
		weight=2500;
		physLayer="item_large";
		class Cargo
		{
			itemsCargoSize[]={0,0};
			openable=0;
			allowOwnedCargoManipulation=1;
		};
	};
	class TerritoryFlag;
	class REV_TerritoryComputer: TerritoryFlag
	{
		scope=2;
		displayName="Territory Computer";
		descriptionShort="At the heart of every survivor's home base is one of these. Why do they need one you may ask? Because I said so, now stop asking questions.";
		model="ElysianTerritoriesPlus\Models\Computer\TerritoryComputer.p3d";
		createdProxiesOnInit[]={};
		attachments[]=
		{
			"Material_FPole_Flag"
		};
		slopeTolerance=0.15000001;
		yawPitchRollLimit[]={45,45,45};
		itemBehaviour=1;
		hiddenSelections[]=
		{
			"light_power",
			"light_working",
			"light_not_working",
			"light_has_floppy",
			"light_no_floppy",
			"screen_lcd",
			"screen_main"
		};
		hiddenSelectionsTextures[]=
		{
			"elysianterritoriesplus\models\computer\data\indicatorlights\camo_indicatorlights_co.paa",
			"elysianterritoriesplus\models\computer\data\indicatorlights\camo_indicatorlights_co.paa",
			"elysianterritoriesplus\models\computer\data\indicatorlights\camo_indicatorlights_co.paa",
			"elysianterritoriesplus\models\computer\data\indicatorlights\camo_indicatorlights_co.paa",
			"elysianterritoriesplus\models\computer\data\indicatorlights\camo_indicatorlights_co.paa",
			"elysianterritoriesplus\models\computer\data\segment_display\segment_display_co.paa",
			"elysianterritoriesplus\models\computer\data\main_display\camo_main_display_co.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"elysianterritoriesplus\models\computer\data\indicatorlights\camo_indicatorlights_off.rvmat",
			"elysianterritoriesplus\models\computer\data\indicatorlights\camo_indicatorlights_off.rvmat",
			"elysianterritoriesplus\models\computer\data\indicatorlights\camo_indicatorlights_off.rvmat",
			"elysianterritoriesplus\models\computer\data\indicatorlights\camo_indicatorlights_off.rvmat",
			"elysianterritoriesplus\models\computer\data\indicatorlights\camo_indicatorlights_off.rvmat",
			"elysianterritoriesplus\models\computer\data\segment_display\segment_display_off.rvmat",
			"elysianterritoriesplus\models\computer\data\main_display\camo_main_display_off.rvmat"
		};
		soundImpactType="metal";
		class AnimationSources
		{
			class switch_on
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=1;
			};
			class switch_off
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=0;
			};
			class floppy_visual
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=1;
			};
			class light_power
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=0;
			};
			class light_working
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=0;
			};
			class light_not_working
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=0;
			};
			class light_has_floppy
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=0;
			};
			class light_no_floppy
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=0;
			};
			class screen_lcd
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=0;
			};
			class screen_main
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=0;
			};
			class flag_mast
			{
				source="user";
				initPhase=1;
				animPeriod=1;
			};
		};
		class GUIInventoryAttachmentsProps
		{
			class Flag
			{
				name="Floppydisk";
				description="";
				attachmentSlots[]=
				{
					"Material_FPole_Flag"
				};
				icon="";
				selection="totem";
			};
		};
		class Construction
		{
		};
	};
	class Territory_Floppydisk: Inventory_Base
	{
		scope=2;
		displayName="Territory Computer Floppy Disk";
		descriptionShort="Contains data required for the Territory Computer to function.";
		inventorySlot[]=
		{
			"Material_FPole_Flag"
		};
		model="ElysianTerritoriesPlus\Models\Floppy\Floppydisk.p3d";
		weight=200;
		itemSize[]={2,2};
		absorbency=0.1;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=100;
					healthLevels[]=
					{
						
						{
							1,
							{}
						},
						
						{
							0.69999999,
							{}
						},
						
						{
							0.5,
							{}
						},
						
						{
							0.30000001,
							{}
						},
						
						{
							0,
							{}
						}
					};
				};
			};
		};
		class AnimationSources
		{
			class inserted
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=1;
			};
			class inventory
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=0;
			};
		};
	};
	class REV_TerritoryComputer_Hologram: Inventory_Base
	{
		scope=2;
		displayName="Territory Computer Hologram";
		model="ElysianTerritoriesPlus\Models\Computer\TerritoryComputer_Placing.p3d";
		hiddenSelections[]=
		{
			"placing"
		};
	};
	class REV_TerritoryComputerKit: REV_KitBase
	{
		scope=2;
		displayName="Territory Computer Kit";
		projectionTypename="REV_TerritoryComputer_Hologram";
		model="ElysianTerritoriesPlus\Models\Kit\TerritoryComputerKit.p3d";
		hiddenSelections[]=
		{
			"camo"
		};
		hiddenSelectionsTextures[]=
		{
			"ElysianTerritoriesPlus\models\kit\data\territorycomputerkit_co.paa"
		};
	};
};
class CfgSlots
{
	class Material_FPole_Flag
	{
		name="Territory_Floppydisk";
		displayName="Floppydisk";
		ghostIcon="";
	};
};
class CfgNonAIVehicles
{
	class ProxyAttachment;
	class ProxyFloppydisk: ProxyAttachment
	{
		scope=2;
		inventorySlot="Material_FPole_Flag";
		model="ElysianTerritoriesPlus\Models\Floppy\Floppydisk.p3d";
	};
	class ProxyFloppydisk_visual: ProxyAttachment
	{
		scope=2;
		inventorySlot="temp_floppy_slot";
		model="ElysianTerritoriesPlus\Models\Floppy\Floppydisk_visual.p3d";
	};
};
class CfgSoundSets
{
	class baseAnimal_SoundSet;
	class Elysian_TerritoryComputer_SoundSet
	{
		soundShaders[]=
		{
			"Elysian_TerritoryComputer_SoundShader"
		};
		sound3DProcessingType="smallItem3DProcessingType";
		volumeCurve="LinearCurve";
		volumeFactor=0.5;
		spatial=1;
		loop=1;
	};
	class Elysian_TerritoryComputer_SwitchOn_SoundSet
	{
		soundShaders[]=
		{
			"Elysian_TerritoryComputer_SwitchOn_SoundShader"
		};
		sound3DProcessingType="smallItem3DProcessingType";
		volumeCurve="LinearCurve";
		volumeFactor=0.5;
		spatial=1;
		loop=0;
	};
	class Elysian_TerritoryComputer_SwitchOff_SoundSet
	{
		soundShaders[]=
		{
			"Elysian_TerritoryComputer_SwitchOff_SoundShader"
		};
		sound3DProcessingType="smallItem3DProcessingType";
		volumeCurve="LinearCurve";
		volumeFactor=0.5;
		spatial=1;
		loop=0;
	};
	class Elysian_TerritoryComputer_InsertFloppy_SoundSet
	{
		soundShaders[]=
		{
			"Elysian_TerritoryComputer_InsertFloppy_SoundShader"
		};
		sound3DProcessingType="smallItem3DProcessingType";
		volumeCurve="LinearCurve";
		volumeFactor=0.25;
		spatial=1;
		loop=0;
	};
	class Elysian_TerritoryComputer_EjectFloppy_SoundSet
	{
		soundShaders[]=
		{
			"Elysian_TerritoryComputer_EjectFloppy_SoundShader"
		};
		sound3DProcessingType="smallItem3DProcessingType";
		volumeCurve="LinearCurve";
		volumeFactor=0.25;
		spatial=1;
		loop=0;
	};
	class Elysian_TerritoryComputer_BootSequence_SoundSet
	{
		soundShaders[]=
		{
			"Elysian_TerritoryComputer_BootSequence_SoundShader"
		};
		sound3DProcessingType="smallItem3DProcessingType";
		volumeCurve="LinearCurve";
		volumeFactor=0.5;
		spatial=1;
		loop=0;
	};
};
class CfgSoundShaders
{
	class Elysian_TerritoryComputer_SoundShader
	{
		samples[]=
		{
			
			{
				"ElysianTerritoriesPlus\Sounds\computer_loop",
				1
			}
		};
		volume=1;
		range=5;
	};
	class Elysian_TerritoryComputer_SwitchOn_SoundShader
	{
		samples[]=
		{
			
			{
				"ElysianTerritoriesPlus\Sounds\switch_on",
				1
			}
		};
		volume=1;
		range=5;
	};
	class Elysian_TerritoryComputer_SwitchOff_SoundShader
	{
		samples[]=
		{
			
			{
				"ElysianTerritoriesPlus\Sounds\switch_off",
				1
			}
		};
		volume=1;
		range=5;
	};
	class Elysian_TerritoryComputer_InsertFloppy_SoundShader
	{
		samples[]=
		{
			
			{
				"ElysianTerritoriesPlus\Sounds\insert_floppy",
				1
			}
		};
		volume=1;
		range=5;
	};
	class Elysian_TerritoryComputer_EjectFloppy_SoundShader
	{
		samples[]=
		{
			
			{
				"ElysianTerritoriesPlus\Sounds\eject_floppy",
				1
			}
		};
		volume=1;
		range=5;
	};
	class Elysian_TerritoryComputer_BootSequence_SoundShader
	{
		samples[]=
		{
			
			{
				"ElysianTerritoriesPlus\Sounds\computer_startup",
				1
			}
		};
		volume=1;
		range=5;
	};
};

class CfgPatches
{
	// Change the name to whatever you want
	class DuskVale_Test
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data"
		};
	};
};

// This is where your slots are made

class CfgSlots
{
	// Changes the slot name, Edit as: Slot_EditMePlease
	class Slot_DHBattery1
	{
		//Actual Class Name (one you look for in admin menu)
		name="Key_Ring1";
		//Display name (one you see in game)
		displayName="Key Ring 1";
		//What do you want the icon for the slot to be?
		ghostIcon="DHClothingPack\Data\Vests\Pouches\utilitypouch.p3d";
	};
};

class CfgVehicles
	// Place thing you want to modify here
{
	class FOG_Bag_6SHTBag
	{
		units[] = {"FOG_Bag_6ShBagT_Green","FOG_Bag_6ShBagT_BlackEMR","FOG_Bag_6ShBagT_Black","FOG_Bag_6ShBagT_RG","FOG_Bag_6ShBagT_CB","FOG_Bag_6ShBagT_White","FOG_Bag_6ShBagT_EMR_White"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Characters","DZ_Scripts"};
	};
};
class cfgVehicles
{
	class Clothing;
	class Inventory_Base;
	class FOG_Bag_6ShTBag_ColorBase: Clothing
	{
		scope = 0;
		displayName = "6SH118 Raid Backpack";
		descriptionShort = "Large capacity backpack, also cleverly nicknamed the Couch Bag";
		model = "FOG_MOD\Bags\6Sh118T\models\Bag6SHT_g.p3d";
		inventorySlot[] = {"Back"};
		itemInfo[] = {"Clothing","Back"};
		simulation = "clothing";
		rotationFlags = 16;
		itemSize[] = {10,12};
		quickBarBonus = 2;
		itemsCargoSize[] = {10,20};
		weight = 1600;
		varWetMax = 0.049;
		heatIsolation = 0.8;
		repairableWithKits[] = {5,2};
		repairCosts[] = {30,25};
		soundAttType = "Outdoor";
		randomQuantity = 4;
		canBeDigged = 0;
		hiddenSelections[] = {"camoflage"};
		hiddenSelectionsTextures[] = {"FOG_MOD\Bags\6Sh118T\data\6sh118T_co.paa"};
		attachments[] += {"Chemlight","WalkieTalkie","Backpack_1","Key_Ring1"};
		class ClothingTypes
		{
			male = "FOG_MOD\Bags\6Sh118T\models\Bag6SHT.p3d";
			female = "FOG_MOD\Bags\6Sh118T\models\Bag6SHT.p3d";
		};
		class Proxyutilitypouch
		{
			scope=0;
			inventorySlot[]=
			{
				"Key_Ring1"
			};
		/*class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 300;
					transferToAttachmentsCoef = 0.5;
					healthLevels[] = {{1.0,{"FOG_MOD\Bags\6Sh118T\data\6sh118T.rvmat"}},{0.7,{"FOG_MOD\Bags\6Sh118T\data\6sh118T.rvmat"}},{0.5,{"FOG_MOD\Bags\6Sh118T\data\6sh118T_damage.rvmat"}},{0.3,{"FOG_MOD\Bags\6Sh118T\data\6sh118T_damage.rvmat"}},{0.0,{"FOG_MOD\Bags\6Sh118T\data\6sh118T_destruct.rvmat"}}};
				};
			};
		}; */
		class AnimEvents
		{
			class SoundWeapon
			{
				class pickUpItem_Light
				{
					soundSet = "pickUpBackPack_Plastic_Light_SoundSet";
					id = 796;
				};
				class pickUpItem
				{
					soundSet = "pickUpBackPack_Plastic_SoundSet";
					id = 797;
				};
				class drop
				{
					soundset = "taloonbag_drop_SoundSet";
					id = 898;
				};
			};
		};
	};
	class FOG_Bag_6ShBagT_Green: FOG_Bag_6ShTBag_ColorBase
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"FOG_MOD\Bags\6Sh118T\data\6sh118T_co.paa"};
	};
	class FOG_Bag_6ShBagT_BlackEMR: FOG_Bag_6ShTBag_ColorBase
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"FOG_MOD\Bags\6Sh118T\data\6sh118T_black_EMR_co.paa"};
	};
	class FOG_Bag_6ShBagT_Black: FOG_Bag_6ShTBag_ColorBase
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"FOG_MOD\Bags\6Sh118T\data\6sh118T_Black_co.paa"};
	};
	class FOG_Bag_6ShBagT_RG: FOG_Bag_6ShTBag_ColorBase
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"FOG_MOD\Bags\6Sh118T\data\6sh118T_RG_co.paa"};
	};
	class FOG_Bag_6ShBagT_CB: FOG_Bag_6ShTBag_ColorBase
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"FOG_MOD\Bags\6Sh118T\data\6sh118T_CB_co.paa"};
	};
	class FOG_Bag_6ShBagT_White: FOG_Bag_6ShTBag_ColorBase
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"FOG_MOD\Bags\6Sh118T\data\6sh118T_White_co.paa"};
	};
	class FOG_Bag_6ShBagT_EMR_White: FOG_Bag_6ShTBag_ColorBase
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"FOG_MOD\Bags\6Sh118T\data\6sh118T_EMR_White_co.paa"};
	};
};


class CfgNonAIVehicles
{
	class ProxyAttahchment;
	// The proxynameofitem.p3d file should be changed.
	class Proxyutilitypouch: ProxyAttahchment
	{
		scope=2;
		// What you name the slot
		inventorySlot="Key_Ring1"
		model="DHClothingPack\Data\Vests\Pouches\utilitypouch.p3d";
	};
};


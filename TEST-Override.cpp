class CfgPatches
{
	class DH_youtubetut
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

class CfgSlots
	{
		class Slot_AClock
		{
			name="AClock";
			displayName="A Clock";
			ghostIcon="batteryd";
		};
	};

class CfgVehicles
{
	class HouseNoDestruct;
	class Inventory_Base;
	class Container_Base;
	class WoodenCrate;
	class ItemBase;
	
	class DH_youtube_MBox1: WoodenCrate
	{
		scope=2;
		displayName="Metal Box";
		descriptionShort="A Metal Box For Base Storage - By King Skyline :D";
		model="tutorial1\Data\box.p3d.";
		attachments[] = {"AClock"};
		inventorySlot=
			{
				"WoodenCrate",
				"woodencrate_1",
				"woodencrate_2",
				"woodencrate_3",
				"woodencrate_4",
				"Truck_01_WoodenCrate1",
				"Truck_01_WoodenCrate2",
				"Truck_01_WoodenCrate3",
				"Truck_01_WoodenCrate4",
				"Truck_01_WoodenCrate5",
				"Truck_01_WoodenCrate6",
				"sea_chest",
				"sea_chest_1",
				"sea_chest_2",
				"sea_chest_3",
				"sea_chest_4",
			};
			forceFarBubble="true";
			physLayer="item_Large";
			itemsize[]={4,8};
			class Cargo
			{
				itemsCargoSize[]={10,7};
			};
		};
	class AlaramClock_ColorBase: Inventory_Base
	{
		Scope = 0;
		inventorySlot[]=
		{
			"AClock"
		};
	};
};

class CfgNonAiVehicles
{
	class ProxyAttachment;
	
	class Proxyutilitypouch: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "AClock";
		model = "tutoial\Data\AclockProx.p3d"
	};
};

/// This is a test - code demonstrating how to create an attachment slot, icon for the attachment slot, name for the attachment slot, and item(s) allowed in said attachment slot.
	
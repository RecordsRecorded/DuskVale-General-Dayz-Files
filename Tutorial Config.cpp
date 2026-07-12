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

class CfgMods
{
	class Addon_Name
	{
		dir="Addon_Name";
		picture="";
		action="";
		hideName=1;
		hidePicture=1;
		name="Addon_Name";
		credits="";
		author="";
		authorID="";
		version="1.0";
		extra=0;
		type="mod";
		dependencies[]=
		{
			"Game",
			"World",
			"Mission"
		};
		
		/* class defs
		{
			class gameScriptModule
			{
				value="";
				files=
				{
					"BeginnerBasics/scripts/3_game"
				};
			};
			class missionScriptModule
			{
				value="";
				files[]=
				{
					"BeginnerBasics/scripts/3_game"
				};
			};
		}; */
	};
};

class CfgVehicles
{
	// Items you Want to mod will go here
};


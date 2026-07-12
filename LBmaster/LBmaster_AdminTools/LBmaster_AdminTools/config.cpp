class CfgPatches
{
	class LBmaster_AdminTools
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]={"LBmaster_Core"};
	};
};

class CfgMods
{
	class LBmaster_AdminTools
	{
		dir="LBmaster_AdminTools";
		picture="";
		action="";
		hideName=0;
		hidePicture=1;
		name="LBmaster_AdminTools";
		author="LBmaster";
		credits="LBmaster";
		authorID="0";
		version="1.0";
		type="mod";
		inputs = "LBmaster_AdminTools/inputsLBmaster.xml";
		dependencies[]=
		{
			"Game",
			"World",
			"Mission"
		};
		class defs
		{
			class gameScriptModule
			{
				value="";
				files[]=
				{
					"LBmaster_AdminTools/scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value="";
				files[]=
				{
					"LBmaster_AdminTools/scripts/4_World"
				};
			};
			class missionScriptModule
			{
				value="";
				files[]=
				{
					"LBmaster_AdminTools/scripts/5_Mission"
				};
			};
		};
	};
};
class CfgVehicles {
	
	class Inventory_Base;
	class LBAdminHandcuffs: Inventory_Base
	{
		scope=2;
		displayName="$STR_CfgVehicles_HandcuffsLocked0";
		model="\dz\gear\tools\Handcuffs_locked.p3d";
		debug_ItemCategory=2;
		animClass="Knife";
		itemSize[]={2,1};
		rotationFlags=17;
		weight=380;
		fragility=0.01;
		OnRestrainChange="Handcuffs";
	};
};

////////////////////////////////////////////////////////////////////
//DeRap: config.bin
//Produced from mikero's Dos Tools Dll version 10.14
//https://mikero.bytex.digital/Downloads
//'now' is Fri Jul 03 20:28:59 2026 : 'file' last modified on Thu Apr 18 15:28:54 2024
////////////////////////////////////////////////////////////////////

#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Dta_Game
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Vehicles_Wheeled"};
	};
};
class CfgVehicles
{
	class Car;
	class Inventory_Base;
	class CarScript: Car
	{
		hasDoors = 1;
		hornSoundSetEXT = "Expansion_Horn_Ext_SoundSet";
		hornSoundSetINT = "Expansion_Horn_Int_SoundSet";
	};
};
class CfgNonAIVehicles
{
	class StaticObject;
	class Static_hangar_lamp: StaticObject{};
	class Static_light_bathroom: StaticObject{};
	class Static_light_kitchen_03: StaticObject{};
	class Static_luxury_lamp: StaticObject{};
	class Static_luxury_light: StaticObject{};
	class Static_FuelStation_Shed: StaticObject{};
	class Static_FuelStation_Shed_Enoch: StaticObject{};
	class Static_Airfield_Light_PAPI1: StaticObject{};
	class Static_Airfield_Light_Edge: StaticObject{};
	class Static_Airfield_Light_Navig: StaticObject{};
	class Static_Airfield_Lamp: StaticObject{};
	class Proxyfluor_lamp
	{
		scope = 2;
		model = "\DZ\structures\furniture\lighting\fluor_lamp\fluor_lamp.p3d";
	};
	class Proxyhangar_lamp
	{
		scope = 2;
		model = "\DZ\structures\furniture\lighting\hangar_lamp\hangar_lamp.p3d";
	};
	class Proxylight_bathroom
	{
		scope = 2;
		model = "\DZ\structures\furniture\lighting\light_bathroom\light_bathroom.p3d";
	};
	class Proxylight_kitchen_03
	{
		scope = 2;
		model = "\DZ\structures\furniture\lighting\light_kitchen_03\light_kitchen_03.p3d";
	};
	class Proxyluxury_lamp
	{
		scope = 2;
		model = "\DZ\structures\furniture\lighting\luxury_lamp\lamp.p3d";
	};
	class Proxyluxury_light
	{
		scope = 2;
		model = "\DZ\structures\furniture\lighting\luxury_light\luxury_light.p3d";
	};
};

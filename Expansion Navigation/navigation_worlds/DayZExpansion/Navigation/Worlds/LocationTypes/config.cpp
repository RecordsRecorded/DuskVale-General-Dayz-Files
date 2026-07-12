////////////////////////////////////////////////////////////////////
//DeRap: LocationTypes\config.bin
//Produced from mikero's Dos Tools Dll version 10.14
//https://mikero.bytex.digital/Downloads
//'now' is Sat Jul 11 20:20:11 2026 : 'file' last modified on Wed Jan 07 13:18:09 2026
////////////////////////////////////////////////////////////////////

#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Navigation_LocationTypes
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Gear_Navigation"};
	};
};
class CfgLocationTypes
{
	class Name
	{
		name = "keypoint";
		drawStyle = "name";
		texture = "#(argb,1,1,1)color(1,1,1,1)";
		color[] = {1.0,1.0,1.0,1};
		size = 0;
		font = "gui/fonts/MetronBook-Bold58";
		textSize = 0.03;
		shadow = 1;
		importance = 1;
	};
	class NameIcon
	{
		name = "keypoint";
		drawStyle = "icon";
		texture = "#(argb,1,1,1)color(1,1,1,1)";
		color[] = {1.0,1.0,1.0,1};
		size = 0;
		font = "gui/fonts/MetronBook-Bold58";
		textSize = 0.03;
		shadow = 0;
		importance = 1;
	};
	class Capital: Name
	{
		textSize = 0.06;
		importance = 7;
	};
	class City: Name
	{
		textSize = 0.05;
		importance = 6;
	};
	class Village: Name
	{
		textSize = 0.037;
		color[] = {0.761,0.761,0.761,1};
		importance = 5;
	};
	class Local: Name
	{
		textSize = 0.040000003;
		color[] = {0.761,0.761,0.761,1};
		importance = 2;
	};
	class Marine: Name
	{
		color[] = {0.05,0.4,0.8,0.8};
		textSize = 0.03;
		importance = 2;
	};
	class Ruin: NameIcon
	{
		texture = "\DZ\gear\navigation\data\map_ruin_ca.paa";
		textSize = 0.03;
		importance = 2;
		size = 7;
	};
	class Camp: NameIcon
	{
		color[] = {0.44,0.38,0.3,1};
		texture = "\DZ\gear\navigation\data\map_camp_ca.paa";
		textSize = 0.04;
		size = 9;
	};
	class Hill: NameIcon
	{
		color[] = {1.0,1.0,1.0,1};
		texture = "\DZ\gear\navigation\data\map_hill_ca.paa";
		size = 9;
	};
	class ViewPoint: NameIcon
	{
		color[] = {0.78,0,0.05,1};
		texture = "\DZ\gear\navigation\data\map_viewpoint_ca.paa";
		size = 9;
	};
	class RockArea: NameIcon
	{
		color[] = {1,1,1,1};
		texture = "\DZ\gear\navigation\data\map_rock_ca.paa";
		size = 9;
	};
	class RailroadStation: NameIcon
	{
		color[] = {1,1,1,1};
		texture = "\DZ\gear\navigation\data\map_station_ca.paa";
		size = 9;
	};
	class IndustrialSite: NameIcon
	{
		color[] = {1,1,1,1};
		texture = "\DZ\gear\navigation\data\map_factory_ca.paa";
		size = 9;
	};
	class LocalOffice: NameIcon
	{
		color[] = {0.22,0.21,0.51,1};
		texture = "\DZ\gear\navigation\data\map_govoffice_ca.paa";
		size = 10;
	};
	class BorderCrossing: NameIcon
	{
		color[] = {0.78,0,0.05,1};
		texture = "\DZ\gear\navigation\data\map_border_cross_ca.paa";
		size = 9;
	};
	class VegetationBroadleaf: NameIcon
	{
		color[] = {0.25,0.4,0.2,1};
		texture = "\DZ\gear\navigation\data\map_broadleaf_ca.paa";
		size = 9;
	};
	class VegetationFir: NameIcon
	{
		color[] = {0.25,0.4,0.2,1};
		texture = "\DZ\gear\navigation\data\map_fir_ca.paa";
		size = 9;
	};
	class VegetationPalm: NameIcon
	{
		color[] = {0.25,0.4,0.2,1};
		texture = "\DZ\gear\navigation\data\map_palm_ca.paa";
		size = 9;
	};
	class VegetationVineyard: NameIcon
	{
		color[] = {0.25,0.4,0.2,1};
		texture = "\DZ\gear\navigation\data\map_vineyard_ca.paa";
		size = 9;
	};
};

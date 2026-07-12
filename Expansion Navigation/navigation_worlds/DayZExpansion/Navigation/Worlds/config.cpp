////////////////////////////////////////////////////////////////////
//DeRap: config.bin
//Produced from mikero's Dos Tools Dll version 10.14
//https://mikero.bytex.digital/Downloads
//'now' is Sat Jul 11 20:20:11 2026 : 'file' last modified on Wed Jan 07 13:18:09 2026
////////////////////////////////////////////////////////////////////

#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Navigation_Worlds
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class MapDefaults
{
	x = 0.1;
	y = 0.1;
	w = 0.8;
	h = 0.6;
	ptsPerSquareSea = 8;
	ptsPerSquareTxt = 10;
	ptsPerSquareCLn = 10;
	ptsPerSquareExp = 10;
	ptsPerSquareCost = 10;
	ptsPerSquareFor = 9;
	ptsPerSquareForEdge = 15;
	ptsPerSquareRoad = 4;
	ptsPerSquareObj = 15;
	scaleMin = 0.001;
	scaleMax = 1.0;
	scaleDefault = 0.16;
	maxSatelliteAlpha = 1.0;
	alphaFadeStartScale = 2;
	alphaFadeEndScale = 2;
	userMapPath = "";
	maxUserMapAlpha = 0;
	alphaUserMapFadeStartScale = 0.34;
	alphaUserMapFadeEndScale = 0.34;
	showCountourInterval = 1;
	colorBackground[] = {0.969,0.957,0.949,1.0};
	colorSea[] = {0.467,0.631,0.851,0.5};
	colorForest[] = {0.38,0.47,0.23,0.0};
	colorForestBorder[] = {0.0,0.0,0.0,0.0};
	colorRocks[] = {0.0,0.0,0.0,0.3};
	colorRocksBorder[] = {0.0,0.0,0.0,0.0};
	colorLevels[] = {0.286,0.177,0.094,0.5};
	colorMainCountlines[] = {0.572,0.354,0.188,0.5};
	colorCountlines[] = {0.572,0.354,0.188,0.0};
	colorMainCountlinesWater[] = {0.491,0.577,0.702,0.6};
	colorCountlinesWater[] = {0.491,0.577,0.702,0.3};
	colorPowerLines[] = {0.1,0.1,0.1,1.0};
	colorRailWay[] = {0.8,0.2,0.0,1.0};
	colorNames[] = {0.1,0.1,0.1,0.9};
	colorInactive[] = {1.0,1.0,1.0,0.0};
	colorOutside[] = {0.0,0.0,0.0,1.0};
	colorTracks[] = {0.84,0.76,0.65,0.15};
	colorTracksFill[] = {0.84,0.76,0.65,1.0};
	colorRoads[] = {0.7,0.7,0.7,1.0};
	colorRoadsFill[] = {1.0,1.0,1.0,1.0};
	colorMainRoads[] = {0.9,0.5,0.3,1.0};
	colorMainRoadsFill[] = {1.0,0.6,0.4,1.0};
	colorGrid[] = {0.1,0.1,0.1,0.6};
	colorGridMap[] = {0.1,0.1,0.1,0.6};
	colorTrails[] = {0.84,0.76,0.65,0.15};
	colorTrailsFill[] = {0.84,0.76,0.65,0.65};
	colorMountPoint[] = {0.45,0.4,0.25,1};
	widthRailWay = 4.0;
	fontLabel = "gui/fonts/MetronBook-Bold28";
	sizeExLabel = 0.04;
	fontGrid = "gui/fonts/MetronBook28";
	sizeExGrid = 0.015;
	fontUnits = "gui/fonts/MetronBook-Bold28";
	sizeExUnits = 0.04;
	fontNames = "gui/fonts/MetronBook-Bold28";
	sizeExNames = 0.04;
	fontInfo = "gui/fonts/MetronBook-Bold28";
	sizeExInfo = 0.04;
	fontLevel = "gui/fonts/MetronBook28";
	sizeExLevel = 0.01;
	text = "#(argb,8,8,3)color(1,1,1,1)";
	textureComboBoxColor = "#(argb,8,8,3)color(1,1,1,1)";
	class Legend
	{
		x = 0.05;
		y = 0.85;
		w = 0.4;
		h = 0.1;
		font = "gui/fonts/MetronBook-Bold58";
		sizeEx = 0.02;
		colorBackground[] = {1,1,1,0.5};
		color[] = {0,0,0,1};
	};
	class Bush
	{
		icon = "\dz\gear\navigation\data\map_bush_ca.paa";
		color[] = {0.23,0.32,0.1,0.8};
		size = 14;
		importance = "0.2 * 14 * 0.05";
		coefMin = 0.25;
		coefMax = 4;
	};
	class SmallTree
	{
		icon = "\dz\gear\navigation\data\map_smalltree_ca.paa";
		color[] = {0.23,0.32,0.1,0.8};
		size = 12;
		importance = "0.6 * 12 * 0.05";
		coefMin = 0.25;
		coefMax = 4;
	};
	class Tree
	{
		icon = "\dz\gear\navigation\data\map_tree_ca.paa";
		color[] = {0.23,0.32,0.1,0.8};
		size = 12;
		importance = "0.9 * 16 * 0.05";
		coefMin = 0.25;
		coefMax = 4;
	};
	class Rock
	{
		icon = "\dz\gear\navigation\data\map_rock_ca.paa";
		color[] = {0.1,0.1,0.1,0.8};
		size = 12;
		importance = "0.5 * 12 * 0.05";
		coefMin = 0.25;
		coefMax = 4;
	};
	class Fuelstation
	{
		icon = "\dz\gear\navigation\data\map_fuelstation_ca.paa";
		size = 16;
		color[] = {1,1,1,1};
		importance = "2 * 16 * 0.05";
		coefMin = 0.75;
		coefMax = 4;
	};
	class Lighthouse
	{
		icon = "\dz\gear\navigation\data\map_lighthouse_ca.paa";
		size = 14;
		color[] = {1,1,1,1};
		importance = "3 * 16 * 0.05";
		coefMin = 0.9;
		coefMax = 4;
	};
	class Stack
	{
		icon = "\dz\gear\navigation\data\map_stack_ca.paa";
		size = 16;
		color[] = {1,1,1,1};
		importance = "2 * 16 * 0.05";
		coefMin = 0.9;
		coefMax = 4;
	};
	class Transmitter
	{
		icon = "\dz\gear\navigation\data\map_transmitter_ca.paa";
		color[] = {1,1,1,1};
		size = 16;
		importance = "2 * 16 * 0.05";
		coefMin = 0.9;
		coefMax = 4;
	};
	class Watertower
	{
		icon = "\dz\gear\navigation\data\map_watertower_ca.paa";
		color[] = {0.2,0.45,0.7,1};
		size = 16;
		importance = "1.2 * 16 * 0.05";
		coefMin = 0.9;
		coefMax = 4;
	};
	class Shipwreck
	{
		icon = "\dz\gear\navigation\data\map_shipwreck_ca.paa";
		color[] = {1,1,1,1};
		size = 16;
		importance = "1.2 * 16 * 0.05";
		coefMin = 0.9;
		coefMax = 4;
	};
	class Monument
	{
		icon = "\dz\gear\navigation\data\map_monument_ca.paa";
		color[] = {1,1,1,1};
		size = 20;
		importance = "1.2 * 16 * 0.05";
		coefMin = 0.25;
		coefMax = 4;
	};
	class BusStop
	{
		icon = "\dz\gear\navigation\data\map_busstop_ca.paa";
		color[] = {0.15,0.26,0.87,1};
		size = 12;
		importance = "1 * 10 * 0.05";
		coefMin = 0.25;
		coefMax = 4;
	};
	class Fountain
	{
		icon = "\dz\gear\navigation\data\map_fountain_ca.paa";
		color[] = {0.2,0.45,0.7,1};
		size = 15;
		importance = "1 * 12 * 0.05";
		coefMin = 0.25;
		coefMax = 4;
	};
	class Waterpump
	{
		icon = "\dz\gear\navigation\data\map_waterpump_ca.paa";
		color[] = {0.2,0.45,0.7,1};
		size = 16;
		importance = "1 * 12 * 0.05";
		coefMin = 0.25;
		coefMax = 4;
	};
	class Waterspring
	{
		icon = "\dz\gear\navigation\data\map_waterspring_ca.paa";
		color[] = {0.1,0.3,0.55,1};
		size = 16;
		importance = "1 * 12 * 0.05";
		coefMin = 0.75;
		coefMax = 4;
	};
	class Hospital
	{
		icon = "\dz\gear\navigation\data\map_hospital_ca.paa";
		color[] = {0.15,0.26,0.87,1};
		size = 17;
		importance = "2 * 16 * 0.05";
		coefMin = 0.5;
		coefMax = 4;
	};
	class Store
	{
		icon = "\dz\gear\navigation\data\map_store_ca.paa";
		color[] = {1,1,1,1};
		size = 20;
		importance = "2 * 16 * 0.05";
		coefMin = 0.5;
		coefMax = 4;
	};
	class Police
	{
		icon = "\dz\gear\navigation\data\map_police_ca.paa";
		color[] = {0.11,0.1,0.25,1};
		size = 20;
		importance = "2 * 16 * 0.05";
		coefMin = 0.5;
		coefMax = 4;
	};
	class FireDep
	{
		icon = "\dz\gear\navigation\data\map_firedep_ca.paa";
		color[] = {0.764,0,0,1};
		size = 20;
		importance = "2 * 16 * 0.05";
		coefMin = 0.5;
		coefMax = 4;
	};
	class TouristSign
	{
		icon = "\dz\gear\navigation\data\map_tsign_ca.paa";
		color[] = {1,1,1,1};
		size = 18;
		importance = "2 * 16 * 0.05";
		coefMin = 0.5;
		coefMax = 4;
	};
	class Tourism
	{
		icon = "\dz\gear\navigation\data\map_tourism_ca.paa";
		color[] = {1,1,1,1};
		size = 14;
		importance = "2 * 16 * 0.05";
		coefMin = 0.5;
		coefMax = 4;
	};
	class Cross
	{
		icon = "\dz\gear\navigation\data\map_cross_ca.paa";
		size = 16;
		color[] = {1,1,1,1};
		importance = "0.7 * 16 * 0.05";
		coefMin = 0.25;
		coefMax = 4;
	};
	class Chapel
	{
		icon = "\dz\gear\navigation\data\map_chapel_ca.paa";
		color[] = {1,1,1,1};
		size = 16;
		importance = "1 * 16 * 0.05";
		coefMin = 0.9;
		coefMax = 4;
	};
	class Church
	{
		icon = "\dz\gear\navigation\data\map_church_ca.paa";
		color[] = {1,1,1,1};
		size = 10;
		importance = "2 * 16 * 0.05";
		coefMin = 0.9;
		coefMax = 4;
	};
	class ViewTower
	{
		icon = "\dz\gear\navigation\data\map_viewtower_ca.paa";
		color[] = {1,1,1,1};
		size = 16;
		importance = "2.5 * 16 * 0.05";
		coefMin = 0.5;
		coefMax = 4;
	};
	class TouristShelter
	{
		icon = "\dz\gear\navigation\data\map_tshelter_ca.paa";
		color[] = {1,1,1,1};
		size = 16;
		importance = "2 * 16 * 0.05";
		coefMin = 0.5;
		coefMax = 4;
	};
	class Quay
	{
		icon = "\dz\gear\navigation\data\map_quay_ca.paa";
		size = 16;
		color[] = {1,1,1,1};
		importance = "2 * 16 * 0.05";
		coefMin = 0.5;
		coefMax = 4;
	};
	class Bunker
	{
		icon = "\dz\gear\navigation\data\map_bunker_ca.paa";
		color[] = {1,1,1,1};
		size = 14;
		importance = "1.5 * 14 * 0.05";
		coefMin = 0.25;
		coefMax = 4;
	};
	class Fortress
	{
		icon = "\dz\gear\navigation\data\map_fortress_ca.paa";
		size = 16;
		color[] = {1,1,1,1};
		importance = "2 * 16 * 0.05";
		coefMin = 0.25;
		coefMax = 4;
	};
	class Rocks
	{
		icon = "\dz\gear\navigation\data\map_rock_ca.paa";
		color[] = {0.1,0.1,0.1,0.8};
		size = 12;
		importance = "0.5 * 12 * 0.05";
		coefMin = 0.25;
		coefMax = 4;
	};
	class Ruin
	{
		icon = "\dz\gear\navigation\data\map_ruin_ca.paa";
		size = 16;
		color[] = {1,1,1,1};
		importance = "1.2 * 16 * 0.05";
		coefMin = 1;
		coefMax = 4;
	};
};
class RscMapControl: MapDefaults
{
	colorCountlines[] = {0.572,0.354,0.188,0.2};
	colorTracks[] = {0.2,0.137,0.078,0};
	colorTracksFill[] = {0.2,0.137,0.078,0};
	colorRoads[] = {0.663,0.663,0.663,1};
	colorRoadsFill[] = {0.663,0.663,0.663,1};
	colorMainRoads[] = {0.518,0.298,0.008,1};
	colorMainRoadsFill[] = {0.518,0.298,0.008,1};
	class Bush
	{
		icon = "\dz\gear\navigation\data\map_bush_ca.paa";
		color[] = {0.23,0.32,0.1,0.2};
		size = 14;
		importance = "0.2 * 14 * 0.05";
		coefMin = 0.25;
		coefMax = 4;
	};
	class SmallTree
	{
		icon = "\dz\gear\navigation\data\map_smalltree_ca.paa";
		color[] = {0.23,0.32,0.1,0.3};
		size = 12;
		importance = "0.6 * 12 * 0.05";
		coefMin = 0.25;
		coefMax = 4;
	};
};

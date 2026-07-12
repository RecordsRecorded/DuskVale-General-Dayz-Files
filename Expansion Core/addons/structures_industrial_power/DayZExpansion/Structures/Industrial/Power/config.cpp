////////////////////////////////////////////////////////////////////
//DeRap: config.bin
//Produced from mikero's Dos Tools Dll version 10.14
//https://mikero.bytex.digital/Downloads
//'now' is Fri Jul 03 20:28:37 2026 : 'file' last modified on Mon Apr 24 12:00:31 2023
////////////////////////////////////////////////////////////////////

#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Structures_Industrial_Power
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Structures_Walls","DayZExpansion_Core_Scripts"};
	};
};
class CfgNonAIVehicles
{
	class ExpansionLampLightBase;
	class Static_Power_Pole_Wood1_Lamp: ExpansionLampLightBase
	{
		position = "-0.000977 3.65 0.418945";
	};
	class Static_Power_Pole_Conc4_Lamp: ExpansionLampLightBase
	{
		position = "-0.81982 5.0445 -0.01017";
	};
	class Static_Power_Pole_Wood1_Lamp_Amp: ExpansionLampLightBase
	{
		position = "-0.000977 3.65 0.418945";
	};
	class Static_Power_Pole_Conc4_Lamp_Amp: ExpansionLampLightBase
	{
		position = "-0.81982 5.0445 -0.01017";
	};
};

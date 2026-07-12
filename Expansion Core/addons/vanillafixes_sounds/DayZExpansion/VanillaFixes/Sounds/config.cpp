////////////////////////////////////////////////////////////////////
//DeRap: config.bin
//Produced from mikero's Dos Tools Dll version 10.14
//https://mikero.bytex.digital/Downloads
//'now' is Fri Jul 03 20:28:26 2026 : 'file' last modified on Wed Dec 20 15:37:08 2023
////////////////////////////////////////////////////////////////////

#define _ARMA_

class CfgPatches
{
	class DayZExpansion_VanillaFixes_Sounds
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Sounds_Effects"};
	};
};
class CfgSoundShaders
{
	class baseVehicles_SoundShader;
	class offroad_engine_failed_start_battery_SoundShader: baseVehicles_SoundShader
	{
		volume = 0.7;
	};
};

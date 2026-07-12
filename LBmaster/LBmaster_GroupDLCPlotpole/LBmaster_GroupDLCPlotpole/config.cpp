class CfgPatches {
	class LBmaster_GroupDLCPlotpole {
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {
		};
	};
};
class CfgMods {
	class LBmaster_GroupDLCPlotpole {
		dir = "LBmaster_GroupDLCPlotpole";
		name = "Advanced Groups Plotpole DLC";

		version = "2.0";
		type = "mod";
		author = "LBmaster";
		credits = "LBmaster";
		authorID = "76561198141097113";
		hideName = 0;
		hidePicture = 0;
		picture = "LBmaster_GroupDLCPlotpole\logo\logo.paa";
		logoSmall = "LBmaster_GroupDLCPlotpole\logo\logo.paa";
		logo = "LBmaster_GroupDLCPlotpole\logo\logo.paa";
		logoOver = "LBmaster_GroupDLCPlotpole\logo\logo.paa";
		tooltip = "LBmasters Plotpole DLC";
		overview = "Plotpole DLC for Advanced Groups binding the Teritorry Flag to a Group";
		action = "https://lbmaster.de";
		defines[] = {
			"LBMASTER_PLOTPOLE_CLIENT",
			"LBMASTER_GROUP_PLOTPOLE"
		};
	};
};

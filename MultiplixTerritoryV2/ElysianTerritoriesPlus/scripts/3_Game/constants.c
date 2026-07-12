class TerritoryPerm 
{
	static int DEFAULTOWNER = 1;
	static int DEFAULTMEMBER = DEPLOY + BUILD + DISMANTLE + LOWERFLAG + ADDMEMBER;
	static int PUBLIC = LOWERFLAG + BUILD;
	static int ADMIN = 1; // 0000 0001
	static int DEPLOY = 2; // 0000 0010
	static int BUILD = 4; // 0000 0100
	static int DISMANTLE = 8; // 0000 1000
	static int LOWERFLAG = 16; // 0001 0000
	static int ADDMEMBER = 32; // 0010 0000
	static int REMOVEMEMBER = 64; // 0100 0000
	static int DEPLOYSPECIAL = 128; // 1000 0000   // Going to be a item white list
	static int REMOVECARCOVER = 256; // 0000 0001 0000 0000  
}

class TerritoryIcons 
{
	static string NoBuildZone				= "Multiplix/images/NoBuild.paa";
	static string TerritoryConflict			= "Multiplix/images/NoBuild.paa";
	static string WithinTerritory			= "Multiplix/images/NoBuild.paa";
	static string DeSpawnWarning			= "Multiplix/images/Build.paa";
	static string BuildPartWarning			= "Multiplix/images/NoBuild.paa";
	static string DismantleWarning			= "Multiplix/images/NoBuild.paa";
	static string LowerFlagWarning			= "Multiplix/images/NoBuild.paa";
	static string TerritoryRequiredWarning	= "Multiplix/images/NoBuild.paa";
}

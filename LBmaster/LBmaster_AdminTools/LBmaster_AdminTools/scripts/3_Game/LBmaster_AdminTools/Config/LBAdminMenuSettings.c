class LBAdminMenuSettings : LBConfigLoader<LBAdminMenuSettings_> {

	override void InitVars() {
		InitVarsInternal("AdminTool", "settings.json", LBConfigType.CONFIG, true); // (easy) Do not touch
	}

	override string GetRequiredVersion() {
		return "Admin_Tools";
	}

	override bool IsClientSideConfig() {
		return true;
	}

}
// This configs contains basic settings for the admin menu behaviour (this should be edited ingame in the admin menu itself!)
class LBAdminMenuSettings_ : LBConfigBase {
	static ref set<string> highlightedPlayers = new set<string>();

	const static int CURRENT_VERSION = 17;

	bool showPlayerInInvisible = false;
	ref array<ref LBESPFilter> espFilters = new array<ref LBESPFilter>();
	ref array<ref array<string>> itemspawnerCategories = {{"Rifle_Base", "Weapon_Base", "Pistol_Base", "Fal_Base", "DefaultMagazine", "Inventory_Base", "Transport", "CarWheel", "CarDoor", "DayZInfected", "SurvivorBase", "AnimalBase"}, {"Transport"}, {"CarWheel", "CarDoor"}, {"House", "Plant"}, {"Clothing_Base"}, {"Container_Base"}, {"Rifle_Base", "Weapon_Base", "Pistol_Base", "Fal_Base"}, {"Magazine_Base"}, {"ItemOptics"}, {"SurvivorBase"}, {"DayZInfected"}, {"AnimalBase"}, {"Edible_Base"}, {"Trap_Base"}};
	ref TStringArray itemspawnerCategoryNames = {"All", "Vehicles", "Car Parts", "Static Objects", "Clothing", "Containers", "Weapons", "Magazines + Ammo", "Optics", "Survivors", "Zombies", "Animals", "Food", "Traps"};
	LBItemSpawnerLocation itemSpawnerDefaultSpawn = LBItemSpawnerLocation.INVENTORY;
	bool displayPlayerTagsInPlayerList = true;
	int presetSpawnerCategory = 0;
	int teleportManagerCategory = 0;
	bool alertJoinWatchlistLocal = true;
	bool alertJoinWatchlistServer = true;
	bool alertJoinWatchlistGlobal = true;
	ref LBColorConfig frameColor = LBColorConfig.Init(200, 100, 100, 100);
	int lastEspDistanceMode = 0;
	int lastEspDistance = 200;
	bool espShowClassnames = false;
	bool espSkeletonHighDetail = false;
	bool espSkeletonDrawJoints = false;
	bool espSkeletonIgnoreDistance = false;
	int espSkeletonColor = -1;
	int espSkeletonColorDead = 0xffff0000;
	int espJointsColor = 0xffff0000;
	bool espEnableDrag = false;
	bool espToSurface = false;
	bool espDragCamera = false;
	int espMinHealth = 0;
	int espMaxHealth = 4;
	float freecamTurbo = 4.0;
	float freecamMouseSensitivity = 15.0;
	float freecamFOV = 55.0;
	int freecamMouseSmooth = 0;
	int freecamMovementSmooth = 0;
	bool freecamAllowUnderground = true;
	float freecamSpeed = 3.0;
	bool freecamResetFOVOnEnter = false;
	bool freecamResetFOVOnLettingGo = false;
	bool itemMapListShowNames = true;
	bool itemMapListShowChildCount = true;
	bool objB_snap_ground = false;
	bool objB_snap_object = true;
	bool objB_camera_rotation = true;
	bool lookingAtCopyItemname = true, lookingAtCopyPos = true, lookingAtCopyDir = true;
	int playerListSort = 0;
	bool mapShowPlayers = true;
	bool mapShowVehicles = true;
	int mapPlayerColor = 0xffffffff;
	int mapVehicleColor = 0xff00ff80;
	int mapBoatColor = 0xff0080ff;
	int mapHeliColor = 0xff0080ff;
	int highlightedPlayerColor = 0xffff2030;
	int watchlistLastCategory = 0;
	int banlistLastCategory = 0;
	int lookAtPositionFormat = 0;
	string lookAtPositionCustomFormat = "<x, y, z>";
	int lookAtDirectionFormat = 0;
	string lookAtDirectionCustomFormat = "<yaw, pitch, roll>";
	ref map<string, ref map<string, ref TStringSet>> constructionEditorConfig = new map<string, ref map<string, ref TStringSet >> ();
	ref map<string, string> constructionEditorFaviourites = new map<string, string>();
	ref TStringArray lastCommands = new TStringArray();

	override void LoadDefault() {
		espFilters.Insert(new LBESPFilter("#lb_adm_players", {"SurvivorBase"}, LBColorConfig.Init(255, 255, 255, 0)));
		espFilters.Insert(new LBESPFilter("#lb_adm_animals", {"AnimalBase"}, LBColorConfig.Init(255, 0, 255, 0)));
		espFilters.Insert(new LBESPFilter("#lb_adm_zombies", {"DayZInfected"}, LBColorConfig.Init(255, 255, 0, 0)));
		espFilters.Insert(new LBESPFilter("#lb_adm_vehicles", {"Transport"}, LBColorConfig.Init(255, 0, 255, 255)));
		espFilters.Insert(new LBESPFilter("#lb_adm_weapons", {"Weapon_Base", "Rifle_Base", "Pistol_Base", "Fal_Base"}, LBColorConfig.Init(255, 255, 255, 255)));
		espFilters.Insert(new LBESPFilter("#lb_adm_magazines", {"DefaultMagazine"}, LBColorConfig.Init(255, 255, 255, 255)));
		espFilters.Insert(new LBESPFilter("#lb_adm_items", {"Fal_Base", "Pistol_Base", "Rifle_Base", "Weapon_Base", "DefaultMagazine", "Inventory_Base", "CarWheel", "CarDoor"}, LBColorConfig.Init(255, 255, 255, 255)));
		AddDefaultConstructions();
	}

	override void UpdateVersion() {
		if (version < 2) {
			AddDefaultConstructions();
		}
		if (version < 3) {
			espSkeletonHighDetail = false;
			espSkeletonDrawJoints = false;
			espSkeletonColor = -1;
			espJointsColor = 0xffff0000;
		}
		if (version < 4) {
			freecamTurbo = 4.0;
			freecamMouseSmooth = 0;
			freecamMovementSmooth = 0;
			freecamAllowUnderground = true;
			freecamSpeed = 3.0;
		}
		if (version < 5) {
			espSkeletonColorDead = 0xffff0000;
			freecamMouseSensitivity = 15.0;
		}
		if (version < 8) {
			lastCommands = new TStringArray();
		}
		if (version < 9) {
			espMinHealth = 0;
			espMaxHealth = 4;
		}
		if (version < 10)
			freecamFOV = 55.0;
		if (version < 15) {
			lookAtPositionCustomFormat = "<x, y, z>";
			lookAtDirectionCustomFormat = "<yaw, pitch, roll>";
		}
		if (version < 16) {
			foreach (array<string> category : itemspawnerCategories) {
				if (category.Find("Weapon_Base") != -1) {
					if (category.Find("Rifle_Base") == -1)
						category.Insert("Rifle_Base");
					if (category.Find("Fal_Base") == -1)
						category.Insert("Fal_Base");
					if (category.Find("Pistol_Base") == -1)
						category.Insert("Pistol_Base");
				}
			}
			foreach (LBESPFilter filter : espFilters) {
				if (filter.items.Find("Weapon_Base") != -1) {
					if (filter.items.Find("Rifle_Base") == -1)
						filter.items.Insert("Rifle_Base");
					if (filter.items.Find("Fal_Base") == -1)
						filter.items.Insert("Fal_Base");
					if (filter.items.Find("Pistol_Base") == -1)
						filter.items.Insert("Pistol_Base");
				}
			}
		}
		if (version < 17) {
			highlightedPlayerColor = 0xffff2030;
			displayPlayerTagsInPlayerList = true;
		}
	}

	string GetFavouriteConstructionConfig(string type) {
		string found;
		if (constructionEditorFaviourites.Find(type, found))
			return found;
		return "";
	}

	void SetContructionConfigFavourite(string type, string preset) {
		constructionEditorFaviourites.Set(type, preset);
	}

	void AddDefaultConstructions() {
		SaveConstructedParts2("bbp_dgate", "T2 Metal", {"frame", "t1_wall_logs_horizontal", "t1_wall_logs_horizontal_2", "t2_metal", "t2_metal_2"}, false);
		SaveConstructedParts2("bbp_dgate", "T1 Wood", {"frame", "t1_wall_logs_horizontal", "t1_wall_logs_horizontal_2"}, false);
		SaveConstructedParts2("bbp_dgate", "T3 Bricks", {"frame", "t3_wall_concrete_bricks", "t3_wall_concrete_bricks_2"}, false);
		SaveConstructedParts2("bbp_dgate", "Frame", {"frame"}, false);
		SaveConstructedParts2("bbp_bwindow", "T1 Logs (No Window)", {"frame", "t1_wall_logs_horizontal"}, false);
		SaveConstructedParts2("bbp_bwindow", "T1 Planks (Window)", {"frame", "t1_door", "t1_wall_planks"}, false);
		SaveConstructedParts2("bbp_bwindow", "T1 Logs (Window + Bars)", {"frame", "t1_door", "t1_wall_logs_horizontal", "window_bars"}, false);
		SaveConstructedParts2("bbp_bwindow", "T3 Bricks (Window + Bars)", {"frame", "t3_door", "t3_wall_concrete_bricks_2", "window_bars"}, false);
		SaveConstructedParts2("bbp_bwindow", "T3 Bricks (No Window)", {"frame", "t3_wall_concrete_bricks_2"}, false);
		SaveConstructedParts2("bbp_bwindow", "T1 Planks (Bars)", {"frame", "t1_wall_planks", "window_bars"}, false);
		SaveConstructedParts2("bbp_bwindow", "T1 Logs (Window)", {"frame", "t1_door", "t1_wall_logs_horizontal"}, false);
		SaveConstructedParts2("bbp_bwindow", "T3 Smooth (Window + Bars)", {"frame", "t3_door", "t3_wall_concrete_bricks", "window_bars"}, false);
		SaveConstructedParts2("bbp_bwindow", "T3 Smooth (Bars)", {"frame", "t3_wall_concrete_bricks", "window_bars"}, false);
		SaveConstructedParts2("bbp_bwindow", "T3 Smooth (No Window)", {"frame", "t3_wall_concrete_bricks"}, false);
		SaveConstructedParts2("bbp_bwindow", "T1 Planks (Window + Bars)", {"frame", "t1_door", "t1_wall_logs_horizontal", "window_bars"}, false);
		SaveConstructedParts2("bbp_bwindow", "T2 Metal (Window + Bars)", {"frame", "t1_door", "t1_wall_planks", "t2_door", "t2_metal_2", "window_bars"}, false);
		SaveConstructedParts2("bbp_bwindow", "T2 Metal (Window)", {"frame", "t1_door", "t1_wall_planks", "t2_door", "t2_metal_2"}, false);
		SaveConstructedParts2("bbp_bwindow", "T2 Metal (No Window)", {"frame", "t1_wall_planks", "t2_metal_2"}, false);
		SaveConstructedParts2("bbp_bwindow", "T3 Smooth (Window)", {"frame", "t3_door", "t3_wall_concrete_bricks"}, false);
		SaveConstructedParts2("bbp_bwindow", "T1 Logs (Bars)", {"frame", "t1_wall_logs_horizontal", "window_bars"}, false);
		SaveConstructedParts2("bbp_bwindow", "T3 Bricks (Window)", {"frame", "t3_door", "t3_wall_concrete_bricks_2"}, false);
		SaveConstructedParts2("bbp_bwindow", "Frame", {"frame"}, false);
		SaveConstructedParts2("bbp_bwindow", "T1 Planks (No Window)", {"frame", "t1_wall_planks"}, false);
		SaveConstructedParts2("bbp_bwindow", "T2 Metal (Bars)", {"frame", "t1_wall_planks", "t2_metal_2", "window_bars"}, false);
		SaveConstructedParts2("bbp_bwindow", "T3 Bricks (Bars)", {"frame", "t3_wall_concrete_bricks_2", "window_bars"}, false);
		SaveConstructedParts2("bbp_right_slope_wall", "T1 Logs", {"frame", "t1_wall_logs_horizontal"}, false);
		SaveConstructedParts2("bbp_right_slope_wall", "T1 Planks", {"frame", "t1_wall_planks"}, false);
		SaveConstructedParts2("bbp_right_slope_wall", "T2 Metal", {"frame", "t1_wall_planks", "t2_metal_2"}, false);
		SaveConstructedParts2("bbp_right_slope_wall", "T3 Smooth", {"frame", "t3_wall_concrete_bricks"}, false);
		SaveConstructedParts2("bbp_right_slope_wall", "Frame", {"frame"}, false);
		SaveConstructedParts2("bbp_lgate", "T2 Metal", {"frame", "t1_wall_logs_horizontal", "t2_metal"}, false);
		SaveConstructedParts2("bbp_lgate", "T1 Wood", {"frame", "t1_wall_logs_horizontal"}, false);
		SaveConstructedParts2("bbp_lgate", "Frame", {"frame"}, false);
		SaveConstructedParts2("bbp_lgate", "T3 Metal", {"frame", "t3_wall_concrete_bricks"}, false);
		SaveConstructedParts2("bbp_spillar", "T3 Smooth", {"frame", "t3_wall_concrete_bricks"}, false);
		SaveConstructedParts2("bbp_spillar", "Frame", {"frame"}, false);
		SaveConstructedParts2("bbp_left_slope_wall", "T1 Logs", {"frame", "t1_wall_logs_horizontal"}, false);
		SaveConstructedParts2("bbp_left_slope_wall", "T1 Planks", {"frame", "t1_wall_planks"}, false);
		SaveConstructedParts2("bbp_left_slope_wall", "T2 Metal", {"frame", "t1_wall_planks", "t2_metal_2"}, false);
		SaveConstructedParts2("bbp_left_slope_wall", "T3 Smooth", {"frame", "t3_wall_concrete_bricks"}, false);
		SaveConstructedParts2("bbp_left_slope_wall", "Frame", {"frame"}, false);
		SaveConstructedParts2("bbp_bstair", "Built", {"frame"}, false);
		SaveConstructedParts2("bbp_roof_hatch", "T2 Metal", {"frame", "stair", "t1_door", "t1_wall_logs_horizontal", "t2_door", "t2_metal"}, false);
		SaveConstructedParts2("bbp_roof_hatch", "T1 Wood", {"frame", "stair", "t1_door", "t1_wall_logs_horizontal"}, false);
		SaveConstructedParts2("bbp_roof_hatch", "T3 Smooth", {"frame", "stair", "t3_door", "t3_wall_concrete_bricks"}, false);
		SaveConstructedParts2("bbp_roof_hatch", "Frame", {"frame"}, false);
		SaveConstructedParts2("bbp_mesh_gate", "T2 Doors", {"frame", "t2_door", "t2_metal"}, false);
		SaveConstructedParts2("bbp_mesh_gate", "T2 Doors + Barbed Wire", {"frame", "t2_door", "t2_metal", "t2_metal_2"}, false);
		SaveConstructedParts2("bbp_mesh_gate", "Frame", {"frame"}, false);
		SaveConstructedParts2("bbp_sgate", "T2 Metal", {"frame", "t1_wall_logs_horizontal", "t2_metal"}, false);
		SaveConstructedParts2("bbp_sgate", "T1 Wood", {"frame", "t1_wall_logs_horizontal"}, false);
		SaveConstructedParts2("bbp_sgate", "Frame", {"frame"}, false);
		SaveConstructedParts2("bbp_sgate", "T3 Metal", {"frame", "t3_wall_concrete_bricks"}, false);
		SaveConstructedParts2("bbp_triangle_bfloor", "T2 Metal", {"frame", "t1_wall_logs_horizontal", "t2_metal"}, false);
		SaveConstructedParts2("bbp_triangle_bfloor", "T1 Wood", {"frame", "t1_wall_logs_horizontal"}, false);
		SaveConstructedParts2("bbp_triangle_bfloor", "T3 Smooth", {"frame", "t3_wall_concrete_bricks"}, false);
		SaveConstructedParts2("bbp_triangle_bfloor", "Frame", {"frame"}, false);
		SaveConstructedParts2("fence", "Wood", {"base", "wall_base_down", "wall_base_up", "wall_wood_down", "wall_wood_up"}, false);
		SaveConstructedParts2("fence", "Wood Gate", {"base", "wall_base_down", "wall_base_up", "wall_gate", "wall_wood_down", "wall_wood_up"}, false);
		SaveConstructedParts2("fence", "Metal", {"base", "wall_base_down", "wall_base_up", "wall_metal_down", "wall_metal_up"}, false);
		SaveConstructedParts2("fence", "Wood Platform", {"base", "wall_base_down", "wall_base_up", "wall_platform", "wall_platform_stairs_left", "wall_platform_stairs_right", "wall_wood_down", "wall_wood_up"}, false);
		SaveConstructedParts2("fence", "Base", {"base"}, false);
		SaveConstructedParts2("fence", "Metal Gate", {"base", "wall_base_down", "wall_base_up", "wall_gate", "wall_metal_down", "wall_metal_up"}, false);
		SaveConstructedParts2("fence", "Frame", {"base", "wall_base_down", "wall_base_up"}, false);
		SaveConstructedParts2("watchtower", "Level 2 Stairs", {"level_1_base", "level_1_roof", "level_1_stairs", "level_2_base", "level_2_roof", "level_2_stairs"}, false);
		SaveConstructedParts2("watchtower", "Level 3 Roof", {"level_1_base", "level_1_roof", "level_1_stairs", "level_2_base", "level_2_roof", "level_2_stairs", "level_3_base", "level_3_roof"}, false);
		SaveConstructedParts2("watchtower", "Level 1 Wood All", {"level_1_base", "level_1_wall_1_base_down", "level_1_wall_1_base_up", "level_1_wall_1_wood_down", "level_1_wall_1_wood_up", "level_1_wall_2_base_down", "level_1_wall_2_base_up", "level_1_wall_2_wood_down", "level_1_wall_2_wood_up", "level_1_wall_3_base_down", "level_1_wall_3_base_up", "level_1_wall_3_wood_down", "level_1_wall_3_wood_up"}, false);
		SaveConstructedParts2("watchtower", "Level 1 Frame", {"level_1_base", "level_1_wall_1_base_down", "level_1_wall_1_base_up", "level_1_wall_2_base_down", "level_1_wall_2_base_up", "level_1_wall_3_base_down", "level_1_wall_3_base_up"}, false);
		SaveConstructedParts2("watchtower", "All Frames with Stairs", {"level_1_base", "level_1_roof", "level_1_stairs", "level_1_wall_1_base_down", "level_1_wall_1_base_up", "level_1_wall_2_base_down", "level_1_wall_2_base_up", "level_1_wall_3_base_down", "level_1_wall_3_base_up", "level_2_base", "level_2_roof", "level_2_stairs", "level_2_wall_1_base_down", "level_2_wall_1_base_up", "level_2_wall_2_base_down", "level_2_wall_2_base_up", "level_2_wall_3_base_down", "level_2_wall_3_base_up", "level_3_base", "level_3_roof", "level_3_wall_1_base_down", "level_3_wall_1_base_up", "level_3_wall_2_base_down", "level_3_wall_2_base_up", "level_3_wall_3_base_down", "level_3_wall_3_base_up"}, false);
		SaveConstructedParts2("watchtower", "Level 1 Stairs", {"level_1_base", "level_1_roof", "level_1_stairs"}, false);
		SaveConstructedParts2("watchtower", "Level 2 Roof", {"level_1_base", "level_1_roof", "level_1_stairs", "level_2_base", "level_2_roof"}, false);
		SaveConstructedParts2("watchtower", "All Wood", {"level_1_base", "level_1_roof", "level_1_stairs", "level_1_wall_1_base_down", "level_1_wall_1_base_up", "level_1_wall_1_wood_down", "level_1_wall_1_wood_up", "level_1_wall_2_base_down", "level_1_wall_2_base_up", "level_1_wall_2_wood_down", "level_1_wall_2_wood_up", "level_1_wall_3_base_down", "level_1_wall_3_base_up", "level_1_wall_3_wood_down", "level_1_wall_3_wood_up", "level_2_base", "level_2_roof", "level_2_stairs", "level_2_wall_1_base_down", "level_2_wall_1_base_up", "level_2_wall_1_wood_down", "level_2_wall_1_wood_up", "level_2_wall_2_base_down", "level_2_wall_2_base_up", "level_2_wall_2_wood_down", "level_2_wall_2_wood_up", "level_2_wall_3_base_down", "level_2_wall_3_base_up", "level_2_wall_3_wood_down", "level_2_wall_3_wood_up", "level_3_base", "level_3_roof", "level_3_wall_1_base_down", "level_3_wall_1_base_up", "level_3_wall_1_wood_down", "level_3_wall_1_wood_up", "level_3_wall_2_base_down", "level_3_wall_2_base_up", "level_3_wall_2_wood_down", "level_3_wall_2_wood_up", "level_3_wall_3_base_down", "level_3_wall_3_base_up", "level_3_wall_3_wood_down", "level_3_wall_3_wood_up"}, false);
		SaveConstructedParts2("watchtower", "Base", {"level_1_base"}, false);
		SaveConstructedParts2("watchtower", "All Metal", {"level_1_base", "level_1_roof", "level_1_stairs", "level_1_wall_1_base_down", "level_1_wall_1_base_up", "level_1_wall_1_metal_down", "level_1_wall_1_metal_up", "level_1_wall_2_base_down", "level_1_wall_2_base_up", "level_1_wall_2_metal_down", "level_1_wall_2_metal_up", "level_1_wall_3_base_down", "level_1_wall_3_base_up", "level_1_wall_3_metal_down", "level_1_wall_3_metal_up", "level_2_base", "level_2_roof", "level_2_stairs", "level_2_wall_1_base_down", "level_2_wall_1_base_up", "level_2_wall_1_metal_down", "level_2_wall_1_metal_up", "level_2_wall_2_base_down", "level_2_wall_2_base_up", "level_2_wall_2_metal_down", "level_2_wall_2_metal_up", "level_2_wall_3_base_down", "level_2_wall_3_base_up", "level_2_wall_3_metal_down", "level_2_wall_3_metal_up", "level_3_base", "level_3_roof", "level_3_wall_1_base_down", "level_3_wall_1_base_up", "level_3_wall_1_metal_down", "level_3_wall_1_metal_up", "level_3_wall_2_base_down", "level_3_wall_2_base_up", "level_3_wall_2_metal_down", "level_3_wall_2_metal_up", "level_3_wall_3_base_down", "level_3_wall_3_base_up", "level_3_wall_3_metal_down", "level_3_wall_3_metal_up"}, false);
		SaveConstructedParts2("watchtower", "Level 1 Roof", {"level_1_base", "level_1_roof"}, false);
		SaveConstructedParts2("bbp_bramp", "Built", {"frame"}, false);
		SaveConstructedParts2("bbp_bfloor", "T2 Metal", {"frame", "t1_wall_logs_horizontal", "t2_metal"}, false);
		SaveConstructedParts2("bbp_bfloor", "T1 Wood", {"frame", "t1_wall_logs_horizontal"}, false);
		SaveConstructedParts2("bbp_bfloor", "T3 Smooth", {"frame", "t3_wall_concrete_bricks"}, false);
		SaveConstructedParts2("bbp_bfloor", "Frame", {"frame"}, false);
		SaveConstructedParts2("bbp_rgate", "T2 Metal", {"frame", "t1_wall_logs_horizontal", "t2_metal"}, false);
		SaveConstructedParts2("bbp_rgate", "T1 Wood", {"frame", "t1_wall_logs_horizontal"}, false);
		SaveConstructedParts2("bbp_rgate", "Frame", {"frame"}, false);
		SaveConstructedParts2("bbp_rgate", "T3 Metal", {"frame", "t3_wall_concrete_bricks"}, false);
		SaveConstructedParts2("bbp_bpillar", "T3 Smooth", {"frame", "t3_wall_concrete_bricks"}, false);
		SaveConstructedParts2("bbp_bpillar", "Frame", {"frame"}, false);
		SaveConstructedParts2("bbp_mesh_fence", "T2 Metal", {"frame", "t2_metal"}, false);
		SaveConstructedParts2("bbp_mesh_fence", "T2 Metal + Barbed Wire", {"frame", "t2_metal", "t2_metal_2"}, false);
		SaveConstructedParts2("bbp_mesh_fence", "Frame", {"frame"}, false);
		SaveConstructedParts2("bbp_bdoor", "T3 Bricks", {"frame", "t3_door", "t3_wall_concrete_bricks_2"}, false);
		SaveConstructedParts2("bbp_bdoor", "T1 Logs", {"frame", "t1_door", "t1_wall_logs_horizontal"}, false);
		SaveConstructedParts2("bbp_bdoor", "T1 Planks", {"frame", "t1_door", "t1_wall_planks"}, false);
		SaveConstructedParts2("bbp_bdoor", "T2 Metal", {"frame", "t1_door", "t1_wall_planks", "t2_door", "t2_metal_2"}, false);
		SaveConstructedParts2("bbp_bdoor", "T3 Smooth", {"frame", "t3_door", "t3_wall_concrete_bricks"}, false);
		SaveConstructedParts2("bbp_bdoor", "Frame", {"frame"}, false);
		SaveConstructedParts2("bbp_sdoor", "T1 Logs", {"frame", "t1_door", "t1_wall_logs_horizontal"}, false);
		SaveConstructedParts2("bbp_sdoor", "T1 Planks", {"frame", "t1_door", "t1_wall_planks"}, false);
		SaveConstructedParts2("bbp_sdoor", "T2 Metal", {"frame", "t1_door", "t1_wall_planks", "t2_door", "t2_metal_2"}, false);
		SaveConstructedParts2("bbp_sdoor", "T3 Smooth", {"frame", "t3_door", "t3_wall_concrete_bricks"}, false);
		SaveConstructedParts2("bbp_sdoor", "Frame", {"frame"}, false);
		SaveConstructedParts2("bbp_metal_stair", "Railing Right", {"frame", "railing_2"}, false);
		SaveConstructedParts2("bbp_metal_stair", "Railing Left", {"frame", "railing_1"}, false);
		SaveConstructedParts2("bbp_metal_stair", "Railing Both", {"frame", "railing_1", "railing_2"}, false);
		SaveConstructedParts2("bbp_metal_stair", "Base", {"frame"}, false);
		SaveConstructedParts2("bbp_half_bwall", "T3 Bricks", {"frame", "t3_wall_concrete_bricks_2"}, false);
		SaveConstructedParts2("bbp_half_bwall", "T1 Logs", {"frame", "t1_wall_logs_horizontal"}, false);
		SaveConstructedParts2("bbp_half_bwall", "T1 Planks", {"frame", "t1_wall_planks"}, false);
		SaveConstructedParts2("bbp_half_bwall", "T2 Metal", {"frame", "t1_wall_planks", "t2_metal_2"}, false);
		SaveConstructedParts2("bbp_half_bwall", "T3 Smooth", {"frame", "t3_wall_concrete_bricks"}, false);
		SaveConstructedParts2("bbp_half_bwall", "Frame", {"frame"}, false);
		SaveConstructedParts2("bbp_half_swall", "T1 Logs", {"frame", "t1_wall_logs_horizontal"}, false);
		SaveConstructedParts2("bbp_half_swall", "T1 Planks", {"frame", "t1_wall_planks"}, false);
		SaveConstructedParts2("bbp_half_swall", "T2 Metal", {"frame", "t1_wall_planks", "t2_metal_2"}, false);
		SaveConstructedParts2("bbp_half_swall", "T3 Smooth", {"frame", "t3_wall_concrete_bricks"}, false);
		SaveConstructedParts2("bbp_half_swall", "Frame", {"frame"}, false);
		SaveConstructedParts2("bbp_sstair", "Built", {"frame"}, false);
		SaveConstructedParts2("bbp_broof", "T2 Metal", {"frame", "t1_wall_logs_horizontal", "t2_metal"}, false);
		SaveConstructedParts2("bbp_broof", "T1 Wood", {"frame", "t1_wall_logs_horizontal"}, false);
		SaveConstructedParts2("bbp_broof", "T3 Smooth", {"frame", "t3_wall_concrete_bricks"}, false);
		SaveConstructedParts2("bbp_broof", "Frame", {"frame"}, false);
		SaveConstructedParts2("bbp_sroof", "T2 Metal", {"frame", "t1_wall_logs_horizontal", "t2_metal"}, false);
		SaveConstructedParts2("bbp_sroof", "T1 Wood", {"frame", "t1_wall_logs_horizontal"}, false);
		SaveConstructedParts2("bbp_sroof", "T3 Smooth", {"frame", "t3_wall_concrete_bricks"}, false);
		SaveConstructedParts2("bbp_sroof", "Frame", {"frame"}, false);
		SaveConstructedParts2("bbp_floor_hatch", "T2 Metal", {"frame", "stair", "t1_door", "t1_wall_logs_horizontal", "t2_door", "t2_metal"}, false);
		SaveConstructedParts2("bbp_floor_hatch", "T1 Wood", {"frame", "stair", "t1_door", "t1_wall_logs_horizontal"}, false);
		SaveConstructedParts2("bbp_floor_hatch", "T3 Smooth", {"frame", "stair", "t3_door", "t3_wall_concrete_bricks"}, false);
		SaveConstructedParts2("bbp_floor_hatch", "Frame", {"frame"}, false);
		SaveConstructedParts2("bbp_rhanger_roof", "T2 Metal", {"frame", "t1_wall_logs_horizontal", "t2_metal"}, false);
		SaveConstructedParts2("bbp_rhanger_roof", "T1 Wood", {"frame", "t1_wall_logs_horizontal"}, false);
		SaveConstructedParts2("bbp_rhanger_roof", "Frame", {"frame"}, false);
		SaveConstructedParts2("bbp_rhanger_roof", "T3 Metal", {"frame", "t3_wall_concrete_bricks"}, false);
		SaveConstructedParts2("bbp_bwall", "T3 Bricks", {"frame", "t3_wall_concrete_bricks_2"}, false);
		SaveConstructedParts2("bbp_bwall", "T1 Logs", {"frame", "t1_wall_logs_horizontal"}, false);
		SaveConstructedParts2("bbp_bwall", "T1 Planks", {"frame", "t1_wall_planks"}, false);
		SaveConstructedParts2("bbp_bwall", "T2 Metal", {"frame", "t1_wall_planks", "t2_metal_2"}, false);
		SaveConstructedParts2("bbp_bwall", "T3 Smooth", {"frame", "t3_wall_concrete_bricks"}, false);
		SaveConstructedParts2("bbp_bwall", "Frame", {"frame"}, false);
		SaveConstructedParts2("bbp_swall", "T1 Logs", {"frame", "t1_wall_logs_horizontal"}, false);
		SaveConstructedParts2("bbp_swall", "T1 Planks", {"frame", "t1_wall_planks"}, false);
		SaveConstructedParts2("bbp_swall", "T2 Metal", {"frame", "t1_wall_planks", "t2_metal_2"}, false);
		SaveConstructedParts2("bbp_swall", "T3 Smooth", {"frame", "t3_wall_concrete_bricks"}, false);
		SaveConstructedParts2("bbp_swall", "Frame", {"frame"}, false);
		SaveConstructedParts2("bbp_sfloor", "T2 Metal", {"frame", "t1_wall_logs_horizontal", "t2_metal"}, false);
		SaveConstructedParts2("bbp_sfloor", "T1 Wood", {"frame", "t1_wall_logs_horizontal"}, false);
		SaveConstructedParts2("bbp_sfloor", "T3 Smooth", {"frame", "t3_wall_concrete_bricks"}, false);
		SaveConstructedParts2("bbp_sfloor", "Frame", {"frame"}, false);
		SaveConstructedParts2("bbp_triangle_broof", "T2 Metal", {"frame", "t1_wall_logs_horizontal", "t2_metal"}, false);
		SaveConstructedParts2("bbp_triangle_broof", "T1 Wood", {"frame", "t1_wall_logs_horizontal"}, false);
		SaveConstructedParts2("bbp_triangle_broof", "T3 Smooth", {"frame", "t3_wall_concrete_bricks"}, false);
		SaveConstructedParts2("bbp_triangle_broof", "Frame", {"frame"}, false);
		SaveConstructedParts2("bbp_bfoundation", "T2 Metal", {"frame", "t1_wall_logs_horizontal", "t2_metal"}, false);
		SaveConstructedParts2("bbp_bfoundation", "T1 Wood", {"frame", "t1_wall_logs_horizontal"}, false);
		SaveConstructedParts2("bbp_bfoundation", "T3 Smooth", {"frame", "t3_wall_concrete_bricks"}, false);
		SaveConstructedParts2("bbp_bfoundation", "Frame", {"frame"}, false);
		SaveConstructedParts2("bbp_swindow", "T1 Logs (No Window)", {"frame", "t1_wall_logs_horizontal"}, false);
		SaveConstructedParts2("bbp_swindow", "T1 Planks (Window)", {"frame", "t1_door", "t1_wall_planks"}, false);
		SaveConstructedParts2("bbp_swindow", "T3 Smooth (No Window)", {"frame", "t3_wall_concrete_bricks"}, false);
		SaveConstructedParts2("bbp_swindow", "T1 Logs (Window)", {"frame", "t1_door", "t1_wall_logs_horizontal"}, false);
		SaveConstructedParts2("bbp_swindow", "T3 Smooth (Window + Bars)", {"frame", "t3_door", "t3_wall_concrete_bricks", "window_bars"}, false);
		SaveConstructedParts2("bbp_swindow", "T3 Smooth (Bars)", {"frame", "t3_wall_concrete_bricks", "window_bars"}, false);
		SaveConstructedParts2("bbp_swindow", "T1 Planks (Bars)", {"frame", "t1_wall_planks", "window_bars"}, false);
		SaveConstructedParts2("bbp_swindow", "T1 Planks (Window + Bars)", {"frame", "t1_door", "t1_wall_logs_horizontal", "window_bars"}, false);
		SaveConstructedParts2("bbp_swindow", "T2 Metal (Window + Bars)", {"frame", "t1_door", "t1_wall_planks", "t2_door", "t2_metal_2", "window_bars"}, false);
		SaveConstructedParts2("bbp_swindow", "T2 Metal (Window)", {"frame", "t1_door", "t1_wall_planks", "t2_door", "t2_metal_2"}, false);
		SaveConstructedParts2("bbp_swindow", "T2 Metal (No Window)", {"frame", "t1_wall_planks", "t2_metal_2"}, false);
		SaveConstructedParts2("bbp_swindow", "T3 Smooth (Window)", {"frame", "t3_door", "t3_wall_concrete_bricks"}, false);
		SaveConstructedParts2("bbp_swindow", "T1 Logs (Bars)", {"frame", "t1_wall_logs_horizontal", "window_bars"}, false);
		SaveConstructedParts2("bbp_swindow", "T1 Logs (Window + Bars)", {"frame", "t1_door", "t1_wall_logs_horizontal", "window_bars"}, false);
		SaveConstructedParts2("bbp_swindow", "Frame", {"frame"}, false);
		SaveConstructedParts2("bbp_swindow", "T1 Planks (No Window)", {"frame", "t1_wall_planks"}, false);
		SaveConstructedParts2("bbp_swindow", "T2 Metal (Bars)", {"frame", "t1_wall_planks", "t2_metal_2", "window_bars"}, false);
		SaveConstructedParts2("bbp_lhanger_roof", "T2 Metal", {"frame", "t1_wall_logs_horizontal", "t2_metal"}, false);
		SaveConstructedParts2("bbp_lhanger_roof", "T1 Wood", {"frame", "t1_wall_logs_horizontal"}, false);
		SaveConstructedParts2("bbp_lhanger_roof", "Frame", {"frame"}, false);
		SaveConstructedParts2("bbp_lhanger_roof", "T3 Metal", {"frame", "t3_wall_concrete_bricks"}, false);
		SaveConstructedParts2("bbp_triangle_foundation", "T2 Metal", {"frame", "t1_wall_logs_horizontal", "t2_metal"}, false);
		SaveConstructedParts2("bbp_triangle_foundation", "T1 Logs", {"frame", "t1_wall_logs_horizontal"}, false);
		SaveConstructedParts2("bbp_triangle_foundation", "T3 Smooth", {"frame", "t3_wall_concrete_bricks"}, false);
		SaveConstructedParts2("bbp_triangle_foundation", "Frame", {"frame"}, false);
		SaveConstructedParts2("bbp_slope_roof", "T2 Metal", {"frame", "t1_wall_logs_horizontal", "t2_metal"}, false);
		SaveConstructedParts2("bbp_slope_roof", "T1 Wood", {"frame", "t1_wall_logs_horizontal"}, false);
		SaveConstructedParts2("bbp_slope_roof", "T3 Smooth", {"frame", "t3_wall_concrete_bricks"}, false);
		SaveConstructedParts2("bbp_slope_roof", "Frame", {"frame"}, false);
	}

	void SaveConstructedParts2(string itemname, string presetname, TStringArray parts, bool save) {
		TStringSet s = LBArrayTools<string>.ToSet(parts);
		SaveConstructedParts(itemname, presetname, s, save);
	}

	bool IsContructionPresetExisting(string itemname, string presetname) {
		if (!constructionEditorConfig)
			return false;
		itemname = LBStringTools.ToLowerString(itemname);
		map<string, ref TStringSet> presets = constructionEditorConfig.Get(itemname);
		if (!presets)
			return false;
		return presets.Contains(presetname);
	}

	void SaveConstructedParts(string itemname, string presetname, TStringSet parts, bool save = true) {
		if (!constructionEditorConfig)
			constructionEditorConfig = new map<string, ref map<string, ref TStringSet>>();
		itemname = LBStringTools.ToLowerString(itemname);
		map<string, ref TStringSet> presets = constructionEditorConfig.Get(itemname);
		if (!presets) {
			presets = new map<string, ref TStringSet>();
			constructionEditorConfig.Insert(itemname, presets);
		}
		presets.Set(presetname, parts);
		LBAdminMenuSettings.Loader.Save();
	}

	TStringSet GetAvailablePresets(string itemname) {
		if (!constructionEditorConfig)
			constructionEditorConfig = new map<string, ref map<string, ref TStringSet>>();
		itemname = LBStringTools.ToLowerString(itemname);
		map<string, ref TStringSet> presets = constructionEditorConfig.Get(itemname);
		if (!presets)
			return new TStringSet();
		TStringSet keys = new TStringSet();
		foreach (string name, TStringSet parts : presets) {
			keys.Insert(name);
		}
		return keys;
	}

	void DeletePreset(string itemname, string presetname) {
		if (!constructionEditorConfig)
			return;
		itemname = LBStringTools.ToLowerString(itemname);
		map<string, ref TStringSet> presets = constructionEditorConfig.Get(itemname);
		if (!presets)
			return;
		presets.Remove(presetname);
		LBAdminMenuSettings.Loader.Save();
	}

	TStringSet GetConstructedParts(string itemname, string presetname) {
		if (!constructionEditorConfig)
			return null;
		itemname = LBStringTools.ToLowerString(itemname);
		map<string, ref TStringSet> presets = constructionEditorConfig.Get(itemname);
		if (!presets)
			return null;
		return presets.Get(presetname);
	}

	int GetFrameColor() {
		return frameColor.GetColorARGB();
	}

	override bool OnLoad() {
		foreach (LBESPFilter filter : espFilters) {
			filter.UpdateChildren();
		}
		return super.OnLoad();
	}

	int GetItemColor(EntityAI ent) {
		if (!ent)
			return -1;
		Man pb = Man.Cast(ent);
		if (pb && pb.GetIdentity()) {
			string steamid = pb.GetIdentity().GetPlainId();
			if (highlightedPlayers.Find(steamid) != -1) {
				return highlightedPlayerColor;
			}
			foreach (LBAdminPlayerTeam team : LBAdminPlayerTeamsList.Get.teams) {
				if (team.overwriteESPColor && team.playerSteamids.Find(steamid) != -1) {
					return team.espColor.GetColorARGB();
				}
			}
		}
		string typeLower = LBStringTools.ToLowerString(ent.GetType());
		foreach (LBESPFilter filter : espFilters) {
			if (filter.IsInFilter(typeLower))
				return filter.color.GetColorARGB();
		}
		return -1;
	}

	TStringSet GetESPFilter() {
		TStringSet items = new TStringSet();
		foreach (LBESPFilter filter : espFilters) {
			if (filter.active)
				items.InsertSet(filter.children);
		}
		return items;
	}

}
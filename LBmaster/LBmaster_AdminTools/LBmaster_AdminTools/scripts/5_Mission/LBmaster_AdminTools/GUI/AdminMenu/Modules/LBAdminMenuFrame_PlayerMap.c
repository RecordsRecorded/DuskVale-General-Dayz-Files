class LBAdminMenuFrame_PlayerMap : LBAdminMenuFrame {

	override string GetTitle() {
		return "#lb_adm_title_map";
	}

	override string GetButtonIcon() {
		return "LBmaster_AdminTools/gui/icons/map.paa";
	}

	override void GetMinSize(out int width, out int height) {
		width = 300;
		height = 20;
	}

	override void GetInitialSize(out int width, out int height) {
		width = 800;
		height = 500;
	}

	override TStringArray GetRequiredPermissionsOne() {
		return {"admin.teleport.map", "admin.view.playerpositions", "admin.view.vehiclepositions"};
	}

	override string GetInfoString() {
		return "#lb_adm_desc_map";
	}

	TextWidget txt_info;
	CheckBoxWidget chckbx_players, chckbx_vehicles;
	MapWidget mapWidget;

	PlayerIdentity selectedPlayer;
	ref array<ref Param5<PlayerIdentity, vector, string, int, bool>> players = new array<ref Param5<PlayerIdentity, vector, string, int, bool >> (); // Identity, Position, VehicleClassname, vehicleID, driver
	ref array<ref Param3<int, float, float>> vehicles = new array<ref Param3<int, float, float >> ();
	int playerCount, vehicleCount;

	override void UpdateSlow() {
		if (IsVisible())
			RequestPositions();
	}

	void RequestPositions() {
		g_Game.RPCSingleParam(null, LBAdminToolRPCs.PLAYER_POSITIONS, new Param2<bool, bool>(chckbx_players.IsChecked(), chckbx_vehicles.IsChecked()), true);
	}

	override void Show(bool show) {
		super.Show(show);
		if (show) {
			chckbx_players.SetChecked(LBAdminMenuSettings.Get.mapShowPlayers);
			chckbx_vehicles.SetChecked(LBAdminMenuSettings.Get.mapShowVehicles);
			chckbx_players.Show(LBAdmins.Get().HasPermission("admin.view.playerpositions"));
			chckbx_vehicles.Show(LBAdmins.Get().HasPermission("admin.view.vehiclepositions"));
		}
	}

	override void Update(float timeslice, bool otherMenu, bool leftClicked) {
		super.Update(timeslice, otherMenu, leftClicked);
		if (IsVisible()) {
			vector worldPos = mapWidget.ScreenToMap(Vector(mouseX, mouseY, 0));
			worldPos[1] = g_Game.SurfaceY(worldPos[0], worldPos[2]);
			if (parent.IsVisible() && !LBFreecamCamera.instance)
				UpdateMapInfo(worldPos, false);
			else if (LBFreecamCamera.instance)
				UpdateMapInfo(LBFreecamCamera.position, false);
			else if (!selectedPlayer)
				UpdateMapInfo(worldPos, true);

			if (hasPrio && (g_Game.LBIsKeyDown(KeyCode.KC_LCONTROL) || g_Game.LBIsKeyPressed(KeyCode.KC_RCONTROL)) && g_Game.LBIsKeyPressed(KeyCode.KC_C)) {
				string copystr = worldPos.ToString();
				g_Game.CopyToClipboard(copystr);
				LBCopiedPopup.ShowAtCursor();
			}
			if (LBFreecamCamera.instance)
				UpdateMarkers();
		}
	}

	void UpdateMapInfo(vector pos, bool myPlayer) {
		vector playerPos = vector.Zero;
		if (g_Game.GetPlayer())
			playerPos = g_Game.GetPlayer().GetPosition();
		if (myPlayer) {
			txt_info.SetText("" + playerPos.ToString(false));
		} else {
			float distance3d = vector.Distance(playerPos, pos);
			float distance2d = vector.Distance(Vector(playerPos[0], 0, playerPos[2]), Vector(pos[0], 0, pos[2]));
			txt_info.SetText("" + pos.ToString(false) + LBTranslatedString("    #lb_adm_desc_map_distance", {"dist_3d%", "" + distance3d, "%dist_2d%", "" + distance2d}).Get());
		}
	}

	override void OnInit() {
		LBAdminMenuFrame_PlayerList list = LBAdminMenuFrame_PlayerList.Cast(parent.GetFrame("LBAdminMenuFrame_PlayerList"));
		list.Event_Player_Selected.Insert(OnPlayerSelected);
	}

	override void OnRPC(Object target, int rpc_type, ParamsReadContext ctx) {
		if (rpc_type == LBAdminToolRPCs.PLAYER_POSITIONS) {
			mapWidget.ClearUserMarks();
			if (!ctx.Read(playerCount) || !ctx.Read(vehicleCount))
				return;
			if (!ctx.Read(players) || !ctx.Read(vehicles)) {
				return;
			}
			UpdateMarkers();
		}
	}

	void UpdateMarkers() {
		mapWidget.ClearUserMarks();
		AddVehicleMarkers();
		ZoomToPlayer(selectedPlayer, !IsParentVisible(), false);
		AddTeleportMarkers();
		AddFreecamMarker();
		AddObjectBuilderMarker();
		AddPlayerMarkers();
		chckbx_players.SetText("#lb_adm_player (" + playerCount + ")");
		chckbx_vehicles.SetText("#lb_adm_vehicles (" + vehicleCount + ")");
	}

	void AddVehicleMarkers() {
		foreach (Param3<int, float, float> vehicle : vehicles) {
			string classname = LBInherit.Get().HashToItemname(vehicle.param1);
			if (classname == "")
				classname = "" + vehicle.param1;
			string displayname = LBItemHelper.GetDisplayName(classname);
			if (displayname == "")
				displayname = classname;
			int vehicleType = GetVehicleType(classname);
			if (vehicleType == 0)
				mapWidget.AddUserMark(Vector(vehicle.param2, 0, vehicle.param3), displayname, LBAdminMenuSettings.Get.mapVehicleColor, "LBmaster_AdminTools\\gui\\icons\\car.paa");
			else if (vehicleType == 1)
				mapWidget.AddUserMark(Vector(vehicle.param2, 0, vehicle.param3), displayname, LBAdminMenuSettings.Get.mapBoatColor, "LBmaster_AdminTools\\gui\\icons\\ship.paa");
			else if (vehicleType == 2)
				mapWidget.AddUserMark(Vector(vehicle.param2, 0, vehicle.param3), displayname, LBAdminMenuSettings.Get.mapHeliColor, "LBmaster_AdminTools\\gui\\icons\\heli.paa");
		}
	}

	void AddObjectBuilderMarker() {
		LBAdminMenuFrame_ObjectBuilder objectBuilder = LBAdminMenuFrame_ObjectBuilder.Cast(parent.GetFrame("LBAdminMenuFrame_ObjectBuilder"));
		if (objectBuilder && objectBuilder.visible && objectBuilder.currentBuildingSet) {
			foreach (LBObjectBuilderObject obj : objectBuilder.currentBuildingSet.objects) {
				mapWidget.AddUserMark(obj.position, obj.classname, ARGB(255, 255, 255, 0), "LBmaster_AdminTools\\gui\\icons\\marker.paa");
			}
		}
	}

	void AddFreecamMarker() {
		if (LBFreecamCamera.instance) {
			mapWidget.AddUserMark(LBFreecamCamera.position, Widget.TranslateString("#lb_adm_text_freecam"), ARGB(255, 255, 0, 255), "LBmaster_AdminTools\\gui\\icons\\camera.paa");
		}
	}

	void AddTeleportMarkers() {
		LBAdminMenuFrame_TeleportPos positionsFrame = LBAdminMenuFrame_TeleportPos.Cast(parent.GetFrame("LBAdminMenuFrame_TeleportPos"));
		if (positionsFrame && positionsFrame.visible) {
			LBPositionsListBase config = positionsFrame.GetSelectedPositions();
			if (config && config.GetPositions()) {
				array<ref LB_TeleportPos> positions = config.GetPositions();
				foreach (LB_TeleportPos pos2 : positions) {
					mapWidget.AddUserMark(pos2.pos, pos2.name, ARGB(255, 255, 0, 0), "LBmaster_AdminTools\\gui\\icons\\marker.paa");
				}
			}
		}
	}

	void AddPlayerMarkers() {
		map<int, ref array<int>> vehiclePlayers = new map<int, ref array<int >>;
		for (int i = 0; i < players.Count(); ++i) {
			Param5<PlayerIdentity, vector, string, int, bool> player = players.Get(i);
			if (player.param3 == "") {
				if (player.param1)
					mapWidget.AddUserMark(player.param2, player.param1.GetName(), LBAdminMenuSettings.Get.mapPlayerColor, "LBmaster_AdminTools\\gui\\icons\\player.paa");
			} else {
				array<int> vehPlayers;
				if (!vehiclePlayers.Find(player.param4, vehPlayers)) {
					vehPlayers = new TIntArray();
					vehiclePlayers.Insert(player.param4, vehPlayers);
				}
				vehPlayers.Insert(i);
			}
		}
		foreach (int index, array<int> playersIndices : vehiclePlayers) {
			int driver = -1;
			string str = LBItemHelper.GetDisplayName(players.Get(playersIndices.Get(0)).param3) + ": ";
			int vehicleType = GetVehicleType(players.Get(playersIndices.Get(0)).param3);
			foreach (int playerIndex : playersIndices) {
				if (players.Get(playerIndex).param5) {
					driver = playerIndex;
					playersIndices.RemoveItem(playerIndex);
					break;
				}
			}
			vector pos;
			if (driver != -1) {
				if (vehicleType == 0)
					str = str + players.Get(driver).param1.GetName() + " (#lb_adm_driver)";
				else if (vehicleType == 1)
					str = str + players.Get(driver).param1.GetName() + " (#lb_adm_captain)";
				else if (vehicleType == 2)
					str = str + players.Get(driver).param1.GetName() + " (#lb_adm_pilot)";
				pos = players.Get(driver).param2;
			} else {
				if (vehicleType == 0)
					str = str + " (#lb_adm_no_driver)";
				else if (vehicleType == 1)
					str = str + " (#lb_adm_no_captain)";
				else if (vehicleType == 2)
					str = str + " (#lb_adm_no_pilot)";
			}
			foreach (int playerIndex2 : playersIndices) {
				pos = players.Get(playerIndex2).param2;
				str = str + ", " + players.Get(playerIndex2).param1.GetName();
			}
			str = Widget.TranslateString(str);
			if (vehicleType == 0)
				mapWidget.AddUserMark(pos, str, LBAdminMenuSettings.Get.mapVehicleColor, "LBmaster_AdminTools\\gui\\icons\\car.paa");
			else if (vehicleType == 1)
				mapWidget.AddUserMark(pos, str, LBAdminMenuSettings.Get.mapBoatColor, "LBmaster_AdminTools\\gui\\icons\\ship.paa");
			else if (vehicleType == 2)
				mapWidget.AddUserMark(pos, str, LBAdminMenuSettings.Get.mapHeliColor, "LBmaster_AdminTools\\gui\\icons\\heli.paa");
		}
	}

	void OnPlayerSelected(PlayerIdentity selected) {
		ZoomToPlayer(selected);
	}

	void ZoomToPlayer(PlayerIdentity selected, bool move = true, bool scale = true) {
		this.selectedPlayer = selected;
		if (selected) {
			foreach (Param5<PlayerIdentity, vector, string, int, bool> player : players) {
				if (player && player.param1 && player.param1.GetPlainId() == selected.GetPlainId()) {
					ZoomToPos(player.param2, move, scale);
					return;
				}
			}
		}
	}

	void ZoomToPos(vector pos, bool move = true, bool scale = true) {
		if (scale)
			mapWidget.SetScale(0.05);
		if (move)
			mapWidget.SetMapPos(pos);
		if (!parent.IsVisible())
			UpdateMapInfo(pos, false);
	}

	override bool OnDoubleClick(Widget w, int x, int y, int button) {
		if (super.OnDoubleClick(w, x, y, button))
			return true;
		if (w == mapWidget) {
			vector mousePos = Vector(x, y, 0);
			vector mapPos = mapWidget.ScreenToMap(mousePos);
			mapPos[1] = g_Game.SurfaceY(mapPos[0], mapPos[2]);
			if (LBFreecamCamera.instance) {
				LBFreecamCamera.position = mapPos + "0 1 0";
				return true;
			}
			g_Game.RPCSingleParam(null, LBAdminToolRPCs.TP_MAP, new Param2<vector, vector>(mapPos, vector.Zero), true);
			return true;
		}
		return false;
	}

	override bool OnClick(Widget w, int x, int y, int button) {
		if (super.OnClick(w, x, y, button))
			return true;
		if (w == chckbx_players) {
			LBAdminMenuSettings.Get.mapShowPlayers = chckbx_players.IsChecked();
			LBAdminMenuSettings.Loader.Save();
			RequestPositions();
			return true;
		} else if (w == chckbx_vehicles) {
			LBAdminMenuSettings.Get.mapShowVehicles = chckbx_vehicles.IsChecked();
			LBAdminMenuSettings.Loader.Save();
			RequestPositions();
			return true;
		}
		return false;
	}

	int GetVehicleType(string itemname) {
		string lower = LBStringTools.ToLowerString(itemname);
		if (lower.IndexOf("heli") != -1)
			return 2;
		if (lower.IndexOf("boat") != -1)
			return 1;
		return 0;
	}
}

class LBMarker {

	static ref array<LBMarker> allMarkers = new array<LBMarker>();
	static bool hideAllMarkers = false, updateFOWRequired = true;
	static vector cameraPos;

	LBMarkerType type; // Type of the Marker. 0 = Static Marker (persistent), 1 = Dynamic Marker (deleted after server restart), 2 = Ping Marker, 3 = Group Marker, 4 = Group Player Marker, 5 = Private Marker
	int uid; // Unique ID of the Marker. This should be unique, otherwise it will only be displayed once in the Marker list. If you have a lot of the same markers like extracts, you can give it the same UID for all Markers, so you only have it once in the Marker list. Otherwise do not use the same UID for multiple markers
	string name; // Name displayed on the map
	string icon; // Icon Path for this marker
	vector position = vector.Zero; // Position of the Marker
	int currentSubgroup; // Only relevant for Group Player Markers (Type 4) to keep track of which subgroup the player joined. Also used for pings to check if they should be displayed
	int colorA = 255; // Alpha (Transparency) Color of this marker and the icon. 255 for fully opaque and 0 for fully transparent
	int colorR = 255; // Red Color
	int colorG = 255; // Green Color
	int colorB = 255; // Blue Color
	string creatorSteamID = ""; // Steamid of the user created this marker
	float circleRadius = 0; // If set to something greater than 0, this marker will have a radius marked around it
	int circleColorA = 255; // Alpha Color of the circle
	int circleColorR = 255; // Red Color of the circle
	int circleColorG = 255; // Green Color of the circle
	int circleColorB = 255; // Blue Color of the circle
	bool circleStriked = false; // Set to 1, the circle will have lines to fill the circle. When set to 0, only the border of the circle will be shown
	int circleLayer = -1; // Multiple Circles sharing a single layer will intersect with eachother instead of overlap
	bool showAllPlayerNametags; // When set to 1, playernames above the players will be shown within the circle radius. If you only want to see the playernames and don't want to show the radius, set the Alpha color to 0

	[NonSerialized()]
	bool overwriteMaxDistance = false;
	[NonSerialized()]
	float overwrittenMaxDistance = -1;
	[NonSerialized()]
	static bool streamerMode;
	[NonSerialized()]
	bool visibleOnScreen = false;
	[NonSerialized()]
	Widget mainWidget;
	[NonSerialized()]
	Widget bottomWidget;
	[NonSerialized()]
	ImageWidget iconWidget;
	[NonSerialized()]
	ImageWidget iconExtraWidget;
	[NonSerialized()]
	TextWidget nameWidget;
	[NonSerialized()]
	TextWidget distanceWidget;
	[NonSerialized()]
	LBGroup parentGroup;
	[NonSerialized()]
	ref MarkerConfigEntry cachedMarkerConfig = null;
	[NonSerialized()]
	float dist;
	[NonSerialized()]
	bool showMap = true;
	[NonSerialized()]
	bool showGPS = true;
	[NonSerialized()]
	bool show3D = true;
	[NonSerialized()]
	bool disable3dDifferentSubgroup = false;
	[NonSerialized()]
	int state = 0;
	[NonSerialized()]
	int lastcolor = 0;
	[NonSerialized()]
	bool hidden = false;
	[NonSerialized()]
	bool inFogOfWar = false;

	[NonSerialized()]
	Widget compassWidget;
	[NonSerialized()]
	ImageWidget compassIconWidget;
	[NonSerialized()]
	TextWidget compassNameWidget;
	[NonSerialized()]
	bool createdCompassWidget = false;
	[NonSerialized()]
	string lastIcon = "";
	[NonSerialized()]
	bool showBottom = true;
	[NonSerialized()]
	float widthMainWidget;
	[NonSerialized()]
	float heightMainWidget;
	[NonSerialized()]
	LBMarkerVisibilityEntry visibilityEntry;

	[NonSerialized()]
	static bool compassInit = false;
	[NonSerialized()]
	static float currentCameraAngle = 0.0;
	[NonSerialized()]
	static Widget compassWidgetGlobal;

	static void InitCompassWidgets() {
		compassInit = true;
		if (allMarkers) {
			foreach (LBMarker marker : allMarkers) {
				if (marker) {
					marker.InitCompassWidget();
				}
			}
		}
	}

	static void UpdateAllMarkers() {
		if (allMarkers) {
			cameraPos = g_Game.GetCurrentCameraPosition();
			bool alive = LBPlayerUtils.IsClientPlayerAlive();
			foreach (LBMarker marker : allMarkers) {
				if (marker) {
					if (marker.show3D && alive && marker.UpdateMarkerClient() && !hideAllMarkers) {
						marker.SetWidgetPosition();
					} else {
						marker.SetVisibleOnScreen(false);
						if (marker.compassWidget)
							marker.compassWidget.Show(false);
					}
				}
			}
		}
	}

	static void UpdateAllMarkersSlow() {
		streamerMode = LBLayoutConfig.Get.streamerModeEnabled;
		if (allMarkers) {
			//LBLogger.Debug("UpdateAllMarkersSlow " + allMarkers.Count());
			foreach (LBMarker marker : allMarkers) {
				if (marker) {
					marker.UpdateMarkerSlow();
				}
			}
		}
		updateFOWRequired = false;
	}

	LBMarkerVisibilityEntry GetVisibilityEntry() {
		if (!visibilityEntry) {
			visibilityEntry = LBMarkerVisibilityManager.Get.GetVisibilityOrAdd(this.uid);
		}
		return visibilityEntry;
	}

	void SetupMarker(LBMarkerType type_, string name_, string icon_, vector pos_) {
		this.type = type_;
		this.name = name_;
		this.icon = icon_;
		this.position = pos_;
		this.uid = Math.RandomInt(200, int.MAX - 1);
	}

	void InitMarker() {
		if (GetMarkerConfig() && !GetMarkerConfig().display3d)
			return;
		if (mainWidget || g_Game.IsServer())
			return;
		mainWidget = LBLayoutManager.Get().CreateLayout("3DMarker");
		if (mainWidget) {
			iconWidget = ImageWidget.Cast(mainWidget.FindAnyWidget("icon"));
			iconExtraWidget = ImageWidget.Cast(mainWidget.FindAnyWidget("icon_alt"));
			nameWidget = TextWidget.Cast(mainWidget.FindAnyWidget("name"));
			distanceWidget = TextWidget.Cast(mainWidget.FindAnyWidget("distance"));
			bottomWidget = mainWidget.FindAnyWidget("bottom");
			showBottom = true;

			if (GetIcon().Length() > 0) {
				LBLogger.Debug("Loading Image: " + GetIcon(), "AdvancedGroups");
				iconWidget.LoadImageFile(0, GetIcon());
			} else {
				iconWidget.Show(false);
			}
			if (type != LBMarkerType.GROUP_PING) {
				nameWidget.SetText(name);
			} else {
				nameWidget.SetText("");
				nameWidget.Show(false);
				float pingSize = LBMarkerVisibilityManager.Get.pingSize;
				iconWidget.SetSize(pingSize, pingSize);
			}
			mainWidget.Update();
			SetDistance();

			SetColor(true);
		}
		SetVisibleOnScreen(false);
		string printname = name + "";
		printname.Replace("%", "");
		LBLogger.Debug("Init Marker " + printname + ". Created Layout: " + (mainWidget != null), "AdvancedGroups");
		UpdateMarkerSlow();
	}

	void InitCompassWidget() {
		if (createdCompassWidget || !GetMarkerConfig())
			return;
		createdCompassWidget = true;
		if (!GetMarkerConfig().displayCompass)
			return;
		if (compassWidget || g_Game.IsServer())
			return;
		if (!compassInit)
			return;
		compassWidget = LBLayoutManager.Get().CreateLayout("CompassMarker", compassWidgetGlobal);
		if (compassWidget) {
			compassIconWidget = ImageWidget.Cast(compassWidget.FindAnyWidget("icon"));
			compassNameWidget = TextWidget.Cast(compassWidget.FindAnyWidget("name"));
			if (GetIcon().Length() > 0) {
				LBLogger.Debug("Loading Image: " + GetIcon(), "AdvancedGroups");
				compassIconWidget.LoadImageFile(0, GetIcon());
			} else {
				compassIconWidget.Show(false);
			}
			compassNameWidget.SetText(name);
			compassWidget.Show(false);
			SetColor(true);
		}

	}

	void OverwriteMaxDistance(float distance) {
		overwriteMaxDistance = true;
		overwrittenMaxDistance = distance;
	}

	void ResetMaxDistanceOverwrite() {
		overwriteMaxDistance = false;
	}

	float GetMaxDistance() {
		if (overwriteMaxDistance)
			return overwrittenMaxDistance;
		MarkerConfigEntry cfg = GetMarkerConfig();
		return cfg.maxDistance;
	}

	string GetIcon() {
		return icon;
	}

	bool SetIcon(string icon_) {
		if (this.icon != icon_) {
			this.icon = icon_;
			if (iconWidget)
				iconWidget.LoadImageFile(0, icon);
			return true;
		}
		return false;
	}

	bool IsGroupMarker() {
		return type == LBMarkerType.GROUP_PING || type == LBMarkerType.GROUP_PLAYER_MARKER;
	}

	void UpdateMarkerSlow() {
		if (!LBGroupMiscConfig.Get || !LBGroupSettingsConfig.Get || !LBMarkerSettingsConfig.Get) {
			return;
		}
		if (LBGroupMiscConfig.Get.enableCompassHud)
			InitCompassWidget();
		SetColor();
		if (LBGroupSettingsConfig.Get.enableSubGroups && IsGroupMarker()) {
			PlayerBase pb = PlayerBase.Cast(g_Game.GetPlayer());
			if (!pb)
				return;
			int mySubgroup = pb.GetMySubGroup();
			disable3dDifferentSubgroup = false;
			if (mySubgroup != currentSubgroup)
				disable3dDifferentSubgroup = true;
			if (disable3dDifferentSubgroup) {
				show3D = false;
				return;
			}
		}
		if (GetMarkerConfig()) {
			disable3dDifferentSubgroup = false;
			showMap = ShowMarkerMapOrGPS(true);
			showGPS = ShowMarkerMapOrGPS(false);
			show3D = ShowMarker3D();
		}
		SetDistance();

		GetMainWidgetSize();
		//LBLogger.Debug("Show Marker: " + name + ": " + show, "AdvancedGroups");
	}

	void GetMainWidgetSize() {
		if (!mainWidget)
			return;
		mainWidget.GetScreenSize(widthMainWidget, heightMainWidget);
		widthMainWidget = widthMainWidget / 2;
		heightMainWidget = heightMainWidget / 2;
	}

	void OnMarkerRPCClient(int type_, ParamsReadContext ctx) {
		LBLogger.Debug("Client Marker RPC Received " + type_, "AdvancedGroups");
		if (type_ == LBGroupRPCs.POSITION) {
			float x, y, z;
			if (!ctx.Read(x) || !ctx.Read(y) || !ctx.Read(z))
				return;
			vector vec = Vector(x, y, z);
			SetPosition(vec);
		} else if (type_ == LBGroupRPCs.SUBGRUOP) {
			int grp = 0;
			if (!ctx.Read(grp))
				return;
			LBLogger.Debug("Setting new Subgroup: " + grp, "AdvancedGroups");
			SetSubGroup(grp);
		} else if (type_ == LBGroupRPCs.NAME) {
			string name_;
			if (!ctx.Read(name_))
				return;
			SetName(name_);
		} else if (type_ == LBGroupRPCs.COLOR) {
			int a, r, g, b;
			if (!ctx.Read(a) || !ctx.Read(r) || !ctx.Read(g) || !ctx.Read(b))
				return;
			SetColorARGB(a, r, g, b);
			SetColor(true);
		} else if (type_ == LBGroupRPCs.CIRCLE) {
			float radius;
			int ca, cr, cg, cb;
			bool striked;
			int layer;
			LBLogger.Debug("Reading new Marker Circle Info", "AdvancedGroups");
			if (!ctx.Read(radius) || !ctx.Read(ca) || !ctx.Read(cr) || !ctx.Read(cg) || !ctx.Read(cb) || !ctx.Read(striked) || !ctx.Read(layer)) {
				LBLogger.Error("Failed to read new Server Marker Circle Info", "AdvancedGroups");
				return;
			}
			SetRadius(radius, ca, cr, cg, cb, striked, false, layer);
		} else if (type_ == LBGroupRPCs.VISIBILITY) {
			bool hidden_ = false;
			if (!ctx.Read(hidden_))
				return;
			SetHidden(hidden_);
		}
	}

	void AddToAllList() {
		if (allMarkers)
			allMarkers.Insert(this);
	}

	void RemoveFromAllList() {
		if (allMarkers)
			allMarkers.RemoveItem(this);
	}

	void LBMarker() {
		AddToAllList();
	}

	void SetSubGroup(int grp) {
		currentSubgroup = grp;
	}

	void SetPosition(vector pos) {
		position = pos;
		if (type == LBMarkerType.PRIVATE_MARKER) {
			LBPrivateMarkerManager.Loader.Save();
		}
	}

	void SendPositionToServer() {
		ScriptRPC rpc = CreateRPCCall(LBGroupRPCs.POSITION);
		rpc.Write(position[0]);
		rpc.Write(position[1]);
		rpc.Write(position[2]);
		SendMarkerRPC(rpc);
	}

	void SetRadius(float radius, bool sync = true) {
		if (this.circleRadius == radius)
			return;
		if (g_Game.IsDedicatedServer() || !sync) {
			this.circleRadius = radius;
		}
		if (sync) {
			SyncRadiusServer(radius, circleColorA, circleColorR, circleColorG, circleColorB, circleStriked, circleLayer);
		}
	}

	bool SetRadius(float radius, int cColorA, int cColorR, int cColorG, int cColorB, bool striked, bool sync = true, int layer = -1) {
		bool changed = false;
		if (circleRadius != radius || circleColorA != cColorA || circleColorR != cColorR || circleColorG != cColorG || circleColorB != cColorB || striked != circleStriked || layer != circleLayer)
			changed = true;
		LBLogger.Debug("SetRadius: " + changed + " Radius: " + radius + " A:" + cColorA + " R:" + cColorR + " G:" + cColorG + " B:" + cColorB + " Striked:" + striked + " Sync: " + sync + " Layer: " + layer, "AdvancedGroups");
		if (!changed)
			return false;
		if (g_Game.IsDedicatedServer() || !sync) {
			this.circleStriked = striked;
			this.circleLayer = layer;
			this.circleRadius = radius;
			this.circleColorA = cColorA;
			this.circleColorR = cColorR;
			this.circleColorG = cColorG;
			this.circleColorB = cColorB;
		}
		if (sync) {
			SyncRadiusServer(radius, cColorA, cColorR, cColorG, cColorB, striked, layer);
		}
		return true;
	}

	bool SetHidden(bool hidden_) {
		bool oldHidden = this.hidden;
		this.hidden = hidden_;
		return oldHidden != this.hidden;
	}

	bool SetRadius(float radius, int color, bool striked, bool sync = true, int layer = -1) {
		int a, r, g, b;
		LBConverter.ARGBToComponents(color, a, r, g, b);
		return SetRadius(radius, a, r, g, b, striked, sync, layer);
	}

	int GetCircleColor() {
		return ARGB(circleColorA, circleColorR, circleColorG, circleColorB);
	}

	void SyncRadiusServer(float radius, int ca, int cr, int cg, int cb, bool striked, int layer) {
		ScriptRPC rpc = CreateRPCCall(LBGroupRPCs.CIRCLE);
		rpc.Write(radius);
		rpc.Write(ca);
		rpc.Write(cr);
		rpc.Write(cg);
		rpc.Write(cb);
		rpc.Write(striked);
		rpc.Write(layer);
		SendMarkerRPC(rpc);
	}

	bool SetName(string name_) {
		if (this.name != name_) {
			this.name = name_;
			if (nameWidget)
				nameWidget.SetText(name, false);
			return true;
		}
		return false;
	}

	void ~LBMarker() {
		if (allMarkers)
			allMarkers.RemoveItem(this);
		if (mainWidget) {
			mainWidget.Unlink();
			mainWidget = null;
		}
		if (compassWidget) {
			compassWidget.Unlink();
			compassWidget = null;
		}
	}

	void SetColor(bool force = false) {
		int rgb = Get3DColorARGB();
		if (force || lastcolor != rgb) {
			if (iconWidget)
				iconWidget.SetColor(rgb);
			if (nameWidget)
				nameWidget.SetColor(rgb);
			if (compassNameWidget && compassIconWidget) {
				compassNameWidget.SetColor(rgb);
				compassIconWidget.SetColor(rgb);
			}
			if (distanceWidget) {
				distanceWidget.SetColor(ARGB(colorA, 255, 255, 255));
			}
			lastcolor = rgb;
		}
	}

	bool SetColorInt(int argb) {
		int a, r, g, b;
		LBConverter.ARGBToComponents(argb, a, r, g, b);
		return SetColorARGB(a, r, g, b);
	}

	bool SetColorARGB(int a, int r, int g, int b) {
		if (colorA != a || colorR != r || colorG != g || colorB != b) {
			colorA = a;
			colorR = r;
			colorG = g;
			colorB = b;
			return true;
		}
		return false;
	}

	bool SetColorARGBGlobal(int a, int r, int g, int b) {
		if (SetColorARGB(a, r, g, b)) {
			ScriptRPC rpc = CreateRPCCall(LBGroupRPCs.COLOR);
			rpc.Write(a);
			rpc.Write(r);
			rpc.Write(g);
			rpc.Write(b);
			LBLogger.Debug("Sending Color Change: " + a + " " + r + " " + g + " " + b, "AdvancedGroups");
			SendMarkerRPC(rpc);
			return true;
		}
		return false;
	}

	bool SetIconGlobal(string icon_) {
		if (SetIcon(icon_)) {
			ScriptRPC rpc = CreateRPCCall(LBGroupRPCs.ICON);
			rpc.Write(icon_);
			SendMarkerRPC(rpc);
			return true;
		}
		return false;
	}

	bool SetNameGlobal(string name_) {
		if (SetName(name_)) {
			ScriptRPC rpc = CreateRPCCall(LBGroupRPCs.NAME);
			rpc.Write(name_);
			SendMarkerRPC(rpc);
			return true;
		}
		return false;
	}

	MarkerConfigEntry GetMarkerConfig() {
		if (cachedMarkerConfig && !updateFOWRequired && !UpdatedFogOfWar())
			return cachedMarkerConfig;
		if (!LBMarkerSettingsConfig.Get)
			return null;
		cachedMarkerConfig = LBMarkerSettingsConfig.Get.GetMarkerConfigEntry(type, inFogOfWar);
		return cachedMarkerConfig;
	}

	bool UpdatedFogOfWar() {
		if (!LBFOWConfig.Get || !LBFOWConfig.Get.enabled || !LBFOWPlayerInfo_.CLIENT_RECEIVED_INFO) {
			inFogOfWar = false;
			return false;
		}
		bool before = inFogOfWar;
		inFogOfWar = IsInsideFogOfWar();
		return inFogOfWar != before;
	}

	bool IsInsideFogOfWar() {
		return !LBFOWPlayerInfo_.CLIENT_RECEIVED_INFO.IsPositionRevealed(position);
	}

	float GetCompassPosY() {
		if (type != LBMarkerType.GROUP_PING)
			return 0;
		return 0.5;
	}

	void SetDistance() {
		if (!g_Game || !g_Game.GetPlayer() || !ShowDistance3D() || !distanceWidget) {
			if (showBottom && bottomWidget)
				bottomWidget.Show(false, false);
			showBottom = false;
			return;
		} else if (!showBottom) {
			if (bottomWidget)
				bottomWidget.Show(true, false);
			showBottom = true;
		}
		dist = vector.Distance(position, cameraPos);
		if (dist < 1000) {
			int m = dist;
			distanceWidget.SetText("" + m.ToString() + "m", false);
		} else {
			float km = ((float)((int)(dist / 100))) / 10;
			distanceWidget.SetText("" + km.ToString() + "km", false);
		}
	}

	bool IsInRadius2D(vector position_) {
		float radiusSquared = circleRadius * circleRadius;
		float dX = position_[0] - position[0];
		float dZ = position_[2] - position[2];
		float distSquared = dX * dX + dZ * dZ;
		LBLogger.Verbose("Radius Check for " + name + ": " + radiusSquared + " " + dX + " " + dZ + " " + distSquared, "AdvancedGroups");
		return distSquared <= radiusSquared;
	}

	bool ShowMarkerMapOrGPS(bool isMap) {
		if (hidden)
			return false;
		if (streamerMode && (type == LBMarkerType.SERVER_STATIC || type == LBMarkerType.SERVER_DYNAMIC))
			return false;
		if (!GetMarkerConfig() || (isMap && !GetMarkerConfig().displayMap) || (!isMap && !GetMarkerConfig().displayGPS))
			return false;
		if (!LBMarkerVisibilityManager.Get.IsMapVisible(this, false))
			return false;
		if (type != LBMarkerType.GROUP_PING && !LBMarkerVisibilityManager.Get.IsGlobal2DVisible(type) || type == LBMarkerType.GROUP_PING && !LBMarkerVisibilityManager.Get.IsGlobal2DVisible(LBMarkerType.GROUP_MARKER))
			return false;
		LBServerMarker serverMarker;
		if (Class.CastTo(serverMarker, this)) {
			return ((isMap && serverMarker.displayMap) || (!isMap && serverMarker.displayGPS));
		}
		if (type == LBMarkerType.GROUP_PING || type == LBMarkerType.GROUP_MARKER || type == LBMarkerType.GROUP_PLAYER_MARKER) {

			PlayerBase pb = PlayerBase.Cast(g_Game.GetPlayer());
			if (!pb || !pb.GetLBGroup())
				return false;
			LBGroup grp = pb.GetLBGroup();
			LBGroupPermission myPerm = pb.GetPermission();
			if (!myPerm)
				return false;

			if (!myPerm.CanSeeMarkerType(type))
				return false;
			int mySubgroup = pb.GetMyGroupMarker().currentSubgroup;

			LBGroupMember memberMarker;
			if (Class.CastTo(memberMarker, this)) {
				bool checkSubgroupsPlayer = false;
				if (!LBGroupSettingsConfig.Get.enableSubGroupSharedPlayerMapMarker && LBGroupSettingsConfig.Get.enableSubGroups)
					checkSubgroupsPlayer = true;
				return (!checkSubgroupsPlayer || mySubgroup == memberMarker.currentSubgroup);
			} else if (type == LBMarkerType.GROUP_PING) {
				bool checkSubgroupsPing = false;
				if (!LBGroupSettingsConfig.Get.enableSubGroupSharedPingMapMarker && LBGroupSettingsConfig.Get.enableSubGroups)
					checkSubgroupsPing = true;
				return (!checkSubgroupsPing || mySubgroup == currentSubgroup);
			}
		}
		return true;
	}

	bool ShowMarker3D() {
		if (hidden)
			return false;
		if (streamerMode && (type == LBMarkerType.SERVER_STATIC || type == LBMarkerType.SERVER_DYNAMIC))
			return false;
		MarkerConfigEntry cfg = GetMarkerConfig();
		//	LBLogger.Debug("MarkerEntry: " + cfg, "AdvancedGroups");
		if (!cfg)
			return false;
		dist = vector.Distance(position, cameraPos);
		float maxDistance = GetMaxDistance();
		//	LBLogger.Debug("Dist: " + dist + " from: " + pos + " To: " + position, "AdvancedGroups");
		if (maxDistance < 0 || maxDistance > dist) {
			if (!LBMarkerVisibilityManager.Get.Is3DVisiblie(this))
				return false;
		} else {
			return false;
		}
		if (type == LBMarkerType.GROUP_PING || type == LBMarkerType.GROUP_MARKER || type == LBMarkerType.GROUP_PLAYER_MARKER) {

			PlayerBase pb = PlayerBase.Cast(g_Game.GetPlayer());
			if (!pb || !pb.GetLBGroup())
				return false;
			LBGroup grp = pb.GetLBGroup();
			LBGroupPermission myPerm = pb.GetPermission();
			if (!myPerm)
				return false;

			if (!myPerm.CanSeeMarkerType(type))
				return false;
			int mySubgroup = pb.GetMyGroupMarker().currentSubgroup;

			LBGroupMember memberMarker;
			if (Class.CastTo(memberMarker, this)) {
				bool checkSubgroupsPlayer = false;
				if (!LBGroupSettingsConfig.Get.enableSubGroupSharedPlayerMapMarker && LBGroupSettingsConfig.Get.enableSubGroups)
					checkSubgroupsPlayer = true;
				return (!checkSubgroupsPlayer || mySubgroup == memberMarker.currentSubgroup);
			} else if (type == LBMarkerType.GROUP_PING) {
				bool checkSubgroupsPing = false;
				if (!LBGroupSettingsConfig.Get.enableSubGroupSharedPingMapMarker && LBGroupSettingsConfig.Get.enableSubGroups)
					checkSubgroupsPing = true;
				return (!checkSubgroupsPing || mySubgroup == currentSubgroup);
			}
		}
		return true;
	}

	bool ShowDistance3D() {
		if (type == LBMarkerType.GROUP_PLAYER_MARKER && (LBLayoutConfig.Get.playerMarkerStyleIndex & 0x04) != 0)
			return false;
		MarkerConfigEntry cfg = GetMarkerConfig();
		return cfg && cfg.displayDistance;
	}

	bool ShouldCenterWidget() {
		return type == LBMarkerType.GROUP_PING || type == LBMarkerType.GROUP_PLAYER_MARKER;
	}

	int GetColorARGB() {
		if (type == LBMarkerType.GROUP_PING) {
			return LBColorManager.Get.GetColor("Ping 3D Marker");
		}
		return ARGB(colorA, colorR, colorG, colorB);
	}

	int Get3DColorARGB() {
		return GetColorARGB();
	}

	bool UpdateMarkerClient() {
		if (!mainWidget || (!showMap && !show3D))
			return false;
		if (iconWidget) {
			string new_icon = GetIcon();
			if (new_icon != lastIcon) {
				LBLogger.Debug("Loading Image: " + new_icon, "AdvancedGroups");
				iconWidget.LoadImageFile(0, new_icon);
				lastIcon = new_icon;
			}
		}
		return true;
	}

	bool SetWidgetPosition() {
		if (!mainWidget || !position) {
			return false;
		}
		if (colorA == 0 || !show3D) {
			SetVisibleOnScreen(false);
			return false;
		}
		if (compassWidget) {
			float angle = GetMarkerAngle();
			float posX = (angle / 180.0) - 0.5;
			if (posX < -1)
				posX += 2;
			compassWidget.SetPos(posX, GetCompassPosY());
		}
		vector screenPos = g_Game.GetScreenPos(position);
		float x = screenPos[0];
		float y = screenPos[1];
		if (ShouldCenterWidget()) {
			x -= widthMainWidget;
			y -= heightMainWidget;
		}
		if (x <= 0 || x >= LBWidgetUtils.screenWidth || y <= 0 || y >= LBWidgetUtils.screenHeight || screenPos[2] <= 0) {
			SetVisibleOnScreen(false);
			return false;
		}
		mainWidget.SetPos(x, y);
		SetVisibleOnScreen(true);
		return true;
	}

	float GetMarkerAngle() {
		vector camPos = g_Game.GetCurrentCameraPosition();
		vector dir = camPos - position;
		dir = dir.Normalized();
		vector angles = dir.VectorToAngles();
		float angle = angles[0] - currentCameraAngle + 360;
		while (angle > 180)
			angle -= 360;
		return angle;
	}

	bool IsMainWidgetVisible() {
		return visibleOnScreen && !disable3dDifferentSubgroup && show3D;
	}

	void ShowMainWidget(bool show) {
		if (!mainWidget)
			return;
		if (!mainWidget.IsVisible() && show) {
			mainWidget.Show(show);
			GetMainWidgetSize();
		}
		mainWidget.Show(show);
	}

	void SetVisibleOnScreen(bool b) {
		visibleOnScreen = b;
		ShowMainWidget(IsMainWidgetVisible());
		if (compassWidget) {
			compassWidget.Show(!disable3dDifferentSubgroup && show3D);
		}
	}

	bool ReadFromCtx(ParamsReadContext ctx) {
		if (!ctx.Read(type))
			return false;
		if (!ctx.Read(uid))
			return false;
		if (!ctx.Read(name))
			return false;
		if (!ctx.Read(icon))
			return false;
		if (!ctx.Read(position))
			return false;
		if (!ctx.Read(colorA))
			return false;
		if (!ctx.Read(colorR))
			return false;
		if (!ctx.Read(colorG))
			return false;
		if (!ctx.Read(colorB))
			return false;
		if (!ctx.Read(currentSubgroup))
			return false;
		if (!ctx.Read(circleRadius))
			return false;
		if (!ctx.Read(circleColorA))
			return false;
		if (!ctx.Read(circleColorR))
			return false;
		if (!ctx.Read(circleColorG))
			return false;
		if (!ctx.Read(circleColorB))
			return false;
		if (!ctx.Read(circleStriked))
			return false;
		if (!ctx.Read(circleLayer))
			return false;
		if (!ctx.Read(showAllPlayerNametags))
			return false;
		return true;
	}

	int CalcHash(bool includePosition = true) {
		int pos = 0;
		if (includePosition)
			pos = position.ToString().Hash();
		int hash = type + uid + name.Hash() + pos + colorA * 4546546 + colorB * 45426365 + colorR * 52412 + colorB * 4241 + currentSubgroup * 745423;
		hash = hash + ((int) circleRadius) * 546353 + circleColorA * 7422221 + circleColorR * 756742 + circleColorG * 876543 + circleColorB * 787465 + circleLayer * 745472;
		if (circleStriked)
			hash = hash + 7432435;
		return hash;
	}

	void WriteToCtx(ParamsWriteContext ctx, bool steamids_ = true, bool positions_ = true) {
		ctx.Write(type);
		ctx.Write(uid);
		ctx.Write(name);
		ctx.Write(icon);
		if (positions_)
			ctx.Write(position);
		else
			ctx.Write(vector.Zero);
		ctx.Write(colorA);
		ctx.Write(colorR);
		ctx.Write(colorG);
		ctx.Write(colorB);
		ctx.Write(currentSubgroup);
		ctx.Write(circleRadius);
		ctx.Write(circleColorA);
		ctx.Write(circleColorR);
		ctx.Write(circleColorG);
		ctx.Write(circleColorB);
		ctx.Write(circleStriked);
		ctx.Write(circleLayer);
		ctx.Write(showAllPlayerNametags);
	}

	ScriptRPC CreateRPCCall(int type_) {
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(type_);
		rpc.Write(uid);
		return rpc;
	}

	void SendMarkerRPC(ScriptRPC rpc) {
		if (type == LBMarkerType.PRIVATE_MARKER)
			return;
		if (!parentGroup && g_Game.IsClient() && g_Game.IsMultiplayer()) {
			PlayerBase pb = PlayerBase.Cast(g_Game.GetPlayer());
			parentGroup = pb.GetLBGroup();
		}
		if (parentGroup) {
#ifdef LBMASTER_GROUPS_DEBUG
			LBLogger.Debug("Sending Marker RPC...", "AdvancedGroups");
#endif
			if (g_Game.IsServer()) {
				parentGroup.SendRPCToGroupMembers(rpc);
			} else {
				parentGroup.SendRPCToServer(rpc);
			}
		} else if (type == LBMarkerType.SERVER_STATIC || type == LBMarkerType.SERVER_DYNAMIC) {
			LBLogger.Debug("Sending Static Marker RPC", "AdvancedGroups");
			rpc.Send(null, LBGroupRPCs.MARKER_RPC, true);
		} else {
			if (!parentGroup)
				LBLogger.Debug("Failed to send Marker RPC for Marker: " + type + " No Parent Group ! ", "AdvancedGroups");
			else
				LBLogger.Debug("Failed to send Marker RPC for Marker: " + type + " Parent Group: " + parentGroup.shortname, "AdvancedGroups");
		}
	}

}
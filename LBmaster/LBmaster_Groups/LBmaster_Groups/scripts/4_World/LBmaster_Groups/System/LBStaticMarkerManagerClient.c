class LBStaticMarkerManagerClient {

	ref array<ref LBServerMarker> staticMarkers = new array<ref LBServerMarker>();

	static ref LBStaticMarkerManagerClient g_LBGroupGroups;

	static void Delete() {
		g_LBGroupGroups = null;
	}

	static LBStaticMarkerManagerClient Get() {
		if (!g_LBGroupGroups) {
			g_LBGroupGroups = new LBStaticMarkerManagerClient();
		}
		return g_LBGroupGroups;
	}

	void RequestGlobalMarkerAdd(LBMarker marker) {
		ScriptRPC rpc = new ScriptRPC();
		marker.WriteToCtx(rpc);
		rpc.Send(null, LBGroupRPCs.CONFIG_GLOBAL_MARKER_ADD, true);
	}

	void RequestGlobalMarkerRemove(int uid) {
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(uid);
		rpc.Send(null, LBGroupRPCs.CONFIG_GLOBAL_MARKER_REMOVE, true);
	}

	void RequestGlobalMarkerUpdate(int uid) {
		LBServerMarker marker = FindMarkerByUID(uid);
		if (!uid)
			return;
		ScriptRPC rpc = new ScriptRPC();
		marker.WriteToCtx(rpc);
		rpc.Send(null, LBGroupRPCs.CONFIG_GLOBAL_MARKER_CHANGE, true);
	}

	void StaticMarkersReceivedRPC(ParamsReadContext ctx) {
		int count;
		if (!ctx.Read(count)) {
			LBLogger.Error("Unable to receive Static Markers from Server !", "AdvancedGroups");
			return;
		}
		LBLogger.Debug("Trying to read " + count + " Static Markers from the Server", "AdvancedGroups");
		DeleteStaticMarkers();
		for (int i = 0; i < count; i++) {
			LBServerMarker mark = new LBServerMarker;
			if (mark.ReadFromCtx(ctx)) {
				AddNewMarker(mark);
				LBLogger.Debug("Received Static Marker from Server: " + mark.name + " " + mark.icon + " " + mark.colorA + " " + mark.colorR + " " + mark.colorG + " " + mark.colorB + " " + mark.CalcHash(), "AdvancedGroups");
			} else {
				LBLogger.Error("Failed to read received Markers from Server. Index: " + i, "AdvancedGroups");
				return;
			}
		}
		LBLogger.Info("Received Static Markers from Server: " + staticMarkers.Count(), "AdvancedGroups");

	}

	void StaticMarkerAddedRPC(ParamsReadContext ctx) {
		LBServerMarker mark = new LBServerMarker;
		if (!mark.ReadFromCtx(ctx))
			return;
		AddNewMarker(mark);
	}

	void StaticMarkerRemovedRPC(ParamsReadContext ctx) {
		int uid = 0;
		if (!ctx.Read(uid))
			return;
		DeleteMarker(uid);
	}

	void StaticMarkerChangedRPC(ParamsReadContext ctx) {
		LBServerMarker mark = new LBServerMarker;
		if (!mark.ReadFromCtx(ctx))
			return;
		LBServerMarker previous = FindMarkerByUID(mark.uid);
		if (!previous) {
			AddNewMarker(mark);
			return;
		}
		ChangeMarker(previous, mark);
	}

	void ChangeMarker(LBServerMarker myMarker, LBServerMarker from) {
		myMarker.name = from.name;
		myMarker.colorA = from.colorA;
		myMarker.colorR = from.colorR;
		myMarker.colorG = from.colorG;
		myMarker.colorB = from.colorB;
		myMarker.circleRadius = from.circleRadius;
		myMarker.circleColorA = from.circleColorA;
		myMarker.circleColorR = from.circleColorR;
		myMarker.circleColorG = from.circleColorG;
		myMarker.circleColorB = from.circleColorB;
		myMarker.circleStriked = from.circleStriked;
		myMarker.circleLayer = from.circleLayer;
		myMarker.showAllPlayerNametags = from.showAllPlayerNametags;
		myMarker.icon = from.icon;
	}

	void MarkerRPC(ParamsReadContext ctx) {
		int uid, type_;
		if (!ctx.Read(type_) || !ctx.Read(uid))
			return;
		LBServerMarker marker = FindMarkerByUID(uid);
		LBLogger.Debug("Marker UID: " + uid + " RPC Type: " + type_ + " Marker: " + marker, "AdvancedGroups");
		if (!marker)
			return;
		marker.OnMarkerRPCClient(type_, ctx);
	}

	void DeleteStaticMarkers() {
		staticMarkers.Clear();
	}

	void AddNewMarker(LBServerMarker marker) {
		AddMarker(marker);
		marker.InitMarker();
	}

	void AddMarker(LBServerMarker marker) {
		if (!marker)
			return;
		staticMarkers.Insert(marker);
	}

	void DeleteMarker(LBServerMarker marker) {
		if (!marker)
			return;
		staticMarkers.RemoveItem(marker);
	}

	void DeleteMarker(int uid) {
		LBServerMarker marker = FindMarkerByUID(uid);
		while (marker) {
			staticMarkers.RemoveItem(marker);
			marker = FindMarkerByUID(uid);
		}
	}

	LBServerMarker FindMarkerByUID(int uid) {
		foreach (LBServerMarker marker : staticMarkers) {
			if (marker.uid == uid)
				return marker;
		}
		return null;
	}

	bool FindNearestMarker(vector position, out LBMarker markero, out float distance) {
		if (staticMarkers.Count() == 0)
			return false;
		float bestDist = 0;
		LBMarker bestMarker = null;
		foreach (LBMarker marker : staticMarkers) {
			if (!bestMarker || vector.Distance(position, marker.position) < bestDist) {
				bestMarker = marker;
				bestDist = vector.Distance(position, marker.position) < bestDist;
			}
		}
		if (bestMarker) {
			markero = bestMarker;
			distance = bestDist;
			return true;
		}
		return false;
	}

}
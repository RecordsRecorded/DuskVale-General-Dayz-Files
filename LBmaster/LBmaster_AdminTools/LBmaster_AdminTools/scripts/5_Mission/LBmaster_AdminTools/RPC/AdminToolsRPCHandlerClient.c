[RegisterLBRPCHandler(LBConverter.TypenameToString(AdminToolsRPCHandlerClient), LBRPCHandlerType.CLIENT)]
class AdminToolsRPCHandlerClient : LB_RPCHandler {

	ref array<ref Param4<string, vector, int, int>> receivedMapItems = new array<ref Param4<string, vector, int, int >> ();
	ref map<string, ref Param3<int, int, int>> ceValues = new map<string, ref Param3<int, int, int >> ();
	bool requestedAllItems = false;
	ref ScriptInvoker Event_Map_Items_Received = new ScriptInvoker();

	void AdminToolsRPCHandlerClient() {
		RegisterRPC(LBAdminToolRPCs.SET_TIME, ScriptCaller.Create(HandleSetTimeRPC));
		RegisterRPC(LBAdminToolRPCs.SET_MODIFIER, ScriptCaller.Create(HandleSetModifierRPC));
		RegisterRPC(LBAdminToolRPCs.REQUEST_ITEMS, ScriptCaller.Create(HandleReceiveItemList));
		RegisterRPC(LBAdminToolRPCs.SPECTATE_OBJECT, ScriptCaller.Create(HandleSpectateObject));
		RegisterRPC(LBAdminToolRPCs.SPECTATE_POSITION, ScriptCaller.Create(HandleSpectatePosition));
		RegisterRPC(LBAdminToolRPCs.SELECT_PLAYER, ScriptCaller.Create(HandleSelectPlayerRPC));
		RegisterRPC(LBAdminToolRPCs.WATCHLIST_ADD, ScriptCaller.Create(HandleWatchlistAdd));
		RegisterRPC(LBAdminToolRPCs.WATCHLIST_MODIFY, ScriptCaller.Create(HandleWatchlistModify));
		RegisterRPC(LBAdminToolRPCs.WATCHLIST_REMOVE, ScriptCaller.Create(HandleWatchlistRemove));
		RegisterRPC(LBAdminToolRPCs.PRESET_DELETE, ScriptCaller.Create(HandlePresetDelete));
		RegisterRPC(LBAdminToolRPCs.PRESET_RENAME, ScriptCaller.Create(HandlePresetRename));
		RegisterRPC(LBAdminToolRPCs.PRESET_CREATE, ScriptCaller.Create(HandlePresetCreate));
		RegisterRPC(LBAdminToolRPCs.PRESET_SAVE, ScriptCaller.Create(HandlePresetSave));
		RegisterRPC(LBAdminToolRPCs.GLOBAL_BANLIST_UPDATE, ScriptCaller.Create(HandleBanlistUpdate));
		RegisterRPC(LBAdminToolRPCs.GLOBAL_WATCHLIST_UPDATE, ScriptCaller.Create(HandleWatchlistUpdate));
		RegisterRPC(LBAdminToolRPCs.INFO_SYNC_MORE, ScriptCaller.Create(HandleSendMoreInfo));
		RegisterRPC(LBAdminToolRPCs.MODS_SYNC_MORE, ScriptCaller.Create(HandleSendMoreMods));
		RegisterRPC(LBAdminToolRPCs.PLAYER_CONFIG_MORE, ScriptCaller.Create(HandlePlayerMoreInfo));
	}

	void HandleWatchlistUpdate() {
		LBAdminWebRequestWatchlist info;
		if (!ctx.Read(info) || !LBWatchlistGlobal.Get)
			return;
		LBWatchlistGlobal.Get.currentIncrement = 0;
		LBWatchlistGlobal.Get.OnWatchlistInfoReceived(info);
	}

	void HandleBanlistUpdate() {
		LBAdminWebRequestBanlist info;
		if (!ctx.Read(info) || !LBGlobalBanManager.Get)
			return;
		LBGlobalBanManager.Get.currentIncrement = 0;
		LBGlobalBanManager.Get.OnBanInfoReceived(info);
		LBAdminMenuFrame_Banmanager.reloadEntries = true;
	}

	void HandleSendMoreInfo() {
		int count = g_Game.ConfigGetChildrenCount("CfgPatches");
		TStringArray loadedPatches = new TStringArray();
		for (int i = 0; i < count; ++i) {
			string name;
			g_Game.ConfigGetChildName("CfgPatches", i, name);
			loadedPatches.Insert(name);
		}
		rpc.Write(loadedPatches);
		Respond();
	}

	void HandleSendMoreMods() {
		int count = g_Game.ConfigGetChildrenCount("CfgMods");
		TStringArray loadedMods = new TStringArray();
		for (int i = 0; i < count; ++i) {
			string name;
			g_Game.ConfigGetChildName("CfgMods", i, name);
			loadedMods.Insert(name);
		}
		rpc.Write(loadedMods);
		Respond();
	}

	void HandlePresetDelete() {
		Param1<string> param;
		if (!ctx.Read(param))
			return;
		LBAdminItemPresetConfigServer.Get.OnDeleteRequest(param);
	}

	void HandlePresetRename() {
		Param2<string, string> param;
		if (!ctx.Read(param))
			return;
		LBAdminItemPresetConfigServer.Get.OnRenameRequest(param);

	}

	void HandlePresetCreate() {
		Param1<string> param;
		if (!ctx.Read(param))
			return;
		LBAdminItemPresetConfigServer.Get.OnCreateRequest(param);
	}

	void HandlePresetSave() {
		Param1<ref LBAdminItemPresetItem> param;
		if (!ctx.Read(param))
			return;
		LBAdminItemPresetConfigServer.Get.OnSaveRequest(param);
	}

	void HandleWatchlistAdd() {
		LBWatchlistServer.Get.HandleAddRPC(ctx);
	}

	void HandleWatchlistModify() {
		LBWatchlistServer.Get.HandleAddRPC(ctx);
	}

	void HandleWatchlistRemove() {
		LBWatchlistServer.Get.HandleRemoveRPC(ctx);
	}

	void HandlePlayerMoreInfo() {
		TStringArray arr = new TStringArray();
		foreach (ActionBase action : ActionManagerBase.m_ActionsArray) {
			arr.Insert(action.ClassName());
		}
		rpc.Write(arr);
		Respond();
	}

	void HandleReceiveItemList() {
		receivedMapItems.Clear();
		int sliceSize = (g_Game.GetWorld().GetWorldSize() + 319) / 320;
		for (int x = sliceSize - 1; x >= 0; --x) {
			for (int z = sliceSize - 1; z >= 0; --z) {
				int count;
				if (!ctx.Read(count))
					break;
				for (int i = 0; i < count; ++i) {
					int hash, itemxz, childCounts;
					if (!ctx.Read(hash) || !ctx.Read(itemxz) || !ctx.Read(childCounts))
						continue;
					string itemname = LBInherit.Get().HashToItemname(hash);
					if (itemname == "")
						LBLogger.Error("Could not find Itemname with Hash " + hash, "AdminTools");
					int itemx = itemxz & 0xffff;
					int itemz = (itemxz >> 16) & 0xffff;
					int layer1Children = childCounts & 0xffff;
					int allChildren = ((childCounts >> 16) & 0xffff) + layer1Children;
					vector pos = Vector(itemx / 100.0 + x * 320, 0, itemz / 100.0 + z * 320);
					Param4<string, vector, int, int> param = new Param4<string, vector, int, int>(itemname, pos, layer1Children, allChildren);
					receivedMapItems.Insert(param);
				}
			}
		}
		array<ref Param4<int, int, int, int>> ceParams;
		ctx.Read(ceParams);
		ceValues.Clear();
		foreach (Param4<int, int, int, int> ceParam : ceParams) {
			itemname = LBInherit.Get().HashToItemname(ceParam.param1);
			ceValues.Insert(itemname, new Param3<int, int, int>(ceParam.param2, ceParam.param3, ceParam.param4));
		}
		Event_Map_Items_Received.Invoke();
		requestedAllItems = false;
	}

	void HandleSetModifierRPC() {
		MissionGameplay mg = MissionGameplay.Cast(g_Game.GetMission());
		if (!mg)
			return;
		Param3<string, bool, int> param;
		if (!ctx.Read(param))
			return;
		mg.LBSetModifierEnabled(param.param1, param.param2, param.param3);
	}

	void HandleSetTimeRPC() {
		Param1<ref LBTimePreset> param;
		if (!ctx.Read(param) || !param.param1)
			return;
		param.param1.Apply();
	}

	void HandleSelectPlayerRPC() {
		MissionGameplay mg = MissionGameplay.Cast(g_Game.GetMission());
		mg.wasSpectating = false;
	}

	void HandleSpectatePosition() {
		vector pos;
		int low, high;
		if (!ctx.Read(pos) || !ctx.Read(low) || !ctx.Read(high))
			return;
		if (DayZPlayerCameraSpectator.currentSpecCamera) {
			DayZPlayerCameraSpectator.currentSpecCamera.SetPlayerPos(pos);
			DayZPlayerCameraSpectator.currentSpecCamera.SetPlayer(pos, low, high);
		}
	}

	void HandleSpectateObject() {
		vector pos;
		int low, high;
		PlayerIdentity myIdentity;
		if (!ctx.Read(pos) || !ctx.Read(low) || !ctx.Read(high) || !ctx.Read(myIdentity))
			return;
		PlayerBase.lb_wasSpectating = true;
		DayZPlayerCameraSpectator.SetPlayer(pos, low, high);
		if (PlayerBase.lb_tempPlayer) {
			g_Game.ObjectDelete(PlayerBase.lb_tempPlayer);
			g_Game.ObjectDelete(PlayerBase.tempPlayerParent);
			PlayerBase.lb_tempPlayer = null;
		}
		if (!PlayerBase.lb_tempPlayer) {
			PlayerBase.lb_tempPlayer = PlayerBase.Cast(g_Game.CreateObject("SurvivorM_Rolf", vector.Zero, true));
			PlayerBase.tempPlayerParent = g_Game.CreateObject("Matchbox", "0 10 0", true);
			g_Game.SelectPlayer(myIdentity, PlayerBase.lb_tempPlayer);
			PlayerBase.tempPlayerParent.AddChild(PlayerBase.lb_tempPlayer, -1, false);
		}
	}

}
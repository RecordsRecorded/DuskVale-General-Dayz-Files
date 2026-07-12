class LBESPManager {

	static ref array<EntityAI> allItems = new array<EntityAI>();
	static ref array<PlayerBase> allPlayers = new array<PlayerBase>();
	static ref array<PlayerBase> remainPlayers = new array<PlayerBase>();
	static ref array<Widget> itemWidgets = new array<Widget>();
	static ref array<TextWidget> distanceWidgets = new array<TextWidget>();
	static ref array<int> itemTypes = new array<int>();
	static ref array<bool> filteredItems = new array<bool>();
	static ref TStringSet filter = new TStringSet();
	static ref array<EntityAI> highlightedItems = new array<EntityAI>();

	static CanvasWidget skeletonCanvas;
	static Widget rootWidget, markerPanel;
	static bool enabled = false, skeletonEnabled = false, useClassnames;
	static float maxDistance = -1;
	static int distanceMode = 0;
	static Widget lastHighlighted = null;
	static EntityAI highlightedEntity = null;
	static bool disableHighlighting = false;
	static bool updatedHighlightedItems = false;
	static int lastGroupTagsUpdated = 0;

	static const ref TStringArray PLAYER_BONES = {"pelvis", "spine", "spine1", "spine2", "spine3", "neck", "neck1", "head", "face_forehead", "lefthand", "leftarm", "rightarm", "righthand", "rightfoot", "rightleg", "leftfoot", "leftleg"};
	static const ref TStringArray ZOMBIE_BONES = {"pelvis", "spine", "spine1", "spine2", "spine3", "neck", "neck1", "head", "lefthand", "leftarm", "rightarm", "righthand", "rightfoot", "rightleg", "leftfoot", "leftleg"};
	static const ref TStringArray ANIMAL_BONES = {"pelvis", "spine1", "spine2", "chest", "neck1", "neck2", "neck3", "head"};

	static void ToggleESP() {
		if (enabled)
			DisableESP();
		else
			EnableESP();
	}

	static void DisableESP() {
		enabled = false;
		UpdateRootWidget();
		itemWidgets = new array<Widget>();
		distanceWidgets = new array<TextWidget>();
		allItems = new array<EntityAI>();
		itemTypes = new array<int>();
		filteredItems = new array<bool>();
		g_Game.RPCSingleParam(null, LBAdminToolRPCs.TOGGLE_ESP, new Param1<bool>(false), true);
	}

	static void UpdateColors() {
		int count = allItems.Count();
		for (int i = 0; i < count; ++i) {
			Widget w = itemWidgets.Get(i);
			EntityAI item = allItems.Get(i);
			TextWidget txt = TextWidget.Cast(w.FindAnyWidget("txt"));
			int color = GetItemColor(item);
			txt.SetColor(color);
			int outlineARGB = (txt.GetOutlineColor() & 0x00ffffff) | (color & 0xff000000);
			txt.SetOutline(txt.GetOutlineSize(), outlineARGB);
		}
	}

	static void SetPlayerName(EntityAI item, TextWidget txt, bool addToRemain) {
		if (useClassnames) {
			txt.SetText(item.GetType());
			return;
		}
		PlayerBase pb = PlayerBase.Cast(item);
		if (pb && pb.GetIdentity()) {
#ifdef LBmaster_Groups
			string groupname, groupshortname;
			if (LBGroupInfoShare.GetPlayerGroup(pb.GetIdentity().GetPlainId(), groupname, groupshortname)) {
				txt.SetText("[" + groupshortname + "] " + pb.GetIdentity().GetName());
			} else {
				txt.SetText(pb.GetIdentity().GetName());
			}
#else
			txt.SetText(pb.GetIdentity().GetName());
#endif
		} else {
			if (pb && addToRemain)
				remainPlayers.Insert(pb);
			string dpName = item.GetDisplayName();
			if (dpName == "")
				txt.SetText(item.GetType());
			else
				txt.SetText(dpName);
		}
	}

	static void UpdateNames() {
		int count = allItems.Count();
		for (int i = 0; i < count; ++i) {
			Widget w = itemWidgets.Get(i);
			EntityAI item = allItems.Get(i);
			TextWidget txt = TextWidget.Cast(w.FindAnyWidget("txt"));
			if (item)
				SetPlayerName(item, txt, false);
		}
	}

	static void UpdateFilter() {
		filter = LBAdminMenuSettings.Get.GetESPFilter();
		filteredItems = new array<bool>();
		foreach (EntityAI ent : allItems) {
			filteredItems.Insert(IsItemFiltered(ent));
		}
	}

	static bool IsItemFiltered(EntityAI ent) {
		if (!ent)
			return false;
		int healthLevel = 4 - ent.GetHealthLevel();
		if (LBAdminMenuSettings.Get.espMinHealth > healthLevel || LBAdminMenuSettings.Get.espMaxHealth < healthLevel)
			return false;
		if (filter.Count() == 0)
			return true;
		string lower = LBStringTools.ToLowerString(ent.GetType());
		return filter.Find(lower) != -1;
	}

	static int GetItemType(EntityAI item) {
		if (!item)
			return -1;
		if (PlayerBase.Cast(item))
			return 1;
		if (AnimalBase.Cast(item))
			return 2;
		if (ZombieBase.Cast(item))
			return 3;
		if (CarScript.Cast(item) || BoatScript.Cast(item))
			return 4;
		return 0;
	}

	static int GetItemColor(EntityAI item) {
		return LBAdminMenuSettings.Get.GetItemColor(item);
	}

	static void EnableESP() {
		DisableESP();
		UpdateFilter();
		enabled = true;
		useClassnames = LBAdminMenuSettings.Get.espShowClassnames;
		UpdateRootWidget();
		int mapSize = g_Game.GetWorld().GetWorldSize();
		DayZPlayerUtils.SceneGetEntitiesInBox("0 -1000 0", Vector(mapSize, 10000, mapSize), allItems);
		foreach (EntityAI ent : allItems) {
			InitItem(ent);
		}
		g_Game.RPCSingleParam(null, LBAdminToolRPCs.TOGGLE_ESP, new Param1<bool>(true), true);
	}

	static void UpdateRootWidget() {
		if (skeletonEnabled || enabled)
			CreateRootWidget();
		else if (!skeletonEnabled && !enabled) {
			DestroyRootWidget();
			return;
		}
		skeletonCanvas.Show(skeletonEnabled);
		markerPanel.Show(enabled);
		if (!enabled)
			LBWidgetUtils.DeleteAllChildren(markerPanel);
	}

	static void CreateRootWidget() {
		if (rootWidget)
			return;
		rootWidget = LBLayoutManager.Get().CreateLayout("ESPOverlay");
		markerPanel = rootWidget.FindAnyWidget("markerPanel");
		skeletonCanvas = CanvasWidget.Cast(rootWidget.FindAnyWidget("skeletonCanvas"));
	}

	static void DestroyRootWidget() {
		if (rootWidget)
			rootWidget.Unlink();
		rootWidget = null;
	}

	static void SetMaxDistance(float dist) {
		maxDistance = dist;
	}

	static void AddItem(EntityAI item) {
		if (Head.Cast(item) && !item.GetInventory())
			return;
		allItems.Insert(item);
		InitItem(item);
	}

	static void UpdateHealthLevel(EntityAI item) {
		int index = allItems.Find(item);
		if (index == -1)
			return;
		UpdateHealthLevel(item, itemWidgets.Get(index));
	}

	static void UpdateHealthLevel(EntityAI item, Widget parent) {
		Widget color = parent.FindAnyWidget("color");
		int level = item.GetHealthLevel();
		color.SetColor(GetHealthColor(level));
		color.SetAlpha(0.7);
	}

	static int GetHealthColor(int level) {
		switch (level) {
			case -1:
				return 0;
			case GameConstants.STATE_PRISTINE:
				return Colors.COLOR_PRISTINE;
			case GameConstants.STATE_WORN:
				return Colors.COLOR_WORN;
			case GameConstants.STATE_DAMAGED:
				return Colors.COLOR_DAMAGED;
			case GameConstants.STATE_BADLY_DAMAGED:
				return Colors.COLOR_BADLY_DAMAGED;
			case GameConstants.STATE_RUINED:
				return Colors.COLOR_RUINED;
		}
		return -1;
	}

	static int GetWidgetIndex(Widget w) {
		return itemWidgets.Find(w);
	}

	static void InitItem(EntityAI item) {
		if (item) {
			Widget w = LBLayoutManager.Get().CreateLayout("ESPMarker", markerPanel);
			itemWidgets.Insert(w);
			TextWidget txt = TextWidget.Cast(w.FindAnyWidget("txt"));
			SetPlayerName(item, txt, true);
			int color = GetItemColor(item);
			txt.SetColor(color);
			int outlineARGB = (txt.GetOutlineColor() & 0x00ffffff) | (color & 0xff000000);
			txt.SetOutline(txt.GetOutlineSize(), outlineARGB);
			TextWidget distance = TextWidget.Cast(w.FindAnyWidget("distance"));
			distanceWidgets.Insert(distance);
			UpdateHealthLevel(item, w);
		} else {
			itemWidgets.Insert(null);
			distanceWidgets.Insert(null);
		}
		itemTypes.Insert(GetItemType(item));
		filteredItems.Insert(IsItemFiltered(item));
	}

	static void RemovePlayer(PlayerBase player) {
		if (allPlayers)
			allPlayers.RemoveItem(player);
	}

	static void RemoveItem(EntityAI item) {
		int index = allItems.Find(item);
		if (index == -1)
			return;
		allItems.Remove(index);
		Widget w = itemWidgets.Get(index);
		if (w)
			w.Unlink();
		w = distanceWidgets.Get(index);
		if (w)
			w.Unlink();
		itemWidgets.Remove(index);
		distanceWidgets.Remove(index);
		itemTypes.Remove(index);
		filteredItems.Remove(index);
	}

	static void SetSkeletonEnabled(bool enable) {
		skeletonEnabled = enable;
		UpdateRootWidget();
		if (!enable && skeletonCanvas)
			skeletonCanvas.Clear();
	}

	static void DrawBones(PlayerBase player, int thickness, TStringArray bones) {
		vector lastScreen;
		bool lastVisible = false;
		for (int i = 0; i < bones.Count(); ++i) {
			string boneName = bones.Get(i);
			int bone = player.GetBoneIndexByName(boneName);
			if (bone < 0)
				continue;
			vector world = player.GetBonePositionWS(bone);

			vector screen = g_Game.GetScreenPos(world);
			if (screen[2] <= 0 || screen[0] <= 0 || screen[0] >= LBWidgetUtils.screenWidth || screen[1] <= 0 || screen[1] >= LBWidgetUtils.screenHeight) {
				lastVisible = false;
				continue;
			}
			if (lastVisible) {
				int color = LBAdminMenuSettings.Get.espSkeletonColor;
				if (!player.IsAlive())
					color = LBAdminMenuSettings.Get.espSkeletonColorDead;
				skeletonCanvas.DrawLine(screen[0], screen[1], lastScreen[0], lastScreen[1], thickness, color);
			}
			if (LBAdminMenuSettings.Get.espSkeletonDrawJoints)
				skeletonCanvas.DrawLine(screen[0] - thickness / 2, screen[1] - thickness / 2, screen[0] + thickness / 2, screen[1] + thickness / 2, thickness + 3, LBAdminMenuSettings.Get.espJointsColor);
			lastScreen = screen;
			lastVisible = true;
		}
	}

	static void DrawPlayer(PlayerBase player, float distance) {
		if (!player)
			return;
		int thickness = 3;
		if (distance > 10)
			thickness = 2;
		if (distance > 100)
			thickness = 1;
		DrawBones(player, thickness, {"righttoebase", "rightfoot", "rightlegroll", "rightleg", "rightupleg", "pelvis"});
		DrawBones(player, thickness, {"lefttoebase", "leftfoot", "leftlegroll", "leftleg", "leftupleg", "pelvis"});
		DrawBones(player, thickness, {"pelvis", "spine", "spine1", "spine2", "spine3", "neck", "neck1", "head", "face_forehead"});
		DrawBones(player, thickness, {"lefthand", "leftforearmroll", "leftforearm", "leftarmroll", "leftarm", "neck", "rightarm", "rightarmroll", "rightforearm", "rightforearmroll", "righthand"});
		if (LBAdminMenuSettings.Get.espSkeletonHighDetail) {
			DrawBones(player, thickness, {"lefthandring3", "lefthandring2", "lefthandring1", "lefthand", "lefthandpinky1", "lefthandpinky2", "lefthandpinky3"});
			DrawBones(player, thickness, {"lefthandmiddle3", "lefthandmiddle2", "lefthandmiddle1", "lefthand", "lefthandindex1", "lefthandindex2", "lefthandindex3"});
			DrawBones(player, thickness, {"lefthandthumb3", "lefthandthumb2", "lefthandthumb1", "lefthand"});

			DrawBones(player, thickness, {"righthandring3", "righthandring2", "righthandring1", "righthand", "righthandpinky1", "righthandpinky2", "righthandpinky3"});
			DrawBones(player, thickness, {"righthandmiddle3", "righthandmiddle2", "righthandmiddle1", "righthand", "righthandindex1", "righthandindex2", "righthandindex3"});
			DrawBones(player, thickness, {"righthandthumb3", "righthandthumb2", "righthandthumb1", "righthand"});

			DrawBones(player, thickness, {"face_forehead", "face_eyelidupperleft", "face_browmiddle", "face_forehead"});
			DrawBones(player, thickness, {"face_forehead", "face_eyelidupperright", "face_browmiddle", "face_forehead"});
			/*
						vector startScreen, endScreen;
						Weapon_Base wpn = Weapon_Base.Cast(player.GetItemInHands());
						HumanCommandWeapons wpnCmd = player.GetCommandModifier_Weapons();

						if (wpn) {
							vector end = player.GetBonePositionWS(player.GetBoneIndexByName("lefthandindex2"));
							vector start = player.GetBonePositionWS(player.GetBoneIndexByName("righthandindex3"));
							vector dir = (end - start).Normalized();
							if (ClipLineToScreen(start, start + dir * 200, startScreen, endScreen)) {
								//LBESPManager.skeletonCanvas.DrawLine(startScreen[0], startScreen[1], endScreen[0], endScreen[1], 1.0, ARGB(255, 255, 0, 0));
							}
							Debug.DestroyAllShapes();
							Debug.DrawLine(start, start + dir * 200);
						} else {
							int index = player.GetBoneIndexByName("head");
							if (index != -1) {
								float headQuat[4];
								player.GetBoneRotationWS(index, headQuat);
								vector angles = Math3D.QuatToAngles(headQuat) + "270.4 0 0";
								//angles[0] = angles[0] - neckAngles[0];
								if (angles[0] > 360)
									angles[0] = angles[0] - 360;
								angles[1] = angles[2] + 90;
								dir = angles.AnglesToVector();
								vector headPos = player.GetBonePositionWS(index);
								if (ClipLineToScreen(headPos, headPos + dir * 200, startScreen, endScreen)) {
									LBESPManager.skeletonCanvas.DrawLine(startScreen[0], startScreen[1], endScreen[0], endScreen[1], 1.0, ARGB(255, 255, 0, 0));
								}
							}
						}
			*/

		}
	}

	static bool IsOnScreen(vector screenPos) {
		return screenPos[0] > 0 && screenPos[0] < LBWidgetUtils.screenWidth && screenPos[1] > 0 && screenPos[1] < LBWidgetUtils.screenHeight && screenPos[2] > 0;
	}

	static bool ClipLineToScreen(vector startWS, vector endWS, out vector startSS, out vector endSS) {
		startSS = g_Game.GetScreenPos(startWS);
		endSS = g_Game.GetScreenPos(endWS);
		bool startOs = IsOnScreen(startSS);
		bool endOs = IsOnScreen(endSS);
		if (startOs && endOs) {
			return true;
		} else if (startOs) {
			vector dir = (endWS - startWS).Normalized();
			vector dirSS = g_Game.GetScreenPos(startWS + dir);
			if (!IsOnScreen(dirSS)) {
				return false;
			}
			float length = (endWS - startWS).Length() * 10;
			endSS = startSS + (dirSS - startSS) * length;
			return true;
		} else if (endOs) {
			dir = (startWS - endWS).Normalized();
			dirSS = g_Game.GetScreenPos(endWS + dir);
			if (!IsOnScreen(dirSS)) {
				return false;
			}
			length = (startWS - endWS).Length() * 10;
			startSS = endSS + (dirSS - endSS) * length;
			return true;
		} else {
			vector nearest = Math3D.NearestPoint(startWS, endWS, g_Game.GetCurrentCameraPosition());
			vector nearSS = g_Game.GetScreenPos(nearest);
			if (!IsOnScreen(nearSS)) {
				return false;
			}
			dir = (startWS - nearest).Normalized();
			dirSS = g_Game.GetScreenPos(nearest + dir);
			if (!IsOnScreen(dirSS)) {
				return false;
			}
			length = (startWS - nearest).Length();
			startSS = nearSS - (dirSS - nearSS) * length;
			endSS = nearSS + (dirSS - nearSS) * length;

			return true;
		}

		return false;
	}

	static void OnUpdate() {
		if (!enabled && !skeletonEnabled)
			return;
		vector camPos;
		if (distanceMode == 0 || (LBFreecamCamera.IsActive() && LBFreecamCamera.fullMap))
			camPos = g_Game.GetCurrentCameraPosition();
		else
			camPos = g_Game.GetPlayer().GetPosition();
		if (skeletonEnabled && skeletonCanvas) {
			skeletonCanvas.Clear();
			foreach (PlayerBase player : allPlayers) {
				if (!player)
					continue;
				float dist = vector.Distance(player.GetPosition(), camPos);
				if (LBAdminMenuSettings.Get.espSkeletonIgnoreDistance || dist <= maxDistance)
					DrawPlayer(player, dist);
			}
		}
		if (!enabled)
			return;
		for (int a = 0; a < remainPlayers.Count(); ++a) {
			PlayerBase pb2 = remainPlayers.Get(a);
			if (!pb2) {
				remainPlayers.Remove(a);
				--a;
				continue;
			}
			if (pb2.GetIdentity()) {
				int index = allItems.Find(pb2);
				if (index != -1) {
					Widget w2 = itemWidgets.Get(index);
					if (w2) {
						TextWidget txt = TextWidget.Cast(w2.FindAnyWidget("txt"));
						SetPlayerName(pb2, txt, false);
					}
				}
				remainPlayers.Remove(a);
				--a;
				continue;
			}
		}
#ifdef LBmaster_Groups
		if (LBGroupInfoShare.lastChange != lastGroupTagsUpdated) {
			lastGroupTagsUpdated = LBGroupInfoShare.lastChange;
			UpdateNames();
		}
#endif
		int count = allItems.Count();
		int highlighted = -1;
		float highlightedDistance = 0;
		int hBWMax = LBWidgetUtils.screenWidth / 2;
		int hBWMin = hBWMax - 150 * LBWidgetUtils.heightScale;
		int hBHMax = LBWidgetUtils.screenHeight / 2;
		int hBHMin = hBHMax - 20 * LBWidgetUtils.heightScale;
		highlightedEntity = null;
		for (int i = 0; i < count; ++i) {
			Widget w = itemWidgets.Get(i);
			if (!w)
				continue;
			if (!filteredItems.Get(i)) { // Item not in filter
				w.Show(false);
				continue;
			}
			EntityAI item = allItems.Get(i);
			int type = itemTypes.Get(i);
			if (item) {
				vector position = item.GetPosition();
				if (type == 1) {
					PlayerBase pb = PlayerBase.Cast(item);
					if (!pb.IsFlagSet(EntityFlags.VISIBLE)) {
						w.Show(false);
						continue;
					}
					int bone = pb.GetBoneIndexByName("Head");
					if (bone != -1)
						position = pb.GetBonePositionWS(bone);
				} else if (type == 2) {
					AnimalBase an = AnimalBase.Cast(item);
					bone = an.GetBoneIndexByName("Head");
					if (bone != -1)
						position = an.GetBonePositionWS(bone);
				} else if (type == 3) {
					ZombieBase zm = ZombieBase.Cast(item);
					bone = zm.GetBoneIndexByName("Head");
					if (bone != -1)
						position = zm.GetBonePositionWS(bone);
				}
				float distance = vector.Distance(position, camPos);
				w.SetSort(1000 - ((int) Math.Clamp(distance, 0, 1000)));
				if (maxDistance < 0 || distance <= maxDistance) {
					vector pos = g_Game.GetScreenPos(position);
					w.SetScreenPos(pos[0], pos[1]);
					dist = pos[2];
					if (pos[0] > 0 && pos[1] > 0 && dist > 0) {
						w.Show(true);
						distanceWidgets.Get(i).SetText("(" + ((int) distance) + "m)");
						if (pos[0] >= hBWMin && pos[0] <= hBWMax && pos[1] >= hBHMin && pos[1] <= hBHMax) {
							if (highlighted == -1 || highlightedDistance > distance) {
								highlighted = i;
								highlightedDistance = distance;
								highlightedEntity = item;
							}
						}
					} else {
						w.Show(false);
					}
				} else {
					w.Show(false);
				}
			} else {
				allItems.Remove(i);
				if (w)
					w.Unlink();
				w = distanceWidgets.Get(i);
				if (w)
					w.Unlink();
				distanceWidgets.Remove(i);
				itemWidgets.Remove(i);
				itemTypes.Remove(i);
				filteredItems.Remove(i);
				--i;
				--count;
			}
		}
		if (highlighted != -1 && !disableHighlighting) {
			Widget highlight = itemWidgets.Get(highlighted);
			HighlightWidget(highlight);
		} else if (lastHighlighted) {
			ResetHighlight();
		} else {
			lastHighlighted = null;
		}
	}

	static void SetHighlighting(bool enable) {
		if (!enable && lastHighlighted)
			ResetHighlight();
		disableHighlighting = !enable;
	}

	static void ResetHighlight() {
		lastHighlighted.FindAnyWidget("highlight").Show(false);
		lastHighlighted = null;
	}

	static void RemoveHighlightedItemFromList(EntityAI item) {
		highlightedItems.RemoveItem(item);
		int index = allItems.Find(item);
		if (index >= 0) {
			Widget w = itemWidgets.Get(index);
			w.FindAnyWidget("highlight").Show(false);
		}
	}

	static void HighlightWidgetsIn(int x, int y, int w, int h) {
		highlightedItems.Clear();
		for (int i = 0; i < itemWidgets.Count(); ++i) {
			Widget wgd = itemWidgets.Get(i);
			if (!wgd)
				continue;

			if (!wgd.IsVisible()) {
				wgd.FindAnyWidget("highlight").Show(false);
				continue;
			}
			float wX, wY;
			wgd.GetScreenPos(wX, wY);
			if (x < wX && y < wY && x + w >= wX && y + h >= wY) {
				wgd.FindAnyWidget("highlight").Show(true);
				highlightedItems.Insert(allItems.Get(i));
			} else {
				wgd.FindAnyWidget("highlight").Show(false);
			}
		}
		updatedHighlightedItems = true;
	}

	static void HighlightWidget(Widget highlight) {
		if (lastHighlighted != highlight) {
			highlight.FindAnyWidget("highlight").Show(true);
			updatedHighlightedItems = true;
			if (lastHighlighted)
				lastHighlighted.FindAnyWidget("highlight").Show(false);
		}
		lastHighlighted = highlight;
	}

}
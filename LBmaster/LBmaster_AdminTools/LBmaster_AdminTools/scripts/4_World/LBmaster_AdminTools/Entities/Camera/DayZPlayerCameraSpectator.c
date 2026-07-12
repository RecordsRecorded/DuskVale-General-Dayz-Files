class DayZPlayerCameraSpectator : Camera {

	PlayerBase targetObj;
	float distance, rotation, angle, m_SendUpdateAcc; //!< distance from start
	UAInput zoomIn, zoomOut;
	static int playerLow, playerHigh, headSlotId;
	static vector lastPlayerPos, lastPlayerOrient;
	static bool nextFullMap = false;

	static DayZPlayerCameraSpectator currentSpecCamera;

	void DayZPlayerCameraSpectator() {
		currentSpecCamera = this;
		zoomOut = GetUApi().GetInputByName("UALBMADMSpectatorZoomOut");
		zoomIn = GetUApi().GetInputByName("UALBMADMSpectatorZoomIn");
		SetEventMask(EntityEvent.FRAME);
		headSlotId = InventorySlots.GetSlotIdFromString("Head");
		if (nextFullMap) {
			nextFullMap = false;
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Call(SetCameraToFreecam);
		}
	}

	void SetCameraToFreecam() {
		if (PlayerBase.lb_tempPlayer) {
			g_Game.ObjectDelete(PlayerBase.lb_tempPlayer);
			g_Game.ObjectDelete(PlayerBase.tempPlayerParent);
			PlayerBase.lb_tempPlayer = null;
		}
		if (!PlayerBase.lb_tempPlayer) {
			PlayerBase.lb_tempPlayer = PlayerBase.Cast(g_Game.CreateObject("SurvivorM_Rolf", vector.Zero, true));
			PlayerBase.tempPlayerParent = g_Game.CreateObject("Matchbox", "0 10 0", true);
			g_Game.SelectPlayer(null, PlayerBase.lb_tempPlayer);
			PlayerBase.tempPlayerParent.AddChild(PlayerBase.lb_tempPlayer, -1, false);
		}
		if (LBFreecamCamera.instance != null) {
			LBFreecamCamera.instance.fullMap = true;
			return;
		}
		LBFreecamCamera.Set(true);
		LBFreecamCamera.instance.fullMap = true;
	}

	void ~DayZPlayerCameraSpectator() {
		distance = 1.0;
		UpdateHead();
	}

	void SetPlayerPos(vector pos) {
		lastPlayerPos = pos;
		g_Game.UpdateSpectatorPosition(pos);
	}

	static void SetPlayer(vector pos, int low, int high) {
		playerLow = low;
		playerHigh = high;
		lastPlayerPos = pos;
	}

	void UpdateEntityVisibility(EntityAI ent) {
		if (ent) {
			if (distance < 0.5)
				ent.SetInvisible(true);
			else
				ent.SetInvisible(false);
		}
	}

	void UpdateHead() {
		if (targetObj) {
			int headBone = targetObj.GetBoneIndexByName("head");
			lastPlayerPos = targetObj.GetBonePositionWS(headBone);
			lastPlayerOrient = targetObj.GetOrientation();
			UpdateEntityVisibility(targetObj.GetInventory().FindPlaceholderForSlot(headSlotId)); // Head
			UpdateEntityVisibility(targetObj.GetInventory().FindAttachmentByName("Head")); // Helmet
			UpdateEntityVisibility(targetObj.GetInventory().FindAttachmentByName("Headgear")); // Helmet
			UpdateEntityVisibility(targetObj.GetInventory().FindAttachmentByName("Mask")); // Helmet
			UpdateEntityVisibility(targetObj.GetInventory().FindAttachmentByName("Eyewear")); // Helmet

		}
	}

	override void EOnFrame(IEntity other, float timeSlice) {
		m_SendUpdateAcc += timeSlice;
		if (!targetObj) {
			targetObj = PlayerBase.Cast(g_Game.GetObjectByNetworkId(playerLow, playerHigh));
		}
		UpdateHead();
		if (!g_Game.GetUIManager().GetMenu()) {
			distance = Math.Clamp(distance + (zoomOut.LocalValue() - zoomIn.LocalValue()) * Math.Sqrt(distance + 0.01), 0.01, 1000);
			rotation -= GetUApi().GetInputByID(UAAimLeft).LocalValue() - GetUApi().GetInputByID(UAAimRight).LocalValue();
			angle += GetUApi().GetInputByID(UAAimDown).LocalValue() - GetUApi().GetInputByID(UAAimUp).LocalValue();
		}
		angle = Math.Clamp(angle, -89, 89);
		float height = Math.Tan(Math.DEG2RAD * angle);
		float cos = Math.Cos(Math.DEG2RAD * (rotation + lastPlayerOrient[0]));
		float sin = Math.Sin(Math.DEG2RAD * (rotation + lastPlayerOrient[0]));
		vector posRel = Vector(sin, height, cos);
		posRel = posRel.Normalized() * distance;
		vector dir = DirectionToAngles(-posRel);
		vector transform[4];
		Math3D.YawPitchRollMatrix(dir, transform);
		transform[3] = lastPlayerPos + posRel;
		SetTransform(transform);
		if (m_SendUpdateAcc > 0.5) {
			g_Game.RPCSingleParam(null, LBAdminToolRPCs.SPECTATE_POSITION, new Param1<bool>(true), true);
			m_SendUpdateAcc = 0;
		}
	}

	vector DirectionToAngles(vector dir) {
		vector mat[4];
		Math3D.DirectionAndUpMatrix(dir, vector.Up, mat);
		return Math3D.MatrixToAngles(mat);
	}
}
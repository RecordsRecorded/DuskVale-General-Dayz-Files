modded class VicinityItemManager {

	override void RefreshVicinityItems() {
		if ((!DayZPlayerCameraSpectator.currentSpecCamera || !DayZPlayerCameraSpectator.currentSpecCamera.targetObj) && !LBFreecamCamera.instance) {
			super.RefreshVicinityItems();
			return;
		}

		PlayerBase player;
		vector playerPosition;
		vector headingDirection;
		if (DayZPlayerCameraSpectator.currentSpecCamera) {
			player = DayZPlayerCameraSpectator.currentSpecCamera.targetObj;
			playerPosition	= player.GetPosition();
			headingDirection	= MiscGameplayFunctions.GetHeadingVector(player);
		} else if (LBFreecamCamera.instance) {
			player = PlayerBase.Cast(g_Game.GetPlayer());
			playerPosition = LBFreecamCamera.position;
			headingDirection = LBFreecamCamera.orientation.AnglesToVector();
		}
		vector playerHeadPositionFixed	= playerPosition;
		playerHeadPositionFixed[1]	= playerPosition[1] + 1.6;
		array<Object> objectsInVicinity	= new array<Object>();
		array<CargoBase> proxyCargos	= new array<CargoBase>();
		array<Object> filteredObjects	= new array<Object>();
		array<Object> allFoundObjects	= new array<Object>();

		if (m_VicinityItems)
			m_VicinityItems.Clear();

		if (m_VicinityCargos)
			m_VicinityCargos.Clear();

		g_Game.GetObjectsAtPosition3D(playerPosition, VICINITY_ACTOR_DISTANCE, objectsInVicinity, proxyCargos);

		foreach (CargoBase cargoObject : proxyCargos)
			AddVicinityCargos(cargoObject);

		foreach (Object actorInRadius : objectsInVicinity) {
			if (allFoundObjects.Find(actorInRadius) == INDEX_NOT_FOUND)
				allFoundObjects.Insert(actorInRadius);

			if (ExcludeFromContainer_Phase1(actorInRadius))
				continue;

			if (filteredObjects.Find(actorInRadius) == INDEX_NOT_FOUND)
				filteredObjects.Insert(actorInRadius);
		}

		if (objectsInVicinity)
			objectsInVicinity.Clear();

		g_Game.GetObjectsAtPosition3D(playerPosition, VICINITY_DISTANCE, objectsInVicinity, proxyCargos);

		foreach (Object objectInRadius : objectsInVicinity) {
			if (allFoundObjects.Find(objectInRadius) == INDEX_NOT_FOUND)
				allFoundObjects.Insert(objectInRadius);

			if (ExcludeFromContainer_Phase2(objectInRadius))
				continue;

			if (filteredObjects.Find(objectInRadius) == INDEX_NOT_FOUND)
				filteredObjects.Insert(objectInRadius);
		}

		if (objectsInVicinity)
			objectsInVicinity.Clear();

		DayZPlayerUtils.GetEntitiesInCone(playerPosition, headingDirection, VICINITY_CONE_ANGLE, VICINITY_CONE_REACH_DISTANCE, CONE_HEIGHT_MIN, CONE_HEIGHT_MAX, objectsInVicinity);

		RaycastRVParams rayInput;
		array<ref RaycastRVResult> raycastResults = new array<ref RaycastRVResult>();
		foreach (Object objectInCone : objectsInVicinity) {
			if (allFoundObjects.Find(objectInCone) == INDEX_NOT_FOUND)
				allFoundObjects.Insert(objectInCone);

			if (ExcludeFromContainer_Phase3(objectInCone))
				continue;

			if (filteredObjects.Find(objectInCone) == INDEX_NOT_FOUND) {
				rayInput	= new RaycastRVParams(playerHeadPositionFixed, objectInCone.GetPosition());
				rayInput.flags	= CollisionFlags.NEARESTCONTACT;
				rayInput.type	= ObjIntersectView;
				rayInput.radius = 0.1;
				DayZPhysics.RaycastRVProxy(rayInput, raycastResults, {player});

				foreach (RaycastRVResult result : raycastResults) {
					if (vector.DistanceSq(result.pos, playerHeadPositionFixed) > VICINITY_CONE_REACH_DISTANCE * VICINITY_CONE_REACH_DISTANCE)
						continue;

					if (result.hierLevel > 0 && result.parent == objectInCone)
						filteredObjects.Insert(objectInCone);
					else if (result.hierLevel == 0 && result.obj == objectInCone)
						filteredObjects.Insert(objectInCone);
				}
			}
		}

		foreach (Object filteredObject: filteredObjects) {
			EntityAI entity = EntityAI.Cast(filteredObject);
			if (entity)
				m_VicinityItems.Insert(entity);
		}

	}

	override bool ExcludeFromContainer_Phase1(Object actor_in_radius) {
		if (!LBFreecamCamera.instance)
			return super.ExcludeFromContainer_Phase1(actor_in_radius);
		EntityAI entity;
		if (!Class.CastTo(entity, actor_in_radius))
			return true;

		PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
		if (entity == player || entity.IsParticle() || entity.IsScriptedLight() || entity.IsBeingPlaced() || entity.IsHologram())
			return true;
		return false;
	}

}
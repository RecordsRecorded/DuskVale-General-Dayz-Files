modded class MissionServer extends MissionBase
{
	void MissionServer()
	{
		GetDayZGame().Event_OnRPC.Insert(OnRPC);
	}

	void ~MissionServer()
	{
		if (GetDayZGame())
		{
			GetDayZGame().Event_OnRPC.Remove(OnRPC);
		}
	}

	override void OnMissionStart() 
	{
		super.OnMissionStart();
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.OnTerritoryInit, 500, false); // Delay so CE is Initialized 
	}
	
	void OnTerritoryInit()
	{
		GetMultiTerritoriesConfig().DebugLog("OnInit");
		GetMultiTerritoriesConfig();
	}
	
	
	void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx) 
	{
		if (rpc_type == MULTITERRITORYRPCs.REQUEST_START && sender)
		{
			GetGame().RPCSingleParam(NULL, MULTITERRITORYRPCs.REQUEST_START, new Param1< MultiTerritoriesConfig >(GetMultiTerritoriesConfig()), true, sender);
		}
	}

	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
	{
		super.InvokeOnConnect(player, identity);

		if (!player || !identity)
			return;

		MultiModLogger.Log("Player logged in: " + identity.GetName() + " (" + identity.GetPlainId() + ")" + " (pos=" + player.GetPosition() + ")", "login");

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CheckTerritory, 1111 + Math.RandomIntInclusive(0, 1111), false, player);
	}

	override void InvokeOnDisconnect(PlayerBase player)
	{
		super.InvokeOnDisconnect(player);

		if (player && player.GetIdentity())
		{
			MultiModLogger.Log("Player logged out: " + player.GetIdentity().GetName() + " (" + player.GetIdentity().GetPlainId() + ")" + " (pos=" + player.GetPosition() + ")", "login");
		}
	}

	private void CheckTerritory(PlayerBase player)
	{
		if (!player || !player.GetIdentity())
			return;

		bool loggedInsideTerritory = false;
		vector flagPos;
		float protectionRadius = GetMultiTerritoriesConfig().GetProtectionRadius();

		foreach(vector v : TerritoryFlag.FLAG_LOCATIONS)
		{
			if (vector.Distance(v, player.GetPosition()) <= (protectionRadius + 10))
			{
				autoptr TerritoryFlag flag;
				autoptr array<Object> objectsNearFlag = new array<Object>;
				GetGame().GetObjectsAtPosition(v, 1, objectsNearFlag, null);

				// Find flag
				for (int x = 0; x < objectsNearFlag.Count(); x++)
				{
					flag = TerritoryFlag.Cast(objectsNearFlag.Get(x));
					if (flag)
					{
						if (flag.HasRaisedFlag() && !flag.IsTerritoryMember(player.GetIdentity().GetId()))
						{
							loggedInsideTerritory = true;
							flagPos = v;
						}

						break;
					}
				}
			}
		}

		if (loggedInsideTerritory)
		{
			int randomX, randomZ;
			float hitFraction;
			vector movePos, hitPosition, hitNormal, to;
			Object hitObject;
			PhxInteractionLayers collisionLayerMask = PhxInteractionLayers.ITEM_LARGE | PhxInteractionLayers.BUILDING | PhxInteractionLayers.VEHICLE;

			// Attempt to find 1000 spots with no roof above
			for (int i = 0; i < 1000; i++)
			{
				// Calculate random position
				randomX = Math.RandomFloatInclusive(-protectionRadius, protectionRadius);
				randomZ = Math.RandomFloatInclusive(-protectionRadius, protectionRadius);
				movePos = flagPos;

				if (randomX <= 0)
					randomX -= (protectionRadius + Math.RandomFloatInclusive(0, 25));
				else
					randomX += (protectionRadius + Math.RandomFloatInclusive(0, 25));

				if (randomZ <= 0)
					randomZ -= (protectionRadius + Math.RandomFloatInclusive(0, 25));
				else
					randomZ += (protectionRadius + Math.RandomFloatInclusive(0, 25));
				
				movePos[0] = movePos[0] + randomX;
				movePos[2] = movePos[2] + randomZ;
				movePos[1] = GetGame().SurfaceY(movePos[0], movePos[2]);
				to = movePos + "0 25 0";

				// Teleport player if no roof/tree above them
				if (!DayZPhysics.RayCastBullet(movePos, to, collisionLayerMask, null, hitObject, hitPosition, hitNormal, hitFraction))
				{
					DeveloperTeleport.SetPlayerPosition(player, movePos);
					MultiModLogger.Log("Moved player out of territory: " + player.GetIdentity().GetName() + " (" + player.GetIdentity().GetPlainId() + ") - took " + i + " roof checks");
					
					int liquidType;
					string surfaceType;
					GetGame().SurfaceUnderObject(player, surfaceType, liquidType);

					// Don't allow spawning player inside a new territory
					foreach (vector v2 : TerritoryFlag.FLAG_LOCATIONS)
					{
						if (vector.Distance(v2, player.GetPosition()) <= (protectionRadius + 10))
						{
							continue;
						}
					}

					// If player spawns into water, try a new spawn
					//if (liquidType != LIQUID_WATER)
					//{
						GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(NotifyPlayerOfTeleport, 5000, false, player);
						break;
					//}
				}
				else
					MultiModLogger.Log("Can't move player out of territory - attempts=" + i);
			}
		}
	}

	private void NotifyPlayerOfTeleport(PlayerBase player)
	{
		MultiFunctions.SendPlayerMessage(player, "Territory: You have been moved outside of this territory as you do not belong to it and we don't allow logging out in bases to offline raid.");
	}
}

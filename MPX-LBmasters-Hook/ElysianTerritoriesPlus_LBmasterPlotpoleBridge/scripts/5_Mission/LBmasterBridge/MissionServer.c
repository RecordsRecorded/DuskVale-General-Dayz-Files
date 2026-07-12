#ifdef LBmaster_GroupDLCPlotpole
modded class MissionServer
{
	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
	{
		super.InvokeOnConnect(player, identity);

		if (!player || !identity)
		{
			return;
		}

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ETP_LB_CheckPlotpoleTerritoryLogin, 2500, false, player);
	}

	protected void ETP_LB_CheckPlotpoleTerritoryLogin(PlayerBase player)
	{
		if (!ETP_LB_PlotpoleBridge.CanRun() || !player || !player.GetIdentity())
		{
			return;
		}

		TerritoryFlag flag = ETP_LB_PlotpoleBridge.FindBlockingLBPlotpole(player, player.GetPosition());
		if (!flag)
		{
			return;
		}

		ETP_LB_MovePlayerOutsidePlotpole(player, flag);
	}

	protected void ETP_LB_MovePlayerOutsidePlotpole(PlayerBase player, TerritoryFlag flag)
	{
		if (!player || !player.GetIdentity() || !flag)
		{
			return;
		}

		float radius = flag.ETP_LB_GetPlotpoleRadius();
		if (radius < 1)
		{
			radius = ETP_LB_PlotpoleBridge.GetBaseProtectionRadius();
		}

		vector flagPos = flag.GetPosition();
		vector movePos;
		vector to;
		vector hitPosition;
		vector hitNormal;
		Object hitObject;
		float hitFraction;
		float randomX;
		float randomZ;
		PhxInteractionLayers collisionLayerMask = PhxInteractionLayers.ITEM_LARGE | PhxInteractionLayers.BUILDING | PhxInteractionLayers.VEHICLE;

		for (int i = 0; i < 1000; i++)
		{
			randomX = Math.RandomFloatInclusive(-radius, radius);
			randomZ = Math.RandomFloatInclusive(-radius, radius);
			movePos = flagPos;

			if (randomX <= 0)
			{
				randomX -= (radius + Math.RandomFloatInclusive(0, 25));
			}
			else
			{
				randomX += (radius + Math.RandomFloatInclusive(0, 25));
			}

			if (randomZ <= 0)
			{
				randomZ -= (radius + Math.RandomFloatInclusive(0, 25));
			}
			else
			{
				randomZ += (radius + Math.RandomFloatInclusive(0, 25));
			}

			movePos[0] = movePos[0] + randomX;
			movePos[2] = movePos[2] + randomZ;
			movePos[1] = GetGame().SurfaceY(movePos[0], movePos[2]);
			to = movePos + "0 25 0";

			if (!ETP_LB_PlotpoleBridge.IsSafeTeleportDestination(player, movePos))
			{
				continue;
			}

			if (!DayZPhysics.RayCastBullet(movePos, to, collisionLayerMask, null, hitObject, hitPosition, hitNormal, hitFraction))
			{
				DeveloperTeleport.SetPlayerPosition(player, movePos);
				MultiModLogger.Log("[ETP_LB] Moved player out of LB plotpole territory: " + player.GetIdentity().GetName() + " (" + player.GetIdentity().GetPlainId() + ") - took " + i + " checks", "login");
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ETP_LB_NotifyPlayerOfPlotpoleTeleport, 5000, false, player);
				return;
			}
		}

		MultiModLogger.Log("[ETP_LB] Could not move player out of LB plotpole territory: " + player.GetIdentity().GetName() + " (" + player.GetIdentity().GetPlainId() + ")", "login");
	}

	protected void ETP_LB_NotifyPlayerOfPlotpoleTeleport(PlayerBase player)
	{
		MultiFunctions.SendPlayerMessage(player, "Territory: You have been moved outside of this LB group territory as you do not belong to it and we don't allow logging out in bases to offline raid.");
	}
}
#endif

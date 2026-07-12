modded class ActionDeployObject : ActionContinuousBase
{			 
	protected int m_LastSync = 0;
	protected bool m_CanPlaceHere = false;
	protected vector m_LastCheckLocation = vector.Zero;
	
	#ifdef BASICMAP
	protected ref BasicTerritoryMapMarker BASICT_Marker;
	#endif
	
	void ~ActionDeployObject() 
	{
		RemoveTheBasicMapMarker();
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (super.ActionCondition(player, target, item))
		{
			PlayerBase thePlayer = PlayerBase.Cast(player);
			string theGUID = "";

			if (thePlayer && thePlayer.GetIdentity())
			{
				theGUID = thePlayer.GetIdentity().GetId();
			}

			if (ShouldBlockDeployActionAtPlayerPosition(thePlayer, item, theGUID))
			{
				return false;
			}
			
			GetMultiTerritoriesConfig().DebugLog("ActionCondition player=" + player + " hologram=" + thePlayer.GetHologramLocal());
			
			if (thePlayer && thePlayer.GetHologramLocal() && thePlayer.GetHologramLocal().GetProjectionEntity())
			{
				// Client placement mode - use hologram position
				bool forceTerritoryRecheck = item && item.IsInherited(REV_TerritoryComputerKit);
				if (forceTerritoryRecheck || vector.Distance(m_LastCheckLocation, thePlayer.GetHologramLocal().GetProjectionEntity().GetPosition()) > 0.4)
				{
					#ifdef BASICMAP
					bool ShowTerritoryOnMap = BasicMap().ShowSelfOnMap();
					if (item && item.IsInherited(REV_TerritoryComputerKit) && ShowTerritoryOnMap)
					{
						if (!BASICT_Marker)
						{
							BASICT_Marker = new BasicTerritoryMapMarker("", thePlayer.GetHologramLocal().GetProjectionEntity().GetPosition());
							BASICT_Marker.SetRadius(GetMultiTerritoriesConfig().GetProtectionRadius());
							BasicMap().AddMarker("BasicTerritories", BASICT_Marker);
						} else 
						if (BASICT_Marker.GetPosition() != thePlayer.GetHologramLocal().GetProjectionEntity().GetPosition()) 
						{
							if (BASICT_Marker.GetPosition() == vector.Zero)
							{
								BasicMap().AddMarker("BasicTerritories", BASICT_Marker);
							}
							BASICT_Marker.SetPosition(thePlayer.GetHologramLocal().GetProjectionEntity().GetPosition());
							BASICT_Marker.SetRadius(GetMultiTerritoriesConfig().GetProtectionRadius());
						}
					} else 
					{
						if (BASICT_Marker && BASICT_Marker.GetPosition() != vector.Zero && ShowTerritoryOnMap)
						{
							BasicMap().RemoveMarker("BasicTerritories", BASICT_Marker);
							BASICT_Marker.SetPosition(vector.Zero);
							BASICT_Marker.SetRadius(0);
						}
					}
					#endif
					return CanIPlaceHere(item, thePlayer.GetHologramLocal().GetProjectionEntity(), thePlayer.GetHologramLocal().GetProjectionEntity().GetPosition(), theGUID);
				} else 
				{
					return m_CanPlaceHere;
				}
			} else 
			{
				// Server or no hologram - run territory checks with player position
				RemoveTheBasicMapMarker();
				
				if (thePlayer && thePlayer.GetIdentity()) 
				{
					theGUID = thePlayer.GetIdentity().GetId();
				}
				
				// Use player's current position
				vector checkPos = player.GetPosition();
				return CanIPlaceHere(item, player, checkPos, theGUID);
			}
		}
		
		return false;
	}

	protected bool ShouldBlockDeployActionAtPlayerPosition(PlayerBase player, ItemBase item, string guid)
	{
		if (!player || !item)
		{
			return false;
		}

		if (GetMultiTerritoriesConfig().IsInWhiteList(item.GetType()))
		{
			return false;
		}

		TerritoryFlag territory = TerritoryFlag.GetTerritoryAtPos(player.GetPosition(), item.IsInherited(REV_TerritoryComputerKit));
		if (!territory)
		{
			return false;
		}

		if (item.IsInherited(REV_TerritoryComputerKit))
		{
			return false;
		}

		if (territory.CheckPlayerPermission(guid, TerritoryPerm.DEPLOY))
		{
			return false;
		}

		return true;
	}
	
	override void OnEndClient(ActionData action_data)
	{
		super.OnEndClient(action_data);
	}
	
	void RemoveTheBasicMapMarker()
	{
		#ifdef BASICMAP
		if (GetGame() && GetGame().IsClient() && BASICT_Marker && BASICT_Marker.GetPosition() != vector.Zero)
		{
			BasicMap().RemoveMarker(BasicMap().CLIENT_KEY, BASICT_Marker);
			BASICT_Marker.SetPosition(vector.Zero);
			BASICT_Marker.SetRadius(0);
		}
		#endif
	}
	
	protected bool CanIPlaceHere(EntityAI kit, EntityAI item, vector pos, string GUID = "")
	{
		m_LastCheckLocation = pos;
		if (pos == vector.Zero || !item || !kit)
		{
			m_CanPlaceHere = false;
			return m_CanPlaceHere;
		} else 
		if (!GetMultiTerritoriesConfig().CanBuildHere(pos, item.GetType()) || !GetMultiTerritoriesConfig().CanBuildHere(pos, kit.GetType()))
		{
			GetMultiTerritoriesConfig().SendNotification(GetMultiTerritoriesConfig().NoBuildZoneMessage, TerritoryIcons.NoBuildZone);
			m_CanPlaceHere = false;
			return m_CanPlaceHere;
		} else 
		if (GetMultiTerritoriesConfig().IsInWhiteList(item.GetType()) || GetMultiTerritoriesConfig().IsInWhiteList(kit.GetType()))
		{
			m_CanPlaceHere = true;
			return m_CanPlaceHere;
		} else 
		{
			array<Object> objects = new array<Object>;

			if (kit.GetType() == "Cabin_Kit")
			{
				GetGame().GetObjectsAtPosition(pos, GetMultiTerritoriesConfig().GetProtectionRadius() * 1.2, objects, null);
				Object obj;
				for (int x = 0; x < objects.Count(); x++)
				{
					obj = objects.Get(x);
					if (obj && obj.GetType() == "Prefab_Cabin")
					{
						GetMultiTerritoriesConfig().SendNotification("Sorry, you can only build one cabin per territory!", TerritoryIcons.NoBuildZone);
						m_CanPlaceHere = false;
						return m_CanPlaceHere;
					}
				}
				objects.Clear();
			};
			
			TerritoryFlag territory = TerritoryFlag.GetTerritoryAtPos(pos, kit.IsInherited(REV_TerritoryComputerKit));
			REV_TerritoryComputer computer = REV_TerritoryComputer.Cast(territory);
			if (territory)
			{
				if (kit.IsInherited(REV_TerritoryComputerKit))
				{
					GetMultiTerritoriesConfig().SendNotification(GetMultiTerritoriesConfig().TerritoryConflictMessage, TerritoryIcons.TerritoryConflict);
					m_CanPlaceHere = false;
					return m_CanPlaceHere;
				}

				if (GetGame().IsClient())
				{
					territory.SyncTerritoryRateLimited();
				}

				m_CanPlaceHere = territory.CheckPlayerPermission(GUID, TerritoryPerm.DEPLOY);
				if (!m_CanPlaceHere)
				{
					GetMultiTerritoriesConfig().SendNotification(GetMultiTerritoriesConfig().WithinTerritoryWarning, TerritoryIcons.WithinTerritory);
				}
				return m_CanPlaceHere;
			}

			if (kit && kit.IsInherited(REV_TerritoryComputerKit))
			{
				m_CanPlaceHere = true;
				return m_CanPlaceHere;
			}
		}
		if (GetMultiTerritoriesConfig().RequireTerritory)
		{
			GetMultiTerritoriesConfig().SendNotification(GetMultiTerritoriesConfig().TerritoryRequiredWarningMessage, TerritoryIcons.TerritoryRequiredWarning);
			m_CanPlaceHere = false;
		} else 
		{
			string DeSpawnWarningMessage = GetMultiTerritoriesConfig().DeSpawnWarningMessage;
			int LifeTime = 0;
			int itemLifetime = 0;
			ItemBase theItem;
			ItemBase theKit;
			string ItemName = kit.GetType();
			
			theItem = ItemBase.Cast(item);
			if (theItem)
			{
				itemLifetime = GetMultiTerritoriesConfig().GetKitLifeTime(theItem.GetType());
				if (itemLifetime <= 0)
				{
					itemLifetime = theItem.GetTSyncedLifeTime();
				}
			}

			theKit = ItemBase.Cast(kit);
			if (theKit)
			{
				LifeTime = GetMultiTerritoriesConfig().GetKitLifeTime(theKit.GetType());
				if (LifeTime <= 0)
				{
					LifeTime = theKit.GetTSyncedLifeTime();
				}
				ItemName = theKit.GetDisplayName();
			}

			if (itemLifetime > LifeTime)
			{
				LifeTime = itemLifetime;
				if (theItem)
				{
					ItemName = theItem.GetDisplayName();
				}
			} 

			string NiceExpireTime = GetMultiTerritoriesConfig().NiceExpireTime(LifeTime);
			if (NiceExpireTime != "" && GetMultiTerritoriesConfig().ShouldWarnOutsideTerritoryBuild())
			{
				DeSpawnWarningMessage.Replace("$LIFETIME$", NiceExpireTime);
				DeSpawnWarningMessage.Replace("$ITEMNAME$", ItemName); 
				GetMultiTerritoriesConfig().SendNotification(DeSpawnWarningMessage, TerritoryIcons.DeSpawnWarning);
			}

			m_CanPlaceHere = true;
		}

		return m_CanPlaceHere;
	}
};

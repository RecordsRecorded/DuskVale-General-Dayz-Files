modded class PlayerBase extends ManBase
{
    protected bool CanStartPlacementInCurrentPosition(ItemBase item)
    {
        if (!item || !GetIdentity())
        {
            return true;
        }

        if (GetMultiTerritoriesConfig().IsInWhiteList(item.GetType()))
        {
            return true;
        }

        TerritoryFlag territory = TerritoryFlag.GetTerritoryAtPos(GetPosition(), item.IsInherited(REV_TerritoryComputerKit));
        if (!territory)
        {
            return true;
        }

        if (item.IsInherited(REV_TerritoryComputerKit))
        {
            return true;
        }

        string guid = GetIdentity().GetId();
        bool territoryActive = true;
        REV_TerritoryComputer computer = REV_TerritoryComputer.Cast(territory);
        if (computer)
        {
            territoryActive = computer.IsTurnedOn() && computer.IsFloppyLocked() && computer.HasValidFloppy();
        }
        else
        {
            territoryActive = territory.HasRaisedFlag();
        }

        if (!territoryActive)
        {
            return true;
        }

        if (GetMultiTerritoriesConfig().ServerAdmins.Find(guid) != -1)
        {
            return true;
        }

        if (territory.IsTerritoryOwner(guid))
        {
            return true;
        }

        if (territory.m_TerritoryMembers && territory.m_TerritoryMembers.Check(guid, TerritoryPerm.DEPLOY))
        {
            return true;
        }

        int publicPerms = GetMultiTerritoriesConfig().PublicPermission();
        return (publicPerms & TerritoryPerm.DEPLOY) == TerritoryPerm.DEPLOY;
    }

    void Multi_SendMessage(string message)
    {
        Param1<string> m_MessageParam = new Param1<string>("");
        if (GetGame().IsServer() && m_MessageParam && IsAlive() && !IsPlayerDisconnected() && message != "")
        {
            m_MessageParam.param1 = message;
            GetGame().RPCSingleParam(this, ERPCs.RPC_USER_ACTION_MESSAGE, m_MessageParam, true, GetIdentity());
        }
    }

    void Multi_DisplayClientMessage(string message)
    {
        if (GetGame().GetPlayer())
        {
            GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", message, ""));
        }
    }

    override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
    {
        super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

        if (source && GetIdentity())
        {
            PlayerBase player = PlayerBase.Cast(source);

            if (!player)
            {
                player = PlayerBase.Cast(source.GetHierarchyRootPlayer());
            }

            if (player && player.GetIdentity())
            {
                if (player.GetIdentity().GetPlainId() != this.GetIdentity().GetPlainId())
                    MultiModLogger.Log("PLAYER PVP HIT: " + player.GetIdentity().GetName() + " (" + player.GetIdentity().GetPlainId() + ") hit " + this.GetIdentity().GetName() + " (" + this.GetIdentity().GetPlainId() + ") with " + ammo + " (health=" + this.GetHealth().ToString() + ")", "gunfight");
            }
        }
    }

    override void EEKilled(Object killer)
    {
        super.EEKilled(killer);

        if (!GetIdentity())
            return;

        LogPlayerDeathItems(this);

        if (!killer)
        {
            MultiModLogger.Log("Player death: " + this.GetIdentity().GetName() + " (" + this.GetIdentity().GetPlainId() + ") was killed by a null object / unknown (pos=" + GetPosition() + ")", "pvp");
            return;
        }

        PlayerBase player = PlayerBase.Cast(killer);
        if (player && player.GetIdentity())
        {
            MultiModLogger.Log("PLAYER PVP KILL: " + player.GetIdentity().GetName() + " (" + player.GetIdentity().GetPlainId() + ") killed " + this.GetIdentity().GetName() + " (" + player.GetIdentity().GetPlainId() + ") with " + killer.GetType() + " (pos=" + GetPosition() + ") (killerHealth=" + player.GetHealth().ToString() + ")", "pvp");
            return;
        }

        EntityAI killerAI = EntityAI.Cast(killer);
        if (killerAI)
        {
            Man man = killerAI.GetHierarchyRootPlayer();

            if (!man)
            {
                MultiModLogger.Log("Player death: " + player.GetIdentity().GetName() + " (" + player.GetIdentity().GetPlainId() + ") killed " + this.GetIdentity().GetName() + " (" + player.GetIdentity().GetPlainId() + ") with " + killer.GetType() + " (pos=" + GetPosition() + ") (killerHealth=" + player.GetHealth().ToString() + ")", "pvp");
                return;
            }

            player = PlayerBase.Cast(man);
            if (player && player.GetIdentity())
            {
                MultiModLogger.Log("PLAYER PVP KILL: " + player.GetIdentity().GetName() + " (" + player.GetIdentity().GetPlainId() + ") killed " + this.GetIdentity().GetName() + " (" + player.GetIdentity().GetPlainId() + ") with " + killer.GetType() + " (pos=" + GetPosition() + ") (killerHealth=" + player.GetHealth().ToString() + ")", "pvp");
                return;
            }
        }

        MultiModLogger.Log("Player death: " + this.GetIdentity().GetName() + " (" + this.GetIdentity().GetPlainId() + ") was killed by " + killer.GetType() + " (pos=" + GetPosition() + ")", "pvp");
    }

    void LogPlayerDeathItems(PlayerBase player)
    {
        if (!player || !player.GetIdentity())
            return;

        string logMsg = "items=";
        array<EntityAI> itemsArray = new array<EntityAI>;
        player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, itemsArray);

        if (itemsArray.Count() == 0)
        {
            logMsg = "No items detected";
        }
        else
        {
            for (int i = 0; i < itemsArray.Count(); i++)
            {
                EntityAI item = itemsArray.Get(i);
                if (item && !item.GetType().Contains("SurvivorM") && !item.GetType().Contains("SurvivorF"))
                {
                    float quant = item.GetQuantity();

                    Ammunition_Base ammo = Ammunition_Base.Cast(item);
                    if (ammo)
                    {
                        quant = ammo.GetAmmoCount();
                    }

                    logMsg = logMsg + "\"" + item.GetType() + "(" + item.GetQuantity() + "/" + item.GetHealth() + ")\",";
                }
            }
        }

        MultiModLogger.Log("PlayerInventory (" + player.GetIdentity().GetPlainId() + "/" + player.GetIdentity().GetName() + ")\n" + logMsg, "pvp");
    }

    override void PlacingStartLocal(ItemBase item)
	{
		if ( GetGame().IsMultiplayer() && GetGame().IsServer() )
			return;

        if (!CanStartPlacementInCurrentPosition(item))
        {
            GetMultiTerritoriesConfig().SendNotification(GetMultiTerritoriesConfig().WithinTerritoryWarning, TerritoryIcons.WithinTerritory);
            return;
        }

        super.PlacingStartLocal( item );
		
		if ( item && item.IsInherited(REV_KitBase) )
        {
            Print("[ElysianTerritoriesPlus][INFO] Setting object texture to Void Texture");
            item.SetObjectTexture( 0, item.GetVoidTextureName() );
        }
            
	}

    override void PlacingCancelLocal()
    {
        super.PlacingCancelLocal();

        if ( GetGame().IsMultiplayer() && GetGame().IsServer() )
			return;

        ItemBase item = ItemBase.Cast( GetHumanInventory().GetEntityInHands() );
        if ( item && item.IsInherited(REV_KitBase) )
        {
            item.SetObjectTexture( 0, item.GetSourceTextureName() );
        }
    }
}

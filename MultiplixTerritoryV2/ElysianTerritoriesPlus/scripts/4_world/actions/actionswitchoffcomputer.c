class ActionTurnOffTerritoryComputer: ActionInteractBase
{

    void ActionTurnOffTerritoryComputer()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
        m_Text = "Switch Off";
    }

    protected bool CanSwitchOff(PlayerBase player, REV_TerritoryComputer territoryComputer)
    {
        if (!player || !player.GetIdentity() || !territoryComputer)
        {
            return false;
        }

        return territoryComputer.CanReceiveNewOwner() || territoryComputer.CheckPlayerPermission(player.GetIdentity().GetId(), TerritoryPerm.ADMIN);
    }

    override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
    {
        Object targetObject = target.GetObject();
        if (!targetObject)
        {
            return false;
        }

		ItemBase target_IB = ItemBase.Cast( target.GetObject() );

		int component_id = target.GetComponentIndex();
		string selection = targetObject.GetActionComponentName(target.GetComponentIndex());

        REV_TerritoryComputer territoryComputer = REV_TerritoryComputer.Cast(target.GetObject());

        if ( territoryComputer && territoryComputer.IsTurnedOn() == true && CanSwitchOff(player, territoryComputer) && REV_TerritoryComputer.IsPrimaryInteractionSelection(selection) )
        {
            return true;
        }

        return false;
    }

    override void OnExecute( ActionData action_data )
    {
        super.OnExecute(action_data);

        REV_TerritoryComputer target = REV_TerritoryComputer.Cast( action_data.m_Target.GetObject() );
        if (target && CanSwitchOff(action_data.m_Player, target))
        {
            target.SwitchPowerOff();

            Print("[ElysianTerritories][INFO] ActionTurnOffTerritoryComputer executed");
        }
    }
    override void OnExecuteClient( ActionData action_data )
    {
        super.OnExecuteClient(action_data);

        REV_TerritoryComputer target = REV_TerritoryComputer.Cast( action_data.m_Target.GetObject() );
        if (target)
        {
            target.ClientPreviewPowerOff();
            target.PlayPowerOffSounds();

            Print("[ElysianTerritories][INFO] ActionTurnOffTerritoryComputer executed on client");
        }
    }
};

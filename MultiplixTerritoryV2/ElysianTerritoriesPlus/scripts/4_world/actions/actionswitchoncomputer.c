class ActionTurnOnTerritoryComputer: ActionInteractBase
{

    void ActionTurnOnTerritoryComputer()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
        m_Text = "Switch On";
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

        if ( territoryComputer && territoryComputer.IsTurnedOn() == false && REV_TerritoryComputer.IsPrimaryInteractionSelection(selection) )
        {
            return true;
        }

        return false;
    }

    override void OnExecute( ActionData action_data )
    {
        super.OnExecute(action_data);

        REV_TerritoryComputer target = REV_TerritoryComputer.Cast( action_data.m_Target.GetObject() );
        if (target)
        {
            target.SwitchPowerOn();

            Print("[ElysianTerritories][INFO] ActionTurnOnTerritoryComputer executed");
        }
    }
    override void OnExecuteClient( ActionData action_data )
    {
        super.OnExecuteClient(action_data);

        REV_TerritoryComputer target = REV_TerritoryComputer.Cast( action_data.m_Target.GetObject() );
        if (target)
        {
            target.ClientPreviewPowerOn();
            target.PlayPowerOnSounds();

            Print("[ElysianTerritories][INFO] ActionTurnOnTerritoryComputer executed on client");
        }
    }
};

modded class ActionConstructor
{
    override void RegisterActions(TTypenameArray actions)
    {
        super.RegisterActions(actions);

        actions.Insert(ActionTurnOnTerritoryComputer);
        actions.Insert(ActionTurnOffTerritoryComputer);
        actions.Insert(REV_ActionDismantleObject);
		actions.Insert(ActionAddTerritoryMember);
		actions.Insert(ActionResetTerritoryMembers);
		actions.Insert(ActionAcceptMembership);
		actions.Insert(ActionOpenTerritoryComputerMenu);
        actions.Insert(ActionInsertFloppy);
    }
}

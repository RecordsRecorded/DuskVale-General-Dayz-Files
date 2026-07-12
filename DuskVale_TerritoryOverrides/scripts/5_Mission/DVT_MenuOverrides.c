// DuskVale Territory Overrides - mission scripts.
//
// Hide the EJECT button in the territory computer menu for everyone except
// the territory owner (or a server admin). The server-side RPC gate in
// 4_World is the authoritative check; this just keeps the UI honest.
modded class MultiplixTerritoryComputerMenu
{
	override protected void RefreshCommandBar(bool canClaim, bool canResetAdmin, bool canResetMembers, bool canInvite, bool inviteActive, bool canJoin, bool canLeave, bool canEjectFloppy)
	{
		REV_TerritoryComputer targetComputer = GetTargetComputer();
		if (targetComputer)
		{
			string selfGuid = GetPlayerGuid();
			bool isOwner = targetComputer.IsTerritoryOwner(selfGuid);
			bool isServerAdmin = GetMultiTerritoriesConfig().ServerAdmins.Find(selfGuid) != -1;
			bool hasFloppyAttached = targetComputer.FindAttachmentBySlotName("Material_FPole_Flag") != null;
			canEjectFloppy = (canClaim || isOwner || isServerAdmin) && hasFloppyAttached;
		}

		super.RefreshCommandBar(canClaim, canResetAdmin, canResetMembers, canInvite, inviteActive, canJoin, canLeave, canEjectFloppy);
	}
}

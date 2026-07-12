class LBConsoleCommandModifierGodmode : LBConsoleCommandModifierBase {

	[LBConsoleCommandHandler.Get().Register("LBConsoleCommandModifierGodmode")]
	override void Register() {
		super.Register();
		prefix = "godmode";
		modifier = LBPlayerModifier.GODMODE;
	}

}
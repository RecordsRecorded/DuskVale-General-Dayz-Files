class LBConsoleCommandModifierInvisible : LBConsoleCommandModifierBase {

	[LBConsoleCommandHandler.Get().Register("LBConsoleCommandModifierInvisible")]
	override void Register() {
		super.Register();
		prefix = "invisible";
		modifier = LBPlayerModifier.INVISIBLE;
	}

}
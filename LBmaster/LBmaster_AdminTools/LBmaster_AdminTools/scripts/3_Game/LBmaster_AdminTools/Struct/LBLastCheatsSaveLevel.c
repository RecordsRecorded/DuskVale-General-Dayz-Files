enum LBLastCheatsSaveLevel {

	NEVER = 0, // Never restore the information
	RELOG = 1, // Restore it when logging back into the same server. Only saved for the last server, so joining another server will reset the last enabled modifiers
	GAME_CRASH = 2, // Restore when logging back into the same server within 5 minutes of disconnecting. Server restarts or connecting to another server will wipe this information
	JOINED = 3, // Every time you join a server. Doesn't matter which server
	ADMIN_MODE_ENABLED = 4 // Every time you enable admin mode on a server

}
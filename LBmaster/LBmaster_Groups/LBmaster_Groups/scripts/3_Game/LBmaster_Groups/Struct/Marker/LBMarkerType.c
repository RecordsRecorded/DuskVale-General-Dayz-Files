enum LBMarkerType {

	SERVER_STATIC = 0, // Static Marker (persistent over restarts)
	SERVER_DYNAMIC, // Static Markers, but for events and deleted after restart
	GROUP_PING, // Tactical-Ping Markers
	GROUP_MARKER, // Normal Markers placed for all Group Members
	GROUP_PLAYER_MARKER, // The Marker above Players inside the Group
	PRIVATE_MARKER // Privats Markers only visible to the player who placed it and saved clientside

}
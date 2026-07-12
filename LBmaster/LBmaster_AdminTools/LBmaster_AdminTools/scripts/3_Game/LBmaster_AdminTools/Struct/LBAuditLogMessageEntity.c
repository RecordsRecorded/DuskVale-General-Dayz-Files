class LBAuditLogMessageEntity {

	string classname; // itemname of the entity
	vector position; // Position of the entity
	string steamid; // Steamid if the entity is a player

	void LBAuditLogMessageEntity(Object target) {
		if (!target)
			return;
		classname = target.GetType();
		position = target.GetPosition();
		Man man = Man.Cast(target);
		if (man && man.GetIdentity())
			steamid = man.GetIdentity().GetPlainId();
	}

}
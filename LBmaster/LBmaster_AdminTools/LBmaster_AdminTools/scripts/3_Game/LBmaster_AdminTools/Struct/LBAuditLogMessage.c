class LBAuditLogMessage {

	int timestamp; // Unix timestamp of the action
	string timestampString; // Timestamp in human readable form
	string action; // Name of the action performed
	string message; // Message from the audit log system
	vector position; // Position where the action was performed
	ref array<ref LBAuditLogMessageEntity> targets = new array<ref LBAuditLogMessageEntity>(); // List of items or positions related to the action

	void LBAuditLogMessage(string action_, string message_, vector position_, array<Object> targets_) {
		LBDate date = LBDate.Init(true);
		timestamp = date.GetTimestamp();
		timestampString = date.ToFormattedString();
		action = action_;
		message = message_;
		position = position_;
		if (targets_) {
			foreach (Object target : targets_)
				targets.Insert(new LBAuditLogMessageEntity(target));
		}
	}

}
class LBAdminAuditLogConfig : LBConfigLoader<LBAdminAuditLogConfig_> {

	override void InitVars() {
		InitVarsInternal("AdminTool", "auditlog.json", LBConfigType.CONFIG, true, "admin.change.auditlogsettings", "admin.change.auditlogsettings"); // (easy)
	}

	override string GetRequiredVersion() {
		return "Admin_Tools";
	}

}
// Settings for the Admin Audit Log
class LBAdminAuditLogConfig_ : LBConfigBase {

	static const int CURRENT_VERSION = 2;
	bool enabled = true; // Toggle if any audit logs should be written at all
	int keepLogsTime = 604800; // Time in seconds until audit log entries are deleted. Default is 7 days
	ref map<string, bool> loggedActions = new map<string, bool>(); // List of all AuditLog actions to enable/disable logging to only log what is needed

}
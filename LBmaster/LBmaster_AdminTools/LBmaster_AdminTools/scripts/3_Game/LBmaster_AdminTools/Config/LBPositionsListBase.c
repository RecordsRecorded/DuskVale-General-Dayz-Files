class LBPositionsListBase : LBConfigBase {

	array<ref LB_TeleportPos> GetPositions();
	void RemovePosition(LB_TeleportPos pos);
	void AddPosition(LB_TeleportPos pos);
	void ChangePosition(LB_TeleportPos pos, LB_TeleportPos old);
	void Save(LB_TeleportPos pos);
}
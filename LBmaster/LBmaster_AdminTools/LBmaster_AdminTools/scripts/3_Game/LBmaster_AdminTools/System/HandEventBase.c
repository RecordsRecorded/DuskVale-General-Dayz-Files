modded class HandEventBase {

	vector playerPos;

	override void ReadFromContext(ParamsReadContext ctx) {
		super.ReadFromContext(ctx);
		ctx.Read(playerPos);
	}

	override void WriteToContext(ParamsWriteContext ctx) {
		super.WriteToContext(ctx);
		vector pos = vector.Zero;
		if (m_Player)
			pos = m_Player.GetPosition();
		ctx.Write(pos);
	}

}
class LBObjectBuilderObject {

	string classname;
	vector position;
	vector orientation;
	float scale;
	private bool enabled;
	bool locked = false;
	[NonSerialized()]
	Object object;

	void LBObjectBuilderObject(string classname_, vector position_, vector orientation_, float scale_, bool enabled_) {
		this.classname = classname_;
		this.position = position_;
		this.orientation = orientation_;
		this.scale = scale_;
		this.enabled = enabled_;
	}

	void WriteToFile(Serializer serial) {
		serial.Write(classname);
		serial.Write(position[0]);
		serial.Write(position[1]);
		serial.Write(position[2]);
		serial.Write(orientation[0]);
		serial.Write(orientation[1]);
		serial.Write(orientation[2]);
		serial.Write(scale);
		serial.Write(enabled);
		serial.Write(locked);
	}

	bool ReadFromFile(Serializer serial, int version) {
		if (version == 3) {
			int dummy;
			return serial.Read(dummy) && serial.Read(classname) && serial.Read(position) && serial.Read(orientation) && serial.Read(scale) && serial.Read(enabled);
		}
		if (!serial.Read(classname))
			return false;
		if (!ReadVector(serial, position))
			return false;
		if (!ReadVector(serial, orientation))
			return false;
		if (!serial.Read(scale))
			return false;
		if (!serial.Read(enabled))
			return false;
		if (version > 4 && !serial.Read(locked))
			return false;
		return true;
	}

	bool ReadVector(Serializer serial, out vector vec) {
		float x, y, z;
		if (!serial.Read(x) || !serial.Read(y) || !serial.Read(z))
			return false;
		vec = Vector(x, y, z);
		return true;
	}

	bool IsEnabled() {
		return enabled;
	}

	void SetEnabled(bool enabled_) {
		this.enabled = enabled_;
		Spawn("");
	}

	void Spawn(string setName) {
		Despawn();
		if (!enabled)
			return;
#ifndef SERVER
		if (!LBItemHelper.CanSpawnLocalPreview(classname))
			return;
#endif
		int flags = ECE_CREATEPHYSICS | ECE_UPDATEPATHGRAPH | ECE_KEEPHEIGHT | ECE_SETUP | ECE_NOSURFACEALIGN | ECE_LOCAL | ECE_DYNAMIC_PERSISTENCY | ECE_NOLIFETIME;
		object = g_Game.CreateObjectEx(classname, position, flags);
		if (!object) {
#ifdef SERVER
			LBLogger.Error("Could not spawn building " + classname + " at " + position + " in building set " + setName, "AdminTools");
#endif
			return;
		}
#ifdef DIAG
		LBLogger.Debug("Sucessfully spawned building " + classname + " at " + position + " in building set " + setName, "AdminTools");
#endif
		object.SetPosition(position);
		object.SetOrientation(orientation);
		if (scale != 1.0)
			object.SetScale(scale);
#ifdef SERVER
		if (object.CanAffectPathgraph()) {
			object.SetAffectPathgraph(true, false);
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(g_Game.UpdatePathgraphRegionByObject, 100, false, object);
		}
		g_Game.RemoteObjectTreeCreate(object);
		EntityAI ent;
		if (Class.CastTo(ent, object) && ent.ConfigGetInt("scope") == 2) {
			ent.SetTakeable(false);
			if (!ent.IsTakeable()) {
				EnScript.SetClassVar(ent, "m_IsHologram", 0, true);
				ent.SetSynchDirty();
			}
			ent.SetAllowDamage(false);
		}
#endif
		object.Update();
	}

	void Despawn() {
		if (g_Game && object) {
#ifdef SERVER
			LBLogger.Debug("Deleted Static Building: " + classname + " at " + object.GetPosition(), "AdminTools");
#endif
			g_Game.ObjectDelete(object);
		}
	}

}
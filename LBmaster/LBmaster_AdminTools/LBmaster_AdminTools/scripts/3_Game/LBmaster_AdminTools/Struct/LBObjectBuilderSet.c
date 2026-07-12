class LBObjectBuilderSet {

	private static const int VERSION = 5;

	string name;
	private bool enabled;
	ref array<ref LBObjectBuilderObject> objects = new array<ref LBObjectBuilderObject>();

	void LBObjectBuilderSet(string name_, bool enabled_) {
		this.name = name_;
		this.enabled = enabled_;
	}

	void ~LBObjectBuilderSet() {
		if (g_Game) {
			Despawn();
		}
	}

	void Rename(string folder, string newName) {
		Delete(folder);
		this.name = newName;
		Save(folder);
	}

	void Save(string folder) {
		if (!FileExist(folder))
			LBmasterConfigMover.CreateFolders(folder);
		FileSerializer serial = new FileSerializer();
		if (serial.Open(folder + name + ".lbset", FileMode.WRITE)) {
			LBObjectBuilderSet set_ = this;
			set_.WriteToFile(serial);
			serial.Close();
		} else {
			LBLogger.Error("Could not open File Serializer to save Building Set " + name, "AdminTools");
		}
	}

	void WriteToFile(Serializer serial) {
		serial.Write(VERSION);
		serial.Write(name);
		serial.Write(enabled);
		int count = objects.Count();
		serial.Write(count);
		foreach (LBObjectBuilderObject object : objects)
			object.WriteToFile(serial);
	}

	bool ReadFromFile(Serializer serial, out int version) {
		if (!serial.Read(version))
			return false;
		if (!serial.Read(name))
			return false;
		if (!serial.Read(enabled))
			return false;
		int count;
		if (!serial.Read(count))
			return false;
		for (int i = 0; i < count; ++i) {
			LBObjectBuilderObject obj = new LBObjectBuilderObject("", vector.Zero, vector.Zero, 1, false);
			if (!obj.ReadFromFile(serial, version))
				return false;
			objects.Insert(obj);
		}
		return true;
	}

	void Delete(string folder) {
		if (!FileExist(folder + name + ".lbset"))
			return;
		if (!FileExist(folder + "Deleted"))
			MakeDirectory(folder + "Deleted");
		int i = 0;
		while (FileExist(folder + "Deleted/" + name + "(" + i + ").lbset"))
			++i;
		CopyFile(folder + name + ".lbset", folder + "Deleted/" + name + "(" + i + ").lbset");
		DeleteFile(folder + name + ".lbset");
	}

	static LBObjectBuilderSet Load(string file) {
		if (!FileExist(file))
			return null;
		LBObjectBuilderSet set_ = new LBObjectBuilderSet("", false);
		FileSerializer serial = new FileSerializer();
		if (serial.Open(file, FileMode.READ)) {
			int version;
			if (set_.ReadFromFile(serial, version)) {
				serial.Close();
				LBLogger.Info("Sucessfully Loaded Building Set: " + file + " (" + set_.name + ") Version " + version, "AdminTools");
				return set_;
			}
			LBLogger.Error("Could not read Building Set " + file, "AdminTools");
			serial.Close();
			return null;
		}
		LBLogger.Error("Could not open File Serializer for Building Set " + file, "AdminTools");
		return null;
	}

	bool IsEnabled() {
		return enabled;
	}

	void SetEnabled(bool enabled_) {
		this.enabled = enabled_;
		if (enabled)
			Spawn();
		else
			Despawn();
	}

	void Despawn() {
		if (!objects)
			return;
		foreach (LBObjectBuilderObject object : objects) {
			object.Despawn();
		}
	}

	void Spawn() {
#ifdef SERVER
		if (!enabled)
			return;
#endif
		LBLogger.Info("Spawning Object builder set: " + name + " with " + objects.Count() + " items", "AdminTools");
		foreach (LBObjectBuilderObject object : objects) {
			object.Spawn(name);
		}
	}

}
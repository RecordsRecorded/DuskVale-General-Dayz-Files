class LBFOWSector {

	int x1, y1, x2, y2;
	bool deleted = false;

	void LBFOWSector(int x1_, int y1_, int x2_, int y2_) {
		this.x1 = x1_;
		this.y1 = y1_;
		this.x2 = x2_;
		this.y2 = y2_;
	}

	vector GetFrom(float sectorSize) {
		return Vector(x1 * sectorSize, 0, y1 * sectorSize);
	}

	vector GetTo(float sectorSize) {
		return Vector((x2 + 1) * sectorSize, 0, (y2 + 1) * sectorSize);
	}

}
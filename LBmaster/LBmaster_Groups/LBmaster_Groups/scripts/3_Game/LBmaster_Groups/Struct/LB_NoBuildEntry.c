class LB_NoBuildEntry {

	string name; // Name of this position. This will replace the `{pos}` placeholder when sending the error message to the player
	float x; // X position of the Zone
	float y; // Y position (or rather the Z coordinate) of the Zone
	float r; // Radius of the Zone

	bool InZone(vector v, float additionalRadius = 0.0) {
		float xDiff = v[0] - x;
		float yDiff = v[2] - y;
		float dist = Math.Sqrt(xDiff * xDiff + yDiff * yDiff);
		return dist <= r + additionalRadius;
	}
}
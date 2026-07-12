class LBNamedWeatherPreset : LBWeatherPreset {

	string name;

	static LBNamedWeatherPreset Init(string name_, Param3<float, float, float> weatherValues, Param4<float, float, float, float> fogValues, Param2<float, float> windValues, float time, float duration) {
		LBNamedWeatherPreset preset = new LBNamedWeatherPreset();
		preset.name = name_;
		if (weatherValues) {
			preset.overcast = weatherValues.param1;
			preset.rain = weatherValues.param2;
			preset.snowfall = weatherValues.param3;
		} else {
			preset.overcast = -1;
			preset.rain = -1;
			preset.snowfall = -1;
		}
		if (fogValues) {
			preset.fog = fogValues.param1;
			preset.volFogDensity = fogValues.param2;
			preset.volFogHeight = fogValues.param3;
			preset.volFogBias = fogValues.param4;
		} else {
			preset.fog = -1;
			preset.volFogDensity = -1;
			preset.volFogHeight = -1;
			preset.volFogBias = -1;
		}
		if (windValues) {
			preset.windMag = windValues.param1;
			preset.windDir = windValues.param2;
		} else {
			preset.windMag = -1;
			preset.windDir = -1;
		}
		preset.defaultTime = time;
		preset.defaultDuration = duration;
		return preset;
	}

}
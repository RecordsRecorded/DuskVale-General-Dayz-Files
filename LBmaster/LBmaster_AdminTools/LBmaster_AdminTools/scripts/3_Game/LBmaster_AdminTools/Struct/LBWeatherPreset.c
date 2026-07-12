class LBWeatherPreset {

	float overcast;
	float rain;
	float snowfall;
	float fog;
	float volFogDensity;
	float volFogHeight;
	float volFogBias;
	float windMag;
	float windDir;

	float defaultTime;
	float defaultDuration;

	string LBToString() {
		string part1 = "{overcast=" + overcast + ", rain=" + rain + ", snowfall=" + snowfall + ", fog=" + fog + ", fogDensity=" + volFogDensity + ", fogHeight=";
		return part1 + volFogHeight + ", fogBias=" + volFogBias + ", windMag=" + windMag + ", windDir=" + windDir + ", time=" + defaultTime + ", duration=" + defaultDuration + "}";
	}

	void FromEditValues(EditBoxWidget edit_overcast, EditBoxWidget edit_rain, EditBoxWidget edit_snowfall, EditBoxWidget edit_fog, EditBoxWidget edit_fog_dens, EditBoxWidget edit_fog_height, EditBoxWidget edit_fog_bias, EditBoxWidget edit_wind, EditBoxWidget edit_winddir, EditBoxWidget edit_duration, EditBoxWidget edit_fade_time) {
		overcast = edit_overcast.GetText().ToFloat();
		rain = edit_rain.GetText().ToFloat();
		snowfall = edit_snowfall.GetText().ToFloat();
		fog = edit_fog.GetText().ToFloat();
		volFogDensity = edit_fog_dens.GetText().ToFloat();
		volFogHeight = edit_fog_height.GetText().ToFloat();
		volFogBias = edit_fog_bias.GetText().ToFloat();
		windMag = edit_wind.GetText().ToFloat();
		windDir = edit_winddir.GetText().ToFloat();
		defaultTime = edit_fade_time.GetText().ToFloat();
		defaultDuration = edit_duration.GetText().ToFloat();
	}

	void SetEditValues(EditBoxWidget edit_overcast, EditBoxWidget edit_rain, EditBoxWidget edit_snowfall, EditBoxWidget edit_fog, EditBoxWidget edit_fog_dens, EditBoxWidget edit_fog_height, EditBoxWidget edit_fog_bias, EditBoxWidget edit_wind, EditBoxWidget edit_winddir, EditBoxWidget edit_duration, EditBoxWidget edit_fade_time) {
		edit_overcast.SetText("" + overcast);
		edit_rain.SetText("" + rain);
		edit_snowfall.SetText("" + snowfall);
		edit_fog.SetText("" + fog);
		edit_fog_dens.SetText("" + volFogDensity);
		edit_fog_height.SetText("" + volFogHeight);
		edit_fog_bias.SetText("" + volFogBias);
		edit_wind.SetText("" + windMag);
		edit_winddir.SetText("" + windDir);
		edit_fade_time.SetText("" + defaultTime);
		edit_duration.SetText("" + defaultDuration);
	}

	void SendTo(PlayerIdentity target) {
		g_Game.RPCSingleParam(null, LBAdminToolRPCs.SET_WEATHER, new Param1<ref LBWeatherPreset>(this), true, target);
	}

	void Validate() {

	}

	void CopyFrom(LBWeatherPreset other) {
		overcast = other.overcast;
		rain = other.rain;
		snowfall = other.snowfall;
		fog = other.fog;
		volFogDensity = other.volFogDensity;
		volFogHeight = other.volFogHeight;
		volFogBias = other.volFogBias;
		windMag = other.windMag;
		windDir = other.windDir;
		defaultTime = other.defaultTime;
		defaultDuration = other.defaultDuration;
	}

	void InitCurrent() {
		Weather weather = g_Game.GetWeather();
		overcast = weather.GetOvercast().GetActual();
		rain = weather.GetRain().GetActual();
		snowfall = weather.GetSnowfall().GetActual();
		fog = weather.GetFog().GetActual();
		windMag = weather.GetWindMagnitude().GetActual();
		windDir = weather.GetWindDirection().GetActual();
		volFogDensity = weather.GetDynVolFogDistanceDensity();
		volFogHeight = weather.GetDynVolFogHeightDensity();
		volFogBias = weather.GetDynVolFogHeightBias();
	}

	void InitForecast() {
		Weather weather = g_Game.GetWeather();
		overcast = weather.GetOvercast().GetForecast();
		rain = weather.GetRain().GetForecast();
		snowfall = weather.GetSnowfall().GetForecast();
		fog = weather.GetFog().GetForecast();
		windMag = weather.GetWindMagnitude().GetForecast();
		windDir = weather.GetWindDirection().GetForecast();
	}

	void Apply(bool instant) {
		int time = defaultTime;
		if (instant)
			time = 0;
		Weather weather = g_Game.GetWeather();
		if (rain != -1)
			weather.GetRain().Set(rain, time, defaultDuration);
		if (snowfall != -1)
			weather.GetSnowfall().Set(snowfall, time, defaultDuration);
		if (fog != -1)
			weather.GetFog().Set(fog, time, defaultDuration);
		if (overcast != -1)
			weather.GetOvercast().Set(overcast, time, defaultDuration);
		if (windMag != -1)
			weather.GetWindMagnitude().Set(windMag, time, defaultDuration);
		if (windDir != -1) {
			if (windDir > 180)
				weather.GetWindDirection().Set(windDir * Math.DEG2RAD - Math.PI2, time, defaultDuration);
			else
				weather.GetWindDirection().Set(windDir * Math.DEG2RAD, time, defaultDuration);
		}
		if (volFogDensity != -1)
			weather.SetDynVolFogDistanceDensity(volFogDensity, time);
		if (volFogHeight != -1)
			weather.SetDynVolFogHeightDensity(volFogHeight, time);
		if (volFogBias != -1)
			weather.SetDynVolFogHeightBias(volFogBias, time);
	}

}
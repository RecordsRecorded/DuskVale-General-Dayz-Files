class LBAdminMenuFrame_WeatherManager : LBAdminMenuFrame {

	override string GetTitle() {
		return "#lb_adm_title_environment_manager";
	}

	override string GetButtonIcon() {
		return "LBmaster_AdminTools/gui/icons/weather_time.paa";
	}

	override void GetMinSize(out int width, out int height) {
		width = 400;
		height = 20;
	}

	override void GetInitialSize(out int width, out int height) {
		width = 600;
		height = 400;
	}

	override TStringArray GetRequiredPermissionsOne() {
		return {"admin.change.weather", "admin.change.time"};
	}

	override string GetInfoString() {
		return "#lb_adm_desc_environment_manager";
	}

	TextWidget current_time, current_weather;
	SliderWidget slider_year, slider_month, slider_day, slider_hour, slider_minute, slider_overcast, slider_rain, slider_snowfall, slider_fog, slider_fog_dens;
	SliderWidget slider_fog_height, slider_fog_bias, slider_wind, slider_winddir, slider_duration, slider_fade_time;
	Widget current_overcast, forecast_overcast, current_rain, forecast_rain, current_snowfall, forecast_snowfall, current_fog, forecast_fog, current_fog_dens;
	Widget current_fog_height, current_fog_bias, current_wind, forecast_wind, current_winddir, forecast_winddir;
	EditBoxWidget edit_year, edit_month, edit_day, edit_hour, edit_minute, edit_overcast, edit_rain, edit_snowfall, edit_fog, edit_fog_dens, edit_fog_height;
	EditBoxWidget edit_fog_bias, edit_wind, edit_winddir, edit_duration, edit_fade_time;
	CheckBoxWidget chckbx_preview_time, chckbx_preview_weather;
	TextListboxWidget list_presets, list_weather_presets;
	ButtonWidget btn_apply, btn_save_preset, btn_new_preset, btn_delete_preset, btn_apply_weather, btn_save_weather, btn_new_weather, btn_delete_weather;

	ref LBTimePreset currentTime = new LBTimePreset();
	ref LBTimePreset originalTime = new LBTimePreset();

	ref LBWeatherPreset currentWeather = new LBWeatherPreset();
	ref LBWeatherPreset originalWeather = new LBWeatherPreset();
	ref LBWeatherPreset forecastWeather = new LBWeatherPreset();

	int lastWidth, lastHeight;

	override void UpdateSlow() {
		currentTime.InitCurrent();
		current_time.SetText(" #lb_adm_current_time: " + currentTime.ToDateString());
		if (!chckbx_preview_weather.IsChecked()) {
			UpdateForecast();
			UpdateCurrentWeather();
		}
	}

	override bool OnMouseWheel(Widget w, int x, int y, int wheel) {
		if (super.OnMouseWheel(w, x, y, wheel))
			return true;
		EditBoxWidget edit = EditBoxWidget.Cast(w);
		if (edit) {
			SliderWidget nearSlider = GetSlider(edit, layoutRoot);
			if (nearSlider) {
				OnSliderMouseWheel(nearSlider, x, y, wheel);
			}
			return true;
		}
		SliderWidget slider = SliderWidget.Cast(w);
		if (slider) {
			OnSliderMouseWheel(slider, x, y, wheel);
			return true;
		}
		return false;
	}

	void OnSliderMouseWheel(SliderWidget slider, int x, int y, int wheel) {
		float min = slider.GetMin();
		float max = slider.GetMax();
		float step = slider.GetStep();
		float diff = max - min;
		if (step > 0 && slider != slider_duration && slider != slider_fade_time && slider != slider_winddir) {
			float stepCount = diff / step;
			if (stepCount > 100) {
				step = diff / 100;
			}
		}
		float next = slider.GetCurrent() + wheel * step;
		next = Math.Clamp(next, min, max);
		slider.SetCurrent(next);
		OnChange(slider, x, y, false);
	}

	SliderWidget GetSlider(EditBoxWidget edit, Widget parentWidget) {
		float editX, editY;
		edit.GetScreenPos(editX, editY);
		Widget child = parentWidget.GetChildren();
		while (child) {
			float cX, cY;
			child.GetScreenPos(cX, cY);
			if (Math.AbsFloat(cY - editY) < 5 && SliderWidget.Cast(child))
				return SliderWidget.Cast(child);
			SliderWidget found = GetSlider(edit, child);
			if (found)
				return found;
			child = child.GetSibling();
		}
		return null;
	}

	override bool OnClick(Widget w, int x, int y, int button) {
		if (super.OnClick(w, x, y, button))
			return true;
		if (w == btn_apply) {
			SendApplyDate();
			return true;
		} else if (w == btn_save_preset) {
			LBNamedTimePreset preset = GetSelectedPreset();
			if (!preset)
				return false;
			LBSimpleInputPopup.Get().Show("#lb_save", "#lb_adm_save_your_preset", preset.name, ScriptCaller.Create(SavePreset));
			return true;
		} else if (w == btn_new_preset) {
			LBSimpleInputPopup.Get().Show("#lb_add", "#lb_adm_name_your_new_preset", "", ScriptCaller.Create(CreateNewPreset));
			return true;
		} else if (w == btn_delete_preset) {
			preset = GetSelectedPreset();
			if (!preset)
				return false;
			LBWarningPopup.Get().Show("#lb_delete", LBTranslatedString("#lb_adm_message_delete_building_set", {"%name%", preset.name}).Get(), ScriptCaller.Create(ConfirmDelete));
			return true;
		} else if (w == chckbx_preview_time) {
			if (chckbx_preview_time.IsChecked()) {
				originalTime.InitCurrent();
			} else {
				originalTime.Apply();
			}
			return true;
		} else if (w == chckbx_preview_weather) {
			if (chckbx_preview_weather.IsChecked()) {
				originalWeather.InitCurrent();
			} else {
				originalWeather.Apply(true);
			}
			return true;
		} else if (w == btn_apply_weather) {
			SendApplyWeather();
			return true;
		} else if (w == btn_new_weather) {
			LBSimpleInputPopup.Get().Show("#lb_add", "#lb_adm_name_your_new_preset", "", ScriptCaller.Create(CreateNewWeather));
			return true;
		} else if (w == btn_save_weather) {
			LBNamedWeatherPreset weather = GetSelectedWeather();
			if (!weather)
				return false;
			LBSimpleInputPopup.Get().Show("#lb_save", "#lb_adm_save_your_weather_preset", weather.name, ScriptCaller.Create(SaveWeatherPreset));
			return true;
		} else if (w == btn_delete_weather) {
			weather = GetSelectedWeather();
			if (!weather)
				return false;
			LBWarningPopup.Get().Show("#lb_delete", LBTranslatedString("#lb_adm_message_delete_building_set", {"%name%", weather.name}).Get(), ScriptCaller.Create(ConfirmDeleteWeather));
			return true;
		}
		return false;
	}

	void UpdateCurrentWeather() {
		originalWeather.InitCurrent();
		current_overcast.SetPos(originalWeather.overcast, 0);
		current_rain.SetPos(originalWeather.rain, 0);
		current_snowfall.SetPos(originalWeather.snowfall, 0);
		current_fog.SetPos(originalWeather.fog, 0);
		current_wind.SetPos(originalWeather.windMag / 20.0, 0);
		current_winddir.SetPos(originalWeather.windDir / Math.PI + 0.5, 0);
		current_fog_bias.SetPos(originalWeather.volFogBias / 1000.0, 0);
		current_fog_dens.SetPos(originalWeather.volFogDensity, 0);
		current_fog_height.SetPos(originalWeather.volFogHeight / 2000.0, 0);
	}

	void UpdateForecast() {
		forecastWeather.InitForecast();
		forecast_overcast.SetPos(forecastWeather.overcast, 0);
		forecast_rain.SetPos(forecastWeather.rain, 0);
		forecast_snowfall.SetPos(forecastWeather.snowfall, 0);
		forecast_fog.SetPos(forecastWeather.fog, 0);
		forecast_wind.SetPos(forecastWeather.windMag / 20.0, 0);
		forecast_winddir.SetPos(forecastWeather.windDir / Math.PI + 0.5, 0);
	}

	override bool OnDoubleClick(Widget w, int x, int y, int button) {
		if (super.OnDoubleClick(w, x, y, button))
			return true;
		if (w == list_presets) {
			if (LoadSelectedPreset())
				SendApplyDate();
			return true;
		} else if (w == list_weather_presets) {
			if (LoadSelectedWeather())
				SendApplyWeather();
		}
		return false;
	}

	void SendApplyWeather() {
		currentWeather.FromEditValues(edit_overcast, edit_rain, edit_snowfall, edit_fog, edit_fog_dens, edit_fog_height, edit_fog_bias, edit_wind, edit_winddir, edit_duration, edit_fade_time);
		currentWeather.SendTo(null);
	}

	void CreateNewWeather(string name) {
		LBNamedWeatherPreset newPreset = new LBNamedWeatherPreset();
		newPreset.name = name;
		LBWeatherManagerConfig.Get.weatherPresets.Insert(newPreset);
		LBWeatherManagerConfig.Loader.Save();
		list_weather_presets.AddItem(newPreset.name, newPreset, 0);
		list_weather_presets.SelectRow(list_weather_presets.GetNumItems() - 1);
		NotificationSystem.AddNotificationExtended(5.0, "#lb_adm_admin_tools", LBTranslatedString("#lb_adm_weather_preset_created", {"%name%", name}).Get(), LBIconConfig.Get.info);
	}

	void SaveWeatherPreset(string name) {
		LBNamedWeatherPreset preset = GetSelectedWeather();
		if (!preset)
			return;
		preset.name = name;
		preset.FromEditValues(edit_overcast, edit_rain, edit_snowfall, edit_fog, edit_fog_dens, edit_fog_height, edit_fog_bias, edit_wind, edit_winddir, edit_duration, edit_fade_time);
		LBWeatherManagerConfig.Loader.Save();
		list_weather_presets.SetItem(list_weather_presets.GetSelectedRow(), name, preset, 0);
		NotificationSystem.AddNotificationExtended(5.0, "#lb_adm_admin_tools", LBTranslatedString("#lb_adm_weather_preset_saved", {"%name%", name}).Get(), LBIconConfig.Get.info);
	}

	void ConfirmDeleteWeather() {
		LBNamedWeatherPreset weather = GetSelectedWeather();
		if (!weather)
			return;
		list_weather_presets.RemoveRow(list_weather_presets.GetSelectedRow());
		LBWeatherManagerConfig.Get.weatherPresets.RemoveItem(weather);
		LBWeatherManagerConfig.Loader.Save();
		NotificationSystem.AddNotificationExtended(5.0, "#lb_adm_admin_tools", LBTranslatedString("#lb_adm_weather_preset_deleted", {"%name%", weather.name}).Get(), LBIconConfig.Get.info);
	}

	void ConfirmDelete() {
		LBNamedTimePreset preset = GetSelectedPreset();
		if (!preset)
			return;
		list_presets.RemoveRow(list_presets.GetSelectedRow());
		LBWeatherManagerConfig.Get.timePresets.RemoveItem(preset);
		LBWeatherManagerConfig.Loader.Save();
		NotificationSystem.AddNotificationExtended(5.0, "#lb_adm_admin_tools", LBTranslatedString("#lb_adm_time_preset_deleted", {"%name%", preset.name}).Get(), LBIconConfig.Get.info);
	}

	void CreateNewPreset(string newName) {
		LBNamedTimePreset newPreset = new LBNamedTimePreset();
		newPreset.name = newName;
		LBWeatherManagerConfig.Get.timePresets.Insert(newPreset);
		LBWeatherManagerConfig.Loader.Save();
		list_presets.AddItem(newPreset.name, newPreset, 0);
		list_presets.SelectRow(list_presets.GetNumItems() - 1);
		NotificationSystem.AddNotificationExtended(5.0, "#lb_adm_admin_tools", LBTranslatedString("#lb_adm_time_preset_created", {"%name%", newName}).Get(), LBIconConfig.Get.info);
	}

	void SavePreset(string newName) {
		LBNamedTimePreset preset = GetSelectedPreset();
		if (!preset)
			return;
		preset.name = newName;
		preset.FromEditValues(edit_year, edit_month, edit_day, edit_hour, edit_minute);
		LBWeatherManagerConfig.Loader.Save();
		list_presets.SetItem(list_presets.GetSelectedRow(), newName, preset, 0);
		NotificationSystem.AddNotificationExtended(5.0, "#lb_adm_admin_tools", LBTranslatedString("#lb_adm_time_preset_saved", {"%name%", newName}).Get(), LBIconConfig.Get.info);
	}

	override void Update(float timeslice, bool otherMenu, bool leftClicked) {
		super.Update(timeslice, otherMenu, leftClicked);
		if (chckbx_preview_time.IsChecked()) {
			currentTime.FromEditValues(edit_year, edit_month, edit_day, edit_hour, edit_minute);
			originalTime.Apply();
			currentTime.Apply();
		}
		if (chckbx_preview_weather.IsChecked()) {
			currentWeather.FromEditValues(edit_overcast, edit_rain, edit_snowfall, edit_fog, edit_fog_dens, edit_fog_height, edit_fog_bias, edit_wind, edit_winddir, edit_duration, edit_fade_time);
			originalWeather.Apply(true);
			currentWeather.Apply(true);
		}
	}
	void SendApplyDate() {
		currentTime.FromEditValues(edit_year, edit_month, edit_day, edit_hour, edit_minute);
		currentTime.SendTo(null);
	}

	LBNamedTimePreset GetSelectedPreset() {
		int row;
		if (!LBWidgetUtils.CheckSelectedInRange(list_presets, row))
			return null;
		LBNamedTimePreset preset;
		list_presets.GetItemData(row, 0, preset);
		return preset;
	}

	LBNamedWeatherPreset GetSelectedWeather() {
		int row;
		if (!LBWidgetUtils.CheckSelectedInRange(list_weather_presets, row))
			return null;
		LBNamedWeatherPreset preset;
		list_weather_presets.GetItemData(row, 0, preset);
		return preset;
	}

	bool LoadSelectedPreset() {
		LBNamedTimePreset preset = GetSelectedPreset();
		if (!preset)
			return false;
		currentTime.CopyFrom(preset);
		currentTime.Validate();
		currentTime.SetEditValues(edit_year, edit_month, edit_day, edit_hour, edit_minute);
		return true;
	}

	bool LoadSelectedWeather() {
		LBNamedWeatherPreset preset = GetSelectedWeather();
		if (!preset)
			return false;
		currentWeather.CopyFrom(preset);
		currentWeather.Validate();
		currentWeather.SetEditValues(edit_overcast, edit_rain, edit_snowfall, edit_fog, edit_fog_dens, edit_fog_height, edit_fog_bias, edit_wind, edit_winddir, edit_duration, edit_fade_time);
		return true;
	}

	override void Show(bool show) {
		super.Show(show);
		if (show) {
			list_presets.ClearItems();
			foreach (LBNamedTimePreset presetTime : LBWeatherManagerConfig.Get.timePresets) {
				list_presets.AddItem(presetTime.name, presetTime, 0);
			}
			list_weather_presets.ClearItems();
			foreach (LBNamedWeatherPreset presetWeather : LBWeatherManagerConfig.Get.weatherPresets) {
				list_weather_presets.AddItem(presetWeather.name, presetWeather, 0);
			}
			currentTime.InitCurrent();
			currentTime.SetEditValues(edit_year, edit_month, edit_day, edit_hour, edit_minute);
			UpdateForecast();
			UpdateCurrentWeather();
			currentWeather.SetEditValues(edit_overcast, edit_rain, edit_snowfall, edit_fog, edit_fog_dens, edit_fog_height, edit_fog_bias, edit_wind, edit_winddir, edit_duration, edit_fade_time);
			UpdateTimeSliders();
			UpdateWeatherSliders();
		}
	}

	void UpdateTimeSliders() {
		slider_year.SetCurrent(currentTime.year + 1960);
		if (currentTime.month <= 0)
			slider_month.SetCurrent(0);
		else
			slider_month.SetCurrent(currentTime.month);
		if (currentTime.day <= 0)
			slider_day.SetCurrent(0);
		else
			slider_day.SetCurrent(currentTime.day);
		slider_hour.SetCurrent(currentTime.hour);
		slider_minute.SetCurrent(currentTime.minute);
	}

	void UpdateWeatherSliders() {
		slider_overcast.SetCurrent(currentWeather.overcast);
		slider_rain.SetCurrent(currentWeather.rain);
		slider_snowfall.SetCurrent(currentWeather.snowfall);
		slider_fog.SetCurrent(currentWeather.fog);
		slider_fog_dens.SetCurrent(currentWeather.volFogDensity);
		slider_fog_height.SetCurrent(currentWeather.volFogHeight);
		slider_fog_bias.SetCurrent(currentWeather.volFogBias);
		slider_wind.SetCurrent(currentWeather.windMag);
		slider_winddir.SetCurrent(currentWeather.windDir * Math.RAD2DEG);
		slider_duration.SetCurrent(currentWeather.defaultDuration);
		slider_fade_time.SetCurrent(currentWeather.defaultTime);
	}

	override bool OnChange(Widget w, int x, int y, bool finished) {
		if (super.OnChange(w, x, y, finished))
			return true;
		if (w == slider_year) {
			if (slider_year.GetCurrent() == slider_year.GetMin())
				currentTime.year = -1;
			else
				currentTime.year = slider_year.GetCurrent();
			edit_year.SetText("" + currentTime.year);
			return true;
		} else if (w == slider_month) {
			if (slider_month.GetCurrent() == slider_month.GetMin())
				currentTime.month = -1;
			else
				currentTime.month = slider_month.GetCurrent();
			edit_month.SetText("" + currentTime.month);
			return true;
		} else if (w == slider_day) {
			if (slider_day.GetCurrent() == slider_day.GetMin())
				currentTime.day = -1;
			else
				currentTime.day = slider_day.GetCurrent();
			edit_day.SetText("" + currentTime.day);
			return true;
		} else if (w == slider_hour) {
			if (slider_hour.GetCurrent() == slider_hour.GetMin())
				currentTime.hour = -1;
			else
				currentTime.hour = slider_hour.GetCurrent();
			edit_hour.SetText("" + currentTime.hour);
			return true;
		} else if (w == slider_minute) {
			if (slider_minute.GetCurrent() == slider_minute.GetMin())
				currentTime.minute = -1;
			else
				currentTime.minute = slider_minute.GetCurrent();
			edit_minute.SetText("" + currentTime.minute);
			return true;
		} else if (w == slider_overcast) {
			if (slider_overcast.GetCurrent() == slider_overcast.GetMin())
				currentWeather.overcast = -1;
			else if (slider_overcast.GetCurrent() < 0)
				currentWeather.overcast = 0;
			else
				currentWeather.overcast = slider_overcast.GetCurrent();
			edit_overcast.SetText("" + currentWeather.overcast);
		} else if (w == slider_rain) {
			if (slider_rain.GetCurrent() == slider_rain.GetMin())
				currentWeather.rain = -1;
			else if (slider_rain.GetCurrent() < 0)
				currentWeather.rain = 0;
			else
				currentWeather.rain = slider_rain.GetCurrent();
			edit_rain.SetText("" + currentWeather.rain);
		} else if (w == slider_snowfall) {
			if (slider_snowfall.GetCurrent() == slider_snowfall.GetMin())
				currentWeather.snowfall = -1;
			else if (slider_snowfall.GetCurrent() < 0)
				currentWeather.snowfall = 0;
			else
				currentWeather.snowfall = slider_snowfall.GetCurrent();
			edit_snowfall.SetText("" + currentWeather.snowfall);
		} else if (w == slider_fog) {
			if (slider_fog.GetCurrent() == slider_fog.GetMin())
				currentWeather.fog = -1;
			else if (slider_fog.GetCurrent() < 0)
				currentWeather.fog = 0;
			else
				currentWeather.fog = slider_fog.GetCurrent();
			edit_fog.SetText("" + currentWeather.fog);
		} else if (w == slider_fog_dens) {
			if (slider_fog_dens.GetCurrent() == slider_fog_dens.GetMin())
				currentWeather.volFogDensity = -1;
			else if (slider_fog_dens.GetCurrent() < 0)
				currentWeather.volFogDensity = 0;
			else
				currentWeather.volFogDensity = slider_fog_dens.GetCurrent();
			edit_fog_dens.SetText("" + currentWeather.volFogDensity);
		} else if (w == slider_fog_height) {
			currentWeather.volFogHeight = slider_fog_height.GetCurrent();
			edit_fog_height.SetText("" + currentWeather.volFogHeight);
		} else if (w == slider_fog_bias) {
			currentWeather.volFogBias = slider_fog_bias.GetCurrent();
			edit_fog_bias.SetText("" + currentWeather.volFogBias);
		} else if (w == slider_wind) {
			if (slider_wind.GetCurrent() == slider_wind.GetMin())
				currentWeather.windMag = -1;
			else if (slider_wind.GetCurrent() < 0)
				currentWeather.windMag = 0;
			else
				currentWeather.windMag = slider_wind.GetCurrent();
			edit_wind.SetText("" + currentWeather.windMag);
		} else if (w == slider_winddir) {
			if (slider_winddir.GetCurrent() == slider_winddir.GetMin())
				currentWeather.windDir = -1;
			else if (slider_winddir.GetCurrent() < 0)
				currentWeather.windDir = 0;
			else
				currentWeather.windDir = slider_winddir.GetCurrent();
			edit_winddir.SetText("" + currentWeather.windDir);
		} else if (w == slider_duration) {
			currentWeather.defaultDuration = slider_duration.GetCurrent();
			edit_duration.SetText("" + currentWeather.defaultDuration);
		} else if (w == slider_fade_time) {
			currentWeather.defaultTime = slider_fade_time.GetCurrent();
			edit_fade_time.SetText("" + currentWeather.defaultTime);
		} else if (w == edit_overcast) {
			slider_overcast.SetCurrent(edit_overcast.GetText().ToFloat());
		} else if (w == edit_rain) {
			slider_rain.SetCurrent(edit_rain.GetText().ToFloat());
		} else if (w == edit_snowfall) {
			slider_snowfall.SetCurrent(edit_snowfall.GetText().ToFloat());
		} else if (w == edit_fog) {
			slider_fog.SetCurrent(edit_fog.GetText().ToFloat());
		} else if (w == edit_fog_dens) {
			slider_fog_dens.SetCurrent(edit_fog_dens.GetText().ToFloat());
		} else if (w == edit_fog_height) {
			slider_fog_height.SetCurrent(edit_fog_height.GetText().ToFloat());
		} else if (w == edit_fog_bias) {
			slider_fog_bias.SetCurrent(edit_fog_bias.GetText().ToFloat());
		} else if (w == edit_wind) {
			slider_wind.SetCurrent(edit_wind.GetText().ToFloat());
		} else if (w == edit_winddir) {
			slider_winddir.SetCurrent(edit_winddir.GetText().ToFloat());
		} else if (w == edit_duration) {
			slider_duration.SetCurrent(edit_duration.GetText().ToFloat());
		} else if (w == edit_fade_time) {
			slider_fade_time.SetCurrent(edit_fade_time.GetText().ToFloat());
		} else if (w == edit_year) {
			slider_year.SetCurrent(edit_year.GetText().ToInt());
		} else if (w == edit_month) {
			slider_month.SetCurrent(edit_month.GetText().ToInt());
		} else if (w == edit_day) {
			slider_day.SetCurrent(edit_day.GetText().ToInt());
		} else if (w == edit_hour) {
			slider_hour.SetCurrent(edit_hour.GetText().ToInt());
		} else if (w == edit_minute) {
			slider_minute.SetCurrent(edit_minute.GetText().ToInt());
		}
		return false;
	}

	override void OnShow() {
		super.OnShow();
		if (pinned) {
			SetSize(lastWidth, lastHeight);
		}
	}

	override void OnHide() {
		super.OnHide();
		if (pinned) {
			GetSize(lastWidth, lastHeight);
			SetSize(400, 20);
		}
	}

	override bool OnItemSelected(Widget w, int x, int y, int row, int column, int oldRow, int oldColumn) {
		if (super.OnItemSelected(w, x, y, row, column, oldRow, oldColumn))
			return true;
		if (w == list_presets) {
			LoadSelectedPreset();
			UpdateTimeSliders();
			return true;
		} else if (w == list_weather_presets) {
			LoadSelectedWeather();
			UpdateWeatherSliders();
			return true;
		}
		return false;
	}
}
class LBTimePreset {

	int year = -1, month = -1, day = -1, hour = -1, minute = -1;

	string LBToString() {
		return "{year=" + year + ", month=" + month + ", day=" + day + ", hour=" + hour + ", minute=" + minute + "}";
	}

	void InitCurrent() {
		g_Game.GetWorld().GetDate(year, month, day, hour, minute);
	}

	void Apply() {
		int c_year, c_month, c_day, c_hour, c_minute;
		g_Game.GetWorld().GetDate(c_year, c_month, c_day, c_hour, c_minute);
		if (year != -1)
			c_year = year;
		if (month != -1)
			c_month = month;
		if (day != -1)
			c_day = day;
		if (hour != -1)
			c_hour = hour;
		if (minute != -1)
			c_minute = minute;
		g_Game.GetWorld().SetDate(c_year, c_month, c_day, c_hour, c_minute);
	}

	void CopyFrom(LBTimePreset from) {
		year = from.year;
		month = from.month;
		day = from.day;
		hour = from.hour;
		minute = from.minute;
	}

	void SendTo(PlayerIdentity target) {
		g_Game.RPCSingleParam(null, LBAdminToolRPCs.SET_TIME, new Param1<ref LBTimePreset>(this), true, target);
	}

	void Validate() {
		if (year < 1960 && year != -1)
			year = 1960;
		else if (year > 2100)
			year = 2100;

		if (month < 1 && month != -1)
			month = 1;
		else if (month > 12)
			month = 12;

		if (day < 1 && day != -1)
			day = 1;
		else if (day > 31)
			day = 31;

		if (hour < 0 && hour != -1)
			hour = 0;
		else if (hour > 23)
			hour = 23;

		if (minute < 0 && minute != -1)
			minute = 0;
		else if (minute > 59)
			minute = 59;
	}

	string ToDateString() {
		LBDate date = LBDate.Init(year, month, day, hour, minute, 0);
		string str = date.ToFormattedString();
		if (str.Length() > 3)
			str = str.Substring(0, str.Length() - 3);
		return str;
	}

	void SetEditValues(EditBoxWidget w_year, EditBoxWidget w_month, EditBoxWidget w_day, EditBoxWidget w_hour, EditBoxWidget w_minute) {
		w_year.SetText("" + year);
		w_month.SetText("" + month);
		w_day.SetText("" + day);
		w_hour.SetText("" + hour);
		w_minute.SetText("" + minute);
	}

	void FromEditValues(EditBoxWidget w_year, EditBoxWidget w_month, EditBoxWidget w_day, EditBoxWidget w_hour, EditBoxWidget w_minute) {
		year = w_year.GetText().ToInt();
		month = w_month.GetText().ToInt();
		day = w_day.GetText().ToInt();
		hour = w_hour.GetText().ToInt();
		minute = w_minute.GetText().ToInt();
		Validate();
	}

	void FromAndValidateEditValues(EditBoxWidget w_year, EditBoxWidget w_month, EditBoxWidget w_day, EditBoxWidget w_hour, EditBoxWidget w_minute) {
		FromEditValues(w_year, w_month, w_day, w_hour, w_minute);
		SetEditValues(w_year, w_month, w_day, w_hour, w_minute);
	}

}
class LBAdminMenuConsole : LBMenuBase {

	WrapSpacerWidget panel_available;
	ScrollWidget scroll_panel;
	EditBoxWidget edit_input;

	ref array<ref LBConsoleCommandBase> commands;

	LBConsoleCommandBase bestCommand = null;
	int lastCommandIndex = -1;

	override string GetLayoutName() {
		return "AdminMenuConsole";
	}

	override void OnInit() {
		super.OnInit();
		commands = LBConsoleCommandHandler.Get().GetAvailableCommands();
		SetFocus(edit_input);
		LoadAvailableCommands();
	}

	override void Update(float timeslice) {
		super.Update(timeslice);
		if (g_Game.LBIsKeyPressed(KeyCode.KC_UP)) {
			LoadPreviousCommand();
		} else if (g_Game.LBIsKeyPressed(KeyCode.KC_DOWN)) {
			LoadNextCommand();
		}
	}

	void LoadPreviousCommand() {
		if (lastCommandIndex + 1 < LBAdminMenuSettings.Get.lastCommands.Count())
			++lastCommandIndex;
		edit_input.SetText(LBAdminMenuSettings.Get.lastCommands.Get(lastCommandIndex));
		LoadAvailableCommands();
	}

	void LoadNextCommand() {
		if (lastCommandIndex >= 0)
			--lastCommandIndex;
		if (lastCommandIndex >= 0)
			edit_input.SetText(LBAdminMenuSettings.Get.lastCommands.Get(lastCommandIndex));
		else
			edit_input.SetText("");
		LoadAvailableCommands();
	}

	void LoadAvailableCommands() {
		LBWidgetUtils.DeleteAllChildren(panel_available);
		bestCommand = null;
		string input = edit_input.GetText();
		if (input.IndexOf("/") != 0)
			input = "/" + input;
		foreach (LBConsoleCommandBase command : commands) {
			if (command.MatchInput(input)) {
				Widget w = LBLayoutManager.Get().CreateLayout("AdminMenuCommandEntry", panel_available);
				TextWidget txt = TextWidget.Cast(w);
				txt.SetText(command.GetCommandPreview());
				if (command.prefix.Length() < input.Length() && command.IsValid()) {
					txt.SetColor(ARGB(255, 0, 255, 0));
					txt.SetSort(10);
					if (!bestCommand) {
						bestCommand = command;
					}
				}
			}
		}
	}

	override bool OnChange(Widget w, int x, int y, bool finished) {
		if (super.OnChange(w, x, y, finished))
			return true;
		if (w == edit_input) {
			LoadAvailableCommands();
			if (finished && bestCommand) {
				if (LBAdminMenuSettings.Get.lastCommands.Count() == 0 || LBAdminMenuSettings.Get.lastCommands.Get(0) != edit_input.GetText()) {
					LBAdminMenuSettings.Get.lastCommands.InsertAt(edit_input.GetText(), 0);
					if (LBAdminMenuSettings.Get.lastCommands.Count() > 100)
						LBAdminMenuSettings.Get.lastCommands.Remove(100);
					LBAdminMenuSettings.Loader.Save();
				}
				bestCommand.Execute();
				CloseMe();
				return true;
			}
			if (finished && edit_input.GetText() == "")
				CloseMe();
			return true;
		}
		return false;
	}

}
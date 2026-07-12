class LBConsoleCommandBase {

	string prefix; // Set the prefix of your command in the Register() method
	ref TStringArray parameter = new TStringArray(); // Set the parameter you want to display here

	// Cache last parsed Parameters. This should not be directly edited by you
	// Will be valid in HasPermission(), IsValid() and Execute();
	ref TStringArray parsedParameter = new TStringArray();
	string rawParameter = ""; // Raw non parsed parameters as text

	// Always overwrite this method in your own Command!
	// Check my default commands in the Commands folder
	// Don't forget to add the following code to register your command
	// [LBConsoleCommandHandler.Get().Register("COMMAND_CLASSNAME")]
	void Register() {
		// Do the stuff here. Set the prefix without the /
		// example
		// prefix = "tpp";
		// paramter.Insert("position");
	}

	bool HasPermission() {
		// You can check a permission here, which the player must have
		// return true if the player has permissions. You can use the LBAdmins.Get().HasPermissions("my.permission.name"); to check for permissions
		// If the player does not have the permission for the command, he will not see it in the available commands list
		return true;
	}

	bool IsValid() {
		// do your validation here
		return true;
	}

	void Execute() {
		// If IsValid was true, Execute will be called. This cannot be called without the command first being validated
		// This is all called client side. You can send an RPC to the server here
	}

	// Internal stuff you should not need to overwrite

	// Method to pretty print the command and the parameter set in the Register function
	string GetCommandPreview() {
		string paramAll = "";
		foreach (string param : parameter)
			paramAll = paramAll + " " + param;
		if (parameter.Count() > 0)
			return "/" + prefix + paramAll;
		return "/" + prefix;
	}

	// This method takes the raw input from the user and tries to parse it and check if it starts with the prefix
	// If you return false here, the command will not show up in the available commands list
	bool MatchInput(string input) {
		rawParameter = input;
		string start = LBStringTools.ToLowerString("/" + prefix);
		string inputLower = LBStringTools.ToLowerString(input);
		parsedParameter.Clear();
		if (start.Length() >= inputLower.Length())
			return start.IndexOf(inputLower) == 0;
		if (inputLower.IndexOf(start) != 0)
			return false;
		string args = inputLower.Substring(start.Length(), inputLower.Length() - start.Length());
		if (args.Length() > 0 && args[0] != " ")
			return false;
		args.Split(" ", parsedParameter);
		return HasPermission();
	}

}
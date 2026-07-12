[DayZGame.RegisterLBAdminPage("LBCustomPagesConfigAdmin", "LBmaster_Groups/logo/logo.paa", "Custom Map Pages", "groups.change.pages")]
class LBCustomPagesConfigAdmin : LBAdmin_Menu_Page {

	WrapSpacerWidget content_small, content_full, content_rules;
	MultilineEditBoxWidget edit_lines;
	CheckBoxWidget chckbx_enabled, chckbx_fullsize;
	XComboBoxWidget combo_pages;
	EditBoxWidget edit_priority, edit_btn_text;
	ButtonWidget btn_preview_full, btn_remove, btn_add, btn_close_preview;
	Widget preview_small, panel_background, preview_full, panel_rules;
	ScrollWidget scrollPanel_small, scrollPanel_full, scrollPanel_rules;

	override void OnPageShown() {
		super.OnPageShown();
		linked.LoadLinkedVars();
		LoadFormattingRules();
	}

	override array<string> GetEditedConfigs() {
		return {"LBPagesConfig"};
	}

	override void RegisterAllLinkedVars() {
		// Info Page
		linked.RegisterLinkedList("GetConfig().customPages", combo_pages, btn_add, btn_remove).SetListManager(new LBListManager<LBCustomPageSettings>()).SetChangeTrigger("LoadLineContent");
		linked.RegisterLinkedVar("GetConfig().customPages[combo_pages].enabled", chckbx_enabled);
		linked.RegisterLinkedVar("GetConfig().customPages[combo_pages].priority", edit_priority);
		linked.RegisterLinkedVar("GetConfig().customPages[combo_pages].buttonText", edit_btn_text, true);
		linked.RegisterLinkedVar("GetConfig().customPages[combo_pages].fullSize", chckbx_fullsize);

	}

	void LoadLineContent() {
		int index;
		if (!LBWidgetUtils.CheckSelectedInRange(combo_pages, index))
			return;
		LBCustomPageSettings settings = GetConfig().customPages.Get(index);
		edit_lines.SetText("");
		for (int i = 0; i < settings.lines.Count(); ++i) {
			edit_lines.SetLine(i, settings.lines.Get(i));
		}
		UpdateSmallPreview();
	}

	override bool OnClick(Widget w, int x, int y, int button) {
		if (super.OnClick(w, x, y, button))
			return true;
		if (w == btn_close_preview) {
			panel_background.Show(false);
			return true;
		} else if (w == btn_preview_full) {
			panel_background.Show(true);
			LoadFullPreview();
			return true;
		}
		return false;
	}

	override void KeyDown(int key) {
		Widget focussed = GetFocus();
		if (focussed == edit_lines) {
			UpdateSmallPreview();
		}
		super.KeyDown(key);
	}

	TStringArray GetEditLines() {
		TStringArray lines = new TStringArray();
		int count = edit_lines.GetLinesCount();
		lines.Reserve(count);
		for (int i = 0; i < count; ++i) {
			string line;
			edit_lines.GetLine(i, line);
			lines.Insert(line);
		}
		return lines;
	}

	void UpdateSmallPreview() {
		TStringArray lines = GetEditLines();
		LBWidgetUtils.DeleteAllChildren(content_small);
		LBWidgetUtils.CreateLines(scrollPanel_small, content_small, lines);
		int index;
		if (!LBWidgetUtils.CheckSelectedInRange(combo_pages, index))
			return;
		GetConfig().customPages.Get(index).lines = lines;
	}

	void LoadFullPreview() {
		TStringArray lines = GetEditLines();
		LBWidgetUtils.DeleteAllChildren(content_full);
		LBWidgetUtils.CreateLines(scrollPanel_full, content_full, lines);
	}

	void LoadFormattingRules() {
		TStringArray lines = new TStringArray();
		lines.Insert("$args(size(20),center,rgb(0,255,0));General");
		lines.Insert("To format a line it needs to look like this: \"$args(ARGUMENTS);TEXT\". Make sure to not forget the $ at the start");
		lines.Insert("TEXT must be replaced with your text you want to display");
		lines.Insert("ARGUMENTS must be replaced with the arguments explained below. Multiple arguments are separated with a comma");
		lines.Insert("$args(size(20),center,rgb(0,255,0));Arguments list");
		lines.Insert("$args(bold,rgb(255,128,0));bold");
		lines.Insert("make the text bold");
		lines.Insert("$args(bold,rgb(255,128,0));underline");
		lines.Insert("will make the text underlined");
		lines.Insert("$args(bold,rgb(255,128,0));italic");
		lines.Insert("make the text italic");
		lines.Insert("$args(bold,rgb(255,128,0));argb(A,R,G,B)");
		lines.Insert("replace A,R,G and B with the ARGB color values from 0 to 255 to change the color of the text in this line");
		lines.Insert("$args(bold,rgb(255,128,0));rgb(R,G,B)");
		lines.Insert("same as above, but alpha channel will be 255 (full) by default");
		lines.Insert("$args(bold,rgb(255,128,0));size(PX)");
		lines.Insert("set the size of the text in pixels. Default size is 16");
		lines.Insert("$args(bold,rgb(255,128,0));indent(PX)");
		lines.Insert("indent this line by x pixels");
		lines.Insert("$args(bold,rgb(255,128,0));left");
		lines.Insert("Sets the text alignment to left (default alignment)");
		lines.Insert("$args(bold,rgb(255,128,0));right");
		lines.Insert("Sets the text alignment to right");
		lines.Insert("$args(bold,rgb(255,128,0));center");
		lines.Insert("Sets the text alignment to center");
		lines.Insert("$args(bold,rgb(255,128,0));outline(A,R,G,B,SIZE)");
		lines.Insert("add a colored outline around the text with the ARGB color values and the size (0-10)");
		lines.Insert("$args(bold,rgb(255,128,0));shadow(A,R,G,B,SIZE,OPACITY,OFFSET_X,OFFSET_Y)");
		lines.Insert("add a text shadow with the ARGB color values, the size (0-10), Opacity (0.0 - 1.0), X Offset (0.0 - 10.0) and Y Offset (0.0 - 10.0). The last 3 values are optional");
		lines.Insert("$args(bold,rgb(255,128,0));sectionStart(INDENT,OPEN)");
		lines.Insert("starts a new section of text, which can be expanded (INDENT in pixels how far the new section will be moved in, OPEN (0 = closed by default, 1 = open by default). The line where the sectionStart argument is in, will be the line, which can be clicked to open and close the section");
		lines.Insert("$args(bold,rgb(255,128,0));sectionEnd");
		lines.Insert("must be placed into the arguments list of the last element, which should be in the current opened section. Do not Start and End a section in the same line");
		lines.Insert("$args(bold,rgb(255,128,0));img(PATH) or img(PATH,WIDTH,HEIGHT)");
		lines.Insert("PATH is the path to the image. Width and height are in pixels. To automatically calculate the height or the width based on the image size to not stretch it, you can use a as an argument for the width and set the height ot what you need. Same works for when setting the height to a. The img(PATH) is equal to img(PATH,a,a). Example: img(LBmaster_Groups/gui/images/heavy_crafting.paa,a,250)");
		lines.Insert("$args(bold,rgb(255,128,0));qrcode(size,PATH)");
		lines.Insert("PATH is the URL you want the QR code to point to. Size is the size in pixel on a 1920x1080 display and will scale with bigger screens. Recommended size is between 200 and 300");
		lines.Insert("$args(size(20),center,rgb(0,255,0));Examples");
		lines.Insert(" - $args(size(30),center,rgb(255,0,0));Rules: \"Rules\" displayed in red, centered and with a size of 30 pixels");
		LBWidgetUtils.DeleteAllChildren(content_rules);
		LBWidgetUtils.CreateLines(scrollPanel_rules, content_rules, lines);
	}

	LBPagesConfig_ GetConfig() {
		return LBPagesConfig.Get;
	}

}
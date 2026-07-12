#ifndef LB_DISABLE_CHAT
class ChatEventHandler : ScriptedWidgetEventHandler {

	EditBoxWidget m_edit_box;
	ChatInputMenu chatMenu;

	void ChatEventHandler(ChatInputMenu menu) {
		chatMenu = menu;
		m_edit_box = menu.GetEditBox();
		m_edit_box.SetHandler(this);
	}

	override bool OnChange(Widget w, int x, int y, bool finished) {
		return chatMenu.OnChangeLB(w, x, y, finished);
	}

}
#endif
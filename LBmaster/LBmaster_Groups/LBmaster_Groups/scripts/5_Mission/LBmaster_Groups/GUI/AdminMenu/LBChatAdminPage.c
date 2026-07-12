#ifndef LB_DISABLE_CHAT
[DayZGame.RegisterLBAdminPage("LBChatAdminPage", "LBmaster_Groups/logo/logo.paa", "#lb_ag_admin_menu_chat", "groups.change.chat")]
class LBChatAdminPage : LBAdmin_Menu_Page {

	XComboBoxWidget combo_channel, combo_prefix;
	CheckBoxWidget chckbx_display_tags, chckbx_force_display_tags, chckbx_allow_vote, chckbx_filter_badwords, chckbx_block_filtered_msg, chckbx_global, chckbx_group;
	CheckBoxWidget chckbx_direct, chckbx_default, chckbx_channel_muted, chckbx_allow_write, chckbx_apply_filter;
	ButtonWidget btn_change_be_color, btn_add_word, btn_remove_word, btn_create_channel, btn_delete_channel, btn_change_channel_color, btn_change_prefix_color;
	ButtonWidget btn_add_steamid, btn_remove_steamid, btn_create_prefix, btn_delete_prefix;
	EditBoxWidget input_min_players_online, input_mute_time, input_min_players_voting, input_block_message, input_mute_bad_word, input_add_bad_word, input_slow_mode_seconds;
	EditBoxWidget edit_channel_perm_write, edit_channel_perm_read, input_channel_name, input_prefix_name, input_permission, input_add_to_prefix;
	TextListboxWidget list_bad_words, list_prefix_members;
	Widget panel_general, battleye_color_preview, panel_channel, channel_color_preview, panel_prefix, prefix_color_preview;

	override void OnShow() {
		linked.LoadLinkedVars();
	}

	override void RegisterAllLinkedVars() {

		ApplyWidgetPermission(panel_general, "groups.change.chat");
		ApplyWidgetPermission(panel_channel, "groups.change.chat");
		ApplyWidgetPermission(panel_prefix, "groups.change.chatprefix");

		linked.RegisterLinkedVar("GetConfig().displayGroupTagsInfrontOfName", chckbx_display_tags);
		linked.RegisterLinkedVar("GetConfig().forceDisplayGroupTagsInChat", chckbx_force_display_tags);
		linked.RegisterLinkedVar("GetConfig().enableMuteVote", chckbx_allow_vote);
		linked.RegisterLinkedVar("GetConfig().muteVoteMinPlayers", input_min_players_online);
		linked.RegisterLinkedVar("GetConfig().muteVotePercentile", input_min_players_voting);
		linked.RegisterLinkedVar("GetConfig().muteVoteMuteTimeMins", input_mute_time);
		linked.RegisterLinkedVar("GetConfig().colorBattleyeMessage", btn_change_be_color).EnableColorPicker(true, "", "", "", "", battleye_color_preview);
		linked.RegisterLinkedVar("GetConfig().enabledBadWordsCensor", chckbx_filter_badwords);
		linked.RegisterLinkedVar("GetConfig().blockBadWordContainingMessages", chckbx_block_filtered_msg);
		linked.RegisterLinkedVar("GetConfig().badWordsBlockedMessage", input_block_message);
		linked.RegisterLinkedVar("GetConfig().badWordsMuteTime", input_mute_bad_word);
		linked.RegisterLinkedList("GetConfig().badWords", list_bad_words, btn_add_word, btn_remove_word, input_add_bad_word);
		linked.RegisterLinkedList("GetConfig().prefixGroups", combo_prefix, btn_create_prefix, btn_delete_prefix).SetListManager(new LBListManager<PrefixGroup>());
		linked.RegisterLinkedVar("GetConfig().prefixGroups[combo_prefix].color", btn_change_prefix_color).EnableColorPicker(false, "", "R", "G", "B", prefix_color_preview);
		linked.RegisterLinkedVar("GetConfig().prefixGroups[combo_prefix].prefix", input_prefix_name, true);
		linked.RegisterLinkedVar("GetConfig().prefixGroups[combo_prefix].permissionToApplyGroup", input_permission);
		linked.RegisterLinkedList("GetConfig().prefixGroups[combo_prefix].members", list_prefix_members, btn_add_steamid, btn_remove_steamid, input_add_to_prefix);
		linked.RegisterLinkedList("GetConfig().channels", combo_channel, btn_create_channel, btn_delete_channel).SetListManager(new LBListManager<ChannelCfg>());
		linked.RegisterLinkedVar("GetConfig().channels[combo_channel].channelName", input_channel_name, true);
		linked.RegisterLinkedVar("GetConfig().channels[combo_channel].channelColor", btn_change_channel_color).EnableColorPicker(true, "", "", "", "", channel_color_preview);
		linked.RegisterLinkedVar("GetConfig().channels[combo_channel].globalChannel", chckbx_global);
		linked.RegisterLinkedVar("GetConfig().channels[combo_channel].groupChannel", chckbx_group);
		linked.RegisterLinkedVar("GetConfig().channels[combo_channel].directChannel", chckbx_direct);
		linked.RegisterLinkedVar("GetConfig().channels[combo_channel].defaultChannel", chckbx_default);
		linked.RegisterLinkedVar("GetConfig().channels[combo_channel].applyChatFilter", chckbx_apply_filter);
		linked.RegisterLinkedVar("GetConfig().channels[combo_channel].muted", chckbx_channel_muted);
		linked.RegisterLinkedVar("GetConfig().channels[combo_channel].readChannelPermission", edit_channel_perm_read);
		linked.RegisterLinkedVar("GetConfig().channels[combo_channel].writeChannelPermission", edit_channel_perm_write);
		linked.RegisterLinkedVar("GetConfig().channels[combo_channel].canReceiveMessagesFromPlayers", chckbx_allow_write);
		linked.RegisterLinkedVar("GetConfig().channels[combo_channel].slowModeSecondsBetweenMessages", input_slow_mode_seconds);
	}

	ChatConfig_ GetConfig() {
		return ChatConfig.Get;
	}

	override array<string> GetEditedConfigs() {
		return {"ChatConfig"};
	}

	override void OnRPC(Object target, int rpc_type, ParamsReadContext ctx) {
		if (rpc_type == LBmaster_Core_RPCs.CONFIG_SYNC) {
			linked.LoadLinkedVars();
		}
	}
	override bool OnClick(Widget w, int x, int y, int button) {
		super.OnClick(w, x, y, button);
		if (w == btn_create_prefix) {
			linked.OnVarChange(combo_prefix, -1);
		} else if (w == btn_create_channel) {
			linked.OnVarChange(combo_channel, -1);
		}
		return false;
	}
}
#endif

#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		int count;
		tag_t *ch;
		tag_t item_t, itemr_t,window_t,topline_t,rule_t;
		ITEM_find_item("000165", &item_t);
		ITEM_ask_latest_rev(item_t, &itemr_t); 
		BOM_create_window(&window_t);
		BOM_set_window_top_line(window_t, item_t, itemr_t, NULLTAG, &topline_t);
		CFM_find("Latest Working", &rule_t);
		BOM_set_window_config_rule(window_t, rule_t);
		BOM_line_ask_all_child_lines(topline_t, &count, &ch); 
		cout << " Total children = " << count << endl;
		for (int i = 0; i < count; i++)
		{
			char *id, *item_name;
			AOM_UIF_ask_value(ch[i], "bl_config_string", &id);
			AOM_UIF_ask_value(ch[i], "bl_item_object_name", &item_name);
			cout << " itemid = " << id;
			cout << " itemname = " << item_name<<endl;
		} 
		
		BOM_save_window(window_t);
		BOM_close_window(window_t);
	}
	return(0);
}
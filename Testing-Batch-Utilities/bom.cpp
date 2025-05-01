#include"Header.h"
void BOM_ask_child(int c, tag_t *k)
{
	int x;
	int subcount;
	char *id, *item_name;
	tag_t *sub_child;
	for (int i = 0; i < c; i++)
	{
		AOM_UIF_ask_value(k[i], "bl_item_item_id", &id);
		AOM_UIF_ask_value(k[i], "bl_item_object_name", &item_name);
		cout << " id = " << id;
		cout << " Item name = " << item_name << endl;
		BOM_line_ask_all_child_lines(k[i], &subcount, &sub_child);
		cout << " Total count of child = " << subcount << endl;
		if (subcount > 0)
		{
			BOM_ask_child(subcount, sub_child);
		}
	}
}
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		int count;
		tag_t *ch;
		tag_t item_t, itemr_t, window_t, bomview_t, bvr_t, topline_t;

		ITEM_find_item("000472", &item_t);
		ITEM_ask_latest_rev(item_t, &itemr_t);
		BOM_create_window(&window_t);
		PS_create_bom_view(NULLTAG, NULL, NULL, item_t, &bomview_t);
		AOM_save_with_extensions(bomview_t);
		PS_create_bvr(bomview_t, NULL, NULL, false, itemr_t, &bvr_t);
		AOM_save_with_extensions(bvr_t);
		BOM_set_window_top_line(window_t, item_t, itemr_t, bomview_t, &topline_t);
		BOM_line_ask_all_child_lines(topline_t, &count, &ch);
		cout << endl;
		cout << " Total count of child = " << count << endl;
		if (count > 0)
		{
			BOM_ask_child(count, ch);
		}
	}
	return(0);
}
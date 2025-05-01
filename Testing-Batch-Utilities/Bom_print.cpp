#include"Header.h"
int ITK_user_main(int argc, char *argv[])
{
	int c = login();
	if (c == 1)
	{
		cout << " Login successful " << endl;
		tag_t item_tag, rev_tag, window_tag, top_b_line;
		char  *rev_name, *d_value, *obj_name;
		int count;
		tag_t *child;
		ITEM_find_item("000150", &item_tag);
		ITEM_find_revision(item_tag, "A", &rev_tag);
		ITEM_ask_rev_name2(rev_tag, &rev_name);
		BOM_create_window(&window_tag);
		BOM_save_window(window_tag);
		BOM_set_window_top_line(window_tag, NULLTAG, rev_tag, NULLTAG, &top_b_line);
		BOM_line_ask_all_child_lines(top_b_line, &count, &child);
		cout << "************************** " << endl;
		cout << " count = " << count << endl;
		for (int i = 0; i < count; i++)
		{
			AOM_UIF_ask_value(child[i], "bl_item_item_id", &d_value);
			printf("\nitem id=%s", d_value);
			AOM_UIF_ask_value(child[i], "bl_item_object_name", &obj_name);
			printf("\nchild name=%s", obj_name);
		}
	}
	return(0);
}
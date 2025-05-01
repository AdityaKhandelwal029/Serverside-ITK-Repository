#include"Header.h"
ofstream i1("C:/Mini_project/custom_all_properties.csv");
void BOM_ask_child(int c, tag_t *k)
{
	int subcount;
	char *id, *item_name;
	tag_t *sub_child;
	for (int i = 0; i < c; i++)
	{
		int allprop;
		char **prop_name;
		AOM_ask_prop_names(k[i], &allprop, &prop_name);
		cout << " Total property count = " << allprop;
		for (int p = 0; p < allprop; p++)
		{
			char *value;
			AOM_UIF_ask_value(k[i], prop_name[p], &value);
			cout << " prop_name =" << prop_name[p];
			cout << " value = " << value << endl;
			i1 << " prop_name= " << prop_name[p] << " value= " << value << endl;
		}
		/*AOM_UIF_ask_value(k[i], "bl_item_item_id", &id);
		AOM_UIF_ask_value(k[i], "bl_item_object_name", &item_name);
		cout << " id = " << id;
		cout << " Item name = " << item_name << endl;
		i1 << " Bl item id = " << id << endl;
		i1 << " Bl item objct name = " << item_name << endl;*/
		BOM_line_ask_all_child_lines(k[i], &subcount, &sub_child);
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
		string item_id, rev_id;
		int count;
		tag_t *ch;
		tag_t item_t, *itemr_t, window_t, bomview_t, bvr_t, topline_t;
		i1 << " Login successfull ";
		cout << endl;
		cout << " Please enter item id = ";
		cin >> item_id;
		cout << " Please enter item revision id such as A/B/C/.. = ";
		cin >> rev_id;
		const char *a1 = item_id.c_str();
		ITEM_find_item(a1, &item_t);
		ITEM_list_all_revs(item_t, &count, &itemr_t);
		for (int i = 0; i < count; i++)
		{
			char *value;
			AOM_UIF_ask_value(itemr_t[i], "item_revision_id", &value);
			if (value == rev_id)
			{
				BOM_create_window(&window_t);
				PS_create_bom_view(NULLTAG, NULL, NULL, item_t, &bomview_t);
				AOM_save_with_extensions(bomview_t);
				PS_create_bvr(bomview_t, NULL, NULL, false, itemr_t[i], &bvr_t);
				AOM_save_with_extensions(bvr_t);
				BOM_set_window_top_line(window_t, item_t, itemr_t[i], bomview_t, &topline_t);
				BOM_line_ask_all_child_lines(topline_t, &count, &ch);
				cout << endl;
				cout << " Total count of child = " << count << endl;
				i1 << " Item id given as an input = ";
				i1 << item_id;
				i1 << endl;
				i1 << "Item found successfully " << endl;
				if (count > 0)
				{
					BOM_ask_child(count, ch);
				}
			}
			else
			{
				cout << " Revision not found or any error occured " << endl;
			}
		}
	}
	return(0);
}
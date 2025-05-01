#include"Header.h"
ofstream i1("C:/Mini_project/Bominformation2.csv");
void BOM_ask_child(int c, tag_t *k)
{
	int x;
	int subcount;
	tag_t *sub_child;
	for (int i = 0; i < c; i++)
	{
		string px1[16] = { "bl_indented_title","bl_item_object_type","bl_config_string","bl_rev_release_status_list","bl_sequence_no","bl_part_source","bl_uom","bl_ref_designator","bl_all_notes","bl_variant_state","bl_rev_has_variants","bl_variant_condition","bl_is_occ_configured","bl_has_date_effectivity","bl_rev_checked_out","fnd0bl_has_active_markup" };
		for (int j = 0; j < 16; j++)
		{
			char *id;
			const char *cha = px1[j].c_str();
			AOM_UIF_ask_value(k[i], cha, &id);
			cout << " Property name = " << px1[j];
			cout << " Property value = " << id << endl;
			i1 << "Property name = " << px1[j];
			i1 << " Property value = " << id << endl;
		}
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
		string iw;
		int count;
		tag_t *ch;
		tag_t item_t, itemr_t, window_t, bomview_t, bvr_t, topline_t;
		i1 << " Login successfull ";
		cout << endl;
		cout << " Please enter item id = ";
		cin >> iw;
		const char *a1 = iw.c_str();
		ITEM_find_item(a1, &item_t);
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
		i1 << " Item id given as an input = ";
		i1 << a1;
		i1 << endl;
		i1 << " Item found successfully " << endl;
		if (count > 0)
		{
			BOM_ask_child(count, ch);
		}
	}
	return(0);
}
#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		tag_t i1, i2, i3, i4, ir1, ir2, ir3, ir4, e, e1, e2, e3;
		tag_t item_tag, window, itemr_tag, bom_view, top_bomline,bvr_tag;
		cout << ITEM_find_item("000030", &item_tag) << endl;
		cout << ITEM_ask_latest_rev(item_tag, &itemr_tag) << endl;
		cout << BOM_create_window(&window) << endl;
		cout << PS_create_bom_view(NULLTAG, NULL, NULL, item_tag, &bom_view) << endl;
		cout << AOM_save_with_extensions(bom_view) << endl;
		PS_create_bvr(bom_view, NULL, NULL, false,itemr_tag,& bvr_tag);
		AOM_save_with_extensions(bvr_tag);
		cout << BOM_set_window_top_line(window, item_tag, itemr_tag, NULLTAG, &top_bomline) << endl;
		cout << ITEM_find_item("000031", &i1) << endl;
		cout << ITEM_ask_latest_rev(i1, &ir1) << endl;
		cout << ITEM_find_item("000032", &i2) << endl;
		cout << ITEM_ask_latest_rev(i2, &ir2) << endl;
		cout << ITEM_find_item("000033", &i3) << endl;
		cout << ITEM_ask_latest_rev(i3, &ir3) << endl;
		cout << ITEM_find_item("000034", &i4) << endl;
		cout << ITEM_ask_latest_rev(i4, &ir4) << endl;
		cout << BOM_line_add(top_bomline, i1, ir1, NULLTAG, &e) << endl;
		cout << AOM_save_with_extensions(e) << endl;
		cout << BOM_line_add(top_bomline, i2, ir2, NULLTAG, &e1) << endl;
		cout << AOM_save_with_extensions(e1) << endl;
		cout << BOM_line_add(top_bomline, i3, ir3, NULLTAG, &e2) << endl;
		cout << AOM_save_with_extensions(e2) << endl;
		cout << BOM_line_add(top_bomline, i4, ir4, NULLTAG, &e3) << endl;
		cout << AOM_save_with_extensions(e3) << endl;
		cout << BOM_save_window(window) << endl;
		cout << BOM_close_window(window) << endl;
	}
	return(0);
}

#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		
	}
	return(0);
}

//#include"Header.h"
//int ITK_user_main(int argc, char* argv[])
//{
//	int x = login();
//	if (x == 1)
//	{
//		int revison_count, x;
//		tag_t item_tag, itemr_tag, window_tag, top_bom_line, *rev_list, *children;
//		ITEM_find_item("000147", &item_tag);
//		ITEM_find_revision(item_tag, "A", &itemr_tag);
//		ITEM_list_all_revs(item_tag, &revison_count, &rev_list);
//		cout << " Toatal number of revision = " << revison_count << endl;
//		BOM_create_window(&window_tag);
//		BOM_save_window(window_tag);
//		BOM_set_window_top_line(window_tag, item_tag, itemr_tag, NULLTAG, &top_bom_line);
//		BOM_line_ask_all_child_lines(top_bom_line, &x, &children);
//	}
//	return(0);
//}
//
//#include"Header.h"
//int ITK_user_main(int argc, char* argv[])
//{
//	int x = login();
//	if (x == 1)
//	{
//		tag_t item_t, itemr_t, window, bom_view, topb_line, item2_t, item2r_t, bla, bvr, item3_t, item3r_t, x;
//		ITEM_create_item("000227", "abcd", "Item", "A", &item_t, &itemr_t);
//		ITEM_create_item("000228", "efgh", "Item", "A", &item2_t, &item2r_t);
//		ITEM_create_item("000229", "hijk", "Item", "A", &item3_t, &item3r_t);
//		ITEM_save_item(item_t);
//		ITEM_save_item(item2_t);
//		ITEM_save_item(item3_t);
//		BOM_create_window(&window);
//		PS_create_bom_view(NULLTAG, NULL, NULL, item_t, &bom_view);
//		AOM_save_with_extensions(bom_view);
//		PS_create_bvr(bom_view, NULL, NULL, false, itemr_t, &bvr);
//		AOM_save_with_extensions(bvr);
//		BOM_set_window_top_line(window, item_t, itemr_t, bom_view, &topb_line);
//		BOM_line_add(topb_line, item2_t, item2r_t, NULLTAG, &bla);
//		BOM_line_add(bla, item3_t, item3r_t, NULLTAG, &x);
//		AOM_save_with_extensions(bla);
//		AOM_save_with_extensions(x);
//		BOM_save_window(window);
//		BOM_close_window(window);
//	}
//	return(0);
//}
//#include"Header.h"
//int ITK_user_main(int argc, char* argv[])
//{
//	int x = login();
//	if (x == 1)
//	{
//		tag_t i1, i2, i3, i4, ir1, ir2, ir3, ir4, e, e1, e2, e3;
//		tag_t item_tag, window, itemr_tag, bom_view, top_bomline, BVR_tag;
//		//ITEM_find_item("000030", &item_tag);
//		//ITEM_ask_latest_rev(item_tag, &itemr_tag);
//
//		ITEM_create_item("121245", "BOM", "Item", "", &item_tag, &itemr_tag);
//		AOM_save_with_extensions(item_tag);
//		AOM_save_with_extensions(itemr_tag);
//		BOM_create_window(&window);
//		PS_create_bom_view(NULLTAG, NULL, NULL, item_tag, &bom_view);
//		AOM_save_with_extensions(bom_view);
//		PS_create_bvr(bom_view, NULL, NULL, false, itemr_tag, &BVR_tag);
//		AOM_save_with_extensions(BVR_tag);
//		BOM_set_window_top_line(window, item_tag, itemr_tag, bom_view, &top_bomline);
//		ITEM_find_item("000031", &i1);
//		ITEM_ask_latest_rev(i1, &ir1);
//		ITEM_find_item("000032", &i2);
//		ITEM_ask_latest_rev(i2, &ir2);
//		ITEM_find_item("000033", &i3);
//		ITEM_ask_latest_rev(i3, &ir3);
//		ITEM_find_item("000034", &i4);
//		ITEM_ask_latest_rev(i4, &ir4);
//		BOM_line_add(top_bomline, i1, ir1, NULLTAG, &e);
//		AOM_save_with_extensions(e);
//		BOM_line_add(top_bomline, i2, ir2, NULLTAG, &e1);
//		AOM_save_with_extensions(e1);
//		BOM_line_add(top_bomline, i3, ir3, NULLTAG, &e2);
//		AOM_save_with_extensions(e2);
//		BOM_line_add(top_bomline, i4, ir4, NULLTAG, &e3);
//		AOM_save_with_extensions(e3);
//		BOM_save_window(window);
//		BOM_close_window(window);
//	}
//	return(0);
//}
////EMH_ask_error_text(iFail, &cError);
////printf("The error is %s", cError);
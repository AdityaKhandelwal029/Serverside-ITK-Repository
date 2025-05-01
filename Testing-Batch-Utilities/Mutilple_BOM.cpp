#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		tag_t item_t, itemr_t, window, bom_view, topb_line, item2_t, item2r_t, bla, bvr,item3_t,item3r_t,x;
		tag_t bom_view2, topb_line2, bvr2, window2,bla2;
		ITEM_create_item("000287", "abcd", "Item", "A", &item_t, &itemr_t);
		ITEM_create_item("000288", "efgh", "Item", "A", &item2_t, &item2r_t);
		ITEM_create_item("000289", "hijk", "Item", "A", &item3_t, &item3r_t);
		ITEM_save_item(item_t);
		ITEM_save_item(item2_t);
		ITEM_save_item(item3_t);
		BOM_create_window(&window);
		PS_create_bom_view(NULLTAG, NULL, NULL, item_t, &bom_view);
		AOM_save_with_extensions(bom_view);
		PS_create_bvr(bom_view, NULL, NULL, false, itemr_t, &bvr);
		AOM_save_with_extensions(bvr);
		BOM_set_window_top_line(window, item_t, itemr_t, bom_view, &topb_line);
		BOM_line_add(topb_line, item2_t, item2r_t, NULLTAG, &bla);
		AOM_save_with_extensions(bla);
		BOM_save_window(window);
		BOM_close_window(window);

		BOM_create_window(&window2);
		PS_create_bom_view(NULLTAG, NULL, NULL, item2_t, &bom_view2);
		AOM_save_with_extensions(bom_view2);
		PS_create_bvr(bom_view2, NULL, NULL, false, item2r_t, &bvr2);
		AOM_save_with_extensions(bvr2);
		BOM_set_window_top_line(window2, item2_t, item2r_t, bom_view2, &topb_line2);
		BOM_line_add(topb_line2, item3_t, item3r_t, NULLTAG, &bla2);
		AOM_save_with_extensions(bla2);
		BOM_save_window(window2);
		BOM_close_window(window2);
	}
	return(0);
}
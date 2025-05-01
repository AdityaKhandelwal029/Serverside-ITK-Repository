#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		tag_t item_t, itemr_t,window,bom_view,topb_line, item2_t, item2r_t,bla,bvr;
		ITEM_create_item("000879", "abcd", "Item", "A", &item_t, &itemr_t);
		ITEM_create_item("000880", "efgh", "Item", "A", &item2_t, &item2r_t);
		ITEM_save_item(item_t);
		ITEM_save_item(item2_t);
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
	}
	return(0);
}
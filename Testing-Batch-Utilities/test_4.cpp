#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		tag_t item_t, itemr_t,window_t,bomview_t,bvr_t,topbom_line,bla1,bla2,bla3;
		ITEM_create_item("012145", "Main_item_1", "Item", "A", &item_t, &itemr_t);
		ITEM_save_item(item_t);
		BOM_create_window(&window_t);
		PS_create_bom_view(NULLTAG, NULL, NULL, item_t, &bomview_t);
		AOM_save_with_extensions(bomview_t); 
		PS_create_bvr(bomview_t,NULL,NULL,false,itemr_t,&bvr_t);
		AOM_save_with_extensions(bvr_t);
		BOM_set_window_top_line(window_t, item_t, itemr_t, bomview_t,&topbom_line); 
		tag_t i1, ir1, i2, ir2;
		ITEM_create_item("012146", "i1", "Item", "A", &i1, &ir1);
		ITEM_save_item(i1);
		ITEM_create_item("012147", "i2", "Item", "A", &i2, &ir2);
		ITEM_save_item(i2);
		BOM_line_add(topbom_line, i1, ir1, NULLTAG, &bla1);
		AOM_save_with_extensions(bla1);
		BOM_line_add(topbom_line, i2, ir2, NULLTAG, &bla2);
		AOM_save_without_extensions(bla2); 
		tag_t i3, ir3;
		ITEM_create_item("012148 ", "i3", "Item", "A", &i3, &ir3);
		ITEM_save_item(i3);
		tag_t bomview1_t, bvr1_t, topbom1_t, window1_t;
		BOM_create_window(&window1_t);
		PS_create_bom_view(NULLTAG, NULL, NULL, i1, &bomview1_t);
		AOM_save_with_extensions(bomview1_t);
		PS_create_bvr(bomview1_t, NULL, NULL, false, ir1, &bvr1_t);
		AOM_save_with_extensions(bvr1_t);
		BOM_set_window_top_line(window1_t, i1, ir1, bomview1_t, &topbom1_t);
		BOM_line_add(topbom1_t, i3, ir3, NULLTAG,&bla3);
		AOM_save_with_extensions(bla3);
		BOM_save_window(window1_t);
		BOM_save_window(window_t);
	}
	return(0);
}
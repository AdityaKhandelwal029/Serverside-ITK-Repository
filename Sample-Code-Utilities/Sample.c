#include "Header.h"
int ITK_user_main(int argc, char *argv[])
{
	char *uname = NULL;
	char *pass = NULL;
	char *grp = NULL;
	int Status = ITK_ok;
	tag_t item_type_tag = NULLTAG;
	tag_t create_item_tag = NULLTAG;
	tag_t item_tag = NULLTAG;
	tag_t t_item = NULLTAG;
	tag_t t_itemrev = NULLTAG;
	tag_t twindow = NULLTAG;
	tag_t tBOMtopline = NULLTAG;
	tag_t t_itemrev1 = NULLTAG;
	tag_t trev_1 = NULLTAG;
	tag_t t_itemrev2 = NULLTAG;
	tag_t trev_2 = NULLTAG;
	tag_t t_itemrev3 = NULLTAG;
	tag_t trev_3 = NULLTAG;
	tag_t newline1 = NULLTAG;
	tag_t newline2 = NULLTAG;
	tag_t newline = NULLTAG;
	tag_t BOM_viewtag = NULLTAG;
	tag_t BVR_tag = NULLTAG;
	tag_t BOM_viewtag1 = NULLTAG;
	tag_t BVR_tag1 = NULLTAG;
	tag_t BOM_viewtag2 = NULLTAG;
	tag_t BVR_tag2 = NULLTAG;

	uname = ITK_ask_cli_argument("-u=");

	pass = ITK_ask_cli_argument("-p=");

	grp = ITK_ask_cli_argument("-g=");


Error_check(ITK_init_module(uname, pass, grp));
Error_check(TCTYPE_ask_type("Item", &item_type_tag));
Error_check(TCTYPE_construct_create_input(item_type_tag, &create_item_tag));
Error_check(AOM_set_value_string(create_item_tag, "object_name", "PUNCH_0103"));
Error_check(TCTYPE_create_object(create_item_tag, &item_tag));
Error_check(ITEM_save_item(item_tag));
Error_check(ITEM_ask_latest_rev(item_tag, &t_itemrev));
Error_check(BOM_create_window(&twindow));
Error_check(PS_create_bom_view(NULLTAG, NULL, NULL, item_tag, &BOM_viewtag));
Error_check(PS_create_bvr(BOM_viewtag, NULL, NULL, false, t_itemrev, &BVR_tag));
Error_check(BOM_set_window_top_line(twindow, t_item, t_itemrev, BOM_viewtag, &tBOMtopline));
Error_check(ITEM_find_item("000065", &trev_1));
Error_check(ITEM_ask_latest_rev(trev_1, &t_itemrev1));
Error_check(ITEM_find_item("000073", &trev_2));
Error_check(ITEM_ask_latest_rev(trev_2, &t_itemrev2));
Error_check(ITEM_find_item("000074", &trev_3));
Error_check(ITEM_ask_latest_rev(trev_3, &t_itemrev3));
Error_check(BOM_line_add(tBOMtopline, trev_1, t_itemrev1, NULLTAG, &newline));
Error_check(BOM_line_add(tBOMtopline, trev_2, t_itemrev2, NULLTAG, &newline1));
Error_check(BOM_line_add(tBOMtopline, trev_3, t_itemrev3, NULLTAG, &newline2));
Error_check(BOM_save_window(twindow)); 
Error_check(BOM_close_window(twindow));
return Status;
}









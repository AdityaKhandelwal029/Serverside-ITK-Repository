#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		char *uname = NULL;
		char *pass = NULL;
		char *grp = NULL;
		char* Error_msg = NULL;
		char* item_id = NULL;
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
		tag_t ITEMTag = NULLTAG;
		cout << (ITEM_find_item("000030", &ITEMTag)) << endl;
		cout << (ITEM_ask_latest_rev(ITEMTag, &t_itemrev)) << endl;
		cout << (BOM_create_window(&twindow)) << endl;
		cout << (PS_create_bom_view(NULLTAG, NULL, NULL, ITEMTag, &BOM_viewtag)) << endl;
		cout << (AOM_save_with_extensions(BOM_viewtag)) << endl;
		cout << (PS_create_bvr(BOM_viewtag, NULL, NULL, false, t_itemrev, &BVR_tag)) << endl;
		cout << (AOM_save_with_extensions(BVR_tag)) << endl;
		cout << (BOM_set_window_top_line(twindow, ITEMTag, t_itemrev, NULLTAG, &tBOMtopline)) << endl;
		cout << (ITEM_find_item("000031", &trev_1)) << endl;
		cout << (ITEM_ask_latest_rev(trev_1, &t_itemrev1)) << endl;
		cout << (ITEM_find_item("000032", &trev_2)) << endl;
		cout << (ITEM_ask_latest_rev(trev_2, &t_itemrev2)) << endl;
		cout << (ITEM_find_item("000033", &trev_3)) << endl;
		cout << (ITEM_ask_latest_rev(trev_3, &t_itemrev3)) << endl;
		cout << (BOM_line_add(tBOMtopline, trev_1, t_itemrev1, NULLTAG, &newline)) << endl;
		cout << (AOM_save_with_extensions(newline)) << endl;
		cout << (BOM_line_add(tBOMtopline, trev_2, t_itemrev2, NULLTAG, &newline1)) << endl;
		cout << (AOM_save_with_extensions(newline1)) << endl;
		cout << (BOM_line_add(tBOMtopline, trev_3, t_itemrev3, NULLTAG, &newline2)) << endl;
		cout << (AOM_save_with_extensions(newline2)) << endl;
		cout << (BOM_save_window(twindow)) << endl;
		cout << (BOM_close_window(twindow)) << endl;
	}
	return(0);
}
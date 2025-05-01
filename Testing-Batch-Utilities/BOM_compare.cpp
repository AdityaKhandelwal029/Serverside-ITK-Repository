#include"Header.h"

int ITK_user_main(int argc, char *argv[])
{
	int ifail;
	char* msg;
	tag_t tItem = NULLTAG;
	tag_t TRev = NULLTAG;
	tag_t window1 = NULLTAG;
	tag_t top_bom_line = NULLTAG;
	tag_t rule1 = NULLTAG;
	tag_t window2 = NULLTAG;
	tag_t top_bom_line2 = NULLTAG;
	tag_t rule2 = NULLTAG;
	int report_length = NULLTAG;
	char** report_lines = NULLTAG;
	tag_t* report_items = NULLTAG;
	tag_t TRevB;
	ifail = ITK_init_module("infodba", "infodba", "dba");
	ifail = ITEM_find_item("000142", &tItem);
	ifail = ITEM_find_revision(tItem, "A", &TRev);
	ifail = ITEM_find_revision(tItem, "B", &TRevB);
	ifail = BOM_create_window(&window1);
	ifail = BOM_set_window_top_line(window1, tItem, TRev, NULLTAG, &top_bom_line);
	ifail = CFM_find("Latest Working", &rule1);
	ifail = BOM_set_window_config_rule(window1, rule1);
	ifail = BOM_create_window(&window2);
	ifail = BOM_set_window_top_line(window2, tItem, TRevB, NULLTAG, &top_bom_line2);
	ifail = CFM_find("Latest Working", &rule2);
	ifail = BOM_set_window_config_rule(window2, rule2);
	ifail = BOM_compare_execute(NULLTAG, top_bom_line, top_bom_line2, "IMAN_bcm_ext_var_level", BOM_compare_output_report);
	ifail = BOM_compare_report(top_bom_line, &report_length, &report_lines, &report_items);
	for (int i = 0; i < report_length; i++)
	{
		printf("\n %s \n", report_lines[i]);
	}
	if (report_lines)
		MEM_free(report_lines);
	if (report_items)
		MEM_free(report_items);

	ifail = BOM_close_window(window1);
	ifail = BOM_close_window(window2);

	ifail = ITK_exit_module(TRUE);

	return ITK_ok;


}
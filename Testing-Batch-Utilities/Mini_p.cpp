#include"Header.h"
int ITK_user_main(int argc, char *argv[])
{
	int x = login();
	if (x == 1)
	{
		tag_t mitem_t, mitemr_t, mwindow_t, mtopline_t, mbvr_t, mbomview_t, mbla;
		ITEM_create_item("008980", "Main_Item", "Item", "A", &mitem_t, &mitemr_t);
		ITEM_save_item(mitem_t);
		BOM_create_window(&mwindow_t);
		PS_create_bom_view(NULLTAG, NULL, NULL, mitem_t, &mbomview_t);
		AOM_save_with_extensions(mbomview_t);
		PS_create_bvr(mbomview_t, NULL, NULL, false, mitemr_t, &mbvr_t);
		AOM_save_with_extensions(mbvr_t);
		BOM_set_window_top_line(mwindow_t, mitem_t, mitemr_t, mbomview_t, &mtopline_t);
		for (int i = 100; i <= 105; i++)
		{
			cout << " Inside loop " << endl;
			string a = "557", b;
			int x = i;
			while (x != 0)
			{
				int y;
				y = x % 10;
				b = b + char(48 + y);
				x = x / 10;
			}
			reverse(b.begin(), b.end());
			a = a + b;
			printf(" a = %s\n", a);
			const char *c = a.c_str();
			tag_t sitem_t, sitemr_t;
			ITEM_create_item(c, "subitem", "Item", "A", &sitem_t, &sitemr_t);
			ITEM_save_item(sitem_t);
			BOM_line_add(mtopline_t, sitem_t, sitemr_t, NULLTAG, &mbla);
			AOM_save_with_extensions(mbla);
			BOM_save_window(mwindow_t);
			tag_t swindow_t, sbomview_t, stopline_t, sbvr_t, sbla;
			BOM_create_window(&swindow_t);
			PS_create_bom_view(NULLTAG, NULL, NULL, sitem_t, &sbomview_t);
			AOM_save_with_extensions(sbomview_t);
			PS_create_bvr(sbomview_t, NULL, NULL, false, sitemr_t, &sbvr_t);
			AOM_save_with_extensions(sbvr_t);
			BOM_set_window_top_line(swindow_t, sitem_t, sitemr_t, sbomview_t, &stopline_t);
			for (int j = 1; j <= 5; j++)
			{
				string q = "76879";
				q = q + char(48 + j);
				printf(" b = %s\n", q);
				const char *cc = q.c_str();
				tag_t citem_t, citemr_t;
				ITEM_create_item(cc, "child_item", "Item", "A", &citem_t, &citemr_t);
				ITEM_save_item(citem_t);
				BOM_line_add(stopline_t, citem_t, citemr_t, NULLTAG, &sbla);
				AOM_save_with_extensions(sbla);
				BOM_save_window(swindow_t);
			}
		}
	}
	return(0);
}
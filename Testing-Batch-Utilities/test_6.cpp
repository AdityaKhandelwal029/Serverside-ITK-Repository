#include"Header.h"
int ITK_user_main(int argc, char *argv[])
{
	int x = login();
	if (x == 1)
	{
		int mq = 100;
		int ch, dept; 
		cout << endl;
		cout << " Please enter total number of child's = ";
		cin >> ch;
		cout << " Please enter to depth = ";
		cin >> dept;
		ch = ch + 100;
		tag_t mitem_t, mitemr_t, mwindow_t, mtopline_t, mbvr_t, mbomview_t, mbla;
		ITEM_create_item("210987", "Main_Item", "Item", "A", &mitem_t, &mitemr_t);
		ITEM_save_item(mitem_t);
		BOM_create_window(&mwindow_t);
		PS_create_bom_view(NULLTAG, NULL, NULL, mitem_t, &mbomview_t);
		AOM_save_with_extensions(mbomview_t);
		PS_create_bvr(mbomview_t, NULL, NULL, false, mitemr_t, &mbvr_t);
		AOM_save_with_extensions(mbvr_t);
		BOM_set_window_top_line(mwindow_t, mitem_t, mitemr_t, mbomview_t, &mtopline_t);
		for (int i = 100; i <= ch; i++)
		{
			cout << " Inside loop " << endl;
			string a = "909", b;
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
            
			vector<tag_t> aitem_t, aitemr_t, awindow_t, atopline_t, abvr_t,abomview_t;
			for (int j = 1; j <= dept; j++)
			{
				string q = "952", r;
				int x = mq;
				while (x != 0)
				{
					int l = x % 10;
					r = r + char(48 + l);
					x = x / 10;
				}
				reverse(r.begin(), r.end());
				q = q + r;
				printf(" b = %s\n", q);
				const char *cc = q.c_str();
				tag_t hitem_t, hitemr_t, hwindow_t, htopline_t, hbvr_t, hbla,hbomview_t;
				ITEM_create_item(cc, "child_item", "Item", "A",&hitem_t,&hitemr_t);
				ITEM_save_item(hitem_t);
				BOM_create_window(&hwindow_t);
				PS_create_bom_view(NULLTAG, NULL, NULL, hitem_t, &hbomview_t);
				AOM_save_with_extensions(hbomview_t);
				PS_create_bvr(hbomview_t, NULL, NULL, false, hitemr_t, &hbvr_t);
				AOM_save_with_extensions(hbvr_t);
				BOM_set_window_top_line(hwindow_t, hitem_t, hitemr_t, hbomview_t, &htopline_t);

				aitem_t.push_back(hitem_t);
				aitemr_t.push_back(hitemr_t);
				awindow_t.push_back(hwindow_t);
				abomview_t.push_back(hbomview_t);
				abvr_t.push_back(hbvr_t);
				atopline_t.push_back(htopline_t);
				mq++;
			}
			for (int k = 0; k < dept; k++)
			{
				tag_t abla;
				if (k == 0)
				{
					BOM_line_add(stopline_t, aitem_t[k], aitemr_t[k], NULLTAG, &abla);
					AOM_save_with_extensions(abla);
					BOM_save_window(swindow_t);
				}
				else
				{
					BOM_line_add(atopline_t[k-1], aitem_t[k], aitemr_t[k], NULLTAG, &abla);
					AOM_save_with_extensions(abla);
					BOM_save_window(awindow_t[k-1]);
				}
			}
		}
	}
	return(0);
}
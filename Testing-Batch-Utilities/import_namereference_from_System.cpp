#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		int r_count;
		tag_t item_tag;
		tag_t *itemr_list;
		ITEM_find_item("000137", &item_tag);
		ITEM_list_all_revs(item_tag, &r_count, &itemr_list);
		cout << " Total revision found = " << r_count<<endl;
		for (int i = 0; i < r_count; i++)
		{
			char *rev_name;
			int secobj_count;
			tag_t rel_type,*sec_obj;
			ITEM_ask_rev_name2(itemr_list[i], &rev_name);
			cout << " Item_revision name = " << rev_name<<endl;
			GRM_find_relation_type("IMAN_specification", &rel_type);
			GRM_list_secondary_objects_only(itemr_list[i],rel_type,&secobj_count,&sec_obj);
			for (int j = 0; j < secobj_count; j++)
			{
				char *sec_obj_name;
				AOM_ask_name(sec_obj[j],&sec_obj_name);
				cout << " secondary object name = " << sec_obj_name << endl;
				AOM_refresh(sec_obj[j], TRUE);
				AE_import_named_ref(sec_obj[j], "Text", "D:\\sample_data_set\\a.txt", "", SS_TEXT);
				AOM_save(sec_obj[j]);
				AOM_refresh(sec_obj[j],FALSE);
			}
			MEM_free(sec_obj);
		}
	}
	return(0);
}


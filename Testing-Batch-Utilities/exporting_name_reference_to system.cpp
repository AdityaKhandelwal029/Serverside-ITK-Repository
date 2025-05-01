#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		int itemr_count,secobj_count;
		tag_t *itemr_list,*secobj_list;
		tag_t item_tag,relation_type;
		ITEM_find_item("000139",&item_tag);
		ITEM_list_all_revs(item_tag, &itemr_count, &itemr_list);
		for (int i = 0; i < itemr_count; i++)
		{
			char *itemr_name;
			AOM_ask_name(itemr_list[i], &itemr_name);
			cout << " Name of itemrevision = " << itemr_name << endl;
			GRM_find_relation_type("IMAN_specification", &relation_type);
			GRM_list_secondary_objects_only(itemr_list[i], relation_type, &secobj_count,&secobj_list);
			cout << " Secondary obj count  = " << secobj_count<<endl;
			for (int j = 0; j < secobj_count; j++)
			{
				AE_export_named_ref(secobj_list[j], "Text", "D:\\sample_data_set\\caq.txt");
			}
		}
	}
	return(0);
}


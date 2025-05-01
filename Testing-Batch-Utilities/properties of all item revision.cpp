#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		int count;
		tag_t item_tag, itemr_tag;
		tag_t *itemr_list;
		ITEM_find_item("000136", &item_tag);
		ITEM_list_all_revs(item_tag, &count, &itemr_list);
		for (int i = 0; i < count; i++)
		{
			char *itemr_name;
			int prop_count;
			char **prop_name;
			AOM_ask_name(itemr_list[i], &itemr_name);
			cout << " **************Itemr name = " << itemr_name << endl;
			AOM_ask_prop_names(itemr_list[i], &prop_count, &prop_name);
			cout << " Total number of properties = " << prop_count;
			for (int j = 0; j < prop_count; j++)
			{
				char *propd_name,*propd_value;
				AOM_UIF_ask_name(itemr_list[i], prop_name[j], &propd_name);
				cout << " property display _name = " << propd_name ;
				AOM_UIF_ask_value(itemr_list[i], prop_name[j], &propd_value);
				cout << " property display value = " << propd_value<<endl;
			}
		}
	}
	return(0);
}


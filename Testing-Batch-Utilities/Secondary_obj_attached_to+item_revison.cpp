#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		int count;
		tag_t item_tag, itemr_tag;
		tag_t *rev_list;
		ITEM_find_item("000136", &item_tag);
		ITEM_list_all_revs(item_tag, &count, &rev_list);
		cout << " Total revisons  = " << count << endl;
		for (int i = 0; i < count; i++)
		{
			char *rev_name;
			int secondaryobj_count;
			tag_t *secondaryobj_list;
			AOM_ask_name(rev_list[i], &rev_name);
			cout << " Item revision name = " << rev_name << endl;
			GRM_list_secondary_objects_only(rev_list[i], NULLTAG, &secondaryobj_count, &secondaryobj_list);
			for (int j = 0; j < secondaryobj_count; j++)
			{
				char *secobj_name;
				AOM_ask_name(secondaryobj_list[j], &secobj_name);
				cout << "          Secondary obj name = " << secobj_name << endl;
			}
		}
	}
	return(0);
}
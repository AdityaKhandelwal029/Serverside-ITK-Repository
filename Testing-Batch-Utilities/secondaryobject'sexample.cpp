#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		int count;
		tag_t item_t,*secondaryobj_list;
		ITEM_find_item("000243", &item_t); 
		GRM_list_secondary_objects_only(item_t, NULLTAG, &count, &secondaryobj_list);
		cout << " Count of object = " << count << endl;
		for (int i = 0; i < count; i++)
		{
			char *dvalue;
			AOM_UIF_ask_value(secondaryobj_list[i], "object_name", &dvalue);
			cout << " i = " << i + 1 << " Name = " << dvalue << endl;
		}
	}
	return(0);
}
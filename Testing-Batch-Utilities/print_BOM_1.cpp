#include"Header.h"
int ITK_user_main(int argc, char*argv[])
{
	int c = login();
	if (c == 1)
	{
		cout << " Login successfull " << endl;
		int count;
		tag_t item_tag,*itemr_list;
		ITEM_find_item("000029",&item_tag);
		ITEM_list_all_revs(item_tag, &count, &itemr_list);
		cout << " Total number of items = " << count<<endl;
		for (int i = 0; i < count; i++)
		{
			char *item_id, *obj_name;
			AOM_UIF_ask_value(itemr_list[i],"item_id",&item_id);
			AOM_UIF_ask_value(itemr_list[i], "object_name", &obj_name);
			cout << " object id = " << item_id;
			cout << " object name = " << obj_name << endl;
		}
	}
	return(0);
}
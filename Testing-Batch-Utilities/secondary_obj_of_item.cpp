#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		int count;
		char *obj_name;
		tag_t *itemobj_list;
		tag_t item_tag,relataiontype_tag;
		ITEM_find_item("000147", &item_tag);
		GRM_find_relation_type("*", &relataiontype_tag);
		GRM_list_secondary_objects_only(item_tag, relataiontype_tag,&count, &itemobj_list);
		cout << " count = " << count;
		for (int i = 0; i < count; i++)
		{
			AOM_UIF_ask_value(itemobj_list[i],"object_name", &obj_name);
			cout << " obj name = " << obj_name << endl;
		}
	}
	return(0);
}


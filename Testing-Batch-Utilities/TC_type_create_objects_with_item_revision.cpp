#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		int c = 1;
		tag_t itemrevtype_tag, create_tag, object_tag, itemtag;
		ITEM_find_item("002096", &itemtag);
		TCTYPE_find_type("ItemRevision", "ItemRevision", &itemrevtype_tag);
		TCTYPE_construct_create_input(itemrevtype_tag, &create_tag);
		AOM_UIF_set_value(create_tag, "object_name", "TCTYPEITEMREV1");
		AOM_set_value_tag(create_tag, "items_tag", itemtag);
		c=TCTYPE_create_object(create_tag, &object_tag);
		AOM_save(object_tag);
	
		if (c == 0)
		{
			cout << " ItemRev created successfully " << endl;
		}
		else
		{
			cout << " ItemRev creation failed " << endl;
		}
	}
	return(0);
}
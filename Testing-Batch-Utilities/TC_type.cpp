#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		int c = 1;
		tag_t itemtype_tag, create_tag, object_tag;
		cout<<TCTYPE_find_type("ItemRevision", "ItemRevision", &itemtype_tag)<<endl;
		cout<<TCTYPE_construct_create_input(itemtype_tag, &create_tag)<<endl;
		cout<<AOM_UIF_set_value(create_tag, "object_name", "CUSTOM-DOCUMENT-TEST")<<endl;
		c = TCTYPE_create_object(create_tag, &object_tag);
		cout<<AOM_save(object_tag)<<endl; 
		cout << " c  = " << c << endl;
		if (c == 0)
		{
			cout << " Item created successfully " << endl;
			ITK_set_bypass(TRUE);
			tag_t item_t;
			cout << ITEM_find_item("000461", &item_t) << endl;
			cout << AOM_refresh(item_t, TRUE) << endl;
			cout << AOM_set_value_tag(item_t, "a5Tag_property", object_tag) << endl;
			cout << AOM_save(item_t) << endl;
			cout << AOM_refresh(item_t, FALSE) << endl;
			ITK_set_bypass(FALSE);
		}
		else
		{
			cout << " Item creation failed " << endl;
		}
	}
	return(0);
}
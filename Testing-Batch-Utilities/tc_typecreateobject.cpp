#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		int c = 1;
		tag_t itemtype_tag,create_tag,object_tag;
		TCTYPE_find_type("Document", "Item", &itemtype_tag); 
		TCTYPE_construct_create_input(itemtype_tag,&create_tag); 
		AOM_UIF_set_value(create_tag, "object_name", "TCTYPEITEM100");
		c=TCTYPE_create_object(create_tag, &object_tag); 
		AOM_save(object_tag); 
		if (c == 0)
		{
			cout << " Item created successfully " << endl;
		}
		else
		{
			cout << " Item creation failed " << endl;
		}
	}
	return(0);
}
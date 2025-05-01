#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login()
	if (x == 1)
	{
		int out;
		tag_t *objects_created;
		int x[5] = { 1,1,1,1,1 };
		tag_t y[5];
		tag_t item_t,create_input;
		TCTYPE_find_type("Item", "Item", &item_t);  
		for (int i = 0; i < 5; i++)
		{
			TCTYPE_construct_create_input(item_t, &create_input); 
			AOM_UIF_set_value(create_input, "object_name","ITEMTC" ); 
			y[i] = create_input;
		}  
		TCTYPE_create_objects(5, &x[0], &y[0], &out, &objects_created); 
		cout << " Objects reated count = " << out << endl; 
	}
	return(0);
}
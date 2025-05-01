#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		char *class_name;
		tag_t item_t,class_id,rev_t;
		ITEM_find_item("000187", &item_t); 
		ITEM_ask_latest_rev(item_t, &rev_t);
		POM_class_of_instance(rev_t,&class_id); 
		POM_name_of_class(class_id, &class_name);
		cout << " class name = " << class_name;
	}
	return(0);
}
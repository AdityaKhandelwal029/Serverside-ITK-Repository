//Item copy rev - Revise operation
//Item create from rev - save as operation 

#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		tag_t item_t;
		tag_t lst_item_rev_t; 
		tag_t new_rev_t;
		ITEM_find_item("002095",&item_t); 
		ITEM_ask_latest_rev(item_t, &lst_item_rev_t);
		ITEM_copy_rev(lst_item_rev_t, NULL, &new_rev_t);  
		AOM_save_with_extensions(new_rev_t);
	}
	return(0);
}


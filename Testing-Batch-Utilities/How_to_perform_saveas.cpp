#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		tag_t item_t; 
		tag_t lstitemrev_t;
		tag_t newitem_t;
		tag_t newitemrev_t;
		cout<<ITEM_find_item("002096", &item_t)<<endl;
		cout<<ITEM_ask_latest_rev(item_t, &lstitemrev_t)<<endl; 
		// New item is created with the help of this api. but that newly created item is not attached in the newstuff folder of the user
		cout<<ITEM_copy_item(lstitemrev_t, "NEWIT-1", "NEWIT-1", &newitem_t, &newitemrev_t)<<endl;
		cout << ITEM_save_item(newitem_t) << endl;
	}
	return(0);
}

#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{

		tag_t user_tag, home_folder,item_tag,itemr_tag;
		SA_find_user2("installuser", &user_tag);
		SA_ask_user_home_folder(user_tag, &home_folder);
		ITEM_create_item("001111", "item1000", "Document", "A", &item_tag, &itemr_tag);
		AOM_save(item_tag);//saves the create object
		FL_insert(home_folder, itemr_tag, 20);
		AOM_save(home_folder);
		return(0);
	}
}
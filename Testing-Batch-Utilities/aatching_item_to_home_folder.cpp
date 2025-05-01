#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		tag_t user_tag, home_folder, item_tag, itemr_tag;
		SA_find_user2("infodba", &user_tag);
		SA_ask_user_home_folder(user_tag, &home_folder);
		ITEM_create_item("001988", "sample117", "Item", "A", &item_tag, &itemr_tag);
		AOM_save(item_tag);
		FL_insert(home_folder, item_tag,31);
		AOM_save(home_folder);
	}
	return(0);
}
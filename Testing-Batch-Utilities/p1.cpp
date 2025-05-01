#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		tag_t item_tag, itemr_tag,user_tag,homef_tag;
		ITEM_create_item("000045","test_item_10","Item","A",&item_tag,&itemr_tag);
		ITEM_save_item(item_tag);
		SA_find_user2("infodba", &user_tag);
		SA_ask_user_home_folder(user_tag, &homef_tag);
		FL_insert(homef_tag, item_tag, 10);
		AOM_save_without_extensions(homef_tag);
	}
	return(0);
}
